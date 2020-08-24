/*----------------------------------------------------------------------------------
3DSEを扱うAudioAssetEffect3D class
	->AudioAssetEffect3D.cpp
------------------------------------------------------------------------------------*/
#include "AudioAssetEffect3D.hpp"

// Framework namespace
namespace SGFramework
{
	//Asset関連を格納したAsset namespace
	namespace Asset
	{
		//----------------------------------------------------------------------------------
		//[Open]
		//アセットのロードを行う
		//引数1: 音声が所属するSubmixLayer, default = monoral root layer
		//引数2: ソースの数 (最大同時再生数)
		//引数3: 再生間隔
		//引数4: スムーズに聞こえる範囲
		//引数5: 引数5の角度
		//引数6: 音量カーブ距離のスケーラー 
		//引数7: ドップラー効果のスケーラー
		//引数8: 再生開始秒数, default = cDefault (Begin)
		//引数9: ループ時開始秒数, default = cDefault (Begin)
		//引数10: 再生終了秒数, default = cDefault (End)
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

				//エミッター情報設定
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


				//Message作成
				AudioMessage message(MessageType::Effect3DFlag | MessageType::AddEffect3D);
				message.attachment.addEffect3D.asset = m_thisPointer.DynamicCast<AudioAssetEffect3D>();
				message.attachment.addEffect3D.instanceID = instanceID();
				//Message送信
				Audio::SendAudioMessage(message);
			}
			catch (...) { throw; }
		}

		//----------------------------------------------------------------------------------
		//[Reload]
		//アセットのリロードを行う
		void AudioAssetEffect3D::Reload()
		{
			if (IS_FALSE(getIsFileOpened())) return;
			
			//Openに必要な情報を記録
			sgstring path = name;
			const auto& submixLayer = Audio::getLayer(m_submixLayerID);
			uint setAmountOfSource = m_amountOfSource;
			float setPlaybackInterval = m_playbackInterval;
			float setPlayBeginSeconds = m_playBeginSeconds;
			float setLoopBeginSeconds = m_loopBeginSeconds;
			float setPlayEndSeconds = m_playEndSeconds;
			
			//エミッター保存
			AtomicOperation::LockAtomic(m_lock);
			auto emitterBuffer = m_emitterBuffer;
			AtomicOperation::UnlockAtomic(m_lock);

			//Release
			Close();

			//Open
			Open(submixLayer, setAmountOfSource, m_playbackInterval,
				0.0f, 0.0f, 0.0f, 0.0f,
				setPlayBeginSeconds, setLoopBeginSeconds, setPlayEndSeconds);

			//エミッター更新
			AtomicOperation::LockAtomic(m_lock);
			m_emitter = emitterBuffer;
			m_emitterBuffer = emitterBuffer;
			AtomicOperation::UnlockAtomic(m_lock);
		}

		//----------------------------------------------------------------------------------
		//[Close]
		//ファイルを閉じる
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
		//実際に再生を行う
		void AudioAssetEffect3D::PlayExecution(const Vector3 & position, const Vector3 & velocityPerSec,
			const Vector3 & forward, const Vector3 & up, uint loopCount)
		{
			//再生間隔に満たない場合再生せず終了
			if (Time::time - m_playElapased < m_playbackInterval) return;
			m_playElapased = Time::time;

			//X3DAudio用
			X3DAUDIO_DSP_SETTINGS dsp = {};
			XAUDIO2_FILTER_PARAMETERS filterParameters = {};
			//再生完了フラグ
			bool isPlayCompleted = false;

			//DSP設定
			dsp.DstChannelCount = Audio::m_masterChannels;
			dsp.SrcChannelCount = m_channels;
			dsp.pMatrixCoefficients = m_x3dBuffer;
			//エミッター設定
			m_emitter.Position = position;
			m_emitter.OrientFront = forward;
			m_emitter.OrientTop = up;
			m_emitter.Velocity = velocityPerSec;

			//useVoice == -1 -> メインボイス
			if (m_useVoice == -1)
			{
				//GetState
				m_voice->GetState(&m_state);
				//再生していなければ
				if (m_state.BuffersQueued == 0)
				{
					//X3DAudio計算
					X3DAudioCalculate(Audio::m_x3dHandle, &Audio::m_x3dListener, &m_emitter, m_flags, &dsp);
					//フィルタ設定
					filterParameters.Type = LowPassFilter;
					filterParameters.Frequency = 2.0f * sinf(MathAF::cPi / 6.0f * dsp.LPFDirectCoefficient);
					filterParameters.OneOverQ = 1.0f;

					//Voiceに計算結果を設定
					m_voice->SetOutputMatrix(Audio::m_masteringVoice, m_channels, Audio::m_masterChannels, dsp.pMatrixCoefficients);
					m_voice->SetFrequencyRatio(dsp.DopplerFactor);
					m_voice->SetOutputMatrix(Audio::getLayer(m_submixLayerID)->m_voice, 1, 1, &dsp.ReverbLevel);
					m_voice->SetFilterParameters(&filterParameters);

					//ループカウント代入
					m_bufferInfo.LoopCount = loopCount;
					//再生
					m_voice->FlushSourceBuffers();
					m_voice->SubmitSourceBuffer(&m_bufferInfo);
					m_voice->Start();
					isPlayCompleted = true;
				}
			}
			//サブボイス
			else
			{
				//GetState
				m_subVoices[m_useVoice]->GetState(&m_state);
				//再生していなければ
				if (m_state.BuffersQueued == 0)
				{
					//X3DAudio計算
					X3DAudioCalculate(Audio::m_x3dHandle, &Audio::m_x3dListener, &m_emitter, m_flags, &dsp);
					//フィルタ設定
					filterParameters.Type = LowPassFilter;
					filterParameters.Frequency = 2.0f * sinf(X3DAUDIO_PI / 6.0f * dsp.LPFDirectCoefficient);
					filterParameters.OneOverQ = 1.0f;

					//Voiceに計算結果を設定
					m_subVoices[m_useVoice]->SetOutputMatrix(Audio::m_masteringVoice, m_channels, Audio::m_masterChannels, dsp.pMatrixCoefficients);
					m_subVoices[m_useVoice]->SetFrequencyRatio(dsp.DopplerFactor);
					m_subVoices[m_useVoice]->SetOutputMatrix(Audio::getLayer(m_submixLayerID)->m_voice, m_channels, 1, &dsp.ReverbLevel);
					m_subVoices[m_useVoice]->SetFilterParameters(&filterParameters);

					//ループカウント代入
					m_bufferInfo.LoopCount = loopCount;
					//再生
					m_subVoices[m_useVoice]->FlushSourceBuffers();
					m_subVoices[m_useVoice]->SubmitSourceBuffer(&m_bufferInfo);
					m_subVoices[m_useVoice]->Start();
					isPlayCompleted = true;
				}
			}

			//再生できた
			if (isPlayCompleted)
			{
				//再生間隔初期化
				m_playElapased = 0.0f;
				//カウンター加算
				++m_useVoice;
				//超過設定
				if (m_useVoice >= m_subVoices.size())
					m_useVoice = -1;
			}
		}

		//----------------------------------------------------------------------------------
		//[StopExecution]
		//実際に停止を行う
		void AudioAssetEffect3D::StopExecution()
		{
			//停止し初期化
			m_voice->Stop();
			m_voice->FlushSourceBuffers();
			
			//サブボイスも停止
			for (auto& e : m_subVoices)
			{
				e->Stop();
				e->FlushSourceBuffers();
			}
		}

		//----------------------------------------------------------------------------------
		//[ReleaseVoice]
		//音声を開放する
		void AudioAssetEffect3D::ReleaseVoice()
		{
			using namespace Detail::Audio;
			
			if (m_voice == nullptr) return;

			if (Audio::m_masteringVoice == nullptr)
				throw InvalidArgument(L"Error!! AudioAssetEffect3D->ReleaseVoice", L"Master voice was destroyed");

			AtomicOperation::LockAtomic(m_lock);

			//初期値にすべて初期化
			ZeroMemory(&m_emitter, sizeof(m_emitter));
			ZeroMemory(&m_emitterBuffer, sizeof(m_emitterBuffer));
			m_flags = m_cDefaultFlags;
			m_flagsBuffer = m_cDefaultFlags;
		
			ReleaseBaseVoice();

			AtomicOperation::UnlockAtomic(m_lock);


			//Message作成
			AudioMessage message(MessageType::Effect3DFlag | MessageType::RemoveEffect3D);
			message.attachment.onlyInstanceID = instanceID();
			//Message送信
			Audio::SendAudioMessage(message);
		}
	}
}