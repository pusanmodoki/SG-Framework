#ifndef SGFRAMEWORK_HEADER_SCENE_MANAGER_HPP_
#define SGFRAMEWORK_HEADER_SCENE_MANAGER_HPP_
#include <stack>
#include <vector>
#include "../02_CustomVariable/CustomVariable.hpp"
#include "Scene.hpp"

//Framework namespace
namespace SGFramework
{
	class GUI;

	//Scene
	namespace Scene
	{
		//Sceneを管理するSceneManager class
		//このクラスの関数は全て非スレッドセーフです
		class SceneManager final
		{
		public:
			//friend
			friend class Application;
			friend class GUI;

			//----------------------------------------------------------------------------------
			//[コンストラクタ]
			//シーンマネージャーの登録を行う
			//引数1: 最初に読み込むシーン, InstantiateFirstSceneで指定する 
			//引数2: デバッグモード
			inline SceneManager(UniquePointer<BaseScene>&& FirstScene, bool isDebugMode);
			//----------------------------------------------------------------------------------
			//[デストラクタ]
			//シーン開放を行う
			inline ~SceneManager();
			//----------------------------------------------------------------------------------
			//[InstantiateFirstScene]
			//最初のシーンインスタンスを作成する
			template<class SceneClass>
			inline static UniquePointer<BaseScene> InstantiateFirstScene();

			//----------------------------------------------------------------------------------
			//[RegisterUnloadSharedPointer]
			//UnloadPointerを登録する, 登録したPointerはシーン削除後も開放されない
			//template: shared pointer type
			//return: 成功したらture, 失敗(すでにkeyが存在)した場合false
			//引数1: pointer
			//引数2: key
			template<class PointerType>
			inline static bool RegisterUnloadSharedPointer(SharedPointer<PointerType> pointer, const sgstring& key);

			//----------------------------------------------------------------------------------
			//[FindUnloadSharedPointer]
			//template: キャストしたSharedPointer, dynamic_castなどで確認を行わないので注意
			//return: 登録したUnloadSharedPointer, keyが無効な場合null pointer
			//引数1: key
			template<class PointerType>
			inline static SharedPointer<PointerType> FindUnloadSharedPointer(const sgstring& key);
			
			//----------------------------------------------------------------------------------
			//[UnregisterUnloadSharedPointer]
			//UnloadSharedPointerを登録解除する
			//return: 成功した場合true, 失敗した場合false
			//引数1: key
			inline static bool UnregisterUnloadSharedPointer(const sgstring& key);

			//now scene (read function property)
			SGF_FUNCTION_PROPERTY static const UniquePointer<BaseScene>& getNowScene()
			{
				return m_scenes.top();
			}
			//unload pointers (read function property)
			SGF_FUNCTION_PROPERTY static uint getNumUnloadPointers()
			{
				return m_unloadCounter.load();
			}

		private:
			//----------------------------------------------------------------------------------
			//[GameStart]
			//最初のシーンのStart処理を行う
			inline static void GameStart();
			//----------------------------------------------------------------------------------
			//[Update]
			//シーンの更新を行う
			inline static void Update();
			//----------------------------------------------------------------------------------
			//[Sync]
			//シーンの同期更新を行う
			//return: Sceneの続行可否
			inline static bool Sync();
			//----------------------------------------------------------------------------------
			//[LateUpdate]
			//シーンの同期更新を行う
			inline static void LateUpdate();
			//----------------------------------------------------------------------------------
			//[FixedUpdate]
			//シーンのFixed並列更新を行う
			inline static void FixedUpdate();
			//----------------------------------------------------------------------------------
			//[MakeDrawCommand]
			//シーンの描画を行う
			inline static void MakeDrawCommand();
			//----------------------------------------------------------------------------------
			//[Release]
			//シーンの全開放を行う
			inline static void Release();
			//----------------------------------------------------------------------------------
			//[LoadScene]
			//次シーンの読み込みを行う
			inline static bool LoadScene();

			//Scenes
			static std::stack<UniquePointer<BaseScene>, std::vector<UniquePointer<BaseScene>>> m_scenes;
			//unload pointers
			static std::unordered_map<std::wstring, Detail::Pointer::BaseCountPointer*> m_unloadSharedPointers;
			static std::mutex m_mutex;		//mutex
			//Unload pointer カウント
			static std::atomic<ushort> m_unloadCounter;	
			static bool m_isEndUpdate;			//更新終了フラグ
			static bool m_isInstantiate;			//インスタンスあるかなフラグ
		};


		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//シーンマネージャーの登録を行う
		//引数1: 最初に読み込むシーン, InstantiateFirstSceneで指定する 
		//引数2: デバッグモード
		inline SceneManager::SceneManager(UniquePointer<BaseScene>&& FirstScene, bool isDebugMode)
		{
			//インスタンス作成していない && ポインタが有効
			if (IS_FALSE(m_isInstantiate) & FirstScene.getIsOwned())
			{
				m_isInstantiate = true;						//代入
				IsDebug::m_isValue = isDebugMode;	//デバッグモード

				//追加
				m_scenes.emplace(std::move(FirstScene));
			}
		}
		//----------------------------------------------------------------------------------
		//[デストラクタ]
		//シーン開放を行う
		inline SceneManager::~SceneManager()
		{
			Release();	//開放
		}
		//----------------------------------------------------------------------------------
		//[InstantiateFirstScene]
		//最初のシーンインスタンスを作成する
		//throw: 既にインスタンスがある場合
		//return: 作成したシーン
		template<class SceneClass>
		inline UniquePointer<BaseScene> SceneManager::InstantiateFirstScene()
		{
			//BaseSceneが基底クラスでないならError
			static_assert(std::is_base_of_v<BaseScene, SceneClass>, L"InstantiateFirstScene template should be the Base of BaseScene class");
			//インスタンスがもうある
			if (IS_TRUE(m_isInstantiate))
			{
				//ErrorMessage
				MessageBox(Window::HwndHolder::main, L"Not Allow Instantiate", L"Error!! SceneManager->InstantiateFirstScene", MB_OK);
				//throw
				throw InvalidArgument(L"Error!! SceneManager->InstantiateFirstScene", L"Not Allow Instantiate");
			}

			//MakeUnique
			auto result = UniquePointer<SceneClass>(new SceneClass());
			//return
			return std::move(result.MovingDownCast<BaseScene>());
		}

		//----------------------------------------------------------------------------------
		//[RegisterUnloadSharedPointer]
		//UnloadPointerを登録する, 登録したPointerはシーン削除後も開放されない
		//return: 成功したらture, 失敗(すでにkeyが存在)した場合false
		//引数1: pointer
		//引数2: key
		template<class PointerType>
		inline bool SceneManager::RegisterUnloadSharedPointer(SharedPointer<PointerType> pointer, const sgstring & key)
		{
			std::lock_guard<std::mutex> guard(m_mutex);
			if (key != L"" && m_unloadSharedPointers.find(key) == m_unloadSharedPointers.end())
			{	
				AtomicOperation::Add(m_unloadCounter, 1U);
				Detail::Pointer::BaseCountPointer* newPointer = new SharedPointer<PointerType>(pointer);
				m_unloadSharedPointers.try_emplace(key, newPointer);
				return true;
			}
			else
				return false;
		}

		//----------------------------------------------------------------------------------
		//[FindUnloadSharedPointer]
		//template: キャストしたSharedPointer, dynamic_castなどで確認を行わないので注意
		//return: 登録したUnloadSharedPointer, keyが無効な場合null pointer
		//引数1: key
		template<class PointerType>
		inline SharedPointer<PointerType> SceneManager::FindUnloadSharedPointer(const sgstring & key)
		{
			std::lock_guard<std::mutex> guard(m_mutex);
			if (m_unloadSharedPointers.find(key) != m_unloadSharedPointers.end())
				return *reinterpret_cast<SharedPointer<PointerType>*>(m_unloadSharedPointers.at(key));
			else
				return SharedPointer<PointerType>::getNull();
		}

		//----------------------------------------------------------------------------------
		//[UnregisterUnloadSharedPointer]
		//UnloadSharedPointerを登録解除する
		//return: 成功した場合true, 失敗した場合false
		//引数1: key
		inline bool SceneManager::UnregisterUnloadSharedPointer(const sgstring & key)
		{
			std::lock_guard<std::mutex> guard(m_mutex);
			if (m_unloadSharedPointers.find(key) != m_unloadSharedPointers.end())
			{
				AtomicOperation::Subtract(m_unloadCounter, 1U);
				delete m_unloadSharedPointers.at(key);
				m_unloadSharedPointers.erase(key);
				return true;
			}
			else
				return false;
		}

		//----------------------------------------------------------------------------------
		//[GameStart]
		//最初のシーンのStart処理を行う
		inline void SceneManager::GameStart()
		{
			SceneBaton batton;
			m_scenes.top()->StartScene(batton);
		}
		//----------------------------------------------------------------------------------
		//[Update]
		//シーンの並列更新を行う
		inline void SceneManager::Update()
		{
			//if ((m_scenes.empty() ^ true) ^ m_isEndUpdate)
			{
				m_scenes.top()->Update();
				m_scenes.top()->UpdateSubScene();
			}
		}
		//----------------------------------------------------------------------------------
		//[Sync]
		//シーンの同期更新を行う
		inline bool SceneManager::Sync()
		{
			//if ((m_scenes.empty() ^ true) ^ m_isEndUpdate)
			{
				bool result = m_scenes.top()->Sync();
				m_scenes.top()->SyncSubScene();
				if (IS_TRUE(result)) return true;
				else return LoadScene();
			}
			//return false;
		}
		//----------------------------------------------------------------------------------
		//[LateUpdate]
		//シーンの同期更新を行う
		inline void SceneManager::LateUpdate()
		{
			//if ((m_scenes.empty() ^ true) ^ m_isEndUpdate)
			{
				m_scenes.top()->LateUpdate();
				m_scenes.top()->LateUpdateSubScene();
			}
		}
		//----------------------------------------------------------------------------------
		//[FixedUpdate]
		//シーンのFixed並列更新を行う
		inline void SceneManager::FixedUpdate()
		{
			//if ((m_scenes.empty() ^ true) ^ m_isEndUpdate)
			{
				m_scenes.top()->FixedUpdate();
				m_scenes.top()->FixedUpdateSubScene();
			}
		}
		//----------------------------------------------------------------------------------
		//[MakeDrawCommand]
		//シーンの描画を行う
		inline void SceneManager::MakeDrawCommand()
		{
			//if ((m_scenes.empty() ^ true) ^ m_isEndUpdate)
			{
				m_scenes.top()->MakeDrawCommand();
				m_scenes.top()->MakeDrawCommandSubScene();
			}
		}
		//----------------------------------------------------------------------------------
		//[Release]
		//シーンの全開放を行う
		inline void SceneManager::Release()
		{
			std::lock_guard<std::mutex> guard(m_mutex);
			
			SceneBaton null;
			uint size = static_cast<uint>(m_scenes.size());
			bool bNull1, bNull2;
			for (uint i = 0; i < size; i++)
			{
				m_scenes.top()->EndScene(null, bNull1, bNull2);
				m_scenes.top().Release();
				m_scenes.pop();
			}

			for (auto& e : m_unloadSharedPointers)
				delete e.second;
			m_unloadSharedPointers.clear();
		}

		//----------------------------------------------------------------------------------
		//[LoadScene]
		//次シーンの読み込みを行う
		inline bool SceneManager::LoadScene()
		{
			std::lock_guard<std::mutex> guard(m_mutex);
	
			//Endで格納される情報
			SceneBaton baton = {};
			bool isClear = false, isPop = false;
			//End, 次シーンポインタゲット
			auto ptr = m_scenes.top()->EndScene(baton, isClear, isPop);

			//Pop
			if (isPop)
			{
				m_scenes.top().Release();
				m_scenes.pop();
			}
			//Clear
			if (IS_TRUE(isClear) & IS_FALSE(m_scenes.empty()))
			{
				Release();
			}

			//ポインタ有効
			if (IS_TRUE(ptr.getIsOwned()))
			{
				//シーン追加
				m_scenes.emplace(std::move(ptr));
				//Start
				ptr->StartScene(baton);
				return true;
			}
			//次シーンポインタ無効&まだシーンがありPopした->一つ戻る
			else if (IS_FALSE(m_scenes.empty()) & IS_TRUE(isPop))
			{
				//Rewind
				m_scenes.top()->RewindScene(baton);
				return true;
			}
			//それ以外は終了ｔみなす
			else
			{
				m_isEndUpdate = true;
				return false;
			}
		}
	}
}
#endif // !SGFRAMEWORK_HEADER_SCENE_MANAGER_HPP_