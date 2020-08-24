/*----------------------------------------------------------------------------------
BGMを扱うAudioAssetMusic class
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_AUDIO_ASSET_MUSIC_HPP_
#define SGFRAMEWORK_HEADER_AUDIO_ASSET_MUSIC_HPP_
#include <xaudio2.h>
#include "../../02_CustomVariable/CustomVariable.hpp"
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
		//BGMを扱うAudioAssetMusic class
		class AudioAssetMusic final : public BaseClass::BaseAudioAsset
		{
		public:
			//friend
			friend class Audio;
			friend class AssetManager;
			template <typename TAsset>
			friend SharedPointer<TAsset> Asset::InstantiateAsset(const sgstring& filePath, bool isRegisterSharedAsset);

			//再生キュー
			struct PlayQueue
			{
				static constexpr byte cPlay = 0x01;				//Playフラグ
				static constexpr byte cPause = 0x02;			//Pauseフラグ
				static constexpr byte cStop = 0x04;				//Stopフラグ
				static constexpr byte cReplay = 0x08;			//Replayフラグ
				static constexpr byte cSeekBegin = 0x10;	//SeekBeginフラグ

				byte state = 0;	//フラグ
			};

			//----------------------------------------------------------------------------------
			//[デストラクタ]
			//アセットの開放を行う
			inline ~AudioAssetMusic() { ReleaseVoice(); CloseBaseAsset(getType(), true); }


			//----------------------------------------------------------------------------------
			//[Open]
			//アセットのロードを行う
			//引数1: 音声が所属するSubmixLayer, default = not monoral root layer
			//引数2: ストリーミングの分割間隔 (引数4 ~ 6の秒数はこの値の倍数である必要がある), default = cDefault (0.2f)
			//引数3: 再生開始秒数, default = cDefault (Begin)
			//引数4: ループ時開始秒数, default = cDefault (Begin)
			//引数5: 再生終了秒数, default = cDefault (End)
			void Open(WeakPointer<Audio::SubmixLayer> submixLayer = Audio::submixRootLayer(), float setStreamingPlaybackInterval = 0.2f,
				float setPlayBeginSeconds = cDefault, float setLoopBeginSeconds = cDefault, float setPlayEndSeconds = cDefault);

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
			//引数1: ループの可否, default = true
			inline void Play(bool isLoop = true)
			{
				//Lockしてキュー追加
				AtomicOperation::LockAtomic(m_lock);
				m_playQueue.state |= PlayQueue::cPlay;
				m_isExitLoop = isLoop ^ true;
				AtomicOperation::UnlockAtomic(m_lock);

				//通知
				m_updateCondition.NotifyOne(m_cFlagPlay);
			}
			//----------------------------------------------------------------------------------
			//[Stop]
			//再生を全て停止する, 実際に停止されるのは1フレーム後となる
			inline void Stop() override
			{
				//Lockしてキュー追加
				AtomicOperation::LockAtomic(m_lock);
				m_playQueue.state |= PlayQueue::cStop;
				AtomicOperation::UnlockAtomic(m_lock);

				//通知
				m_updateCondition.NotifyOne(m_cFlagPlay);
			}
			//----------------------------------------------------------------------------------
			//[Pause]
			//再生中なら一時停止する, 実際に実行されるのは1フレーム後となる
			inline void Pause()
			{
				//Lockしてキュー追加
				AtomicOperation::LockAtomic(m_lock);
				m_playQueue.state |= PlayQueue::cPause;
				AtomicOperation::UnlockAtomic(m_lock);

				//通知
				m_updateCondition.NotifyOne(m_cFlagPlay);
			}
			//----------------------------------------------------------------------------------
			//[Replay]
			//最初から再生される, 実際に実行されるのは1フレーム後となる
			inline void Replay()
			{
				AtomicOperation::LockAtomic(m_lock);
				m_playQueue.state |= PlayQueue::cReplay;
				AtomicOperation::UnlockAtomic(m_lock);

				//通知
				m_updateCondition.NotifyOne(m_cFlagPlay);
			}
			//----------------------------------------------------------------------------------
			//[SeekBegin]
			//再生位置を初期位置に戻す, 実際に実行されるのは1フレーム後となる
			inline void SeekBegin()
			{
				AtomicOperation::LockAtomic(m_lock);
				m_playQueue.state |= PlayQueue::cSeekBegin;
				AtomicOperation::UnlockAtomic(m_lock);

				//通知
				m_updateCondition.NotifyOne(m_cFlagPlay);
			} 
			//----------------------------------------------------------------------------------
			//[ExitLoop]
			//現在の再生でループ再生を終了する
			//引数1: 設定値 (Play, Stop, Replay実行でfalseに初期化)
			inline void ExitLoop(bool isSet) 
			{
				AtomicOperation::LockAtomic(m_lock);
				m_isExitLoop = isSet;
				AtomicOperation::UnlockAtomic(m_lock); 
			}
			//----------------------------------------------------------------------------------
			//[CheckError]
			//return: 再生エラー情報, default-> S_OK
			inline HRESULT CheckError() { return m_callback.errorInfo; }

			//get asset type (read function property)
			SGF_FUNCTION_PROPERTY Asset::AssetType getType() override { return Asset::AssetType::AudioMusic; }

			GetOnlyProperty<PlayQueue> playQueue;						//再生キュー
			GetOnlyProperty<float> streamingPlaybackInterval;			//ストリーミング再生間隔
			GetOnlyProperty<std::atomic_bool> isPlayNow;				//再生中 -> true
			GetOnlyProperty<std::atomic_bool> isFirstFullPlayback;	//1ループ終了でtrue, StopやReplay関数でfalseにリセット
			GetOnlyProperty<std::atomic_bool> isPauseNow;			//一時再生停止中 -> true

		private:
			//コールバック
			struct MusicCallback : public IXAudio2VoiceCallback
			{
				//コンストラクタ
				inline MusicCallback(ConditionVariable::MultipleBit& updateCondition) : errorInfo(S_OK), updateCondition(updateCondition) {}
				
				void STDMETHODCALLTYPE OnVoiceProcessingPassStart(UINT32 BytesRequired) {}
				void STDMETHODCALLTYPE OnVoiceProcessingPassEnd(void) {}
				void STDMETHODCALLTYPE OnStreamEnd(void) {}
				void STDMETHODCALLTYPE OnBufferStart(void *pBufferContext) {}
				void STDMETHODCALLTYPE OnBufferEnd(void *pBufferContext) { updateCondition.NotifyOne(m_cFlagUpdate); }
				void STDMETHODCALLTYPE OnLoopEnd(void *pBufferContext) {}
				void STDMETHODCALLTYPE OnVoiceError(void *pBufferContext, HRESULT Error) { errorInfo = Error; }

				std::atomic<HRESULT> errorInfo;				//ErrorInfo
				ConditionVariable::MultipleBit& updateCondition;	//アップデートイベント
			};

			//----------------------------------------------------------------------------------
			//[コンストラクタ]
			//初期化を行う
			inline AudioAssetMusic() : BaseAudioAsset(), playQueue(m_playQueue),
				streamingPlaybackInterval(m_streamingPlaybackInterval), isPauseNow(m_isPauseNow), 
				m_updateCondition(m_cFlagsAll), isPlayNow(m_isPlayNow), isFirstFullPlayback(m_isFirstFullPlayback) {}

			//----------------------------------------------------------------------------------
			//[PlayUpdate]
			//更新を行う, 別スレッド動作
			void PlayUpdate();
			
			static constexpr uint m_cBufferMax = 2;						//バッファ最大数
			static constexpr uint m_cFlagPlay = 1;							//Play関数通知
			static constexpr uint m_cFlagUpdate = 2;						//コールバックのアップデート通知
			static constexpr uint m_cFlagQuit = 4;							//終了通知
			//All Flag
			static constexpr uint m_cFlagsAll = m_cFlagPlay | m_cFlagUpdate | m_cFlagQuit;

			XAUDIO2_BUFFER m_bufferInfo = {};							//バッファー
			XAUDIO2_VOICE_STATE m_state = {};						//state
			std::thread m_updateThread;										//Update関数スレッド
			MusicCallback m_callback = { m_updateCondition };		//コールバック
			ConditionVariable::MultipleBit m_updateCondition;		//条件変数
			HMMIO m_mmioHandle = nullptr;								//MMIOのハンドル
			uint64 m_useThread = 0;											//割り当てられたスレッドアフィニティ
			PlayQueue m_playQueue;											//キュー
			byte* m_buffers = nullptr;											//再生バッファー
			byte* m_readPoint = nullptr;										//読み込みポイント
			float m_streamingPlaybackInterval = 0.0f;					//ストリーミング再生間隔
			uint m_bufferLength = 0;											//バッファーの長さ
			uint m_readLength = 0;												//読み込んだ長さ
			int m_seek = 0;															//シークしてきた長さ
			int m_beginOffset = 0;												//初期ポイントオフセット
			int m_loopOffset = 0;													//ループ初期オフセット
			int m_beginPoint = 0;													//初期ポイント
			int m_endPoint = 0;													//終了ポイント
			std::atomic_bool m_isPauseNow = false;						//一時再生停止中 -> true
			std::atomic_bool m_isPlayNow = false;						//再生中 -> true
			std::atomic_bool m_isFirstFullPlayback = false;			//1ループ終了でtrue, StopやReplay関数でfalseにリセット
			std::atomic_bool m_lock = false;									//lock
			byte m_useBuffer = 0;												//使用バッファ
			bool m_isExitLoop = false;											//ループ終了フラグ

		private:
			//----------------------------------------------------------------------------------
			//[PlayExecution]
			//実際に再生を行う
			void PlayExecution(bool& isExitLoop);
			//----------------------------------------------------------------------------------
			//[StopExecution]
			//実際に停止を行う
			void StopExecution(bool& isExitLoop);
			//----------------------------------------------------------------------------------
			//[PauseExecution]
			//実際に一時停止を行う
			void PauseExecution();
			//----------------------------------------------------------------------------------
			//[ReplayExecution]
			//実際に再生を行う
			void ReplayExecution(bool& isExitLoop);
			//----------------------------------------------------------------------------------
			//[SeekBeginExecution]
			//実際にシークを行う
			void SeekBeginExecution();
			//----------------------------------------------------------------------------------
			//[ReleaseVoice]
			//音声を開放する
			void ReleaseVoice();
		};
	}
}
#endif // !SGFRAMEWORK_HEADER_AUDIO_ASSET_MUSIC_HPP_