/*----------------------------------------------------------------------------------
オーディオを管理するAudio class
	->UpdateSmoothを記述するAudioSmooth.cpp
------------------------------------------------------------------------------------*/
#include "Audio.hpp"
#include "AudioAsset/AudioAsset.hpp"
#include "AudioAsset/AudioAssetMusic.hpp"
#include "AudioAsset/AudioAssetEffect.hpp"
#include "AudioAsset/AudioAssetEffect3D.hpp"

// Framework namespace
namespace SGFramework
{
	//----------------------------------------------------------------------------------
	//[UpdateSmooth]
	//スムースの更新を行う
	//引数1: delta time
	void Audio::UpdateSmooth(float deltaTime)
	{
		//名前省略用
		static Audio::SmoothAudio::Values* s_value = nullptr;
		//result
		static float s_result = 0.0f;

		//smooth audio
		for (auto it =  m_smoothAudios.begin(); it != m_smoothAudios.end();)
		{
			//volume update
			if (IS_TRUE(it->second.volume.isEnabled))
			{
				//value取得
				s_value = &it->second.volume;

				//スピード加算
				s_result = s_value->now + s_value->speed * deltaTime;
				//超過設定
				if ((s_value->now > s_value->target && s_result < s_value->target)
					|| (s_value->now < s_value->target && s_result > s_value->target))
					s_result = s_value->target;

				//set
				it->second.asset->setVolume(s_result);
				s_value->now = s_result;

				//Target到達で無効化
				if (s_result == s_value->target)
				{
					s_value->isEnabled = false;
					if (IS_TRUE(s_value->isStop))
						it->second.asset->Stop();
				}
			}
			//speed update
			if (IS_TRUE(it->second.speed.isEnabled))
			{
				//value取得
				s_value = &it->second.speed;

				//スピード加算
				s_result = s_value->now + s_value->speed * deltaTime;
				//超過設定
				if ((s_value->now > s_value->target && s_result < s_value->target)
					|| (s_value->now < s_value->target && s_result > s_value->target))
					s_result = s_value->target;

				//set
				it->second.asset->setPlayBackSpeed(s_result);
				s_value->now = s_result;

				//Target到達で無効化
				if (s_result == s_value->target)
				{
					s_value->isEnabled = false;
					if (IS_TRUE(s_value->isStop))
						it->second.asset->Stop();
				}
			}

			//イテレータを進める or 削除
			if ((it->second.volume.isEnabled | it->second.speed.isEnabled))
				++it;
			else
				it = m_smoothAudios.erase(it);
		}

		//smooth submix
		for (auto it = m_smoothSubmixes.begin(); it != m_smoothSubmixes.end();)
		{
			//スピード加算
			s_result = it->second.now + it->second.speed * deltaTime;
			//超過設定
			if ((it->second.now > it->second.target && s_result < it->second.target)
				|| (it->second.now < it->second.target && s_result > it->second.target))
				s_result = it->second.target;

			//Set
			it->first->SetVolume(s_result);
			it->second.now = s_result;

			//イテレータを進める or 削除
			if (s_result < it->second.target) ++it;
			else it = m_smoothSubmixes.erase(it);
		}
	}
}