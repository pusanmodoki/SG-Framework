/*----------------------------------------------------------------------------------
GUIを管理するGUI class
------------------------------------------------------------------------------------*/
#include "GUI.hpp"
#include "../08_Input/Input.hpp"
#include "../05_Graphics/Graphics.hpp"
#include "../97_Application/Application.hpp"

// Framework namespace
namespace SGFramework
{
	//----------------------------------------------------------------------------------
	//[BuildDebugStream]
	//DebugStreamを登録情報を基に構築する
	void GUI::BuildDebugStream()
	{
		m_drawConsoleMessages.reserve(m_cDebugDrawReserveSize);
	}

	//----------------------------------------------------------------------------------
	//[SyncDebugStream]
	//DebugStreamからConsoleMessageへ変換し保存する
	void GUI::SyncDebugStream()
	{
		//読み込みバッファ
		static wchar readBuffer[m_cBufferLength] = { L'\0' };

		//クリア = true->コマンドバッファクリア
		if (IS_TRUE(m_isClearDebugStreams.load()))
		{
			m_drawConsoleMessages.clear();
			AtomicOperation::Init(m_isClearDebugStreams, false);
		}

		//現在時刻
		std::string timeString(Time::CurrentTimeToStdString());

		//ストリームループ
		for (auto& e : m_debugStreams)
		{
			//ヘッダー = time + thread name
			std::string header = (timeString + "(" + e.second.threadName + "): ");

			//getline->console messageに変換、スタック
			while (e.second.stream.getline(readBuffer, m_cBufferLength))
				m_drawConsoleMessages.emplace_back(ConsoleMessage(header, readBuffer));

			//このままだとストリームが使えなくなるのでswap
			std::wstringstream swap;
			e.second.stream.swap(swap);
		}
	}

	//----------------------------------------------------------------------------------
	//[DrawDebugStream]
	//DebugStream(ConsoleMessage)を描画する
	void GUI::DrawDebugStream()
	{
		//Show Window?
		static bool s_isShowWindow = false;

		//Ctrl + F12->切り替え
		if ((Input::Keybord::GetKeyDown(KeyCode::Control) ||
			Input::Keybord::GetKeyDown(KeyCode::F12))
			&& (Input::Keybord::GetKey(KeyCode::Control) &&
				Input::Keybord::GetKey(KeyCode::F12)))
			s_isShowWindow = !s_isShowWindow;

		//描画なし->終了
		if (IS_FALSE(s_isShowWindow)) return;

		// We specify a default position/size in case there's no data in the .ini file. Typically this isn't required! We only do it to make the Demo applications a little more welcoming.
		Vector2 screenSize = Graphics::screen.screenSize->ToFloat();
		ImGui::SetNextWindowPos(ImVec2(screenSize.x * 0.9f, screenSize.y * 0.8f), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(screenSize.x * 0.2f, screenSize.y * 0.4f), ImGuiCond_FirstUseEver);

		//Window Open
		if (!ImGui::Begin("SGFramework Debug Window", nullptr))
		{
			ImGui::End();
			return;
		}
		else
		{
			//Now Time Datas
			if (Time::targetFrameRate() != VariableLimit::cFloatMax)
				ImGui::Text("FPS: %f, Target frame rate: %f, World time scale: %f", 
					Time::fps(), Time::targetFrameRate(), Time::worldTimeScale());
			else
				ImGui::Text("FPS: %f, Target frame rate: Infinity, World time scale: %f",
					Time::fps(), Time::worldTimeScale());

			ImGui::Text("World deltaTime-> Scaled: %f, Unscaled: %f",
				Time::worldDeltaTime(), Time::unScaledWorldDeltaTime());

			ImGui::Text("Fixed deltaTime-> Scaled: %f, Unscaled: %f",
				Time::fixedDeltaTime(), Time::unScaledFixedDeltaTime());
			ImGui::Spacing();

			bool isStopApplication = Application::m_isUpdateReady.load() ^ true;
			ImGui::Checkbox("Stop application", &isStopApplication);
			isStopApplication ^= true;
			AtomicOperation::Init(Application::m_isUpdateReady, isStopApplication);
			ImGui::Spacing();

			//Draw Application Infomation
			DrawApplicationInfomations();

			//Draw Scene Infomation
			DrawSceneInfomations();
			
			//Draw Debug Console
			DrawDebugConsole();

			//End
			ImGui::End();
		}
	}

	//----------------------------------------------------------------------------------
	//[DrawApplicationInfomations]
	//DebugStream->Draw Application Infomations 
	void GUI::DrawApplicationInfomations()
	{
		//Header
		if (ImGui::CollapsingHeader("Application Infomations"))
		{
			//Application Infos
			ImGui::Text("Application name: %s", Application::productName->to_std_string().c_str());
			ImGui::Text("Version: %s", Application::version->to_std_string().c_str());

			//Hardware Infomation Node
			ImGui::Separator();
			if (ImGui::TreeNode("Hardware Infomation"))
			{
				ImGui::Text("Hardware all threads: %d", Thread::numHardwareThreads());
				ImGui::Text("Framework all running threads: %d", Thread::getNumRunningFrameworkThreads());
				ImGui::Text("Framework all using hardware threads: %d", Thread::getNumFrameworkUsingHardwareThreads());
				ImGui::Text("Framework all general threads: %d", Thread::numUpdateThreads());
				ImGui::Text("Function lines: %d", Thread::numFunctionLines());
				ImGui::Spacing();

				ImGui::Text("Video card name: %s", Graphics::videoCardName->to_std_string().c_str());
				ImGui::Text("Screen resolution: %s", Graphics::screen.resolution->ToStdString().c_str());
				ImGui::Text("Screen dpi: %s", Graphics::screen.dpi->ToStdString().c_str());
				ImGui::Text("Application window size: %s", Graphics::screen.windowSize->ToStdString().c_str());
				ImGui::Text("Application screen Size: %s", Graphics::screen.screenSize->ToStdString().c_str());
				ImGui::TreePop();
			}

			//Time Infomation Node
			ImGui::Separator();
			if (ImGui::TreeNode("Time Infomation"))
			{
				ImGui::Text("StartUp elapased time: %f", Time::time());

				ImGui::Spacing();
				ImGui::Text("Time Layers:");
				{
					ShowTimeLayers(Time::rootTimeLayer());
				}

				ImGui::TreePop();
			}
			
			//Audio Infomation Node
			ImGui::Separator();
			if (ImGui::TreeNode("Audio Infomation"))
			{
				ImGui::Text("Master channels: %s", Audio::getChannelsString().to_std_string().c_str());
				ImGui::Text("Master sample rate: %d", Audio::sampleRate());

				ImGui::Spacing();
				ImGui::Text("Submix Layers:");
				{
					ShowAudioLayers(Audio::submixRootLayer());
				}
				ImGui::Text("Submix Monoral Layers:");
				{
					ShowAudioLayers(Audio::submixMonoralRootLayer());
				}

				ImGui::TreePop();
			}

			//Input Infomation Node
			ImGui::Separator();
			if (ImGui::TreeNode("Input Infomation"))
			{
				ImGui::Text("Any:");
				ImGui::Text("Enabled any: %s", IS_TRUE(Input::Any::isAnyMode()) ? "true" : "false");
				ImGui::Spacing();

				ImGui::Text("Mouse:");
				ImGui::Text("Enabled mouse inside frame: %s", IS_TRUE(Input::Mouse::isMouseInsideFrame()) ? "true" : "false");
				ImGui::Text("Move acceleration ded zone: %f", Input::Mouse::dedZone());
				ImGui::Spacing();

				ImGui::Text("Joystick:");
				ImGui::Text("Enabled joysticks: %s", IS_TRUE(Input::Joystick::isUseJoystick()) ? "true" : "false");
				ImGui::Text("Connect joysticks: %d", Input::Joystick::numUseJoysticks());
				ImGui::Text("Analog acceleration ded zone: %f", Input::Joystick::dedZone());
				ImGui::Spacing();

				ImGui::Text("Window:");
				ImGui::Text("Move acceleration ded zone: %f", Input::Window::dedZone());
				ImGui::Spacing();

				ImGui::Text("User Axes:");
				if (Input::m_axisPacks.size() == 0)
				{
					ImGui::Text("Not register user axes");
				}
				for (auto& e : Input::m_axisPacks)
				{
					std::string myName = "Axis name: " + e.first.to_std_string();
					if (ImGui::TreeNode(myName.c_str()))
					{
						ImGui::Text("Register axis codes: %d", e.second.axisCodes.size());
						ImGui::Text("Register puls axis joystic codes: %d", e.second.joystickCodeWithPlus.size());
						ImGui::Text("Register minus axis joystic codes: %d", e.second.joystickCodeWithMinus.size());
						ImGui::Text("Register puls axis key and mouse codes: %d", e.second.keyAndMouseCodeWithPlus.size());
						ImGui::Text("Register minus axis key and mouse codes: %d", e.second.keyAndMouseCodeWithMinus.size());
						ImGui::TreePop();
					}
				}
				ImGui::TreePop();
			}

			//End Infomations
			ImGui::Separator();
		}
	}

	//----------------------------------------------------------------------------------
	//[DrawSceneInfomations]
	//DebugStream->Draw Scene Infomations
	void GUI::DrawSceneInfomations()
	{
		//Header
		if (ImGui::CollapsingHeader("Scene Infomations"))
		{
			ImGui::Text("All instantiate object classes: %d", Object::getNumNowTotaleInstantiateObjects());

			//Scene Infomation Node
			if (ImGui::TreeNode("Scene Infomation"))
			{
				//SceneManager lock(裏でUpdateとガベージコレクタが走っているため)
				std::lock_guard<std::mutex> guard(Scene::SceneManager::m_mutex);
				
				//Now Scene
				ImGui::Text("Now Scene:");
				ShowScene(Scene::SceneManager::getNowScene());

				ImGui::Spacing();

				//All Scenes Node
				if (ImGui::TreeNode("All Load Scenes"))
				{
					for (auto& e : Scene::SceneManager::m_scenes._Get_container())
						ShowScene(e);
					ImGui::TreePop();
				}
				ImGui::TreePop();
			}

			//Asset Infomation Node
			if (ImGui::TreeNode("Asset Infomation"))
			{
				//AssetManager lock(裏で監視スレッドが走っているため)
				Administrator::AssetManager::LockGuardAsset(Administrator::AssetManager::m_lock, false);

				//Assets roop
				for (auto& e : Administrator::AssetManager::m_assets)
				{
					//Pointer Lockできなければ描画を行わない
					if (IS_FALSE(e.second.LockShared())) continue;

					std::string path = "File path: " + e.second->name().to_std_string();

					//Asset Detail Node
					if (ImGui::TreeNode(path.c_str()))
					{
						std::string str = "File size (KB): %f";
						float fileSize = e.second->fileSizes->getKiloByte();
						
						//ファイルサイズの単位を調整
						if (fileSize > 1024.0f)
						{
							fileSize = e.second->fileSizes->getMegaByte();
							str = "File size (MB): %f";
							if (fileSize > 1024.0f)
							{
								fileSize = e.second->fileSizes->getGigaByte();
								str = "File size (GB): %f";
							}
						}
						
						ImGui::Text("Asset Type: %s", Asset::StdStringForAssetType(e.second->getType()).c_str());
						ImGui::Text("Instace ID: %d", e.second->instanceID());
						ImGui::Text("Enabled shared: %s", IS_TRUE(e.second->getIsSharedAsset()) ? "true" : "false");
						ImGui::Text(str.c_str(), fileSize);
						ImGui::Text("Last write time: %s", e.second->lastWriteTime->ToString(true, true).to_std_string().c_str());
						ImGui::TreePop();
					}
					//Pointer Unlock
					e.second.UnlockShared();
				}
				ImGui::TreePop();
			}

			//Transform Objects Viewer Node
			if (ImGui::TreeNode("Transform Objects Viewer"))
			{
				//RootList Lock(裏でUpdate, ガベージコレクタが走っているため)
				AtomicOperation::LockGuard guard(Transform::m_rootList.m_lock);
				
				//Transforms Loop
				for (size_t i = 0, max = Transform::m_rootList.m_list.size(); i < max; ++i)
				{
					std::string header = "Function line: " + std::to_string(i)
						+ ", Num root objects: " + std::to_string(Transform::m_rootList.m_list[i].size());
					
					//Transform Detail Node
					if (ImGui::TreeNode(header.c_str()))
					{						
						for (auto& e : Transform::m_rootList.m_list[i])
							ShowTransform(e.second);
						ImGui::TreePop();
					}
				}
				ImGui::TreePop();
			}
		}
	}

	//----------------------------------------------------------------------------------
	//[DrawDebugConsole]
	//DebugStream->Draw Debug Console Window
	void GUI::DrawDebugConsole()
	{
		//Header
		if (ImGui::CollapsingHeader("Debug Console"))
		{
			//Begin Child Window
			ImGui::BeginChild("Console Window", 
				ImVec2(ImGui::GetWindowContentRegionWidth(), Graphics::screen.screenSizeFloat->y * 0.5f),
				true, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysVerticalScrollbar
						| ImGuiWindowFlags_MenuBar);
			
			//MenuBar
			if (ImGui::BeginMenuBar())
			{
				//Menu
				if (ImGui::BeginMenu("Console Window"))
				{
					//Clear->Set Flag
					if (ImGui::MenuItem("Clear Console")) 
					{
						AtomicOperation::Init(m_isClearDebugStreams, true);
					}
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}
			
			//Message Loop
			for (auto& e : m_drawConsoleMessages)
			{
				e.Draw();	
				//スクロールを下へ
				ImGui::SetScrollHereY(1.0f);
			}

			//End Child Window
			ImGui::EndChild();
		}
	}

	//----------------------------------------------------------------------------------
	//[ShowTimeLayers]
	//DebugStream->Draw Time Layers
	//引数1: draw layer
	void GUI::ShowTimeLayers(const WeakPointer<Time::TimeLayer>& layer)
	{
		std::string myName = "Layer name: " + layer->layerName->to_std_string();

		//Layer Detail Node
		if (ImGui::TreeNode(myName.c_str()))
		{
			ImGui::Text("ID: %d, Scale: %f", layer->layerID(), layer->timeScale());
			ImGui::Text("Delta: %f, Fixed: %f", layer->deltaTime(), layer->fixedDeltaTime());
			ImGui::Text("Unscaled delta: %f, Unscaled fixed: %f", layer->unscaledDeltaTime(), layer->unscaledFixedDeltaTime());
			
			//Children Exists? ->Draw
			if (layer->getNumChildrens() > 1)
			{
				ImGui::Spacing();
				ImGui::Text("Childrens:");
				for (auto it = layer->getChildrensIteratorBegin(); it != layer->getChildrensIteratorEnd(); ++it)
					ShowTimeLayers(*it);
			}

			ImGui::TreePop();
		}
	}

	//----------------------------------------------------------------------------------
	//[ShowAudioLayers]
	//DebugStream->Draw Audio Submix Layers
	//引数1: draw layer
	void GUI::ShowAudioLayers(const WeakPointer<Audio::SubmixLayer>& layer)
	{
		std::string myName = "Layer name: " + layer->layerName->to_std_string();

		//Layer Detail Node
		if (ImGui::TreeNode(myName.c_str()))
		{
			ImGui::Text("ID: %d, Cahnnels: %s", layer->layerID(), layer->getChannelsString().to_std_string().c_str());
			ImGui::Text("Volue: %f, Processing stage: %d", layer->getVolume(), layer->processingStage());

			//Children Exists? ->Draw
			if (layer->getNumChildrens() > 1)
			{
				ImGui::Spacing();
				ImGui::Text("Childrens:");
				for (auto it = layer->getChildrensIteratorBegin(); it != layer->getChildrensIteratorEnd(); ++it)
					ShowAudioLayers(*it);
			}

			ImGui::TreePop();
		}
	}

	//----------------------------------------------------------------------------------
	//[ShowScene]
	//DebugStream->Draw Sub Scenes
	//引数1: draw scene
	void GUI::ShowScene(const UniquePointer<Scene::BaseScene>& scene)
	{
		ImGui::Text("Scene name: %s", scene->getSceneName().to_std_string().c_str());
		ImGui::Text("Scene id: %d", scene->m_sceneID);
		ImGui::Spacing();

		//SubScene Node
		if (scene->numSubScenes > 0 
			&& ImGui::TreeNode("Sub Scenes"))
		{
			//Lock Guard (裏でUpdate, ガベージコレクタが走っているため)
			std::lock_guard<std::mutex> subGuard(scene->m_mutex);

			//Draw Loop
			for (auto& subScene : scene->m_subScenes)
			{
				ImGui::Text("Sub scene name: %s", subScene.second->getSceneName().to_std_string().c_str());
				ImGui::Text("Sub scene id: %d", subScene.second->sceneID());
				ImGui::Text("Sub scene sub id: %d", subScene.second->subID());
				ImGui::Text("Sub scene isActive: %s", IS_TRUE(subScene.second->isActive()) ? "true" : "false");
			}
			ImGui::TreePop();
		}
	}

	//----------------------------------------------------------------------------------
	//[ShowTransform]
	//DebugStream->Draw Transforms
	//引数1: draw transform
	void GUI::ShowTransform(Transform* transform)
	{
		//Transform Detail Node
		if (ImGui::TreeNode(("Name: " + transform->name->to_std_string()).c_str()))
		{
			{
				//Variables Lock (一々呼び出されるので効率化)
				Transform::LockGuardTransform guard(transform->m_lock);

				ImGui::Text("Instance ID: %d", transform->instanceID());
				ImGui::Text("Tag: %s", transform->getTag().getName().to_std_string().c_str());
				ImGui::Text("Layer: %s", transform->getLayer().getLayaerName().to_std_string().c_str());
				ImGui::Text("Attached game object: %s", IS_TRUE(transform->gameObject.getIsValid()) ? "true" : "false");
				ImGui::Text("Attached time layer: %s", IS_TRUE(transform->timeLayer.getIsValid()) ? 
					transform->timeLayer->layerName->to_std_string().c_str() : "Not attached");
				ImGui::Text("Num attached components: %d", transform->m_components.size());
				ImGui::Spacing();

				//World Infomation Node
				if (ImGui::TreeNode("World Infomation"))
				{
					ImGui::Text("World position: %s", transform->getWorldPosition().ToStdString().c_str());
					ImGui::Text("World rotation: %s", transform->getWorldRotation().ToStdString().c_str());
					ImGui::Text("World scale: %s", transform->getWorldScale().ToStdString().c_str());
					ImGui::TreePop();
				}
				//Local Infomation Node
				if (ImGui::TreeNode("Local Infomation"))
				{
					ImGui::Text("Local position: %s", transform->getLocalPosition().ToStdString().c_str());
					ImGui::Text("Local rotation: %s", transform->getLocalRotation().ToStdString().c_str());
					ImGui::Text("Local scale: %s", transform->getLocalScale().ToStdString().c_str());
					ImGui::TreePop();
				}
				//Flags Infomation Node
				if (ImGui::TreeNode("Flags"))
				{
					ImGui::Text("Is active: %s", IS_TRUE(transform->getIsActiveSelf()) ? "true" : "false");
					ImGui::Text("Is static: %s", IS_TRUE(transform->getIsStatic()) ? "true" : "false");
					ImGui::Text("Is deleted: %s", IS_TRUE(transform->getIsDestroy()) ? "true" : "false");
					//Attachd GameObject->Draw GameObject Flags
					if (IS_TRUE((transform->gameObject.getIsValid())))
					{
						ImGui::Text("Game object-> is enabled find: %s", IS_TRUE(transform->gameObject->cIsFindEnabled) ? "true" : "false");
						ImGui::Text("Game object-> is auto gabage collection: %s", IS_TRUE(transform->gameObject->cIsUsingAutoGabageCollection) ? "true" : "false");
						ImGui::Text("Game object-> is using mutex pointer: %s", IS_TRUE(transform->gameObject->cIsUsingMutexPointer) ? "true" : "false");
					}
					ImGui::TreePop();
				}
			}
			ImGui::Spacing();
			
			//Childrens Node
			if (transform->m_childrens.size() > 0
				&&ImGui::TreeNode("Childrens"))
			{
				for (auto& e : transform->m_childrens)
				{
					ImGui::Separator();
					ShowTransform(e.getPointer());
				}
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}
		ImGui::Separator();
	}
}