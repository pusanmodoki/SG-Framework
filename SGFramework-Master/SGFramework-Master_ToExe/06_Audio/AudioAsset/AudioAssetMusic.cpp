/*----------------------------------------------------------------------------------
BGM������AudioAssetMusic class
	��AudioAssetMusic.cpp
------------------------------------------------------------------------------------*/
#include "../../03_MathAF/MathAF.hpp"
#include "AudioAssetMusic.hpp"
#include "../Audio.hpp"
#include "../../07_Thread/Thread.hpp"

// Framework namespace
namespace SGFramework
{
	//Asset�֘A���i�[����Asset namespace
	namespace Asset
	{
		//----------------------------------------------------------------------------------
		//[Open]
		//�A�Z�b�g�̃��[�h���s��
		//����1: ��������������SubmixLayer,d default = not monoral root layer
		//����2: �X�g���[�~���O�̕����Ԋu (����4 ~ 6�̕b���͂��̒l�̔{���ł���K�v������), default = cDefault (0.2f)
		//����3: �Đ��J�n�b��, default = cDefault (Begin)
		//����4: ���[�v���J�n�b��, default = cDefault (Begin)
		//����5: �Đ��I���b��, default = cDefault (End)
		void AudioAssetMusic::Open(WeakPointer<Audio::SubmixLayer> submixLayer, float setStreamingPlaybackInterval,
			float setPlayBeginSeconds, float setLoopBeginSeconds, float setPlayEndSeconds)
		{
			if (IS_TRUE(getIsFileOpened())) return;

			//XAudio���������ŏI��
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

			//mmio�`���Ńt�@�C���I�[�v��
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

			//�\�[�X�{�C�X�쐬
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
			//�b�����
			m_audioSeconds =
				static_cast<float>(((m_beginPoint + mmioSeek(m_mmioHandle, 0, SEEK_END)) / format.Format.nBlockAlign) / format.Format.nSamplesPerSec);
			//�ŏ��ɃV�[�N��߂�
			mmioSeek(m_mmioHandle, m_beginPoint, SEEK_SET);

			//�f�t�H���g���ۂ��ŕ��򂵁A�f�t�H���g�̏ꍇ��0��������
			if (IS_FALSE(MathAF::IsMinusSign(setPlayBeginSeconds))) m_playBeginSeconds = setPlayBeginSeconds;
			else m_playBeginSeconds = 0.0f;
			if (IS_FALSE(MathAF::IsMinusSign(setLoopBeginSeconds))) m_loopBeginSeconds = setLoopBeginSeconds;
			else m_loopBeginSeconds = 0.0f;		
			//���
			m_playEndSeconds = setPlayEndSeconds;
			m_streamingPlaybackInterval = setStreamingPlaybackInterval;
			m_sourceSampleRate = format.Format.nSamplesPerSec;
			m_channels = format.Format.nChannels;
			m_submixLayerID = submixLayer->m_layerID;
		
			//�o�C�g�P�ʃI�t�Z�b�g�쐬
			m_beginOffset = static_cast<int>(static_cast<float>(format.Format.nAvgBytesPerSec) * m_playBeginSeconds);
			m_loopOffset = static_cast<int>(static_cast<float>(format.Format.nAvgBytesPerSec) * m_loopBeginSeconds);
			
			//�I���|�C���g�ݒ�
			if (IS_FALSE(MathAF::IsMinusSign(m_playEndSeconds)))
				m_endPoint = static_cast<int>(static_cast<float>(format.Format.nAvgBytesPerSec) * m_playEndSeconds);
			else m_endPoint = VariableLimit::cIntMax;		

			//BeginOffset����ꍇ�V�[�N
			if (m_beginOffset)
				m_beginPoint = mmioSeek(m_mmioHandle, m_beginOffset, SEEK_CUR);
			
			//�o�b�t�@�����w��
			m_bufferLength = format.Format.nAvgBytesPerSec / static_cast<ulong>(1.0f / setStreamingPlaybackInterval);
			//new buffer
			m_buffers = new byte[m_bufferLength * m_cBufferMax];
			//�����ǂݍ��݃|�C���g�ݒ�
			m_readPoint = m_buffers;
			//read
			m_readLength = mmioRead(m_mmioHandle, reinterpret_cast<HPSTR>(m_readPoint), m_bufferLength);
			//�V�[�N���Z
			m_seek += m_readLength;

			//�Đ�����ݒ�
			m_bufferInfo.AudioBytes = m_readLength;
			m_bufferInfo.pAudioData = m_readPoint;
			m_bufferInfo.PlayBegin = 0;
			m_bufferInfo.PlayLength = m_readLength / format.Format.nBlockAlign;
			if (m_readLength >= m_bufferLength && m_seek < m_endPoint) m_bufferInfo.Flags = 0;
			else m_bufferInfo.Flags = XAUDIO2_END_OF_STREAM;

			//BaseAsset
			OpenBaseAsset();
			
			//Update�ʃX���b�h�ŊJ�n
			m_updateThread = std::thread([this]() { this->PlayUpdate(); });
		}

		//----------------------------------------------------------------------------------
		//[Reload]
		//�A�Z�b�g�̃����[�h���s��
		void AudioAssetMusic::Reload()
		{
			if (IS_FALSE(getIsFileOpened())) return;
		
			//Open�ɕK�v�ȏ����L�^
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
		//�t�@�C�������
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
		//�X�V���s��, �ʃX���b�h����
		void AudioAssetMusic::PlayUpdate()
		{
			//UseThread���ݒ�Ȃ�g�p�X���b�h�����蓖�� & �ݒ�
			if (m_useThread == 0)
				m_useThread = Thread::AutoThreadAllocation(m_updateThread.native_handle());
			//���蓖�čς݂Ȃ炻�̃X���b�h���w��
			else
				Thread::ThreadAllocation(m_updateThread.native_handle(), m_useThread);

			//�o�^
			Thread::RegisterRunningFrameworkThread();

			//�Đ����[�v
			while (true)
			{
				//�ʒm�ҋ@
				uint waitResult = m_updateCondition.Wait();
				//�Đ��t���O
				bool isPlayStart = false;

				//�A�b�v�f�[�g�I���Ȃ烋�[�v������
				if (waitResult & m_cFlagQuit) break;

				uint state = 0;
				bool isExitLoop = false;
				{
					//Lock 
					AtomicOperation::LockAtomic(m_lock);

					//�L���[, flag�R�s�[ 
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


				//�A�b�v�f�[�g�C�x���g����
				if ((waitResult & m_cFlagUpdate) || IS_TRUE(isPlayStart))
				{
					//�X�e�[�g�擾
					m_voice->GetState(&m_state);

					//�o�b�t�@�[�ɋl�ߍ��߂邾���l�ߍ���
					while (m_state.BuffersQueued < m_cBufferMax)
					{
						//�ǂݍ��݃|�C���g�Đݒ�
						m_readPoint = m_buffers + m_bufferLength * m_useBuffer;
						//����g�p�o�b�t�@�ݒ�
						m_useBuffer = (m_useBuffer + 1) % m_cBufferMax;
						//Read
						m_readLength = mmioRead(m_mmioHandle, reinterpret_cast<HPSTR>(m_readPoint), m_bufferLength);

						//Seek���Z
						m_seek += m_readLength;
						//�I�������ɕK�v
						m_state.BuffersQueued++;

						//Flag�ݒ�
						m_bufferInfo.Flags = XAUDIO2_END_OF_STREAM;
						//�܂��ǂݍ��߂�Ȃ�0��Flag�ݒ�
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

						//�o�b�t�@�[�ݒ�
						m_bufferInfo.AudioBytes = m_readLength;
						m_bufferInfo.pAudioData = m_readPoint;
						//�o�b�t�@�[�ǉ�
						m_voice->SubmitSourceBuffer(&m_bufferInfo);

						//Flag���I���ݒ�Ȃ�
						if (m_bufferInfo.Flags & XAUDIO2_END_OF_STREAM)
						{
							//���[�v�I���t���O����Ȃ�~�߂ďI��
							if (IS_TRUE(isExitLoop))
							{
								StopExecution(isExitLoop);
								continue;
							}

							//���[�v2��ڂ��ۂ��ŕ��򂵁A���[�v�|�C���g�܂ŃV�[�N
							if (IS_TRUE(m_isFirstFullPlayback.load()))
								mmioSeek(m_mmioHandle, -m_seek, SEEK_CUR);
							else
							{
								mmioSeek(m_mmioHandle, (-m_seek) - m_beginOffset + m_loopOffset, SEEK_CUR);
								//���[�v����
								AtomicOperation::Init(m_isFirstFullPlayback, true);
							}

							//�V�[�N������
							m_seek = 0;
						}
					}
				}


				{
					//Lock 
					AtomicOperation::LockAtomic(m_lock);

					//�t���O�R�s�[ 
					m_isExitLoop = isExitLoop;
					
					//Unlock 
					AtomicOperation::UnlockAtomic(m_lock);
				}
			}

			//�o�^����
			Thread::UnRegisterRunningFrameworkThread();
		}

		//----------------------------------------------------------------------------------
		//[PlayExecution]
		//���ۂɍĐ����s��
		void AudioAssetMusic::PlayExecution(bool& isExitLoop)
		{
			//�Đ����Ă��Ȃ� or �|�[�Y��
			if (IS_FALSE(m_isPlayNow.load()) | m_isPauseNow.load())
			{
				//�|�[�Y���łȂ��Ȃ�o�b�t�@�ǉ�
				if (IS_FALSE(m_isPauseNow.load()))
				{
					m_voice->GetState(&m_state);
					if (m_state.BuffersQueued == 0)
					{
						m_voice->SubmitSourceBuffer(&m_bufferInfo);
					}
				}
				//�Đ�
				m_voice->Start();
				//�t���O�ݒ�
				AtomicOperation::Init(m_isPlayNow, true);
				AtomicOperation::Init(m_isPauseNow, false);
			}
		}

		//----------------------------------------------------------------------------------
		//[StopExecution]
		//���ۂɒ�~���s��
		void AudioAssetMusic::StopExecution(bool& isExitLoop)
		{
			//�Đ���
			if (IS_TRUE(m_isPlayNow.load()))
			{
				//�|�[�Y���łȂ��Ȃ��~
				if (IS_FALSE(m_isPauseNow.load())) 
					m_voice->Stop();
				
				//�t���O������
				AtomicOperation::Init(m_isPlayNow, false);
				AtomicOperation::Init(m_isPauseNow, false);
				AtomicOperation::Init(m_isFirstFullPlayback, false);
				isExitLoop = false;
				//�o�b�t�@�N���A
				m_voice->FlushSourceBuffers();

				//�����|�C���g�փV�[�N
				mmioSeek(m_mmioHandle, m_beginPoint, SEEK_SET);
				//�V�[�N������
				m_seek = 0;
			}
		}

		//----------------------------------------------------------------------------------
		//[PauseExecution]
		//���ۂɈꎞ��~���s��
		void AudioAssetMusic::PauseExecution()
		{
			//�Đ��� & �|�[�Y���ł͂Ȃ�
			if (m_isPlayNow.load() & IS_FALSE(m_isPauseNow.load()))
			{
				//��~
				m_voice->Stop();
				//�t���O�ݒ�
				AtomicOperation::Init(m_isPauseNow, true);
			}
		}

		//----------------------------------------------------------------------------------
		//[ReplayExecution]
		//���ۂɍĐ����s��
		void AudioAssetMusic::ReplayExecution(bool& isExitLoop)
		{
			//��~���o�b�t�@�N���A
			m_voice->Stop();
			m_voice->FlushSourceBuffers();

			//�����|�C���g�փV�[�N, �V�[�N�ϐ�������
			mmioSeek(m_mmioHandle, m_beginPoint, SEEK_SET);
			m_seek = 0;

			//�Đ�
			m_voice->Start();

			//�t���O������
			AtomicOperation::Init(m_isPlayNow, true);
			AtomicOperation::Init(m_isPauseNow, false);
			AtomicOperation::Init(m_isFirstFullPlayback, false);
			isExitLoop = false;
		}

		//----------------------------------------------------------------------------------
		//[SeekBeginExecution]
		//���ۂɃV�[�N���s��
		void AudioAssetMusic::SeekBeginExecution()
		{
			//�V�[�N, �V�[�N�ϐ�������
			mmioSeek(m_mmioHandle, m_beginPoint, SEEK_SET);
			m_seek = 0;
		}

		//----------------------------------------------------------------------------------
		//[ReleaseVoice]
		//�������J������
		//����1: getPack()->Release?, default = false
		void AudioAssetMusic::ReleaseVoice()
		{
			{
				AtomicOperation::LockGuard guard(m_lock);
				if (m_updateThread.joinable())
				{
					//�A�b�v�f�[�g�ʒm
					m_updateCondition.NotifyOne(m_cFlagQuit);
					//�A�b�v�f�[�g���s�X���b�h�I���ҋ@
					m_updateThread.join();
				}
				else
					return;
			}

			if (m_voice == nullptr || Audio::m_masteringVoice == nullptr)
				return;

			//�{�C�X�폜
			m_voice.Release(true);
			//���������
			delete[] m_buffers;
			//MMIO�n���h�������
			mmioClose(m_mmioHandle, 0);
			
			//�����l�ɂ��ׂď�����
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