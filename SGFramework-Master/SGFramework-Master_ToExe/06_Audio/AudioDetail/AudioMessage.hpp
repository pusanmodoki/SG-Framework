/*----------------------------------------------------------------------------------
Audio Messageを扱うAudioMessage structure
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_AUDIO_MESSAGE_HPP_
#define SGFRAMEWORK_HEADER_AUDIO_MESSAGE_HPP_
#include <xaudio2.h>
#include "../../02_CustomVariable/CustomVariable.hpp"
#include "AudioMessagePack.hpp"

// Framework namespace
namespace SGFramework
{
	//prototype
	namespace BaseClass
	{
		class BaseAudioAsset;
		class BaseAudioSEAsset;
	}
	namespace Asset
	{
		class AudioAssetMusic;
		class AudioAssetEffect;
		class AudioAssetEffect3D;
	}

	//パーツ
	namespace Detail
	{
		//Audio
		namespace Audio
		{
			//Audio Messageを扱うAudioMessage structure
			struct AudioMessage
			{
				//付帯情報を格納するAttachment union
				union Attachment
				{
					inline Attachment() { ZeroMemory(this, sizeof(Attachment)); }
					inline ~Attachment() {}

					PackSubmixSmooth smoothSubmix;
					PackAudioSmooth smoothAudio;
					PackAddEffect addEffect;
					PackPlayEffect playEffect;
					PackAddEffect3D addEffect3D;
					PackPlayEffect3D playEffect3D;
					uint onlyInstanceID;
					struct { byte byteData[56]; };
				};

				//[コンストラクタ]
				inline AudioMessage(uint messageType)
					: attachment(), type(messageType) {}
				//[コンストラクタ]->memcpy
				inline AudioMessage(const AudioMessage& copy)
				{
					memcpy_s(this, sizeof(AudioMessage), &copy, sizeof(AudioMessage));
				}
				//----------------------------------------------------------------------------------
				//[Release]
				//Pointerなどが付帯されている場合開放する
				inline void Release();

				Attachment attachment;		//付帯情報
				uint type;							//message type
			};
			//58バイトに収めたい。でも無理でした。64バイト。
			static constexpr size_t cMessageSize = sizeof(AudioMessage);

			//----------------------------------------------------------------------------------
			//[Release]
			//Pointerなどが付帯されている場合開放する
			inline void AudioMessage::Release()
			{
				if ((type == (MessageType::EffectFlag | MessageType::AddEffect))
					&& IS_TRUE(attachment.addEffect.asset.getIsValid()))
				{
					attachment.addEffect.asset.Reset();
				}
				else if ((type == (MessageType::Effect3DFlag | MessageType::AddEffect3D))
					&& IS_TRUE(attachment.addEffect3D.asset.getIsValid()))
				{
					attachment.addEffect3D.asset.Reset();
				}
				else if ((type == (MessageType::SmoothFlag | MessageType::AddSmoothAudio))
					&& IS_TRUE(attachment.smoothAudio.asset.getIsValid()))
				{
					attachment.smoothAudio.asset.Reset();
				}
			}
		}
	}
}
#endif //!SGFRAMEWORK_HEADER_AUDIO_MESSAGE_HPP_