#include <atlstr.h>
#include "Application.hpp"

//Framework namespace
namespace SGFramework
{
	void Application::StartUp(HINSTANCE hInstance, int nCmdShow)
	{
		using namespace SGFramework::ApplicationSetting;

		try
		{				
			if (GetSystemMetrics(SM_REMOTESESSION))
				m_isRemote = true;

			if (IS_FALSE(Scene::SceneManager::m_isInstantiate))
				throw InvalidArgument(L"Error!! Application->StartUp", L"Instance of SceneManager not found");

			ImportSetting();
			m_wCex = WindowCreate(hInstance, nCmdShow);
		
			if (IS_FALSE(static_cast<bool>(m_applicationSetting.getPack(TiredInfo::cIsEnableCloseButton)[0].valueInt)))
			{
				HMENU menu = GetSystemMenu(Window::HwndHolder::main, false);
				RemoveMenu(menu, SC_CLOSE, MF_BYCOMMAND);
			}

							
			Transform::StartUp(m_applicationSetting.getPack(TiredInfo::cNumFunctionLines));
			
			GabageCollector::StartUp(m_applicationSetting.getPack(TiredInfo::cNumDeleteCapacity), m_applicationSetting.getPack(TiredInfo::cNumDeleteOfOneFlame));
			
			Layer::ReadLayers(m_applicationSetting.getGroop(TiredInfo::cGroopLayers));
			
			Tag::ReadTags(m_applicationSetting.getGroop(TiredInfo::cGroopTags));
			//BaseClass::BaseModelAssetFbx::StartUp();
			
			Thread::StartUp(m_applicationSetting.getPack(TiredInfo::cNumFunctionLines), m_applicationSetting.getPack(TiredInfo::cNumMaxUsingThreads));
			
			Physics::StartUp(m_applicationSetting.getPack(TiredInfo::cGravity), m_applicationSetting.getPack(TiredInfo::cHitPairVectorReserves));
			
			Graphics::StartUp(m_applicationSetting.getPack(TiredInfo::cIsVsync), m_applicationSetting.getPack(TiredInfo::cIsNoRestrictionsFPS), 
				m_applicationSetting.getPack(TiredInfo::cDepth), m_applicationSetting.getPack(TiredInfo::cNear), m_applicationSetting.getPack(TiredInfo::cClearColor), m_isRemote);
			
			Audio::StartUp(m_applicationSetting.getPack(TiredInfo::cMaterVolume), m_applicationSetting.getPack(TiredInfo::cChannels), 
				m_applicationSetting.getPack(TiredInfo::cSampleRate), m_applicationSetting.getPack(TiredInfo::cAudioUpdateInterval),
				m_applicationSetting.getPack(TiredInfo::cAudioSmoothReserve), m_applicationSetting.getGroop(TiredInfo::cGroopSubmixAudios));
			
			Input::StartUp(m_applicationSetting.getPack(TiredInfo::cRapidInterval), m_applicationSetting.getPack(TiredInfo::cMouseDedZone), 
				m_applicationSetting.getPack(TiredInfo::cJoystickDedZone), m_applicationSetting.getPack(TiredInfo::cWindowDedZone),
				m_applicationSetting.getPack(TiredInfo::cIsMouseInsideFrame));
			
			GUI::StratUp(Window::HwndHolder::main, m_applicationSetting.getPack(TiredInfo::cGUIVertexShaderDebugPath),
				m_applicationSetting.getPack(TiredInfo::cGUIPixelShaderDebugPath), m_applicationSetting.getPack(TiredInfo::cGUIVertexShaderReleasePath),
				m_applicationSetting.getPack(TiredInfo::cGUIPixelShaderReleasePath), m_applicationSetting.getPack(TiredInfo::cIsUsingGUI),
				m_applicationSetting.getPack(TiredInfo::cFontSizePerDpi));
			
			Administrator::AssetManager::StartMonitoring();
		}
		catch (InvalidArgument val)
		{
			InvalidMessageDirect(val);
			throw;
		}

		m_mainThread = std::thread(ApplicationMain);
		Thread::setAffinityApplicationMain(m_mainThread.native_handle());
		GUI::RegisterDebugStream(L"ApplicationMain", m_mainThread.get_id());
		GUI::BuildDebugStream();
	}


	void Application::ShutDown()
	{
		if (IS_TRUE(m_mainThread.joinable()))
			m_mainThread.join();
		GUI::ShutDown();
		Thread::ShutDown();
		Administrator::AssetManager::EndMonitoring();
		GabageCollector::GabageCollection();
		
		Scene::SceneManager::Release();
		GabageCollector::GabageCollection();
		
		Transform::Shutdown();
		Input::Shutdown();
		Audio::ShutDown();
		Graphics::ShutDown();
		Physics::ShutDown();
		GabageCollector::ShutDown();
		//BaseClass::BaseModelAssetFbx::ShutDown();

		UnregisterClass(m_productName, Window::HinstanceHolder::main);
	}
}