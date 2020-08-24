/*----------------------------------------------------------------------------------
オーディオを管理するAudio class
	->SubmixLayerを記述するAudioSubmixLayer.cpp
------------------------------------------------------------------------------------*/
#include "Audio.hpp"

// Framework namespace
namespace SGFramework
{
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//Root用初期化を行う
	Audio::SubmixLayer::SubmixLayer() : voice(m_voice), layerName(m_layerName), layerID(m_layerID),
		channels(m_channels), processingStage(m_processingStage), m_childrens(), m_sendVoices(), m_layerName(L"Null"),
		m_parent(), m_voice(nullptr),m_layerID(VariableLimit::cUintMax), m_processingStage(VariableLimit::cUintMax)
	{
		m_sends.pSends = nullptr;
		m_sends.SendCount = 0;

		XAUDIO2_SEND_DESCRIPTOR send;
		send.Flags = 0;
		send.pOutputVoice = nullptr;

		m_sendVoices.emplace_back(send);
	}

	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//非Root用初期化を行う
	//引数1: レイヤー名
	//引数2: レイヤーID
	Audio::SubmixLayer::SubmixLayer(const sgstring & name, uint id)
		: voice(m_voice), layerName(m_layerName), layerID(m_layerID), channels(m_channels), processingStage(m_processingStage),
		m_childrens(), m_sendVoices(), m_layerName(name), m_parent(), m_voice(nullptr), m_layerID(id), m_processingStage(VariableLimit::cUintMax)
	{
		m_sends.pSends = nullptr;
		m_sends.SendCount = 0;

		XAUDIO2_SEND_DESCRIPTOR send;
		send.Flags = 0;
		send.pOutputVoice = nullptr;

		m_sendVoices.emplace_back(send);
	}

	//----------------------------------------------------------------------------------
	//[デストラクタ]
	//DestroyVoiceを行う
	Audio::SubmixLayer::~SubmixLayer()
	{
		if (m_voice && Audio::m_xaudio2)
		{
			m_voice->DestroyVoice();
			m_voice = nullptr;
		}
	}


	//----------------------------------------------------------------------------------
	//[SettingRoot]
	//Root用設定を行う
	//引数1: 音量
	void Audio::SubmixLayer::SettingRoot(float volume)
	{
		//設定
		m_layerName = cRootName;
		m_layerID = cRootID;
		m_processingStage = 0;
		m_channels = Audio::m_masterChannels;

		//Create
		auto result = Audio::m_xaudio2->CreateSubmixVoice(&m_voice, m_channels, Audio::m_sampleRate);
		if (FAILED(result))
			throw InvalidArgument(L"Error!! Audio::SubmixLayer->SettingRoot", L"CreateSubmixVoice error", result);
	
		//設定
		m_sendVoices[0].pOutputVoice = m_voice;
		m_sends.SendCount = 1;
		m_sends.pSends = m_sendVoices.data();
		//音量セット
		m_voice->SetVolume(volume);
	}

	//----------------------------------------------------------------------------------
	//[SettingMonoralRoot]
	//MonoralRoot用設定を行う
	//引数1: 音量
	void Audio::SubmixLayer::SettingMonoralRoot(float volume)
	{
		//設定
		m_layerName = cMonoralRootName;
		m_layerID = cMonoralRootID;
		m_processingStage = 0;
		m_channels = Audio::cMonoral;

		//Create
		auto result = Audio::m_xaudio2->CreateSubmixVoice(&m_voice, 1, Audio::m_sampleRate);
		if (FAILED(result))
			throw InvalidArgument(L"Error!! Audio::SubmixLayer->SettingMonoralRoot", L"CreateSubmixVoice error", result);

		//設定
		m_sendVoices[0].pOutputVoice = m_voice;
		m_sends.SendCount = 1;
		m_sends.pSends = m_sendVoices.data();
		//音量セット
		m_voice->SetVolume(volume);
	}

	//----------------------------------------------------------------------------------
	//[SettingSubmix]
	//非Root用設定を行う
	//引数1: 音量
	void Audio::SubmixLayer::SettingSubmix(float volume)
	{
		//processingStage
		uint stage = 1;
		//channels設定
		m_channels = Audio::m_masterChannels;

		//親探索ループ
		for (auto pt = m_parent; pt->m_parent.getIsValid(); pt = pt->m_parent)
		{
			//ステージ加算
			++stage;

			//Send登録
			XAUDIO2_SEND_DESCRIPTOR send;
			send.Flags = 0;
			send.pOutputVoice = pt->m_voice;

			m_sendVoices.emplace_back(send);

			if (IS_FALSE(pt->m_parent.getIsValid()) && pt->m_layerID == cMonoralRootID)
				m_channels = Audio::cMonoral;
		}
		
		//設定
		this->m_processingStage = stage;

		//Create
		auto result = Audio::m_xaudio2->CreateSubmixVoice(&m_voice, m_channels, Audio::m_sampleRate, 0, stage);
		if (FAILED(result))
			throw InvalidArgument(L"Error!! Audio::SubmixLayer->SettingRoot", L"CreateSubmixVoice error", result);

		//設定
		m_sendVoices[0].pOutputVoice = m_voice;
		m_sends.SendCount = static_cast<uint>(m_sendVoices.size());
		m_sends.pSends = m_sendVoices.data();
		//音量セット
		m_voice->SetVolume(volume);
	}
}