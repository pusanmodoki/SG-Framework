/*----------------------------------------------------------------------------------
BGM������AudioAssetMusic class
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

	//Asset�֘A���i�[����Asset namespace
	namespace Asset
	{
		//BGM������AudioAssetMusic class
		class AudioAssetMusic final : public BaseClass::BaseAudioAsset
		{
		public:
			//friend
			friend class Audio;
			friend class AssetManager;
			template <typename TAsset>
			friend SharedPointer<TAsset> Asset::InstantiateAsset(const sgstring& filePath, bool isRegisterSharedAsset);

			//�Đ��L���[
			struct PlayQueue
			{
				static constexpr byte cPlay = 0x01;				//Play�t���O
				static constexpr byte cPause = 0x02;			//Pause�t���O
				static constexpr byte cStop = 0x04;				//Stop�t���O
				static constexpr byte cReplay = 0x08;			//Replay�t���O
				static constexpr byte cSeekBegin = 0x10;	//SeekBegin�t���O

				byte state = 0;	//�t���O
			};

			//----------------------------------------------------------------------------------
			//[�f�X�g���N�^]
			//�A�Z�b�g�̊J�����s��
			inline ~AudioAssetMusic() { ReleaseVoice(); CloseBaseAsset(getType(), true); }


			//----------------------------------------------------------------------------------
			//[Open]
			//�A�Z�b�g�̃��[�h���s��
			//����1: ��������������SubmixLayer, default = not monoral root layer
			//����2: �X�g���[�~���O�̕����Ԋu (����4 ~ 6�̕b���͂��̒l�̔{���ł���K�v������), default = cDefault (0.2f)
			//����3: �Đ��J�n�b��, default = cDefault (Begin)
			//����4: ���[�v���J�n�b��, default = cDefault (Begin)
			//����5: �Đ��I���b��, default = cDefault (End)
			void Open(WeakPointer<Audio::SubmixLayer> submixLayer = Audio::submixRootLayer(), float setStreamingPlaybackInterval = 0.2f,
				float setPlayBeginSeconds = cDefault, float setLoopBeginSeconds = cDefault, float setPlayEndSeconds = cDefault);

			//----------------------------------------------------------------------------------
			//[Reload]
			//�A�Z�b�g�̃����[�h���s��
			void Reload() override;
			//----------------------------------------------------------------------------------
			//[Close]
			//�t�@�C�������
			void Close()  override;


			//----------------------------------------------------------------------------------
			//[Play]
			//�Đ����s��, ���ۂɍĐ������̂�1�t���[����ƂȂ�
			//����1: ���[�v�̉�, default = true
			inline void Play(bool isLoop = true)
			{
				//Lock���ăL���[�ǉ�
				AtomicOperation::LockAtomic(m_lock);
				m_playQueue.state |= PlayQueue::cPlay;
				m_isExitLoop = isLoop ^ true;
				AtomicOperation::UnlockAtomic(m_lock);

				//�ʒm
				m_updateCondition.NotifyOne(m_cFlagPlay);
			}
			//----------------------------------------------------------------------------------
			//[Stop]
			//�Đ���S�Ē�~����, ���ۂɒ�~�����̂�1�t���[����ƂȂ�
			inline void Stop() override
			{
				//Lock���ăL���[�ǉ�
				AtomicOperation::LockAtomic(m_lock);
				m_playQueue.state |= PlayQueue::cStop;
				AtomicOperation::UnlockAtomic(m_lock);

				//�ʒm
				m_updateCondition.NotifyOne(m_cFlagPlay);
			}
			//----------------------------------------------------------------------------------
			//[Pause]
			//�Đ����Ȃ�ꎞ��~����, ���ۂɎ��s�����̂�1�t���[����ƂȂ�
			inline void Pause()
			{
				//Lock���ăL���[�ǉ�
				AtomicOperation::LockAtomic(m_lock);
				m_playQueue.state |= PlayQueue::cPause;
				AtomicOperation::UnlockAtomic(m_lock);

				//�ʒm
				m_updateCondition.NotifyOne(m_cFlagPlay);
			}
			//----------------------------------------------------------------------------------
			//[Replay]
			//�ŏ�����Đ������, ���ۂɎ��s�����̂�1�t���[����ƂȂ�
			inline void Replay()
			{
				AtomicOperation::LockAtomic(m_lock);
				m_playQueue.state |= PlayQueue::cReplay;
				AtomicOperation::UnlockAtomic(m_lock);

				//�ʒm
				m_updateCondition.NotifyOne(m_cFlagPlay);
			}
			//----------------------------------------------------------------------------------
			//[SeekBegin]
			//�Đ��ʒu�������ʒu�ɖ߂�, ���ۂɎ��s�����̂�1�t���[����ƂȂ�
			inline void SeekBegin()
			{
				AtomicOperation::LockAtomic(m_lock);
				m_playQueue.state |= PlayQueue::cSeekBegin;
				AtomicOperation::UnlockAtomic(m_lock);

				//�ʒm
				m_updateCondition.NotifyOne(m_cFlagPlay);
			} 
			//----------------------------------------------------------------------------------
			//[ExitLoop]
			//���݂̍Đ��Ń��[�v�Đ����I������
			//����1: �ݒ�l (Play, Stop, Replay���s��false�ɏ�����)
			inline void ExitLoop(bool isSet) 
			{
				AtomicOperation::LockAtomic(m_lock);
				m_isExitLoop = isSet;
				AtomicOperation::UnlockAtomic(m_lock); 
			}
			//----------------------------------------------------------------------------------
			//[CheckError]
			//return: �Đ��G���[���, default-> S_OK
			inline HRESULT CheckError() { return m_callback.errorInfo; }

			//get asset type (read function property)
			SGF_FUNCTION_PROPERTY Asset::AssetType getType() override { return Asset::AssetType::AudioMusic; }

			GetOnlyProperty<PlayQueue> playQueue;						//�Đ��L���[
			GetOnlyProperty<float> streamingPlaybackInterval;			//�X�g���[�~���O�Đ��Ԋu
			GetOnlyProperty<std::atomic_bool> isPlayNow;				//�Đ��� -> true
			GetOnlyProperty<std::atomic_bool> isFirstFullPlayback;	//1���[�v�I����true, Stop��Replay�֐���false�Ƀ��Z�b�g
			GetOnlyProperty<std::atomic_bool> isPauseNow;			//�ꎞ�Đ���~�� -> true

		private:
			//�R�[���o�b�N
			struct MusicCallback : public IXAudio2VoiceCallback
			{
				//�R���X�g���N�^
				inline MusicCallback(ConditionVariable::MultipleBit& updateCondition) : errorInfo(S_OK), updateCondition(updateCondition) {}
				
				void STDMETHODCALLTYPE OnVoiceProcessingPassStart(UINT32 BytesRequired) {}
				void STDMETHODCALLTYPE OnVoiceProcessingPassEnd(void) {}
				void STDMETHODCALLTYPE OnStreamEnd(void) {}
				void STDMETHODCALLTYPE OnBufferStart(void *pBufferContext) {}
				void STDMETHODCALLTYPE OnBufferEnd(void *pBufferContext) { updateCondition.NotifyOne(m_cFlagUpdate); }
				void STDMETHODCALLTYPE OnLoopEnd(void *pBufferContext) {}
				void STDMETHODCALLTYPE OnVoiceError(void *pBufferContext, HRESULT Error) { errorInfo = Error; }

				std::atomic<HRESULT> errorInfo;				//ErrorInfo
				ConditionVariable::MultipleBit& updateCondition;	//�A�b�v�f�[�g�C�x���g
			};

			//----------------------------------------------------------------------------------
			//[�R���X�g���N�^]
			//���������s��
			inline AudioAssetMusic() : BaseAudioAsset(), playQueue(m_playQueue),
				streamingPlaybackInterval(m_streamingPlaybackInterval), isPauseNow(m_isPauseNow), 
				m_updateCondition(m_cFlagsAll), isPlayNow(m_isPlayNow), isFirstFullPlayback(m_isFirstFullPlayback) {}

			//----------------------------------------------------------------------------------
			//[PlayUpdate]
			//�X�V���s��, �ʃX���b�h����
			void PlayUpdate();
			
			static constexpr uint m_cBufferMax = 2;						//�o�b�t�@�ő吔
			static constexpr uint m_cFlagPlay = 1;							//Play�֐��ʒm
			static constexpr uint m_cFlagUpdate = 2;						//�R�[���o�b�N�̃A�b�v�f�[�g�ʒm
			static constexpr uint m_cFlagQuit = 4;							//�I���ʒm
			//All Flag
			static constexpr uint m_cFlagsAll = m_cFlagPlay | m_cFlagUpdate | m_cFlagQuit;

			XAUDIO2_BUFFER m_bufferInfo = {};							//�o�b�t�@�[
			XAUDIO2_VOICE_STATE m_state = {};						//state
			std::thread m_updateThread;										//Update�֐��X���b�h
			MusicCallback m_callback = { m_updateCondition };		//�R�[���o�b�N
			ConditionVariable::MultipleBit m_updateCondition;		//�����ϐ�
			HMMIO m_mmioHandle = nullptr;								//MMIO�̃n���h��
			uint64 m_useThread = 0;											//���蓖�Ă�ꂽ�X���b�h�A�t�B�j�e�B
			PlayQueue m_playQueue;											//�L���[
			byte* m_buffers = nullptr;											//�Đ��o�b�t�@�[
			byte* m_readPoint = nullptr;										//�ǂݍ��݃|�C���g
			float m_streamingPlaybackInterval = 0.0f;					//�X�g���[�~���O�Đ��Ԋu
			uint m_bufferLength = 0;											//�o�b�t�@�[�̒���
			uint m_readLength = 0;												//�ǂݍ��񂾒���
			int m_seek = 0;															//�V�[�N���Ă�������
			int m_beginOffset = 0;												//�����|�C���g�I�t�Z�b�g
			int m_loopOffset = 0;													//���[�v�����I�t�Z�b�g
			int m_beginPoint = 0;													//�����|�C���g
			int m_endPoint = 0;													//�I���|�C���g
			std::atomic_bool m_isPauseNow = false;						//�ꎞ�Đ���~�� -> true
			std::atomic_bool m_isPlayNow = false;						//�Đ��� -> true
			std::atomic_bool m_isFirstFullPlayback = false;			//1���[�v�I����true, Stop��Replay�֐���false�Ƀ��Z�b�g
			std::atomic_bool m_lock = false;									//lock
			byte m_useBuffer = 0;												//�g�p�o�b�t�@
			bool m_isExitLoop = false;											//���[�v�I���t���O

		private:
			//----------------------------------------------------------------------------------
			//[PlayExecution]
			//���ۂɍĐ����s��
			void PlayExecution(bool& isExitLoop);
			//----------------------------------------------------------------------------------
			//[StopExecution]
			//���ۂɒ�~���s��
			void StopExecution(bool& isExitLoop);
			//----------------------------------------------------------------------------------
			//[PauseExecution]
			//���ۂɈꎞ��~���s��
			void PauseExecution();
			//----------------------------------------------------------------------------------
			//[ReplayExecution]
			//���ۂɍĐ����s��
			void ReplayExecution(bool& isExitLoop);
			//----------------------------------------------------------------------------------
			//[SeekBeginExecution]
			//���ۂɃV�[�N���s��
			void SeekBeginExecution();
			//----------------------------------------------------------------------------------
			//[ReleaseVoice]
			//�������J������
			void ReleaseVoice();
		};
	}
}
#endif // !SGFRAMEWORK_HEADER_AUDIO_ASSET_MUSIC_HPP_