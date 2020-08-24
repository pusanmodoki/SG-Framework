/*----------------------------------------------------------------------------------
BaseAudioAsset�n�N���X�̃x�[�X�ƂȂ�BaseAudioAsset class
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
		//AudioAsset�n�N���X�̃x�[�X�ƂȂ�BaseAudioAsset class
		class BaseAudioAsset;
		//AudioAssetSE�n�N���X�̃x�[�X�ƂȂ�BaseAudioSEAsset class
		class BaseAudioSEAsset;


		//BaseAudioAsset�n�N���X�̃x�[�X�ƂȂ�BaseAudioAsset class
		class BaseAudioAsset : public BaseClass::BaseAsset
		{
		public:
			//friend
			friend class SGFramework::Audio;
			//�f�t�H���g�l
			static constexpr float cDefault = -1;

			//----------------------------------------------------------------------------------
			//[Stop]
			//�Đ���S�Ē�~����, ���ۂɒ�~�����̂�1�t���[����ƂȂ�
			virtual void Stop() = 0;

			//----------------------------------------------------------------------------------
			//[ChangeSpeedSmoothly]
			//�Đ��X�s�[�h��target�܂Ŋ��炩�ɕύX����
			//����1: �ύXtarget
			//����2: �ύX�ɂ�����b��
			//����3: �I�����~���邩�ۂ�, default = false
			inline void ChangeSpeedSmoothly(float target, float seconds, bool isStop = false)
			{
				using namespace Detail::Audio;

				//���݂̒l
				float speed = getPlayBackSpeed();
				//�����Ȃ�I��
				if (target == speed) return;

				//Message�p
				AudioMessage message(MessageType::SmoothFlag | MessageType::AddSmoothAudio);

				//Message�̓Y�t������
				message.attachment.smoothAudio.asset = m_thisPointer.DynamicCast<BaseAudioAsset>();
				message.attachment.smoothAudio.smoothType = SmoothType::Speed;
				message.attachment.smoothAudio.now = speed;
				message.attachment.smoothAudio.target = target;
				message.attachment.smoothAudio.speed = (target - speed) / seconds;
				message.attachment.smoothAudio.isStop = isStop;

				//Message���M
				Audio::SendAudioMessage(message);
			}
			//----------------------------------------------------------------------------------
			//[ChangeVolumeSmoothly]
			//�Đ����ʂ�target�܂Ŋ��炩�ɕύX����
			//����1: �ύXtarget
			//����2: �ύX�ɂ�����b��
			//����3: �I�����~���邩�ۂ�, default = false
			inline void ChangeVolumeSmoothly(float target, float seconds, bool isStop = false)
			{
				using namespace Detail::Audio;

				//���݂̒l
				float volume = getVolume();
				//�����Ȃ�I��
				if (target == volume) return;

				//Message�p
				AudioMessage message(MessageType::SmoothFlag | MessageType::AddSmoothAudio);
				
				//Message�̓Y�t������
				message.attachment.smoothAudio.asset = m_thisPointer.DynamicCast<BaseAudioAsset>();
				message.attachment.smoothAudio.smoothType = SmoothType::Volume;
				message.attachment.smoothAudio.now = volume;
				message.attachment.smoothAudio.target = target;
				message.attachment.smoothAudio.speed = (target - volume) / seconds;
				message.attachment.smoothAudio.isStop = isStop;

				//Message���M
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

			GetOnlyProperty<float> audioSeconds;											//�t�@�C���̕b��
			GetOnlyProperty<float> playBeginSeconds;										//�Đ��J�n�b��
			GetOnlyProperty<float> loopBeginSeconds;										//���[�v���J�n�b��
			GetOnlyProperty<float> playEndSeconds;										//�Đ��I���b��
			GetOnlyProperty<uint> sourceSampleRate;										//�\�[�X�̃T���v�����[�g (�Đ����[�g��Audio��sampleRate)
			GetOnlyProperty<uint> channels;													//�\�[�X�̃`�����l����(�Đ����[�g��SubmixLayer��channels)
			GetOnlyProperty<uint> submixLayerID;											//���g�̏�������T�u�~�b�N�X���C���[ID

		protected:
			//�R���X�g���N�^, �Q�b�^�[�ɒl������܂���
			inline BaseAudioAsset() : BaseAsset(), audioSeconds(m_audioSeconds), 
				playBeginSeconds(m_playBeginSeconds), loopBeginSeconds(m_loopBeginSeconds),
				playEndSeconds(m_playEndSeconds), sourceSampleRate(m_sourceSampleRate), 
				channels(m_channels), submixLayerID(m_submixLayerID) {}
		
			static constexpr uint m_cBaseFlagPublicState = 0;
			static constexpr uint m_cBaseFlagPrivateState = 1;
			
			//���C���{�C�X
			ComWearFunction<IXAudio2SourceVoice> m_voice;	
			float m_audioSeconds = 0.0f;								//�t�@�C���̕b��
			float m_playBeginSeconds = 0.0f;						//�Đ��J�n�b��
			float m_loopBeginSeconds = 0.0f;						//���[�v���J�n�b��
			float m_playEndSeconds = 0.0f;							//�Đ��I���b��
			uint m_sourceSampleRate = 0;							//�\�[�X�̃T���v�����[�g (�Đ����[�g��Audio��sampleRate)
			uint m_channels = 0;											//�\�[�X�̃`�����l����(�Đ����[�g��SubmixLayer��channels)
			uint m_submixLayerID = 0;								//���g�̏�������T�u�~�b�N�X���C���[ID
		};


		//AudioAssetSE�n�N���X�̃x�[�X�ƂȂ�BaseAudioSEAsset class
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

			//�\�[�X�̐� (�ő哯���Đ���)
			GetOnlyProperty<uint> amountOfSource;
			//�Đ��Ԋu
			GetOnlyProperty<float> playbackInterval;

		protected:
			//[�R���X�g���N�^]
			inline BaseAudioSEAsset() : BaseAudioAsset(), 
				amountOfSource(m_amountOfSource), playbackInterval(m_playbackInterval) {}

			//----------------------------------------------------------------------------------
			//[LoadFile]
			//�A�Z�b�g�̃��[�h���s��
			//����1: ��������������SubmixLayer, default = not monoral root layer
			//����2: �\�[�X�̐� (�ő哯���Đ���)
			//����3: �Đ��Ԋu
			//����4: �Đ��J�n�b��, default = cDefault (Begin)
			//����5: ���[�v���J�n�b��, default = cDefault (Begin)
			//����6: �Đ��I���b��, default = cDefault (End)
			void LoadFile(const WeakPointer<Audio::SubmixLayer>& submixLayer = Audio::submixRootLayer(),
				uint amountOfSource = 1, float playbackInterval = 0.0f, float setPlayBeginSeconds = cDefault,
				float setLoopBeginSeconds = cDefault, float setPlayEndSeconds = cDefault);
			//----------------------------------------------------------------------------------
			//[ReleaseBaseVoice]
			//�������J������
			void ReleaseBaseVoice();

			//----------------------------------------------------------------------------------
			//[FindChunk�֐�]
			//Chunk��T��,�T���v�����炢��������
			bool FindChunk(const HANDLE hFile, const DWORD fourcc, DWORD & dwChunkSize, DWORD & dwChunkDataPosition);
			//----------------------------------------------------------------------------------
			//[ReadChunkData�֐�]
			//Chunk��ǂݍ���,�T���v�����炢��������
			bool ReadChunkData(const HANDLE hFile, void * buffer, const DWORD buffersize, const DWORD bufferoffset);

			//SE�p�̃T�u�{�C�X
			std::vector<ComWearFunction<IXAudio2SourceVoice>> m_subVoices;
			//�Đ��o�b�t�@�[
			ComWearDelete<byte> m_buffer = nullptr;
			//X3DAudio�p�o�b�t�@
			ComWearDelete<float> m_x3dBuffer = nullptr;
			XAUDIO2_BUFFER m_bufferInfo = {};			//�o�b�t�@�[
			XAUDIO2_VOICE_STATE m_state = {};		//state
			float m_playbackInterval = 0.0f;					//�Đ��Ԋu
			float m_playElapased = 0.0f;						//�Đ��Ԋu�v���p
			uint m_amountOfSource = 0;						//�\�[�X�̐� (�ő哯���Đ���)
			int m_useVoice = -1;									//�g�p�{�C�X�J�E���^
			std::atomic_bool m_lock = false;					//lock
		};
	}
}
#endif // !SGFRAMEWORK_HEADER_AUDIO_ASSET_HPP_