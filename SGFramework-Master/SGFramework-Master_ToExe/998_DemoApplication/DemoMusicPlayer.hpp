#ifndef DEMO_MUSIC_PLAYER_HPP_
#define DEMO_MUSIC_PLAYER_HPP_
#include "../SGFramwork.hpp"


//Demo Music Player
class DemoMusicPlayer : public Component
{
public:

	//----------------------------------------------------------------------------------
	//[EnabledCallbacks]
	//return: 自身がCallされるCallback Flags, Component::CallbackFunctions参照
	inline virtual uint EnableCallbacks() { return CallbackFunctions::Awake | CallbackFunctions::Update | CallbackFunctions::OnDestroy;}
	
	//----------------------------------------------------------------------------------
	//[Awake]
	//インスタンス化した瞬間に呼ばれるCallback関数
	inline virtual void Awake() 
	{
		m_musicAssets[0] = Asset::InstantiateAsset<Asset::AudioAssetMusic>(L"Assets/Audio/BGM01.wav");
		m_musicAssets[0]->Open();
		m_musicAssets[1] = Asset::InstantiateAsset<Asset::AudioAssetMusic>(L"Assets/Audio/BGM02.wav");
		m_musicAssets[1]->Open();
		m_effectAsset = Asset::InstantiateAsset<Asset::AudioAssetEffect>(L"Assets/Audio/DemoSE2D.wav");
		m_effectAsset->Open();
		m_effect3DAsset = Asset::InstantiateAsset<Asset::AudioAssetEffect3D>(L"Assets/Audio/DemoSE3D.wav");
		m_effect3DAsset->Open(Audio::submixMonoralRootLayer(), 100, 0.5f, 10.0f);
	}

	inline virtual void OnDestroy()
	{
		int a = 0;
		a = 0;
	}

	//----------------------------------------------------------------------------------
	//[Update]
	//毎フレームUpdateブロックでCallbackされる関数
	inline virtual void Update()
	{
		//MKey + NumKeyで命令実行
		bool isPushMKey = Input::Keybord::GetKey(KeyCode::M);

		//Play Music
		if (isPushMKey && Input::Keybord::GetKeyDown(KeyCode::Num1))
		{
			//PlayするMusicを切り替え
			m_playMusicAsset = m_musicAssets[static_cast<int>(m_isPlayIndex)];
			m_isPlayIndex = !m_isPlayIndex;
		
			//Play & Change volume smoothly(0 -> 1)
			m_playMusicAsset->Play(true);
			m_playMusicAsset->setVolume(0.0f);
			m_playMusicAsset->ChangeVolumeSmoothly(1.0f, 1.0f);
			
			//Log
			GUI::getLogStream() << L"Play Music";
		}
		//Change Music
		else if ((!m_isChangeMusic & isPushMKey) && Input::Keybord::GetKeyDown(KeyCode::Num2))
		{
			//Set Flag & Change volume smoothly(1 -> 0)
			m_playMusicAsset->ChangeVolumeSmoothly(0.0f, 1.0f);
			m_isChangeMusic = true;
			//Log
			GUI::getLogStream() << L"Change Music";
		}
		//Stop Music
		else if ((!m_isStopMusic & isPushMKey) && Input::Keybord::GetKeyDown(KeyCode::Num3))
		{
			//Set Flag & Change volume smoothly(1 -> 0)
			m_playMusicAsset->ChangeVolumeSmoothly(0.0f, 1.0f, true);
			m_isStopMusic = true;
			//Log
			GUI::getLogStream() << L"Stop Music";
		}
		//Play SE
		else if (isPushMKey && Input::Keybord::GetKeyDown(KeyCode::Num4))
		{
			//GetState
			m_effectAsset->Play();
			//Log
			GUI::getLogStream() << L"Play SE";
		}
		//Play SE
		else if (isPushMKey && Input::Keybord::GetKeyDown(KeyCode::Num5))
		{
			float x = (MathAF::PingPong(Time::time(), 40.0f) - 20.0f) * 10.0f;

			//GetState
			m_effect3DAsset->Play(Const::Vector3::right * x, Const::Vector3::right * 2.0f * Time::worldDeltaTime(),
				Const::Quaternion::identity);
			//Log
			GUI::getLogStream() << L"Play 3D SE: " << (Const::Vector3::right * x).x;
		}
		//Stop
		if (m_isStopMusic && m_playMusicAsset->getVolume() == 0.0f)
		{
			//m_playMusicAsset->Stop();
			m_isStopMusic = false;
		}
		//Change
		if (m_isChangeMusic && m_playMusicAsset->getVolume() == 0.0f)
		{
			m_playMusicAsset->Stop();
			
			//PlayするMusicを切り替え
			m_playMusicAsset = m_musicAssets[static_cast<int>(m_isPlayIndex)];
			m_isPlayIndex = !m_isPlayIndex;
			
			//Play & Change volume smoothly(0 -> 1)
			m_playMusicAsset->Play(true);
			m_playMusicAsset->setVolume(0.0f);
			m_playMusicAsset->ChangeVolumeSmoothly(1.0f, 1.0f);
			m_isChangeMusic = false;
		}
	}

private:
	SharedPointer<Asset::AudioAssetMusic> m_playMusicAsset;
	SharedPointer<Asset::AudioAssetMusic> m_musicAssets[2];
	SharedPointer<Asset::AudioAssetEffect> m_effectAsset;
	SharedPointer<Asset::AudioAssetEffect3D> m_effect3DAsset;

	bool m_isStopMusic = false;
	bool m_isChangeMusic = false;
	bool m_isPlayIndex = false;
};

#endif //!DEMO_MUSIC_PLAYER_HPP_