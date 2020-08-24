/*----------------------------------------------------------------------------------
通常SEを扱うAudioAssetEffect class
	->AudioAssetEffect.cpp
------------------------------------------------------------------------------------*/
#include "AudioAssetEffect.hpp"

// Framework namespace
namespace SGFramework
{
	//Asset関連を格納したAsset namespace
	namespace Asset
	{
		//----------------------------------------------------------------------------------
		//[Open]
		//アセットのロードを行う
		//引数1: 音声が所属するSubmixLayer, default = not monoral root layer
		//引数2: ソースの数 (最大同時再生数)
		//引数3: 再生間隔
		//引数4: 再生開始秒数, default = cDefault (Begin)
		//引数5: ループ時開始秒数, default = cDefault (Begin)
		//引数6: 再生終了秒数, default = cDefault (End)
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
			
				//Message作成
				AudioMessage message(MessageType::EffectFlag | MessageType::AddEffect);
				message.attachment.addEffect.asset = m_thisPointer.DynamicCast<AudioAssetEffect>();
				message.attachment.addEffect.instanceID = instanceID();
				//Message送信
				Audio::SendAudioMessage(message);
			}
			catch (...) { throw; }
		}

		//----------------------------------------------------------------------------------
		//[Reload]
		//アセットのリロードを行う
		void AudioAssetEffect::Reload()
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

			//Release
			Close();

			//Open
			Open(submixLayer, setAmountOfSource, m_playbackInterval, 
				setPlayBeginSeconds, setLoopBeginSeconds, setPlayEndSeconds);
			}

		//----------------------------------------------------------------------------------
		//[Close]
		//ファイルを閉じる
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
		//実際に再生を行う
		void AudioAssetEffect::PlayExecution(uint loopCount)
		{
			//再生間隔に満たない場合再生せず終了
			if (Time::time - m_playElapased < m_playbackInterval) return;
			m_playElapased = Time::time;

			//再生完了フラグ
			bool isPlayCompleted = false;

			//useVoice == -1 -> メインボイス
			if (m_useVoice == -1)
			{
				//GetState
				m_voice->GetState(&m_state);
				//再生していなければ
				if (m_state.BuffersQueued == 0)
				{
					//ループカウント代入
					m_bufferInfo.LoopCount = loopCount;
					//再生
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
					//ループカウント代入
					m_bufferInfo.LoopCount = loopCount;
					//再生
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
		void AudioAssetEffect::StopExecution()
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
		void AudioAssetEffect::ReleaseVoice()
		{
			using namespace Detail::Audio;
			
			if (m_voice == nullptr) return;

			if (Audio::m_masteringVoice == nullptr)
				throw InvalidArgument(L"Error!! AudioAssetEffect->ReleaseVoice", L"Master voice was destroyed");

			AtomicOperation::LockAtomic(m_lock);

			ReleaseBaseVoice();

			AtomicOperation::UnlockAtomic(m_lock);

			//Message作成
			AudioMessage message(MessageType::EffectFlag | MessageType::RemoveEffect);
			message.attachment.onlyInstanceID = instanceID();
			//Message送信
			Audio::SendAudioMessage(message);
		}
	}
}