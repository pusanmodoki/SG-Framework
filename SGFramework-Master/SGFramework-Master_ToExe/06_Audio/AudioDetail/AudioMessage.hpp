/*----------------------------------------------------------------------------------
Audio Message������AudioMessage structure
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

	//�p�[�c
	namespace Detail
	{
		//Audio
		namespace Audio
		{
			//Audio Message������AudioMessage structure
			struct AudioMessage
			{
				//�t�я����i�[����Attachment union
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

				//[�R���X�g���N�^]
				inline AudioMessage(uint messageType)
					: attachment(), type(messageType) {}
				//[�R���X�g���N�^]->memcpy
				inline AudioMessage(const AudioMessage& copy)
				{
					memcpy_s(this, sizeof(AudioMessage), &copy, sizeof(AudioMessage));
				}
				//----------------------------------------------------------------------------------
				//[Release]
				//Pointer�Ȃǂ��t�т���Ă���ꍇ�J������
				inline void Release();

				Attachment attachment;		//�t�я��
				uint type;							//message type
			};
			//58�o�C�g�Ɏ��߂����B�ł������ł����B64�o�C�g�B
			static constexpr size_t cMessageSize = sizeof(AudioMessage);

			//----------------------------------------------------------------------------------
			//[Release]
			//Pointer�Ȃǂ��t�т���Ă���ꍇ�J������
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