/*----------------------------------------------------------------------------------
通常SEを扱うAudioAssetEffect class
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_AUDIO_ASSET_EFFECT_HPP_
#define SGFRAMEWORK_HEADER_AUDIO_ASSET_EFFECT_HPP_
#include <xaudio2.h>
#include "../../02_CustomVariable/CustomVariable.hpp"
#include "../../03_MathAF/MathAF.hpp"
#include "../../04_Asset/Asset/BaseAsset.hpp"
#include "AudioAsset.hpp"
#include "../Audio.hpp"

// Framework namespace
namespace SGFramework
{
	//prototype
	class Audio;
	class Audio::SubmixLayer;
	class AssetManager;

	//Asset関連を格納したAsset namespace
	namespace Asset
	{
		//通常SEを扱うAudioAssetEffect class
		class AudioAssetEffect final : public BaseClass::BaseAudioSEAsset
		{
		public:
			//friend
			friend class Audio;
			friend class AssetManager;
			template <typename TAsset>
			friend SharedPointer<TAsset> Asset::InstantiateAsset(const sgstring& filePath, bool isRegisterSharedAsset);


			//----------------------------------------------------------------------------------
			//[デストラクタ]
			//アセットの開放を行う
			inline ~AudioAssetEffect() { ReleaseVoice(); CloseBaseAsset(getType(), true); }

			//----------------------------------------------------------------------------------
			//[Open]
			//アセットのロードを行う
			//引数1: 音声が所属するSubmixLayer, default = not monoral root layer
			//引数2: ソースの数 (最大同時再生数)
			//引数3: 再生間隔
			//引数4: 再生開始秒数, default = cDefault (Begin)
			//引数5: ループ時開始秒数, default = cDefault (Begin)
			//引数6: 再生終了秒数, default = cDefault (End)
			void Open(WeakPointer<Audio::SubmixLayer> submixLayer = Audio::submixRootLayer(),
				uint amountOfSource = 1, float playbackInterval = 0.0f, float setPlayBeginSeconds = cDefault, 
				float setLoopBeginSeconds = cDefault, float setPlayEndSeconds = cDefault);

			//----------------------------------------------------------------------------------
			//[Reload]
			//アセットのリロードを行う
			void Reload() override;
			//----------------------------------------------------------------------------------
			//[Close]
			//ファイルを閉じる
			void Close()  override;


			//----------------------------------------------------------------------------------
			//[Play]
			//再生を行う, 実際に再生されるのは1フレーム後となる
			//引数1: ループ回数, default = 0
			inline void Play(uint loopCount = 0)
			{
				using namespace Detail::Audio;

				//Message作成
				AudioMessage message(MessageType::EffectFlag | MessageType::PlayEffect);
				message.attachment.playEffect.instanceID = instanceID();
				message.attachment.playEffect.loopCount = loopCount;
				//Message送信
				Audio::SendAudioMessage(message);
			}
			//----------------------------------------------------------------------------------
			//[Stop]
			//再生を全て停止する, 実際に停止されるのは1フレーム後となる
			inline void Stop()
			{
				using namespace Detail::Audio;

				//Message作成
				AudioMessage message(MessageType::EffectFlag | MessageType::StopEffect);
				message.attachment.onlyInstanceID = instanceID();
				//Message送信
				Audio::SendAudioMessage(message);
			}

			//get asset type (read function property)
			SGF_FUNCTION_PROPERTY Asset::AssetType getType() override { return Asset::AssetType::AudioEffect; }

		private:	
			//----------------------------------------------------------------------------------
			//[コンストラクタ]
			//初期化を行う
			inline AudioAssetEffect() : BaseAudioSEAsset() {}

			//----------------------------------------------------------------------------------
			//[PlayExecution]
			//実際に再生を行う
			void PlayExecution(uint loopCount);
			//----------------------------------------------------------------------------------
			//[StopExecution]
			//実際に停止を行う
			void StopExecution();
			//----------------------------------------------------------------------------------
			//[ReleaseVoice]
			//音声を開放する
			void ReleaseVoice();
		};
	}
}
#endif //!SGFRAMEWORK_HEADER_AUDIO_ASSET_EFFECT_HPP_ 