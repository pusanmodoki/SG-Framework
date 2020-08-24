/*----------------------------------------------------------------------------------
�I�[�f�B�I���Ǘ�����Audio class
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

	//�I�[�f�B�I���Ǘ�����Audio class
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
		//name�̃T�u�~�b�N�X���C���[����������
		//return: ���݂��Ă����ꍇlayer�̃|�C���^, �Ȃ��ꍇ�͋�|�C���^
		//����1: ���C���[��
		inline static SharedPointer<SubmixLayer> FindLayer(const sgstring& name);

		//submix layer[id]<static>(get function property)
		//return:  layer[id], not found->null
		//argument 1: id
		SGF_FUNCTION_PROPERTY static SharedPointer<SubmixLayer> getLayer(uint id)
		{
			//root����->�����return->�Ȃ���΋�|�C���^
			return id == 0 ? m_submixRootLayer :
				id == 1 ? m_submixMonoralRootLayer :
				id <= m_layerCounter ? 
				m_submixLayers[id - 2] : SharedPointer<Audio::SubmixLayer>::getNull();
		}

		//master channels mask(used XAudio2)<static> (read only property)
		SGF_FUNCTION_PROPERTY sgstring getMasterChannelsMask() const
		{
			//�`�����l���}�X�N
			uint channelMask = SPEAKER_MONO;
			//�`�����l������
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
			//�`�����l������
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
		//MonoralRootLayer ���̃��C���[�̎q���C���[�͑S�ă`�����l������1�ɂȂ�
		static GetOnlyProperty<SharedPointer<SubmixLayer>> submixMonoralRootLayer;
		//�K��̃`�����l����
		static GetOnlyProperty<uint> masterChannels;
		//�}�X�^�[�T���v�����[�g
		static GetOnlyProperty<uint> sampleRate;
		
		//SubmixAudio�����C���[�\���Œ񋟂���SubmixLayer class
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
			//[�f�X�g���N�^]
			//DestroyVoice���s��
			~SubmixLayer();

			//----------------------------------------------------------------------------------
			//[ChangeVolumeSmoothly]
			//�Đ����ʂ�target�܂Ŋ��炩�ɕύX����
			//����1: �ύXtarget
			//����2: �ύX�ɂ�����b��
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
			//argument 1: volume magnification (�{��, �ʏ�1)
			SGF_FUNCTION_PROPERTY float setVolume(float magnification) { m_voice->SetVolume(magnification); return magnification; }

			//master channels mask(used XAudio2)<static> (read only property)
			SGF_FUNCTION_PROPERTY sgstring getChannelsMask() const
			{
				//�`�����l���}�X�N
				uint channelMask = SPEAKER_MONO;
				//�`�����l������
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
				//�`�����l������
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

			//SubmixRootLayer��ID
			static constexpr uint cRootID = 0;
			//SubmixRootLayer��Name
			static constexpr wchar cRootName[] = L"root";
			//SubmixMonoralRootLayer��ID
			static constexpr uint cMonoralRootID = 1;
			//SubmixMonoralRootLayer��Name
			static constexpr wchar cMonoralRootName[] = L"monoralRoot";

			GetOnlyProperty<IXAudio2SubmixVoice*> voice;	//Voice
			GetOnlyProperty<sgstring> layerName;				//���C���[��
			GetOnlyProperty<uint> layerID;							//���C���[ID
			GetOnlyProperty<uint> processingStage;				//���C���[�̊K�w�̐[��
			GetOnlyProperty<uint> channels;						//���C���[�̃`�����l����
		private:
			//----------------------------------------------------------------------------------
			//[�R���X�g���N�^]
			//Root�p���������s��
			SubmixLayer();
			//----------------------------------------------------------------------------------
			//[�R���X�g���N�^]
			//��Root�p���������s��
			//����1: ���C���[��
			//����2: ���C���[ID
			SubmixLayer(const sgstring& name, uint id);
			//----------------------------------------------------------------------------------
			//[SettingRoot]
			//Root�p�ݒ���s��
			//����1: ����
			void SettingRoot(float volume);
			//----------------------------------------------------------------------------------
			//[SettingMonoralRoot]
			//MonoralRoot�p�ݒ���s��
			//����1: ����
			void SettingMonoralRoot(float volume);
			//----------------------------------------------------------------------------------
			//[SettingSubmix]
			//��Root�p�ݒ���s��
			//����1: ����
			void SettingSubmix(float volume);	
			
			std::vector<WeakPointer<SubmixLayer>> m_childrens;		//�q���C���[
			std::vector<XAUDIO2_SEND_DESCRIPTOR> m_sendVoices;	//VoiceSends�p
			sgstring m_layerName;														//���C���[��
			XAUDIO2_VOICE_SENDS m_sends;										//Sneds
			WeakPointer<SubmixLayer> m_parent;								//�e���C���[
			IXAudio2SubmixVoice* m_voice;											//Voice
			uint m_layerID;																	//���C���[ID
			uint m_processingStage;														//���C���[�̊K�w�̐[��
			uint m_channels;																//���C���[�̃`�����l����
		};

	private:

		//���X�i�[�ݒ�p�o�b�t�@
		struct X3DAudioListenerBuffer
		{
			std::atomic_bool lock;					//lock
			X3DAUDIO_LISTENER listner = {};	//listner
		};
		struct SmoothSubmix
		{
			inline SmoothSubmix(float now, float target, float speed)
				: now(now), target(target), speed(speed) {}
			float now = 0.0f;			//���ݒl
			float target = 0.0f;			//�^�[�Q�b�g
			float speed = 0.0f;			//�ύX�X�s�[�h (�b��)
		};
		struct SmoothAudio
		{
			//�^�C�v���Ƃ̒l
			struct Values
			{
				float now = 0.0f;			//���ݒl
				float target = 0.0f;			//�^�[�Q�b�g
				float speed = 0.0f;			//�ύX�X�s�[�h (�b��)
				bool isEnabled = false;	//�L���H
				bool isStop = false;		//�I�����~�H
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
		//�N�������������s��
		//����1: �}�X�^�[����
		//����2: �`�����l����
		//����3: �T���v�����[�g
		//����4: �T�u�~�b�N�X���C���[
		static void StartUp(const ReadElement::Pack& masterVolume, const ReadElement::Pack& masterChannels, 
			const ReadElement::Pack& sampleRate, const ReadElement::Pack& audioUpdateInterval, 
			const ReadElement::Pack& audioSmoothReserve, const ReadElement::Groop& submixLayers);
		//----------------------------------------------------------------------------------
		//[ShutDown]
		//�I���������s��
		static void ShutDown();
		//----------------------------------------------------------------------------------
		//[UpdateThread]
		//�I�[�f�B�I�̍X�V���s��, �ʃX���b�h�p�֐�
		static void UpdateThread();
		//----------------------------------------------------------------------------------
		//[ProcessingMessage]
		//���b�Z�[�W����������
		//����1: message
		static void ProcessingMessage(const Detail::Audio::AudioMessage& message);
		//----------------------------------------------------------------------------------
		//[UpdateSmooth]
		//�X���[�X�̍X�V���s��
		//����1: delta time
		static void UpdateSmooth(float deltaTime);

		//----------------------------------------------------------------------------------
		//[SendAudioMessage]
		//���b�Z�[�W�𑗐M����
		//����1: message
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
		//�T�u�~�b�N�X���C���[
		static std::vector<SharedPointer<SubmixLayer>> m_submixLayers;	
		//Submix root
		static SharedPointer<SubmixLayer> m_submixRootLayer;
		//Submix monoral root
		static SharedPointer<SubmixLayer> m_submixMonoralRootLayer;
		//Audio thread
		static std::thread m_audioThread;
		//Audio thread�X�V�Ԋu(chrono)
		static std::chrono::milliseconds m_nowAudioUpdateInterval;
		//Audio thread�X�V�Ԋu(chrono)
		static std::chrono::milliseconds m_audioUpdateIntervalForChrono;
		//Audio thread�X�V�Ԋu(float)
		static float m_audioUpdateIntervalForFloat;
		//�K��̃`�����l����
		static uint m_masterChannels;
		//�}�X�^�[�T���v�����[�g
		static uint m_sampleRate;
		//�T�u�~�b�N�X���C���[�J�E���^
		static uint m_layerCounter;
		//message lock
		static std::atomic_bool m_lockMessages;
		//audio thread �I���t���O
		static std::atomic_bool m_isEndUpdate;
	};


	//----------------------------------------------------------------------------------
	//[FindLayer]
	//name�̃T�u�~�b�N�X���C���[����������
	//return: ���݂��Ă����ꍇlayer�̃|�C���^, �Ȃ��ꍇ�͋�|�C���^
	//����1: ���C���[��
	SharedPointer<Audio::SubmixLayer> Audio::FindLayer(const sgstring& name)
	{
		//Root����
		if (name == m_submixRootLayer->m_layerName) return m_submixRootLayer;
		if (name == m_submixMonoralRootLayer->m_layerName) return m_submixRootLayer;
		//����, ��������return
		for (auto& e : m_submixLayers)
			if (name == e->m_layerName) return e;
		//�Ȃ��̂ŋ�|�C���^
		return SharedPointer<Audio::SubmixLayer>();
	}

	//----------------------------------------------------------------------------------
	//[SendAudioMessage]
	//���b�Z�[�W�𑗐M����
	//����1: message
	inline void Audio::SendAudioMessage(const Detail::Audio::AudioMessage& message)
	{
		AtomicOperation::LockAtomic(m_lockMessages);
		m_messages.emplace_back(message);
		AtomicOperation::UnlockAtomic(m_lockMessages);
	}

	//----------------------------------------------------------------------------------
	//[ChangeVolumeSmoothly]
	//�Đ����ʂ�target�܂Ŋ��炩�ɕύX����
	//����1: �ύXtarget
	//����2: �ύX�ɂ�����b��
	inline void Audio::SubmixLayer::ChangeVolumeSmoothly(float target, float seconds)
	{
		using namespace Detail::Audio;

		//Message�p
		AudioMessage message((MessageType::SmoothFlag | MessageType::AddSmoothSubmix));
		//���ݒl
		float volume = getVolume();

		//Message�̓Y�t������
		message.attachment.smoothSubmix.submixVoice = m_voice;
		message.attachment.smoothSubmix.now = volume;
		message.attachment.smoothSubmix.target = target;
		message.attachment.smoothSubmix.speed = (target - volume) / seconds;
		
		//Message���M
		Audio::SendAudioMessage(message);
	}
}

#endif // !SGFRAMEWORK_HEADER_AUDIO_H_