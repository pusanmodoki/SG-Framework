/*----------------------------------------------------------------------------------
3DSE������AudioAssetEffect3D class
	->AudioAssetEffect3D.cpp
------------------------------------------------------------------------------------*/
#include "AudioAssetEffect3D.hpp"

// Framework namespace
namespace SGFramework
{
	//Asset�֘A���i�[����Asset namespace
	namespace Asset
	{
		//----------------------------------------------------------------------------------
		//[Open]
		//�A�Z�b�g�̃��[�h���s��
		//����1: ��������������SubmixLayer, default = monoral root layer
		//����2: �\�[�X�̐� (�ő哯���Đ���)
		//����3: �Đ��Ԋu
		//����4: �X���[�Y�ɕ�������͈�
		//����5: ����5�̊p�x
		//����6: ���ʃJ�[�u�����̃X�P�[���[ 
		//����7: �h�b�v���[���ʂ̃X�P�[���[
		//����8: �Đ��J�n�b��, default = cDefault (Begin)
		//����9: ���[�v���J�n�b��, default = cDefault (Begin)
		//����10: �Đ��I���b��, default = cDefault (End)
		void AudioAssetEffect3D::Open(WeakPointer<Audio::SubmixLayer> submixLayer,
			uint amountOfSource, float playbackInterval, float innerRadius, float innerRadiusAngle, float curveDistanceScaler, 
			float dopplerScaler, float setPlayBeginSeconds, float setLoopBeginSeconds, float setPlayEndSeconds)
		{
			using namespace Detail::Audio;

			if (IS_TRUE(getIsFileOpened())) return;

			try
			{
				//Load
				LoadFile(submixLayer, amountOfSource, playbackInterval,
					setPlayBeginSeconds, setLoopBeginSeconds, setPlayEndSeconds);

				//new
				XAUDIO2_VOICE_DETAILS detalis = {};
				Audio::m_masteringVoice->GetVoiceDetails(&detalis);
				m_x3dBuffer.Init(new float[detalis.InputChannels]);

				//�G�~�b�^�[���ݒ�
				m_emitter.ChannelCount = m_channels;
				m_emitter.ChannelRadius = 0.0f;
				m_emitter.CurveDistanceScaler = curveDistanceScaler;
				m_emitter.DopplerScaler = dopplerScaler;
				m_emitter.InnerRadius = innerRadius;
				m_emitter.InnerRadiusAngle = innerRadiusAngle;
				m_emitter.OrientFront = Const::Vector3::zero;
				m_emitter.OrientTop = Const::Vector3::zero;
				m_emitter.pChannelAzimuths = nullptr;
				m_emitter.pCone = nullptr;
				m_emitter.pLFECurve = nullptr;
				m_emitter.pLPFDirectCurve = nullptr;
				m_emitter.pLPFReverbCurve = nullptr;
				m_emitter.Position = Const::Vector3::zero;
				m_emitter.pReverbCurve = nullptr;
				m_emitter.pVolumeCurve = nullptr;
				m_emitterBuffer = m_emitter;

				//BaseAsset
				OpenBaseAsset();	


				//Message�쐬
				AudioMessage message(MessageType::Effect3DFlag | MessageType::AddEffect3D);
				message.attachment.addEffect3D.asset = m_thisPointer.DynamicCast<AudioAssetEffect3D>();
				message.attachment.addEffect3D.instanceID = instanceID();
				//Message���M
				Audio::SendAudioMessage(message);
			}
			catch (...) { throw; }
		}

		//----------------------------------------------------------------------------------
		//[Reload]
		//�A�Z�b�g�̃����[�h���s��
		void AudioAssetEffect3D::Reload()
		{
			if (IS_FALSE(getIsFileOpened())) return;
			
			//Open�ɕK�v�ȏ����L�^
			sgstring path = name;
			const auto& submixLayer = Audio::getLayer(m_submixLayerID);
			uint setAmountOfSource = m_amountOfSource;
			float setPlaybackInterval = m_playbackInterval;
			float setPlayBeginSeconds = m_playBeginSeconds;
			float setLoopBeginSeconds = m_loopBeginSeconds;
			float setPlayEndSeconds = m_playEndSeconds;
			
			//�G�~�b�^�[�ۑ�
			AtomicOperation::LockAtomic(m_lock);
			auto emitterBuffer = m_emitterBuffer;
			AtomicOperation::UnlockAtomic(m_lock);

			//Release
			Close();

			//Open
			Open(submixLayer, setAmountOfSource, m_playbackInterval,
				0.0f, 0.0f, 0.0f, 0.0f,
				setPlayBeginSeconds, setLoopBeginSeconds, setPlayEndSeconds);

			//�G�~�b�^�[�X�V
			AtomicOperation::LockAtomic(m_lock);
			m_emitter = emitterBuffer;
			m_emitterBuffer = emitterBuffer;
			AtomicOperation::UnlockAtomic(m_lock);
		}

		//----------------------------------------------------------------------------------
		//[Close]
		//�t�@�C�������
		void AudioAssetEffect3D::Close()
		{
			if (IS_FALSE(getIsFileOpened())) return;
		
			//Release
			ReleaseVoice();
			//BaseAsset
			CloseBaseAsset(getType(), false);
		}

		//----------------------------------------------------------------------------------
		//[PlayExecution]
		//���ۂɍĐ����s��
		void AudioAssetEffect3D::PlayExecution(const Vector3 & position, const Vector3 & velocityPerSec,
			const Vector3 & forward, const Vector3 & up, uint loopCount)
		{
			//�Đ��Ԋu�ɖ����Ȃ��ꍇ�Đ������I��
			if (Time::time - m_playElapased < m_playbackInterval) return;
			m_playElapased = Time::time;

			//X3DAudio�p
			X3DAUDIO_DSP_SETTINGS dsp = {};
			XAUDIO2_FILTER_PARAMETERS filterParameters = {};
			//�Đ������t���O
			bool isPlayCompleted = false;

			//DSP�ݒ�
			dsp.DstChannelCount = Audio::m_masterChannels;
			dsp.SrcChannelCount = m_channels;
			dsp.pMatrixCoefficients = m_x3dBuffer;
			//�G�~�b�^�[�ݒ�
			m_emitter.Position = position;
			m_emitter.OrientFront = forward;
			m_emitter.OrientTop = up;
			m_emitter.Velocity = velocityPerSec;

			//useVoice == -1 -> ���C���{�C�X
			if (m_useVoice == -1)
			{
				//GetState
				m_voice->GetState(&m_state);
				//�Đ����Ă��Ȃ����
				if (m_state.BuffersQueued == 0)
				{
					//X3DAudio�v�Z
					X3DAudioCalculate(Audio::m_x3dHandle, &Audio::m_x3dListener, &m_emitter, m_flags, &dsp);
					//�t�B���^�ݒ�
					filterParameters.Type = LowPassFilter;
					filterParameters.Frequency = 2.0f * sinf(MathAF::cPi / 6.0f * dsp.LPFDirectCoefficient);
					filterParameters.OneOverQ = 1.0f;

					//Voice�Ɍv�Z���ʂ�ݒ�
					m_voice->SetOutputMatrix(Audio::m_masteringVoice, m_channels, Audio::m_masterChannels, dsp.pMatrixCoefficients);
					m_voice->SetFrequencyRatio(dsp.DopplerFactor);
					m_voice->SetOutputMatrix(Audio::getLayer(m_submixLayerID)->m_voice, 1, 1, &dsp.ReverbLevel);
					m_voice->SetFilterParameters(&filterParameters);

					//���[�v�J�E���g���
					m_bufferInfo.LoopCount = loopCount;
					//�Đ�
					m_voice->FlushSourceBuffers();
					m_voice->SubmitSourceBuffer(&m_bufferInfo);
					m_voice->Start();
					isPlayCompleted = true;
				}
			}
			//�T�u�{�C�X
			else
			{
				//GetState
				m_subVoices[m_useVoice]->GetState(&m_state);
				//�Đ����Ă��Ȃ����
				if (m_state.BuffersQueued == 0)
				{
					//X3DAudio�v�Z
					X3DAudioCalculate(Audio::m_x3dHandle, &Audio::m_x3dListener, &m_emitter, m_flags, &dsp);
					//�t�B���^�ݒ�
					filterParameters.Type = LowPassFilter;
					filterParameters.Frequency = 2.0f * sinf(X3DAUDIO_PI / 6.0f * dsp.LPFDirectCoefficient);
					filterParameters.OneOverQ = 1.0f;

					//Voice�Ɍv�Z���ʂ�ݒ�
					m_subVoices[m_useVoice]->SetOutputMatrix(Audio::m_masteringVoice, m_channels, Audio::m_masterChannels, dsp.pMatrixCoefficients);
					m_subVoices[m_useVoice]->SetFrequencyRatio(dsp.DopplerFactor);
					m_subVoices[m_useVoice]->SetOutputMatrix(Audio::getLayer(m_submixLayerID)->m_voice, m_channels, 1, &dsp.ReverbLevel);
					m_subVoices[m_useVoice]->SetFilterParameters(&filterParameters);

					//���[�v�J�E���g���
					m_bufferInfo.LoopCount = loopCount;
					//�Đ�
					m_subVoices[m_useVoice]->FlushSourceBuffers();
					m_subVoices[m_useVoice]->SubmitSourceBuffer(&m_bufferInfo);
					m_subVoices[m_useVoice]->Start();
					isPlayCompleted = true;
				}
			}

			//�Đ��ł���
			if (isPlayCompleted)
			{
				//�Đ��Ԋu������
				m_playElapased = 0.0f;
				//�J�E���^�[���Z
				++m_useVoice;
				//���ߐݒ�
				if (m_useVoice >= m_subVoices.size())
					m_useVoice = -1;
			}
		}

		//----------------------------------------------------------------------------------
		//[StopExecution]
		//���ۂɒ�~���s��
		void AudioAssetEffect3D::StopExecution()
		{
			//��~��������
			m_voice->Stop();
			m_voice->FlushSourceBuffers();
			
			//�T�u�{�C�X����~
			for (auto& e : m_subVoices)
			{
				e->Stop();
				e->FlushSourceBuffers();
			}
		}

		//----------------------------------------------------------------------------------
		//[ReleaseVoice]
		//�������J������
		void AudioAssetEffect3D::ReleaseVoice()
		{
			using namespace Detail::Audio;
			
			if (m_voice == nullptr) return;

			if (Audio::m_masteringVoice == nullptr)
				throw InvalidArgument(L"Error!! AudioAssetEffect3D->ReleaseVoice", L"Master voice was destroyed");

			AtomicOperation::LockAtomic(m_lock);

			//�����l�ɂ��ׂď�����
			ZeroMemory(&m_emitter, sizeof(m_emitter));
			ZeroMemory(&m_emitterBuffer, sizeof(m_emitterBuffer));
			m_flags = m_cDefaultFlags;
			m_flagsBuffer = m_cDefaultFlags;
		
			ReleaseBaseVoice();

			AtomicOperation::UnlockAtomic(m_lock);


			//Message�쐬
			AudioMessage message(MessageType::Effect3DFlag | MessageType::RemoveEffect3D);
			message.attachment.onlyInstanceID = instanceID();
			//Message���M
			Audio::SendAudioMessage(message);
		}
	}
}