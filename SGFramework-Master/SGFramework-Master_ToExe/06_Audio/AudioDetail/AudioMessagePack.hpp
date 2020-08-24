/*----------------------------------------------------------------------------------
�I�[�f�B�I���Ǘ�����Audio class
	->Message����Union�Ŏg�p����\���̓����`����AudioMessagePack.hpp
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_AUDIO_MESSAGE_PACK_HPP_
#define SGFRAMEWORK_HEADER_AUDIO_MESSAGE_PACK_HPP_
#include <xaudio2.h>
#include "../../02_CustomVariable/CustomVariable.hpp"

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
			//Message��Smooth��ނ𔻕ʂ���SmoothType enum
			struct SmoothType
			{
				enum Enum
				{
					Speed = 0x1,		//speed Change
					Volume = 0x2	//volume Change
				};
			};
			//Message�̎�ނ��`����MessageType enum
			struct MessageType
			{
				enum Enum
				{
					SmoothFlag = 0x100,				//smooth Message Flag
					AddSmoothSubmix = 0x001,	//add Smooth->Submix
					AddSmoothAudio = 0x002,		//add Smooth->Audio Asset

					EffectFlag = 0x200,				//effect Message Flag
					AddEffect = 0x001,				//add Effect
					RemoveEffect = 0x002,			//remove Effect
					PlayEffect = 0x004,				//play Effect
					StopEffect = 0x008,				//stop Effect

					Effect3DFlag = 0x400,			//effect3d Message Flag
					AddEffect3D = 0x001,			//add Effect3D
					RemoveEffect3D = 0x002,		//remove Effect3D
					PlayEffect3D = 0x004,			//play Effect3D
					StopEffect3D = 0x008,			//stop Effect
				};
			};

			//AddSmoothSubmix message�Ŏg�p����PackSubmixSmooth structure
			struct PackSubmixSmooth
			{
				//submixVoice
				IXAudio2SubmixVoice* submixVoice;
				float now;				//���ݒl
				float target;			//�^�[�Q�b�g
				float speed;			//�ύX�X�s�[�h (�b��)
			};
			//AddSmoothAudio message�Ŏg�p����PackAudioSmooth structure
			struct PackAudioSmooth
			{
				//asset
				WeakPointer<BaseClass::BaseAudioAsset> asset;
				//smooth type
				SmoothType::Enum smoothType;
				float now;				//���ݒl
				float target;			//�^�[�Q�b�g
				float speed;			//�ύX�X�s�[�h (�b��)
				bool isStop;			//�ύX��������~���邩�ۂ�
			};

			//AddEffect message�Ŏg�p����PackAddEffect structure
			struct PackAddEffect
			{
				//Asset, if remove -> null
				WeakPointer<Asset::AudioAssetEffect> asset;
				uint instanceID;			//asset instance id
			};
			//PlayEffect message�Ŏg�p����PackPlayEffect structure
			struct PackPlayEffect
			{
				uint instanceID;			//asset instance id
				uint loopCount;			//���[�v��
			};

			//AddEffect3D message�Ŏg�p����PackAddEffect3D structure
			struct PackAddEffect3D
			{
				//Asset, if remove -> null
				WeakPointer<Asset::AudioAssetEffect3D> asset;
				uint instanceID;	//asset instance id
			};
			//PlayEffect3D message�Ŏg�p����PackPlayEffect3D structure
			struct PackPlayEffect3D
			{
				Vector3 position;	//���W
				Vector3 velocity;	//�ړ��� (�b��)
				Vector3 forward;	//�O�����x�N�g��
				Vector3 up;			//������x�N�g��
				uint loopCount;		//���[�v��
				uint instanceID;		//asset instance id
			};
		}
	}
}

#endif //!SGFRAMEWORK_HEADER_AUDIO_MESSAGE_PACK_HPP_