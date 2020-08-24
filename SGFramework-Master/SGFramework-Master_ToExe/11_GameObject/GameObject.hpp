#ifndef SGFRAMEWORK_HEADER_GAME_OBJECT_HPP_
#define SGFRAMEWORK_HEADER_GAME_OBJECT_HPP_
#include "../05_Graphics/TransformUsing.hpp"
#include "../09_Scene/SceneManager.hpp"

//Framework namespace
namespace SGFramework
{
	class Application;

	//全ゲームオブジェクトの基底クラスとなるGameObject class
	//専用のコールバックシステムを持ち、EnableCallbacksと各関数のoverrideで実現できる
	class GameObject : public Object
	{
	private:
		class GameObjectCallback;
	protected:
		struct InstantiateRequiredInfo;
		struct InstantiateParentInfo;

	public:
		friend class Application;

		//Callbackを指定するフラグを定義したCallbackFunctions
		struct CallbackFunctions
		{
			friend class GameObject;

			enum Enum : uint
			{
				//Callback Awake Function Flag
				Awake = 0x00000001,
				//Callback OnEnable Function Flag
				OnEnable = 0x00000002,
				//Callback Start Function Flag
				Start = 0x00000004,
				//Callback Update Function Flag
				Update = 0x00000008,
				//Callback Sync Function Flag
				Sync = 0x00000010,
				//Callback LateUpdate Function Flag
				LateUpdate = 0x00000020,
				//Callback MakeDrawCommand Function Flag
				MakeDrawCommand = 0x00000040,
				//Callback FixedUpdate Function Flag
				FixedUpdate = 0x00000080,
				//Callback OnDisable Function Flag
				OnDisable = 0x00000100,
				//Callback OnDestroy Function Flag
				OnDestroy = 0x00000200,

				//Callback OnCollisionEnter Function Flag
				OnCollisionEnter = 0x00000400,
				//Callback OnCollisionStay Function Flag
				OnCollisionStay = 0x00000800,
				//Callback OnCollisionExit Function Flag
				OnCollisionExit = 0x00001000,
				//Callback OnTriggerEnter Function Flag
				OnTriggerEnter = 0x00002000,
				//Callback OnTriggerStay Function Flag
				OnTriggerStay = 0x00004000,
				//Callback OnTriggerExit Function Flag
				OnTriggerExit = 0x00008000,

				//Callback ChangeUpperHierarchy Function Flag
				ChangeUpperHierarchy = 0x00010000,

				//Callback Start & Update & Sync & Ondestroy Function Flag
				StandardCallbacks = Start | Update | Sync,
			};
		};

		//----------------------------------------------------------------------------------
		//[デストラクタ]
		inline ~GameObject()
		{
			if (IS_TRUE(cIsFindEnabled))
			{
				AtomicOperation::LockAtomic(m_staticLock);
				m_findObjects.erase(instanceID());
				AtomicOperation::UnlockAtomic(m_staticLock);
			}
		}
		

		//static functions
	

		//----------------------------------------------------------------------------------
		//[Instantiate]
		//GameObjectをインスタンス化する (値は全てvirtual InstantiateSetting Callbackで設定)
		//template: GameObject継承クラス
		//return: インスタンス化したGameObject
		//引数1: position, InstantiateSettingで設定されたものを上書きします
		//引数2: rotation, InstantiateSettingで設定されたものを上書きします
		//引数3: SharedPointer->isUsedMutexCounter, default = ture
		//引数4: SharedPointer->isAutoGabageCollection, default = ture
		//引数5: 各アプリケーションでカスタマイズできるuser flags data, default = 0
		template <class GameObjectDerived>
		static SharedPointer<GameObjectDerived> Instantiate(const Vector3& position,
			const Quaternion& rotation, uint userFlags = 0, bool isUsedMutexCounter = true, bool isAutoGabageCollection = true);
		//----------------------------------------------------------------------------------
		//[Instantiate]
		//GameObjectをインスタンス化する (値は全てvirtual InstantiateSetting Callbackで設定)
		//template: GameObject継承クラス
		//return: インスタンス化したGameObject
		//引数1: name, InstantiateSettingで設定されたものを上書きします
		//引数2: SharedPointer->isUsedMutexCounter, default = ture
		//引数3: SharedPointer->isAutoGabageCollection, default = ture
		//引数4: 各アプリケーションでカスタマイズできるuser flags data, default = 0
		template <class GameObjectDerived>
		static SharedPointer<GameObjectDerived> Instantiate(const sgstring& name, 
			uint userFlags = 0, bool isUsedMutexCounter = true, bool isAutoGabageCollection = true);
		//----------------------------------------------------------------------------------
		//[Instantiate]
		//GameObjectをインスタンス化する (値は全てvirtual InstantiateSetting Callbackで設定)
		//template: GameObject継承クラス
		//return: インスタンス化したGameObject
		//引数1: SharedPointer->isUsedMutexCounter, default = ture
		//引数2: SharedPointer->isAutoGabageCollection, default = ture
		//引数3: 各アプリケーションでカスタマイズできるuser flags data, default = 0
		template <class GameObjectDerived>
		static SharedPointer<GameObjectDerived> Instantiate(uint userFlags = 0, bool isUsedMutexCounter = true,
			bool isAutoGabageCollection = true);

		//----------------------------------------------------------------------------------
		//[Instantiate]
		//GameObjectをインスタンス化する (値は全てvirtual InstantiateSetting Callbackで設定)
		//template: GameObject継承クラス
		//return: インスタンス化したGameObject, parentが無効の場合null
		//引数1: 親Transform, InstantiateSettingで設定されたものを上書きします
		//引数2: position, rotation, scale -> true = world : false = local , InstantiateSettingで設定されたものを上書きします
		//引数3: position, InstantiateSettingで設定されたものを上書きします
		//引数4: rotation, InstantiateSettingで設定されたものを上書きします
		//引数5: 各アプリケーションでカスタマイズできるuser flags data, default = 0
		//引数6: SharedPointer->isUsedMutexCounter, default = ture
		//引数7: SharedPointer->isAutoGabageCollection, default = ture
		template <class GameObjectDerived>
		static SharedPointer<GameObjectDerived> Instantiate(SharedPointer<Transform> parent,
			bool isInstantiateInWorldSpace, const Vector3& position, const Quaternion& rotation, 
			uint userFlags = 0, bool isUsedMutexCounter = true, bool isAutoGabageCollection = true);
		//----------------------------------------------------------------------------------
		//[Instantiate]
		//GameObjectをインスタンス化する (値は全てvirtual InstantiateSetting Callbackで設定)
		//template: GameObject継承クラス
		//return: インスタンス化したGameObject, parentが無効の場合null
		//引数1: 親Transform, InstantiateSettingで設定されたものを上書きします
		//引数2: position, rotation, scale -> true = world : false = local , InstantiateSettingで設定されたものを上書きします
		//引数3: name, InstantiateSettingで設定されたものを上書きします
		//引数4: 各アプリケーションでカスタマイズできるuser flags data, default = 0
		//引数5: SharedPointer->isUsedMutexCounter, default = ture
		//引数6: SharedPointer->isAutoGabageCollection, default = ture
		template <class GameObjectDerived>
		static SharedPointer<GameObjectDerived> Instantiate(SharedPointer<Transform> parent,
			bool isInstantiateInWorldSpace, const sgstring& name, uint userFlags = 0,
			bool isUsedMutexCounter = true, bool isAutoGabageCollection = true);
		//----------------------------------------------------------------------------------
		//[Instantiate]
		//GameObjectをインスタンス化する (値は全てvirtual InstantiateSetting Callbackで設定)
		//template: GameObject継承クラス
		//return: インスタンス化したGameObject, parentが無効の場合null
		//引数1: 親Transform, InstantiateSettingで設定されたものを上書きします
		//引数2: position, rotation, scale -> true = world : false = local , InstantiateSettingで設定されたものを上書きします
		//引数3: 各アプリケーションでカスタマイズできるuser flags data, default = 0
		//引数4: SharedPointer->isUsedMutexCounter, default = ture
		//引数5: SharedPointer->isAutoGabageCollection, default = ture
		template <class GameObjectDerived>
		static SharedPointer<GameObjectDerived> Instantiate(SharedPointer<Transform> parent,
			bool isInstantiateInWorldSpace, uint userFlags = 0, bool isUsedMutexCounter = true, bool isAutoGabageCollection = true);

		//----------------------------------------------------------------------------------
		//[InstantiateManual]
		//GameObjectをインスタンス化する (値は全て手動設定)
		//template: GameObject継承クラス
		//return: インスタンス化したGameObject
		//引数1: position, default = zero
		//引数2: scale, default = one
		//引数3: rotation, default = identity
		//引数4: TimeLayer, default = root layer
		//引数5: name, default = "Game Object"
		//引数6: use function line if root transform, default = auto
		//引数7: isStatic, default = false
		//引数8: layer, default = Default Layer
		//引数9: tag, default = Default Tag
		//引数10: Find系関数用リストに登録する, パフォーマンスに大きく関係します, default = true
		//引数11: SharedPointer->isUsedMutexCounter, default = ture
		//引数12: SharedPointer->isAutoGabageCollection, default = ture
		template <class GameObjectDerived>
		static SharedPointer<GameObjectDerived> InstantiateManual(const Vector3& position = Const::Vector3::zero,
			const Vector3& scale = Const::Vector3::one, const Quaternion& rotation = Const::Quaternion::identity,
			WeakPointer<Time::TimeLayer> timeLayer = Time::rootTimeLayer(), 
			const sgstring& name = L"Game Object", int functionLineAtRoot = Transform::cAutoSelectFunctionLine,
			bool isStatic = false, Layer layer = Layer::DefaultLayer(), const Tag & tag = Tag::DefaultTag(),
			bool isFindEnabled = true, bool isUsedMutexCounter = true, bool isAutoGabageCollection = true);
		//----------------------------------------------------------------------------------
		//[InstantiateManual]
		//GameObjectをインスタンス化する (値は全て手動設定)
		//template: GameObject継承クラス
		//return: インスタンス化したGameObject, parentが無効の場合null
		//引数1: 親Transform
		//引数2: position, rotation, scale -> true = world : false = local 
		//引数3: position, default = zero
		//引数4: scale, default = one
		//引数5: rotation, default = identity
		//引数6: TimeLayer, default = root layer
		//引数7: name, default = "Game Object"
		//引数8: use function line if root transform, default = auto
		//引数9: isStatic, default = false
		//引数10: layer, default = Default Layer
		//引数11: tag, default = Default Tag
		//引数12: Find系関数用リストに登録する, パフォーマンスに大きく関係します, default = true
		//引数13: SharedPointer->isUsedMutexCounter, default = ture
		//引数14: SharedPointer->isAutoGabageCollection, default = ture
		template <class GameObjectDerived>
		static SharedPointer<GameObjectDerived> InstantiateManual(SharedPointer<Transform> parent,
			bool isInstantiateInWorldSpace, const Vector3& position = Const::Vector3::zero,
			const Vector3& scale = Const::Vector3::one, const Quaternion& rotation = Const::Quaternion::identity,
			WeakPointer<Time::TimeLayer> timeLayer = Time::rootTimeLayer(),
			const sgstring& name = L"Game Object", int functionLineAtRoot = Transform::cAutoSelectFunctionLine,
			bool isStatic = false, Layer layer = Layer::DefaultLayer(), const Tag & tag = Tag::DefaultTag(),
			bool isFindEnabled = true, bool isUsedMutexCounter = true, bool isAutoGabageCollection = true);



		template <class GameObjectDerived>
		static void DestroyObject(SharedPointer<GameObjectDerived> object)
		{
			AtomicOperation::LockAtomic(m_staticLock);
			Transform::LockGuardTransform lock(object->m_transform);
			object->m_transform->m_callbackEnabledFlags = 0;
			object->m_transform->m_callbackIndexs.clear();
			m_destroyObjects.emplace_back(std::move(object.DownCast<GameObject>()));
			AtomicOperation::UnlockAtomic(m_staticLock);
		}




		//----------------------------------------------------------------------------------
		//[FindWithName]
		//現在存在する全てのオブジェクトからnameをkeyに検索する
		//注意！ sub sceneに存在するオブジェクトもヒットします
		//return: Hitした最も古いオブジェクト
		inline static WeakPointer<GameObject> FindWithName(const sgstring& name);
		//----------------------------------------------------------------------------------
		//[FindWithTag]
		//現在存在する全てのオブジェクトからtagをkeyに検索する
		//注意！ sub sceneに存在するオブジェクトもヒットします
		//return: Hitした最も古いオブジェクト
		inline static WeakPointer<GameObject> FindWithTag(const Tag& tag);
		//----------------------------------------------------------------------------------
		//[FindWithLayer]
		//現在存在する全てのオブジェクトからlayerをkeyに検索する
		//注意！ sub sceneに存在するオブジェクトもヒットします
		//return: Hitした最も古いオブジェクト
		inline static WeakPointer<GameObject> FindWithLayer(const Layer& layer);

		//----------------------------------------------------------------------------------
		//[FindWithNameEx]
		//現在存在する全てのオブジェクトからnameをkeyに検索する, (All Search)
		//注意！ sub sceneに存在するオブジェクトもヒットします
		//return: Hitした全てのオブジェクト配列
		inline static std::vector<WeakPointer<GameObject>> FindWithNameEx(const sgstring& name);
		//----------------------------------------------------------------------------------
		//[FindWithNameEx]
		//現在存在する全てのオブジェクトからtagをkeyに検索する, (All Search)
		//注意！ sub sceneに存在するオブジェクトもヒットします
		//return: Hitした全てのオブジェクト配列
		inline static std::vector<WeakPointer<GameObject>> FindWithTagEx(const Tag& tag);
		//----------------------------------------------------------------------------------
		//[FindWithNameEx]
		//現在存在する全てのオブジェクトからlayerをkeyに検索する, (All Search)
		//注意！ sub sceneに存在するオブジェクトもヒットします
		//return: Hitした全てのオブジェクト配列
		inline static std::vector<WeakPointer<GameObject>> FindWithLayerEx(const Layer& layer);

		
		//transform functions


		//----------------------------------------------------------------------------------
		//[AddComponent]
		//Componentを追加する (ディープコピー)
		//引数1: component継承オブジェクト
		template <class ComponentClass>
		inline WeakPointer<ComponentClass> AddComponent(SharedPointer<ComponentClass> component)
		{
			return std::move(m_transform->AddComponent<ComponentClass>(component));
		}
		//----------------------------------------------------------------------------------
		//[AddComponent]
		//Componentを追加する (Instantiate)
		//return: 追加したComponent
		//引数1: Callback有効化フラグ, default = true
		template <class ComponentClass>
		inline WeakPointer<ComponentClass> AddComponent(bool isEnabled = true)
		{
			return std::move(m_transform->AddComponent<ComponentClass>(isEnabled));
		}

		//----------------------------------------------------------------------------------
		//[FindComponent]
		//return: ComponentClass型のコンポーネントが紐付けられている場合 : 最も古いコンポーネント
		//				紐付けられていない場合 : Null
		template <class ComponentClass>
		inline WeakPointer<ComponentClass> FindComponent()
		{
			return std::move(m_transform->FindComponent<ComponentClass>());
		}
		//----------------------------------------------------------------------------------
		//[FindComponentInParent]
		//ComponentClass型コンポーネントをthisからParentへと探査する
		//return: ComponentClass型のコンポーネントが紐付けられている場合 : 最初に見つかったコンポーネント
		//				紐付けられていない場合 : Null
		//引数1: 自身も含めるか, default = true
		template <class ComponentClass>
		inline WeakPointer<ComponentClass> FindComponentInParent(bool isIncludeYourself = true)
		{
			return std::move(m_transform->FindComponentInParent<ComponentClass>(isIncludeYourself));
		}
		//----------------------------------------------------------------------------------
		//[FindComponentInChildren]
		//ComponentClass型コンポーネントをChildrenから探査する
		//return: ComponentClass型のコンポーネントが紐付けられている場合 : 最初に見つかったコンポーネント
		//				紐付けられていない場合 : Null
		//引数1: 自身も含めるか, default = true
		template <class ComponentClass>
		inline WeakPointer<ComponentClass> FindComponentInChildren(bool isIncludeYourself = true)
		{
			return std::move(m_transform->FindComponentInChildren<ComponentClass>(isIncludeYourself));
		}
		//----------------------------------------------------------------------------------
		//[FindComponentEx]
		//return: ComponentClass型のコンポーネントが紐付けられている場合 : 全ての同型コンポーネント
		//				紐付けられていない場合 : Null
		template <class ComponentClass>
		inline std::vector<WeakPointer<ComponentClass>> FindComponentEx()
		{
			return std::move(m_transform->FindComponentEx<ComponentClass>());
		}
		//----------------------------------------------------------------------------------
		//[FindComponentInParentEx]
		//ComponentClass型コンポーネントをthisからParentへと探査する
		//return: ComponentClass型のコンポーネントが紐付けられている場合 : 全ての同型コンポーネント
		//				紐付けられていない場合 : Null
		//引数1: 自身も含めるか, default = true
		template <class ComponentClass>
		inline std::vector<WeakPointer<ComponentClass>> FindComponentInParentEx(bool isIncludeYourself = true)
		{
			return std::move(m_transform->FindComponentInParentEx<ComponentClass>(isIncludeYourself));
		}
		//----------------------------------------------------------------------------------
		//[FindComponentInChildrenEx]
		//ComponentClass型コンポーネントをChildrenから探査する
		//return: ComponentClass型のコンポーネントが紐付けられている場合 : 全ての同型コンポーネント
		//				紐付けられていない場合 : Null
		//引数1: 自身も含めるか, default = true
		template <class ComponentClass>
		inline std::vector<WeakPointer<ComponentClass>> FindComponentInChildrenEx(bool isIncludeYourself = true)
		{
			return std::move(m_transform->FindComponentInChildrenEx<ComponentClass>(isIncludeYourself));
		}

		//----------------------------------------------------------------------------------
		//[RemoveComponent]
		//Componentを削除する (template型に対応した最も古いコンポーネント)
		template <class ComponentClass>
		inline void RemoveComponent() { m_transform->RemoveComponent<ComponentClass>(); }
		//----------------------------------------------------------------------------------
		//[RemoveComponentEx]
		//Componentを削除する (template型に対応した全てのコンポーネント)
		template <class ComponentClass>
		inline void RemoveComponentEx() { m_transform->RemoveComponentEx<ComponentClass>(); }
		//----------------------------------------------------------------------------------
		//[RemoveComponentAll]
		//Componentを全て削除する
		inline void RemoveComponentAll() { m_transform->RemoveComponentAll(); }


		//virtual function


		//----------------------------------------------------------------------------------
		//[InstantiateSetting]
		//Instantiate時にCallbackされる関数, 引数に与えられた情報を関数内で設定する
		//引数3以降は既にInstantiateで設定しているので, 変更を加えても実際に挙動が変わるわけではない (not reference)
		//引数1: required settings values (必ず設定するべし！)
		//引数2: parent values
		//引数3: 各アプリケーションでカスタマイズできるuser flags data
		//引数3: SharedPointer->isUsedMutexCounter
		//引数4: SharedPointer->isAutoGabageCollection
		inline virtual void InstantiateSetting(InstantiateRequiredInfo& requiredInfo, InstantiateParentInfo& parentInfo,
			uint userFlags, bool isUsedMutexCounter, bool isAutoGabageCollection) {}


		//virtual component functions


		//----------------------------------------------------------------------------------
		//[EnabledCallbacks]
		//return: 自身がCallされるCallback Flags, GameObject::CallbackFunctions参照
		inline virtual uint EnableCallbacks() { return 0; }
		//----------------------------------------------------------------------------------
		//[Awake]
		//インスタンス化した瞬間に呼ばれるCallback関数
		inline virtual void Awake() {}
		//----------------------------------------------------------------------------------
		//[OnEnable]
		//インスタンス化後isEnabledがtrueになった瞬間に呼ばれるCallback関数
		inline virtual void OnEnable() {}
		//----------------------------------------------------------------------------------
		//[Start]
		//インスタンス化後同期フレームでCallbackされる関数
		inline virtual void Start() {}

		//----------------------------------------------------------------------------------
		//[Update]
		//毎フレームUpdateブロックでCallbackされる関数
		inline virtual void Update() {}
		//----------------------------------------------------------------------------------
		//[Sync]
		//毎フレームUpdate後の動機ブロック (Sync) でCallbackされる関数
		inline virtual void Sync() {}
		//----------------------------------------------------------------------------------
		//[LateUpdate]
		//Update, Sync後に呼ばれる更新ブロック (LateUpdate) でCallbackされる関数
		inline virtual void LateUpdate() {}
		//----------------------------------------------------------------------------------
		//[FixedUpdate]
		//定期的に呼ばれるFixedフレーム内でUpdateとしてCallbackされる関数
		inline virtual void FixedUpdate() {}
		//----------------------------------------------------------------------------------
		//[MakeDrawCommand]
		//Update, Sync, LateUpdate ブロック後にCallbackされる,
		//		DrawCommandを作成するための関数
		inline virtual void MakeDrawCommand() {}

		//----------------------------------------------------------------------------------
		//[OnDisable]
		//isEnabledがfalseになった瞬間にCallbackされる関数
		inline virtual void OnDisable() {}
		//----------------------------------------------------------------------------------
		//[OnDestroy]
		//インスタンスが削除される瞬間にCallbackされる関数
		inline virtual void OnDestroy() {}

		//----------------------------------------------------------------------------------
		//[OnCollisionEnter]
		//Colliderが当たった瞬間にCallbackされる関数
		//引数1: 接触オブジェクトの情報
		inline virtual void OnCollisionEnter(const ContactInfo& contactInfo) {}
		//----------------------------------------------------------------------------------
		//[OnCollisionStay]
		//Colliderが当たっている間Callbackされる関数
		//引数1: 接触オブジェクトの情報
		inline virtual void OnCollisionStay(const ContactInfo& contactInfo) {}
		//----------------------------------------------------------------------------------
		//[OnCollisionExit]
		//Colliderが離れた瞬間にCallbackされる関数
		//引数1: 接触オブジェクトの情報 (オブジェクトが削除されていた場合、ポインタはNull->isValid)
		inline virtual void OnCollisionExit(const ContactInfo& contactInfo) {}

		//----------------------------------------------------------------------------------
		//[OnTriggerEnter]
		//Triggerが当たった瞬間にCallbackされる関数
		//引数1: 接触オブジェクトの情報
		inline virtual void OnTriggerEnter(const ContactInfo& contactInfo) {}
		//----------------------------------------------------------------------------------
		//[OnTriggerStay]
		//Triggerが当たっている間Callbackされる関数
		//引数1: 接触オブジェクトの情報
		inline virtual void OnTriggerStay(const ContactInfo& contactInfo) {}
		//----------------------------------------------------------------------------------
		//[OnTriggerExit]
		//Triggerが離れた瞬間にCallbackされる関数
		//引数1: 接触オブジェクトの情報 (オブジェクトが削除されていた場合、ポインタはNull->isValid)
		inline virtual void OnTriggerExit(const ContactInfo& contactInfo) {}

		//----------------------------------------------------------------------------------
		//[ChangeUpperHierarchy]
		//parent以上の階層関係に変更が行われた(parentの変更など)瞬間にCallbackされる関数
		//仕様上UpdateFlameで呼び出される可能性があることに留意すること (parentの変更などを行ったスレッドで呼び出し)
		//引数1: new parent (nullの可能性あり, 直上の親ではない可能性があり)
		//引数2: old parent (nullの可能性あり, 直上の親ではない可能性があり)
		inline virtual void ChangeUpperHierarchy(WeakPointer<Transform> newParent, WeakPointer<Transform> oldParent) {}


		//propertys


		//layer (read function property)
		SGF_FUNCTION_PROPERTY Layer getLayer() { return m_transform->getLayer(); }

		//tag (read function property)
		SGF_FUNCTION_PROPERTY Tag getTag() { return m_transform->getTag(); }
		//tag (set function property)
		//argument 1: set value
		SGF_FUNCTION_PROPERTY Tag setTag(const Tag& set) { return m_transform->setTag(set); }

		//function line(get function property)
		SGF_FUNCTION_PROPERTY int getFunctionLine() { return m_transform->getFunctionLine(); }

		//this transform is active? (read function property)
		SGF_FUNCTION_PROPERTY bool getIsActiveSelf() { return m_transform->getIsActiveSelf(); }
		//set this transform is active (set function property)
		//argument 1: set value (update next frame)
		SGF_FUNCTION_PROPERTY bool setIsActive(bool active) { return m_transform->setIsActive(active); }

		//this transform is active? (read function property)
		SGF_FUNCTION_PROPERTY bool getIsStatic() { return m_transform->getIsStatic(); }
		//this transform is root? (read function property)
		SGF_FUNCTION_PROPERTY bool getIsRoot() { return m_transform->getIsRoot(); }
		//this transform is destroy? (read function property)
		SGF_FUNCTION_PROPERTY bool getIsDestroy() { return m_transform->getIsDestroy(); }

		//transform->isActiveSelf & game object callback isEnabled (read function property)
		SGF_FUNCTION_PROPERTY bool getIsActiveAndEnabledCallback() { return gameObjectCallbackComponent->getIsActiveAndEnabled(); }
		//game object callback is enabled? (read function property)
		SGF_FUNCTION_PROPERTY bool getIsEnabledCallback() { return gameObjectCallbackComponent->getIsEnabled(); }
		//game object callback is enabled? (set function property
		//argument 1: set value
		SGF_FUNCTION_PROPERTY bool setIsEnabledCallback(bool set) { return gameObjectCallbackComponent->setIsEnabled(set); }

		//this game object pointer (weak)
		WeakPointer<GameObject> gameObject;
		//this transform pointer (weak)
		WeakPointer<Transform> transform;
		//this time layer pointer (weak)
		WeakPointer <Time::TimeLayer> timeLayer;
		//callback component (weak)
		WeakPointer<GameObjectCallback> gameObjectCallbackComponent;
		//this scene pointer (const)
		const UniquePointer<Scene::BaseScene>& cThisScene;
		//this object find enabled?
		const bool cIsFindEnabled;
		//this object pointer->used mutex counter?
		const bool cIsUsingMutexPointer;
		//this object pointer->auto gabage collection?
		const bool cIsUsingAutoGabageCollection;
		

	protected:
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		inline GameObject()
			: Object(L""), cThisScene(Scene::SceneManager::getNowScene()),
			cIsFindEnabled(false), cIsUsingMutexPointer(false), cIsUsingAutoGabageCollection(false) {}


		//Instantiate実行時に使用するInstantiateRequiredInfo structure
		struct InstantiateRequiredInfo
		{
			//TimeLayer, default = root layer
			WeakPointer<Time::TimeLayer> timeLayer = Time::rootTimeLayer();
			//name, default = "Game Object"
			sgstring name = L"Game Object";
			//rotation(Instantiateの設定値で上書きされる可能性があります), default = identity
			Quaternion rotation = Const::Quaternion::identity;
			//position(Instantiateの設定値で上書きされる可能性があります), default = zero
			Vector3 position = Const::Vector3::zero;
			//scale, default = one
			Vector3 scale = Const::Vector3::one;
			//layer, default = Default Layer
			Layer layer = Layer::DefaultLayer();
			//tag, default = Default Tag
			Tag tag = Tag::DefaultTag();
			//use function line if root transform, default = auto
			int functionLineAtRoot = Transform::cAutoSelectFunctionLine;
			//Find系関数用リストに登録する, パフォーマンスに大きく関係します, default = true
			bool isFindEnabled = true;
			//isStatic, default = false
			bool isStatic = false;
		};

		//Instantiate実行時に使用するInstantiateParentInfo structure
		struct InstantiateParentInfo
		{
			//親Transform
			SharedPointer<Transform> parent = SharedPointer<Transform>::getNull();
			//position, rotation, scale -> true = world : false = local 
			bool isInstantiateInWorldSpace = true;
		};

	private:
		//callbackを実際に行うコンポーネントとなるGameObjectCallback class
		class GameObjectCallback : public Component
		{
		public:
			//----------------------------------------------------------------------------------
			//[コンストラクタ]
			inline GameObjectCallback(GameObject& gameObject) : gameObjectReference(gameObject) {}

			//GameObjectのものを呼び出す
			inline uint EnableCallbacks() override { return gameObjectReference.EnableCallbacks(); }
			inline void Awake() override { gameObjectReference.Awake(); }
			inline void OnEnable() override { gameObjectReference.OnEnable(); }
			inline void Start() override { gameObjectReference.Start(); }
			inline void Update() override { gameObjectReference.Update(); }
			inline void Sync() override { gameObjectReference.Sync(); }
			inline void LateUpdate() override { gameObjectReference.LateUpdate(); }
			inline void FixedUpdate() override { gameObjectReference.FixedUpdate(); }
			inline void MakeDrawCommand() override { gameObjectReference.MakeDrawCommand(); }
			inline void OnDisable() override { gameObjectReference.OnDisable(); }
			inline void OnDestroy() override { gameObjectReference.OnDestroy(); }
			inline void OnCollisionEnter(const ContactInfo& contactInfo) override { gameObjectReference.OnCollisionEnter(contactInfo); }
			inline void OnCollisionStay(const ContactInfo& contactInfo) override { gameObjectReference.OnCollisionStay(contactInfo); }
			inline void OnCollisionExit(const ContactInfo& contactInfo) override { gameObjectReference.OnCollisionExit(contactInfo); }
			inline void OnTriggerEnter(const ContactInfo& contactInfo) override { gameObjectReference.OnTriggerEnter(contactInfo); }
			inline void OnTriggerStay(const ContactInfo& contactInfo) override { gameObjectReference.OnTriggerStay(contactInfo); }
			inline void OnTriggerExit(const ContactInfo& contactInfo) override { gameObjectReference.OnTriggerExit(contactInfo); }
			inline void ChangeUpperHierarchy(WeakPointer<Transform> newParent, WeakPointer<Transform> oldParent) 
			{
				gameObjectReference.ChangeUpperHierarchy(newParent, oldParent);
			}

		private:
			//自身の参照
			GameObject& gameObjectReference;
		};

		//----------------------------------------------------------------------------------
		//[RegisterObjects]
		//オブジェクトリストへの登録
		inline void RegisterObjects()
		{
			//オブジェクトリストに登録
			AtomicOperation::LockAtomic(m_staticLock);
			m_findObjects.try_emplace(instanceID(), this);
			AtomicOperation::UnlockAtomic(m_staticLock);
		}

		//Find用オブジェクトリスト
		static std::unordered_map<uint, GameObject*> m_findObjects;
		//Destroy用オブジェクトリスト
		static std::vector<SharedPointer<GameObject>> m_destroyObjects;
		//リスト用Lock
		static std::atomic_bool m_staticLock;

		//transform (shared)
		SharedPointer<Transform> m_transform;
	};


	//----------------------------------------------------------------------------------
	//[Instantiate]
	//GameObjectをインスタンス化する (値は全てvirtual InstantiateSetting Callbackで設定)
	//template: GameObject継承クラス
	//return: インスタンス化したGameObject
	//引数1: position, InstantiateSettingで設定されたものを上書きします
	//引数2: rotation, InstantiateSettingで設定されたものを上書きします
	//引数3: 各アプリケーションでカスタマイズできるuser flags data, default = 0
	//引数4: SharedPointer->isUsedMutexCounter, default = ture
	//引数5: SharedPointer->isAutoGabageCollection, default = ture
	template<class GameObjectDerived>
	inline SharedPointer<GameObjectDerived> GameObject::Instantiate(const Vector3 & position, const Quaternion & rotation,
		uint userFlags, bool isUsedMutexCounter, bool isAutoGabageCollection)
	{
		static_assert(std::is_base_of_v<GameObject, GameObjectDerived>,
			"GameObject::Instantiate templatre -> GameObject derived class only!");

		//ポインタ作成, friendクラスを触るのでGameObjectにダウンキャストしたものも用意
		SharedPointer<GameObjectDerived> result(new GameObjectDerived(), isUsedMutexCounter, isAutoGabageCollection);
		SharedPointer<GameObject> downCast = result.DownCast<GameObject>();
		//InstantiateInfo
		InstantiateRequiredInfo info0;
		InstantiateParentInfo info1;

		//値を設定
		downCast->InstantiateSetting(info0, info1, userFlags, isUsedMutexCounter, isAutoGabageCollection);

		//各変数の初期化
		*const_cast<bool*>(&downCast->cIsUsingMutexPointer) = isUsedMutexCounter;
		*const_cast<bool*>(&downCast->cIsUsingAutoGabageCollection) = isAutoGabageCollection;
		*const_cast<bool*>(&downCast->cIsFindEnabled) = info0.isFindEnabled;

		//parentが存在しない->instantiate root transform
		if (IS_FALSE(info1.parent.getIsValid()))
		{
			downCast->m_transform = Transform::Instantiate(position, info0.scale, rotation, info0.name,
				info0.functionLineAtRoot, info0.isStatic, info0.layer, info0.tag, isUsedMutexCounter, isAutoGabageCollection);
		}
		//parentが存在する->instantiate children transform
		else
		{
			downCast->m_transform = Transform::Instantiate(info1.parent, info1.isInstantiateInWorldSpace,
				position, info0.scale, rotation, info0.name, info0.functionLineAtRoot,
				info0.isStatic, info0.layer, info0.tag, isUsedMutexCounter, isAutoGabageCollection);
		}
		downCast->setName(downCast->m_transform->name());

		//ポインタの登録
		downCast->gameObject = downCast;
		downCast->timeLayer = info0.timeLayer;
		//transform->ポインタの登録
		downCast->transform = downCast->m_transform;
		downCast->m_transform->timeLayer = info0.timeLayer;
		downCast->m_transform->gameObject = downCast;

		//find enabled -> 登録
		if (IS_TRUE(downCast->cIsFindEnabled))
			downCast->RegisterObjects();

		//Add Component (AddComponentから呼び出すと参照設定ができない)
		{
			//callback instantiate
			SharedPointer<GameObjectCallback> component(new GameObjectCallback(*downCast),
				isUsedMutexCounter, isAutoGabageCollection);
			//各変数を初期化
			downCast->gameObjectCallbackComponent = component;
			component->transform = downCast->m_transform;
			component->setName(downCast->m_transform->name());
			component->timeLayer = info0.timeLayer;
			component->gameObject = downCast;
			component->m_isEnabled = component->m_isOldEnabled = true;
			component->m_isOldActive = downCast->m_transform->m_isActive;
			component->m_thisPointer = component.DownCast<Component>();

			//Callback flags取得
			uint flags = component->EnableCallbacks();
			//private sync設定
			if (component->m_callbackEnabledFlags & Component::CallbackFunctions::m_cSyncIncludeFlags)
				flags |= Component::CallbackFunctions::m_cSync;
			//初期化
			component->m_callbackEnabledFlags = component->m_saveCallbackEnabledFlags = flags;

			//componentリストへ登録
			downCast->m_transform->m_components.emplace_back(component.DownCast<Component>());

			//再Allocate
			downCast->m_transform->AllocateCallback();

			//invoke callback?
			if (IS_TRUE(component->m_isEnabled))
			{
				if (component->m_thisPointer->m_callbackEnabledFlags & CallbackFunctions::Awake)
				{
					component->m_thisPointer->Awake();
					component->m_thisPointer->m_callbackEnabledFlags ^= CallbackFunctions::Awake;
				}
				if (component->m_thisPointer->m_callbackEnabledFlags & CallbackFunctions::OnEnable)
					component->m_thisPointer->OnEnable();
			}
		}
		return result;
	}
	//----------------------------------------------------------------------------------
	//[Instantiate]
	//GameObjectをインスタンス化する (値は全てvirtual InstantiateSetting Callbackで設定)
	//template: GameObject継承クラス
	//return: インスタンス化したGameObject
	//引数1: name, InstantiateSettingで設定されたものを上書きします
	//引数2: 各アプリケーションでカスタマイズできるuser flags data, default = 0
	//引数3: SharedPointer->isUsedMutexCounter, default = ture
	//引数4: SharedPointer->isAutoGabageCollection, default = ture
	template<class GameObjectDerived>
	inline SharedPointer<GameObjectDerived> GameObject::Instantiate(const sgstring & name, 
		uint userFlags, bool isUsedMutexCounter, bool isAutoGabageCollection)
	{
		static_assert(std::is_base_of_v<GameObject, GameObjectDerived>,
			"GameObject::Instantiate templatre -> GameObject derived class only!");

		//ポインタ作成, friendクラスを触るのでGameObjectにダウンキャストしたものも用意
		SharedPointer<GameObjectDerived> result(new GameObjectDerived(), isUsedMutexCounter, isAutoGabageCollection);
		SharedPointer<GameObject> downCast = result.DownCast<GameObject>();
		//InstantiateInfo
		InstantiateRequiredInfo info0;
		InstantiateParentInfo info1;

		//値を設定
		downCast->InstantiateSetting(info0, info1, userFlags, isUsedMutexCounter, isAutoGabageCollection);

		//各変数の初期化
		*const_cast<bool*>(&downCast->cIsUsingMutexPointer) = isUsedMutexCounter;
		*const_cast<bool*>(&downCast->cIsUsingAutoGabageCollection) = isAutoGabageCollection;
		*const_cast<bool*>(&downCast->cIsFindEnabled) = info0.isFindEnabled;

		//parentが存在しない->instantiate root transform
		if (IS_FALSE(info1.parent.getIsValid()))
		{
			downCast->m_transform = Transform::Instantiate(info0.position, info0.scale, info0.rotation, name,
				info0.functionLineAtRoot, info0.isStatic, info0.layer, info0.tag, isUsedMutexCounter, isAutoGabageCollection);
		}
		//parentが存在する->instantiate children transform
		else
		{
			downCast->m_transform = Transform::Instantiate(info1.parent, info1.isInstantiateInWorldSpace,
				info0.position, info0.scale, info0.rotation, name, info0.functionLineAtRoot,
				info0.isStatic, info0.layer, info0.tag, isUsedMutexCounter, isAutoGabageCollection);
		}
		downCast->setName(downCast->m_transform->name());

		//ポインタの登録
		downCast->gameObject = downCast;
		downCast->timeLayer = info0.timeLayer;
		//transform->ポインタの登録
		downCast->transform = downCast->m_transform;
		downCast->m_transform->timeLayer = info0.timeLayer;
		downCast->m_transform->gameObject = downCast;

		//find enabled -> 登録
		if (IS_TRUE(downCast->cIsFindEnabled))
			downCast->RegisterObjects();

		//Add Component (AddComponentから呼び出すと参照設定ができない)
		{
			//callback instantiate
			SharedPointer<GameObjectCallback> component(new GameObjectCallback(*downCast),
				isUsedMutexCounter, isAutoGabageCollection);
			//各変数を初期化
			downCast->gameObjectCallbackComponent = component;
			component->transform = downCast->m_transform;
			component->setName(downCast->m_transform->name());
			component->timeLayer = info0.timeLayer;
			component->gameObject = downCast;
			component->m_isEnabled = component->m_isOldEnabled = true;
			component->m_isOldActive = downCast->m_transform->m_isActive;
			component->m_thisPointer = component.DownCast<Component>();

			//Callback flags取得
			uint flags = component->EnableCallbacks();
			//private sync設定
			if (component->m_callbackEnabledFlags & Component::CallbackFunctions::m_cSyncIncludeFlags)
				flags |= Component::CallbackFunctions::m_cSync;
			//初期化
			component->m_callbackEnabledFlags = component->m_saveCallbackEnabledFlags = flags;

			//componentリストへ登録
			downCast->m_transform->m_components.emplace_back(component.DownCast<Component>());

			//再Allocate
			downCast->m_transform->AllocateCallback();

			//invoke callback?
			if (IS_TRUE(component->m_isEnabled))
			{
				if (component->m_thisPointer->m_callbackEnabledFlags & CallbackFunctions::Awake)
				{
					component->m_thisPointer->Awake();
					component->m_thisPointer->m_callbackEnabledFlags ^= CallbackFunctions::Awake;
				}
				if (component->m_thisPointer->m_callbackEnabledFlags & CallbackFunctions::OnEnable)
					component->m_thisPointer->OnEnable();
			}
		}
		return result;
	}
	//----------------------------------------------------------------------------------
	//[Instantiate]
	//GameObjectをインスタンス化する (値は全てvirtual InstantiateSetting Callbackで設定)
	//template: GameObject継承クラス
	//return: インスタンス化したGameObject
	//引数1: 各アプリケーションでカスタマイズできるuser flags data, default = 0
	//引数2: SharedPointer->isUsedMutexCounter, default = ture
	//引数3: SharedPointer->isAutoGabageCollection, default = ture
	template <class GameObjectDerived>
	inline SharedPointer<GameObjectDerived> GameObject::Instantiate(uint userFlags,
		bool isUsedMutexCounter, bool isAutoGabageCollection)
	{
		static_assert(std::is_base_of_v<GameObject, GameObjectDerived>,
			"GameObject::Instantiate templatre -> GameObject derived class only!");

		//ポインタ作成, friendクラスを触るのでGameObjectにダウンキャストしたものも用意
		SharedPointer<GameObjectDerived> result(new GameObjectDerived(), isUsedMutexCounter, isAutoGabageCollection);
		SharedPointer<GameObject> downCast = result.DownCast<GameObject>();
		//InstantiateInfo
		InstantiateRequiredInfo info0;
		InstantiateParentInfo info1;

		//値を設定
		downCast->InstantiateSetting(info0, info1, userFlags, isUsedMutexCounter, isAutoGabageCollection);

		//各変数の初期化
		*const_cast<bool*>(&downCast->cIsUsingMutexPointer) = isUsedMutexCounter;
		*const_cast<bool*>(&downCast->cIsUsingAutoGabageCollection) = isAutoGabageCollection;
		*const_cast<bool*>(&downCast->cIsFindEnabled) = info0.isFindEnabled;
		
		//parentが存在しない->instantiate root transform
		if (IS_FALSE(info1.parent.getIsValid()))
		{
			downCast->m_transform = Transform::Instantiate(info0.position, info0.scale, info0.rotation, info0.name,
				info0.functionLineAtRoot, info0.isStatic, info0.layer, info0.tag, isUsedMutexCounter, isAutoGabageCollection);
		}
		//parentが存在する->instantiate children transform
		else
		{
			downCast->m_transform = Transform::Instantiate(info1.parent, info1.isInstantiateInWorldSpace,
				info0.position, info0.scale, info0.rotation, info0.name, info0.functionLineAtRoot, 
				info0.isStatic, info0.layer, info0.tag, isUsedMutexCounter, isAutoGabageCollection);
		}
		downCast->setName(downCast->m_transform->name());

		//ポインタの登録
		downCast->gameObject = downCast;
		downCast->timeLayer = info0.timeLayer;
		//transform->ポインタの登録
		downCast->transform = downCast->m_transform;
		downCast->m_transform->timeLayer = info0.timeLayer;
		downCast->m_transform->gameObject = downCast;

		//find enabled -> 登録
		if (IS_TRUE(downCast->cIsFindEnabled))
			downCast->RegisterObjects();

		//Add Component (AddComponentから呼び出すと参照設定ができない)
		{
			//callback instantiate
			SharedPointer<GameObjectCallback> component(new GameObjectCallback(*downCast),
				isUsedMutexCounter, isAutoGabageCollection);
			//各変数を初期化
			downCast->gameObjectCallbackComponent = component;
			component->transform = downCast->m_transform;
			component->setName(downCast->m_transform->name());
			component->timeLayer = info0.timeLayer;
			component->gameObject = downCast;
			component->m_isEnabled = component->m_isOldEnabled = true;
			component->m_isOldActive = downCast->m_transform->m_isActive;
			component->m_thisPointer = component.DownCast<Component>();

			//Callback flags取得
			uint flags = component->EnableCallbacks();
			//private sync設定
			if (component->m_callbackEnabledFlags & Component::CallbackFunctions::m_cSyncIncludeFlags)
				flags |= Component::CallbackFunctions::m_cSync;
			//初期化
			component->m_callbackEnabledFlags = component->m_saveCallbackEnabledFlags = flags;

			//componentリストへ登録
			downCast->m_transform->m_components.emplace_back(component.DownCast<Component>());

			//再Allocate
			downCast->m_transform->AllocateCallback();

			//invoke callback?
			if (IS_TRUE(component->m_isEnabled))
			{
				if (component->m_thisPointer->m_callbackEnabledFlags & CallbackFunctions::Awake)
				{
					component->m_thisPointer->Awake();
					component->m_thisPointer->m_callbackEnabledFlags ^= CallbackFunctions::Awake;
				}
				if (component->m_thisPointer->m_callbackEnabledFlags & CallbackFunctions::OnEnable)
					component->m_thisPointer->OnEnable();
			}
		}
		return result;
	}

	//----------------------------------------------------------------------------------
	//[Instantiate]
	//GameObjectをインスタンス化する (値は全てvirtual InstantiateSetting Callbackで設定)
	//template: GameObject継承クラス
	//return: インスタンス化したGameObject, parentが無効の場合null
	//引数1: 親Transform, InstantiateSettingで設定されたものを上書きします
	//引数2: position, rotation, scale -> true = world : false = local , InstantiateSettingで設定されたものを上書きします
	//引数3: position, InstantiateSettingで設定されたものを上書きします
	//引数4: rotation, InstantiateSettingで設定されたものを上書きします
	//引数5: 各アプリケーションでカスタマイズできるuser flags data, default = 0
	//引数6: SharedPointer->isUsedMutexCounter, default = ture
	//引数7: SharedPointer->isAutoGabageCollection, default = ture
	template<class GameObjectDerived>
	inline SharedPointer<GameObjectDerived> GameObject::Instantiate(SharedPointer<Transform> parent,
		bool isInstantiateInWorldSpace, const Vector3 & position, const Quaternion & rotation, 
		uint userFlags, bool isUsedMutexCounter, bool isAutoGabageCollection)
	{
		static_assert(std::is_base_of_v<GameObject, GameObjectDerived>,
			"GameObject::Instantiate templatre -> GameObject derived class only!");

		//parentが存在しない->return
		if (IS_FALSE(parent.getIsValid()))
			return SharedPointer<GameObjectDerived>::getNull();

		//ポインタ作成, friendクラスを触るのでGameObjectにダウンキャストしたものも用意
		SharedPointer<GameObjectDerived> result(new GameObjectDerived(), isUsedMutexCounter, isAutoGabageCollection);
		SharedPointer<GameObject> downCast = result.DownCast<GameObject>();
		//InstantiateInfo
		InstantiateRequiredInfo info0;
		InstantiateParentInfo info1;

		//値を設定
		downCast->InstantiateSetting(info0, info1, userFlags, isUsedMutexCounter, isAutoGabageCollection);

		//各変数の初期化
		*const_cast<bool*>(&downCast->cIsUsingMutexPointer) = isUsedMutexCounter;
		*const_cast<bool*>(&downCast->cIsUsingAutoGabageCollection) = isAutoGabageCollection;
		*const_cast<bool*>(&downCast->cIsFindEnabled) = info0.isFindEnabled;

		downCast->m_transform = Transform::Instantiate(parent, isInstantiateInWorldSpace,
			position, info0.scale, rotation, info0.name, info0.functionLineAtRoot,
			info0.isStatic, info0.layer, info0.tag, isUsedMutexCounter, isAutoGabageCollection);
		downCast->setName(downCast->m_transform->name());

		//ポインタの登録
		downCast->gameObject = downCast;
		downCast->timeLayer = info0.timeLayer;
		//transform->ポインタの登録
		downCast->transform = downCast->m_transform;
		downCast->m_transform->timeLayer = info0.timeLayer;
		downCast->m_transform->gameObject = downCast;

		//find enabled -> 登録
		if (IS_TRUE(downCast->cIsFindEnabled))
			downCast->RegisterObjects();

		//Add Component (AddComponentから呼び出すと参照設定ができない)
		{
			//callback instantiate
			SharedPointer<GameObjectCallback> component(new GameObjectCallback(*downCast),
				isUsedMutexCounter, isAutoGabageCollection);
			//各変数を初期化
			downCast->gameObjectCallbackComponent = component;
			component->transform = downCast->m_transform;
			component->setName(downCast->m_transform->name());
			component->timeLayer = info0.timeLayer;
			component->gameObject = downCast;
			component->m_isEnabled = component->m_isOldEnabled = true;
			component->m_isOldActive = downCast->m_transform->m_isActive;
			component->m_thisPointer = component.DownCast<Component>();

			//Callback flags取得
			uint flags = component->EnableCallbacks();
			//private sync設定
			if (component->m_callbackEnabledFlags & Component::CallbackFunctions::m_cSyncIncludeFlags)
				flags |= Component::CallbackFunctions::m_cSync;
			//初期化
			component->m_callbackEnabledFlags = component->m_saveCallbackEnabledFlags = flags;

			//componentリストへ登録
			downCast->m_transform->m_components.emplace_back(component.DownCast<Component>());

			//再Allocate
			downCast->m_transform->AllocateCallback();

			//invoke callback?
			if (IS_TRUE(component->m_isEnabled))
			{
				if (component->m_thisPointer->m_callbackEnabledFlags & CallbackFunctions::Awake)
				{
					component->m_thisPointer->Awake();
					component->m_thisPointer->m_callbackEnabledFlags ^= CallbackFunctions::Awake;
				}
				if (component->m_thisPointer->m_callbackEnabledFlags & CallbackFunctions::OnEnable)
					component->m_thisPointer->OnEnable();
			}
		}
		return result;
	}

	//----------------------------------------------------------------------------------
	//[Instantiate]
	//GameObjectをインスタンス化する (値は全てvirtual InstantiateSetting Callbackで設定)
	//template: GameObject継承クラス
	//return: インスタンス化したGameObject, parentが無効の場合null
	//引数1: 親Transform, InstantiateSettingで設定されたものを上書きします
	//引数2: position, rotation, scale -> true = world : false = local , InstantiateSettingで設定されたものを上書きします
	//引数3: name, InstantiateSettingで設定されたものを上書きします
	//引数4: 各アプリケーションでカスタマイズできるuser flags data, default = 0
	//引数5: SharedPointer->isUsedMutexCounter, default = ture
	//引数6: SharedPointer->isAutoGabageCollection, default = ture
	template<class GameObjectDerived>
	inline SharedPointer<GameObjectDerived> GameObject::Instantiate(SharedPointer<Transform> parent,
		bool isInstantiateInWorldSpace, const sgstring & name, uint userFlags, bool isUsedMutexCounter, bool isAutoGabageCollection)
	{
		static_assert(std::is_base_of_v<GameObject, GameObjectDerived>,
			"GameObject::Instantiate templatre -> GameObject derived class only!");

		//parentが存在しない->return
		if (IS_FALSE(parent.getIsValid()))
			return SharedPointer<GameObjectDerived>::getNull();

		//ポインタ作成, friendクラスを触るのでGameObjectにダウンキャストしたものも用意
		SharedPointer<GameObjectDerived> result(new GameObjectDerived(), isUsedMutexCounter, isAutoGabageCollection);
		SharedPointer<GameObject> downCast = result.DownCast<GameObject>();
		//InstantiateInfo
		InstantiateRequiredInfo info0;
		InstantiateParentInfo info1;

		//値を設定
		downCast->InstantiateSetting(info0, info1, userFlags, isUsedMutexCounter, isAutoGabageCollection);

		//各変数の初期化
		*const_cast<bool*>(&downCast->cIsUsingMutexPointer) = isUsedMutexCounter;
		*const_cast<bool*>(&downCast->cIsUsingAutoGabageCollection) = isAutoGabageCollection;
		*const_cast<bool*>(&downCast->cIsFindEnabled) = info0.isFindEnabled;

		downCast->m_transform = Transform::Instantiate(parent, isInstantiateInWorldSpace,
			info0.position, info0.scale, info0.rotation, name, info0.functionLineAtRoot,
			info0.isStatic, info0.layer, info0.tag, isUsedMutexCounter, isAutoGabageCollection);
		downCast->setName(downCast->m_transform->name());

		//ポインタの登録
		downCast->gameObject = downCast;
		downCast->timeLayer = info0.timeLayer;
		//transform->ポインタの登録
		downCast->transform = downCast->m_transform;
		downCast->m_transform->timeLayer = info0.timeLayer;
		downCast->m_transform->gameObject = downCast;

		//find enabled -> 登録
		if (IS_TRUE(downCast->cIsFindEnabled))
			downCast->RegisterObjects();

		//Add Component (AddComponentから呼び出すと参照設定ができない)
		{
			//callback instantiate
			SharedPointer<GameObjectCallback> component(new GameObjectCallback(*downCast),
				isUsedMutexCounter, isAutoGabageCollection);
			//各変数を初期化
			downCast->gameObjectCallbackComponent = component;
			component->transform = downCast->m_transform;
			component->setName(downCast->m_transform->name());
			component->timeLayer = info0.timeLayer;
			component->gameObject = downCast;
			component->m_isEnabled = component->m_isOldEnabled = true;
			component->m_isOldActive = downCast->m_transform->m_isActive;
			component->m_thisPointer = component.DownCast<Component>();

			//Callback flags取得
			uint flags = component->EnableCallbacks();
			//private sync設定
			if (component->m_callbackEnabledFlags & Component::CallbackFunctions::m_cSyncIncludeFlags)
				flags |= Component::CallbackFunctions::m_cSync;
			//初期化
			component->m_callbackEnabledFlags = component->m_saveCallbackEnabledFlags = flags;

			//componentリストへ登録
			downCast->m_transform->m_components.emplace_back(component.DownCast<Component>());

			//再Allocate
			downCast->m_transform->AllocateCallback();

			//invoke callback?
			if (IS_TRUE(component->m_isEnabled))
			{
				if (component->m_thisPointer->m_callbackEnabledFlags & CallbackFunctions::Awake)
				{
					component->m_thisPointer->Awake();
					component->m_thisPointer->m_callbackEnabledFlags ^= CallbackFunctions::Awake;
				}
				if (component->m_thisPointer->m_callbackEnabledFlags & CallbackFunctions::OnEnable)
					component->m_thisPointer->OnEnable();
			}
		}
		return result;
	}
	//----------------------------------------------------------------------------------
	//[Instantiate]
	//GameObjectをインスタンス化する (値は全てvirtual InstantiateSetting Callbackで設定)
	//template: GameObject継承クラス
	//return: インスタンス化したGameObject, parentが無効の場合null
	//引数1: 親Transform, InstantiateSettingで設定されたものを上書きします
	//引数2: position, rotation, scale -> true = world : false = local , InstantiateSettingで設定されたものを上書きします
	//引数3: 各アプリケーションでカスタマイズできるuser flags data, default = 0
	//引数4: SharedPointer->isUsedMutexCounter, default = ture
	//引数5: SharedPointer->isAutoGabageCollection, default = ture
	template <class GameObjectDerived>
	inline SharedPointer<GameObjectDerived> GameObject::Instantiate(SharedPointer<Transform> parent,
		bool isInstantiateInWorldSpace, uint userFlags, bool isUsedMutexCounter, bool isAutoGabageCollection)
	{
		static_assert(std::is_base_of_v<GameObject, GameObjectDerived>,
			"GameObject::Instantiate templatre -> GameObject derived class only!");

		//parentが存在しない->return
		if (IS_FALSE(parent.getIsValid()))
			return SharedPointer<GameObjectDerived>::getNull();

		//ポインタ作成, friendクラスを触るのでGameObjectにダウンキャストしたものも用意
		SharedPointer<GameObjectDerived> result(new GameObjectDerived(), isUsedMutexCounter, isAutoGabageCollection);
		SharedPointer<GameObject> downCast = result.DownCast<GameObject>();
		//InstantiateInfo
		InstantiateRequiredInfo info0;
		InstantiateParentInfo info1;

		//値を設定
		downCast->InstantiateSetting(info0, info1, userFlags, isUsedMutexCounter, isAutoGabageCollection);

		//各変数の初期化
		*const_cast<bool*>(&downCast->cIsUsingMutexPointer) = isUsedMutexCounter;
		*const_cast<bool*>(&downCast->cIsUsingAutoGabageCollection) = isAutoGabageCollection;
		*const_cast<bool*>(&downCast->cIsFindEnabled) = info0.isFindEnabled;

		//parentが存在しない->instantiate root transform
		if (IS_FALSE(info1.parent.getIsValid()))
		{
			downCast->m_transform = Transform::Instantiate(info0.position, info0.scale, info0.rotation, info0.name,
				info0.functionLineAtRoot, info0.isStatic, info0.layer, info0.tag, isUsedMutexCounter, isAutoGabageCollection);
		}
		//parentが存在する->instantiate children transform
		else
		{
			downCast->m_transform = Transform::Instantiate(info1.parent, info1.isInstantiateInWorldSpace,
				info0.position, info0.scale, info0.rotation, info0.name, info0.functionLineAtRoot,
				info0.isStatic, info0.layer, info0.tag, isUsedMutexCounter, isAutoGabageCollection);
		}
		downCast->setName(downCast->m_transform->name());

		//ポインタの登録
		downCast->gameObject = downCast;
		downCast->timeLayer = info0.timeLayer;
		//transform->ポインタの登録
		downCast->transform = downCast->m_transform;
		downCast->m_transform->timeLayer = info0.timeLayer;
		downCast->m_transform->gameObject = downCast;

		//find enabled -> 登録
		if (IS_TRUE(downCast->cIsFindEnabled))
			downCast->RegisterObjects();

		//Add Component (AddComponentから呼び出すと参照設定ができない)
		{
			//callback instantiate
			SharedPointer<GameObjectCallback> component(new GameObjectCallback(*downCast),
				isUsedMutexCounter, isAutoGabageCollection);
			//各変数を初期化
			downCast->gameObjectCallbackComponent = component;
			component->transform = downCast->m_transform;
			component->setName(downCast->m_transform->name());
			component->timeLayer = info0.timeLayer;
			component->gameObject = downCast;
			component->m_isEnabled = component->m_isOldEnabled = true;
			component->m_isOldActive = downCast->m_transform->m_isActive;
			component->m_thisPointer = component.DownCast<Component>();

			//Callback flags取得
			uint flags = component->EnableCallbacks();
			//private sync設定
			if (component->m_callbackEnabledFlags & Component::CallbackFunctions::m_cSyncIncludeFlags)
				flags |= Component::CallbackFunctions::m_cSync;
			//初期化
			component->m_callbackEnabledFlags = component->m_saveCallbackEnabledFlags = flags;

			//componentリストへ登録
			downCast->m_transform->m_components.emplace_back(component.DownCast<Component>());

			//再Allocate
			downCast->m_transform->AllocateCallback();

			//invoke callback?
			if (IS_TRUE(component->m_isEnabled))
			{
				if (component->m_thisPointer->m_callbackEnabledFlags & CallbackFunctions::Awake)
				{
					component->m_thisPointer->Awake();
					component->m_thisPointer->m_callbackEnabledFlags ^= CallbackFunctions::Awake;
				}
				if (component->m_thisPointer->m_callbackEnabledFlags & CallbackFunctions::OnEnable)
					component->m_thisPointer->OnEnable();
			}
		}
		return result;
	}

	//----------------------------------------------------------------------------------
	//[InstantiateManual]
	//GameObjectをインスタンス化する (値は全て手動設定)
	//template: GameObject継承クラス
	//return: インスタンス化したGameObject
	//引数1: position, default = zero
	//引数2: scale, default = one
	//引数3: rotation, default = identity
	//引数4: TimeLayer, default = root layer
	//引数5: name, default = "Game Object"
	//引数6: use function line if root transform, default = auto
	//引数7: isStatic, default = false
	//引数8: layer, default = Default Layer
	//引数9: tag, default = Default Tag
	//引数10: Find系関数用リストに登録する, パフォーマンスに大きく関係します, default = true
	//引数11: SharedPointer->isUsedMutexCounter, default = ture
	//引数12: SharedPointer->isAutoGabageCollection, default = ture
	template <class GameObjectDerived>
	inline SharedPointer<GameObjectDerived> GameObject::InstantiateManual(const Vector3 & position,
		const Vector3 & scale, const Quaternion & rotation, WeakPointer<Time::TimeLayer> timeLayer, 
		const sgstring & name, int functionLineAtRoot, bool isStatic, 
		Layer layer, const Tag & tag, bool isFindEnabled, bool isUsedMutexCounter, bool isAutoGabageCollection)
	{
		static_assert(std::is_base_of_v<GameObject, GameObjectDerived>,
			"GameObject::Instantiate templatre -> GameObject derived class only!");

		//ポインタ作成, friendクラスを触るのでGameObjectにダウンキャストしたものも用意
		SharedPointer<GameObjectDerived> result(new GameObjectDerived(), isUsedMutexCounter, isAutoGabageCollection);
		SharedPointer<GameObject> downCast = result.DownCast<GameObject>();

		//各変数の初期化
		*const_cast<bool*>(&downCast->cIsUsingMutexPointer) = isUsedMutexCounter;
		*const_cast<bool*>(&downCast->cIsUsingAutoGabageCollection) = isAutoGabageCollection;
		*const_cast<bool*>(&downCast->cIsFindEnabled) = isFindEnabled;
	
		//instantiate root transform
		downCast->m_transform = Transform::Instantiate(position, scale, rotation, name,
			functionLineAtRoot, isStatic, layer, tag, isUsedMutexCounter, isAutoGabageCollection);

		downCast->setName(downCast->m_transform->name());
		//ポインタの登録
		downCast->gameObject = downCast;
		downCast->timeLayer = timeLayer;
		//transform->ポインタの登録
		downCast->transform = downCast->m_transform;
		downCast->m_transform->timeLayer = timeLayer;
		downCast->m_transform->gameObject = downCast;

		//find enabled -> 登録
		if (IS_TRUE(downCast->cIsFindEnabled))
			downCast->RegisterObjects();

		//Add Component (AddComponentから呼び出すと参照設定ができない)
		{
			//callback instantiate
			SharedPointer<GameObjectCallback> component(new GameObjectCallback(*downCast),
				isUsedMutexCounter, isAutoGabageCollection);
			//各変数を初期化
			downCast->gameObjectCallbackComponent = component;
			component->transform = downCast->m_transform;
			component->setName(downCast->m_transform->name());
			component->timeLayer = timeLayer;
			component->gameObject = downCast;
			component->m_isEnabled = component->m_isOldEnabled = true;
			component->m_isOldActive = downCast->m_transform->m_isActive;
			component->m_thisPointer = component.DownCast<Component>();

			//Callback flags取得
			uint flags = component->EnableCallbacks();
			//private sync設定
			if (component->m_callbackEnabledFlags & Component::CallbackFunctions::m_cSyncIncludeFlags)
				flags |= Component::CallbackFunctions::m_cSync;
			//初期化
			component->m_callbackEnabledFlags = component->m_saveCallbackEnabledFlags = flags;

			//componentリストへ登録
			downCast->m_transform->m_components.emplace_back(component.DownCast<Component>());

			//再Allocate
			downCast->m_transform->AllocateCallback();
		
			//invoke callback?
			if (IS_TRUE(component->m_isEnabled))
			{
				if (component->m_thisPointer->m_callbackEnabledFlags & CallbackFunctions::Awake)
				{
					component->m_thisPointer->Awake();
					component->m_thisPointer->m_callbackEnabledFlags ^= CallbackFunctions::Awake;
				}
				if (component->m_thisPointer->m_callbackEnabledFlags & CallbackFunctions::OnEnable)
					component->m_thisPointer->OnEnable();
			}
		}
		return result;
	}
	//----------------------------------------------------------------------------------
	//[InstantiateManual]
	//GameObjectをインスタンス化する (値は全て手動設定)
	//template: GameObject継承クラス
	//return: インスタンス化したGameObject, parentが無効の場合null
	//引数1: 親Transform
	//引数2: position, rotation, scale -> true = world : false = local 
	//引数3: position, default = zero
	//引数4: scale, default = one
	//引数5: rotation, default = identity
	//引数6: TimeLayer, default = root layer
	//引数7: name, default = "Game Object"
	//引数8: use function line if root transform, default = auto
	//引数9: isStatic, default = false
	//引数10: layer, default = Default Layer
	//引数11: tag, default = Default Tag
	//引数12: Find系関数用リストに登録する, パフォーマンスに大きく関係します, default = true
	//引数13: SharedPointer->isUsedMutexCounter, default = ture
	//引数14: SharedPointer->isAutoGabageCollection, default = ture
	template <class GameObjectDerived>
	inline SharedPointer<GameObjectDerived> GameObject::InstantiateManual(SharedPointer<Transform> parent,
		bool isInstantiateInWorldSpace, const Vector3 & position, const Vector3 & scale, const Quaternion & rotation,
		WeakPointer<Time::TimeLayer> timeLayer, const sgstring & name, int functionLineAtRoot,
		bool isStatic, Layer layer, const Tag & tag, bool isFindEnabled, bool isUsedMutexCounter, bool isAutoGabageCollection)
	{
		static_assert(std::is_base_of_v<GameObject, GameObjectDerived>,
			"GameObject::Instantiate templatre -> GameObject derived class only!");

		if (IS_FALSE(parent.getIsValid())) return SharedPointer<GameObjectDerived>::getNull();

		//ポインタ作成, friendクラスを触るのでGameObjectにダウンキャストしたものも用意
		SharedPointer<GameObjectDerived> result(new GameObjectDerived(), isUsedMutexCounter, isAutoGabageCollection);
		SharedPointer<GameObject> downCast = result.DownCast<GameObject>();

		//各変数の初期化
		*const_cast<bool*>(&downCast->cIsUsingMutexPointer) = isUsedMutexCounter;
		*const_cast<bool*>(&downCast->cIsUsingAutoGabageCollection) = isAutoGabageCollection;
		*const_cast<bool*>(&downCast->cIsFindEnabled) = isFindEnabled;
	
		//instantiate children transform
		downCast->m_transform = Transform::Instantiate(parent, isInstantiateInWorldSpace, position, scale, rotation, name,
			functionLineAtRoot, isStatic, layer, tag, isUsedMutexCounter, isAutoGabageCollection);
		downCast->setName(downCast->m_transform->name());

		//ポインタの登録
		downCast->gameObject = downCast;
		downCast->timeLayer = timeLayer;
		//transform->ポインタの登録
		downCast->transform = downCast->m_transform;
		downCast->m_transform->timeLayer = timeLayer;
		downCast->m_transform->gameObject = downCast;
		
		//find enabled -> 登録
		if (IS_TRUE(downCast->cIsFindEnabled))
			downCast->RegisterObjects();

		//Add Component (AddComponentから呼び出すと参照設定ができない)
		{
			//callback instantiate
			SharedPointer<GameObjectCallback> component(new GameObjectCallback(*downCast),
				isUsedMutexCounter, isAutoGabageCollection);
			//各変数を初期化
			downCast->gameObjectCallbackComponent = component;
			component->transform = downCast->m_transform;
			component->setName(downCast->m_transform->name());
			component->timeLayer = timeLayer;
			component->gameObject = downCast;
			component->m_isEnabled = component->m_isOldEnabled = true;
			component->m_isOldActive = downCast->m_transform->m_isActive;
			component->m_thisPointer = component.DownCast<Component>();

			//Callback flags取得
			uint flags = component->EnableCallbacks();
			//private sync設定
			if (component->m_callbackEnabledFlags & Component::CallbackFunctions::m_cSyncIncludeFlags)
				flags |= Component::CallbackFunctions::m_cSync;
			//初期化
			component->m_callbackEnabledFlags = component->m_saveCallbackEnabledFlags = flags;

			//componentリストへ登録
			downCast->m_transform->m_components.emplace_back(component.DownCast<Component>());

			//再Allocate
			downCast->m_transform->AllocateCallback();

			//invoke callback?
			if (IS_TRUE(component->m_isEnabled))
			{
				if (component->m_thisPointer->m_callbackEnabledFlags & CallbackFunctions::Awake)
				{
					component->m_thisPointer->Awake();
					component->m_thisPointer->m_callbackEnabledFlags ^= CallbackFunctions::Awake;
				}
				if (component->m_thisPointer->m_callbackEnabledFlags & CallbackFunctions::OnEnable)
					component->m_thisPointer->OnEnable();
			}
		}
		return result;
	}

	//----------------------------------------------------------------------------------
	//[FindWithName]
	//現在存在する全てのオブジェクトからnameをkeyに検索する
	//注意！ sub sceneに存在するオブジェクトもヒットします
	//return: Hitした最も古いオブジェクト
	inline WeakPointer<GameObject> GameObject::FindWithName(const sgstring & name)
	{	
		AtomicOperation::LockGuard guard(m_staticLock);

		//find->return
		for (auto& e : m_findObjects)
			if (e.second->name() == name) return e.second->gameObject;

		return WeakPointer<GameObject>::getNull();
	}		
	//----------------------------------------------------------------------------------
	//[FindWithTag]
	//現在存在する全てのオブジェクトからtagをkeyに検索する
	//注意！ sub sceneに存在するオブジェクトもヒットします
	//return: Hitした最も古いオブジェクト
	inline WeakPointer<GameObject> GameObject::FindWithTag(const Tag & tag)
	{
		AtomicOperation::LockGuard guard(m_staticLock);

		//find->return
		for (auto& e : m_findObjects)
			if (e.second->getTag() == tag) return e.second->gameObject;
		
		return WeakPointer<GameObject>::getNull();
	}
	//----------------------------------------------------------------------------------
	//[FindWithLayer]
	//現在存在する全てのオブジェクトからlayerをkeyに検索する
	//注意！ sub sceneに存在するオブジェクトもヒットします
	//return: Hitした最も古いオブジェクト
	inline WeakPointer<GameObject> GameObject::FindWithLayer(const Layer & layer)
	{
		AtomicOperation::LockGuard guard(m_staticLock);

		//find->return
		for (auto& e : m_findObjects)
			if (e.second->getLayer() == layer) return e.second->gameObject;
		
		return WeakPointer<GameObject>::getNull();
	}

	//----------------------------------------------------------------------------------
	//[FindWithNameEx]
	//現在存在する全てのオブジェクトからnameをkeyに検索する, (All Search)
	//注意！ sub sceneに存在するオブジェクトもヒットします
	//return: Hitした全てのオブジェクト配列
	inline std::vector<WeakPointer<GameObject>> GameObject::FindWithNameEx(const sgstring & name)
	{
		std::vector<WeakPointer<GameObject>> result;
		AtomicOperation::LockGuard guard(m_staticLock);

		//find->emplace_back
		for (auto& e : m_findObjects)
			if (e.second->name() == name) result.emplace_back(e.second->gameObject);

		return std::move(result);
	}
	//----------------------------------------------------------------------------------
	//[FindWithNameEx]
	//現在存在する全てのオブジェクトからtagをkeyに検索する, (All Search)
	//注意！ sub sceneに存在するオブジェクトもヒットします
	//return: Hitした全てのオブジェクト配列
	inline std::vector<WeakPointer<GameObject>> GameObject::FindWithTagEx(const Tag & tag)
	{
		std::vector<WeakPointer<GameObject>> result;
		AtomicOperation::LockGuard guard(m_staticLock);

		//find->emplace_back
		for (auto& e : m_findObjects)
			if (e.second->getTag() == tag) result.emplace_back(e.second->gameObject);

		return std::move(result);
	}
	//----------------------------------------------------------------------------------
	//[FindWithNameEx]
	//現在存在する全てのオブジェクトからlayerをkeyに検索する, (All Search)
	//注意！ sub sceneに存在するオブジェクトもヒットします
	//return: Hitした全てのオブジェクト配列
	inline std::vector<WeakPointer<GameObject>> GameObject::FindWithLayerEx(const Layer & layer)
	{
		std::vector<WeakPointer<GameObject>> result;
		AtomicOperation::LockGuard guard(m_staticLock);

		//find->emplace_back
		for (auto& e : m_findObjects)
			if (e.second->getLayer() == layer) result.emplace_back(e.second->gameObject);

		return std::move(result);
	}
}
#endif // !SGFRAMEWORK_HEADER_GAME_OBJECT_HPP_