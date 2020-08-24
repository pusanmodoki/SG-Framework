#ifndef SGFRAMEWORK_HEADER_SCENE_HPP_
#define SGFRAMEWORK_HEADER_SCENE_HPP_
#include "../02_CustomVariable/CustomVariable.hpp"

//Framework namespace
namespace SGFramework
{
	class GUI;

	//Scene
	namespace Scene
	{
		//シーン引き継ぎ情報を格納するSceneBaton
		struct SceneBaton
		{
			std::vector<int> valueInts;
			std::vector<float> valueFloats;
			std::vector<sgstring> valueStrings;
			std::vector<Vector3> valueVectors;
			std::vector<SharedPointer<Object>> valueObjects;
		};

		class BaseScene;	//フレームワークで管理するシーンのベースとなるBaseScene class
		class SubScene;		//フレームワークで管理するサブシーンのベースとなるSubBaseScene class
		
		//フレームワークで管理するシーンのベースとなるBaseScene class
		class BaseScene
		{
		public:
			//friend
			friend class SceneManager;
			friend class GUI;

			//----------------------------------------------------------------------------------
			//[デストラクタ]
			//特に何もしない
			inline virtual ~BaseScene(){}

			//----------------------------------------------------------------------------------
			//[StartScene]
			//シーン作成時自動で呼び出されるコールバック関数
			//引数1: 前シーンからの引き継ぎ情報
			virtual void StartScene(SceneBaton& nextBaton) = 0;

			//----------------------------------------------------------------------------------
			//[EndScene]
			//シーン終了時自動で呼び出されるコールバック関数, 終了処理を行う
			//返り値がnullポインタだった場合、アプリケーションが終了する
			//引数1: 次シーンへ引き継ぐ情報, Startで呼び出される
			//引数2: trueを代入すると次シーン以外のスタックを本シーン含めてスタックからクリアする
			//引数2: trueを代入すると本シーンをスタックからする
			virtual UniquePointer<BaseScene> EndScene(SceneBaton& nextBaton, bool& isStackClear, bool& isStackPop) = 0;

			//----------------------------------------------------------------------------------
			//[RewindScene]
			//シーンが終了し次シーンなし & isPopで本シーンに戻ってきた場合に
			//自動で呼び出されるコールバック関数
			//引数1: 次シーンからの引き継ぐ情報
			inline virtual void RewindScene(SceneBaton& nextBaton) {}
			
			//----------------------------------------------------------------------------------
			//[Update]
			//並列更新時自動で呼び出されるコールバック関数
			//他のコールバックをすべて呼び出した後呼び出しされ、オブジェクトの更新等を行う
			inline virtual void Update() {}
			//----------------------------------------------------------------------------------
			//[Sync]
			//同期更新時自動で呼び出されるコールバック関数
			//Update後にシングルスレッドで呼び出され、変数の共有や同期を行う
			//return: シーン続行 -> true
			virtual bool Sync() = 0;

			//----------------------------------------------------------------------------------
			//[LateUpdate]
			//Update & Sync終了後の並列更新時自動で呼び出されるコールバック関数
			//他のコールバックと同時に並列呼び出しされ、オブジェクトの更新等を行う
			inline virtual void LateUpdate() {}

			//----------------------------------------------------------------------------------
			//[FixedUpdate]
			//Fixedフレームでの並列更新時自動で呼び出されるコールバック関数
			//他のコールバックと同時に並列呼び出しされ、オブジェクトの更新等を行う
			inline virtual void FixedUpdate() {}			

			//----------------------------------------------------------------------------------
			//[MakeDrawCommand]
			//描画更新時自動で呼び出されるコールバック関数
			//描画登録等を行う
			inline virtual void MakeDrawCommand() {}

			//----------------------------------------------------------------------------------
			//[FindSubScene]
			//SubSceneを検索する
			//return: 見つかればsubSceneのWeakポインタ, 見つからんかったらnull
			//引数1: SubScene ID(シーン番号 << 16 | サブシーン番号)
			inline WeakPointer<SubScene> FindSubScene(uint subSceneID);
			//----------------------------------------------------------------------------------
			//[FindSubScene]
			//SubSceneを検索する
			//return: 見つかればsubSceneのWeakポインタ, 見つからんかったらnull
			//引数1: SubScene Name
			inline WeakPointer<SubScene> FindSubScene(const sgstring& sceneName);
			//----------------------------------------------------------------------------------
			//[SubSceneBegin]
			//return: SubScene->Begin(非スレッドセーフ)
			inline auto SubSceneBegin();
			//----------------------------------------------------------------------------------
			//[SubSceneEnd]
			//return: SubScene->End(非スレッドセーフ)
			inline auto SubSceneEnd();

			//scene name (read function property)
			SGF_FUNCTION_PROPERTY virtual sgstring getSceneName() const = 0;
			
			GetOnlyProperty<uint> sceneID;
			GetOnlyProperty<ushort> numSubScenes;

		protected:
			//----------------------------------------------------------------------------------
			//[コンストラクタ]
			//カウンターを初期化する
			BaseScene() : m_subIDCounter(0), sceneID(m_sceneID), numSubScenes(m_subIDCounter) {}
			
			//----------------------------------------------------------------------------------
			//[OpenSubScene]
			//Subシーンを開く
			//return: 作成したサブシーンのシェアードポインタ(シーン外に持ち出し厳禁)
			//引数1: Subシーンへのバトンポインタ, nullptr代入可能
			//引数2: 作成されるSubシーンのIDが代入される, nullptr代入可能
			template<class SceneClass>
			inline SharedPointer<SubScene> OpenSubScene(SceneBaton* baton, uint* id);
			//----------------------------------------------------------------------------------
			//[CloseSubScene]
			//Subシーンを閉じる
			//引数1: SubシーンのID
			inline void CloseSubScene(uint id);
			//----------------------------------------------------------------------------------
			//[CloseSubSceneAll]
			//Subシーンを全て閉じる
			inline void CloseSubSceneAll();

	private:
			//----------------------------------------------------------------------------------
			//[UpdateSubScene]
			//SubSceneの並列更新を行う
			inline void UpdateSubScene();
			//----------------------------------------------------------------------------------
			//[SyncSubScene]
			//SubSceneの同期更新を行う
			inline void SyncSubScene();
			//----------------------------------------------------------------------------------
			//[LateUpdateSubScene]
			//SubSceneの並列更新を行う
			inline void LateUpdateSubScene();
			//----------------------------------------------------------------------------------
			//[SubSceneFixedParallelUpdate]
			//SubSceneのFixed並列更新を行う
			inline void FixedUpdateSubScene();
			//----------------------------------------------------------------------------------
			//[MakeDrawCommand]
			//SubSceneの並列描画登録を行う
			inline void MakeDrawCommandSubScene();
			
			//SubSceneたち
			std::unordered_map<uint, SharedPointer<SubScene>> m_subScenes;
			//mutex
			std::mutex m_mutex;
			//SubシーンのIDカウンター
			ushort m_subIDCounter;
			//シーン番号, サブシーン番号はシーン番号 << 16 | サブシーン番号
			uint m_sceneID;
		};

		//フレームワークで管理するサブシーンのベースとなるSubBaseScene class
		class SubScene
		{
		public:
			//----------------------------------------------------------------------------------
			//[デストラクタ]
			//特に何もしない
			inline virtual ~SubScene() {}
			//----------------------------------------------------------------------------------
			//[Start]
			//シーン作成時自動で呼び出されるコールバック関数
			//引数1: 親シーンからの引き継ぎ情報(nullptrな可能性あり)
			virtual void StartScene(SceneBaton* parentBaton) = 0;
			//----------------------------------------------------------------------------------
			//[EndScene]
			//シーン終了時自動で呼び出されるコールバック関数, 終了処理を行う
			virtual void EndScene() {};
			//----------------------------------------------------------------------------------
			//[Update]
			//並列更新時自動で呼び出されるコールバック関数
			//他のコールバックをすべて呼び出した後呼び出しされ、オブジェクトの更新等を行う
			inline virtual void Update() {};
			//----------------------------------------------------------------------------------
			//[Sync]
			//同期更新時自動で呼び出されるコールバック関数
			//Update後に呼び出され、変数の共有や同期を行う
			//return: シーン続行 -> true
			virtual bool Sync() = 0;
			//----------------------------------------------------------------------------------
			//[LateUpdate]
			//Update & Sync終了後の並列更新時自動で呼び出されるコールバック関数
			//他のコールバックと同時に並列呼び出しされ、オブジェクトの更新等を行う
			inline virtual void LateUpdate() {}
			//----------------------------------------------------------------------------------
			//[FixedUpdate]
			//Fixedフレームでの並列更新時自動で呼び出されるコールバック関数
			//オブジェクトの更新登録, 初期情報登録等を行う
			inline virtual void FixedUpdate() {}
			//----------------------------------------------------------------------------------
			//[MakeDrawCommand]
			//描画更新時自動で呼び出されるコールバック関数
			//描画登録等を行う
			virtual void MakeDrawCommand() {}

			//set scene isActive property (set only)
			//argument 1: setting value
			inline bool setActive(bool set) { return m_isActive = set; }

			//scene name (read function property)
			SGF_FUNCTION_PROPERTY virtual sgstring getSceneName() const = 0;
			
			GetOnlyProperty<ushort> subID;		//サブシーンID
			GetOnlyProperty<uint> sceneID;		//シーンID
			GetOnlyProperty<bool> isActive;		//Active

		protected:
			//----------------------------------------------------------------------------------
			//[コンストラクタ]
			//代入初期化
			//引数1: サブシーンID
			SubScene(ushort id, uint parentID) : sceneID(m_sceneID), subID(m_subID), isActive(m_isActive), m_sceneID((parentID << 16) | id), m_subID(id), m_isActive(true) {}
		
		private:
			uint m_sceneID;		//SceneID
			ushort m_subID;		//SubID
			bool m_isActive;		//Active
		};

		//----------------------------------------------------------------------------------
		//[FindSubScene]
		//SubSceneを検索する
		//return: 見つかればsubSceneのWeakポインタ, 見つからんかったらnull
		//引数1: SubScene ID (シーン番号 << 16 | サブシーン番号)
		inline WeakPointer<SubScene> BaseScene::FindSubScene(uint subSceneID)
		{
			std::lock_guard<std::mutex> guard(m_mutex);

			auto find = m_subScenes.find(subSceneID);

			if (find != m_subScenes.end())
				return find->second;
			else
				return WeakPointer<SubScene>::getNull();
		}
		//----------------------------------------------------------------------------------
		//[FindSubScene]
		//SubSceneを検索する
		//return: 見つかればsubSceneのWeakポインタ, 見つからんかったらnull
		//引数1: SubScene Name
		inline WeakPointer<SubScene> BaseScene::FindSubScene(const sgstring & sceneName)
		{
			std::lock_guard<std::mutex> guard(m_mutex);

			for (auto& e : m_subScenes)
			{
				if (e.second->getSceneName() == sceneName)
					return e.second;
			}

			return WeakPointer<SubScene>::getNull();
		}
		//----------------------------------------------------------------------------------
		//[SubSceneBegin]
		//return: SubScene->Begin(非スレッドセーフ)
		inline auto BaseScene::SubSceneBegin()
		{
			return m_subScenes.begin();
		}
		//----------------------------------------------------------------------------------
		//[SubSceneEnd]
		//return: SubScene->End(非スレッドセーフ)
		inline auto BaseScene::SubSceneEnd()
		{
			return m_subScenes.end();
		}

		//----------------------------------------------------------------------------------
		//[OpenSubScene]
		//Subシーンを開く
		//return: 作成したサブシーンのシェアードポインタ(シーン外に持ち出し厳禁)
		//引数1: Subシーンへのバトンポインタ, nullptr代入可能
		//引数2: 作成されるSubシーンのIDが代入される, nullptr代入可能
		template<class SceneClass>
		inline SharedPointer<SubScene> BaseScene::OpenSubScene(SceneBaton* baton, uint * id)
		{
			static_assert(std::is_base_of<SubScene, SceneClass>, L"InstantiateNextScene template should be the base of SubScene class");

			std::lock_guard<std::mutex> guard(m_mutex);

			auto ptr = Pointer::MakeShared<SceneClass>(m_subIDCounter);
			m_subScenes.try_emplace(m_subIDCounter, ptr);

			if (id != nullptr) *id = m_subIDCounter++;

			ptr->StartScene(baton);
			
			return ptr;
		}
		//----------------------------------------------------------------------------------
		//[CloseSubScene]
		//Subシーンを閉じる
		//引数1: SubシーンのID
		inline void BaseScene::CloseSubScene(uint id)
		{
			std::lock_guard<std::mutex> guard(m_mutex);
			
			//シーンID
			auto find = m_subScenes.find(id);
			
			//削除して終了
			if (find != m_subScenes.end())
				m_subScenes.erase(id);
		}
		//----------------------------------------------------------------------------------
		//[CloseSubSceneAll]
		//Subシーンを全て閉じる
		inline void BaseScene::CloseSubSceneAll()
		{
			std::lock_guard<std::mutex> guard(m_mutex);
	
			m_subScenes.clear();
		}

		//----------------------------------------------------------------------------------
		//[UpdateSubScene]
		//SubSceneの並列更新を行う
		inline void BaseScene::UpdateSubScene()
		{
			for (auto& e : m_subScenes)
				e.second->Update();
		}
		//----------------------------------------------------------------------------------
		//[SyncSubScene]
		//SubSceneの同期更新を行う
		inline void BaseScene::SyncSubScene()
		{
			for (auto it = m_subScenes.begin(); it != m_subScenes.end();)
			{
				bool result = it->second->Sync();
				if (IS_FALSE(result))
				{
					it->second->EndScene();
					m_subScenes.erase(it);
				}
				else
					++it;
			}
		}
		//----------------------------------------------------------------------------------
		//[LateUpdateSubScene]
		//SubSceneの並列更新を行う
		inline void BaseScene::LateUpdateSubScene()
		{
			for (auto& e : m_subScenes)
				e.second->LateUpdate();
		}
		//----------------------------------------------------------------------------------
		//[FixedUpdateSubScene]
		//SubSceneのFixed並列更新を行う
		inline void BaseScene::FixedUpdateSubScene()
		{
			for (auto& e : m_subScenes)
				e.second->FixedUpdate();
		}
		//----------------------------------------------------------------------------------
		//[MakeDrawCommandSubScene]
		//SubSceneの並列描画更新を行う
		inline void BaseScene::MakeDrawCommandSubScene()
		{
			for (auto& e : m_subScenes)
				e.second->MakeDrawCommand();
		}
	}
}

#endif // !SGFRAMEWORK_HEADER_SCENE_HPP_
