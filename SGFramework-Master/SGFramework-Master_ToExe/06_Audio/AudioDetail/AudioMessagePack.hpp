/*----------------------------------------------------------------------------------
オーディオを管理するAudio class
	->Message内のUnionで使用する構造体等を定義するAudioMessagePack.hpp
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

	//パーツ
	namespace Detail
	{
		//Audio
		namespace Audio
		{
			//MessageでSmooth種類を判別するSmoothType enum
			struct SmoothType
			{
				enum Enum
				{
					Speed = 0x1,		//speed Change
					Volume = 0x2	//volume Change
				};
			};
			//Messageの種類を定義したMessageType enum
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

			//AddSmoothSubmix messageで使用するPackSubmixSmooth structure
			struct PackSubmixSmooth
			{
				//submixVoice
				IXAudio2SubmixVoice* submixVoice;
				float now;				//現在値
				float target;			//ターゲット
				float speed;			//変更スピード (秒速)
			};
			//AddSmoothAudio messageで使用するPackAudioSmooth structure
			struct PackAudioSmooth
			{
				//asset
				WeakPointer<BaseClass::BaseAudioAsset> asset;
				//smooth type
				SmoothType::Enum smoothType;
				float now;				//現在値
				float target;			//ターゲット
				float speed;			//変更スピード (秒速)
				bool isStop;			//変更完了時停止するか否か
			};

			//AddEffect messageで使用するPackAddEffect structure
			struct PackAddEffect
			{
				//Asset, if remove -> null
				WeakPointer<Asset::AudioAssetEffect> asset;
				uint instanceID;			//asset instance id
			};
			//PlayEffect messageで使用するPackPlayEffect structure
			struct PackPlayEffect
			{
				uint instanceID;			//asset instance id
				uint loopCount;			//ループ回数
			};

			//AddEffect3D messageで使用するPackAddEffect3D structure
			struct PackAddEffect3D
			{
				//Asset, if remove -> null
				WeakPointer<Asset::AudioAssetEffect3D> asset;
				uint instanceID;	//asset instance id
			};
			//PlayEffect3D messageで使用するPackPlayEffect3D structure
			struct PackPlayEffect3D
			{
				Vector3 position;	//座標
				Vector3 velocity;	//移動量 (秒速)
				Vector3 forward;	//前方向ベクトル
				Vector3 up;			//上方向ベクトル
				uint loopCount;		//ループ回数
				uint instanceID;		//asset instance id
			};
		}
	}
}

#endif //!SGFRAMEWORK_HEADER_AUDIO_MESSAGE_PACK_HPP_