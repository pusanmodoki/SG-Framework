/*----------------------------------------------------------------------------------
BGMを扱うAudioAssetMusic class
	→AudioAssetMusic.cpp
------------------------------------------------------------------------------------*/
#include "../../03_MathAF/MathAF.hpp"
#include "AudioAssetMusic.hpp"
#include "../Audio.hpp"
#include "../../07_Thread/Thread.hpp"

// Framework namespace
namespace SGFramework
{
	//Asset関連を格納したAsset namespace
	namespace Asset
	{
		//----------------------------------------------------------------------------------
		//[Open]
		//アセットのロードを行う
		//引数1: 音声が所属するSubmixLayer,d default = not monoral root layer
		//引数2: ストリーミングの分割間隔 (引数4 ~ 6の秒数はこの値の倍数である必要がある), default = cDefault (0.2f)
		//引数3: 再生開始秒数, default = cDefault (Begin)
		//引数4: ループ時開始秒数, default = cDefault (Begin)
		//引数5: 再生終了秒数, default = cDefault (End)
		void AudioAssetMusic::Open(WeakPointer<Audio::SubmixLayer> submixLayer, float setStreamingPlaybackInterval,
			float setPlayBeginSeconds, float setLoopBeginSeconds, float setPlayEndSeconds)
		{
			if (IS_TRUE(getIsFileOpened())) return;

			//XAudio未初期化で終了
			if (Audio::m_xaudio2 == nullptr)
			{
				setIsOpenFailed(true);
				throw InvalidArgument(L"Error!! AudioAssetMusic->Open", L"XAudio2 == nullptr");
			}
			
			setIsOpenFailed(false);

			WAVEFORMATEXTENSIBLE format = {};
			MMIOINFO mmioinfo = {};
			MMCKINFO riffchunkinfo = {};
			MMCKINFO datachunkinfo = {};
			MMCKINFO mmckinfo = {};
			PCMWAVEFORMAT pcmwf = {};
			MMRESULT mmret = {};
			HRESULT result;

			//mmio形式でファイルオープン
			m_mmioHandle = mmioOpen(const_cast<wchar*>(name().c_str()), &mmioinfo, MMIO_READ);
			if (m_mmioHandle == nullptr)
			{
				setIsOpenFailed(true);
				throw InvalidArgument(L"Error!! AudioAssetMusic->Open", L"mmioOpen Failed");
			}

			//??...
			riffchunkinfo.fccType = mmioFOURCC('W', 'A', 'V', 'E');
			mmret = mmioDescend(m_mmioHandle, &riffchunkinfo, NULL, MMIO_FINDRIFF);
			if (mmret != MMSYSERR_NOERROR)
			{
				setIsOpenFailed(true);
				mmioClose(m_mmioHandle, 0);
				throw InvalidArgument(L"Error!! AudioAssetMusic->Open", L"mmioDescend Failed");
			}

			mmckinfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
			mmret = mmioDescend(m_mmioHandle, &mmckinfo, &riffchunkinfo, MMIO_FINDCHUNK);
			if (mmret != MMSYSERR_NOERROR)
			{
				setIsOpenFailed(true);
				mmioClose(m_mmioHandle, 0);
				throw InvalidArgument(L"Error!! AudioAssetMusic->Open", L"mmioDescend Failed");
			}

			if (mmckinfo.cksize >= sizeof(WAVEFORMATEX))
				mmioRead(m_mmioHandle, (HPSTR)&format.Format, sizeof(format.Format));
			else
			{
				mmioRead(m_mmioHandle, (HPSTR)&pcmwf, sizeof(pcmwf));
				memcpy(&format.Format, &pcmwf, sizeof(pcmwf));
				format.Format.cbSize = 0;
			}
			mmioAscend(m_mmioHandle, &mmckinfo, 0);
			datachunkinfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
			mmret = mmioDescend(m_mmioHandle, &datachunkinfo, &riffchunkinfo, MMIO_FINDCHUNK);
			if (mmret != MMSYSERR_NOERROR)
			{
				setIsOpenFailed(true);
				mmioClose(m_mmioHandle, 0);
				throw InvalidArgument(L"Error!! AudioAssetMusic->Open", L"mmioDescend Failed");
			}

			//ソースボイス作成
			IXAudio2SourceVoice* voice = nullptr;
			result = Audio::m_xaudio2->CreateSourceVoice(&voice, &format.Format, XAUDIO2_VOICE_NOPITCH,
				XAUDIO2_DEFAULT_FREQ_RATIO, &m_callback, &submixLayer->m_sends, nullptr);
			if (FAILED(result))
			{
				setIsOpenFailed(true);
				mmioClose(m_mmioHandle, 0);
				throw InvalidArgument(L"Error!! AudioAssetMusic->Open", L"CreateSourceVoice Failed");
			}
			m_voice.Init(voice, &IXAudio2SourceVoice::DestroyVoice);

			//SeekPoint
			m_beginPoint = mmioSeek(m_mmioHandle, 0, SEEK_CUR);
			//秒数代入
			m_audioSeconds =
				static_cast<float>(((m_beginPoint + mmioSeek(m_mmioHandle, 0, SEEK_END)) / format.Format.nBlockAlign) / format.Format.nSamplesPerSec);
			//最初にシークを戻す
			mmioSeek(m_mmioHandle, m_beginPoint, SEEK_SET);

			//デフォルトか否かで分岐し、デフォルトの場合は0を代入する
			if (IS_FALSE(MathAF::IsMinusSign(setPlayBeginSeconds))) m_playBeginSeconds = setPlayBeginSeconds;
			else m_playBeginSeconds = 0.0f;
			if (IS_FALSE(MathAF::IsMinusSign(setLoopBeginSeconds))) m_loopBeginSeconds = setLoopBeginSeconds;
			else m_loopBeginSeconds = 0.0f;		
			//代入
			m_playEndSeconds = setPlayEndSeconds;
			m_streamingPlaybackInterval = setStreamingPlaybackInterval;
			m_sourceSampleRate = format.Format.nSamplesPerSec;
			m_channels = format.Format.nChannels;
			m_submixLayerID = submixLayer->m_layerID;
		
			//バイト単位オフセット作成
			m_beginOffset = static_cast<int>(static_cast<float>(format.Format.nAvgBytesPerSec) * m_playBeginSeconds);
			m_loopOffset = static_cast<int>(static_cast<float>(format.Format.nAvgBytesPerSec) * m_loopBeginSeconds);
			
			//終了ポイント設定
			if (IS_FALSE(MathAF::IsMinusSign(m_playEndSeconds)))
				m_endPoint = static_cast<int>(static_cast<float>(format.Format.nAvgBytesPerSec) * m_playEndSeconds);
			else m_endPoint = VariableLimit::cIntMax;		

			//BeginOffsetある場合シーク
			if (m_beginOffset)
				m_beginPoint = mmioSeek(m_mmioHandle, m_beginOffset, SEEK_CUR);
			
			//バッファ長さ指定
			m_bufferLength = format.Format.nAvgBytesPerSec / static_cast<ulong>(1.0f / setStreamingPlaybackInterval);
			//new buffer
			m_buffers = new byte[m_bufferLength * m_cBufferMax];
			//初期読み込みポイント設定
			m_readPoint = m_buffers;
			//read
			m_readLength = mmioRead(m_mmioHandle, reinterpret_cast<HPSTR>(m_readPoint), m_bufferLength);
			//シーク加算
			m_seek += m_readLength;

			//再生情報を設定
			m_bufferInfo.AudioBytes = m_readLength;
			m_bufferInfo.pAudioData = m_readPoint;
			m_bufferInfo.PlayBegin = 0;
			m_bufferInfo.PlayLength = m_readLength / format.Format.nBlockAlign;
			if (m_readLength >= m_bufferLength && m_seek < m_endPoint) m_bufferInfo.Flags = 0;
			else m_bufferInfo.Flags = XAUDIO2_END_OF_STREAM;

			//BaseAsset
			OpenBaseAsset();
			
			//Update別スレッドで開始
			m_updateThread = std::thread([this]() { this->PlayUpdate(); });
		}

		//----------------------------------------------------------------------------------
		//[Reload]
		//アセットのリロードを行う
		void AudioAssetMusic::Reload()
		{
			if (IS_FALSE(getIsFileOpened())) return;
		
			//Openに必要な情報を記録
			sgstring path = name;
			const auto& submixLayer = Audio::getLayer(m_submixLayerID);
			float setStreamingPlaybackInterval = m_streamingPlaybackInterval;
			float setPlayBeginSeconds = m_playBeginSeconds;
			float setLoopBeginSeconds = m_loopBeginSeconds;
			float setPlayEndSeconds = m_playEndSeconds;
			
			//Release
			Close();

			//Open
			Open(submixLayer, setStreamingPlaybackInterval, setPlayBeginSeconds, setLoopBeginSeconds, setPlayEndSeconds);
		}

		//----------------------------------------------------------------------------------
		//[Close]
		//ファイルを閉じる
		void AudioAssetMusic::Close()
		{
			if (IS_FALSE(getIsFileOpened())) return;
		
			//Release
			ReleaseVoice();
			//BaseAsset
			CloseBaseAsset(getType(), false);
		}


		//----------------------------------------------------------------------------------
		//[PlayUpdate]
		//更新を行う, 別スレッド動作
		void AudioAssetMusic::PlayUpdate()
		{
			//UseThread未設定なら使用スレッドを割り当て & 設定
			if (m_useThread == 0)
				m_useThread = Thread::AutoThreadAllocation(m_updateThread.native_handle());
			//割り当て済みならそのスレッドを指定
			else
				Thread::ThreadAllocation(m_updateThread.native_handle(), m_useThread);

			//登録
			Thread::RegisterRunningFrameworkThread();

			//再生ループ
			while (true)
			{
				//通知待機
				uint waitResult = m_updateCondition.Wait();
				//再生フラグ
				bool isPlayStart = false;

				//アップデート終了ならループ抜ける
				if (waitResult & m_cFlagQuit) break;

				uint state = 0;
				bool isExitLoop = false;
				{
					//Lock 
					AtomicOperation::LockAtomic(m_lock);

					//キュー, flagコピー 
					state = m_playQueue.state;
					m_playQueue.state = 0;
					isExitLoop = m_isExitLoop;

					//Unlock 
					AtomicOperation::UnlockAtomic(m_lock);
				}

				//Seek
				if (state & PlayQueue::cSeekBegin)
					SeekBeginExecution();

				//Replay
				if (state & PlayQueue::cReplay)
				{
					ReplayExecution(isExitLoop);
					isPlayStart = true;
				}
				//Play
				else if (state & PlayQueue::cPlay)
				{
					PlayExecution(isExitLoop);
					isPlayStart = true;
				}

				//Stop
				if (state & PlayQueue::cStop)
					StopExecution(isExitLoop);
				//Pause
				else if (state & PlayQueue::cPause)
					PauseExecution();


				//アップデートイベントあり
				if ((waitResult & m_cFlagUpdate) || IS_TRUE(isPlayStart))
				{
					//ステート取得
					m_voice->GetState(&m_state);

					//バッファーに詰め込めるだけ詰め込む
					while (m_state.BuffersQueued < m_cBufferMax)
					{
						//読み込みポイント再設定
						m_readPoint = m_buffers + m_bufferLength * m_useBuffer;
						//次回使用バッファ設定
						m_useBuffer = (m_useBuffer + 1) % m_cBufferMax;
						//Read
						m_readLength = mmioRead(m_mmioHandle, reinterpret_cast<HPSTR>(m_readPoint), m_bufferLength);

						//Seek加算
						m_seek += m_readLength;
						//終了条件に必要
						m_state.BuffersQueued++;

						//Flag設定
						m_bufferInfo.Flags = XAUDIO2_END_OF_STREAM;
						//まだ読み込めるなら0にFlag設定
						if (m_readLength >= m_bufferLength)
						{
							if (IS_TRUE(m_isFirstFullPlayback.load()))
							{
								if (m_seek < m_endPoint - m_loopOffset)
									m_bufferInfo.Flags = 0;
							}
							else
							{
								if (m_seek + m_beginOffset < m_endPoint)
									m_bufferInfo.Flags = 0;
							}
						}

						//バッファー設定
						m_bufferInfo.AudioBytes = m_readLength;
						m_bufferInfo.pAudioData = m_readPoint;
						//バッファー追加
						m_voice->SubmitSourceBuffer(&m_bufferInfo);

						//Flagが終了設定なら
						if (m_bufferInfo.Flags & XAUDIO2_END_OF_STREAM)
						{
							//ループ終了フラグありなら止めて終了
							if (IS_TRUE(isExitLoop))
							{
								StopExecution(isExitLoop);
								continue;
							}

							//ループ2回目か否かで分岐し、ループポイントまでシーク
							if (IS_TRUE(m_isFirstFullPlayback.load()))
								mmioSeek(m_mmioHandle, -m_seek, SEEK_CUR);
							else
							{
								mmioSeek(m_mmioHandle, (-m_seek) - m_beginOffset + m_loopOffset, SEEK_CUR);
								//ループ完了
								AtomicOperation::Init(m_isFirstFullPlayback, true);
							}

							//シーク初期化
							m_seek = 0;
						}
					}
				}


				{
					//Lock 
					AtomicOperation::LockAtomic(m_lock);

					//フラグコピー 
					m_isExitLoop = isExitLoop;
					
					//Unlock 
					AtomicOperation::UnlockAtomic(m_lock);
				}
			}

			//登録解除
			Thread::UnRegisterRunningFrameworkThread();
		}

		//----------------------------------------------------------------------------------
		//[PlayExecution]
		//実際に再生を行う
		void AudioAssetMusic::PlayExecution(bool& isExitLoop)
		{
			//再生していない or ポーズ中
			if (IS_FALSE(m_isPlayNow.load()) | m_isPauseNow.load())
			{
				//ポーズ中でないならバッファ追加
				if (IS_FALSE(m_isPauseNow.load()))
				{
					m_voice->GetState(&m_state);
					if (m_state.BuffersQueued == 0)
					{
						m_voice->SubmitSourceBuffer(&m_bufferInfo);
					}
				}
				//再生
				m_voice->Start();
				//フラグ設定
				AtomicOperation::Init(m_isPlayNow, true);
				AtomicOperation::Init(m_isPauseNow, false);
			}
		}

		//----------------------------------------------------------------------------------
		//[StopExecution]
		//実際に停止を行う
		void AudioAssetMusic::StopExecution(bool& isExitLoop)
		{
			//再生中
			if (IS_TRUE(m_isPlayNow.load()))
			{
				//ポーズ中でないなら停止
				if (IS_FALSE(m_isPauseNow.load())) 
					m_voice->Stop();
				
				//フラグ初期化
				AtomicOperation::Init(m_isPlayNow, false);
				AtomicOperation::Init(m_isPauseNow, false);
				AtomicOperation::Init(m_isFirstFullPlayback, false);
				isExitLoop = false;
				//バッファクリア
				m_voice->FlushSourceBuffers();

				//初期ポイントへシーク
				mmioSeek(m_mmioHandle, m_beginPoint, SEEK_SET);
				//シーク初期化
				m_seek = 0;
			}
		}

		//----------------------------------------------------------------------------------
		//[PauseExecution]
		//実際に一時停止を行う
		void AudioAssetMusic::PauseExecution()
		{
			//再生中 & ポーズ中ではない
			if (m_isPlayNow.load() & IS_FALSE(m_isPauseNow.load()))
			{
				//停止
				m_voice->Stop();
				//フラグ設定
				AtomicOperation::Init(m_isPauseNow, true);
			}
		}

		//----------------------------------------------------------------------------------
		//[ReplayExecution]
		//実際に再生を行う
		void AudioAssetMusic::ReplayExecution(bool& isExitLoop)
		{
			//停止しバッファクリア
			m_voice->Stop();
			m_voice->FlushSourceBuffers();

			//初期ポイントへシーク, シーク変数初期化
			mmioSeek(m_mmioHandle, m_beginPoint, SEEK_SET);
			m_seek = 0;

			//再生
			m_voice->Start();

			//フラグ初期化
			AtomicOperation::Init(m_isPlayNow, true);
			AtomicOperation::Init(m_isPauseNow, false);
			AtomicOperation::Init(m_isFirstFullPlayback, false);
			isExitLoop = false;
		}

		//----------------------------------------------------------------------------------
		//[SeekBeginExecution]
		//実際にシークを行う
		void AudioAssetMusic::SeekBeginExecution()
		{
			//シーク, シーク変数初期化
			mmioSeek(m_mmioHandle, m_beginPoint, SEEK_SET);
			m_seek = 0;
		}

		//----------------------------------------------------------------------------------
		//[ReleaseVoice]
		//音声を開放する
		//引数1: getPack()->Release?, default = false
		void AudioAssetMusic::ReleaseVoice()
		{
			{
				AtomicOperation::LockGuard guard(m_lock);
				if (m_updateThread.joinable())
				{
					//アップデート通知
					m_updateCondition.NotifyOne(m_cFlagQuit);
					//アップデート実行スレッド終了待機
					m_updateThread.join();
				}
				else
					return;
			}

			if (m_voice == nullptr || Audio::m_masteringVoice == nullptr)
				return;

			//ボイス削除
			m_voice.Release(true);
			//メモリ解放
			delete[] m_buffers;
			//MMIOハンドルを閉じる
			mmioClose(m_mmioHandle, 0);
			
			//初期値にすべて初期化
			m_playQueue.state = 0;
			m_audioSeconds = 0.0f;
			m_playBeginSeconds = 0.0f;
			m_loopBeginSeconds = 0.0f;
			m_playEndSeconds = 0.0f;
			m_sourceSampleRate = 0;
			m_channels = 0;
			m_submixLayerID = 0;
			m_isPauseNow = false;
			m_isPlayNow = false;
			m_isFirstFullPlayback = false;
			ZeroMemory(&m_bufferInfo, sizeof(m_bufferInfo));
			ZeroMemory(&m_state, sizeof(m_state));
			m_callback.errorInfo = 0;
			m_mmioHandle = nullptr;
			m_buffers = nullptr;
			m_readPoint = nullptr;
			m_bufferLength = 0;
			m_readLength = 0;
			m_seek = 0;
			m_beginOffset = 0;
			m_loopOffset = 0;
			m_endPoint = 0;
			m_useBuffer = 0;
			m_streamingPlaybackInterval = 0.0f;
			m_isExitLoop = false;
		}
	}
}