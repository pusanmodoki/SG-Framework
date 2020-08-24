#include "Audio.hpp"
#include "AudioAsset/AudioAssetEffect.hpp"
#include "AudioAsset/AudioAssetEffect3D.hpp"
#include "AudioListener.hpp"
#include "../07_Thread/Thread.hpp"

// Framework namespace
namespace SGFramework
{
	//----------------------------------------------------------------------------------
	//[UpdateThread]
	//オーディオの更新を行う, 別スレッド用関数
	void Audio::UpdateThread()
	{
		//1ミリ秒
		static const std::chrono::milliseconds cOneMilliSeconds(1);
		//delta計算用
		static TimeProcessing::TimeGetterDouble s_timeGetter;
		static double s_nowTime = 0;								//現在時刻取得用
		static double s_oldFlameTime = s_timeGetter();	//delta計算用
		static float s_deltaTime = 0.0f;							//delta time

		while (true)
		{
			if (IS_TRUE(m_isEndUpdate)) return;
			//現在の時刻を取得
			s_nowTime = s_timeGetter();
			//deltaTime取得
			s_deltaTime = static_cast<float>(s_nowTime - s_oldFlameTime);
			//oldTime更新
			s_oldFlameTime = s_nowTime;

			//deltaを基に更新間隔を調整
			if (s_deltaTime > m_audioUpdateIntervalForFloat && m_nowAudioUpdateInterval > cOneMilliSeconds)
				m_nowAudioUpdateInterval -= cOneMilliSeconds;
			else if (s_deltaTime < m_audioUpdateIntervalForFloat)
				m_nowAudioUpdateInterval += cOneMilliSeconds;

			//次回スレッド起動時間設定
			auto nextWakeTime =
				std::chrono::system_clock::now() + m_nowAudioUpdateInterval;

			//Messageリストをswap
			AtomicOperation::LockAtomic(m_lockMessages);
			std::swap(m_messages, m_useThreadMessages);
			AtomicOperation::UnlockAtomic(m_lockMessages);

			//Listener取得
			AudioListener::getListener(m_x3dListener, s_deltaTime);

			//Message更新ループ
			for (auto& e : m_useThreadMessages)
			{
				//Message処理
				ProcessingMessage(e);
				//Release
				e.Release();
			}

			//使用中のメッセージリストを初期化
			m_useThreadMessages.clear();

			//Smooth更新
			UpdateSmooth(s_deltaTime);

			//指定秒数経過するまでロック
			std::this_thread::sleep_until(nextWakeTime);
		}
	}

	//----------------------------------------------------------------------------------
	//[ProcessingMessage]
	//メッセージを処理する
	//引数1: message
	void Audio::ProcessingMessage(const Detail::Audio::AudioMessage & message)
	{
		using namespace Detail::Audio;
		uint messageType = message.type;

		//effect 3d message
		if (messageType & MessageType::Effect3DFlag)
		{
			//play effect message
			if (messageType & MessageType::PlayEffect3D)
			{
				auto& asset = m_3dEffects.at(message.attachment.playEffect3D.instanceID);

				if (IS_TRUE(asset.LockShared()))
				{
					asset->PlayExecution(message.attachment.playEffect3D.position, 
						message.attachment.playEffect3D.velocity, message.attachment.playEffect3D.forward,
						message.attachment.playEffect3D.up, message.attachment.playEffect3D.loopCount);
					asset.UnlockShared();
				}
			}
			//stop effect message
			else if (messageType & MessageType::StopEffect3D)
			{
				auto& asset = m_3dEffects.at(message.attachment.onlyInstanceID);

				if (IS_TRUE(asset.LockShared()))
				{
					asset->StopExecution();
					asset.UnlockShared();
				}
			}
			//add effect message
			else if (messageType & MessageType::AddEffect3D)
			{
				auto asset = message.attachment.addEffect3D.asset;
				
				if (IS_TRUE(asset.LockShared()))
				{
					m_3dEffects.try_emplace(message.attachment.addEffect3D.instanceID, asset);
					asset.UnlockShared();
				}
			}
			//remove effect message
			else if (messageType & MessageType::RemoveEffect3D)
			{
				m_3dEffects.erase(message.attachment.onlyInstanceID);
			}
		}

		//effect 2d message
		else if (messageType & MessageType::EffectFlag)
		{
			//play effect message
			if (messageType & MessageType::PlayEffect)
			{
				auto& asset = m_2dEffects.at(message.attachment.playEffect.instanceID);

				if (IS_TRUE(asset.LockShared()))
				{
					asset->PlayExecution(message.attachment.playEffect.loopCount);
					asset.UnlockShared();
				}
			}
			//stop effect message
			else if (messageType & MessageType::StopEffect)
			{
				auto& asset = m_2dEffects.at(message.attachment.onlyInstanceID);
		
				if (IS_TRUE(asset.LockShared()))
				{
					asset->StopExecution();
					asset.UnlockShared();
				}
			}
			//add effect message
			else if (messageType & MessageType::AddEffect)
			{
				auto asset = message.attachment.addEffect.asset;

				if (IS_TRUE(asset.LockShared()))
				{
					m_2dEffects.try_emplace(message.attachment.addEffect.instanceID, asset);
					asset.UnlockShared();
				}
			}
			//remove effect message
			else if (messageType & MessageType::RemoveEffect)
			{
				m_2dEffects.erase(message.attachment.onlyInstanceID);
			}
		}

		//smooth message
		else if (messageType & MessageType::SmoothFlag)
		{
			//add smooth(audio) message
			if (messageType & MessageType::AddSmoothAudio)
			{
				auto asset = message.attachment.smoothAudio.asset;
				if (IS_FALSE(asset.LockShared())) return;

				if (m_smoothAudios.find(asset->instanceID()) == m_smoothAudios.end())
					m_smoothAudios.try_emplace(asset->instanceID(), SmoothAudio(asset));
			
				auto& smooth = m_smoothAudios.at(asset->instanceID());

				if (message.attachment.smoothAudio.smoothType & SmoothType::Volume)
				{
					smooth.volume.now = message.attachment.smoothAudio.now;
					smooth.volume.speed = message.attachment.smoothAudio.speed;
					smooth.volume.target = message.attachment.smoothAudio.target;
					smooth.volume.isStop = message.attachment.smoothAudio.isStop;
					smooth.volume.isEnabled = true;
				}
				else
				{
					smooth.speed.now = message.attachment.smoothAudio.now;
					smooth.speed.speed = message.attachment.smoothAudio.speed;
					smooth.speed.target = message.attachment.smoothAudio.target;
					smooth.speed.isStop = message.attachment.smoothAudio.isStop;
					smooth.speed.isEnabled = true;
				}

				asset.UnlockShared();
			}
			//add smooth(submix) message
			else if (messageType & MessageType::AddSmoothSubmix)
			{
				auto& attachment = message.attachment.smoothSubmix;

				if (m_smoothSubmixes.find(attachment.submixVoice) == m_smoothSubmixes.end())
					m_smoothSubmixes.try_emplace(attachment.submixVoice, 
						SmoothSubmix(attachment.now, attachment.target, attachment.speed));

				auto& smooth = m_smoothSubmixes.at(attachment.submixVoice);

				smooth.now = message.attachment.smoothAudio.now;
				smooth.speed = message.attachment.smoothAudio.speed;
				smooth.target = message.attachment.smoothAudio.target;
			}
		}
	}
}