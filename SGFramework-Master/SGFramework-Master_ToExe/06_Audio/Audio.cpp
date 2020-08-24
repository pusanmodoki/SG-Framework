/*----------------------------------------------------------------------------------
�I�[�f�B�I���Ǘ�����Audio class
	->�ϐ��Ɗ֐����L�q����Audio.cpp
------------------------------------------------------------------------------------*/
#include "Audio.hpp"
#include "../07_Thread/Thread.hpp"
#include "AudioAsset/AudioAssetMusic.hpp"

// Framework namespace
namespace SGFramework
{
	//IXAudio2
	PointerProperty<IXAudio2*> Audio::xaudio2 = m_xaudio2;
	//IXAudio2MasteringVoice
	PointerProperty<IXAudio2MasteringVoice*> Audio::masteringVoice = m_masteringVoice;
	//RootLayer
	GetOnlyProperty<SharedPointer<Audio::SubmixLayer>> Audio::submixRootLayer = m_submixRootLayer;
	//MonoralRootLayer
	GetOnlyProperty<SharedPointer<Audio::SubmixLayer>> Audio::submixMonoralRootLayer = m_submixMonoralRootLayer;
	//�K��̃`�����l����
	GetOnlyProperty<uint> Audio::masterChannels = m_masterChannels;
	//�}�X�^�[�T���v�����[�g
	GetOnlyProperty<uint> Audio::sampleRate = m_sampleRate;

	//IXAudio2
	IXAudio2* Audio::m_xaudio2 = nullptr;
	//IXAudio2MasteringVoice
	IXAudio2MasteringVoice* Audio::m_masteringVoice = nullptr;
	//X3DAUDIO_HANDLE
	X3DAUDIO_HANDLE Audio::m_x3dHandle = {};
	//X3DAUDIO_LISTENER
	X3DAUDIO_LISTENER Audio::m_x3dListener = {};
	//effects (2D)
	std::unordered_map<uint, WeakPointer<Asset::AudioAssetEffect>> Audio::m_2dEffects;
	//effects (3D)
	std::unordered_map<uint, WeakPointer<Asset::AudioAssetEffect3D>> Audio::m_3dEffects;
	//smooth (submix)
	std::unordered_map<IXAudio2SubmixVoice*, Audio::SmoothSubmix> Audio::m_smoothSubmixes;
	//smooth (audio)
	std::unordered_map<uint, Audio::SmoothAudio> Audio::m_smoothAudios;
	//AudioMessages
	std::vector<Detail::Audio::AudioMessage> Audio::m_messages;
	//AudioMessages (use audio thread)
	std::vector<Detail::Audio::AudioMessage> Audio::m_useThreadMessages;
	//�T�u�~�b�N�X���C���[
	std::vector< SharedPointer<Audio::SubmixLayer>> Audio::m_submixLayers;
	//Submix Root
	SharedPointer <Audio::SubmixLayer> Audio::m_submixRootLayer;
	//Submix Monoral Root
	SharedPointer<Audio::SubmixLayer> Audio::m_submixMonoralRootLayer;
	//Audio thread
	std::thread Audio::m_audioThread;
	//Audio thread�X�V�Ԋu(chrono)
	std::chrono::milliseconds Audio::m_nowAudioUpdateInterval = std::chrono::milliseconds(20);
	//Audio thread�X�V�Ԋu(chrono)
	std::chrono::milliseconds Audio::m_audioUpdateIntervalForChrono = std::chrono::milliseconds(20);
	//Audio thread�X�V�Ԋu(float)
	float Audio::m_audioUpdateIntervalForFloat = 0.02f;
	//�`�����l����
	uint Audio::m_masterChannels = 0;
	//�}�X�^�[�T���v�����[�g
	uint Audio::m_sampleRate = 0;
	//�T�u�~�b�N�X���C���[�J�E���^
	uint Audio::m_layerCounter = 1;
	//message lock
	std::atomic_bool Audio::m_lockMessages = false;
	//audio thread �I���t���O
	std::atomic_bool Audio::m_isEndUpdate = false;


	//AudioListener
	
	//static instance
	WeakPointer<AudioListener> AudioListener::m_instance;
	//instance lock
	std::atomic_bool AudioListener::m_lock = false;



	//----------------------------------------------------------------------------------
	//[StartUp]
	//�N�������������s��
	//����1: �}�X�^�[����
	//����2: �`�����l����
	//����3: �T���v�����[�g
	//����4: �T�u�~�b�N�X���C���[
	void Audio::StartUp(const ReadElement::Pack& masterVolume, const ReadElement::Pack& masterChannels, 
		const ReadElement::Pack& sampleRate, const ReadElement::Pack& audioUpdateInterval, 
		const ReadElement::Pack& audioSmoothReserve, const ReadElement::Groop& submixLayers)
	{
		HRESULT result;	//Result

		//CoInit
		result = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
		if (FAILED(result))
			throw InvalidArgument(L"Error!! Audio->StartUp", L"CoInitializeEx error", result);
		
		//XAudio2Create
		result = XAudio2Create(&m_xaudio2, 0, Thread::getAffinityAudio());
		if (FAILED(result))
		{
			CoUninitialize();
			throw InvalidArgument(L"Error!! Audio->StartUp", L"XAudio2Create error", result);
		}

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

		//CreateMasteringVoice
		result = m_xaudio2->CreateMasteringVoice(&m_masteringVoice, channelMask);
		if (FAILED(result))
		{
			COM_RELEASE(m_xaudio2);
			CoUninitialize();
			throw InvalidArgument(L"Error!! Audio->StartUp", L"CreateMasteringVoice error", result);
		}

		//Root�쐬
		m_submixRootLayer = SharedPointer<SubmixLayer>(new SubmixLayer());
		m_submixMonoralRootLayer = SharedPointer<SubmixLayer>(new SubmixLayer());

		try
		{
			//�l�擾
			m_masterChannels = static_cast<uint>(masterChannels[0].valueInt);
			m_sampleRate = static_cast<uint>(sampleRate[0].valueInt);
			//Root�ݒ�
			m_submixRootLayer->SettingRoot(masterVolume[0].valueFloat);
			m_submixMonoralRootLayer->SettingMonoralRoot(masterVolume[0].valueFloat);
			//�X�V�Ԋu�o�^
			m_audioUpdateIntervalForFloat = audioUpdateInterval[0].valueFloat;
			m_audioUpdateIntervalForChrono = std::chrono::milliseconds(static_cast<int>(audioUpdateInterval[0].valueFloat * 1000.0f));
			m_nowAudioUpdateInterval = m_audioUpdateIntervalForChrono;
			//Smooth�z��m��
			m_smoothAudios.reserve(audioSmoothReserve[0].valueInt);
		}
		catch (...)
		{
			throw;
		}

		//X3DAudio������
		result = X3DAudioInitialize(channelMask, X3DAUDIO_SPEED_OF_SOUND, m_x3dHandle);
		if (FAILED(result))
		{
			ShutDown();
			throw InvalidArgument(L"Error!! Audio->StartUp", L"X3DAudioInitialize error", result);
		}

		//SubmixLayer�̐��������C���[�ǉ�
		for (const auto& e : submixLayers.packs)
			m_submixLayers.emplace_back(SharedPointer<SubmixLayer>(new SubmixLayer(e.header, ++m_layerCounter)));
		//Parent�ݒ胋�[�v
		for (uint i = 0; i <= m_layerCounter - 2; i++)
		{
			//�e�Ƃ������������񂪂���͂�
			if (submixLayers[i].elements.size() >= 2)
			{
				try
				{
					//Find
					auto parent = FindLayer(submixLayers[i][1].valueString);
					//�Ȃ����throw
					if (IS_FALSE(parent.getIsValid())) 
						throw InvalidArgument(L"Error!! Audio->StartUp", L"SubmixLayer Parent not found");
					//�ݒ�
					m_submixLayers[i]->m_parent = std::move(parent);
					//�q���ǉ�
					m_submixLayers[i]->m_parent->m_childrens.emplace_back(m_submixLayers[i]);
				}
				catch (...)
				{
					throw;
				}
			}
			//�Ȃ��̂Ŏq��
			else
			{
				//�ݒ�
				m_submixLayers[i]->m_parent = m_submixRootLayer;
				//�q���ǉ�
				m_submixLayers[i]->m_parent->m_childrens.emplace_back(m_submixLayers[i]);
			}
		}

		//SettingSubmix���[�v, volume���K�v�Ȃ̂ŕϑ��I
		{
			uint i = 0;
			for (auto& e : m_submixLayers)
			{
				try
				{
					//SettingSubmix
					e->SettingSubmix(submixLayers[i][0].valueFloat);
				}
				catch (...)
				{
					throw;
				}
				i++;
			}
		}

		//Smooth�z��m��
		m_smoothSubmixes.reserve(m_submixLayers.size() + 2);

		//Thread�쐬
		m_audioThread = std::thread(UpdateThread);
		Thread::setAffinityAudioUpdate(m_audioThread.native_handle());
	}

	//----------------------------------------------------------------------------------
	//[ShutDown]
	//�I���������s��
	void Audio::ShutDown()
	{
		if (m_masteringVoice == nullptr) return;

		//thread join
		if (IS_TRUE(m_audioThread.joinable()))
		{
			AtomicOperation::Init(m_isEndUpdate, true);
			m_audioThread.join();
		}

		//processing message
		for (auto& e : m_messages)
			ProcessingMessage(e);
		for (auto& e : m_useThreadMessages)
			ProcessingMessage(e);

		m_messages.clear();
		m_useThreadMessages.clear();

		//Clear, DestroyVoice
		m_2dEffects.clear();
		m_3dEffects.clear();
		m_submixLayers.clear();
		m_submixRootLayer.WaiverAndReset();
		m_submixMonoralRootLayer.WaiverAndReset();
		//Destroy
		if (m_masteringVoice)
		{
			m_masteringVoice->DestroyVoice();
			m_masteringVoice = nullptr;
		}
		//Release
		COM_RELEASE(m_xaudio2);
		//Uninit
		CoUninitialize();
	}
}