/*----------------------------------------------------------------------------------
�ʏ�SE������AudioAssetEffect class
	->AudioAssetEffect.cpp
------------------------------------------------------------------------------------*/
#include "AudioAssetEffect.hpp"

// Framework namespace
namespace SGFramework
{
	//Asset�֘A���i�[����Asset namespace
	namespace Asset
	{
		//----------------------------------------------------------------------------------
		//[Open]
		//�A�Z�b�g�̃��[�h���s��
		//����1: ��������������SubmixLayer, default = not monoral root layer
		//����2: �\�[�X�̐� (�ő哯���Đ���)
		//����3: �Đ��Ԋu
		//����4: �Đ��J�n�b��, default = cDefault (Begin)
		//����5: ���[�v���J�n�b��, default = cDefault (Begin)
		//����6: �Đ��I���b��, default = cDefault (End)
		void AudioAssetEffect::Open(WeakPointer<Audio::SubmixLayer> submixLayer,
			uint amountOfSource, float playbackInterval, float setPlayBeginSeconds,
			float setLoopBeginSeconds, float setPlayEndSeconds)
		{
			using namespace Detail::Audio;
		
			if (IS_TRUE(getIsFileOpened())) return;

			try
			{
				//Load
				LoadFile(submixLayer, amountOfSource, playbackInterval,
					setPlayBeginSeconds, setLoopBeginSeconds, setPlayEndSeconds);

				//BaseAsset
				OpenBaseAsset();
			
				//Message�쐬
				AudioMessage message(MessageType::EffectFlag | MessageType::AddEffect);
				message.attachment.addEffect.asset = m_thisPointer.DynamicCast<AudioAssetEffect>();
				message.attachment.addEffect.instanceID = instanceID();
				//Message���M
				Audio::SendAudioMessage(message);
			}
			catch (...) { throw; }
		}

		//----------------------------------------------------------------------------------
		//[Reload]
		//�A�Z�b�g�̃����[�h���s��
		void AudioAssetEffect::Reload()
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

			//Release
			Close();

			//Open
			Open(submixLayer, setAmountOfSource, m_playbackInterval, 
				setPlayBeginSeconds, setLoopBeginSeconds, setPlayEndSeconds);
			}

		//----------------------------------------------------------------------------------
		//[Close]
		//�t�@�C�������
		void AudioAssetEffect::Close()
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
		void AudioAssetEffect::PlayExecution(uint loopCount)
		{
			//�Đ��Ԋu�ɖ����Ȃ��ꍇ�Đ������I��
			if (Time::time - m_playElapased < m_playbackInterval) return;
			m_playElapased = Time::time;

			//�Đ������t���O
			bool isPlayCompleted = false;

			//useVoice == -1 -> ���C���{�C�X
			if (m_useVoice == -1)
			{
				//GetState
				m_voice->GetState(&m_state);
				//�Đ����Ă��Ȃ����
				if (m_state.BuffersQueued == 0)
				{
					//���[�v�J�E���g���
					m_bufferInfo.LoopCount = loopCount;
					//�Đ�
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
					//���[�v�J�E���g���
					m_bufferInfo.LoopCount = loopCount;
					//�Đ�
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
		void AudioAssetEffect::StopExecution()
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
		void AudioAssetEffect::ReleaseVoice()
		{
			using namespace Detail::Audio;
			
			if (m_voice == nullptr) return;

			if (Audio::m_masteringVoice == nullptr)
				throw InvalidArgument(L"Error!! AudioAssetEffect->ReleaseVoice", L"Master voice was destroyed");

			AtomicOperation::LockAtomic(m_lock);

			ReleaseBaseVoice();

			AtomicOperation::UnlockAtomic(m_lock);

			//Message�쐬
			AudioMessage message(MessageType::EffectFlag | MessageType::RemoveEffect);
			message.attachment.onlyInstanceID = instanceID();
			//Message���M
			Audio::SendAudioMessage(message);
		}
	}
}