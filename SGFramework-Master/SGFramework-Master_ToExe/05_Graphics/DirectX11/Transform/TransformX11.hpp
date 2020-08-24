/*----------------------------------------------------------------------------------
���݂̍s�����ۑ�����TransformX11 class
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_TRANSFORM_X11_HPP_
#define SGFRAMEWORK_HEADER_TRANSFORM_X11_HPP_
#include <atomic>
#include "../../../02_CustomVariable/CustomVariable.hpp"
#include "../../../03_MathAF/MathAF.hpp"
#include "../GraphicsX11/GraphicsX11.hpp"

//Framework namespace
namespace SGFramework
{
	//friend�p
	class Thread;
	class Application;
	class GameObject;
	class Physics;
	class GUI;
	namespace BaseClass
	{
		class BaseRigidBody;
	}

	struct ContactInfo;

	//�t���[�����[�N�̊Ǘ�������Administrator namespace
	namespace Administrator
	{
		//GraphicsDetail namespace
		namespace GraphicsDetail
		{
			//friend�p
			class CameraX11;
			class PolygonSquareX11;
			class ComponentX11;
			template<class MeshVertexType, class MeshMaterialPointerType>
			class MeshRendererX11;

			//���݂̍s�����ۑ�����TransformX11 class
			//<����!!>�e�q�֌W�̂���TransformX11�͕K���ʃX���b�h(Thread::FunctionLine)�ōX�V�����Ă͂Ȃ�Ȃ�
			//<����!!>�e�̋��Ȃ��I�u�W�F�N�g��world�Z�Z�̒l�ōX�V���s��, �q�I�u�W�F�N�g��world�Z�Z�͎Q�Ɛ�p, local�Z�Z�̒l�ōX�V���s��
			class TransformX11 final : public Object
			{
			private:
				//LockGuard�pValue
				struct Lock
				{
					inline Lock() : id(std::thread::id()), counter(0) {}

					std::thread::id id;	//thread id
					uint64 counter;		//counter
				};

			public:
				//friend
				friend class Thread;
				friend class GameObject;
				friend class CameraX11;				
				friend class ComponentX11;
				friend class PolygonSquareX11;
				friend class GUI;
				friend class Application;
				friend class Physics;
				friend class BaseClass::BaseRigidBody;
				template<class MeshVertexType, class MeshMaterialPointerType>
				friend class MeshRendererX11;

				DELETE_ALL_DEFAULT_CLASS_FUNCTIONS(TransformX11);
				
				//Transform public Variables
				struct TransformVariables
				{
					friend TransformX11;
					
					Vector3 worldPosition;		//world position
					Quaternion worldRotation;	//world rotation
					Vector3 worldScale;			//world scale

					Vector3 localPosition;			//local position
					Quaternion localRotation;	//local rotation
					Vector3 localScale;				//local scale
					
				private:
					inline TransformVariables(const Quaternion& worldRotation,
					const Vector3& worldPosition, const Vector3& worldScale, 
					const Quaternion& localRotation, const Vector3& localPosition, const Vector3& localScale) 
						: worldRotation(worldRotation), worldPosition(worldPosition), worldScale(worldScale),
					localRotation(localRotation), localPosition(localPosition), localScale(localScale) {}
					
					inline TransformVariables(const TransformVariables& copy)
					{
						memcpy_s(this, sizeof(TransformVariables), &copy, sizeof(TransformVariables));
					}
				};
				//Transfrom lock atomic
				class LockGuardTransform
				{
				public:
					friend class TransformX11;

					//�R���X�g���N�^
					inline LockGuardTransform(SharedPointer<TransformX11> transform) 
						: m_lock(const_cast<TransformX11*>(transform.getPointer())->m_lock)
					{
						std::thread::id thisID = std::this_thread::get_id();
						Lock newLock, oldLock;
						bool result = false;

						//�����X���b�h�����b�N���Ă��Ȃ���΃��b�N����܂őҋ@
						newLock.id = thisID;
						do
						{
							oldLock = m_lock.load();
							oldLock.id = m_cDefauldID;
							newLock.counter = oldLock.counter + 1;

							result = m_lock.compare_exchange_weak(oldLock, newLock);
							if (IS_FALSE(result))
							{
								oldLock = m_lock.load();
								oldLock.id = thisID;
								newLock.counter = oldLock.counter + 1;
								result = m_lock.compare_exchange_weak(oldLock, newLock);
							}
						} while (IS_FALSE(result));
					}
					//�R���X�g���N�^
					inline LockGuardTransform(std::atomic<Lock>& lock) : m_lock(lock)
					{
						std::thread::id thisID = std::this_thread::get_id();
						Lock newLock, oldLock;
						bool result = false;

						//�����X���b�h�����b�N���Ă��Ȃ���΃��b�N����܂őҋ@
						newLock.id = thisID;
						do
						{
							oldLock = m_lock.load();
							oldLock.id = m_cDefauldID;
							newLock.counter = oldLock.counter + 1;

							result = m_lock.compare_exchange_weak(oldLock, newLock);
							if (IS_FALSE(result))
							{
								oldLock = m_lock.load();
								oldLock.id = thisID;
								newLock.counter = oldLock.counter + 1;
								result = m_lock.compare_exchange_weak(oldLock, newLock);
							}
						} while (IS_FALSE(result));
					}
					//�f�X�g���N�^
					inline ~LockGuardTransform()
					{
						Lock newLock, oldLock;

						//�����X���b�h�����b�N���Ă��Ȃ���΃��b�N����܂őҋ@
						do
						{
							oldLock = m_lock.load();
							
							newLock.counter = oldLock.counter - 1;
							
							newLock.id = newLock.counter == 0 ? m_cDefauldID : oldLock.id;

						} while (IS_FALSE(m_lock.compare_exchange_weak(oldLock, newLock)));
					}
				private:
					static std::thread::id m_cDefauldID;
					static std::atomic<bool> m_isNowSyncFrame;

					std::atomic<Lock>& m_lock;
				};

				
				//----------------------------------------------------------------------------------
				//[Instantiate]
				//Transform�����̉�����, �K���V���O���X���b�h��Ԃ̎��Ɏ��̉����邱��
				//return: transform
				//����1: position, default = zero
				//����2: scale, default = one
				//����3: rotation, default = identity
				//����4: name, default = "Transform only object"
				//����5: use function line if root transform, default = auto
				//����6: isStatic, default = false
				//����7: layer, default = Default Layer
				//����8: tag, default = Default Tag
				//����9: SharedPointer->isUsedMutexCounter, default = ture
				//����10: SharedPointer->isAutoGabageCollection, default = ture
				inline static SharedPointer<TransformX11> Instantiate(const Vector3& position = Const::Vector3::zero,
					const Vector3& scale = Const::Vector3::one, const Quaternion& rotation = Const::Quaternion::identity,
					const sgstring& name = L"Transform only object", int functionLineAtRoot = cAutoSelectFunctionLine,
					bool isStatic = false, Layer layer = Layer::DefaultLayer(), const Tag & tag = Tag::DefaultTag(),
					bool isUsedMutexCounter = true, bool isAutoGabageCollection = true);
				//----------------------------------------------------------------------------------
				//[Instantiate]
				//Transform�����̉�����, �K���V���O���X���b�h��Ԃ̎��Ɏ��̉����邱��
				//position = zero, scale = one, rotation = identity
				//return: transform, parent�������ȃ|�C���^�������ꍇNull�|�C���^
				//����1: �eTransform
				//����2: position, rotation, scale -> true = world : false = local 
				//����3: position, default = zero
				//����4: scale, default = one
				//����5: rotation, default = identity
				//����6: name, default = "Transform only object"
				//����7: use function line if root transform, default = auto
				//����8: isStatic, default = false
				//����9: layer, default = Default Layer
				//����10: tag, default = Default Tag
				//����11: SharedPointer->isUsedMutexCounter, default = ture
				//����12: SharedPointer->isAutoGabageCollection, default = ture
				inline static SharedPointer<TransformX11> Instantiate(SharedPointer<TransformX11> parent,
					bool isInstantiateInWorldSpace, const Vector3& position = Const::Vector3::zero,
					const Vector3& scale = Const::Vector3::one, const Quaternion& rotation = Const::Quaternion::identity, 
					const sgstring& name = L"Transform only object", int functionLineAtRoot = cAutoSelectFunctionLine,
					bool isStatic = false, Layer layer = Layer::DefaultLayer(), const Tag & tag = Tag::DefaultTag(),
					bool isUsedMutexCounter = true, bool isAutoGabageCollection = true);

				//----------------------------------------------------------------------------------
				//[�f�X�g���N�^]
				//�q�I�u�W�F�N�g�̊J��
				inline ~TransformX11();

				//----------------------------------------------------------------------------------
				//[Translate]
				//worldPosition + translate
				//return: new worldPosition
				//����1: translate vec
				inline Vector3 Translate(const Vector3& translate)
				{
					LockGuardTransform guard(m_lock);
					m_isNotify = true;
					if (IS_FALSE(m_parent.getIsValid()))
					{
						m_variables.localPosition = m_variables.worldPosition += translate;
						if (m_childrens.size() > 0)
						{
							XmVector quaternion = m_variables.worldRotation.ToXmVector();
							VariablesUpdate(m_variables.worldPosition, quaternion, m_variables.worldScale);
						}
						return m_variables.worldPosition;
					}
					else
					{
						//World�ݒ�
						m_variables.worldPosition += translate;
						//Local�ϊ������l����
						m_variables.localPosition = m_variables.worldPosition - m_parent->getWorldPosition();

						if (m_childrens.size() > 0)
						{
							XmVector quaternion = m_variables.worldRotation.ToXmVector();
							VariablesUpdate(m_variables.worldPosition, quaternion, m_variables.worldScale);
						}
						return m_variables.worldPosition;
					}
				}
				//----------------------------------------------------------------------------------
				//[TranslateLocal]
				//localPosition + translate
				//return: new localPosition
				//����1: translate vec
				inline Vector3 TranslateLocal(const Vector3& translate)
				{
					LockGuardTransform guard(m_lock);
					m_isNotify = true;
					if (IS_FALSE(m_parent.getIsValid()))
					{
						m_variables.localPosition = m_variables.worldPosition += translate;
						if (m_childrens.size() > 0)
						{
							XmVector quaternion = m_variables.worldRotation.ToXmVector();
							VariablesUpdate(m_variables.worldPosition, quaternion, m_variables.worldScale);
						}
						return m_variables.localPosition;
					}
					else
					{
						m_variables.localPosition += translate;
						m_variables.worldPosition = m_parent->m_variables.worldPosition + m_variables.localPosition;

						if (m_childrens.size() > 0)
						{
							XmVector quaternion = m_variables.worldRotation.ToXmVector();
							VariablesUpdate(m_variables.worldPosition, quaternion, m_variables.worldScale);
						}
						return m_variables.localPosition;
					}
				}
				//----------------------------------------------------------------------------------
				//[Rotate]
				//worldRotation * quaternion
				//return: new worldRotation
				//����1: Rotate quaternion
				inline Quaternion Rotate(const Quaternion& quaternion)
				{
					LockGuardTransform guard(m_lock);
					m_isNotify = true;
					if (IS_FALSE(m_parent.getIsValid()))
					{
						m_variables.localRotation = m_variables.worldRotation *= quaternion;
						if (m_childrens.size() > 0)
						{
							XmVector vec = m_variables.worldRotation.ToXmVector();
							VariablesUpdate(m_variables.worldPosition, vec, m_variables.worldScale);
						}
						return m_variables.worldRotation;
					}
					else
					{
						//World�ݒ�
						XmVector vec = DirectX::XMQuaternionMultiply(m_variables.worldRotation, quaternion); 
						//Local�ϊ������l����
						m_variables.localRotation = vec * m_parent->getWorldRotation().getInverse();

						if (m_childrens.size() > 0)
						{
							VariablesUpdate(m_variables.worldPosition, vec, m_variables.worldScale);
						}
						return m_variables.worldRotation = vec;
					}
				}
				//----------------------------------------------------------------------------------
				//[RotateLocal]
				//localRotation * quaternion
				//return: new localRotation
				//����1: Rotate quaternion
				inline Quaternion RotateLocal(const Quaternion& quaternion)
				{
					LockGuardTransform guard(m_lock);
					m_isNotify = true;
					if (IS_FALSE(m_parent.getIsValid()))
					{
						m_variables.localRotation = m_variables.worldRotation *= quaternion;
						if (m_childrens.size() > 0)
						{
							XmVector vec = m_variables.worldRotation.ToXmVector();
							VariablesUpdate(m_variables.worldPosition, vec, m_variables.worldScale);
						}
						return m_variables.localRotation;
					}
					else
					{
						m_variables.localRotation *= quaternion;
						m_variables.worldRotation = m_parent->m_variables.worldRotation * m_variables.localRotation;

						if (m_childrens.size() > 0)
						{
							XmVector quaternion = m_variables.worldRotation.ToXmVector();
							VariablesUpdate(m_variables.worldPosition, quaternion, m_variables.worldScale);
						}
						return m_variables.localRotation;
					}
				}
				//----------------------------------------------------------------------------------
				//[Resize]
				//worldScale * magnification
				//return: new worldScale
				//����1: Scale magnification
				inline Vector3 Resize(const Vector3& magnification)
				{
					LockGuardTransform guard(m_lock);
					m_isNotify = true;
					if (IS_FALSE(m_parent.getIsValid()))
					{
						m_variables.localScale = m_variables.worldScale *= magnification;
						if (m_childrens.size() > 0)
						{
							XmVector vec = m_variables.worldRotation.ToXmVector();
							VariablesUpdate(m_variables.worldPosition, vec, m_variables.worldScale);
						}
						return m_variables.worldScale;
					}
					else
					{
						//World�ݒ�
						m_variables.worldScale *= magnification;
						//Local�ϊ������l����
						m_variables.localScale = m_variables.worldScale / m_parent->getWorldScale();

						if (m_childrens.size() > 0)
						{
							XmVector vec = m_variables.worldRotation.ToXmVector();
							VariablesUpdate(m_variables.worldPosition, vec, m_variables.worldScale);
						}
						return m_variables.worldScale;
					}
				}
				//----------------------------------------------------------------------------------
				//[ResizeLocal]
				//localScale * magnification
				//return: new localScale
				//����1: Scale magnification
				inline Vector3 ResizeLocal(const Vector3& magnification)
				{
					LockGuardTransform guard(m_lock);
					m_isNotify = true;
					if (IS_FALSE(m_parent.getIsValid())) 
					{
						m_variables.localScale = m_variables.worldScale *= magnification;
						if (m_childrens.size() > 0)
						{
							XmVector vec = m_variables.worldRotation.ToXmVector();
							VariablesUpdate(m_variables.worldPosition, vec, m_variables.worldScale);
						}
						return m_variables.localScale;
					}
					else
					{
						m_variables.localScale *= magnification;
						m_variables.worldScale = m_parent->m_variables.worldScale * m_variables.localScale;

						if (m_childrens.size() > 0)
						{
							XmVector quaternion = m_variables.worldRotation.ToXmVector();
							VariablesUpdate(m_variables.worldPosition, quaternion, m_variables.worldScale);
						}
						return m_variables.localScale;
					}
				}

				//----------------------------------------------------------------------------------
				//[FindRoot]
				//reutrn: root�|�C���^, ���Ȃ��ꍇ��Null�|�C���^ (����!���s���ɒT�����s���܂�)
				inline WeakPointer<TransformX11> FindRoot();
				//----------------------------------------------------------------------------------
				//[IsTargetMyUpperHierarchy]
				//return: �����Ώۂ����g�̏�ʊK�w�ɂ���ꍇtrue
				//����1: �����Ώ� 
				inline bool IsTargetMyUpperHierarchy(const SharedPointer<TransformX11>& target);
				//----------------------------------------------------------------------------------
				//[IsTargetMyUpperHierarchy]
				//return: �����Ώۂ����g�̏�ʊK�w�ɂ���ꍇtrue
				//����1: �����Ώ� 
				inline bool IsTargetMyUpperHierarchy(const WeakPointer<TransformX11>& target);
				//----------------------------------------------------------------------------------
				//[RegisterParent]
				//�e��o�^����, �|�C���^���L���ȏꍇ�̂ݐ�������
				//reutrn: ���������ꍇ->true
				//����1: parent�|�C���^
				inline bool RegisterParent(SharedPointer<TransformX11> parent);

				//----------------------------------------------------------------------------------
				//[RegisterChildren]
				//�q��o�^����, �|�C���^���L���ȏꍇ�̂ݐ�������
				//reutrn: ���������ꍇ->true
				//����1: children�|�C���^
				inline bool RegisterChildren(SharedPointer<TransformX11> children);

				//----------------------------------------------------------------------------------
				//[FindChildren]
				//�qtransform����������, index = 0���瑖������̂œ���TransformX11�͒���
				//reutrn: ���������ꍇ->�q�|�C���^, ���s�����ꍇ->Null�|�C���^
				//����1: transform->name
				inline WeakPointer<TransformX11> FindChildren(const sgstring& name);
				//----------------------------------------------------------------------------------
				//[FindChildrenEx]
				//�qtransform����������, index = 0���瑖��������TransformX11��S�ĕԋp����
				//reutrn: ��������vector
				//����1: transform->name
				inline std::vector<WeakPointer<TransformX11>> FindChildrenEx(const sgstring& name);
				//----------------------------------------------------------------------------------
				//[FindChildrenIndex]
				//�qtransform����������, index = 0���瑖������̂œ���TransformX11�͒���
				//reutrn: ���������ꍇ->���݂�index, ���s�����ꍇ->VariableLimit::cUintMax
				//����1: transform->name
				inline uint FindChildrenIndex(const sgstring& name);
				//----------------------------------------------------------------------------------
				//[FindChildrenIndexEx]
				//�qtransform����������, index = 0���瑖��������TransformX11��S�ĕԋp����
				//reutrn: ��������vector
				//����1: transform->name
				inline std::vector<uint> FindChildrenIndexEx(const sgstring& name);

				//----------------------------------------------------------------------------------
				//[UnderTakeChildren]
				//�q��ʂ�transform�̎q�Ɉړ�������, �K������X���b�h��Ŏ��s���邱��
				//index���L�� & ����1���L���ȏꍇ�̂ݐ�������
				//reutrn: ���������ꍇ->true
				//����1: �V�����etransfrom
				//����2: �ڂ��qTransformIndex
				inline bool UnderTakeChildren(SharedPointer<TransformX11> transform, uint index);
				//----------------------------------------------------------------------------------
				//[UnderTakeChildren]
				//�q��ʂ�transform�̎q�Ɉړ�������, �K������X���b�h��Ŏ��s���邱��
				//name���L�� & ����1���L���ȏꍇ�̂ݐ�������
				//reutrn: ���������ꍇ->true
				//����1: �V�����etransfrom
				//����2: �ڂ��qTransform->name
				inline bool UnderTakeChildren(SharedPointer<TransformX11> transform, const sgstring& name);
				//----------------------------------------------------------------------------------
				//[UnderTakeChildrenEx]
				//name�̎q��S�ĕʂ�transform�̎q�Ɉړ�������, �K������X���b�h��Ŏ��s���邱��
				//name���L�� & ����1���L���ȏꍇ�̂ݐ�������
				//reutrn: ���������ꍇ->true
				//����1: �V�����etransfrom
				//����2: �ڂ��qTransform->name
				inline bool UnderTakeChildrenEx(SharedPointer<TransformX11> transform, const sgstring& name);

				//----------------------------------------------------------------------------------
				//[IndependentChildren]
				//�q�I�u�W�F�N�g��e�q�֌W����؂藣���Ɨ�������
				//return: ���������ꍇ->�Ɨ������|�C���^, ���s�����ꍇ->Null�|�C���^
				//����1: index
				inline SharedPointer<TransformX11> IndependentChildren(uint index);
				//----------------------------------------------------------------------------------
				//[IndependentChildren]
				//�q�I�u�W�F�N�g��e�q�֌W����؂藣���Ɨ�������
				//index = 0���瑖�����ŏ��Ɍ�������name�ƍ��v����transform��Ɨ�������
				//return: ���������ꍇ->�Ɨ������|�C���^, ���s�����ꍇ->Null�|�C���^
				//����1: name
				inline SharedPointer<TransformX11> IndependentChildren(const sgstring& name);
				//----------------------------------------------------------------------------------
				//[IndependentChildren]
				//�q�I�u�W�F�N�g��e�q�֌W����؂藣���Ɨ�������, name�ƍ��v����transform��S�ēƗ�������
				//return: ���ʂ��i�[����vector, ���������ꍇ->�Ɨ������|�C���^, ���s�����ꍇ->Null�|�C���^
				//����1: index
				inline std::vector<SharedPointer<TransformX11>> IndependentChildrenEx(const sgstring& name);

				//----------------------------------------------------------------------------------
				//[DestroyChildren]
				//�q�I�u�W�F�N�g���폜����, index���L���ȏꍇ�̂ݐ�������
				//return: �����������ۂ�
				//����1: index
				inline bool DestroyChildren(uint index);
				//----------------------------------------------------------------------------------
				//[DestroyChildren]
				//�q�I�u�W�F�N�g���폜����, index = 0���瑖�����ŏ��Ɍ�������name�ƍ��v����transform���폜����
				//return: �����������ۂ�
				//����1: transform->name
				inline bool DestroyChildren(const sgstring& name);
				//----------------------------------------------------------------------------------
				//[DestroyChildren]
				//�q�I�u�W�F�N�g���폜����, name�ƍ��v����transform��S�č폜����
				//return: �����������ۂ�
				//����1: transform->name
				inline bool DestroyChildrenEx(const sgstring& name);
				//----------------------------------------------------------------------------------
				//[DestroyAllChildren]
				//�q�I�u�W�F�N�g��S�č폜����
				inline void DestroyAllChildren();

				//----------------------------------------------------------------------------------
				//[AddComponent]
				//Component��ǉ����� (�f�B�[�v�R�s�[)
				//����1: component�p���I�u�W�F�N�g
				template <class ComponentClass>
				inline WeakPointer<ComponentClass> AddComponent(SharedPointer<ComponentClass> component);
				//----------------------------------------------------------------------------------
				//[AddComponent]
				//Component��ǉ����� (Instantiate)
				//return: �ǉ�����Component
				//����1: Callback�L�����t���O, default = true
				template <class ComponentClass>
				inline WeakPointer<ComponentClass> AddComponent(bool isEnabled = true);


				//----------------------------------------------------------------------------------
				//[FindComponent]
				//return: ComponentClass�^�̃R���|�[�l���g���R�t�����Ă���ꍇ : �ł��Â��R���|�[�l���g
				//				�R�t�����Ă��Ȃ��ꍇ : Null
				template <class ComponentClass>
				inline WeakPointer<ComponentClass> FindComponent();
				//----------------------------------------------------------------------------------
				//[FindComponentInParent]
				//ComponentClass�^�R���|�[�l���g��this����Parent�ւƒT������
				//return: ComponentClass�^�̃R���|�[�l���g���R�t�����Ă���ꍇ : �ŏ��Ɍ��������R���|�[�l���g
				//				�R�t�����Ă��Ȃ��ꍇ : Null
				//����1: ���g���܂߂邩, default = true
				template <class ComponentClass>
				inline WeakPointer<ComponentClass> FindComponentInParent(bool isIncludeYourself = true);
				//----------------------------------------------------------------------------------
				//[FindComponentInChildren]
				//ComponentClass�^�R���|�[�l���g��Children����T������
				//return: ComponentClass�^�̃R���|�[�l���g���R�t�����Ă���ꍇ : �ŏ��Ɍ��������R���|�[�l���g
				//				�R�t�����Ă��Ȃ��ꍇ : Null
				//����1: ���g���܂߂邩, default = true
				template <class ComponentClass>
				inline WeakPointer<ComponentClass> FindComponentInChildren(bool isIncludeYourself = true);
				//----------------------------------------------------------------------------------
				//[FindComponentEx]
				//return: ComponentClass�^�̃R���|�[�l���g���R�t�����Ă���ꍇ : �S�Ă̓��^�R���|�[�l���g
				//				�R�t�����Ă��Ȃ��ꍇ : Null
				template <class ComponentClass>
				inline std::vector<WeakPointer<ComponentClass>> FindComponentEx();
				//----------------------------------------------------------------------------------
				//[FindComponentInParentEx]
				//ComponentClass�^�R���|�[�l���g��this����Parent�ւƒT������
				//return: ComponentClass�^�̃R���|�[�l���g���R�t�����Ă���ꍇ : �S�Ă̓��^�R���|�[�l���g
				//				�R�t�����Ă��Ȃ��ꍇ : Null
				//����1: ���g���܂߂邩, default = true
				template <class ComponentClass>
				inline std::vector<WeakPointer<ComponentClass>> FindComponentInParentEx(bool isIncludeYourself = true);
				//----------------------------------------------------------------------------------
				//[FindComponentInChildrenEx]
				//ComponentClass�^�R���|�[�l���g��Children����T������
				//return: ComponentClass�^�̃R���|�[�l���g���R�t�����Ă���ꍇ : �S�Ă̓��^�R���|�[�l���g
				//				�R�t�����Ă��Ȃ��ꍇ : Null
				//����1: ���g���܂߂邩, default = true
				template <class ComponentClass>
				inline std::vector<WeakPointer<ComponentClass>> FindComponentInChildrenEx(bool isIncludeYourself = true);

				//----------------------------------------------------------------------------------
				//[RemoveComponent]
				//Component���폜���� (template�^�ɑΉ������ł��Â��R���|�[�l���g)
				template <class ComponentClass>
				inline void RemoveComponent();
				//----------------------------------------------------------------------------------
				//[RemoveComponentEx]
				//Component���폜���� (template�^�ɑΉ������S�ẴR���|�[�l���g)
				template <class ComponentClass>
				inline void RemoveComponentEx();
				//----------------------------------------------------------------------------------
				//[RemoveComponentAll]
				//Component��S�č폜����
				inline void RemoveComponentAll();

				//�v���p�e�B�����Ƃ��ē���ŕϐ��̖����K����g���܂�

				//auto select function line
				static constexpr int cAutoSelectFunctionLine = VariableLimit::cIntMin;

				//transform
				WeakPointer<TransformX11> transform;
				//game object (GameObject->Instantiate���琶�����Ȃ������ꍇ��null)
				WeakPointer<GameObject> gameObject;
				//time layer (GameObject->Instantiate���琶�����Ȃ������ꍇ��null)
				WeakPointer<Time::TimeLayer> timeLayer;

				//set world matrix graphics (draw function only property)
				SGF_FUNCTION_PROPERTY void setDrawMatrix();

				//parent transfotm (get function property)
				SGF_FUNCTION_PROPERTY WeakPointer<TransformX11>& getParent() { LockGuardTransform guard(m_lock); return m_parent; }
				//children transfotms (get function property)
				//argument 1: index
				SGF_FUNCTION_PROPERTY WeakPointer<TransformX11> getChildren(uint index)
				{ 
					LockGuardTransform guard(m_lock); 
					//Index���L���Ȃ�ԋp	
					return m_childrens.size() > index ? WeakPointer<TransformX11>(m_childrens[index]) : std::move(WeakPointer<TransformX11>());
				}
				
				//num childrens (read function property)
				SGF_FUNCTION_PROPERTY uint getNumChildrens() { LockGuardTransform guard(m_lock); return static_cast<uint>(m_childrens.size()); }
				
				//world position (read function property)
				SGF_FUNCTION_PROPERTY Vector3 getWorldPosition() { LockGuardTransform guard(m_lock); return m_variables.worldPosition; }
				//world position (set function property)
				//argument 1: set value
				SGF_FUNCTION_PROPERTY Vector3 setWorldPosition(const Vector3& set)
				{
					LockGuardTransform guard(m_lock);
					m_isNotify = true;
					if (IS_FALSE(m_parent.getIsValid()))
					{
						m_variables.worldPosition = m_variables.localPosition = set;
						if (m_childrens.size() > 0)
						{
							XmVector quaternion = m_variables.worldRotation.ToXmVector();
							VariablesUpdate(m_variables.worldPosition, quaternion, m_variables.worldScale);
						}
						return m_variables.worldPosition;
					}
					else
					{
						//Local�ϊ������l����
						m_variables.localPosition = set - m_parent->getWorldPosition();
						//World�ݒ�
						m_variables.worldPosition = set;

						if (m_childrens.size() > 0)
						{
							XmVector quaternion = m_variables.worldRotation.ToXmVector();
							VariablesUpdate(m_variables.worldPosition, quaternion, m_variables.worldScale);
						}
						return m_variables.worldPosition;
					}
				}

				//world rotation (read function property)
				SGF_FUNCTION_PROPERTY Quaternion getWorldRotation() { LockGuardTransform guard(m_lock); return m_variables.worldRotation; }
				//world rotation (set function property)
				//argument 1: set value
				SGF_FUNCTION_PROPERTY Quaternion setWorldRotation(const Quaternion& set)
				{
					LockGuardTransform guard(m_lock);
					m_isNotify = true;
					if (IS_FALSE(m_parent.getIsValid()))
					{
						m_variables.worldRotation = m_variables.localRotation = set;
						if (m_childrens.size() > 0)
						{
							XmVector quaternion = m_variables.worldRotation.ToXmVector();
							VariablesUpdate(m_variables.worldPosition, quaternion, m_variables.worldScale);
						}
						return m_variables.worldRotation;
					}
					else
					{
						//Local�ϊ������l����
						m_variables.localRotation = set * m_parent->getWorldRotation().getInverse();
						//World�ݒ�
						m_variables.worldRotation = set;

						if (m_childrens.size() > 0)
						{
							XmVector quaternion = m_variables.worldRotation.ToXmVector();
							VariablesUpdate(m_variables.worldPosition, quaternion, m_variables.worldScale);
						}
						return m_variables.worldRotation;
					}
				}
				
				//world scale (read function property)
				SGF_FUNCTION_PROPERTY Vector3 getWorldScale() { LockGuardTransform guard(m_lock); return m_variables.worldScale; }
				//world scale (set function property)
				//argument 1: set value
				SGF_FUNCTION_PROPERTY Vector3 setWorldScale(const Vector3& set)
				{
					LockGuardTransform guard(m_lock);
					m_isNotify = true;
					if (IS_FALSE(m_parent.getIsValid()))
					{
						m_variables.worldScale = m_variables.localScale = set;
						if (m_childrens.size() > 0)
						{
							XmVector quaternion = m_variables.worldRotation.ToXmVector();
							VariablesUpdate(m_variables.worldPosition, quaternion, m_variables.worldScale);
						}
						return m_variables.worldScale;
					}
					else
					{
						//Local�ϊ������l����
						m_variables.localScale = set / m_parent->getWorldScale();
						//World�ݒ�
						m_variables.worldScale = set;

						if (m_childrens.size() > 0)
						{
							XmVector quaternion = m_variables.worldRotation.ToXmVector();
							VariablesUpdate(m_variables.worldPosition, quaternion, m_variables.worldScale);
						}
						return m_variables.worldScale;
					}
				}

				//local position (read function property)
				SGF_FUNCTION_PROPERTY Vector3 getLocalPosition() { LockGuardTransform guard(m_lock); return m_variables.localPosition; }
				//local position (set function property)
				//argument 1: set value
				SGF_FUNCTION_PROPERTY Vector3 setLocalPosition(const Vector3& set)
				{
					LockGuardTransform guard(m_lock);
					m_isNotify = true;
					if (IS_FALSE(m_parent.getIsValid()))
					{
						m_variables.worldPosition = m_variables.localPosition = set;

						if (m_childrens.size() > 0)
						{
							XmVector quaternion = m_variables.worldRotation.ToXmVector();
							VariablesUpdate(m_variables.worldPosition, quaternion, m_variables.worldScale);
						}
						return m_variables.localPosition;
					}
					else
					{
						m_variables.localPosition = set;
						m_variables.worldPosition = m_parent->m_variables.worldPosition + m_variables.localPosition;
						if (m_childrens.size() > 0)
						{
							XmVector quaternion = m_variables.worldRotation.ToXmVector();
							VariablesUpdate(m_variables.worldPosition, quaternion, m_variables.worldScale);
						}
						return m_variables.localPosition;
					}
				}

				//local rotation (read function property)
				SGF_FUNCTION_PROPERTY Quaternion getLocalRotation() { LockGuardTransform guard(m_lock); return m_variables.localRotation; }
				//local rotation (set function property)
				//argument 1: set value
				SGF_FUNCTION_PROPERTY Quaternion setLocalRotation(const Quaternion& set)
				{
					LockGuardTransform guard(m_lock);
					
					if (IS_FALSE(m_parent.getIsValid()))
					{					
						m_variables.worldRotation = m_variables.localRotation = set;

						if (m_childrens.size() > 0)
						{
							XmVector quaternion = m_variables.worldRotation.ToXmVector();
							VariablesUpdate(m_variables.worldPosition, quaternion, m_variables.worldScale);
						}
						return m_variables.localRotation;
					}
					else
					{
						m_variables.localRotation = set;
						m_variables.worldRotation = m_parent->m_variables.worldRotation * m_variables.localRotation;
						if (m_childrens.size() > 0)
						{
							XmVector quaternion = m_variables.worldRotation.ToXmVector();
							VariablesUpdate(m_variables.worldPosition, quaternion, m_variables.worldScale);
						}
						return m_variables.localRotation;
					}
				}

				//local scale (read function property)
				SGF_FUNCTION_PROPERTY Vector3 getLocalScale() { LockGuardTransform guard(m_lock); return m_variables.localScale; }
				//local scale (set function property)
				//argument 1: set value
				SGF_FUNCTION_PROPERTY Vector3 setLocalScale(const Vector3& set)
				{
					LockGuardTransform guard(m_lock);
					m_isNotify = true;
					if (IS_FALSE(m_parent.getIsValid()))
					{
						m_variables.worldScale = m_variables.localScale = set;
						if (m_childrens.size() > 0)
						{
							XmVector quaternion = m_variables.worldRotation.ToXmVector();
							VariablesUpdate(m_variables.worldPosition, quaternion, m_variables.worldScale);
						}
						return m_variables.localScale;
					}
					else
					{
						m_variables.localScale = set;
						m_variables.worldScale = m_parent->m_variables.worldScale * m_variables.localScale;
						if (m_childrens.size() > 0)
						{
							XmVector quaternion = m_variables.worldRotation.ToXmVector();
							VariablesUpdate(m_variables.worldPosition, quaternion, m_variables.worldScale);
						}
						return m_variables.localScale;
					}
				}

				//worldRotation -> right (read function property)
				SGF_FUNCTION_PROPERTY Vector3 getRight() { LockGuardTransform guard(m_lock); return m_variables.worldRotation.getRight(); }
				//worldRotation -> up (read function property)
				SGF_FUNCTION_PROPERTY Vector3 getUp() { LockGuardTransform guard(m_lock); return m_variables.worldRotation.getUp(); }
				//worldRotation -> forward (read function property)
				SGF_FUNCTION_PROPERTY Vector3 getForward() { LockGuardTransform guard(m_lock); return m_variables.worldRotation.getForward(); }
				//worldRotation -> forward (set function property)
				//argument 1: set value (change rotation) 
				SGF_FUNCTION_PROPERTY Vector3 setForward(const Vector3& set)
				{
					LockGuardTransform guard(m_lock);
					m_isNotify = true;
					if (IS_FALSE(m_parent.getIsValid()))
					{
						m_variables.worldRotation = m_variables.localRotation = Quaternion::LookForward(set);

						if (m_childrens.size() > 0)
						{
							XmVector quaternion = m_variables.worldRotation.ToXmVector();
							VariablesUpdate(m_variables.worldPosition, quaternion, m_variables.worldScale);
						}
					}
					else
					{
						Quaternion look = Quaternion::LookForward(set);
					
						//Local�ϊ������l����
						m_variables.localRotation = look * m_parent->getWorldRotation().getInverse();
						//World�ݒ�
						m_variables.worldRotation = look;
						if (m_childrens.size() > 0)
						{
							XmVector quaternion = m_variables.worldRotation.ToXmVector();
							VariablesUpdate(m_variables.worldPosition, quaternion, m_variables.worldScale);
						}
					}

					return set;
				}

				//localRotation -> right (read function property)
				SGF_FUNCTION_PROPERTY Vector3 getRightLocal() { LockGuardTransform guard(m_lock); return m_variables.localRotation.getRight(); }
				//localRotation -> up (read function property)
				SGF_FUNCTION_PROPERTY Vector3 getUpLocal() { LockGuardTransform guard(m_lock); return m_variables.localRotation.getUp(); }
				//localRotation -> forward (read function property)
				SGF_FUNCTION_PROPERTY Vector3 getForwardLocal() { LockGuardTransform guard(m_lock); return m_variables.localRotation.getForward(); }
				//localRotation -> forward (set function property)
				//argument 1: set value (change rotation) 
				SGF_FUNCTION_PROPERTY Vector3 setForwardLocal(const Vector3& set)
				{
					LockGuardTransform guard(m_lock);
					m_isNotify = true;
					if (IS_FALSE(m_parent.getIsValid()))
					{
						m_variables.worldRotation = m_variables.localRotation = Quaternion::LookForward(set);
						if (m_childrens.size() > 0)
						{
							XmVector quaternion = m_variables.worldRotation.ToXmVector();
							VariablesUpdate(m_variables.worldPosition, quaternion, m_variables.worldScale);
						}
					}
					else
					{
						m_variables.localRotation = Quaternion::LookForward(set);
						m_variables.worldRotation = m_parent->m_variables.worldRotation * m_variables.localRotation;
						if (m_childrens.size() > 0)
						{
							XmVector quaternion = m_variables.worldRotation.ToXmVector();
							VariablesUpdate(m_variables.worldPosition, quaternion, m_variables.worldScale);
						}
					}

					return set;
				}

				//update matrix (read function property)
				SGF_FUNCTION_PROPERTY const Vector4x4& getUpdateMatrix() { LockGuardTransform guard(m_lock); return m_updateMatrix; }
				//graphics matrix (read function property)
				SGF_FUNCTION_PROPERTY const Vector4x4& getGraphicsMatrix() { LockGuardTransform guard(m_lock); return m_drawMatrix; }


				//layer (read function property)
				SGF_FUNCTION_PROPERTY Layer getLayer() { return m_layer; }
				//tag (read function property)
				SGF_FUNCTION_PROPERTY Tag getTag() { LockGuardTransform guard(m_lock); return m_tag; }
				//tag (set function property)
				//argument 1: set value
				SGF_FUNCTION_PROPERTY Tag setTag(const Tag& tag) { LockGuardTransform guard(m_lock); return m_tag = tag; }
				//function line(get function property)
				SGF_FUNCTION_PROPERTY int getFunctionLine()
				{
					if (IS_FALSE(m_parent.getIsValid()))
						return m_functionLineAtRoot;
					else
					{
						WeakPointer<TransformX11> result;
						for (result = m_parent; result->m_parent.getIsValid(); result = result->m_parent) {}
						return result->m_functionLineAtRoot;
					}
				}

				//this transform is active? (read function property)
				SGF_FUNCTION_PROPERTY bool getIsActiveSelf() { return m_isActive; }
				//set this transform is active (set function property)
				//argument 1: set value (update next frame)
				SGF_FUNCTION_PROPERTY bool setIsActive(bool active) { LockGuardTransform guard(m_lock); return m_isActivePublic = active; }
				//this transform is static? (read function property)
				SGF_FUNCTION_PROPERTY bool getIsStatic() { LockGuardTransform guard(m_lock); return static_cast<bool>(m_flags & m_cIsStaticFlag); }
				//this transform is root? (read function property)
				SGF_FUNCTION_PROPERTY bool getIsRoot() { LockGuardTransform guard(m_lock); return IS_FALSE(m_parent.getIsValid()); }
				//this transform is destroy? (read function property)
				SGF_FUNCTION_PROPERTY bool getIsDestroy() { LockGuardTransform guard(m_lock); return m_flags & m_cIsDestroyFlag ? true : false; }
				
			private:
				//RootList�p
				struct RootList
				{
				public:
					friend class GUI;

					//set num function line property
					SGF_FUNCTION_PROPERTY void setFunctionLines(int numLines) { m_numLines = numLines; m_list.resize(m_numLines, std::unordered_map<uint, TransformX11*>()); }
					//get num function line property
					SGF_FUNCTION_PROPERTY int getFunctionLines() { return m_numLines; }
					//get function line property
					//argument 1: index
					SGF_FUNCTION_PROPERTY std::unordered_map<uint, TransformX11*>& getFunctionLine(int index) { return m_list[index]; }

					//----------------------------------------------------------------------------------
					//[AcquisitionFunctionLineIndex]
					//Index�擾
					inline bool AcquisitionFunctionLineIndex(int& index)
					{
						int oldCounter = 0, newCounter = 0;

						do
						{
							oldCounter = m_counter.load();
							newCounter = oldCounter + 1;
						} while (!m_counter.compare_exchange_weak(oldCounter, newCounter));

						if (newCounter < m_numLines)
						{
							index = newCounter;
							return true;
						}
						else return false;
					}
					//----------------------------------------------------------------------------------
					//[AcquisitionFunctionLineIndexForSingleThread]
					//Index�擾
					inline bool AcquisitionFunctionLineIndexForSingleThread(int& index)
					{
						int result = ++m_counter;
						if (result < m_numLines)
						{
							index = result;
							return true;
						}
						else return false;
					}
					//----------------------------------------------------------------------------------
					//[ResetFunctionLineIndex]
					//Index�J�E���^�[������
					inline void ResetFunctionLineIndex() { m_counter = -1; }
					//----------------------------------------------------------------------------------
					//[Add]
					//Root���X�g�ɒǉ�
					inline void Add(TransformX11* transform)
					{
						AtomicOperation::LockAtomic(m_lock);
						m_list[transform->m_functionLineAtRoot].try_emplace(transform->instanceID(), transform);
						AtomicOperation::UnlockAtomic(m_lock);
					}
					//----------------------------------------------------------------------------------
					//[Erase]
					//Root���X�g����폜
					inline void Erase(TransformX11* transform)
					{
						AtomicOperation::LockAtomic(m_lock);
						m_list[transform->m_functionLineAtRoot].erase(transform->instanceID());
						AtomicOperation::UnlockAtomic(m_lock);
					}
					//----------------------------------------------------------------------------------
					//[Shutdown]
					//�I��
					inline void Shutdown()
					{
						AtomicOperation::LockAtomic(m_lock);
						m_numLines = 0;
						AtomicOperation::UnlockAtomic(m_lock);
					}

				private:
					//map
					std::vector<std::unordered_map<uint, TransformX11*>> m_list;
					//lock
					std::atomic<bool> m_lock = false;
					//�J�E���^
					std::atomic<int> m_counter;
					//line��
					int m_numLines;
				};

				//----------------------------------------------------------------------------------
				//[�R���X�g���N�^]
				//�������������
				inline TransformX11(const sgstring& name, const Vector3& position,
					const Vector3& scale, const Quaternion& rotation, int functionLine, bool isStatic, Layer layer, 
					const Tag& tag, bool isUsedMutexCounter, bool isAutoGabageCollection);

				//----------------------------------------------------------------------------------
				//[StartUp]
				inline static void StartUp(const ReadElement::Pack& lines);
				//----------------------------------------------------------------------------------
				//[Shutdown]
				inline static void Shutdown();
				//----------------------------------------------------------------------------------
				//[AutoFunctionLine]
				//return: auto select function line
				inline static int AutoFunctionLine();
				//----------------------------------------------------------------------------------
				//[StartSyncFrame]
				//Set Lock Flags
				inline static void StartSyncFrame() { AtomicOperation::Init(LockGuardTransform::m_isNowSyncFrame, true); }
				//----------------------------------------------------------------------------------
				//[EndSyncFrame]
				//Set Lock Flags
				inline static void EndSyncFrame() { AtomicOperation::Init(LockGuardTransform::m_isNowSyncFrame, false); }
				//----------------------------------------------------------------------------------
				//[UpdateForSingleThread]
				//BackGroundUpdate (ALL)
				inline static void UpdateForSingleThread(uint callbackType);
				//----------------------------------------------------------------------------------
				//[UpdateForMultiThread1]
				//BackGroundUpdate (ALL)
				inline static void UpdateForMultiThread1();
				//----------------------------------------------------------------------------------
				//[UpdateForMultiThread1]
				//BackGroundUpdate (ALL)
				inline static void UpdateForMultiThread1(uint callbackType);
				//----------------------------------------------------------------------------------
				//[UpdateAndMakeMatrixForMultiThread]
				//BackGroundUpdate (ALL)
				inline static void UpdateAndMakeMatrixForMultiThread(const std::vector<int>& lines, uint callbackType);
				//----------------------------------------------------------------------------------
				//[SyncAndUpdateForMultiThread]
				//BackGroundUpdate (Sync)
				inline static void SyncAndUpdateForMultiThread(const std::vector<int>& lines, uint callbackType);
				//----------------------------------------------------------------------------------
				//[SyncForMultiThread1]
				//BackGroundUpdate (Sync)
				inline static void SyncForMultiThread1(uint callbackType);
				//----------------------------------------------------------------------------------
				//[InvokeCallbackForMultiThread]
				//ALL Callback Invoke
				inline static void InvokeCallbackForMultiThread(const std::vector<int>& lines, uint callbackType);
				//----------------------------------------------------------------------------------
				//[InvokeCallbackForMultiThread1]
				//ALL Callback Invoke
				inline static void InvokeCallbackForMultiThread1(uint callbackType);
				//----------------------------------------------------------------------------------
				//[InvokeCallbackForMultiThread1]
				//ALL Callback Invoke
				inline static void InvokeCallbackForSingleThread(uint callbackType);
				//----------------------------------------------------------------------------------
				//[EndUpdateForMultiThread]
				//BackGroundUpdate (ALL And Sync)
				inline static void EndUpdateForMultiThread();

				//----------------------------------------------------------------------------------
				//[UpdateForRoot]
				//BackGroundUpdate (Parent�����Ȃ�TransformX11��)
				inline void UpdateForRoot();
				//----------------------------------------------------------------------------------
				//[UpdateForChildren]
				//BackGroundUpdate (Parent������TransformX11��)
				inline void UpdateForChildren(Matrix& parentMatrix, Quaternion& rotation,
					Vector3& scale, Vector3& position, bool isActive, bool isStatic, bool isChanged);
				//----------------------------------------------------------------------------------
				//[UpdateForRoot]
				//BackGroundUpdate (Parent�����Ȃ�TransformX11��)
				inline void UpdateForRoot(uint callbackType);
				//----------------------------------------------------------------------------------
				//[UpdateForChildren]
				//BackGroundUpdate (Parent������TransformX11��)
				inline void UpdateForChildren(uint callbackType, Matrix& parentMatrix, Quaternion& rotation, 
					Vector3& scale, Vector3& position, bool isActive, bool isStatic, bool isChanged);
				//----------------------------------------------------------------------------------
				//[SyncForBackGround]
				//BackGround SyncAndUpdate
				inline void SyncAndUpdateForBackGround(uint callbackType);
				//----------------------------------------------------------------------------------
				//[SyncForBackGround1]
				//BackGroundSync
				inline void SyncForBackGround1();
				//----------------------------------------------------------------------------------
				//[InitLocal]
				//LocalSync���s��
				//����1: world->local?
				inline void InitLocal(bool isUseWorldInfo);
				//----------------------------------------------------------------------------------
				//[SyncDrawMatrix]
				//GraphicsUpdate�pMatrix�𓯊�����
				inline void SyncDrawMatrix();
				//----------------------------------------------------------------------------------
				//[AllocateCallback]
				//m_callbackIndexs��ݒ�
				inline void AllocateCallback();
				//----------------------------------------------------------------------------------
				//[InvokeCallback]
				//Invoke Callback in this and childrens
				inline void InvokeCallback(uint callbackType);
				//----------------------------------------------------------------------------------
				//[InvokeCallbackChildrens]
				//Invoke Callback in and childrens
				inline void InvokeCallbackChildrens(uint callbackType);
				//----------------------------------------------------------------------------------
				//[InvokeCallback]
				//Invoke Callback in this and childrens
				inline void InvokeCallback(uint callbackType, const WeakPointer<TransformX11>& pointer0, const WeakPointer<TransformX11>& pointer1);
				//----------------------------------------------------------------------------------
				//[InvokeCallbackChildrens]
				//Invoke Callback in and childrens
				inline void InvokeCallbackChildrens(uint callbackType, const WeakPointer<TransformX11>& pointer0, const WeakPointer<TransformX11>& pointer1);
				//----------------------------------------------------------------------------------
				//[InvokeCallbackPhysics]
				//Invoke Callback in this physics
				inline void InvokeCallbackPhysics(uint callbackType, const ContactInfo& info);
				//----------------------------------------------------------------------------------
				//[VariablesUpdate]
				//World values update in this childrens invoke
				inline void VariablesUpdate(Vector3& position,  XmVector& quaternion, Vector3& scale);
				//----------------------------------------------------------------------------------
				//[VariablesUpdateChildren]
				//World values update in this childrens
				inline void VariablesUpdateChildren(Vector3& position, XmVector& quaternion, Vector3& scale);
				//----------------------------------------------------------------------------------
				//[ParentSettings]
				//CopyInstantiate�p, parent��ݒ肵����
				inline void ParentSettings(WeakPointer<TransformX11> parent);
				
				//flags
				static constexpr uint m_cIsStaticFlag = 0x1;
				static constexpr uint m_cIsDestroyFlag = 0x2;
				static constexpr uint m_cIsUsedMutexCounterFlag = 0x4;
				static constexpr uint m_cIsAutoGabageCollectionFlag = 0x8;

				//root list
				static RootList m_rootList;

				//ParentTransformX11�|�C���^
				WeakPointer<TransformX11> m_parent;
				//ChildrenTransformX11 Venctor
				std::vector<SharedPointer<TransformX11>> m_childrens;
				//Components
				std::vector<SharedPointer<ComponentX11>> m_components;
				//�����y���p�R�[���o�b�Nindex���X�g
				std::unordered_map<uint, std::vector<uint>> m_callbackIndexs;
				
				TransformVariables m_variables;				//public variables
				Vector4x4 m_updateMatrix;					//Update�pMatrix
				Vector4x4 m_drawMatrix;						//Draw�pMatrix
				Vector4x4* m_staticMatrix;						//Static����p->Bake����
				Tag m_tag;											//Tag
				Layer m_layer;										//Layer
				std::atomic<Lock> m_lock;					//lock atomic
				uint m_callbackEnabledFlags;					//callback flags
				int m_functionLineAtRoot;						//Root���ǂ���FunctionLine�ɑ����邩
				byte m_flags;											//flags
				bool m_isActive;										//isActive
				bool m_isActivePublic;							//public isActive
				bool m_isNotify;										//access�p
			};

			//Component�n�N���X��Base�ƂȂ�ComponentX11 class
			class ComponentX11 : public Object
			{
			public:
				friend TransformX11;
				friend GameObject;
				
				//delete
				ComponentX11(const ComponentX11&) = delete;
				ComponentX11(ComponentX11&&) = delete;
				ComponentX11& operator = (const ComponentX11&) = delete;
				ComponentX11& operator = (ComponentX11&&) = delete;

				//Callback���w�肷��t���O���`����CallbackFunctions
				struct CallbackFunctions
				{
					friend ComponentX11;
					friend TransformX11;
					friend GameObject;

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
				private:
					//Private Callback Sync Function Flag
					static constexpr uint m_cSync = 0x80000000;

					//Num Callback All Function Flags
					static constexpr uint m_cNumAllCallbacks = 17;
					//Num Private Callback All Function Flags
					static constexpr uint m_cPrivateCallbacksStartValue = m_cSync;
					//Num Private Callback All Function Flags
					static constexpr uint m_cNumAllPrivateCallbacks = 1;
					//Not Callback Allocate Flags
					static constexpr uint m_cExclusionAllocateFlags = Awake | OnEnable |  Start;
					//Sync Include Sync Callback Flags
					static constexpr uint m_cSyncIncludeFlags = Awake | OnEnable | Start | OnDisable;
				};

				//----------------------------------------------------------------------------------
				//[Instantiate]
				//�R���|�[�l���g���C���X�^���X������
				//����1: copy
				//����2: �o�^����Transform
				//����3: Used mutex counter (Required for Multi thread), default = true
				//����4: Auto gabage collection?, default = true
				template <class ComponentClass>
				inline static SharedPointer<ComponentClass> Instantiate(SharedPointer<ComponentClass> copy, SharedPointer<TransformX11> transform,
					bool isUsedMutexCounter = true, bool isAutoGabageCollection = true);
				//----------------------------------------------------------------------------------
				//[Instantiate]
				//�R���|�[�l���g���C���X�^���X������
				//����1: �o�^����Transform
				//����2: is Enabled, default = true
				//����3: Used mutex counter (Required for Multi thread), default = true
				//����4: Auto gabage collection?, default = true
				template <class ComponentClass>
				inline static SharedPointer<ComponentClass> Instantiate(SharedPointer<TransformX11> transform, bool isEnabled = true,
					bool isUsedMutexCounter = true, bool isAutoGabageCollection = true);

				//----------------------------------------------------------------------------------
				//[EnabledCallbacks]
				//return: ���g��Call�����Callback Flags, Component::CallbackFunctions�Q��
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
				inline virtual void ChangeUpperHierarchy(WeakPointer<TransformX11> newParent, WeakPointer<TransformX11> oldParent) {}


				//----------------------------------------------------------------------------------
				//[ChangeCallbackFlag]
				//CallbackEnabledFlags�̒l��ύX����, Awake�Ȃǂ͍ēx�Ăяo���悤�ɐݒ肷��Ƒ����Ă΂��
				//Copy Instantiate���s����Copy��͏�����ԂƂȂ�
				//����1: flag
				//����2: set value
				inline void ChangeCallbackFlags(uint flags, bool setValue);

				//transform
				WeakPointer<TransformX11> transform;
				//game object (Transform��GameObject->Instantiate���琶�����Ȃ������ꍇ��null)
				WeakPointer<GameObject> gameObject;
				//time layer (Transform��GameObject->Instantiate���琶�����Ȃ������ꍇ��null)
				WeakPointer<Time::TimeLayer> timeLayer;
				
				//transform->isActiveSelf & this isEnabled (read function property)
				SGF_FUNCTION_PROPERTY bool getIsActiveAndEnabled() { TransformX11::LockGuardTransform guard(transform->m_lock); return transform.getIsValid() & transform->getIsActiveSelf() & m_isEnabled; }
				//component is enabled? (read function property, not lokced)
				SGF_FUNCTION_PROPERTY bool getIsEnabled() { TransformX11::LockGuardTransform guard(transform->m_lock); return m_isEnabled; }
				//component is enabled? (set function property, not lokced)
				//argument 1: set value
				SGF_FUNCTION_PROPERTY bool setIsEnabled(bool set) { TransformX11::LockGuardTransform guard(transform->m_lock); return m_isEnabled = set; }

			protected:
				//----------------------------------------------------------------------------------
				//[�R���X�g���N�^]
				inline ComponentX11() : Object(""), transform(), m_isEnabled(false), 
					m_callbackEnabledFlags(0), m_saveCallbackEnabledFlags(0), m_isOldEnabled(false) {}
				
				//this pointer
				WeakPointer<ComponentX11> m_thisPointer;
				//is enabled
				bool m_isEnabled;

			private:
				//----------------------------------------------------------------------------------
				//[AllocateCallback]
				//callback table���쐬����
				inline static void AllocateCallback();
				//----------------------------------------------------------------------------------
				//[RefreshCallbackFlags]
				//Callback Flags��������Ԃɂ���
				inline void RefreshCallbackFlags();
				//----------------------------------------------------------------------------------
				//[BackGroundSync]
				//Background Sync
				inline void BackGroundSync();
				//----------------------------------------------------------------------------------
				//[InvokeCallback]
				//Callback���Ăяo��
				inline void InvokeCallback(uint callbackType);
				//----------------------------------------------------------------------------------
				//[InvokeCallbackPhysics]
				//Callback���Ăяo�� (Physics)
				inline void InvokeCallbackPhysics(uint callbackType, const ContactInfo& info);
				//----------------------------------------------------------------------------------
				//[InvokeCallbackPointer]
				//Callback���Ăяo�� (Pointer)
				inline void InvokeCallbackPointer(uint callbackType, const WeakPointer<TransformX11>& pointer0, const WeakPointer<TransformX11>& pointer1);

				//Callback Table
				static std::unordered_map<uint, void (ComponentX11::*)()> m_functionTable;
				//Callback Table (Physics)
				static std::unordered_map<uint, void (ComponentX11::*)(const ContactInfo&)> m_physicsFunctionTable;
				//Callback Table (Pointer)
				static std::unordered_map<uint, void (ComponentX11::*)(WeakPointer<TransformX11>, WeakPointer<TransformX11>)> m_pointerFunctionTable;

				uint m_callbackEnabledFlags;			//Now Flags
				uint m_saveCallbackEnabledFlags;	//Instantiate Flags
				bool m_isOldEnabled;						//old enabled
				bool m_isOldActive;						//old Active
			};

			//----------------------------------------------------------------------------------
			//[Instantiate]
			//Transform�����̉�����, �K���V���O���X���b�h��Ԃ̎��Ɏ��̉����邱��
			//return: transform
			//����1: position, default = zero
			//����2: scale, default = one
			//����3: rotation, default = identity
			//����4: name, default = "Transform only object"
			//����5: use function line if root transform, default = auto
			//����6: isStatic, default = false
			//����7: layer, default = Default Layer
			//����8: tag, default = Default Tag
			//����9: SharedPointer->isUsedMutexCounter, default = ture
			//����10: SharedPointer->isAutoGabageCollection, default = ture
			inline SharedPointer<TransformX11> TransformX11::Instantiate(const Vector3 & position, const Vector3 & scale,
				const Quaternion & rotation, const sgstring & name, int functionLineAtRoot, bool isStatic, Layer layer, const Tag & tag,
				bool isUsedMutexCounter, bool isAutoGabageCollection)
			{
				SharedPointer<TransformX11> result(new TransformX11(name, position, scale,
					rotation, functionLineAtRoot != cAutoSelectFunctionLine ? functionLineAtRoot : AutoFunctionLine(), isStatic, layer, 
					tag, isUsedMutexCounter, isAutoGabageCollection),
					isUsedMutexCounter, isAutoGabageCollection);
				result->transform = result;
				m_rootList.Add(result.getPointer());
				return std::move(result);
			}
			//----------------------------------------------------------------------------------
			//[Instantiate]
			//Transform�����̉�����, �K���V���O���X���b�h��Ԃ̎��Ɏ��̉����邱��
			//position = zero, scale = one, rotation = identity
			//return: transform, parent�������ȃ|�C���^�������ꍇNull�|�C���^
			//����1: �eTransform
			//����2: position, rotation, scale -> true = world : false = local 
			//����3: position, default = zero
			//����4: scale, default = one
			//����5: rotation, default = identity
			//����6: name, default = "Transform only object"
			//����7: use function line if root transform, default = auto
			//����8: isStatic, default = false
			//����9: layer, default = Default Layer
			//����10: tag, default = Default Tag
			//����11: SharedPointer->isUsedMutexCounter, default = ture
			//����12: SharedPointer->isAutoGabageCollection, default = ture
			inline SharedPointer<TransformX11> TransformX11::Instantiate(SharedPointer<TransformX11> parent,
			bool isInstantiateInWorldSpace, const Vector3 & position, const Vector3 & scale, const Quaternion & rotation,
				const sgstring & name, int functionLineAtRoot, bool isStatic, Layer layer, const Tag & tag,
				bool isUsedMutexCounter, bool isAutoGabageCollection)
			{
				if (parent.getIsValid())
				{
					SharedPointer<TransformX11> result(new TransformX11(name, position, scale, rotation,
						functionLineAtRoot != cAutoSelectFunctionLine ? functionLineAtRoot : AutoFunctionLine(), isStatic, layer, 
						tag, isUsedMutexCounter, isAutoGabageCollection),
						isUsedMutexCounter, isAutoGabageCollection);
					result->transform = result;
					result->m_parent = parent;

					//�d��������
					bool isDuplication = false;
					do
					{
						isDuplication = false;
						for (auto& e : parent->m_childrens)
						{
							if (e->name() == result->name())
							{
								isDuplication = true;
								break;
							}
						}
						if (IS_TRUE(isDuplication))
							result->setName(Object::CreateCopyName(name));
					} while (IS_TRUE(isDuplication));

					parent->m_childrens.emplace_back(result);

					result->InitLocal(isInstantiateInWorldSpace);
					return result;
				}
				else
					return SharedPointer<TransformX11>();
			}

			//----------------------------------------------------------------------------------
			//[�f�X�g���N�^]
			//�q�I�u�W�F�N�g�̊J��
			inline TransformX11::~TransformX11()
			{
				LockGuardTransform guard(m_lock);
				m_flags |= m_cIsDestroyFlag;

				//�e������ꍇ
				if (m_parent.LockShared())
				{
					//�e���X�g���玩�����������郋�[�v
					for (auto it = m_parent->m_childrens.begin(); it != m_parent->m_childrens.end();)
					{
						if (IS_TRUE(m_parent.LockShared()))
						{
							//����������erase & break
							if ((*it)->instanceID() == this->instanceID())
							{
								m_parent->m_childrens.erase(it);
								break;
							}
							else
								++it;
						}
						else
							break;
					}

					//�|�C���^���b�N����
					m_parent.UnlockShared();
				}
				//�e�����Ȃ�
				else
				{
					//TopParentList���玩�g���폜
					m_rootList.Erase(this);
				}

				//�q���폜
				DestroyAllChildren();

				//Remove

				//Callback ���[�v
				for (auto& e : m_components)
				{
					//Callback
					if (e->m_isEnabled && (e->m_callbackEnabledFlags & ComponentX11::CallbackFunctions::OnDisable))
						e->OnDisable();
					if (e->m_isEnabled && (e->m_callbackEnabledFlags & ComponentX11::CallbackFunctions::OnDestroy))
						e->OnDestroy();
				}
				//Clear
				m_components.clear();
			}

			//set world matrix property (graphics functino only)
			SGF_FUNCTION_PROPERTY void TransformX11::setDrawMatrix()
			{
				using GraphicsX11 = SGFramework::Administrator::GraphicsX11;

				//�R���X�^���g�o�b�t�@�X�V
				GraphicsX11::m_setConstantBufferObject.worldMatrix = m_drawMatrix.getTranspose();
				GraphicsX11::m_setConstantBufferObject.worldInverseMatrix = m_drawMatrix.getInverse();
				GraphicsX11::context->UpdateSubresource(GraphicsX11::m_constantBufferObject, 0, nullptr, &GraphicsX11::m_setConstantBufferObject, 0, 0);

				//�R���X�^���g�o�b�t�@�ݒ�
				GraphicsX11::context->VSSetConstantBuffers(GraphicsX11::m_cConstantBufferObjectSlot, 1, &GraphicsX11::m_constantBufferObject);
				GraphicsX11::context->PSSetConstantBuffers(GraphicsX11::m_cConstantBufferObjectSlot, 1, &GraphicsX11::m_constantBufferObject);
			}

			//----------------------------------------------------------------------------------
			//[FindRoot]
			//reutrn: root�|�C���^, ���Ȃ��ꍇ��Null�|�C���^ (����!���s���ɒT�����s���܂�)
			inline WeakPointer<TransformX11> TransformX11::FindRoot()
			{
				LockGuardTransform guard(m_lock);

				if (IS_TRUE(m_parent.getIsValid()))
				{
					WeakPointer<TransformX11> result;
					
					for (result = m_parent; result->m_parent.getIsValid(); result = result->m_parent) {}
					
					return std::move(result);
				}
				else
					return WeakPointer<TransformX11>();
			}
			//----------------------------------------------------------------------------------
			//[IsTargetMyUpperHierarchy]
			//return: �����Ώۂ����g�̏�ʊK�w�ɂ���ꍇtrue
			//����1: �����Ώ� 
			inline bool TransformX11::IsTargetMyUpperHierarchy(const SharedPointer<TransformX11>& target)
			{
				if (IS_FALSE(target.getIsValid())) return false;

				LockGuardTransform guard(m_lock);

				if (IS_TRUE(m_parent.getIsValid()))
				{					
					for (auto parent = m_parent; parent->m_parent.getIsValid(); parent = parent->m_parent)
						if (parent->instanceID == target->instanceID) return true;

					return false;
				}
				else
					return false;
			}
			//----------------------------------------------------------------------------------
			//[IsTargetMyUpperHierarchy]
			//return: �����Ώۂ����g�̏�ʊK�w�ɂ���ꍇtrue
			//����1: �����Ώ� 
			inline bool TransformX11::IsTargetMyUpperHierarchy(const WeakPointer<TransformX11>& target)
			{
				if (IS_FALSE(target.getIsValid())) return false;

				LockGuardTransform guard(m_lock);

				if (IS_TRUE(m_parent.getIsValid()))
				{
					for (auto parent = m_parent; parent->m_parent.getIsValid(); parent = parent->m_parent)
						if (parent->instanceID == target->instanceID) return true;

					return false;
				}
				else
					return false;
			}
			//----------------------------------------------------------------------------------
			//[RegisterParent]
			//�e��o�^����, ���g�ɐe�����Ȃ� & �|�C���^���L���ȏꍇ�̂ݐ�������
			//�e��ς���ꍇ�͐e�N���X����UnderTake�����s���邱��
			//reutrn: ���������ꍇ->true
			//����1: parent�|�C���^
			inline bool TransformX11::RegisterParent(SharedPointer<TransformX11> parent)
			{
				//�|�C���^����
				if (IS_TRUE(parent.getIsValid())) return false;

				LockGuardTransform guard(m_lock);

				//�e������
				if (IS_TRUE(m_parent.LockShared()))
				{
					//�e���X�g���玩�����������郋�[�v
					for (auto it = m_parent->m_childrens.begin(); it != m_childrens.end();)
					{
						//����������erase & break
						if ((*it)->instanceID() == this->instanceID())
						{
							m_parent->m_childrens.erase(it);
							break;
						}
					}
					m_parent.UnlockShared();
				}
				//���Ȃ�
				m_rootList.Erase(this);

				auto oldParent = m_parent;
				
				//�e�ɓo�^, �e�̎q�����X�g�Ɏ�����o�^
				m_parent = parent;
				m_parent->m_childrens.push_back(transform.InstantiateShared());

				InvokeCallback(ComponentX11::CallbackFunctions::ChangeUpperHierarchy, oldParent, m_parent);

				//Sync
				InitLocal(true);
					
				return false;
			}

			//----------------------------------------------------------------------------------
			//[RegisterChildren]
			//�q��o�^����, �|�C���^���L�� & �|�C���^�ɐe�����Ȃ��ꍇ�̂ݐ�������
			//�e��ς���ꍇ�͐e�N���X����UnderTake�����s���邱��
			//reutrn: ���������ꍇ->true
			//����1: children�|�C���^
			inline bool TransformX11::RegisterChildren(SharedPointer<TransformX11> children)
			{
				//�q��������
				if (IS_FALSE(children.getIsValid())) return false;

				LockGuardTransform guard(m_lock);

				//�e������
				if (IS_TRUE(children->m_parent.LockShared()))
				{

					//�e���X�g���玩�����������郋�[�v
					for (auto it = children->m_parent->m_childrens.begin(); it != m_childrens.end();)
					{
						//����������erase & break
						if ((*it)->instanceID() == this->instanceID())
						{
							children->m_parent->m_childrens.erase(it);
							break;
						}
					}
					children->m_parent.UnlockShared();
				}
				

				//�ǉ�
				m_childrens.push_back(children);
				//this�o�^
				children->m_parent = transform;
				//Init
				children->InitLocal(true);

				return true;
			}

			//----------------------------------------------------------------------------------
			//[FindChildren]
			//�qtransform����������, index = 0���瑖������̂œ���TransformX11�͒���
			//reutrn: ���������ꍇ->�q�|�C���^, ���s�����ꍇ->Null�|�C���^
			//����1: transform->name
			inline WeakPointer<TransformX11> TransformX11::FindChildren(const sgstring& name)
			{
				LockGuardTransform guard(m_lock);

				//Name�̃I�u�W�F�N�g�����݂���Εԋp
				for (auto& e : m_childrens)
					if (e->name() == name) return e;

				//Error
				return WeakPointer<TransformX11>();
			}
			//----------------------------------------------------------------------------------
			//[FindChildrenEx]
			//�qtransform����������, index = 0���瑖��������TransformX11��S�ĕԋp����
			//reutrn: ��������vector
			//����1: transform->name
			inline std::vector<WeakPointer<TransformX11>> TransformX11::FindChildrenEx(const sgstring & name)
			{
				LockGuardTransform guard(m_lock);

				//result
				std::vector<WeakPointer<TransformX11>> result;

				//�I�u�W�F�N�g�����݂���Βǉ�
				for (auto& e : m_childrens)
					if (e->name() == name) result.emplace_back(e);

				//return move
				return std::move(result);
			}
			//----------------------------------------------------------------------------------
			//[FindChildrenIndex]
			//�qtransform����������, index = 0���瑖������̂œ���TransformX11�͒���
			//reutrn: ���������ꍇ->���݂�index, ���s�����ꍇ->VariableLimit::cUintMax
			//����1: transform->name
			inline uint TransformX11::FindChildrenIndex(const sgstring& name)
			{
				LockGuardTransform guard(m_lock);

				//Name�̃I�u�W�F�N�g�����݂���Εԋp
				for (uint i = 0; i < static_cast<uint>(m_childrens.size()); i++)
					if (m_childrens[i]->name() == name) return i;

				//Error
				return VariableLimit::cUintMax;
			}
			//----------------------------------------------------------------------------------
			//[FindChildrenIndexEx]
			//�qtransform����������, index = 0���瑖��������TransformX11��S�ĕԋp����
			//reutrn: ��������vector
			//����1: transform->name
			inline std::vector<uint> TransformX11::FindChildrenIndexEx(const sgstring & name)
			{
				LockGuardTransform guard(m_lock);

				//result
				std::vector<uint> result;

				//�I�u�W�F�N�g�����݂���Βǉ�
				for (uint i = 0, max = static_cast<uint>(m_childrens.size()); i < max; ++i)
					if (m_childrens[i]->name() == name) result.emplace_back(i);

				//return move
				return std::move(result);
			}

			//----------------------------------------------------------------------------------
			//[UnderTakeChildren]
			//�q��ʂ�transform�̎q�Ɉړ�������, �K������X���b�h��Ŏ��s���邱��
			//index���L�� & ����1���L���ȏꍇ�̂ݐ�������
			//reutrn: ���������ꍇ->true
			//����1: �V�����etransfrom
			//����2: �ڂ��qTransformIndex
			inline bool TransformX11::UnderTakeChildren(SharedPointer<TransformX11> transform, uint index)
			{
				LockGuardTransform guard(m_lock);

				//index�L�� & transform�L��
				if (m_childrens.size() > index && IS_TRUE(transform.getIsValid()))
				{
					//�ۑ�
					auto children = m_childrens[index];

					//�q���̐e��ύX
					m_childrens[index]->m_parent = transform;
					//Init
					m_childrens[index]->InitLocal(true);
					
					//transform�̎q���Ɉڂ�
					transform->m_childrens.push_back(m_childrens[index]);				
					//�����̎q�����X�g����폜
					m_childrens.erase(m_childrens.begin() + index);
					
					//callback
					children->InvokeCallback(ComponentX11::CallbackFunctions::ChangeUpperHierarchy, children->m_parent, this->transform);
					return true;
				}
				//����->false
				else
					return false;
			}
			//----------------------------------------------------------------------------------
			//[UnderTakeChildren]
			//�q��ʂ�transform�̎q�Ɉړ�������, �K������X���b�h��Ŏ��s���邱��
			//name���L�� & ����1���L���ȏꍇ�̂ݐ�������
			//reutrn: ���������ꍇ->true
			//����1: �V�����etransfrom
			//����2: �ڂ��qTransform->name
			inline bool TransformX11::UnderTakeChildren(SharedPointer<TransformX11> transform, const sgstring& name)
			{				
				return UnderTakeChildren(transform, FindChildrenIndex(name));
			}
			//----------------------------------------------------------------------------------
			//[UnderTakeChildrenEx]
			//name�̎q��S�ĕʂ�transform�̎q�Ɉړ�������, �K������X���b�h��Ŏ��s���邱��
			//name���L�� & ����1���L���ȏꍇ�̂ݐ�������
			//reutrn: ���������ꍇ->true
			//����1: �V�����etransfrom
			//����2: �ڂ��qTransform->name
			inline bool TransformX11::UnderTakeChildrenEx(SharedPointer<TransformX11> transform, const sgstring& name)
			{
				LockGuardTransform guard(m_lock);

				//transform�L��
				if (IS_TRUE(transform.getIsValid()))
				{
					bool isResult = false;

					while (true)
					{
						uint index = FindChildrenIndex(name);
						if (index == VariableLimit::cUintMax) break;

						isResult = true;

						//�ۑ�
						auto children = m_childrens[index];
						//�q���̐e��ύX
						m_childrens[index]->m_parent = transform;
						//Init
						m_childrens[index]->InitLocal(true);

						//transform�̎q���Ɉڂ�
						transform->m_childrens.push_back(m_childrens[index]);
						//�����̎q�����X�g����폜
						m_childrens.erase(m_childrens.begin() + index);
						//callback
						children->InvokeCallback(ComponentX11::CallbackFunctions::ChangeUpperHierarchy, children->m_parent, this->transform);
					}

					return isResult;
				}
				//����->false
				else
					return false;
			}

			//----------------------------------------------------------------------------------
			//[IndependentChildren]
			//�q�I�u�W�F�N�g��e�q�֌W����؂藣���Ɨ�������
			//return: ���������ꍇ->�Ɨ������|�C���^, ���s�����ꍇ->Null�|�C���^
			//����1: index
			inline SharedPointer<TransformX11> TransformX11::IndependentChildren(uint index)
			{
				LockGuardTransform guard(m_lock);

				//index�L��
				if (m_childrens.size() > index)
				{
					//world����ݒ�
					m_childrens[index]->m_variables.localRotation = m_childrens[index]->m_variables.worldRotation;
					m_childrens[index]->m_variables.localPosition = m_childrens[index]->m_variables.worldPosition;
					m_childrens[index]->m_variables.localScale = m_childrens[index]->m_variables.worldScale;

					//result�擾
					auto result = std::move(m_childrens[index]);
					//parent������
					result->m_parent.Reset();
					//Root�ǉ�
					m_rootList.Add(result.getPointer());
					//���X�g�폜
					m_childrens.erase(m_childrens.begin() + index);
					
					//callback
					result->InvokeCallback(ComponentX11::CallbackFunctions::ChangeUpperHierarchy, result->m_parent, this->transform);

					//return move
					return std::move(result);
				}
				//index���� -> Null
				else
					return SharedPointer<TransformX11>();
			}
			//----------------------------------------------------------------------------------
			//[IndependentChildren]
			//�q�I�u�W�F�N�g��e�q�֌W����؂藣���Ɨ�������
			//index = 0���瑖�����ŏ��Ɍ�������name�ƍ��v����transform��Ɨ�������
			//return: ���������ꍇ->�Ɨ������|�C���^, ���s�����ꍇ->Null�|�C���^
			//����1: name
			inline SharedPointer<TransformX11> TransformX11::IndependentChildren(const sgstring& name)
			{
				//Index�������Ď��s
				return std::move(IndependentChildren(FindChildren(name)));
			}
			//----------------------------------------------------------------------------------
			//[IndependentChildren]
			//�q�I�u�W�F�N�g��e�q�֌W����؂藣���Ɨ�������, name�ƍ��v����transform��S�ēƗ�������
			//return: ���ʂ��i�[����vector, ���������ꍇ->�Ɨ������|�C���^, ���s�����ꍇ->Null�|�C���^
			//����1: index
			inline std::vector<SharedPointer<TransformX11>> TransformX11::IndependentChildrenEx(const sgstring& name)
			{
				LockGuardTransform guard(m_lock);

				//result
				std::vector<SharedPointer<TransformX11>> result;
			
				while (true)
				{
					uint find = FindChildrenIndex(name);
					if (find == VariableLimit::cUintMax) break;

					result.emplace_back(IndependentChildren(find));
				}

				return result;
			}

			//----------------------------------------------------------------------------------
			//[DestroyChildren]
			//�q�I�u�W�F�N�g���폜����, index���L���ȏꍇ�̂ݐ�������
			//return: �����������ۂ�
			//����1: index
			inline bool TransformX11::DestroyChildren(uint index)
			{
				LockGuardTransform guard(m_lock);

				//index�L��
				if (m_childrens.size() > index)
				{
					//erase����return
					m_childrens.erase(m_childrens.begin() + index);
					return true;
				}
				//index����
				else
					return false;
			}
			//----------------------------------------------------------------------------------
			//[DestroyChildren]
			//�q�I�u�W�F�N�g���폜����, index = 0���瑖�����ŏ��Ɍ�������name�ƍ��v����transform���폜����
			//return: �����������ۂ�
			//����1: transform->name
			inline bool TransformX11::DestroyChildren(const sgstring & name)
			{
				LockGuardTransform guard(m_lock);

				//Index�������Ď��s
				return DestroyChildren(FindChildren(name));
			}
			//----------------------------------------------------------------------------------
			//[DestroyChildren]
			//�q�I�u�W�F�N�g���폜����, name�ƍ��v����transform��S�č폜����
			//return: �����������ۂ�
			//����1: transform->name
			inline bool TransformX11::DestroyChildrenEx(const sgstring & name)
			{
				LockGuardTransform guard(m_lock);

				bool result = false;

				while (true)
				{
					uint find = FindChildrenIndex(name);
					if (find == VariableLimit::cUintMax) break;

					result = true;
					DestroyChildren(FindChildren(name));
				}

				return result;
			}
			//----------------------------------------------------------------------------------
			//[DestroyAllChildren]
			//�q�I�u�W�F�N�g���폜����
			inline void TransformX11::DestroyAllChildren()
			{
				LockGuardTransform guard(m_lock);

				//�q��������Ȃ�폜
				if (IS_FALSE(m_childrens.empty()))
					m_childrens.clear();
			}

			//----------------------------------------------------------------------------------
			//[AddComponent]
			//Component��ǉ����� (�f�B�[�v�R�s�[)
			//����1: component�p���I�u�W�F�N�g
			template <class ComponentClass>
			inline WeakPointer<ComponentClass> TransformX11::AddComponent(SharedPointer<ComponentClass> component)
			{
				static_assert(std::is_base_of_v<ComponentX11, ComponentClass>,
					"TransformX11::AddComponent templatre -> Component derived class only!");

				LockGuardTransform guard(m_lock);

				//�ǉ�
				auto component = std::move(ComponentX11::Instantiate<ComponentClass>(
					component, transform.InstantiateShared(), m_flags & m_cIsUsedMutexCounterFlag ? true : false,
					m_flags & m_cIsAutoGabageCollectionFlag ? true : false).DownCast<ComponentX11>());
				m_components.emplace_back(component.DownCast<ComponentX11>());
				//��Allocate
				this->AllocateCallback();

				return m_components.back();
			}
			//----------------------------------------------------------------------------------
			//[AddComponent]
			//Component��ǉ����� (Instantiate)
			//return: �ǉ�����Component
			//����1: Callback�L�����t���O, default = true
			template <class ComponentClass>
			inline WeakPointer<ComponentClass> TransformX11::AddComponent(bool isEnabled)
			{
				static_assert(std::is_base_of_v<ComponentX11, ComponentClass>,
					"TransformX11::AddComponent templatre -> Component derived class only!");

				LockGuardTransform guard(m_lock);

				//�ǉ�
				auto component = std::move(ComponentX11::Instantiate<ComponentClass>(std::move(transform.InstantiateShared()), isEnabled));
				m_components.emplace_back(component.DownCast<ComponentX11>());
				//��Allocate
				this->AllocateCallback();
				
				return component;
			}

			//----------------------------------------------------------------------------------
			//[RemoveComponent]
			//Component���폜���� (template�^�ɑΉ������ł��Â��R���|�[�l���g)
			template <class ComponentClass>
			inline void TransformX11::RemoveComponent()
			{
				static_assert(std::is_base_of_v<ComponentX11, ComponentClass>,
					"TransformX11::RemoveComponentEx templatre -> Component derived class only!");

				LockGuardTransform guard(m_lock);

				//�������[�v
				for (auto it = m_components.begin(); it != m_components.end(); ++it)
				{				
					//Hit
					if (dynamic_cast<ComponentClass*>(it->getPointer()) != nullptr)
					{
						//Callback
						if ((*it)->m_isEnabled && (*it)->m_callbackEnabledFlags & ComponentX11::CallbackFunctions::OnDisable)
							(*it)->OnDisable();
						if ((*it)->m_callbackEnabledFlags & ComponentX11::CallbackFunctions::OnDestroy)
							(*it)->OnDestroy();
						
						//Erase
						m_components.erase(it);
						//��Allocate
						this->AllocateCallback();
						
						return;
					}
				}
			}
			//----------------------------------------------------------------------------------
			//[FindComponent]
			//return: ComponentClass�^�̃R���|�[�l���g���R�t�����Ă���ꍇ : �ł��Â��R���|�[�l���g
			//				�R�t�����Ă��Ȃ��ꍇ : Null
			template <class ComponentClass>
			inline WeakPointer<ComponentClass> TransformX11::FindComponent()
			{
				static_assert(std::is_base_of_v<ComponentX11, ComponentClass>,
					"TransformX11::FindComponent templatre -> Component derived class only!");

				LockGuardTransform guard(m_lock);

				//�������[�v
				for (auto& e : m_components)
				{
					//Cast
					auto cast = e.DynamicCast<ComponentClass>();
					//Hit
					if (cast.getIsValid()) return cast;
				}
				return SharedPointer<ComponentClass>();
			}
			//----------------------------------------------------------------------------------
			//[FindComponentInParent]
			//ComponentClass�^�R���|�[�l���g��this����Parent�ւƒT������
			//return: ComponentClass�^�̃R���|�[�l���g���R�t�����Ă���ꍇ : �ŏ��Ɍ��������R���|�[�l���g
			//				�R�t�����Ă��Ȃ��ꍇ : Null
			//����1: ���g���܂߂邩, default = true
			template <class ComponentClass>
			inline WeakPointer<ComponentClass> TransformX11::FindComponentInParent(bool isIncludeYourself)
			{
				static_assert(std::is_base_of_v<ComponentX11, ComponentClass>,
					"TransformX11::FindComponentInParent templatre -> Component derived class only!");

				LockGuardTransform guard(m_lock);

				//���g���܂߂�
				if (IS_TRUE(isIncludeYourself))
				{		
					//�������[�v
					for (auto& e : m_components)
					{
						//Cast
						auto cast = e.DynamicCast<ComponentClass>();
						//Hit
						if (cast.getIsValid()) return cast;
					}
				}
				//�Č����H
				return IS_TRUE(m_parent.getIsValid()) ? m_parent->FindComponentInParent<ComponentClass>() : WeakPointer<ComponentClass>();
			}
			//----------------------------------------------------------------------------------
			//[FindComponentInChildren]
			//ComponentClass�^�R���|�[�l���g��Children����T������
			//return: ComponentClass�^�̃R���|�[�l���g���R�t�����Ă���ꍇ : �ŏ��Ɍ��������R���|�[�l���g
			//				�R�t�����Ă��Ȃ��ꍇ : Null
			//����1: ���g���܂߂邩, default = true
			template <class ComponentClass>
			inline WeakPointer<ComponentClass> TransformX11::FindComponentInChildren(bool isIncludeYourself)
			{
				static_assert(std::is_base_of_v<ComponentX11, ComponentClass>,
					"TransformX11::FindComponentInChildren templatre -> Component derived class only!");

				LockGuardTransform guard(m_lock);

				//���g���܂߂�
				if (IS_TRUE(isIncludeYourself))
				{
					//Children�������[�v
					for (auto& children : m_childrens)
					{
						//�������[�v
						for (auto& e : children->m_components)
						{
							//Cast
							auto cast = e.DynamicCast<ComponentClass>();
							//Hit
							if (cast.getIsValid()) return cast;
						}
					}
				}

				//���������[�v
				for (auto& children : m_childrens)
				{
					auto childrenResult = children->FindComponentInChildren<ComponentClass>();
					if (childrenResult.getIsValid()) return childrenResult;
				}
				
				//������Ȃ�������
				return WeakPointer<ComponentClass>();
			}
			//----------------------------------------------------------------------------------
			//[FindComponentEx]
			//return: ComponentClass�^�̃R���|�[�l���g���R�t�����Ă���ꍇ : �S�Ă̓��^�R���|�[�l���g
			//				�R�t�����Ă��Ȃ��ꍇ : Null
			template <class ComponentClass>
			inline std::vector<WeakPointer<ComponentClass>> TransformX11::FindComponentEx()
			{
				static_assert(std::is_base_of_v<ComponentX11, ComponentClass>,
					"TransformX11::FindComponentEx templatre -> Component derived class only!");

				LockGuardTransform guard(m_lock);

				std::vector<WeakPointer<ComponentClass>> result;

				//�������[�v
				for (auto& e : m_components)
				{
					//Cast
					auto cast = e.DynamicCast<ComponentClass>();
					//Hit
					if (cast.getIsValid()) result.emplase_back(cast);
				}

				return std::move(result);
			}
			//----------------------------------------------------------------------------------
			//[FindComponentInParentEx]
			//ComponentClass�^�R���|�[�l���g��this����Parent�ւƒT������
			//return: ComponentClass�^�̃R���|�[�l���g���R�t�����Ă���ꍇ : �S�Ă̓��^�R���|�[�l���g
			//				�R�t�����Ă��Ȃ��ꍇ : Null
			//����1: ���g���܂߂邩, default = true
			template <class ComponentClass>
			inline std::vector<WeakPointer<ComponentClass>> TransformX11::FindComponentInParentEx(bool isIncludeYourself)
			{
				static_assert(std::is_base_of_v<ComponentX11, ComponentClass>,
					"TransformX11::FindComponentInParentEx templatre -> Component derived class only!");

				LockGuardTransform guard(m_lock);

				std::vector<WeakPointer<ComponentClass>> result;

				//���g���܂߂�
				if (IS_TRUE(isIncludeYourself))
				{
					//�������[�v
					for (auto& e : m_components)
					{
						//Cast
						auto cast = e.DynamicCast<ComponentClass>();
						//Hit
						if (cast.getIsValid()) result.emplase_back(cast);
					}
				}

				if (IS_FALSE(m_parent.getIsValid()))
					return std::move(result);
				else
				{
					auto parentResult = std::move(FindComponentInParentEx<ComponentClass>());
					if (IS_FALSE(parentResult.empty())) result.insert(result.end(), parentResult.begin(), parentResult.end());
				}
			}
			//----------------------------------------------------------------------------------
			//[FindComponentInChildrenEx]
			//ComponentClass�^�R���|�[�l���g��Children����T������
			//return: ComponentClass�^�̃R���|�[�l���g���R�t�����Ă���ꍇ : �S�Ă̓��^�R���|�[�l���g
			//				�R�t�����Ă��Ȃ��ꍇ : Null
			//����1: ���g���܂߂邩, default = true
			template <class ComponentClass>
			inline std::vector<WeakPointer<ComponentClass>> TransformX11::FindComponentInChildrenEx(bool isIncludeYourself)
			{
				static_assert(std::is_base_of_v<ComponentX11, ComponentClass>,
					"TransformX11::FindComponentInChildrenEx templatre -> Component derived class only!");

				LockGuardTransform guard(m_lock);

				std::vector<WeakPointer<ComponentClass>> result;

				//���g���܂߂�
				if (IS_TRUE(isIncludeYourself))
				{
					//Children�������[�v
					for (auto& children : m_childrens)
					{
						//�������[�v
						for (auto& e : children->m_components)
						{
							//Cast
							auto cast = e.DynamicCast<ComponentClass>();
							//Hit
							if (cast.getIsValid()) result.emplase_back(cast);
						}
					}
				}

				//���������[�v
				for (auto& children : m_childrens)
				{
					auto childrenResult = children->FindComponentInChildrenEx<ComponentClass>();
					if (IS_FALSE(childrenResult.empty())) result.insert(result.end(), childrenResult.begin(), childrenResult.end());
				}

				return result;
			}
			//----------------------------------------------------------------------------------
			//[RemoveComponentEx]
			//Component���폜���� (template�^�ɑΉ������S�ẴR���|�[�l���g)
			template <class ComponentClass>
			inline void TransformX11::RemoveComponentEx()
			{
				static_assert(std::is_base_of_v<ComponentX11, ComponentClass>,
					"TransformX11::RemoveComponentEx templatre -> Component derived class only!");

				LockGuardTransform guard(m_lock);

				//�������[�v
				for (auto it = m_components.begin(); it != m_components.end();)
				{
					//Hit
					if (dynamic_cast<ComponentClass*>(it->getPointer()) != nullptr)
					{
						//Callback
						if ((*it)->m_isEnabled && ((*it)->m_callbackEnabledFlags & ComponentX11::CallbackFunctions::OnDisable))
							(*it)->OnDisable();
						if ((*it)->m_callbackEnabledFlags & ComponentX11::CallbackFunctions::OnDestroy)
							(*it)->OnDestroy();

						//Erase
						it = m_components.erase(it);
					}
					else
						++it;
				}

				//��Allocate
				this->AllocateCallback();
			}
			//----------------------------------------------------------------------------------
			//[RemoveComponentAll]
			//Component��S�č폜����
			void TransformX11::RemoveComponentAll()
			{
				LockGuardTransform guard(m_lock);

				//Callback ���[�v
				for (auto& e : m_components)
				{
					//Callback
					if (e->m_isEnabled && (e->m_callbackEnabledFlags & ComponentX11::CallbackFunctions::OnDisable))
						e->OnDisable();
					if (e->m_callbackEnabledFlags & ComponentX11::CallbackFunctions::OnDestroy)
						e->OnDestroy();
				}
				//Clear
				m_components.clear();

				//��Allocate
				this->AllocateCallback();
			}

			//----------------------------------------------------------------------------------
			//[�R���X�g���N�^]
			//�������������
			//����1: name
			//����2: position
			//����3: scale
			//����4: rotation
			//����5: isStatic
			//����6: layer
			//����7: tag
			inline TransformX11::TransformX11(const sgstring& name, const Vector3& position,
				const Vector3& scale, const Quaternion& rotation, int functionLine, bool isStatic, Layer layer,
				const Tag& tag, bool isUsedMutexCounter, bool isAutoGabageCollection)
				: Object(name), transform(), m_parent(), m_childrens(), 
				gameObject(WeakPointer<GameObject>::getNull()), timeLayer(WeakPointer<Time::TimeLayer>::getNull()),
				m_components(), m_callbackIndexs(), m_variables(rotation, position, scale, rotation, position, scale), m_lock(Lock()),
				m_updateMatrix(Const::Vector4x4::identity), m_drawMatrix(Const::Vector4x4::identity), m_staticMatrix(nullptr), 
 				m_tag(tag), m_layer(layer), m_callbackEnabledFlags(0), m_functionLineAtRoot(functionLine), m_flags(0), m_isActive(true), m_isActivePublic(true), m_isNotify(true)
			{
				if (IS_TRUE(isStatic)) m_flags |= m_cIsStaticFlag;
				if (IS_TRUE(isUsedMutexCounter)) m_flags |= m_cIsUsedMutexCounterFlag;
				if (IS_TRUE(isAutoGabageCollection)) m_flags |= m_cIsAutoGabageCollectionFlag;

				//Static
				if (m_flags & m_cIsStaticFlag)
				{
					m_staticMatrix = new Vector4x4();

					//�X�VMatrix
					Matrix matrix = Matrix::Identity();

					//world�s��v�Z
					matrix *= Matrix::Scaleing(scale);
					matrix *= rotation.ToMatrix();
					matrix *= Matrix::Translate(position);

					//Vector4x4��Store
					*m_staticMatrix = matrix;
				}
			}


			//----------------------------------------------------------------------------------
			//[StartUp]
			inline void TransformX11::StartUp(const ReadElement::Pack & lines)
			{
				m_rootList.setFunctionLines(lines.elements[0].valueInt);
				ComponentX11::AllocateCallback();
			}
			//----------------------------------------------------------------------------------
			//[Shutdown]
			inline void TransformX11::Shutdown()
			{
				m_rootList.Shutdown();
			}
			//----------------------------------------------------------------------------------
			//[AutoFunctionLine]
			//return: auto select function line
			inline int TransformX11::AutoFunctionLine()
			{
				static std::atomic_int counter = -1;
	
				int oldCounter = 0 ,newCounter = 0;
	
				do
				{
					oldCounter = counter.load();
					newCounter = oldCounter + 1;
				} while (IS_FALSE(counter.compare_exchange_weak(oldCounter, newCounter)));

				if (counter >= m_rootList.getFunctionLines())
					counter = 0;
				
				return counter;
			}
			//----------------------------------------------------------------------------------
			//[UpdateForSingleThread]
			//BackGroundUpdate (ALL)
			inline void TransformX11::UpdateForSingleThread(uint callbackType)
			{
				//���X�g�擾
				int getLine = 0;

				//Update
				while (true)
				{
					if (IS_TRUE(m_rootList.AcquisitionFunctionLineIndexForSingleThread(getLine)))
					{
						auto& transforms = m_rootList.getFunctionLine(getLine);
						for (auto& e : transforms)
						{
							e.second->UpdateForRoot(callbackType);
						}
					}
					else
						break;
				}
			}
			//----------------------------------------------------------------------------------
			//[UpdateForMultiThread1]
			//BackGroundUpdate (ALL)
			inline void TransformX11::UpdateForMultiThread1()
			{
				//���X�g�擾
				int getLine = 0;

				//Update
				while (true)
				{
					if (IS_TRUE(m_rootList.AcquisitionFunctionLineIndex(getLine)))
					{
						auto& transforms = m_rootList.getFunctionLine(getLine);
						for (auto& e : transforms)
						{
							e.second->UpdateForRoot();
						}
					}
					else
						break;
				}
			}
			//----------------------------------------------------------------------------------
			//[UpdateForMultiThread1]
			//BackGroundUpdate (ALL)
			inline void TransformX11::UpdateForMultiThread1(uint callbackType)
			{
				//���X�g�擾
				int getLine = 0;

				//Update
				while (true)
				{
					if (IS_TRUE(m_rootList.AcquisitionFunctionLineIndex(getLine)))
					{
						auto& transforms = m_rootList.getFunctionLine(getLine);
						for (auto& e : transforms)
						{
							e.second->UpdateForRoot(callbackType);
						}
					}					
					else
						break;
				}
			}
			//----------------------------------------------------------------------------------
			//[UpdateForMultiThread1]
			//BackGroundUpdate (ALL)
			inline void TransformX11::UpdateAndMakeMatrixForMultiThread(const std::vector<int>& lines, uint callbackType)
			{
				for (auto e : lines)
				{
					auto& line = m_rootList.getFunctionLine(e);
					for (auto& e : line)
					{
						e.second->UpdateForRoot(callbackType);
					}
				}
			}
			//----------------------------------------------------------------------------------
			//[SyncAndUpdateForMultiThread]
			//BackGroundUpdate (Sync)
			inline void TransformX11::SyncAndUpdateForMultiThread(const std::vector<int>& lines, uint callbackType)
			{
				for (auto e : lines)
				{
					auto& line = m_rootList.getFunctionLine(e);
					for (auto& e : line)
					{
						e.second->SyncAndUpdateForBackGround(callbackType);
					}
				}
			}
			//----------------------------------------------------------------------------------
			//[SyncForMultiThread1]
			//BackGroundUpdate (Sync)
			inline void TransformX11::SyncForMultiThread1(uint callbackType)
			{
				//���X�g�擾
				int getLine = 0;

				//Update
				while (true)
				{
					if (IS_TRUE(m_rootList.AcquisitionFunctionLineIndex(getLine)))
					{
						auto& transforms = m_rootList.getFunctionLine(getLine);
						for (auto& e : transforms)
							e.second->SyncForBackGround1();
						for (auto& e : transforms)
							e.second->InvokeCallback(callbackType);
					}
					else
						break;
				}
			}
			//----------------------------------------------------------------------------------
			//[InvokeCallbackForMultiThread]
			//ALL Callback Invoke
			inline void TransformX11::InvokeCallbackForMultiThread(const std::vector<int>& lines, uint callbackType)
			{
				for (auto e : lines)
				{
					auto& line = m_rootList.getFunctionLine(e);
					for (auto& e : line)
					{
						e.second->InvokeCallback(callbackType);
					}
				}
			}
			//----------------------------------------------------------------------------------
			//[InvokeCallbackForMultiThread1]
			//ALL Callback Invoke
			inline void TransformX11::InvokeCallbackForMultiThread1(uint callbackType)
			{
				//���X�g�擾
				int getLine = 0;

				//Update
				while (true)
				{
					if (IS_TRUE(m_rootList.AcquisitionFunctionLineIndex(getLine)))
					{
						auto& transforms = m_rootList.getFunctionLine(getLine);
						for (auto& e : transforms)
						{
							e.second->InvokeCallback(callbackType);
						}
					}
					else
						break;
				}
			}
			//----------------------------------------------------------------------------------
			//[InvokeCallbackForMultiThread1]
			//ALL Callback Invoke
			inline void TransformX11::InvokeCallbackForSingleThread(uint callbackType)
			{
				//���X�g�擾
				int getLine = 0;

				//Update
				while (true)
				{
					if (IS_TRUE(m_rootList.AcquisitionFunctionLineIndexForSingleThread(getLine)))
					{
						auto& transforms = m_rootList.getFunctionLine(getLine);
						for (auto& e : transforms)
						{
							e.second->InvokeCallback(callbackType);
						}
					}
					else
						break;
				}
			}
			//----------------------------------------------------------------------------------
			//[EndUpdateForMultiThread]
			//BackGroundUpdate (ALL And Sync)
			inline void TransformX11::EndUpdateForMultiThread()
			{
				m_rootList.ResetFunctionLineIndex();
			}

			//----------------------------------------------------------------------------------
			//[UpdateForRoot]
			//BackGroundUpdate (Parent�����Ȃ�TransformX11��)
			inline void TransformX11::UpdateForRoot()
			{
				//�X�VMatrix
				Matrix matrix;

				//Static
				if (m_flags & m_cIsStaticFlag)
					matrix = m_updateMatrix = *m_staticMatrix;
				//Accsess
				else if (IS_TRUE(m_isNotify))
				{
					//world�s��v�Z
					matrix *= Matrix::Scaleing(m_variables.worldScale);
					matrix *= m_variables.worldRotation.ToMatrix();
					matrix *= Matrix::Translate(m_variables.worldPosition);

					//Vector4x4��Store
					m_updateMatrix = matrix;

					//local�ϐ��ɑ��
					m_variables.localRotation = m_variables.worldRotation;
					m_variables.localPosition = m_variables.worldPosition;
					m_variables.localScale = m_variables.worldScale;
				}
				//world�s��ɕϊ�
				else
					matrix = m_updateMatrix;

				//IsActive�ݒ�
				m_isActive = m_isActivePublic;

				//���g�̎q�����X�V
				for (auto& e : m_childrens)
					e->UpdateForChildren(matrix, m_variables.worldRotation,
						m_variables.worldScale, m_variables.worldPosition, m_isActive, static_cast<bool>(m_flags & m_cIsStaticFlag), m_isNotify);

				//Notify����
				m_isNotify = false;
			}
			//----------------------------------------------------------------------------------
			//[UpdateForChildren]
			//BackGroundUpdate (Parent������TransformX11��)
			inline void TransformX11::UpdateForChildren(Matrix& parentMatrix, Quaternion & rotation,
				Vector3 & scale, Vector3& position, bool isActive, bool isStatic, bool isChanged)
			{
				//�X�VMatrix
				Matrix matrix;
				bool isStaticAll = isStatic & static_cast<bool>(m_flags & m_cIsStaticFlag);

				//All Static
				if (IS_TRUE(isStaticAll))
					matrix = m_updateMatrix;
				//this Static
				else if (m_flags & m_cIsStaticFlag)
				{
					//Static Matrix
					matrix = *m_staticMatrix * parentMatrix;
					//Vector4x4��Store
					m_updateMatrix = matrix;

					//world�ϐ��X�V
					m_variables.worldRotation = m_variables.localRotation * rotation;
					m_variables.worldScale = m_variables.localScale * scale;
				}
				//Accsess
				else if (m_isNotify | isChanged)
				{
					//local�s��v�Z
					matrix *= Matrix::Scaleing(m_variables.localScale);
					matrix *= m_variables.localRotation.ToMatrix();
					matrix *= Matrix::Translate(m_variables.localPosition);

					//world�s��ɕϊ�
					matrix *= parentMatrix;
					//Vector4x4��Store
					m_updateMatrix = matrix;
		
					//world�ϐ��X�V
					m_variables.worldRotation = m_variables.localRotation * rotation;
					m_variables.worldPosition = m_variables.localPosition + position;
					m_variables.worldScale = m_variables.localScale * scale;
				}
				//Not Accsess
				else
					matrix = m_updateMatrix;

				//IsActive�ݒ�
				m_isActive = m_isActivePublic;

				//���g�̎q�����X�V
				for (auto& e : m_childrens)
					e->UpdateForChildren(matrix, m_variables.worldRotation,
						m_variables.worldScale, m_variables.worldPosition, m_isActive, isStaticAll, m_isNotify | isChanged);

				//Notify����
				m_isNotify = false;
			}
			//----------------------------------------------------------------------------------
			//[UpdateForRoot]
			//BackGroundUpdate (Parent�����Ȃ�TransformX11��)
			inline void TransformX11::UpdateForRoot(uint callbackType)
			{
				//�X�VMatrix
				Matrix matrix;

				//Static
				if (m_flags & m_cIsStaticFlag)
					matrix = m_updateMatrix = *m_staticMatrix;
				//Accsess
				else if (IS_TRUE(m_isNotify))
				{
					//world�s��v�Z
					matrix *= Matrix::Scaleing(m_variables.worldScale);
					matrix *= m_variables.worldRotation.ToMatrix();
					matrix *= Matrix::Translate(m_variables.worldPosition);

					//Vector4x4��Store
					m_updateMatrix = matrix;

					//local�ϐ��ɑ��
					m_variables.localRotation = m_variables.worldRotation;
					m_variables.localPosition = m_variables.worldPosition;
					m_variables.localScale = m_variables.worldScale;
				}
				//world�s��ɕϊ�
				else
					matrix = m_updateMatrix;

				//IsActive�ݒ�
				m_isActive = m_isActivePublic;

				//���g�̎q�����X�V
				for (auto& e : m_childrens)
					e->UpdateForChildren(callbackType, matrix, m_variables.worldRotation, 
						m_variables.worldScale, m_variables.worldPosition, m_isActive, static_cast<bool>(m_flags & m_cIsStaticFlag), m_isNotify);

				//Notify����
				m_isNotify = false;

				//Component�X�V
				if (IS_TRUE(m_isActive) && (m_callbackEnabledFlags & callbackType))
					for (auto& e : m_callbackIndexs.at(callbackType))
						m_components[e]->InvokeCallback(callbackType);
			}
			//----------------------------------------------------------------------------------
			//[UpdateForChildren]
			//BackGroundUpdate (Parent������TransformX11��)
			inline void TransformX11::UpdateForChildren(uint callbackType, Matrix& parentMatrix, Quaternion& rotation,
				Vector3& scale, Vector3& position, bool isActive, bool isStatic, bool isChanged)
			{
				//�X�VMatrix
				Matrix matrix;
				bool isStaticAll = isStatic & static_cast<bool>(m_flags & m_cIsStaticFlag);

				//All Static
				if (IS_TRUE(isStaticAll))
					matrix = m_updateMatrix;
				//this Static
				else if (m_flags & m_cIsStaticFlag)
				{
					//Static Matrix
					matrix = *m_staticMatrix * parentMatrix;
					//Vector4x4��Store
					m_updateMatrix = matrix;

					//world�ϐ��X�V
					m_variables.worldRotation = m_variables.localRotation * rotation;
					m_variables.worldScale = m_variables.localScale * scale;
				}
				//Accsess
				else if (m_isNotify | isChanged)
				{
					//local�s��v�Z
					matrix *= Matrix::Scaleing(m_variables.localScale);
					matrix *= m_variables.localRotation.ToMatrix();
					matrix *= Matrix::Translate(m_variables.localPosition);

					//world�s��ɕϊ�
					matrix *= parentMatrix;
					//Vector4x4��Store
					m_updateMatrix = matrix;

					//world�ϐ��X�V
					m_variables.worldRotation = m_variables.localRotation * rotation;
					m_variables.worldPosition = m_variables.localPosition + position;
					m_variables.worldScale = m_variables.localScale * scale;
				}
				//Not Accsess
				else
					matrix = m_updateMatrix;

				//IsActive�ݒ�
				m_isActive = m_isActivePublic;

				//���g�̎q�����X�V
				for (auto& e : m_childrens)
					e->UpdateForChildren(callbackType, matrix, m_variables.worldRotation,
						m_variables.worldScale, m_variables.worldPosition, m_isActive, isStaticAll, m_isNotify | isChanged);

				//Notify����
				m_isNotify = false;

				//Component�X�V
				if (IS_TRUE(m_isActive) && (m_callbackEnabledFlags & callbackType))
					for (auto& e : m_callbackIndexs.at(callbackType))
						m_components[e]->InvokeCallback(callbackType);
			}
			//----------------------------------------------------------------------------------
			//[SyncForBackGround]
			//BackGround SyncAndUpdate
			inline void TransformX11::SyncAndUpdateForBackGround(uint callbackType)
			{
				LockGuardTransform guard(m_lock);
				std::thread::id id = m_lock.load().id;

				//Sync
				SyncDrawMatrix();

				//���g�̎q�����X�V
				for (auto& e : m_childrens)
					e->SyncAndUpdateForBackGround(callbackType);

				//Component�X�V
				if (m_callbackEnabledFlags & ComponentX11::CallbackFunctions::m_cSync)
					for (auto& e : m_callbackIndexs.at(ComponentX11::CallbackFunctions::m_cSync))
						m_components[e]->InvokeCallback(ComponentX11::CallbackFunctions::m_cSync);

				//�v�f�����Call
				if ((IS_TRUE(m_isActive) && (m_callbackEnabledFlags & callbackType)) != 0)
				{
					for (auto& e : m_callbackIndexs.at(callbackType))
						m_components[e]->InvokeCallback(callbackType);
				}
			}
			//----------------------------------------------------------------------------------
			//[SyncForRoot]
			//BackGroundSync
			inline void TransformX11::SyncForBackGround1()
			{
				//Sync
				SyncDrawMatrix();
				
				//���g�̎q�����X�V
				for (auto& e : m_childrens)
					e->SyncForBackGround1();
				
				//Component�X�V
				if (m_callbackEnabledFlags & ComponentX11::CallbackFunctions::m_cSync)
					for (auto& e : m_callbackIndexs.at(ComponentX11::CallbackFunctions::m_cSync))
						m_components[e]->InvokeCallback(ComponentX11::CallbackFunctions::m_cSync);
			}
			//----------------------------------------------------------------------------------
			//[InitLocal]
			//LocalSync���s��
			//����1: world->local?
			inline void TransformX11::InitLocal(bool isUseWorldInfo)
			{
				Matrix matrix = m_updateMatrix.ToMatrix();

				//WorldInfo�����Ƃ�LocalInfo�쐬
				if (IS_TRUE(isUseWorldInfo))
				{
					//Copy, rotration�������׌y���̂���XmVector
					XmVector rotation = m_variables.worldRotation;
					m_variables.localPosition = m_variables.worldPosition;
					m_variables.localScale = m_variables.worldScale;

					//�v�Z���[�v
					for (auto pt = m_parent; IS_TRUE(pt.LockShared()); pt = pt->m_parent)
					{
						//matrix, local->world�v�Z
						matrix *= pt->m_updateMatrix.ToMatrix();
						//world->local�v�Z
						rotation = DirectX::XMQuaternionMultiply(rotation, Quaternion::Inverse(pt->m_variables.localRotation));
						m_variables.localPosition -= pt->m_variables.localPosition;
						m_variables.localScale /= pt->m_variables.localScale;
						//Lock����
						pt.UnlockShared();
					}
					//Set
					m_variables.localRotation = rotation;


					//local�s��v�Z
					Matrix localToWorldMatrix = Matrix::Scaleing(m_variables.localScale);
					localToWorldMatrix *= m_variables.localRotation.ToMatrix();
					localToWorldMatrix *= Matrix::Translate(m_variables.localPosition);
					//local->world
					m_updateMatrix = localToWorldMatrix * matrix;
				}
				//LocalInfo�����Ƃ�WorldInfo�쐬
				else
				{
					bool isStaticAll = true;
	
					//local�s��v�Z
					matrix *= Matrix::Scaleing(m_variables.localScale);
					matrix *= m_variables.localRotation.ToMatrix();
					matrix *= Matrix::Translate(m_variables.localPosition);
					//Copy
					m_variables.worldRotation = m_variables.localRotation;
					m_variables.worldScale = m_variables.localScale;

					//�v�Z���[�v
					for (auto pt = m_parent; IS_TRUE(pt.LockShared()); pt = pt->m_parent)
					{
						//Matrix�v�Z, local->world
						matrix *= pt->m_updateMatrix.ToMatrix();
						//world�ϐ���local->world�v�Z
						m_variables.worldScale *= pt->m_variables.localScale;
						m_variables.worldRotation *= pt->m_variables.localRotation;
						//static?
						isStaticAll &= static_cast<bool>(pt->m_flags & m_cIsStaticFlag);
						//Lock����
						pt.UnlockShared();
					}

					//Set
					m_updateMatrix = matrix;
					m_isNotify = false;

					//���g�̎q�����X�V
					for (auto& e : m_childrens)
						e->UpdateForChildren(0, matrix, m_variables.worldRotation,
							m_variables.worldScale, m_variables.worldPosition, m_isActive, isStaticAll, true);
				}

				//Matrix Store
				m_updateMatrix = matrix;
			}
			//----------------------------------------------------------------------------------
			//[SyncDrawMatrix]
			//GraphicsUpdate�pMatrix�𓯊�����
			inline void TransformX11::SyncDrawMatrix()
			{
				//MemoryCopy
				memcpy_s(&m_drawMatrix, sizeof(Vector4x4), &m_updateMatrix, sizeof(Vector4x4));
			}
			//----------------------------------------------------------------------------------
			//[AllocateCallback]
			//m_callbackIndexs��ݒ�
			inline void TransformX11::AllocateCallback()
			{
				LockGuardTransform guard(m_lock);

				//i
				uint componentsIndex = 0;
				//clear
				m_callbackIndexs.clear();
				m_callbackEnabledFlags = 0;

				//�T�����[�v
				for (auto& e : m_components)
				{	
					//�t���O����
					uint flags = (e->m_saveCallbackEnabledFlags - (e->m_saveCallbackEnabledFlags & ComponentX11::CallbackFunctions::m_cExclusionAllocateFlags));
					//�t���O�l����
					if (flags != 0)
					{
						//�V�t�g���Ȃ���True Bit����, Hit�ŃC���f�b�N�X���X�g�ɒǉ�
						for (uint i = 0, shift = 1; i < ComponentX11::CallbackFunctions::m_cNumAllCallbacks; ++i, shift = 1 << i)
							if (e->m_saveCallbackEnabledFlags & shift)
							{
								if (m_callbackIndexs.find(shift) == m_callbackIndexs.end())
									m_callbackIndexs.try_emplace(shift, std::vector<uint>());
								
								m_callbackIndexs.at(shift).emplace_back(componentsIndex);
								m_callbackEnabledFlags |= shift;
							}
						//�V�t�g���Ȃ���True Bit����, Hit�ŃC���f�b�N�X���X�g�ɒǉ�
						for (uint i = ComponentX11::CallbackFunctions::m_cPrivateCallbacksStartValue, shift = 1;
							i < ComponentX11::CallbackFunctions::m_cNumAllPrivateCallbacks; ++i, shift = 1 << i)
							if (e->m_saveCallbackEnabledFlags & shift)
							{
								if (m_callbackIndexs.find(shift) == m_callbackIndexs.end())
									m_callbackIndexs.try_emplace(shift, std::vector<uint>());
								
								m_callbackIndexs.at(shift).emplace_back(componentsIndex);
								m_callbackEnabledFlags |= shift;
							}
					}
					//++i
					++componentsIndex;
				}
			}
			//----------------------------------------------------------------------------------
			//[InvokeCallback]
			//Invoke Callback in this and childrens
			inline void TransformX11::InvokeCallback(uint callbackType)
			{
				//�v�f�����Call
				if (IS_TRUE(m_isActive) && (m_callbackEnabledFlags & callbackType))
				{
					for (auto& e : m_callbackIndexs.at(callbackType))
						m_components[e]->InvokeCallback(callbackType);
				}

				for (auto& e : m_childrens)
					e->InvokeCallback(callbackType);
			}
			//----------------------------------------------------------------------------------
			//[InvokeCallbackChildrens]
			//Invoke Callback in childrens
			inline void TransformX11::InvokeCallbackChildrens(uint callbackType)
			{
				for (auto& e : m_childrens)
					e->InvokeCallback(callbackType);
			}
			//----------------------------------------------------------------------------------
			//[InvokeCallback]
			//Invoke Callback in this and childrens
			inline void TransformX11::InvokeCallback(uint callbackType, 
				const WeakPointer<TransformX11>& pointer0, const WeakPointer<TransformX11>& pointer1)
			{
				//�v�f�����Call
				if (IS_TRUE(m_isActive) && (m_callbackEnabledFlags & callbackType))
				{
					for (auto& e : m_callbackIndexs.at(callbackType))
						m_components[e]->InvokeCallbackPointer(callbackType, pointer0, pointer1);
				}

				for (auto& e : m_childrens)
					e->InvokeCallback(callbackType, pointer0, pointer1);
			}
			//----------------------------------------------------------------------------------
			//[InvokeCallbackChildrens]
			//Invoke Callback in and childrens
			inline void TransformX11::InvokeCallbackChildrens(uint callbackType, 
				const WeakPointer<TransformX11>& pointer0, const WeakPointer<TransformX11>& pointer1)
			{
				for (auto& e : m_childrens)
					e->InvokeCallback(callbackType, pointer0, pointer1);
			}
			//----------------------------------------------------------------------------------
			//[InvokeCallbackPhysics]
			//Invoke Callback in this
			inline void TransformX11::InvokeCallbackPhysics(uint callbackType, const ContactInfo& info)
			{
				//�v�f�����Call
				if (IS_TRUE(m_isActive) && (m_callbackEnabledFlags & callbackType))
				{
					for (auto& e : m_callbackIndexs.at(callbackType))
						m_components[e]->InvokeCallbackPhysics(callbackType, info);
				}
			}
			//----------------------------------------------------------------------------------
			//[VariablesUpdate]
			//World values update in this childrens invoke
			inline void  TransformX11::VariablesUpdate(Vector3& position, XmVector& quaternion, Vector3& scale)
			{
				for (auto& e : m_childrens)
					e->VariablesUpdateChildren(m_variables.worldPosition,
						quaternion, m_variables.worldScale);
			}
			//----------------------------------------------------------------------------------
			//[VariablesUpdate]
			//World values update in this childrens
			inline void  TransformX11::VariablesUpdateChildren(Vector3& position, XmVector& quaternion, Vector3& scale)
			{
				m_variables.worldPosition = position + m_variables.localPosition;
				m_variables.worldRotation = quaternion = DirectX::XMQuaternionMultiply(quaternion, m_variables.localRotation);
				m_variables.worldScale = scale * m_variables.localScale;
				
				for (auto& e : m_childrens)
					e->VariablesUpdateChildren(m_variables.worldPosition,
						quaternion, m_variables.worldScale);
			}
			//----------------------------------------------------------------------------------
			//[ParentSettings]
			//CopyInstantiate�p, parent��ݒ肵����
			inline void TransformX11::ParentSettings(WeakPointer<TransformX11> parent)
			{
				m_parent = parent;

				for (auto& e : m_childrens)
					e->ParentSettings(transform);
			}



			//----------------------------------------------------------------------------------
			//[Instantiate]
			//�R���|�[�l���g���C���X�^���X������
			//����1: copy
			//����2: �o�^����Transform
			//����3: Used mutex counter (Required for Multi thread), default = true
			//����4: Auto gabage collection?, default = true
			template <class ComponentClass>
			inline static SharedPointer<ComponentClass> ComponentX11::Instantiate(SharedPointer<ComponentClass> copy, 
				SharedPointer<TransformX11> transform, bool isUsedMutexCounter, bool isAutoGabageCollection)
			{
				static_assert(std::is_base_of_v<ComponentX11, ComponentClass>,
					"ComponentX11::Instantiate templatre -> Component derived class only!");

				//valid?
				if (IS_FALSE(transform.getIsValid() & copy.getIsValid())) return SharedPointer<ComponentClass>();

				//�쐬
				SharedPointer<ComponentClass> result(new ComponentClass(),
					isUsedMutexCounter, isAutoGabageCollection);
				//�R�s�[
				uint instanceID = result->m_instanceID;

				memcpy_s(result.getPointer(), sizeof(ComponentClass), copy.getPointer(), sizeof(ComponentClass));

				result->m_instanceID = instanceID;
				result->transform = transform;
				result->m_thisPointer = result.DownCast<ComponentX11>();
				result->m_name = transform->name;
				result->timeLayer = transform->timeLayer;
				result->gameObject = transform->gameObject;
				result->RefreshCallbackFlags();
				//Callback
				if (IS_TRUE(result->isEnabled))
				{
					if (result->m_callbackEnabledFlags & CallbackFunctions::Awake)
					{
						result->Awake();
						result->m_callbackEnabledFlags ^= CallbackFunctions::Awake;
					}
					if (result->m_callbackEnabledFlags & CallbackFunctions::OnEnable)
						result->OnEnable();
				}
			}
			//----------------------------------------------------------------------------------
			//[Instantiate]
			//�R���|�[�l���g���C���X�^���X������
			//����1: �o�^����Transform
			//����2: is Enabled, default = true
			//����3: Used mutex counter (Required for Multi thread), default = true
			//����4: Auto gabage collection?, default = true
			template <class ComponentClass>
			inline static SharedPointer<ComponentClass> ComponentX11::Instantiate(SharedPointer<TransformX11> transform, 
				bool isEnabled, bool isUsedMutexCounter, bool isAutoGabageCollection)
			{
				static_assert(std::is_base_of_v<ComponentX11, ComponentClass>,
					"ComponentX11::Instantiate templatre -> Component derived class only!");

				//valid?
				if (IS_FALSE(transform.getIsValid())) return SharedPointer<ComponentClass>();

				//�쐬
				SharedPointer<ComponentClass> result(new ComponentClass(), 
					isUsedMutexCounter, isAutoGabageCollection);
		
				//Copy
				result->transform = transform;
				result->setName(transform->name);
				result->timeLayer = transform->timeLayer;
				result->gameObject = transform->gameObject;
				result->m_isEnabled = result->m_isOldEnabled = isEnabled;
				result->m_isOldActive = transform->m_isActive;
				result->m_thisPointer = result.DownCast<ComponentX11>();

				//Callback?
				uint flags = result->m_thisPointer->EnableCallbacks();
				if (result->m_callbackEnabledFlags & CallbackFunctions::m_cSyncIncludeFlags) 
					flags |= CallbackFunctions::m_cSync;
				
				result->m_callbackEnabledFlags = result->m_saveCallbackEnabledFlags = flags;

				//invoke callback?
				if (IS_TRUE(result->m_isEnabled))
				{
					if (result->m_thisPointer->m_callbackEnabledFlags & CallbackFunctions::Awake)
					{
						result->m_thisPointer->Awake();
						result->m_thisPointer->m_callbackEnabledFlags ^= CallbackFunctions::Awake;
					}
					if (result->m_thisPointer->m_callbackEnabledFlags & CallbackFunctions::OnEnable)
						result->m_thisPointer->OnEnable();
				}

				return result;
			}
			//----------------------------------------------------------------------------------
			//[ChangeCallbackFlag]
			//CallbackEnabledFlags�̒l��ύX����, Awake�Ȃǂ͍ēx�Ăяo���悤�ɐݒ肷��Ƒ����Ă΂��
			//Copy Instantiate���s����Copy��͏�����ԂƂȂ�
			//����1: flag
			//����2: set value
			inline void ComponentX11::ChangeCallbackFlags(uint flags, bool setValue)
			{
				TransformX11::LockGuardTransform guard(transform->m_lock);

				uint saved = m_callbackEnabledFlags;

				if (IS_TRUE(setValue))
					m_callbackEnabledFlags |= flags;
				else
					m_callbackEnabledFlags &= (flags - (flags  & VariableLimit::cUintMax));

				if (m_callbackEnabledFlags & CallbackFunctions::m_cSyncIncludeFlags)
					m_callbackEnabledFlags |= CallbackFunctions::m_cSync;

				m_saveCallbackEnabledFlags = m_callbackEnabledFlags;

				//Callback
				if (IS_TRUE(m_isEnabled))
				{
					if (m_callbackEnabledFlags & CallbackFunctions::Awake)
					{
						Awake();
						m_callbackEnabledFlags ^= CallbackFunctions::Awake;
					}
					if ((saved & CallbackFunctions::OnEnable) == 0 &&
						(m_callbackEnabledFlags & CallbackFunctions::OnEnable))
						OnEnable();
				}

				//��Allocate
				transform->AllocateCallback();
			}

			//----------------------------------------------------------------------------------
			//[AllocateCallback]
			//callback table���쐬����
			inline void ComponentX11::AllocateCallback()
			{
				//Callback Awake Function Flag
				m_functionTable.try_emplace(CallbackFunctions::Awake, &ComponentX11::Awake);
				//Callback OnEnable Function Flag
				m_functionTable.try_emplace(CallbackFunctions::OnEnable, &ComponentX11::OnEnable);
				//Callback Start Function Flag
				m_functionTable.try_emplace(CallbackFunctions::Start, &ComponentX11::Start);
				//Callback Update Function Flag
				m_functionTable.try_emplace(CallbackFunctions::Update, &ComponentX11::Update);
				//Callback Sync Function Flag
				m_functionTable.try_emplace(CallbackFunctions::Sync, &ComponentX11::Sync);
				//Callback LateUpdate Function Flag
				m_functionTable.try_emplace(CallbackFunctions::LateUpdate, &ComponentX11::LateUpdate);
				//Callback FixedUpdate Function Flag
				m_functionTable.try_emplace(CallbackFunctions::FixedUpdate, &ComponentX11::FixedUpdate);
				//Callback OnDisable Function Flag
				m_functionTable.try_emplace(CallbackFunctions::OnDisable, &ComponentX11::OnDisable);
				//Callback OnDestroy Function Flag
				m_functionTable.try_emplace(CallbackFunctions::OnDestroy, &ComponentX11::OnDestroy);
				//Callback MakeDrawCommand Function Flag
				m_functionTable.try_emplace(CallbackFunctions::MakeDrawCommand, &ComponentX11::MakeDrawCommand);

				//Private Callback Sync Function Flag
				m_functionTable.try_emplace(CallbackFunctions::m_cSync, &ComponentX11::BackGroundSync);
	
			
				//Callback OnCollisionEnter Function Flag
				m_physicsFunctionTable.try_emplace(CallbackFunctions::OnCollisionEnter, &ComponentX11::OnCollisionEnter);
				//Callback OnCollisionStay Function Flag
				m_physicsFunctionTable.try_emplace(CallbackFunctions::OnCollisionStay, &ComponentX11::OnCollisionStay);
				//Callback OnCollisionExit Function Flag
				m_physicsFunctionTable.try_emplace(CallbackFunctions::OnCollisionExit, &ComponentX11::OnCollisionExit);
				//Callback OnTriggerEnter Function Flag
				m_physicsFunctionTable.try_emplace(CallbackFunctions::OnTriggerEnter, &ComponentX11::OnTriggerEnter);
				//Callback OnTriggerStay Function Flag
				m_physicsFunctionTable.try_emplace(CallbackFunctions::OnTriggerStay, &ComponentX11::OnTriggerStay);
				//Callback OnTriggerExit Function Flag
				m_physicsFunctionTable.try_emplace(CallbackFunctions::OnTriggerExit, &ComponentX11::OnTriggerExit);

				//Callback ChangeUpperHierarchy Function Flag
				m_pointerFunctionTable.try_emplace(CallbackFunctions::ChangeUpperHierarchy, &ComponentX11::ChangeUpperHierarchy);
			}
			//----------------------------------------------------------------------------------
			//[RefreshCallbackFlags]
			//Callback Flags��������Ԃɂ���
			inline void ComponentX11::RefreshCallbackFlags()
			{
				m_callbackEnabledFlags = m_saveCallbackEnabledFlags;
				m_isOldEnabled = m_isEnabled;
				m_isOldActive = transform->m_isActive;
			}
			//----------------------------------------------------------------------------------
			//[BackGroundSync]
			//Background Sync
			inline void ComponentX11::BackGroundSync()
			{
				bool isActive = transform->m_isActive;

				if (((m_isEnabled ^ m_isOldEnabled) & m_isEnabled)
					| ((isActive ^ m_isOldActive) & isActive))
				{
					if (m_callbackEnabledFlags & CallbackFunctions::Awake)
					{
						Awake();
						m_callbackEnabledFlags ^= CallbackFunctions::Awake;
					}
					if (m_callbackEnabledFlags & CallbackFunctions::OnEnable)
						OnEnable();
				}
				else if ((((m_isEnabled ^ m_isOldEnabled) & m_isOldEnabled) | ((isActive ^ m_isOldActive) & m_isOldActive))
					&& (m_callbackEnabledFlags & CallbackFunctions::OnDisable))
					OnDisable();

				if ((isActive & m_isEnabled) && (m_callbackEnabledFlags & CallbackFunctions::Start))
				{
					Start();
					m_callbackEnabledFlags ^= CallbackFunctions::Start;
				}

				m_isOldEnabled = m_isEnabled;
				m_isOldActive = isActive;
			}
			//----------------------------------------------------------------------------------
			//[InvokeCallback]
			//Callback���Ăяo��
			inline void ComponentX11::InvokeCallback(uint callbackType)
			{
				(this->*m_functionTable.at(callbackType))();
			}
			//----------------------------------------------------------------------------------
			//[InvokeCallbackPhysics]
			//Callback���Ăяo�� (Physics)
			inline void ComponentX11::InvokeCallbackPhysics(uint callbackType, const ContactInfo& info)
			{
				(this->*m_physicsFunctionTable.at(callbackType))(info);
			}
			//----------------------------------------------------------------------------------
			//[InvokeCallbackPointer]
			//Callback���Ăяo�� (Physics)
			inline void ComponentX11::InvokeCallbackPointer(uint callbackType, const WeakPointer<TransformX11>& pointer0, const WeakPointer<TransformX11>& pointer1)
			{
				(this->*m_pointerFunctionTable.at(callbackType))(pointer0, pointer1);
			}
		}
	}
}
#endif //!SGFRAMEWORK_HEADER_TRANSFORM_X11_HPP_