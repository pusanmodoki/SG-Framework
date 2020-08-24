#ifndef SGFRAMEWORK_HEADER_GAME_OBJECT_HPP_
#define SGFRAMEWORK_HEADER_GAME_OBJECT_HPP_
#include "../05_Graphics/TransformUsing.hpp"
#include "../09_Scene/SceneManager.hpp"

//Framework namespace
namespace SGFramework
{
	class Application;

	//�S�Q�[���I�u�W�F�N�g�̊��N���X�ƂȂ�GameObject class
	//��p�̃R�[���o�b�N�V�X�e���������AEnableCallbacks�Ɗe�֐���override�Ŏ����ł���
	class GameObject : public Object
	{
	private:
		class GameObjectCallback;
	protected:
		struct InstantiateRequiredInfo;
		struct InstantiateParentInfo;

	public:
		friend class Application;

		//Callback���w�肷��t���O���`����CallbackFunctions
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
		//[�f�X�g���N�^]
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
		//GameObject���C���X�^���X������ (�l�͑S��virtual InstantiateSetting Callback�Őݒ�)
		//template: GameObject�p���N���X
		//return: �C���X�^���X������GameObject
		//����1: position, InstantiateSetting�Őݒ肳�ꂽ���̂��㏑�����܂�
		//����2: rotation, InstantiateSetting�Őݒ肳�ꂽ���̂��㏑�����܂�
		//����3: SharedPointer->isUsedMutexCounter, default = ture
		//����4: SharedPointer->isAutoGabageCollection, default = ture
		//����5: �e�A�v���P�[�V�����ŃJ�X�^�}�C�Y�ł���user flags data, default = 0
		template <class GameObjectDerived>
		static SharedPointer<GameObjectDerived> Instantiate(const Vector3& position,
			const Quaternion& rotation, uint userFlags = 0, bool isUsedMutexCounter = true, bool isAutoGabageCollection = true);
		//----------------------------------------------------------------------------------
		//[Instantiate]
		//GameObject���C���X�^���X������ (�l�͑S��virtual InstantiateSetting Callback�Őݒ�)
		//template: GameObject�p���N���X
		//return: �C���X�^���X������GameObject
		//����1: name, InstantiateSetting�Őݒ肳�ꂽ���̂��㏑�����܂�
		//����2: SharedPointer->isUsedMutexCounter, default = ture
		//����3: SharedPointer->isAutoGabageCollection, default = ture
		//����4: �e�A�v���P�[�V�����ŃJ�X�^�}�C�Y�ł���user flags data, default = 0
		template <class GameObjectDerived>
		static SharedPointer<GameObjectDerived> Instantiate(const sgstring& name, 
			uint userFlags = 0, bool isUsedMutexCounter = true, bool isAutoGabageCollection = true);
		//----------------------------------------------------------------------------------
		//[Instantiate]
		//GameObject���C���X�^���X������ (�l�͑S��virtual InstantiateSetting Callback�Őݒ�)
		//template: GameObject�p���N���X
		//return: �C���X�^���X������GameObject
		//����1: SharedPointer->isUsedMutexCounter, default = ture
		//����2: SharedPointer->isAutoGabageCollection, default = ture
		//����3: �e�A�v���P�[�V�����ŃJ�X�^�}�C�Y�ł���user flags data, default = 0
		template <class GameObjectDerived>
		static SharedPointer<GameObjectDerived> Instantiate(uint userFlags = 0, bool isUsedMutexCounter = true,
			bool isAutoGabageCollection = true);

		//----------------------------------------------------------------------------------
		//[Instantiate]
		//GameObject���C���X�^���X������ (�l�͑S��virtual InstantiateSetting Callback�Őݒ�)
		//template: GameObject�p���N���X
		//return: �C���X�^���X������GameObject, parent�������̏ꍇnull
		//����1: �eTransform, InstantiateSetting�Őݒ肳�ꂽ���̂��㏑�����܂�
		//����2: position, rotation, scale -> true = world : false = local , InstantiateSetting�Őݒ肳�ꂽ���̂��㏑�����܂�
		//����3: position, InstantiateSetting�Őݒ肳�ꂽ���̂��㏑�����܂�
		//����4: rotation, InstantiateSetting�Őݒ肳�ꂽ���̂��㏑�����܂�
		//����5: �e�A�v���P�[�V�����ŃJ�X�^�}�C�Y�ł���user flags data, default = 0
		//����6: SharedPointer->isUsedMutexCounter, default = ture
		//����7: SharedPointer->isAutoGabageCollection, default = ture
		template <class GameObjectDerived>
		static SharedPointer<GameObjectDerived> Instantiate(SharedPointer<Transform> parent,
			bool isInstantiateInWorldSpace, const Vector3& position, const Quaternion& rotation, 
			uint userFlags = 0, bool isUsedMutexCounter = true, bool isAutoGabageCollection = true);
		//----------------------------------------------------------------------------------
		//[Instantiate]
		//GameObject���C���X�^���X������ (�l�͑S��virtual InstantiateSetting Callback�Őݒ�)
		//template: GameObject�p���N���X
		//return: �C���X�^���X������GameObject, parent�������̏ꍇnull
		//����1: �eTransform, InstantiateSetting�Őݒ肳�ꂽ���̂��㏑�����܂�
		//����2: position, rotation, scale -> true = world : false = local , InstantiateSetting�Őݒ肳�ꂽ���̂��㏑�����܂�
		//����3: name, InstantiateSetting�Őݒ肳�ꂽ���̂��㏑�����܂�
		//����4: �e�A�v���P�[�V�����ŃJ�X�^�}�C�Y�ł���user flags data, default = 0
		//����5: SharedPointer->isUsedMutexCounter, default = ture
		//����6: SharedPointer->isAutoGabageCollection, default = ture
		template <class GameObjectDerived>
		static SharedPointer<GameObjectDerived> Instantiate(SharedPointer<Transform> parent,
			bool isInstantiateInWorldSpace, const sgstring& name, uint userFlags = 0,
			bool isUsedMutexCounter = true, bool isAutoGabageCollection = true);
		//----------------------------------------------------------------------------------
		//[Instantiate]
		//GameObject���C���X�^���X������ (�l�͑S��virtual InstantiateSetting Callback�Őݒ�)
		//template: GameObject�p���N���X
		//return: �C���X�^���X������GameObject, parent�������̏ꍇnull
		//����1: �eTransform, InstantiateSetting�Őݒ肳�ꂽ���̂��㏑�����܂�
		//����2: position, rotation, scale -> true = world : false = local , InstantiateSetting�Őݒ肳�ꂽ���̂��㏑�����܂�
		//����3: �e�A�v���P�[�V�����ŃJ�X�^�}�C�Y�ł���user flags data, default = 0
		//����4: SharedPointer->isUsedMutexCounter, default = ture
		//����5: SharedPointer->isAutoGabageCollection, default = ture
		template <class GameObjectDerived>
		static SharedPointer<GameObjectDerived> Instantiate(SharedPointer<Transform> parent,
			bool isInstantiateInWorldSpace, uint userFlags = 0, bool isUsedMutexCounter = true, bool isAutoGabageCollection = true);

		//----------------------------------------------------------------------------------
		//[InstantiateManual]
		//GameObject���C���X�^���X������ (�l�͑S�Ď蓮�ݒ�)
		//template: GameObject�p���N���X
		//return: �C���X�^���X������GameObject
		//����1: position, default = zero
		//����2: scale, default = one
		//����3: rotation, default = identity
		//����4: TimeLayer, default = root layer
		//����5: name, default = "Game Object"
		//����6: use function line if root transform, default = auto
		//����7: isStatic, default = false
		//����8: layer, default = Default Layer
		//����9: tag, default = Default Tag
		//����10: Find�n�֐��p���X�g�ɓo�^����, �p�t�H�[�}���X�ɑ傫���֌W���܂�, default = true
		//����11: SharedPointer->isUsedMutexCounter, default = ture
		//����12: SharedPointer->isAutoGabageCollection, default = ture
		template <class GameObjectDerived>
		static SharedPointer<GameObjectDerived> InstantiateManual(const Vector3& position = Const::Vector3::zero,
			const Vector3& scale = Const::Vector3::one, const Quaternion& rotation = Const::Quaternion::identity,
			WeakPointer<Time::TimeLayer> timeLayer = Time::rootTimeLayer(), 
			const sgstring& name = L"Game Object", int functionLineAtRoot = Transform::cAutoSelectFunctionLine,
			bool isStatic = false, Layer layer = Layer::DefaultLayer(), const Tag & tag = Tag::DefaultTag(),
			bool isFindEnabled = true, bool isUsedMutexCounter = true, bool isAutoGabageCollection = true);
		//----------------------------------------------------------------------------------
		//[InstantiateManual]
		//GameObject���C���X�^���X������ (�l�͑S�Ď蓮�ݒ�)
		//template: GameObject�p���N���X
		//return: �C���X�^���X������GameObject, parent�������̏ꍇnull
		//����1: �eTransform
		//����2: position, rotation, scale -> true = world : false = local 
		//����3: position, default = zero
		//����4: scale, default = one
		//����5: rotation, default = identity
		//����6: TimeLayer, default = root layer
		//����7: name, default = "Game Object"
		//����8: use function line if root transform, default = auto
		//����9: isStatic, default = false
		//����10: layer, default = Default Layer
		//����11: tag, default = Default Tag
		//����12: Find�n�֐��p���X�g�ɓo�^����, �p�t�H�[�}���X�ɑ傫���֌W���܂�, default = true
		//����13: SharedPointer->isUsedMutexCounter, default = ture
		//����14: SharedPointer->isAutoGabageCollection, default = ture
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
		//���ݑ��݂���S�ẴI�u�W�F�N�g����name��key�Ɍ�������
		//���ӁI sub scene�ɑ��݂���I�u�W�F�N�g���q�b�g���܂�
		//return: Hit�����ł��Â��I�u�W�F�N�g
		inline static WeakPointer<GameObject> FindWithName(const sgstring& name);
		//----------------------------------------------------------------------------------
		//[FindWithTag]
		//���ݑ��݂���S�ẴI�u�W�F�N�g����tag��key�Ɍ�������
		//���ӁI sub scene�ɑ��݂���I�u�W�F�N�g���q�b�g���܂�
		//return: Hit�����ł��Â��I�u�W�F�N�g
		inline static WeakPointer<GameObject> FindWithTag(const Tag& tag);
		//----------------------------------------------------------------------------------
		//[FindWithLayer]
		//���ݑ��݂���S�ẴI�u�W�F�N�g����layer��key�Ɍ�������
		//���ӁI sub scene�ɑ��݂���I�u�W�F�N�g���q�b�g���܂�
		//return: Hit�����ł��Â��I�u�W�F�N�g
		inline static WeakPointer<GameObject> FindWithLayer(const Layer& layer);

		//----------------------------------------------------------------------------------
		//[FindWithNameEx]
		//���ݑ��݂���S�ẴI�u�W�F�N�g����name��key�Ɍ�������, (All Search)
		//���ӁI sub scene�ɑ��݂���I�u�W�F�N�g���q�b�g���܂�
		//return: Hit�����S�ẴI�u�W�F�N�g�z��
		inline static std::vector<WeakPointer<GameObject>> FindWithNameEx(const sgstring& name);
		//----------------------------------------------------------------------------------
		//[FindWithNameEx]
		//���ݑ��݂���S�ẴI�u�W�F�N�g����tag��key�Ɍ�������, (All Search)
		//���ӁI sub scene�ɑ��݂���I�u�W�F�N�g���q�b�g���܂�
		//return: Hit�����S�ẴI�u�W�F�N�g�z��
		inline static std::vector<WeakPointer<GameObject>> FindWithTagEx(const Tag& tag);
		//----------------------------------------------------------------------------------
		//[FindWithNameEx]
		//���ݑ��݂���S�ẴI�u�W�F�N�g����layer��key�Ɍ�������, (All Search)
		//���ӁI sub scene�ɑ��݂���I�u�W�F�N�g���q�b�g���܂�
		//return: Hit�����S�ẴI�u�W�F�N�g�z��
		inline static std::vector<WeakPointer<GameObject>> FindWithLayerEx(const Layer& layer);

		
		//transform functions


		//----------------------------------------------------------------------------------
		//[AddComponent]
		//Component��ǉ����� (�f�B�[�v�R�s�[)
		//����1: component�p���I�u�W�F�N�g
		template <class ComponentClass>
		inline WeakPointer<ComponentClass> AddComponent(SharedPointer<ComponentClass> component)
		{
			return std::move(m_transform->AddComponent<ComponentClass>(component));
		}
		//----------------------------------------------------------------------------------
		//[AddComponent]
		//Component��ǉ����� (Instantiate)
		//return: �ǉ�����Component
		//����1: Callback�L�����t���O, default = true
		template <class ComponentClass>
		inline WeakPointer<ComponentClass> AddComponent(bool isEnabled = true)
		{
			return std::move(m_transform->AddComponent<ComponentClass>(isEnabled));
		}

		//----------------------------------------------------------------------------------
		//[FindComponent]
		//return: ComponentClass�^�̃R���|�[�l���g���R�t�����Ă���ꍇ : �ł��Â��R���|�[�l���g
		//				�R�t�����Ă��Ȃ��ꍇ : Null
		template <class ComponentClass>
		inline WeakPointer<ComponentClass> FindComponent()
		{
			return std::move(m_transform->FindComponent<ComponentClass>());
		}
		//----------------------------------------------------------------------------------
		//[FindComponentInParent]
		//ComponentClass�^�R���|�[�l���g��this����Parent�ւƒT������
		//return: ComponentClass�^�̃R���|�[�l���g���R�t�����Ă���ꍇ : �ŏ��Ɍ��������R���|�[�l���g
		//				�R�t�����Ă��Ȃ��ꍇ : Null
		//����1: ���g���܂߂邩, default = true
		template <class ComponentClass>
		inline WeakPointer<ComponentClass> FindComponentInParent(bool isIncludeYourself = true)
		{
			return std::move(m_transform->FindComponentInParent<ComponentClass>(isIncludeYourself));
		}
		//----------------------------------------------------------------------------------
		//[FindComponentInChildren]
		//ComponentClass�^�R���|�[�l���g��Children����T������
		//return: ComponentClass�^�̃R���|�[�l���g���R�t�����Ă���ꍇ : �ŏ��Ɍ��������R���|�[�l���g
		//				�R�t�����Ă��Ȃ��ꍇ : Null
		//����1: ���g���܂߂邩, default = true
		template <class ComponentClass>
		inline WeakPointer<ComponentClass> FindComponentInChildren(bool isIncludeYourself = true)
		{
			return std::move(m_transform->FindComponentInChildren<ComponentClass>(isIncludeYourself));
		}
		//----------------------------------------------------------------------------------
		//[FindComponentEx]
		//return: ComponentClass�^�̃R���|�[�l���g���R�t�����Ă���ꍇ : �S�Ă̓��^�R���|�[�l���g
		//				�R�t�����Ă��Ȃ��ꍇ : Null
		template <class ComponentClass>
		inline std::vector<WeakPointer<ComponentClass>> FindComponentEx()
		{
			return std::move(m_transform->FindComponentEx<ComponentClass>());
		}
		//----------------------------------------------------------------------------------
		//[FindComponentInParentEx]
		//ComponentClass�^�R���|�[�l���g��this����Parent�ւƒT������
		//return: ComponentClass�^�̃R���|�[�l���g���R�t�����Ă���ꍇ : �S�Ă̓��^�R���|�[�l���g
		//				�R�t�����Ă��Ȃ��ꍇ : Null
		//����1: ���g���܂߂邩, default = true
		template <class ComponentClass>
		inline std::vector<WeakPointer<ComponentClass>> FindComponentInParentEx(bool isIncludeYourself = true)
		{
			return std::move(m_transform->FindComponentInParentEx<ComponentClass>(isIncludeYourself));
		}
		//----------------------------------------------------------------------------------
		//[FindComponentInChildrenEx]
		//ComponentClass�^�R���|�[�l���g��Children����T������
		//return: ComponentClass�^�̃R���|�[�l���g���R�t�����Ă���ꍇ : �S�Ă̓��^�R���|�[�l���g
		//				�R�t�����Ă��Ȃ��ꍇ : Null
		//����1: ���g���܂߂邩, default = true
		template <class ComponentClass>
		inline std::vector<WeakPointer<ComponentClass>> FindComponentInChildrenEx(bool isIncludeYourself = true)
		{
			return std::move(m_transform->FindComponentInChildrenEx<ComponentClass>(isIncludeYourself));
		}

		//----------------------------------------------------------------------------------
		//[RemoveComponent]
		//Component���폜���� (template�^�ɑΉ������ł��Â��R���|�[�l���g)
		template <class ComponentClass>
		inline void RemoveComponent() { m_transform->RemoveComponent<ComponentClass>(); }
		//----------------------------------------------------------------------------------
		//[RemoveComponentEx]
		//Component���폜���� (template�^�ɑΉ������S�ẴR���|�[�l���g)
		template <class ComponentClass>
		inline void RemoveComponentEx() { m_transform->RemoveComponentEx<ComponentClass>(); }
		//----------------------------------------------------------------------------------
		//[RemoveComponentAll]
		//Component��S�č폜����
		inline void RemoveComponentAll() { m_transform->RemoveComponentAll(); }


		//virtual function


		//----------------------------------------------------------------------------------
		//[InstantiateSetting]
		//Instantiate����Callback�����֐�, �����ɗ^����ꂽ�����֐����Őݒ肷��
		//����3�ȍ~�͊���Instantiate�Őݒ肵�Ă���̂�, �ύX�������Ă����ۂɋ������ς��킯�ł͂Ȃ� (not reference)
		//����1: required settings values (�K���ݒ肷��ׂ��I)
		//����2: parent values
		//����3: �e�A�v���P�[�V�����ŃJ�X�^�}�C�Y�ł���user flags data
		//����3: SharedPointer->isUsedMutexCounter
		//����4: SharedPointer->isAutoGabageCollection
		inline virtual void InstantiateSetting(InstantiateRequiredInfo& requiredInfo, InstantiateParentInfo& parentInfo,
			uint userFlags, bool isUsedMutexCounter, bool isAutoGabageCollection) {}


		//virtual component functions


		//----------------------------------------------------------------------------------
		//[EnabledCallbacks]
		//return: ���g��Call�����Callback Flags, GameObject::CallbackFunctions�Q��
		inline virtual uint EnableCallbacks() { return 0; }
		//----------------------------------------------------------------------------------
		//[Awake]
		//�C���X�^���X�������u�ԂɌĂ΂��Callback�֐�
		inline virtual void Awake() {}
		//----------------------------------------------------------------------------------
		//[OnEnable]
		//�C���X�^���X����isEnabled��true�ɂȂ����u�ԂɌĂ΂��Callback�֐�
		inline virtual void OnEnable() {}
		//----------------------------------------------------------------------------------
		//[Start]
		//�C���X�^���X���㓯���t���[����Callback�����֐�
		inline virtual void Start() {}

		//----------------------------------------------------------------------------------
		//[Update]
		//���t���[��Update�u���b�N��Callback�����֐�
		inline virtual void Update() {}
		//----------------------------------------------------------------------------------
		//[Sync]
		//���t���[��Update��̓��@�u���b�N (Sync) ��Callback�����֐�
		inline virtual void Sync() {}
		//----------------------------------------------------------------------------------
		//[LateUpdate]
		//Update, Sync��ɌĂ΂��X�V�u���b�N (LateUpdate) ��Callback�����֐�
		inline virtual void LateUpdate() {}
		//----------------------------------------------------------------------------------
		//[FixedUpdate]
		//����I�ɌĂ΂��Fixed�t���[������Update�Ƃ���Callback�����֐�
		inline virtual void FixedUpdate() {}
		//----------------------------------------------------------------------------------
		//[MakeDrawCommand]
		//Update, Sync, LateUpdate �u���b�N���Callback�����,
		//		DrawCommand���쐬���邽�߂̊֐�
		inline virtual void MakeDrawCommand() {}

		//----------------------------------------------------------------------------------
		//[OnDisable]
		//isEnabled��false�ɂȂ����u�Ԃ�Callback�����֐�
		inline virtual void OnDisable() {}
		//----------------------------------------------------------------------------------
		//[OnDestroy]
		//�C���X�^���X���폜�����u�Ԃ�Callback�����֐�
		inline virtual void OnDestroy() {}

		//----------------------------------------------------------------------------------
		//[OnCollisionEnter]
		//Collider�����������u�Ԃ�Callback�����֐�
		//����1: �ڐG�I�u�W�F�N�g�̏��
		inline virtual void OnCollisionEnter(const ContactInfo& contactInfo) {}
		//----------------------------------------------------------------------------------
		//[OnCollisionStay]
		//Collider���������Ă����Callback�����֐�
		//����1: �ڐG�I�u�W�F�N�g�̏��
		inline virtual void OnCollisionStay(const ContactInfo& contactInfo) {}
		//----------------------------------------------------------------------------------
		//[OnCollisionExit]
		//Collider�����ꂽ�u�Ԃ�Callback�����֐�
		//����1: �ڐG�I�u�W�F�N�g�̏�� (�I�u�W�F�N�g���폜����Ă����ꍇ�A�|�C���^��Null->isValid)
		inline virtual void OnCollisionExit(const ContactInfo& contactInfo) {}

		//----------------------------------------------------------------------------------
		//[OnTriggerEnter]
		//Trigger�����������u�Ԃ�Callback�����֐�
		//����1: �ڐG�I�u�W�F�N�g�̏��
		inline virtual void OnTriggerEnter(const ContactInfo& contactInfo) {}
		//----------------------------------------------------------------------------------
		//[OnTriggerStay]
		//Trigger���������Ă����Callback�����֐�
		//����1: �ڐG�I�u�W�F�N�g�̏��
		inline virtual void OnTriggerStay(const ContactInfo& contactInfo) {}
		//----------------------------------------------------------------------------------
		//[OnTriggerExit]
		//Trigger�����ꂽ�u�Ԃ�Callback�����֐�
		//����1: �ڐG�I�u�W�F�N�g�̏�� (�I�u�W�F�N�g���폜����Ă����ꍇ�A�|�C���^��Null->isValid)
		inline virtual void OnTriggerExit(const ContactInfo& contactInfo) {}

		//----------------------------------------------------------------------------------
		//[ChangeUpperHierarchy]
		//parent�ȏ�̊K�w�֌W�ɕύX���s��ꂽ(parent�̕ύX�Ȃ�)�u�Ԃ�Callback�����֐�
		//�d�l��UpdateFlame�ŌĂяo�����\�������邱�Ƃɗ��ӂ��邱�� (parent�̕ύX�Ȃǂ��s�����X���b�h�ŌĂяo��)
		//����1: new parent (null�̉\������, ����̐e�ł͂Ȃ��\��������)
		//����2: old parent (null�̉\������, ����̐e�ł͂Ȃ��\��������)
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
		//[�R���X�g���N�^]
		inline GameObject()
			: Object(L""), cThisScene(Scene::SceneManager::getNowScene()),
			cIsFindEnabled(false), cIsUsingMutexPointer(false), cIsUsingAutoGabageCollection(false) {}


		//Instantiate���s���Ɏg�p����InstantiateRequiredInfo structure
		struct InstantiateRequiredInfo
		{
			//TimeLayer, default = root layer
			WeakPointer<Time::TimeLayer> timeLayer = Time::rootTimeLayer();
			//name, default = "Game Object"
			sgstring name = L"Game Object";
			//rotation(Instantiate�̐ݒ�l�ŏ㏑�������\��������܂�), default = identity
			Quaternion rotation = Const::Quaternion::identity;
			//position(Instantiate�̐ݒ�l�ŏ㏑�������\��������܂�), default = zero
			Vector3 position = Const::Vector3::zero;
			//scale, default = one
			Vector3 scale = Const::Vector3::one;
			//layer, default = Default Layer
			Layer layer = Layer::DefaultLayer();
			//tag, default = Default Tag
			Tag tag = Tag::DefaultTag();
			//use function line if root transform, default = auto
			int functionLineAtRoot = Transform::cAutoSelectFunctionLine;
			//Find�n�֐��p���X�g�ɓo�^����, �p�t�H�[�}���X�ɑ傫���֌W���܂�, default = true
			bool isFindEnabled = true;
			//isStatic, default = false
			bool isStatic = false;
		};

		//Instantiate���s���Ɏg�p����InstantiateParentInfo structure
		struct InstantiateParentInfo
		{
			//�eTransform
			SharedPointer<Transform> parent = SharedPointer<Transform>::getNull();
			//position, rotation, scale -> true = world : false = local 
			bool isInstantiateInWorldSpace = true;
		};

	private:
		//callback�����ۂɍs���R���|�[�l���g�ƂȂ�GameObjectCallback class
		class GameObjectCallback : public Component
		{
		public:
			//----------------------------------------------------------------------------------
			//[�R���X�g���N�^]
			inline GameObjectCallback(GameObject& gameObject) : gameObjectReference(gameObject) {}

			//GameObject�̂��̂��Ăяo��
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
			//���g�̎Q��
			GameObject& gameObjectReference;
		};

		//----------------------------------------------------------------------------------
		//[RegisterObjects]
		//�I�u�W�F�N�g���X�g�ւ̓o�^
		inline void RegisterObjects()
		{
			//�I�u�W�F�N�g���X�g�ɓo�^
			AtomicOperation::LockAtomic(m_staticLock);
			m_findObjects.try_emplace(instanceID(), this);
			AtomicOperation::UnlockAtomic(m_staticLock);
		}

		//Find�p�I�u�W�F�N�g���X�g
		static std::unordered_map<uint, GameObject*> m_findObjects;
		//Destroy�p�I�u�W�F�N�g���X�g
		static std::vector<SharedPointer<GameObject>> m_destroyObjects;
		//���X�g�pLock
		static std::atomic_bool m_staticLock;

		//transform (shared)
		SharedPointer<Transform> m_transform;
	};


	//----------------------------------------------------------------------------------
	//[Instantiate]
	//GameObject���C���X�^���X������ (�l�͑S��virtual InstantiateSetting Callback�Őݒ�)
	//template: GameObject�p���N���X
	//return: �C���X�^���X������GameObject
	//����1: position, InstantiateSetting�Őݒ肳�ꂽ���̂��㏑�����܂�
	//����2: rotation, InstantiateSetting�Őݒ肳�ꂽ���̂��㏑�����܂�
	//����3: �e�A�v���P�[�V�����ŃJ�X�^�}�C�Y�ł���user flags data, default = 0
	//����4: SharedPointer->isUsedMutexCounter, default = ture
	//����5: SharedPointer->isAutoGabageCollection, default = ture
	template<class GameObjectDerived>
	inline SharedPointer<GameObjectDerived> GameObject::Instantiate(const Vector3 & position, const Quaternion & rotation,
		uint userFlags, bool isUsedMutexCounter, bool isAutoGabageCollection)
	{
		static_assert(std::is_base_of_v<GameObject, GameObjectDerived>,
			"GameObject::Instantiate templatre -> GameObject derived class only!");

		//�|�C���^�쐬, friend�N���X��G��̂�GameObject�Ƀ_�E���L���X�g�������̂��p��
		SharedPointer<GameObjectDerived> result(new GameObjectDerived(), isUsedMutexCounter, isAutoGabageCollection);
		SharedPointer<GameObject> downCast = result.DownCast<GameObject>();
		//InstantiateInfo
		InstantiateRequiredInfo info0;
		InstantiateParentInfo info1;

		//�l��ݒ�
		downCast->InstantiateSetting(info0, info1, userFlags, isUsedMutexCounter, isAutoGabageCollection);

		//�e�ϐ��̏�����
		*const_cast<bool*>(&downCast->cIsUsingMutexPointer) = isUsedMutexCounter;
		*const_cast<bool*>(&downCast->cIsUsingAutoGabageCollection) = isAutoGabageCollection;
		*const_cast<bool*>(&downCast->cIsFindEnabled) = info0.isFindEnabled;

		//parent�����݂��Ȃ�->instantiate root transform
		if (IS_FALSE(info1.parent.getIsValid()))
		{
			downCast->m_transform = Transform::Instantiate(position, info0.scale, rotation, info0.name,
				info0.functionLineAtRoot, info0.isStatic, info0.layer, info0.tag, isUsedMutexCounter, isAutoGabageCollection);
		}
		//parent�����݂���->instantiate children transform
		else
		{
			downCast->m_transform = Transform::Instantiate(info1.parent, info1.isInstantiateInWorldSpace,
				position, info0.scale, rotation, info0.name, info0.functionLineAtRoot,
				info0.isStatic, info0.layer, info0.tag, isUsedMutexCounter, isAutoGabageCollection);
		}
		downCast->setName(downCast->m_transform->name());

		//�|�C���^�̓o�^
		downCast->gameObject = downCast;
		downCast->timeLayer = info0.timeLayer;
		//transform->�|�C���^�̓o�^
		downCast->transform = downCast->m_transform;
		downCast->m_transform->timeLayer = info0.timeLayer;
		downCast->m_transform->gameObject = downCast;

		//find enabled -> �o�^
		if (IS_TRUE(downCast->cIsFindEnabled))
			downCast->RegisterObjects();

		//Add Component (AddComponent����Ăяo���ƎQ�Ɛݒ肪�ł��Ȃ�)
		{
			//callback instantiate
			SharedPointer<GameObjectCallback> component(new GameObjectCallback(*downCast),
				isUsedMutexCounter, isAutoGabageCollection);
			//�e�ϐ���������
			downCast->gameObjectCallbackComponent = component;
			component->transform = downCast->m_transform;
			component->setName(downCast->m_transform->name());
			component->timeLayer = info0.timeLayer;
			component->gameObject = downCast;
			component->m_isEnabled = component->m_isOldEnabled = true;
			component->m_isOldActive = downCast->m_transform->m_isActive;
			component->m_thisPointer = component.DownCast<Component>();

			//Callback flags�擾
			uint flags = component->EnableCallbacks();
			//private sync�ݒ�
			if (component->m_callbackEnabledFlags & Component::CallbackFunctions::m_cSyncIncludeFlags)
				flags |= Component::CallbackFunctions::m_cSync;
			//������
			component->m_callbackEnabledFlags = component->m_saveCallbackEnabledFlags = flags;

			//component���X�g�֓o�^
			downCast->m_transform->m_components.emplace_back(component.DownCast<Component>());

			//��Allocate
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
	//GameObject���C���X�^���X������ (�l�͑S��virtual InstantiateSetting Callback�Őݒ�)
	//template: GameObject�p���N���X
	//return: �C���X�^���X������GameObject
	//����1: name, InstantiateSetting�Őݒ肳�ꂽ���̂��㏑�����܂�
	//����2: �e�A�v���P�[�V�����ŃJ�X�^�}�C�Y�ł���user flags data, default = 0
	//����3: SharedPointer->isUsedMutexCounter, default = ture
	//����4: SharedPointer->isAutoGabageCollection, default = ture
	template<class GameObjectDerived>
	inline SharedPointer<GameObjectDerived> GameObject::Instantiate(const sgstring & name, 
		uint userFlags, bool isUsedMutexCounter, bool isAutoGabageCollection)
	{
		static_assert(std::is_base_of_v<GameObject, GameObjectDerived>,
			"GameObject::Instantiate templatre -> GameObject derived class only!");

		//�|�C���^�쐬, friend�N���X��G��̂�GameObject�Ƀ_�E���L���X�g�������̂��p��
		SharedPointer<GameObjectDerived> result(new GameObjectDerived(), isUsedMutexCounter, isAutoGabageCollection);
		SharedPointer<GameObject> downCast = result.DownCast<GameObject>();
		//InstantiateInfo
		InstantiateRequiredInfo info0;
		InstantiateParentInfo info1;

		//�l��ݒ�
		downCast->InstantiateSetting(info0, info1, userFlags, isUsedMutexCounter, isAutoGabageCollection);

		//�e�ϐ��̏�����
		*const_cast<bool*>(&downCast->cIsUsingMutexPointer) = isUsedMutexCounter;
		*const_cast<bool*>(&downCast->cIsUsingAutoGabageCollection) = isAutoGabageCollection;
		*const_cast<bool*>(&downCast->cIsFindEnabled) = info0.isFindEnabled;

		//parent�����݂��Ȃ�->instantiate root transform
		if (IS_FALSE(info1.parent.getIsValid()))
		{
			downCast->m_transform = Transform::Instantiate(info0.position, info0.scale, info0.rotation, name,
				info0.functionLineAtRoot, info0.isStatic, info0.layer, info0.tag, isUsedMutexCounter, isAutoGabageCollection);
		}
		//parent�����݂���->instantiate children transform
		else
		{
			downCast->m_transform = Transform::Instantiate(info1.parent, info1.isInstantiateInWorldSpace,
				info0.position, info0.scale, info0.rotation, name, info0.functionLineAtRoot,
				info0.isStatic, info0.layer, info0.tag, isUsedMutexCounter, isAutoGabageCollection);
		}
		downCast->setName(downCast->m_transform->name());

		//�|�C���^�̓o�^
		downCast->gameObject = downCast;
		downCast->timeLayer = info0.timeLayer;
		//transform->�|�C���^�̓o�^
		downCast->transform = downCast->m_transform;
		downCast->m_transform->timeLayer = info0.timeLayer;
		downCast->m_transform->gameObject = downCast;

		//find enabled -> �o�^
		if (IS_TRUE(downCast->cIsFindEnabled))
			downCast->RegisterObjects();

		//Add Component (AddComponent����Ăяo���ƎQ�Ɛݒ肪�ł��Ȃ�)
		{
			//callback instantiate
			SharedPointer<GameObjectCallback> component(new GameObjectCallback(*downCast),
				isUsedMutexCounter, isAutoGabageCollection);
			//�e�ϐ���������
			downCast->gameObjectCallbackComponent = component;
			component->transform = downCast->m_transform;
			component->setName(downCast->m_transform->name());
			component->timeLayer = info0.timeLayer;
			component->gameObject = downCast;
			component->m_isEnabled = component->m_isOldEnabled = true;
			component->m_isOldActive = downCast->m_transform->m_isActive;
			component->m_thisPointer = component.DownCast<Component>();

			//Callback flags�擾
			uint flags = component->EnableCallbacks();
			//private sync�ݒ�
			if (component->m_callbackEnabledFlags & Component::CallbackFunctions::m_cSyncIncludeFlags)
				flags |= Component::CallbackFunctions::m_cSync;
			//������
			component->m_callbackEnabledFlags = component->m_saveCallbackEnabledFlags = flags;

			//component���X�g�֓o�^
			downCast->m_transform->m_components.emplace_back(component.DownCast<Component>());

			//��Allocate
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
	//GameObject���C���X�^���X������ (�l�͑S��virtual InstantiateSetting Callback�Őݒ�)
	//template: GameObject�p���N���X
	//return: �C���X�^���X������GameObject
	//����1: �e�A�v���P�[�V�����ŃJ�X�^�}�C�Y�ł���user flags data, default = 0
	//����2: SharedPointer->isUsedMutexCounter, default = ture
	//����3: SharedPointer->isAutoGabageCollection, default = ture
	template <class GameObjectDerived>
	inline SharedPointer<GameObjectDerived> GameObject::Instantiate(uint userFlags,
		bool isUsedMutexCounter, bool isAutoGabageCollection)
	{
		static_assert(std::is_base_of_v<GameObject, GameObjectDerived>,
			"GameObject::Instantiate templatre -> GameObject derived class only!");

		//�|�C���^�쐬, friend�N���X��G��̂�GameObject�Ƀ_�E���L���X�g�������̂��p��
		SharedPointer<GameObjectDerived> result(new GameObjectDerived(), isUsedMutexCounter, isAutoGabageCollection);
		SharedPointer<GameObject> downCast = result.DownCast<GameObject>();
		//InstantiateInfo
		InstantiateRequiredInfo info0;
		InstantiateParentInfo info1;

		//�l��ݒ�
		downCast->InstantiateSetting(info0, info1, userFlags, isUsedMutexCounter, isAutoGabageCollection);

		//�e�ϐ��̏�����
		*const_cast<bool*>(&downCast->cIsUsingMutexPointer) = isUsedMutexCounter;
		*const_cast<bool*>(&downCast->cIsUsingAutoGabageCollection) = isAutoGabageCollection;
		*const_cast<bool*>(&downCast->cIsFindEnabled) = info0.isFindEnabled;
		
		//parent�����݂��Ȃ�->instantiate root transform
		if (IS_FALSE(info1.parent.getIsValid()))
		{
			downCast->m_transform = Transform::Instantiate(info0.position, info0.scale, info0.rotation, info0.name,
				info0.functionLineAtRoot, info0.isStatic, info0.layer, info0.tag, isUsedMutexCounter, isAutoGabageCollection);
		}
		//parent�����݂���->instantiate children transform
		else
		{
			downCast->m_transform = Transform::Instantiate(info1.parent, info1.isInstantiateInWorldSpace,
				info0.position, info0.scale, info0.rotation, info0.name, info0.functionLineAtRoot, 
				info0.isStatic, info0.layer, info0.tag, isUsedMutexCounter, isAutoGabageCollection);
		}
		downCast->setName(downCast->m_transform->name());

		//�|�C���^�̓o�^
		downCast->gameObject = downCast;
		downCast->timeLayer = info0.timeLayer;
		//transform->�|�C���^�̓o�^
		downCast->transform = downCast->m_transform;
		downCast->m_transform->timeLayer = info0.timeLayer;
		downCast->m_transform->gameObject = downCast;

		//find enabled -> �o�^
		if (IS_TRUE(downCast->cIsFindEnabled))
			downCast->RegisterObjects();

		//Add Component (AddComponent����Ăяo���ƎQ�Ɛݒ肪�ł��Ȃ�)
		{
			//callback instantiate
			SharedPointer<GameObjectCallback> component(new GameObjectCallback(*downCast),
				isUsedMutexCounter, isAutoGabageCollection);
			//�e�ϐ���������
			downCast->gameObjectCallbackComponent = component;
			component->transform = downCast->m_transform;
			component->setName(downCast->m_transform->name());
			component->timeLayer = info0.timeLayer;
			component->gameObject = downCast;
			component->m_isEnabled = component->m_isOldEnabled = true;
			component->m_isOldActive = downCast->m_transform->m_isActive;
			component->m_thisPointer = component.DownCast<Component>();

			//Callback flags�擾
			uint flags = component->EnableCallbacks();
			//private sync�ݒ�
			if (component->m_callbackEnabledFlags & Component::CallbackFunctions::m_cSyncIncludeFlags)
				flags |= Component::CallbackFunctions::m_cSync;
			//������
			component->m_callbackEnabledFlags = component->m_saveCallbackEnabledFlags = flags;

			//component���X�g�֓o�^
			downCast->m_transform->m_components.emplace_back(component.DownCast<Component>());

			//��Allocate
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
	//GameObject���C���X�^���X������ (�l�͑S��virtual InstantiateSetting Callback�Őݒ�)
	//template: GameObject�p���N���X
	//return: �C���X�^���X������GameObject, parent�������̏ꍇnull
	//����1: �eTransform, InstantiateSetting�Őݒ肳�ꂽ���̂��㏑�����܂�
	//����2: position, rotation, scale -> true = world : false = local , InstantiateSetting�Őݒ肳�ꂽ���̂��㏑�����܂�
	//����3: position, InstantiateSetting�Őݒ肳�ꂽ���̂��㏑�����܂�
	//����4: rotation, InstantiateSetting�Őݒ肳�ꂽ���̂��㏑�����܂�
	//����5: �e�A�v���P�[�V�����ŃJ�X�^�}�C�Y�ł���user flags data, default = 0
	//����6: SharedPointer->isUsedMutexCounter, default = ture
	//����7: SharedPointer->isAutoGabageCollection, default = ture
	template<class GameObjectDerived>
	inline SharedPointer<GameObjectDerived> GameObject::Instantiate(SharedPointer<Transform> parent,
		bool isInstantiateInWorldSpace, const Vector3 & position, const Quaternion & rotation, 
		uint userFlags, bool isUsedMutexCounter, bool isAutoGabageCollection)
	{
		static_assert(std::is_base_of_v<GameObject, GameObjectDerived>,
			"GameObject::Instantiate templatre -> GameObject derived class only!");

		//parent�����݂��Ȃ�->return
		if (IS_FALSE(parent.getIsValid()))
			return SharedPointer<GameObjectDerived>::getNull();

		//�|�C���^�쐬, friend�N���X��G��̂�GameObject�Ƀ_�E���L���X�g�������̂��p��
		SharedPointer<GameObjectDerived> result(new GameObjectDerived(), isUsedMutexCounter, isAutoGabageCollection);
		SharedPointer<GameObject> downCast = result.DownCast<GameObject>();
		//InstantiateInfo
		InstantiateRequiredInfo info0;
		InstantiateParentInfo info1;

		//�l��ݒ�
		downCast->InstantiateSetting(info0, info1, userFlags, isUsedMutexCounter, isAutoGabageCollection);

		//�e�ϐ��̏�����
		*const_cast<bool*>(&downCast->cIsUsingMutexPointer) = isUsedMutexCounter;
		*const_cast<bool*>(&downCast->cIsUsingAutoGabageCollection) = isAutoGabageCollection;
		*const_cast<bool*>(&downCast->cIsFindEnabled) = info0.isFindEnabled;

		downCast->m_transform = Transform::Instantiate(parent, isInstantiateInWorldSpace,
			position, info0.scale, rotation, info0.name, info0.functionLineAtRoot,
			info0.isStatic, info0.layer, info0.tag, isUsedMutexCounter, isAutoGabageCollection);
		downCast->setName(downCast->m_transform->name());

		//�|�C���^�̓o�^
		downCast->gameObject = downCast;
		downCast->timeLayer = info0.timeLayer;
		//transform->�|�C���^�̓o�^
		downCast->transform = downCast->m_transform;
		downCast->m_transform->timeLayer = info0.timeLayer;
		downCast->m_transform->gameObject = downCast;

		//find enabled -> �o�^
		if (IS_TRUE(downCast->cIsFindEnabled))
			downCast->RegisterObjects();

		//Add Component (AddComponent����Ăяo���ƎQ�Ɛݒ肪�ł��Ȃ�)
		{
			//callback instantiate
			SharedPointer<GameObjectCallback> component(new GameObjectCallback(*downCast),
				isUsedMutexCounter, isAutoGabageCollection);
			//�e�ϐ���������
			downCast->gameObjectCallbackComponent = component;
			component->transform = downCast->m_transform;
			component->setName(downCast->m_transform->name());
			component->timeLayer = info0.timeLayer;
			component->gameObject = downCast;
			component->m_isEnabled = component->m_isOldEnabled = true;
			component->m_isOldActive = downCast->m_transform->m_isActive;
			component->m_thisPointer = component.DownCast<Component>();

			//Callback flags�擾
			uint flags = component->EnableCallbacks();
			//private sync�ݒ�
			if (component->m_callbackEnabledFlags & Component::CallbackFunctions::m_cSyncIncludeFlags)
				flags |= Component::CallbackFunctions::m_cSync;
			//������
			component->m_callbackEnabledFlags = component->m_saveCallbackEnabledFlags = flags;

			//component���X�g�֓o�^
			downCast->m_transform->m_components.emplace_back(component.DownCast<Component>());

			//��Allocate
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
	//GameObject���C���X�^���X������ (�l�͑S��virtual InstantiateSetting Callback�Őݒ�)
	//template: GameObject�p���N���X
	//return: �C���X�^���X������GameObject, parent�������̏ꍇnull
	//����1: �eTransform, InstantiateSetting�Őݒ肳�ꂽ���̂��㏑�����܂�
	//����2: position, rotation, scale -> true = world : false = local , InstantiateSetting�Őݒ肳�ꂽ���̂��㏑�����܂�
	//����3: name, InstantiateSetting�Őݒ肳�ꂽ���̂��㏑�����܂�
	//����4: �e�A�v���P�[�V�����ŃJ�X�^�}�C�Y�ł���user flags data, default = 0
	//����5: SharedPointer->isUsedMutexCounter, default = ture
	//����6: SharedPointer->isAutoGabageCollection, default = ture
	template<class GameObjectDerived>
	inline SharedPointer<GameObjectDerived> GameObject::Instantiate(SharedPointer<Transform> parent,
		bool isInstantiateInWorldSpace, const sgstring & name, uint userFlags, bool isUsedMutexCounter, bool isAutoGabageCollection)
	{
		static_assert(std::is_base_of_v<GameObject, GameObjectDerived>,
			"GameObject::Instantiate templatre -> GameObject derived class only!");

		//parent�����݂��Ȃ�->return
		if (IS_FALSE(parent.getIsValid()))
			return SharedPointer<GameObjectDerived>::getNull();

		//�|�C���^�쐬, friend�N���X��G��̂�GameObject�Ƀ_�E���L���X�g�������̂��p��
		SharedPointer<GameObjectDerived> result(new GameObjectDerived(), isUsedMutexCounter, isAutoGabageCollection);
		SharedPointer<GameObject> downCast = result.DownCast<GameObject>();
		//InstantiateInfo
		InstantiateRequiredInfo info0;
		InstantiateParentInfo info1;

		//�l��ݒ�
		downCast->InstantiateSetting(info0, info1, userFlags, isUsedMutexCounter, isAutoGabageCollection);

		//�e�ϐ��̏�����
		*const_cast<bool*>(&downCast->cIsUsingMutexPointer) = isUsedMutexCounter;
		*const_cast<bool*>(&downCast->cIsUsingAutoGabageCollection) = isAutoGabageCollection;
		*const_cast<bool*>(&downCast->cIsFindEnabled) = info0.isFindEnabled;

		downCast->m_transform = Transform::Instantiate(parent, isInstantiateInWorldSpace,
			info0.position, info0.scale, info0.rotation, name, info0.functionLineAtRoot,
			info0.isStatic, info0.layer, info0.tag, isUsedMutexCounter, isAutoGabageCollection);
		downCast->setName(downCast->m_transform->name());

		//�|�C���^�̓o�^
		downCast->gameObject = downCast;
		downCast->timeLayer = info0.timeLayer;
		//transform->�|�C���^�̓o�^
		downCast->transform = downCast->m_transform;
		downCast->m_transform->timeLayer = info0.timeLayer;
		downCast->m_transform->gameObject = downCast;

		//find enabled -> �o�^
		if (IS_TRUE(downCast->cIsFindEnabled))
			downCast->RegisterObjects();

		//Add Component (AddComponent����Ăяo���ƎQ�Ɛݒ肪�ł��Ȃ�)
		{
			//callback instantiate
			SharedPointer<GameObjectCallback> component(new GameObjectCallback(*downCast),
				isUsedMutexCounter, isAutoGabageCollection);
			//�e�ϐ���������
			downCast->gameObjectCallbackComponent = component;
			component->transform = downCast->m_transform;
			component->setName(downCast->m_transform->name());
			component->timeLayer = info0.timeLayer;
			component->gameObject = downCast;
			component->m_isEnabled = component->m_isOldEnabled = true;
			component->m_isOldActive = downCast->m_transform->m_isActive;
			component->m_thisPointer = component.DownCast<Component>();

			//Callback flags�擾
			uint flags = component->EnableCallbacks();
			//private sync�ݒ�
			if (component->m_callbackEnabledFlags & Component::CallbackFunctions::m_cSyncIncludeFlags)
				flags |= Component::CallbackFunctions::m_cSync;
			//������
			component->m_callbackEnabledFlags = component->m_saveCallbackEnabledFlags = flags;

			//component���X�g�֓o�^
			downCast->m_transform->m_components.emplace_back(component.DownCast<Component>());

			//��Allocate
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
	//GameObject���C���X�^���X������ (�l�͑S��virtual InstantiateSetting Callback�Őݒ�)
	//template: GameObject�p���N���X
	//return: �C���X�^���X������GameObject, parent�������̏ꍇnull
	//����1: �eTransform, InstantiateSetting�Őݒ肳�ꂽ���̂��㏑�����܂�
	//����2: position, rotation, scale -> true = world : false = local , InstantiateSetting�Őݒ肳�ꂽ���̂��㏑�����܂�
	//����3: �e�A�v���P�[�V�����ŃJ�X�^�}�C�Y�ł���user flags data, default = 0
	//����4: SharedPointer->isUsedMutexCounter, default = ture
	//����5: SharedPointer->isAutoGabageCollection, default = ture
	template <class GameObjectDerived>
	inline SharedPointer<GameObjectDerived> GameObject::Instantiate(SharedPointer<Transform> parent,
		bool isInstantiateInWorldSpace, uint userFlags, bool isUsedMutexCounter, bool isAutoGabageCollection)
	{
		static_assert(std::is_base_of_v<GameObject, GameObjectDerived>,
			"GameObject::Instantiate templatre -> GameObject derived class only!");

		//parent�����݂��Ȃ�->return
		if (IS_FALSE(parent.getIsValid()))
			return SharedPointer<GameObjectDerived>::getNull();

		//�|�C���^�쐬, friend�N���X��G��̂�GameObject�Ƀ_�E���L���X�g�������̂��p��
		SharedPointer<GameObjectDerived> result(new GameObjectDerived(), isUsedMutexCounter, isAutoGabageCollection);
		SharedPointer<GameObject> downCast = result.DownCast<GameObject>();
		//InstantiateInfo
		InstantiateRequiredInfo info0;
		InstantiateParentInfo info1;

		//�l��ݒ�
		downCast->InstantiateSetting(info0, info1, userFlags, isUsedMutexCounter, isAutoGabageCollection);

		//�e�ϐ��̏�����
		*const_cast<bool*>(&downCast->cIsUsingMutexPointer) = isUsedMutexCounter;
		*const_cast<bool*>(&downCast->cIsUsingAutoGabageCollection) = isAutoGabageCollection;
		*const_cast<bool*>(&downCast->cIsFindEnabled) = info0.isFindEnabled;

		//parent�����݂��Ȃ�->instantiate root transform
		if (IS_FALSE(info1.parent.getIsValid()))
		{
			downCast->m_transform = Transform::Instantiate(info0.position, info0.scale, info0.rotation, info0.name,
				info0.functionLineAtRoot, info0.isStatic, info0.layer, info0.tag, isUsedMutexCounter, isAutoGabageCollection);
		}
		//parent�����݂���->instantiate children transform
		else
		{
			downCast->m_transform = Transform::Instantiate(info1.parent, info1.isInstantiateInWorldSpace,
				info0.position, info0.scale, info0.rotation, info0.name, info0.functionLineAtRoot,
				info0.isStatic, info0.layer, info0.tag, isUsedMutexCounter, isAutoGabageCollection);
		}
		downCast->setName(downCast->m_transform->name());

		//�|�C���^�̓o�^
		downCast->gameObject = downCast;
		downCast->timeLayer = info0.timeLayer;
		//transform->�|�C���^�̓o�^
		downCast->transform = downCast->m_transform;
		downCast->m_transform->timeLayer = info0.timeLayer;
		downCast->m_transform->gameObject = downCast;

		//find enabled -> �o�^
		if (IS_TRUE(downCast->cIsFindEnabled))
			downCast->RegisterObjects();

		//Add Component (AddComponent����Ăяo���ƎQ�Ɛݒ肪�ł��Ȃ�)
		{
			//callback instantiate
			SharedPointer<GameObjectCallback> component(new GameObjectCallback(*downCast),
				isUsedMutexCounter, isAutoGabageCollection);
			//�e�ϐ���������
			downCast->gameObjectCallbackComponent = component;
			component->transform = downCast->m_transform;
			component->setName(downCast->m_transform->name());
			component->timeLayer = info0.timeLayer;
			component->gameObject = downCast;
			component->m_isEnabled = component->m_isOldEnabled = true;
			component->m_isOldActive = downCast->m_transform->m_isActive;
			component->m_thisPointer = component.DownCast<Component>();

			//Callback flags�擾
			uint flags = component->EnableCallbacks();
			//private sync�ݒ�
			if (component->m_callbackEnabledFlags & Component::CallbackFunctions::m_cSyncIncludeFlags)
				flags |= Component::CallbackFunctions::m_cSync;
			//������
			component->m_callbackEnabledFlags = component->m_saveCallbackEnabledFlags = flags;

			//component���X�g�֓o�^
			downCast->m_transform->m_components.emplace_back(component.DownCast<Component>());

			//��Allocate
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
	//GameObject���C���X�^���X������ (�l�͑S�Ď蓮�ݒ�)
	//template: GameObject�p���N���X
	//return: �C���X�^���X������GameObject
	//����1: position, default = zero
	//����2: scale, default = one
	//����3: rotation, default = identity
	//����4: TimeLayer, default = root layer
	//����5: name, default = "Game Object"
	//����6: use function line if root transform, default = auto
	//����7: isStatic, default = false
	//����8: layer, default = Default Layer
	//����9: tag, default = Default Tag
	//����10: Find�n�֐��p���X�g�ɓo�^����, �p�t�H�[�}���X�ɑ傫���֌W���܂�, default = true
	//����11: SharedPointer->isUsedMutexCounter, default = ture
	//����12: SharedPointer->isAutoGabageCollection, default = ture
	template <class GameObjectDerived>
	inline SharedPointer<GameObjectDerived> GameObject::InstantiateManual(const Vector3 & position,
		const Vector3 & scale, const Quaternion & rotation, WeakPointer<Time::TimeLayer> timeLayer, 
		const sgstring & name, int functionLineAtRoot, bool isStatic, 
		Layer layer, const Tag & tag, bool isFindEnabled, bool isUsedMutexCounter, bool isAutoGabageCollection)
	{
		static_assert(std::is_base_of_v<GameObject, GameObjectDerived>,
			"GameObject::Instantiate templatre -> GameObject derived class only!");

		//�|�C���^�쐬, friend�N���X��G��̂�GameObject�Ƀ_�E���L���X�g�������̂��p��
		SharedPointer<GameObjectDerived> result(new GameObjectDerived(), isUsedMutexCounter, isAutoGabageCollection);
		SharedPointer<GameObject> downCast = result.DownCast<GameObject>();

		//�e�ϐ��̏�����
		*const_cast<bool*>(&downCast->cIsUsingMutexPointer) = isUsedMutexCounter;
		*const_cast<bool*>(&downCast->cIsUsingAutoGabageCollection) = isAutoGabageCollection;
		*const_cast<bool*>(&downCast->cIsFindEnabled) = isFindEnabled;
	
		//instantiate root transform
		downCast->m_transform = Transform::Instantiate(position, scale, rotation, name,
			functionLineAtRoot, isStatic, layer, tag, isUsedMutexCounter, isAutoGabageCollection);

		downCast->setName(downCast->m_transform->name());
		//�|�C���^�̓o�^
		downCast->gameObject = downCast;
		downCast->timeLayer = timeLayer;
		//transform->�|�C���^�̓o�^
		downCast->transform = downCast->m_transform;
		downCast->m_transform->timeLayer = timeLayer;
		downCast->m_transform->gameObject = downCast;

		//find enabled -> �o�^
		if (IS_TRUE(downCast->cIsFindEnabled))
			downCast->RegisterObjects();

		//Add Component (AddComponent����Ăяo���ƎQ�Ɛݒ肪�ł��Ȃ�)
		{
			//callback instantiate
			SharedPointer<GameObjectCallback> component(new GameObjectCallback(*downCast),
				isUsedMutexCounter, isAutoGabageCollection);
			//�e�ϐ���������
			downCast->gameObjectCallbackComponent = component;
			component->transform = downCast->m_transform;
			component->setName(downCast->m_transform->name());
			component->timeLayer = timeLayer;
			component->gameObject = downCast;
			component->m_isEnabled = component->m_isOldEnabled = true;
			component->m_isOldActive = downCast->m_transform->m_isActive;
			component->m_thisPointer = component.DownCast<Component>();

			//Callback flags�擾
			uint flags = component->EnableCallbacks();
			//private sync�ݒ�
			if (component->m_callbackEnabledFlags & Component::CallbackFunctions::m_cSyncIncludeFlags)
				flags |= Component::CallbackFunctions::m_cSync;
			//������
			component->m_callbackEnabledFlags = component->m_saveCallbackEnabledFlags = flags;

			//component���X�g�֓o�^
			downCast->m_transform->m_components.emplace_back(component.DownCast<Component>());

			//��Allocate
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
	//GameObject���C���X�^���X������ (�l�͑S�Ď蓮�ݒ�)
	//template: GameObject�p���N���X
	//return: �C���X�^���X������GameObject, parent�������̏ꍇnull
	//����1: �eTransform
	//����2: position, rotation, scale -> true = world : false = local 
	//����3: position, default = zero
	//����4: scale, default = one
	//����5: rotation, default = identity
	//����6: TimeLayer, default = root layer
	//����7: name, default = "Game Object"
	//����8: use function line if root transform, default = auto
	//����9: isStatic, default = false
	//����10: layer, default = Default Layer
	//����11: tag, default = Default Tag
	//����12: Find�n�֐��p���X�g�ɓo�^����, �p�t�H�[�}���X�ɑ傫���֌W���܂�, default = true
	//����13: SharedPointer->isUsedMutexCounter, default = ture
	//����14: SharedPointer->isAutoGabageCollection, default = ture
	template <class GameObjectDerived>
	inline SharedPointer<GameObjectDerived> GameObject::InstantiateManual(SharedPointer<Transform> parent,
		bool isInstantiateInWorldSpace, const Vector3 & position, const Vector3 & scale, const Quaternion & rotation,
		WeakPointer<Time::TimeLayer> timeLayer, const sgstring & name, int functionLineAtRoot,
		bool isStatic, Layer layer, const Tag & tag, bool isFindEnabled, bool isUsedMutexCounter, bool isAutoGabageCollection)
	{
		static_assert(std::is_base_of_v<GameObject, GameObjectDerived>,
			"GameObject::Instantiate templatre -> GameObject derived class only!");

		if (IS_FALSE(parent.getIsValid())) return SharedPointer<GameObjectDerived>::getNull();

		//�|�C���^�쐬, friend�N���X��G��̂�GameObject�Ƀ_�E���L���X�g�������̂��p��
		SharedPointer<GameObjectDerived> result(new GameObjectDerived(), isUsedMutexCounter, isAutoGabageCollection);
		SharedPointer<GameObject> downCast = result.DownCast<GameObject>();

		//�e�ϐ��̏�����
		*const_cast<bool*>(&downCast->cIsUsingMutexPointer) = isUsedMutexCounter;
		*const_cast<bool*>(&downCast->cIsUsingAutoGabageCollection) = isAutoGabageCollection;
		*const_cast<bool*>(&downCast->cIsFindEnabled) = isFindEnabled;
	
		//instantiate children transform
		downCast->m_transform = Transform::Instantiate(parent, isInstantiateInWorldSpace, position, scale, rotation, name,
			functionLineAtRoot, isStatic, layer, tag, isUsedMutexCounter, isAutoGabageCollection);
		downCast->setName(downCast->m_transform->name());

		//�|�C���^�̓o�^
		downCast->gameObject = downCast;
		downCast->timeLayer = timeLayer;
		//transform->�|�C���^�̓o�^
		downCast->transform = downCast->m_transform;
		downCast->m_transform->timeLayer = timeLayer;
		downCast->m_transform->gameObject = downCast;
		
		//find enabled -> �o�^
		if (IS_TRUE(downCast->cIsFindEnabled))
			downCast->RegisterObjects();

		//Add Component (AddComponent����Ăяo���ƎQ�Ɛݒ肪�ł��Ȃ�)
		{
			//callback instantiate
			SharedPointer<GameObjectCallback> component(new GameObjectCallback(*downCast),
				isUsedMutexCounter, isAutoGabageCollection);
			//�e�ϐ���������
			downCast->gameObjectCallbackComponent = component;
			component->transform = downCast->m_transform;
			component->setName(downCast->m_transform->name());
			component->timeLayer = timeLayer;
			component->gameObject = downCast;
			component->m_isEnabled = component->m_isOldEnabled = true;
			component->m_isOldActive = downCast->m_transform->m_isActive;
			component->m_thisPointer = component.DownCast<Component>();

			//Callback flags�擾
			uint flags = component->EnableCallbacks();
			//private sync�ݒ�
			if (component->m_callbackEnabledFlags & Component::CallbackFunctions::m_cSyncIncludeFlags)
				flags |= Component::CallbackFunctions::m_cSync;
			//������
			component->m_callbackEnabledFlags = component->m_saveCallbackEnabledFlags = flags;

			//component���X�g�֓o�^
			downCast->m_transform->m_components.emplace_back(component.DownCast<Component>());

			//��Allocate
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
	//���ݑ��݂���S�ẴI�u�W�F�N�g����name��key�Ɍ�������
	//���ӁI sub scene�ɑ��݂���I�u�W�F�N�g���q�b�g���܂�
	//return: Hit�����ł��Â��I�u�W�F�N�g
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
	//���ݑ��݂���S�ẴI�u�W�F�N�g����tag��key�Ɍ�������
	//���ӁI sub scene�ɑ��݂���I�u�W�F�N�g���q�b�g���܂�
	//return: Hit�����ł��Â��I�u�W�F�N�g
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
	//���ݑ��݂���S�ẴI�u�W�F�N�g����layer��key�Ɍ�������
	//���ӁI sub scene�ɑ��݂���I�u�W�F�N�g���q�b�g���܂�
	//return: Hit�����ł��Â��I�u�W�F�N�g
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
	//���ݑ��݂���S�ẴI�u�W�F�N�g����name��key�Ɍ�������, (All Search)
	//���ӁI sub scene�ɑ��݂���I�u�W�F�N�g���q�b�g���܂�
	//return: Hit�����S�ẴI�u�W�F�N�g�z��
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
	//���ݑ��݂���S�ẴI�u�W�F�N�g����tag��key�Ɍ�������, (All Search)
	//���ӁI sub scene�ɑ��݂���I�u�W�F�N�g���q�b�g���܂�
	//return: Hit�����S�ẴI�u�W�F�N�g�z��
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
	//���ݑ��݂���S�ẴI�u�W�F�N�g����layer��key�Ɍ�������, (All Search)
	//���ӁI sub scene�ɑ��݂���I�u�W�F�N�g���q�b�g���܂�
	//return: Hit�����S�ẴI�u�W�F�N�g�z��
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