#include "Application.hpp"

//Framework namespace
namespace SGFramework
{
	int Application::ApplicationMain()
	{
		using namespace SGFramework::ApplicationSetting;
		static uint s_fixedCounter = 0;

		Random::InitMainThread();
		Graphics::InitCommandList();

		Time::StartTimeProcessing(m_applicationSetting.getPack(TiredInfo::cIsNoRestrictionsFPS), 
			m_applicationSetting.getPack(TiredInfo::cFixedInterval), m_applicationSetting.getGroop(TiredInfo::cGroopTimeLayers), m_isRemote);

		Scene::SceneManager::GameStart();

		//------------------------------------------------------------------------------- 
		//ゲームループ
		while (IS_FALSE(m_isEndUpdate.load()))
		{
			//FixedUpdate
			while (Time::FixedUpdate() & Administrator::AssetManager::m_isNotStopUpdate)
			{
				//Update Ready?
				if (IS_TRUE(m_isUpdateReady.load()))
				{
					////PhysX
					//Physics::BeginUpdate();

					////Transform更新 & callback
					//Thread::MainThreadInvokeCallbacks(Component::CallbackFunctions::FixedUpdate);
					//Scene::SceneManager::FixedUpdate();
					////Transform更新
					//Thread::MainThreadUpdate();

					////PhysX
					//Physics::Update();
					Physics::SyncPhysicsBegin();

					//GameObject::m_destroyObjects.clear();

					//GabageCollection
					//GabageCollector::GabageCollection();


					Physics::UpdatePhysics();
					Physics::SyncPhysicsEnd();
					++s_fixedCounter;
				}
				//Time判定
				Time::FixedLastUpdate();

				if (s_fixedCounter >= 5)
					Time::setFourceUpdate();
			}

			//Update
			if (Time::Update() & Administrator::AssetManager::m_isNotStopUpdate)
			{
				//Background
				{
					//Wait
					Thread::WaitAudioGraphicsUpdate();

					GameObject::m_destroyObjects.clear();

					//GabageCollection
					GabageCollector::GabageCollection();

					//Thread Start Update
					Thread::MainThreadBeginBeforeSync();

					//Scene Update
					Scene::SceneManager::Update();

					//Thread Sync
					Thread::MainThreadEndBeforeSync();

					//音と画の更新開始
					Thread::StartAudioGraphicsUpdate();
				}

				//Update Ready?
				if (IS_TRUE(m_isUpdateReady.load()))
				{
					//Sync開始
					Transform::StartSyncFrame();
					//callback
					Thread::MainThreadSingleInvokeCallbacks(Component::CallbackFunctions::Sync);
					//Scene Sync
					if (IS_FALSE(Scene::SceneManager::Sync()))
					{
						Transform::EndSyncFrame();
						Application::Quit();
						break;
					}
					//Sync終了
					Transform::EndSyncFrame();
				}

				//Thread Start Update
				Thread::MainThreadBeginAfterSync();

				//Update Ready?
				if (IS_TRUE(m_isUpdateReady.load()))
					Scene::SceneManager::LateUpdate();

				//Meke Draw Command
				//Thread::MainThreadUpdate(Component::CallbackFunctions::MakeDrawCommand);
				Scene::SceneManager::MakeDrawCommand();
				
				//Thread Sync
				Thread::MainThreadEndAfterSync();
			}
		}

		//sleep
		std::this_thread::sleep_for(std::chrono::milliseconds(2));
		return 0;
	}
}