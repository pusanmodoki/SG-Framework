/*----------------------------------------------------------------------------------
オーディオを管理するAudio class
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_AUDIO_HPP_
#define SGFRAMEWORK_HEADER_AUDIO_HPP_
#include <Windows.h>
#include <mmsystem.h>
#include <xaudio2.h>
#include <x3daudio.h>
#include "../02_CustomVariable/CustomVariable.hpp"
#include "../03_MathAF/MathAF.hpp"
#include "AudioDetail/AudioMessage.hpp"
#include "AudioListener.hpp"

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "xaudio2.lib")

// Framework namespace
namespace SGFramework
{
	//friend
	class Application;
	class Thread;
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

	//オーディオを管理するAudio class
	class Audio final
	{
	public:
		//friend
		friend class Application;
		friend class Thread;
		friend class BaseClass::BaseAudioAsset;
		friend class BaseClass::BaseAudioSEAsset;
		friend class Asset::AudioAssetMusic;
		friend class Asset::AudioAssetEffect;
		friend class Asset::AudioAssetEffect3D;
		//prototype
		class SubmixLayer;
		
		DELETE_ALL_DEFAULT_CLASS_FUNCTIONS(Audio);

		//----------------------------------------------------------------------------------
		//[FindLayer]
		//nameのサブミックスレイヤーを検索する
		//return: 存在していた場合layerのポインタ, ない場合は空ポインタ
		//引数1: レイヤー名
		inline static SharedPointer<SubmixLayer> FindLayer(const sgstring& name);

		//submix layer[id]<static>(get function property)
		//return:  layer[id], not found->null
		//argument 1: id
		SGF_FUNCTION_PROPERTY static SharedPointer<SubmixLayer> getLayer(uint id)
		{
			//root分岐->あればreturn->なければ空ポインタ
			return id == 0 ? m_submixRootLayer :
				id == 1 ? m_submixMonoralRootLayer :
				id <= m_layerCounter ? 
				m_submixLayers[id - 2] : SharedPointer<Audio::SubmixLayer>::getNull();
		}

		//master channels mask(used XAudio2)<static> (read only property)
		SGF_FUNCTION_PROPERTY sgstring getMasterChannelsMask() const
		{
			//チャンネルマスク
			uint channelMask = SPEAKER_MONO;
			//チャンネル分岐
			switch (m_masterChannels)
			{
			case 2:
				channelMask = SPEAKER_STEREO;
			case 3:
				channelMask = SPEAKER_2POINT1;
			case 4:
				channelMask = SPEAKER_SURROUND;
			case 5:
				channelMask = SPEAKER_4POINT1;
			case 6:
				channelMask = SPEAKER_5POINT1_SURROUND;
			case 8:
				channelMask = SPEAKER_7POINT1_SURROUND;
			default:
				break;
			}
		}
		//accurate master channels<static> (read only property)
		SGF_FUNCTION_PROPERTY static sgstring getChannelsString()
		{
			//チャンネル分岐
			switch (m_masterChannels)
			{
			case 1:
				return L"1.0";
			case 2:
				return L"2.0";
			case 3:
				return L"2.1";
			case 4:
				return L"3.1";
			case 5:
				return L"4.1";
			case 6:
				return L"5.1";
			case 8:
				return L"7.1";
			default:
				return L"unknown";
			}
		}

		//monoral
		static constexpr uint cMonoral = 1;

		//IXAudio2
		static PointerProperty<IXAudio2*> xaudio2;
		//IXAudio2MasteringVoice
		static PointerProperty<IXAudio2MasteringVoice*> masteringVoice;
		//RootLayer
		static GetOnlyProperty<SharedPointer<SubmixLayer>> submixRootLayer;
		//MonoralRootLayer このレイヤーの子レイヤーは全てチャンネル数が1になる
		static GetOnlyProperty<SharedPointer<SubmixLayer>> submixMonoralRootLayer;
		//規定のチャンネル数
		static GetOnlyProperty<uint> masterChannels;
		//マスターサンプルレート
		static GetOnlyProperty<uint> sampleRate;
		
		//SubmixAudioをレイヤー構造で提供するSubmixLayer class
		class SubmixLayer final
		{
		public:
			//friend
			friend class Audio;
			friend class BaseClass::BaseAudioAsset;
			friend class BaseClass::BaseAudioSEAsset;
			friend class Asset::AudioAssetMusic;
			friend class Asset::AudioAssetEffect;
			friend class Asset::AudioAssetEffect3D;
			
			//----------------------------------------------------------------------------------
			//[デストラクタ]
			//DestroyVoiceを行う
			~SubmixLayer();

			//----------------------------------------------------------------------------------
			//[ChangeVolumeSmoothly]
			//再生音量をtargetまで滑らかに変更する
			//引数1: 変更target
			//引数2: 変更にかける秒数
			inline void ChangeVolumeSmoothly(float target, float seconds);

			//parent layer (get function property)if null = not parent
			SGF_FUNCTION_PROPERTY WeakPointer<SubmixLayer> getParent() const { return m_parent; }
			//childrens iterator cbegin (get function property)
			SGF_FUNCTION_PROPERTY auto getChildrensIteratorBegin() const { return m_childrens.cbegin(); }
			//childrens iterator cend (get function property)
			SGF_FUNCTION_PROPERTY auto getChildrensIteratorEnd() const { return m_childrens.cend(); }
			//num childrens (get function property)
			SGF_FUNCTION_PROPERTY uint getNumChildrens() const { return static_cast<uint>(m_childrens.size()); }
			//volume (get function property)
			SGF_FUNCTION_PROPERTY float getVolume()
			{
				float result = 0.0f;
				//Get
				m_voice->GetVolume(&result);
				return result;
			}
			//volume (get function property)
			//return : volume
			//argument 1: volume magnification (倍率, 通常1)
			SGF_FUNCTION_PROPERTY float setVolume(float magnification) { m_voice->SetVolume(magnification); return magnification; }

			//master channels mask(used XAudio2)<static> (read only property)
			SGF_FUNCTION_PROPERTY sgstring getChannelsMask() const
			{
				//チャンネルマスク
				uint channelMask = SPEAKER_MONO;
				//チャンネル分岐
				switch (m_channels)
				{
				case 2:
					channelMask = SPEAKER_STEREO;
				case 3:
					channelMask = SPEAKER_2POINT1;
				case 4:
					channelMask = SPEAKER_SURROUND;
				case 5:
					channelMask = SPEAKER_4POINT1;
				case 6:
					channelMask = SPEAKER_5POINT1_SURROUND;
				case 8:
					channelMask = SPEAKER_7POINT1_SURROUND;
				default:
					break;
				}
			}
			//accurate master channels<static> (read only property)
			SGF_FUNCTION_PROPERTY sgstring getChannelsString() const
			{
				//チャンネル分岐
				switch (m_channels)
				{
				case 1:
					return L"1.0";
				case 2:
					return L"2.0";
				case 3:
					return L"2.1";
				case 4:
					return L"3.1";
				case 5:
					return L"4.1";
				case 6:
					return L"5.1";
				case 8:
					return L"7.1";
				default:
					return L"unknown";
				}
			}

			//() operator return voice
			inline const IXAudio2SubmixVoice* operator () () { return m_voice; }
			//[] operator return childrebs[index]
			inline const WeakPointer<SubmixLayer>& operator [] (uint index) { return m_childrens[index]; }

			//SubmixRootLayerのID
			static constexpr uint cRootID = 0;
			//SubmixRootLayerのName
			static constexpr wchar cRootName[] = L"root";
			//SubmixMonoralRootLayerのID
			static constexpr uint cMonoralRootID = 1;
			//SubmixMonoralRootLayerのName
			static constexpr wchar cMonoralRootName[] = L"monoralRoot";

			GetOnlyProperty<IXAudio2SubmixVoice*> voice;	//Voice
			GetOnlyProperty<sgstring> layerName;				//レイヤー名
			GetOnlyProperty<uint> layerID;							//レイヤーID
			GetOnlyProperty<uint> processingStage;				//レイヤーの階層の深さ
			GetOnlyProperty<uint> channels;						//レイヤーのチャンネル数
		private:
			//----------------------------------------------------------------------------------
			//[コンストラクタ]
			//Root用初期化を行う
			SubmixLayer();
			//----------------------------------------------------------------------------------
			//[コンストラクタ]
			//非Root用初期化を行う
			//引数1: レイヤー名
			//引数2: レイヤーID
			SubmixLayer(const sgstring& name, uint id);
			//----------------------------------------------------------------------------------
			//[SettingRoot]
			//Root用設定を行う
			//引数1: 音量
			void SettingRoot(float volume);
			//----------------------------------------------------------------------------------
			//[SettingMonoralRoot]
			//MonoralRoot用設定を行う
			//引数1: 音量
			void SettingMonoralRoot(float volume);
			//----------------------------------------------------------------------------------
			//[SettingSubmix]
			//非Root用設定を行う
			//引数1: 音量
			void SettingSubmix(float volume);	
			
			std::vector<WeakPointer<SubmixLayer>> m_childrens;		//子レイヤー
			std::vector<XAUDIO2_SEND_DESCRIPTOR> m_sendVoices;	//VoiceSends用
			sgstring m_layerName;														//レイヤー名
			XAUDIO2_VOICE_SENDS m_sends;										//Sneds
			WeakPointer<SubmixLayer> m_parent;								//親レイヤー
			IXAudio2SubmixVoice* m_voice;											//Voice
			uint m_layerID;																	//レイヤーID
			uint m_processingStage;														//レイヤーの階層の深さ
			uint m_channels;																//レイヤーのチャンネル数
		};

	private:

		//リスナー設定用バッファ
		struct X3DAudioListenerBuffer
		{
			std::atomic_bool lock;					//lock
			X3DAUDIO_LISTENER listner = {};	//listner
		};
		struct SmoothSubmix
		{
			inline SmoothSubmix(float now, float target, float speed)
				: now(now), target(target), speed(speed) {}
			float now = 0.0f;			//現在値
			float target = 0.0f;			//ターゲット
			float speed = 0.0f;			//変更スピード (秒速)
		};
		struct SmoothAudio
		{
			//タイプごとの値
			struct Values
			{
				float now = 0.0f;			//現在値
				float target = 0.0f;			//ターゲット
				float speed = 0.0f;			//変更スピード (秒速)
				bool isEnabled = false;	//有効？
				bool isStop = false;		//終了後停止？
			};
			
			inline SmoothAudio(const WeakPointer<BaseClass::BaseAudioAsset>& asset)
			{ 
				ZeroMemory(this, sizeof(SmoothAudio)); 
				this->asset = asset;
			}
			inline ~SmoothAudio() {}

			static constexpr uint cIndexVolume = 0;
			static constexpr uint cIndexSpeed = 1;

			union
			{
				struct
				{
					Values volume;
					Values speed;
				};
				Values values[2];
			};

			WeakPointer<BaseClass::BaseAudioAsset> asset;
		};

		//----------------------------------------------------------------------------------
		//[StartUp]
		//起動時初期化を行う
		//引数1: マスター音量
		//引数2: チャンネル数
		//引数3: サンプルレート
		//引数4: サブミックスレイヤー
		static void StartUp(const ReadElement::Pack& masterVolume, const ReadElement::Pack& masterChannels, 
			const ReadElement::Pack& sampleRate, const ReadElement::Pack& audioUpdateInterval, 
			const ReadElement::Pack& audioSmoothReserve, const ReadElement::Groop& submixLayers);
		//----------------------------------------------------------------------------------
		//[ShutDown]
		//終了処理を行う
		static void ShutDown();
		//----------------------------------------------------------------------------------
		//[UpdateThread]
		//オーディオの更新を行う, 別スレッド用関数
		static void UpdateThread();
		//----------------------------------------------------------------------------------
		//[ProcessingMessage]
		//メッセージを処理する
		//引数1: message
		static void ProcessingMessage(const Detail::Audio::AudioMessage& message);
		//----------------------------------------------------------------------------------
		//[UpdateSmooth]
		//スムースの更新を行う
		//引数1: delta time
		static void UpdateSmooth(float deltaTime);

		//----------------------------------------------------------------------------------
		//[SendAudioMessage]
		//メッセージを送信する
		//引数1: message
		static inline void SendAudioMessage(const Detail::Audio::AudioMessage& message);


		//IXAudio2
		static IXAudio2* m_xaudio2;																				
		//IXAudio2MasteringVoice
		static IXAudio2MasteringVoice* m_masteringVoice;
		//X3DAUDIO_HANDLE
		static X3DAUDIO_HANDLE m_x3dHandle;
		//X3DAUDIO_LISTENER
		static X3DAUDIO_LISTENER m_x3dListener;
		//effects (2D)
		static std::unordered_map<uint, WeakPointer<Asset::AudioAssetEffect>> m_2dEffects;	
		//effects (3D)
		static std::unordered_map<uint, WeakPointer<Asset::AudioAssetEffect3D>> m_3dEffects;
		//smooth (submix)
		static std::unordered_map<IXAudio2SubmixVoice*, SmoothSubmix> m_smoothSubmixes;
		//smooth (audio)
		static std::unordered_map<uint, SmoothAudio> m_smoothAudios;
		//AudioMessages
		static std::vector<Detail::Audio::AudioMessage> m_messages;
		//AudioMessages (use audio thread)
		static std::vector<Detail::Audio::AudioMessage> m_useThreadMessages;
		//サブミックスレイヤー
		static std::vector<SharedPointer<SubmixLayer>> m_submixLayers;	
		//Submix root
		static SharedPointer<SubmixLayer> m_submixRootLayer;
		//Submix monoral root
		static SharedPointer<SubmixLayer> m_submixMonoralRootLayer;
		//Audio thread
		static std::thread m_audioThread;
		//Audio thread更新間隔(chrono)
		static std::chrono::milliseconds m_nowAudioUpdateInterval;
		//Audio thread更新間隔(chrono)
		static std::chrono::milliseconds m_audioUpdateIntervalForChrono;
		//Audio thread更新間隔(float)
		static float m_audioUpdateIntervalForFloat;
		//規定のチャンネル数
		static uint m_masterChannels;
		//マスターサンプルレート
		static uint m_sampleRate;
		//サブミックスレイヤーカウンタ
		static uint m_layerCounter;
		//message lock
		static std::atomic_bool m_lockMessages;
		//audio thread 終了フラグ
		static std::atomic_bool m_isEndUpdate;
	};


	//----------------------------------------------------------------------------------
	//[FindLayer]
	//nameのサブミックスレイヤーを検索する
	//return: 存在していた場合layerのポインタ, ない場合は空ポインタ
	//引数1: レイヤー名
	SharedPointer<Audio::SubmixLayer> Audio::FindLayer(const sgstring& name)
	{
		//Root分岐
		if (name == m_submixRootLayer->m_layerName) return m_submixRootLayer;
		if (name == m_submixMonoralRootLayer->m_layerName) return m_submixRootLayer;
		//検索, あったらreturn
		for (auto& e : m_submixLayers)
			if (name == e->m_layerName) return e;
		//ないので空ポインタ
		return SharedPointer<Audio::SubmixLayer>();
	}

	//----------------------------------------------------------------------------------
	//[SendAudioMessage]
	//メッセージを送信する
	//引数1: message
	inline void Audio::SendAudioMessage(const Detail::Audio::AudioMessage& message)
	{
		AtomicOperation::LockAtomic(m_lockMessages);
		m_messages.emplace_back(message);
		AtomicOperation::UnlockAtomic(m_lockMessages);
	}

	//----------------------------------------------------------------------------------
	//[ChangeVolumeSmoothly]
	//再生音量をtargetまで滑らかに変更する
	//引数1: 変更target
	//引数2: 変更にかける秒数
	inline void Audio::SubmixLayer::ChangeVolumeSmoothly(float target, float seconds)
	{
		using namespace Detail::Audio;

		//Message用
		AudioMessage message((MessageType::SmoothFlag | MessageType::AddSmoothSubmix));
		//現在値
		float volume = getVolume();

		//Messageの添付情報を代入
		message.attachment.smoothSubmix.submixVoice = m_voice;
		message.attachment.smoothSubmix.now = volume;
		message.attachment.smoothSubmix.target = target;
		message.attachment.smoothSubmix.speed = (target - volume) / seconds;
		
		//Message送信
		Audio::SendAudioMessage(message);
	}
}

#endif // !SGFRAMEWORK_HEADER_AUDIO_H_