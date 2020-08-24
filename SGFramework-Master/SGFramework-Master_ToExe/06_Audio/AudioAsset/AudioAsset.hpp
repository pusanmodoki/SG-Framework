/*----------------------------------------------------------------------------------
BaseAudioAsset系クラスのベースとなるBaseAudioAsset class
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_AUDIO_ASSET_HPP_
#define SGFRAMEWORK_HEADER_AUDIO_ASSET_HPP_
#include <xaudio2.h>
#include "../Audio.hpp"
#include "../../04_Asset/Asset/BaseAsset.hpp"

#ifdef _XBOX //Big-Endian
#define fourccRIFF 'RIFF'
#define fourccDATA 'data'
#define fourccFMT 'fmt '
#define fourccWAVE 'WAVE'
#define fourccXWMA 'XWMA'
#define fourccDPDS 'dpds'
#endif
#ifndef _XBOX //Little-Endian
#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'
#endif

// Framework namespace
namespace SGFramework
{
	//Base Classes
	namespace BaseClass
	{
		//AudioAsset系クラスのベースとなるBaseAudioAsset class
		class BaseAudioAsset;
		//AudioAssetSE系クラスのベースとなるBaseAudioSEAsset class
		class BaseAudioSEAsset;


		//BaseAudioAsset系クラスのベースとなるBaseAudioAsset class
		class BaseAudioAsset : public BaseClass::BaseAsset
		{
		public:
			//friend
			friend class SGFramework::Audio;
			//デフォルト値
			static constexpr float cDefault = -1;

			//----------------------------------------------------------------------------------
			//[Stop]
			//再生を全て停止する, 実際に停止されるのは1フレーム後となる
			virtual void Stop() = 0;

			//----------------------------------------------------------------------------------
			//[ChangeSpeedSmoothly]
			//再生スピードをtargetまで滑らかに変更する
			//引数1: 変更target
			//引数2: 変更にかける秒数
			//引数3: 終了後停止するか否か, default = false
			inline void ChangeSpeedSmoothly(float target, float seconds, bool isStop = false)
			{
				using namespace Detail::Audio;

				//現在の値
				float speed = getPlayBackSpeed();
				//同じなら終了
				if (target == speed) return;

				//Message用
				AudioMessage message(MessageType::SmoothFlag | MessageType::AddSmoothAudio);

				//Messageの添付情報を代入
				message.attachment.smoothAudio.asset = m_thisPointer.DynamicCast<BaseAudioAsset>();
				message.attachment.smoothAudio.smoothType = SmoothType::Speed;
				message.attachment.smoothAudio.now = speed;
				message.attachment.smoothAudio.target = target;
				message.attachment.smoothAudio.speed = (target - speed) / seconds;
				message.attachment.smoothAudio.isStop = isStop;

				//Message送信
				Audio::SendAudioMessage(message);
			}
			//----------------------------------------------------------------------------------
			//[ChangeVolumeSmoothly]
			//再生音量をtargetまで滑らかに変更する
			//引数1: 変更target
			//引数2: 変更にかける秒数
			//引数3: 終了後停止するか否か, default = false
			inline void ChangeVolumeSmoothly(float target, float seconds, bool isStop = false)
			{
				using namespace Detail::Audio;

				//現在の値
				float volume = getVolume();
				//同じなら終了
				if (target == volume) return;

				//Message用
				AudioMessage message(MessageType::SmoothFlag | MessageType::AddSmoothAudio);
				
				//Messageの添付情報を代入
				message.attachment.smoothAudio.asset = m_thisPointer.DynamicCast<BaseAudioAsset>();
				message.attachment.smoothAudio.smoothType = SmoothType::Volume;
				message.attachment.smoothAudio.now = volume;
				message.attachment.smoothAudio.target = target;
				message.attachment.smoothAudio.speed = (target - volume) / seconds;
				message.attachment.smoothAudio.isStop = isStop;

				//Message送信
				Audio::SendAudioMessage(message);
			}
			
			//play back speed (get function property)
			SGF_FUNCTION_PROPERTY float getPlayBackSpeed() { float ret = 0.0f; m_voice->GetFrequencyRatio(&ret); return ret; }
			//play back speed (set function property)
			//argument 1: set value (normal 0.0f ~ 1.0f)
			SGF_FUNCTION_PROPERTY virtual void setPlayBackSpeed(float magnification) { m_voice->SetFrequencyRatio(magnification); }
		
			//play volume (get function property)
			SGF_FUNCTION_PROPERTY float getVolume() { float ret = 0.0f; m_voice->GetVolume(&ret); return ret; }
			//play volume (set function property)
			//argument 1: set value (normal 0.0f ~ 1.0f)
			SGF_FUNCTION_PROPERTY virtual void setVolume(float magnification) { m_voice->SetVolume(magnification); }
			

			//voice pointer (get function property)
			SGF_FUNCTION_PROPERTY IXAudio2SourceVoice* getVoice() const { return m_voice(); }

			GetOnlyProperty<float> audioSeconds;											//ファイルの秒数
			GetOnlyProperty<float> playBeginSeconds;										//再生開始秒数
			GetOnlyProperty<float> loopBeginSeconds;										//ループ時開始秒数
			GetOnlyProperty<float> playEndSeconds;										//再生終了秒数
			GetOnlyProperty<uint> sourceSampleRate;										//ソースのサンプルレート (再生レートはAudioのsampleRate)
			GetOnlyProperty<uint> channels;													//ソースのチャンネル数(再生レートはSubmixLayerのchannels)
			GetOnlyProperty<uint> submixLayerID;											//自身の所属するサブミックスレイヤーID

		protected:
			//コンストラクタ, ゲッターに値代入しまくる
			inline BaseAudioAsset() : BaseAsset(), audioSeconds(m_audioSeconds), 
				playBeginSeconds(m_playBeginSeconds), loopBeginSeconds(m_loopBeginSeconds),
				playEndSeconds(m_playEndSeconds), sourceSampleRate(m_sourceSampleRate), 
				channels(m_channels), submixLayerID(m_submixLayerID) {}
		
			static constexpr uint m_cBaseFlagPublicState = 0;
			static constexpr uint m_cBaseFlagPrivateState = 1;
			
			//メインボイス
			ComWearFunction<IXAudio2SourceVoice> m_voice;	
			float m_audioSeconds = 0.0f;								//ファイルの秒数
			float m_playBeginSeconds = 0.0f;						//再生開始秒数
			float m_loopBeginSeconds = 0.0f;						//ループ時開始秒数
			float m_playEndSeconds = 0.0f;							//再生終了秒数
			uint m_sourceSampleRate = 0;							//ソースのサンプルレート (再生レートはAudioのsampleRate)
			uint m_channels = 0;											//ソースのチャンネル数(再生レートはSubmixLayerのchannels)
			uint m_submixLayerID = 0;								//自身の所属するサブミックスレイヤーID
		};


		//AudioAssetSE系クラスのベースとなるBaseAudioSEAsset class
		class BaseAudioSEAsset : public BaseAudioAsset
		{
		public:
			//play back speed (set function property)
			//argument 1: set value (normal 0.0f ~ 1.0f)
			SGF_FUNCTION_PROPERTY void setPlayBackSpeed(float magnification) override
			{
				m_voice->SetFrequencyRatio(magnification);
				for (auto& e : m_subVoices)
					e->SetFrequencyRatio(magnification);
			}

			//play volume (set function property)
			//argument 1: set value (normal 0.0f ~ 1.0f)
			SGF_FUNCTION_PROPERTY void setVolume(float magnification) override
			{
				m_voice->SetVolume(magnification);
				for (auto& e : m_subVoices)
					e->SetVolume(magnification);
			}
			
			//sub voice pointer (get function property)
			//argument 1: index (max = getNumSubVoices)
			SGF_FUNCTION_PROPERTY IXAudio2SourceVoice* getSubVoice(uint index) const { return m_subVoices[index](); }
			//num sub voices (get function property)
			SGF_FUNCTION_PROPERTY uint getNumSubVoices() const { return static_cast<uint>(m_subVoices.size()); }
			//sub voice begin iterator (get function property)
			SGF_FUNCTION_PROPERTY auto getSubVoiceBegin() const { return m_subVoices.begin(); }
			//sub voice end iterator (get function property)
			SGF_FUNCTION_PROPERTY auto getSubVoiceEnd() const { return m_subVoices.end(); }

			//ソースの数 (最大同時再生数)
			GetOnlyProperty<uint> amountOfSource;
			//再生間隔
			GetOnlyProperty<float> playbackInterval;

		protected:
			//[コンストラクタ]
			inline BaseAudioSEAsset() : BaseAudioAsset(), 
				amountOfSource(m_amountOfSource), playbackInterval(m_playbackInterval) {}

			//----------------------------------------------------------------------------------
			//[LoadFile]
			//アセットのロードを行う
			//引数1: 音声が所属するSubmixLayer, default = not monoral root layer
			//引数2: ソースの数 (最大同時再生数)
			//引数3: 再生間隔
			//引数4: 再生開始秒数, default = cDefault (Begin)
			//引数5: ループ時開始秒数, default = cDefault (Begin)
			//引数6: 再生終了秒数, default = cDefault (End)
			void LoadFile(const WeakPointer<Audio::SubmixLayer>& submixLayer = Audio::submixRootLayer(),
				uint amountOfSource = 1, float playbackInterval = 0.0f, float setPlayBeginSeconds = cDefault,
				float setLoopBeginSeconds = cDefault, float setPlayEndSeconds = cDefault);
			//----------------------------------------------------------------------------------
			//[ReleaseBaseVoice]
			//音声を開放する
			void ReleaseBaseVoice();

			//----------------------------------------------------------------------------------
			//[FindChunk関数]
			//Chunkを探す,サンプルからいただいた
			bool FindChunk(const HANDLE hFile, const DWORD fourcc, DWORD & dwChunkSize, DWORD & dwChunkDataPosition);
			//----------------------------------------------------------------------------------
			//[ReadChunkData関数]
			//Chunkを読み込む,サンプルからいただいた
			bool ReadChunkData(const HANDLE hFile, void * buffer, const DWORD buffersize, const DWORD bufferoffset);

			//SE用のサブボイス
			std::vector<ComWearFunction<IXAudio2SourceVoice>> m_subVoices;
			//再生バッファー
			ComWearDelete<byte> m_buffer = nullptr;
			//X3DAudio用バッファ
			ComWearDelete<float> m_x3dBuffer = nullptr;
			XAUDIO2_BUFFER m_bufferInfo = {};			//バッファー
			XAUDIO2_VOICE_STATE m_state = {};		//state
			float m_playbackInterval = 0.0f;					//再生間隔
			float m_playElapased = 0.0f;						//再生間隔計測用
			uint m_amountOfSource = 0;						//ソースの数 (最大同時再生数)
			int m_useVoice = -1;									//使用ボイスカウンタ
			std::atomic_bool m_lock = false;					//lock
		};
	}
}
#endif // !SGFRAMEWORK_HEADER_AUDIO_ASSET_HPP_