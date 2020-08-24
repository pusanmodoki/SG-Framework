/*----------------------------------------------------------------------------------
現在の行列情報を保存するTransformX11 class
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
	//friend用
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

	//フレームワークの管理をするAdministrator namespace
	namespace Administrator
	{
		//GraphicsDetail namespace
		namespace GraphicsDetail
		{
			//friend用
			class CameraX11;
			class PolygonSquareX11;
			class ComponentX11;
			template<class MeshVertexType, class MeshMaterialPointerType>
			class MeshRendererX11;

			//現在の行列情報を保存するTransformX11 class
			//<注意!!>親子関係のあるTransformX11は必ず別スレッド(Thread::FunctionLine)で更新させてはならない
			//<注意!!>親の居ないオブジェクトはworld〇〇の値で更新を行い, 子オブジェクトはworld〇〇は参照専用, local〇〇の値で更新を行う
			class TransformX11 final : public Object
			{
			private:
				//LockGuard用Value
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

					//コンストラクタ
					inline LockGuardTransform(SharedPointer<TransformX11> transform) 
						: m_lock(const_cast<TransformX11*>(transform.getPointer())->m_lock)
					{
						std::thread::id thisID = std::this_thread::get_id();
						Lock newLock, oldLock;
						bool result = false;

						//同じスレッドがロックしていなければロックするまで待機
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
					//コンストラクタ
					inline LockGuardTransform(std::atomic<Lock>& lock) : m_lock(lock)
					{
						std::thread::id thisID = std::this_thread::get_id();
						Lock newLock, oldLock;
						bool result = false;

						//同じスレッドがロックしていなければロックするまで待機
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
					//デストラクタ
					inline ~LockGuardTransform()
					{
						Lock newLock, oldLock;

						//同じスレッドがロックしていなければロックするまで待機
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
				//Transformを実体化する, 必ずシングルスレッド状態の時に実体化すること
				//return: transform
				//引数1: position, default = zero
				//引数2: scale, default = one
				//引数3: rotation, default = identity
				//引数4: name, default = "Transform only object"
				//引数5: use function line if root transform, default = auto
				//引数6: isStatic, default = false
				//引数7: layer, default = Default Layer
				//引数8: tag, default = Default Tag
				//引数9: SharedPointer->isUsedMutexCounter, default = ture
				//引数10: SharedPointer->isAutoGabageCollection, default = ture
				inline static SharedPointer<TransformX11> Instantiate(const Vector3& position = Const::Vector3::zero,
					const Vector3& scale = Const::Vector3::one, const Quaternion& rotation = Const::Quaternion::identity,
					const sgstring& name = L"Transform only object", int functionLineAtRoot = cAutoSelectFunctionLine,
					bool isStatic = false, Layer layer = Layer::DefaultLayer(), const Tag & tag = Tag::DefaultTag(),
					bool isUsedMutexCounter = true, bool isAutoGabageCollection = true);
				//----------------------------------------------------------------------------------
				//[Instantiate]
				//Transformを実体化する, 必ずシングルスレッド状態の時に実体化すること
				//position = zero, scale = one, rotation = identity
				//return: transform, parentが無効なポインタだった場合Nullポインタ
				//引数1: 親Transform
				//引数2: position, rotation, scale -> true = world : false = local 
				//引数3: position, default = zero
				//引数4: scale, default = one
				//引数5: rotation, default = identity
				//引数6: name, default = "Transform only object"
				//引数7: use function line if root transform, default = auto
				//引数8: isStatic, default = false
				//引数9: layer, default = Default Layer
				//引数10: tag, default = Default Tag
				//引数11: SharedPointer->isUsedMutexCounter, default = ture
				//引数12: SharedPointer->isAutoGabageCollection, default = ture
				inline static SharedPointer<TransformX11> Instantiate(SharedPointer<TransformX11> parent,
					bool isInstantiateInWorldSpace, const Vector3& position = Const::Vector3::zero,
					const Vector3& scale = Const::Vector3::one, const Quaternion& rotation = Const::Quaternion::identity, 
					const sgstring& name = L"Transform only object", int functionLineAtRoot = cAutoSelectFunctionLine,
					bool isStatic = false, Layer layer = Layer::DefaultLayer(), const Tag & tag = Tag::DefaultTag(),
					bool isUsedMutexCounter = true, bool isAutoGabageCollection = true);

				//----------------------------------------------------------------------------------
				//[デストラクタ]
				//子オブジェクトの開放
				inline ~TransformX11();

				//----------------------------------------------------------------------------------
				//[Translate]
				//worldPosition + translate
				//return: new worldPosition
				//引数1: translate vec
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
						//World設定
						m_variables.worldPosition += translate;
						//Local変換した値を代入
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
				//引数1: translate vec
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
				//引数1: Rotate quaternion
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
						//World設定
						XmVector vec = DirectX::XMQuaternionMultiply(m_variables.worldRotation, quaternion); 
						//Local変換した値を代入
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
				//引数1: Rotate quaternion
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
				//引数1: Scale magnification
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
						//World設定
						m_variables.worldScale *= magnification;
						//Local変換した値を代入
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
				//引数1: Scale magnification
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
				//reutrn: rootポインタ, いない場合はNullポインタ (注意!実行時に探査を行います)
				inline WeakPointer<TransformX11> FindRoot();
				//----------------------------------------------------------------------------------
				//[IsTargetMyUpperHierarchy]
				//return: 検索対象が自身の上位階層にいる場合true
				//引数1: 検索対象 
				inline bool IsTargetMyUpperHierarchy(const SharedPointer<TransformX11>& target);
				//----------------------------------------------------------------------------------
				//[IsTargetMyUpperHierarchy]
				//return: 検索対象が自身の上位階層にいる場合true
				//引数1: 検索対象 
				inline bool IsTargetMyUpperHierarchy(const WeakPointer<TransformX11>& target);
				//----------------------------------------------------------------------------------
				//[RegisterParent]
				//親を登録する, ポインタが有効な場合のみ成功する
				//reutrn: 成功した場合->true
				//引数1: parentポインタ
				inline bool RegisterParent(SharedPointer<TransformX11> parent);

				//----------------------------------------------------------------------------------
				//[RegisterChildren]
				//子を登録する, ポインタが有効な場合のみ成功する
				//reutrn: 成功した場合->true
				//引数1: childrenポインタ
				inline bool RegisterChildren(SharedPointer<TransformX11> children);

				//----------------------------------------------------------------------------------
				//[FindChildren]
				//子transformを検索する, index = 0から走査するので同名TransformX11は注意
				//reutrn: 成功した場合->子ポインタ, 失敗した場合->Nullポインタ
				//引数1: transform->name
				inline WeakPointer<TransformX11> FindChildren(const sgstring& name);
				//----------------------------------------------------------------------------------
				//[FindChildrenEx]
				//子transformを検索する, index = 0から走査し同名TransformX11を全て返却する
				//reutrn: 検索結果vector
				//引数1: transform->name
				inline std::vector<WeakPointer<TransformX11>> FindChildrenEx(const sgstring& name);
				//----------------------------------------------------------------------------------
				//[FindChildrenIndex]
				//子transformを検索する, index = 0から走査するので同名TransformX11は注意
				//reutrn: 成功した場合->現在のindex, 失敗した場合->VariableLimit::cUintMax
				//引数1: transform->name
				inline uint FindChildrenIndex(const sgstring& name);
				//----------------------------------------------------------------------------------
				//[FindChildrenIndexEx]
				//子transformを検索する, index = 0から走査し同名TransformX11を全て返却する
				//reutrn: 検索結果vector
				//引数1: transform->name
				inline std::vector<uint> FindChildrenIndexEx(const sgstring& name);

				//----------------------------------------------------------------------------------
				//[UnderTakeChildren]
				//子を別のtransformの子に移動させる, 必ず同一スレッド上で実行すること
				//indexが有効 & 引数1が有効な場合のみ成功する
				//reutrn: 成功した場合->true
				//引数1: 新しい親transfrom
				//引数2: 移す子TransformIndex
				inline bool UnderTakeChildren(SharedPointer<TransformX11> transform, uint index);
				//----------------------------------------------------------------------------------
				//[UnderTakeChildren]
				//子を別のtransformの子に移動させる, 必ず同一スレッド上で実行すること
				//nameが有効 & 引数1が有効な場合のみ成功する
				//reutrn: 成功した場合->true
				//引数1: 新しい親transfrom
				//引数2: 移す子Transform->name
				inline bool UnderTakeChildren(SharedPointer<TransformX11> transform, const sgstring& name);
				//----------------------------------------------------------------------------------
				//[UnderTakeChildrenEx]
				//nameの子を全て別のtransformの子に移動させる, 必ず同一スレッド上で実行すること
				//nameが有効 & 引数1が有効な場合のみ成功する
				//reutrn: 成功した場合->true
				//引数1: 新しい親transfrom
				//引数2: 移す子Transform->name
				inline bool UnderTakeChildrenEx(SharedPointer<TransformX11> transform, const sgstring& name);

				//----------------------------------------------------------------------------------
				//[IndependentChildren]
				//子オブジェクトを親子関係から切り離し独立させる
				//return: 成功した場合->独立したポインタ, 失敗した場合->Nullポインタ
				//引数1: index
				inline SharedPointer<TransformX11> IndependentChildren(uint index);
				//----------------------------------------------------------------------------------
				//[IndependentChildren]
				//子オブジェクトを親子関係から切り離し独立させる
				//index = 0から走査し最初に見つかったnameと合致するtransformを独立させる
				//return: 成功した場合->独立したポインタ, 失敗した場合->Nullポインタ
				//引数1: name
				inline SharedPointer<TransformX11> IndependentChildren(const sgstring& name);
				//----------------------------------------------------------------------------------
				//[IndependentChildren]
				//子オブジェクトを親子関係から切り離し独立させる, nameと合致するtransformを全て独立させる
				//return: 結果を格納したvector, 成功した場合->独立したポインタ, 失敗した場合->Nullポインタ
				//引数1: index
				inline std::vector<SharedPointer<TransformX11>> IndependentChildrenEx(const sgstring& name);

				//----------------------------------------------------------------------------------
				//[DestroyChildren]
				//子オブジェクトを削除する, indexが有効な場合のみ成功する
				//return: 成功したか否か
				//引数1: index
				inline bool DestroyChildren(uint index);
				//----------------------------------------------------------------------------------
				//[DestroyChildren]
				//子オブジェクトを削除する, index = 0から走査し最初に見つかったnameと合致するtransformを削除する
				//return: 成功したか否か
				//引数1: transform->name
				inline bool DestroyChildren(const sgstring& name);
				//----------------------------------------------------------------------------------
				//[DestroyChildren]
				//子オブジェクトを削除する, nameと合致するtransformを全て削除する
				//return: 成功したか否か
				//引数1: transform->name
				inline bool DestroyChildrenEx(const sgstring& name);
				//----------------------------------------------------------------------------------
				//[DestroyAllChildren]
				//子オブジェクトを全て削除する
				inline void DestroyAllChildren();

				//----------------------------------------------------------------------------------
				//[AddComponent]
				//Componentを追加する (ディープコピー)
				//引数1: component継承オブジェクト
				template <class ComponentClass>
				inline WeakPointer<ComponentClass> AddComponent(SharedPointer<ComponentClass> component);
				//----------------------------------------------------------------------------------
				//[AddComponent]
				//Componentを追加する (Instantiate)
				//return: 追加したComponent
				//引数1: Callback有効化フラグ, default = true
				template <class ComponentClass>
				inline WeakPointer<ComponentClass> AddComponent(bool isEnabled = true);


				//----------------------------------------------------------------------------------
				//[FindComponent]
				//return: ComponentClass型のコンポーネントが紐付けられている場合 : 最も古いコンポーネント
				//				紐付けられていない場合 : Null
				template <class ComponentClass>
				inline WeakPointer<ComponentClass> FindComponent();
				//----------------------------------------------------------------------------------
				//[FindComponentInParent]
				//ComponentClass型コンポーネントをthisからParentへと探査する
				//return: ComponentClass型のコンポーネントが紐付けられている場合 : 最初に見つかったコンポーネント
				//				紐付けられていない場合 : Null
				//引数1: 自身も含めるか, default = true
				template <class ComponentClass>
				inline WeakPointer<ComponentClass> FindComponentInParent(bool isIncludeYourself = true);
				//----------------------------------------------------------------------------------
				//[FindComponentInChildren]
				//ComponentClass型コンポーネントをChildrenから探査する
				//return: ComponentClass型のコンポーネントが紐付けられている場合 : 最初に見つかったコンポーネント
				//				紐付けられていない場合 : Null
				//引数1: 自身も含めるか, default = true
				template <class ComponentClass>
				inline WeakPointer<ComponentClass> FindComponentInChildren(bool isIncludeYourself = true);
				//----------------------------------------------------------------------------------
				//[FindComponentEx]
				//return: ComponentClass型のコンポーネントが紐付けられている場合 : 全ての同型コンポーネント
				//				紐付けられていない場合 : Null
				template <class ComponentClass>
				inline std::vector<WeakPointer<ComponentClass>> FindComponentEx();
				//----------------------------------------------------------------------------------
				//[FindComponentInParentEx]
				//ComponentClass型コンポーネントをthisからParentへと探査する
				//return: ComponentClass型のコンポーネントが紐付けられている場合 : 全ての同型コンポーネント
				//				紐付けられていない場合 : Null
				//引数1: 自身も含めるか, default = true
				template <class ComponentClass>
				inline std::vector<WeakPointer<ComponentClass>> FindComponentInParentEx(bool isIncludeYourself = true);
				//----------------------------------------------------------------------------------
				//[FindComponentInChildrenEx]
				//ComponentClass型コンポーネントをChildrenから探査する
				//return: ComponentClass型のコンポーネントが紐付けられている場合 : 全ての同型コンポーネント
				//				紐付けられていない場合 : Null
				//引数1: 自身も含めるか, default = true
				template <class ComponentClass>
				inline std::vector<WeakPointer<ComponentClass>> FindComponentInChildrenEx(bool isIncludeYourself = true);

				//----------------------------------------------------------------------------------
				//[RemoveComponent]
				//Componentを削除する (template型に対応した最も古いコンポーネント)
				template <class ComponentClass>
				inline void RemoveComponent();
				//----------------------------------------------------------------------------------
				//[RemoveComponentEx]
				//Componentを削除する (template型に対応した全てのコンポーネント)
				template <class ComponentClass>
				inline void RemoveComponentEx();
				//----------------------------------------------------------------------------------
				//[RemoveComponentAll]
				//Componentを全て削除する
				inline void RemoveComponentAll();

				//プロパティ扱いとして特例で変数の命名規約を使います

				//auto select function line
				static constexpr int cAutoSelectFunctionLine = VariableLimit::cIntMin;

				//transform
				WeakPointer<TransformX11> transform;
				//game object (GameObject->Instantiateから生成しなかった場合はnull)
				WeakPointer<GameObject> gameObject;
				//time layer (GameObject->Instantiateから生成しなかった場合はnull)
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
					//Indexが有効なら返却	
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
						//Local変換した値を代入
						m_variables.localPosition = set - m_parent->getWorldPosition();
						//World設定
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
						//Local変換した値を代入
						m_variables.localRotation = set * m_parent->getWorldRotation().getInverse();
						//World設定
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
						//Local変換した値を代入
						m_variables.localScale = set / m_parent->getWorldScale();
						//World設定
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
					
						//Local変換した値を代入
						m_variables.localRotation = look * m_parent->getWorldRotation().getInverse();
						//World設定
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
				//RootList用
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
					//Index取得
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
					//Index取得
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
					//Indexカウンター初期化
					inline void ResetFunctionLineIndex() { m_counter = -1; }
					//----------------------------------------------------------------------------------
					//[Add]
					//Rootリストに追加
					inline void Add(TransformX11* transform)
					{
						AtomicOperation::LockAtomic(m_lock);
						m_list[transform->m_functionLineAtRoot].try_emplace(transform->instanceID(), transform);
						AtomicOperation::UnlockAtomic(m_lock);
					}
					//----------------------------------------------------------------------------------
					//[Erase]
					//Rootリストから削除
					inline void Erase(TransformX11* transform)
					{
						AtomicOperation::LockAtomic(m_lock);
						m_list[transform->m_functionLineAtRoot].erase(transform->instanceID());
						AtomicOperation::UnlockAtomic(m_lock);
					}
					//----------------------------------------------------------------------------------
					//[Shutdown]
					//終了
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
					//カウンタ
					std::atomic<int> m_counter;
					//line数
					int m_numLines;
				};

				//----------------------------------------------------------------------------------
				//[コンストラクタ]
				//代入初期化する
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
				//BackGroundUpdate (ParentがいないTransformX11版)
				inline void UpdateForRoot();
				//----------------------------------------------------------------------------------
				//[UpdateForChildren]
				//BackGroundUpdate (ParentがいるTransformX11版)
				inline void UpdateForChildren(Matrix& parentMatrix, Quaternion& rotation,
					Vector3& scale, Vector3& position, bool isActive, bool isStatic, bool isChanged);
				//----------------------------------------------------------------------------------
				//[UpdateForRoot]
				//BackGroundUpdate (ParentがいないTransformX11版)
				inline void UpdateForRoot(uint callbackType);
				//----------------------------------------------------------------------------------
				//[UpdateForChildren]
				//BackGroundUpdate (ParentがいるTransformX11版)
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
				//LocalSyncを行う
				//引数1: world->local?
				inline void InitLocal(bool isUseWorldInfo);
				//----------------------------------------------------------------------------------
				//[SyncDrawMatrix]
				//GraphicsUpdate用Matrixを同期する
				inline void SyncDrawMatrix();
				//----------------------------------------------------------------------------------
				//[AllocateCallback]
				//m_callbackIndexsを設定
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
				//CopyInstantiate用, parentを設定し直す
				inline void ParentSettings(WeakPointer<TransformX11> parent);
				
				//flags
				static constexpr uint m_cIsStaticFlag = 0x1;
				static constexpr uint m_cIsDestroyFlag = 0x2;
				static constexpr uint m_cIsUsedMutexCounterFlag = 0x4;
				static constexpr uint m_cIsAutoGabageCollectionFlag = 0x8;

				//root list
				static RootList m_rootList;

				//ParentTransformX11ポインタ
				WeakPointer<TransformX11> m_parent;
				//ChildrenTransformX11 Venctor
				std::vector<SharedPointer<TransformX11>> m_childrens;
				//Components
				std::vector<SharedPointer<ComponentX11>> m_components;
				//処理軽減用コールバックindexリスト
				std::unordered_map<uint, std::vector<uint>> m_callbackIndexs;
				
				TransformVariables m_variables;				//public variables
				Vector4x4 m_updateMatrix;					//Update用Matrix
				Vector4x4 m_drawMatrix;						//Draw用Matrix
				Vector4x4* m_staticMatrix;						//Static時専用->Bakeする
				Tag m_tag;											//Tag
				Layer m_layer;										//Layer
				std::atomic<Lock> m_lock;					//lock atomic
				uint m_callbackEnabledFlags;					//callback flags
				int m_functionLineAtRoot;						//Root時どこのFunctionLineに属するか
				byte m_flags;											//flags
				bool m_isActive;										//isActive
				bool m_isActivePublic;							//public isActive
				bool m_isNotify;										//access用
			};

			//Component系クラスのBaseとなるComponentX11 class
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

				//Callbackを指定するフラグを定義したCallbackFunctions
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
				//コンポーネントをインスタンス化する
				//引数1: copy
				//引数2: 登録するTransform
				//引数3: Used mutex counter (Required for Multi thread), default = true
				//引数4: Auto gabage collection?, default = true
				template <class ComponentClass>
				inline static SharedPointer<ComponentClass> Instantiate(SharedPointer<ComponentClass> copy, SharedPointer<TransformX11> transform,
					bool isUsedMutexCounter = true, bool isAutoGabageCollection = true);
				//----------------------------------------------------------------------------------
				//[Instantiate]
				//コンポーネントをインスタンス化する
				//引数1: 登録するTransform
				//引数2: is Enabled, default = true
				//引数3: Used mutex counter (Required for Multi thread), default = true
				//引数4: Auto gabage collection?, default = true
				template <class ComponentClass>
				inline static SharedPointer<ComponentClass> Instantiate(SharedPointer<TransformX11> transform, bool isEnabled = true,
					bool isUsedMutexCounter = true, bool isAutoGabageCollection = true);

				//----------------------------------------------------------------------------------
				//[EnabledCallbacks]
				//return: 自身がCallされるCallback Flags, Component::CallbackFunctions参照
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
				inline virtual void ChangeUpperHierarchy(WeakPointer<TransformX11> newParent, WeakPointer<TransformX11> oldParent) {}


				//----------------------------------------------------------------------------------
				//[ChangeCallbackFlag]
				//CallbackEnabledFlagsの値を変更する, Awakeなどは再度呼び出すように設定すると即時呼ばれる
				//Copy Instantiateを行うとCopy先は初期状態となる
				//引数1: flag
				//引数2: set value
				inline void ChangeCallbackFlags(uint flags, bool setValue);

				//transform
				WeakPointer<TransformX11> transform;
				//game object (TransformをGameObject->Instantiateから生成しなかった場合はnull)
				WeakPointer<GameObject> gameObject;
				//time layer (TransformをGameObject->Instantiateから生成しなかった場合はnull)
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
				//[コンストラクタ]
				inline ComponentX11() : Object(""), transform(), m_isEnabled(false), 
					m_callbackEnabledFlags(0), m_saveCallbackEnabledFlags(0), m_isOldEnabled(false) {}
				
				//this pointer
				WeakPointer<ComponentX11> m_thisPointer;
				//is enabled
				bool m_isEnabled;

			private:
				//----------------------------------------------------------------------------------
				//[AllocateCallback]
				//callback tableを作成する
				inline static void AllocateCallback();
				//----------------------------------------------------------------------------------
				//[RefreshCallbackFlags]
				//Callback Flagsを初期状態にする
				inline void RefreshCallbackFlags();
				//----------------------------------------------------------------------------------
				//[BackGroundSync]
				//Background Sync
				inline void BackGroundSync();
				//----------------------------------------------------------------------------------
				//[InvokeCallback]
				//Callbackを呼び出す
				inline void InvokeCallback(uint callbackType);
				//----------------------------------------------------------------------------------
				//[InvokeCallbackPhysics]
				//Callbackを呼び出す (Physics)
				inline void InvokeCallbackPhysics(uint callbackType, const ContactInfo& info);
				//----------------------------------------------------------------------------------
				//[InvokeCallbackPointer]
				//Callbackを呼び出す (Pointer)
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
			//Transformを実体化する, 必ずシングルスレッド状態の時に実体化すること
			//return: transform
			//引数1: position, default = zero
			//引数2: scale, default = one
			//引数3: rotation, default = identity
			//引数4: name, default = "Transform only object"
			//引数5: use function line if root transform, default = auto
			//引数6: isStatic, default = false
			//引数7: layer, default = Default Layer
			//引数8: tag, default = Default Tag
			//引数9: SharedPointer->isUsedMutexCounter, default = ture
			//引数10: SharedPointer->isAutoGabageCollection, default = ture
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
			//Transformを実体化する, 必ずシングルスレッド状態の時に実体化すること
			//position = zero, scale = one, rotation = identity
			//return: transform, parentが無効なポインタだった場合Nullポインタ
			//引数1: 親Transform
			//引数2: position, rotation, scale -> true = world : false = local 
			//引数3: position, default = zero
			//引数4: scale, default = one
			//引数5: rotation, default = identity
			//引数6: name, default = "Transform only object"
			//引数7: use function line if root transform, default = auto
			//引数8: isStatic, default = false
			//引数9: layer, default = Default Layer
			//引数10: tag, default = Default Tag
			//引数11: SharedPointer->isUsedMutexCounter, default = ture
			//引数12: SharedPointer->isAutoGabageCollection, default = ture
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

					//重複名検索
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
			//[デストラクタ]
			//子オブジェクトの開放
			inline TransformX11::~TransformX11()
			{
				LockGuardTransform guard(m_lock);
				m_flags |= m_cIsDestroyFlag;

				//親がいる場合
				if (m_parent.LockShared())
				{
					//親リストから自分を検索するループ
					for (auto it = m_parent->m_childrens.begin(); it != m_parent->m_childrens.end();)
					{
						if (IS_TRUE(m_parent.LockShared()))
						{
							//発見したらerase & break
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

					//ポインタロック解除
					m_parent.UnlockShared();
				}
				//親が居ない
				else
				{
					//TopParentListから自身を削除
					m_rootList.Erase(this);
				}

				//子供削除
				DestroyAllChildren();

				//Remove

				//Callback ループ
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

				//コンスタントバッファ更新
				GraphicsX11::m_setConstantBufferObject.worldMatrix = m_drawMatrix.getTranspose();
				GraphicsX11::m_setConstantBufferObject.worldInverseMatrix = m_drawMatrix.getInverse();
				GraphicsX11::context->UpdateSubresource(GraphicsX11::m_constantBufferObject, 0, nullptr, &GraphicsX11::m_setConstantBufferObject, 0, 0);

				//コンスタントバッファ設定
				GraphicsX11::context->VSSetConstantBuffers(GraphicsX11::m_cConstantBufferObjectSlot, 1, &GraphicsX11::m_constantBufferObject);
				GraphicsX11::context->PSSetConstantBuffers(GraphicsX11::m_cConstantBufferObjectSlot, 1, &GraphicsX11::m_constantBufferObject);
			}

			//----------------------------------------------------------------------------------
			//[FindRoot]
			//reutrn: rootポインタ, いない場合はNullポインタ (注意!実行時に探査を行います)
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
			//return: 検索対象が自身の上位階層にいる場合true
			//引数1: 検索対象 
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
			//return: 検索対象が自身の上位階層にいる場合true
			//引数1: 検索対象 
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
			//親を登録する, 自身に親が居ない & ポインタが有効な場合のみ成功する
			//親を変える場合は親クラスからUnderTakeを実行すること
			//reutrn: 成功した場合->true
			//引数1: parentポインタ
			inline bool TransformX11::RegisterParent(SharedPointer<TransformX11> parent)
			{
				//ポインタ無効
				if (IS_TRUE(parent.getIsValid())) return false;

				LockGuardTransform guard(m_lock);

				//親がいる
				if (IS_TRUE(m_parent.LockShared()))
				{
					//親リストから自分を検索するループ
					for (auto it = m_parent->m_childrens.begin(); it != m_childrens.end();)
					{
						//発見したらerase & break
						if ((*it)->instanceID() == this->instanceID())
						{
							m_parent->m_childrens.erase(it);
							break;
						}
					}
					m_parent.UnlockShared();
				}
				//居ない
				m_rootList.Erase(this);

				auto oldParent = m_parent;
				
				//親に登録, 親の子供リストに自分を登録
				m_parent = parent;
				m_parent->m_childrens.push_back(transform.InstantiateShared());

				InvokeCallback(ComponentX11::CallbackFunctions::ChangeUpperHierarchy, oldParent, m_parent);

				//Sync
				InitLocal(true);
					
				return false;
			}

			//----------------------------------------------------------------------------------
			//[RegisterChildren]
			//子を登録する, ポインタが有効 & ポインタに親が居ない場合のみ成功する
			//親を変える場合は親クラスからUnderTakeを実行すること
			//reutrn: 成功した場合->true
			//引数1: childrenポインタ
			inline bool TransformX11::RegisterChildren(SharedPointer<TransformX11> children)
			{
				//子供が無効
				if (IS_FALSE(children.getIsValid())) return false;

				LockGuardTransform guard(m_lock);

				//親が居た
				if (IS_TRUE(children->m_parent.LockShared()))
				{

					//親リストから自分を検索するループ
					for (auto it = children->m_parent->m_childrens.begin(); it != m_childrens.end();)
					{
						//発見したらerase & break
						if ((*it)->instanceID() == this->instanceID())
						{
							children->m_parent->m_childrens.erase(it);
							break;
						}
					}
					children->m_parent.UnlockShared();
				}
				

				//追加
				m_childrens.push_back(children);
				//this登録
				children->m_parent = transform;
				//Init
				children->InitLocal(true);

				return true;
			}

			//----------------------------------------------------------------------------------
			//[FindChildren]
			//子transformを検索する, index = 0から走査するので同名TransformX11は注意
			//reutrn: 成功した場合->子ポインタ, 失敗した場合->Nullポインタ
			//引数1: transform->name
			inline WeakPointer<TransformX11> TransformX11::FindChildren(const sgstring& name)
			{
				LockGuardTransform guard(m_lock);

				//Nameのオブジェクトが存在すれば返却
				for (auto& e : m_childrens)
					if (e->name() == name) return e;

				//Error
				return WeakPointer<TransformX11>();
			}
			//----------------------------------------------------------------------------------
			//[FindChildrenEx]
			//子transformを検索する, index = 0から走査し同名TransformX11を全て返却する
			//reutrn: 検索結果vector
			//引数1: transform->name
			inline std::vector<WeakPointer<TransformX11>> TransformX11::FindChildrenEx(const sgstring & name)
			{
				LockGuardTransform guard(m_lock);

				//result
				std::vector<WeakPointer<TransformX11>> result;

				//オブジェクトが存在すれば追加
				for (auto& e : m_childrens)
					if (e->name() == name) result.emplace_back(e);

				//return move
				return std::move(result);
			}
			//----------------------------------------------------------------------------------
			//[FindChildrenIndex]
			//子transformを検索する, index = 0から走査するので同名TransformX11は注意
			//reutrn: 成功した場合->現在のindex, 失敗した場合->VariableLimit::cUintMax
			//引数1: transform->name
			inline uint TransformX11::FindChildrenIndex(const sgstring& name)
			{
				LockGuardTransform guard(m_lock);

				//Nameのオブジェクトが存在すれば返却
				for (uint i = 0; i < static_cast<uint>(m_childrens.size()); i++)
					if (m_childrens[i]->name() == name) return i;

				//Error
				return VariableLimit::cUintMax;
			}
			//----------------------------------------------------------------------------------
			//[FindChildrenIndexEx]
			//子transformを検索する, index = 0から走査し同名TransformX11を全て返却する
			//reutrn: 検索結果vector
			//引数1: transform->name
			inline std::vector<uint> TransformX11::FindChildrenIndexEx(const sgstring & name)
			{
				LockGuardTransform guard(m_lock);

				//result
				std::vector<uint> result;

				//オブジェクトが存在すれば追加
				for (uint i = 0, max = static_cast<uint>(m_childrens.size()); i < max; ++i)
					if (m_childrens[i]->name() == name) result.emplace_back(i);

				//return move
				return std::move(result);
			}

			//----------------------------------------------------------------------------------
			//[UnderTakeChildren]
			//子を別のtransformの子に移動させる, 必ず同一スレッド上で実行すること
			//indexが有効 & 引数1が有効な場合のみ成功する
			//reutrn: 成功した場合->true
			//引数1: 新しい親transfrom
			//引数2: 移す子TransformIndex
			inline bool TransformX11::UnderTakeChildren(SharedPointer<TransformX11> transform, uint index)
			{
				LockGuardTransform guard(m_lock);

				//index有効 & transform有効
				if (m_childrens.size() > index && IS_TRUE(transform.getIsValid()))
				{
					//保存
					auto children = m_childrens[index];

					//子供の親を変更
					m_childrens[index]->m_parent = transform;
					//Init
					m_childrens[index]->InitLocal(true);
					
					//transformの子供に移す
					transform->m_childrens.push_back(m_childrens[index]);				
					//自分の子供リストから削除
					m_childrens.erase(m_childrens.begin() + index);
					
					//callback
					children->InvokeCallback(ComponentX11::CallbackFunctions::ChangeUpperHierarchy, children->m_parent, this->transform);
					return true;
				}
				//無効->false
				else
					return false;
			}
			//----------------------------------------------------------------------------------
			//[UnderTakeChildren]
			//子を別のtransformの子に移動させる, 必ず同一スレッド上で実行すること
			//nameが有効 & 引数1が有効な場合のみ成功する
			//reutrn: 成功した場合->true
			//引数1: 新しい親transfrom
			//引数2: 移す子Transform->name
			inline bool TransformX11::UnderTakeChildren(SharedPointer<TransformX11> transform, const sgstring& name)
			{				
				return UnderTakeChildren(transform, FindChildrenIndex(name));
			}
			//----------------------------------------------------------------------------------
			//[UnderTakeChildrenEx]
			//nameの子を全て別のtransformの子に移動させる, 必ず同一スレッド上で実行すること
			//nameが有効 & 引数1が有効な場合のみ成功する
			//reutrn: 成功した場合->true
			//引数1: 新しい親transfrom
			//引数2: 移す子Transform->name
			inline bool TransformX11::UnderTakeChildrenEx(SharedPointer<TransformX11> transform, const sgstring& name)
			{
				LockGuardTransform guard(m_lock);

				//transform有効
				if (IS_TRUE(transform.getIsValid()))
				{
					bool isResult = false;

					while (true)
					{
						uint index = FindChildrenIndex(name);
						if (index == VariableLimit::cUintMax) break;

						isResult = true;

						//保存
						auto children = m_childrens[index];
						//子供の親を変更
						m_childrens[index]->m_parent = transform;
						//Init
						m_childrens[index]->InitLocal(true);

						//transformの子供に移す
						transform->m_childrens.push_back(m_childrens[index]);
						//自分の子供リストから削除
						m_childrens.erase(m_childrens.begin() + index);
						//callback
						children->InvokeCallback(ComponentX11::CallbackFunctions::ChangeUpperHierarchy, children->m_parent, this->transform);
					}

					return isResult;
				}
				//無効->false
				else
					return false;
			}

			//----------------------------------------------------------------------------------
			//[IndependentChildren]
			//子オブジェクトを親子関係から切り離し独立させる
			//return: 成功した場合->独立したポインタ, 失敗した場合->Nullポインタ
			//引数1: index
			inline SharedPointer<TransformX11> TransformX11::IndependentChildren(uint index)
			{
				LockGuardTransform guard(m_lock);

				//index有効
				if (m_childrens.size() > index)
				{
					//world情報を設定
					m_childrens[index]->m_variables.localRotation = m_childrens[index]->m_variables.worldRotation;
					m_childrens[index]->m_variables.localPosition = m_childrens[index]->m_variables.worldPosition;
					m_childrens[index]->m_variables.localScale = m_childrens[index]->m_variables.worldScale;

					//result取得
					auto result = std::move(m_childrens[index]);
					//parent初期化
					result->m_parent.Reset();
					//Root追加
					m_rootList.Add(result.getPointer());
					//リスト削除
					m_childrens.erase(m_childrens.begin() + index);
					
					//callback
					result->InvokeCallback(ComponentX11::CallbackFunctions::ChangeUpperHierarchy, result->m_parent, this->transform);

					//return move
					return std::move(result);
				}
				//index無効 -> Null
				else
					return SharedPointer<TransformX11>();
			}
			//----------------------------------------------------------------------------------
			//[IndependentChildren]
			//子オブジェクトを親子関係から切り離し独立させる
			//index = 0から走査し最初に見つかったnameと合致するtransformを独立させる
			//return: 成功した場合->独立したポインタ, 失敗した場合->Nullポインタ
			//引数1: name
			inline SharedPointer<TransformX11> TransformX11::IndependentChildren(const sgstring& name)
			{
				//Index検索して実行
				return std::move(IndependentChildren(FindChildren(name)));
			}
			//----------------------------------------------------------------------------------
			//[IndependentChildren]
			//子オブジェクトを親子関係から切り離し独立させる, nameと合致するtransformを全て独立させる
			//return: 結果を格納したvector, 成功した場合->独立したポインタ, 失敗した場合->Nullポインタ
			//引数1: index
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
			//子オブジェクトを削除する, indexが有効な場合のみ成功する
			//return: 成功したか否か
			//引数1: index
			inline bool TransformX11::DestroyChildren(uint index)
			{
				LockGuardTransform guard(m_lock);

				//index有効
				if (m_childrens.size() > index)
				{
					//eraseしてreturn
					m_childrens.erase(m_childrens.begin() + index);
					return true;
				}
				//index無効
				else
					return false;
			}
			//----------------------------------------------------------------------------------
			//[DestroyChildren]
			//子オブジェクトを削除する, index = 0から走査し最初に見つかったnameと合致するtransformを削除する
			//return: 成功したか否か
			//引数1: transform->name
			inline bool TransformX11::DestroyChildren(const sgstring & name)
			{
				LockGuardTransform guard(m_lock);

				//Index検索して実行
				return DestroyChildren(FindChildren(name));
			}
			//----------------------------------------------------------------------------------
			//[DestroyChildren]
			//子オブジェクトを削除する, nameと合致するtransformを全て削除する
			//return: 成功したか否か
			//引数1: transform->name
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
			//子オブジェクトを削除する
			inline void TransformX11::DestroyAllChildren()
			{
				LockGuardTransform guard(m_lock);

				//子供がいるなら削除
				if (IS_FALSE(m_childrens.empty()))
					m_childrens.clear();
			}

			//----------------------------------------------------------------------------------
			//[AddComponent]
			//Componentを追加する (ディープコピー)
			//引数1: component継承オブジェクト
			template <class ComponentClass>
			inline WeakPointer<ComponentClass> TransformX11::AddComponent(SharedPointer<ComponentClass> component)
			{
				static_assert(std::is_base_of_v<ComponentX11, ComponentClass>,
					"TransformX11::AddComponent templatre -> Component derived class only!");

				LockGuardTransform guard(m_lock);

				//追加
				auto component = std::move(ComponentX11::Instantiate<ComponentClass>(
					component, transform.InstantiateShared(), m_flags & m_cIsUsedMutexCounterFlag ? true : false,
					m_flags & m_cIsAutoGabageCollectionFlag ? true : false).DownCast<ComponentX11>());
				m_components.emplace_back(component.DownCast<ComponentX11>());
				//再Allocate
				this->AllocateCallback();

				return m_components.back();
			}
			//----------------------------------------------------------------------------------
			//[AddComponent]
			//Componentを追加する (Instantiate)
			//return: 追加したComponent
			//引数1: Callback有効化フラグ, default = true
			template <class ComponentClass>
			inline WeakPointer<ComponentClass> TransformX11::AddComponent(bool isEnabled)
			{
				static_assert(std::is_base_of_v<ComponentX11, ComponentClass>,
					"TransformX11::AddComponent templatre -> Component derived class only!");

				LockGuardTransform guard(m_lock);

				//追加
				auto component = std::move(ComponentX11::Instantiate<ComponentClass>(std::move(transform.InstantiateShared()), isEnabled));
				m_components.emplace_back(component.DownCast<ComponentX11>());
				//再Allocate
				this->AllocateCallback();
				
				return component;
			}

			//----------------------------------------------------------------------------------
			//[RemoveComponent]
			//Componentを削除する (template型に対応した最も古いコンポーネント)
			template <class ComponentClass>
			inline void TransformX11::RemoveComponent()
			{
				static_assert(std::is_base_of_v<ComponentX11, ComponentClass>,
					"TransformX11::RemoveComponentEx templatre -> Component derived class only!");

				LockGuardTransform guard(m_lock);

				//検索ループ
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
						//再Allocate
						this->AllocateCallback();
						
						return;
					}
				}
			}
			//----------------------------------------------------------------------------------
			//[FindComponent]
			//return: ComponentClass型のコンポーネントが紐付けられている場合 : 最も古いコンポーネント
			//				紐付けられていない場合 : Null
			template <class ComponentClass>
			inline WeakPointer<ComponentClass> TransformX11::FindComponent()
			{
				static_assert(std::is_base_of_v<ComponentX11, ComponentClass>,
					"TransformX11::FindComponent templatre -> Component derived class only!");

				LockGuardTransform guard(m_lock);

				//検索ループ
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
			//ComponentClass型コンポーネントをthisからParentへと探査する
			//return: ComponentClass型のコンポーネントが紐付けられている場合 : 最初に見つかったコンポーネント
			//				紐付けられていない場合 : Null
			//引数1: 自身も含めるか, default = true
			template <class ComponentClass>
			inline WeakPointer<ComponentClass> TransformX11::FindComponentInParent(bool isIncludeYourself)
			{
				static_assert(std::is_base_of_v<ComponentX11, ComponentClass>,
					"TransformX11::FindComponentInParent templatre -> Component derived class only!");

				LockGuardTransform guard(m_lock);

				//自身も含める
				if (IS_TRUE(isIncludeYourself))
				{		
					//検索ループ
					for (auto& e : m_components)
					{
						//Cast
						auto cast = e.DynamicCast<ComponentClass>();
						//Hit
						if (cast.getIsValid()) return cast;
					}
				}
				//再検索？
				return IS_TRUE(m_parent.getIsValid()) ? m_parent->FindComponentInParent<ComponentClass>() : WeakPointer<ComponentClass>();
			}
			//----------------------------------------------------------------------------------
			//[FindComponentInChildren]
			//ComponentClass型コンポーネントをChildrenから探査する
			//return: ComponentClass型のコンポーネントが紐付けられている場合 : 最初に見つかったコンポーネント
			//				紐付けられていない場合 : Null
			//引数1: 自身も含めるか, default = true
			template <class ComponentClass>
			inline WeakPointer<ComponentClass> TransformX11::FindComponentInChildren(bool isIncludeYourself)
			{
				static_assert(std::is_base_of_v<ComponentX11, ComponentClass>,
					"TransformX11::FindComponentInChildren templatre -> Component derived class only!");

				LockGuardTransform guard(m_lock);

				//自身も含める
				if (IS_TRUE(isIncludeYourself))
				{
					//Children検索ループ
					for (auto& children : m_childrens)
					{
						//検索ループ
						for (auto& e : children->m_components)
						{
							//Cast
							auto cast = e.DynamicCast<ComponentClass>();
							//Hit
							if (cast.getIsValid()) return cast;
						}
					}
				}

				//孫検索ループ
				for (auto& children : m_childrens)
				{
					auto childrenResult = children->FindComponentInChildren<ComponentClass>();
					if (childrenResult.getIsValid()) return childrenResult;
				}
				
				//見つからなかったよ
				return WeakPointer<ComponentClass>();
			}
			//----------------------------------------------------------------------------------
			//[FindComponentEx]
			//return: ComponentClass型のコンポーネントが紐付けられている場合 : 全ての同型コンポーネント
			//				紐付けられていない場合 : Null
			template <class ComponentClass>
			inline std::vector<WeakPointer<ComponentClass>> TransformX11::FindComponentEx()
			{
				static_assert(std::is_base_of_v<ComponentX11, ComponentClass>,
					"TransformX11::FindComponentEx templatre -> Component derived class only!");

				LockGuardTransform guard(m_lock);

				std::vector<WeakPointer<ComponentClass>> result;

				//検索ループ
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
			//ComponentClass型コンポーネントをthisからParentへと探査する
			//return: ComponentClass型のコンポーネントが紐付けられている場合 : 全ての同型コンポーネント
			//				紐付けられていない場合 : Null
			//引数1: 自身も含めるか, default = true
			template <class ComponentClass>
			inline std::vector<WeakPointer<ComponentClass>> TransformX11::FindComponentInParentEx(bool isIncludeYourself)
			{
				static_assert(std::is_base_of_v<ComponentX11, ComponentClass>,
					"TransformX11::FindComponentInParentEx templatre -> Component derived class only!");

				LockGuardTransform guard(m_lock);

				std::vector<WeakPointer<ComponentClass>> result;

				//自身も含める
				if (IS_TRUE(isIncludeYourself))
				{
					//検索ループ
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
			//ComponentClass型コンポーネントをChildrenから探査する
			//return: ComponentClass型のコンポーネントが紐付けられている場合 : 全ての同型コンポーネント
			//				紐付けられていない場合 : Null
			//引数1: 自身も含めるか, default = true
			template <class ComponentClass>
			inline std::vector<WeakPointer<ComponentClass>> TransformX11::FindComponentInChildrenEx(bool isIncludeYourself)
			{
				static_assert(std::is_base_of_v<ComponentX11, ComponentClass>,
					"TransformX11::FindComponentInChildrenEx templatre -> Component derived class only!");

				LockGuardTransform guard(m_lock);

				std::vector<WeakPointer<ComponentClass>> result;

				//自身も含める
				if (IS_TRUE(isIncludeYourself))
				{
					//Children検索ループ
					for (auto& children : m_childrens)
					{
						//検索ループ
						for (auto& e : children->m_components)
						{
							//Cast
							auto cast = e.DynamicCast<ComponentClass>();
							//Hit
							if (cast.getIsValid()) result.emplase_back(cast);
						}
					}
				}

				//孫検索ループ
				for (auto& children : m_childrens)
				{
					auto childrenResult = children->FindComponentInChildrenEx<ComponentClass>();
					if (IS_FALSE(childrenResult.empty())) result.insert(result.end(), childrenResult.begin(), childrenResult.end());
				}

				return result;
			}
			//----------------------------------------------------------------------------------
			//[RemoveComponentEx]
			//Componentを削除する (template型に対応した全てのコンポーネント)
			template <class ComponentClass>
			inline void TransformX11::RemoveComponentEx()
			{
				static_assert(std::is_base_of_v<ComponentX11, ComponentClass>,
					"TransformX11::RemoveComponentEx templatre -> Component derived class only!");

				LockGuardTransform guard(m_lock);

				//検索ループ
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

				//再Allocate
				this->AllocateCallback();
			}
			//----------------------------------------------------------------------------------
			//[RemoveComponentAll]
			//Componentを全て削除する
			void TransformX11::RemoveComponentAll()
			{
				LockGuardTransform guard(m_lock);

				//Callback ループ
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

				//再Allocate
				this->AllocateCallback();
			}

			//----------------------------------------------------------------------------------
			//[コンストラクタ]
			//代入初期化する
			//引数1: name
			//引数2: position
			//引数3: scale
			//引数4: rotation
			//引数5: isStatic
			//引数6: layer
			//引数7: tag
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

					//更新Matrix
					Matrix matrix = Matrix::Identity();

					//world行列計算
					matrix *= Matrix::Scaleing(scale);
					matrix *= rotation.ToMatrix();
					matrix *= Matrix::Translate(position);

					//Vector4x4にStore
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
				//リスト取得
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
				//リスト取得
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
				//リスト取得
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
				//リスト取得
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
				//リスト取得
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
				//リスト取得
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
			//BackGroundUpdate (ParentがいないTransformX11版)
			inline void TransformX11::UpdateForRoot()
			{
				//更新Matrix
				Matrix matrix;

				//Static
				if (m_flags & m_cIsStaticFlag)
					matrix = m_updateMatrix = *m_staticMatrix;
				//Accsess
				else if (IS_TRUE(m_isNotify))
				{
					//world行列計算
					matrix *= Matrix::Scaleing(m_variables.worldScale);
					matrix *= m_variables.worldRotation.ToMatrix();
					matrix *= Matrix::Translate(m_variables.worldPosition);

					//Vector4x4にStore
					m_updateMatrix = matrix;

					//local変数に代入
					m_variables.localRotation = m_variables.worldRotation;
					m_variables.localPosition = m_variables.worldPosition;
					m_variables.localScale = m_variables.worldScale;
				}
				//world行列に変換
				else
					matrix = m_updateMatrix;

				//IsActive設定
				m_isActive = m_isActivePublic;

				//自身の子供を更新
				for (auto& e : m_childrens)
					e->UpdateForChildren(matrix, m_variables.worldRotation,
						m_variables.worldScale, m_variables.worldPosition, m_isActive, static_cast<bool>(m_flags & m_cIsStaticFlag), m_isNotify);

				//Notify解除
				m_isNotify = false;
			}
			//----------------------------------------------------------------------------------
			//[UpdateForChildren]
			//BackGroundUpdate (ParentがいるTransformX11版)
			inline void TransformX11::UpdateForChildren(Matrix& parentMatrix, Quaternion & rotation,
				Vector3 & scale, Vector3& position, bool isActive, bool isStatic, bool isChanged)
			{
				//更新Matrix
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
					//Vector4x4にStore
					m_updateMatrix = matrix;

					//world変数更新
					m_variables.worldRotation = m_variables.localRotation * rotation;
					m_variables.worldScale = m_variables.localScale * scale;
				}
				//Accsess
				else if (m_isNotify | isChanged)
				{
					//local行列計算
					matrix *= Matrix::Scaleing(m_variables.localScale);
					matrix *= m_variables.localRotation.ToMatrix();
					matrix *= Matrix::Translate(m_variables.localPosition);

					//world行列に変換
					matrix *= parentMatrix;
					//Vector4x4にStore
					m_updateMatrix = matrix;
		
					//world変数更新
					m_variables.worldRotation = m_variables.localRotation * rotation;
					m_variables.worldPosition = m_variables.localPosition + position;
					m_variables.worldScale = m_variables.localScale * scale;
				}
				//Not Accsess
				else
					matrix = m_updateMatrix;

				//IsActive設定
				m_isActive = m_isActivePublic;

				//自身の子供を更新
				for (auto& e : m_childrens)
					e->UpdateForChildren(matrix, m_variables.worldRotation,
						m_variables.worldScale, m_variables.worldPosition, m_isActive, isStaticAll, m_isNotify | isChanged);

				//Notify解除
				m_isNotify = false;
			}
			//----------------------------------------------------------------------------------
			//[UpdateForRoot]
			//BackGroundUpdate (ParentがいないTransformX11版)
			inline void TransformX11::UpdateForRoot(uint callbackType)
			{
				//更新Matrix
				Matrix matrix;

				//Static
				if (m_flags & m_cIsStaticFlag)
					matrix = m_updateMatrix = *m_staticMatrix;
				//Accsess
				else if (IS_TRUE(m_isNotify))
				{
					//world行列計算
					matrix *= Matrix::Scaleing(m_variables.worldScale);
					matrix *= m_variables.worldRotation.ToMatrix();
					matrix *= Matrix::Translate(m_variables.worldPosition);

					//Vector4x4にStore
					m_updateMatrix = matrix;

					//local変数に代入
					m_variables.localRotation = m_variables.worldRotation;
					m_variables.localPosition = m_variables.worldPosition;
					m_variables.localScale = m_variables.worldScale;
				}
				//world行列に変換
				else
					matrix = m_updateMatrix;

				//IsActive設定
				m_isActive = m_isActivePublic;

				//自身の子供を更新
				for (auto& e : m_childrens)
					e->UpdateForChildren(callbackType, matrix, m_variables.worldRotation, 
						m_variables.worldScale, m_variables.worldPosition, m_isActive, static_cast<bool>(m_flags & m_cIsStaticFlag), m_isNotify);

				//Notify解除
				m_isNotify = false;

				//Component更新
				if (IS_TRUE(m_isActive) && (m_callbackEnabledFlags & callbackType))
					for (auto& e : m_callbackIndexs.at(callbackType))
						m_components[e]->InvokeCallback(callbackType);
			}
			//----------------------------------------------------------------------------------
			//[UpdateForChildren]
			//BackGroundUpdate (ParentがいるTransformX11版)
			inline void TransformX11::UpdateForChildren(uint callbackType, Matrix& parentMatrix, Quaternion& rotation,
				Vector3& scale, Vector3& position, bool isActive, bool isStatic, bool isChanged)
			{
				//更新Matrix
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
					//Vector4x4にStore
					m_updateMatrix = matrix;

					//world変数更新
					m_variables.worldRotation = m_variables.localRotation * rotation;
					m_variables.worldScale = m_variables.localScale * scale;
				}
				//Accsess
				else if (m_isNotify | isChanged)
				{
					//local行列計算
					matrix *= Matrix::Scaleing(m_variables.localScale);
					matrix *= m_variables.localRotation.ToMatrix();
					matrix *= Matrix::Translate(m_variables.localPosition);

					//world行列に変換
					matrix *= parentMatrix;
					//Vector4x4にStore
					m_updateMatrix = matrix;

					//world変数更新
					m_variables.worldRotation = m_variables.localRotation * rotation;
					m_variables.worldPosition = m_variables.localPosition + position;
					m_variables.worldScale = m_variables.localScale * scale;
				}
				//Not Accsess
				else
					matrix = m_updateMatrix;

				//IsActive設定
				m_isActive = m_isActivePublic;

				//自身の子供を更新
				for (auto& e : m_childrens)
					e->UpdateForChildren(callbackType, matrix, m_variables.worldRotation,
						m_variables.worldScale, m_variables.worldPosition, m_isActive, isStaticAll, m_isNotify | isChanged);

				//Notify解除
				m_isNotify = false;

				//Component更新
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

				//自身の子供を更新
				for (auto& e : m_childrens)
					e->SyncAndUpdateForBackGround(callbackType);

				//Component更新
				if (m_callbackEnabledFlags & ComponentX11::CallbackFunctions::m_cSync)
					for (auto& e : m_callbackIndexs.at(ComponentX11::CallbackFunctions::m_cSync))
						m_components[e]->InvokeCallback(ComponentX11::CallbackFunctions::m_cSync);

				//要素あればCall
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
				
				//自身の子供を更新
				for (auto& e : m_childrens)
					e->SyncForBackGround1();
				
				//Component更新
				if (m_callbackEnabledFlags & ComponentX11::CallbackFunctions::m_cSync)
					for (auto& e : m_callbackIndexs.at(ComponentX11::CallbackFunctions::m_cSync))
						m_components[e]->InvokeCallback(ComponentX11::CallbackFunctions::m_cSync);
			}
			//----------------------------------------------------------------------------------
			//[InitLocal]
			//LocalSyncを行う
			//引数1: world->local?
			inline void TransformX11::InitLocal(bool isUseWorldInfo)
			{
				Matrix matrix = m_updateMatrix.ToMatrix();

				//WorldInfoをもとにLocalInfo作成
				if (IS_TRUE(isUseWorldInfo))
				{
					//Copy, rotrationだけ負荷軽減のためXmVector
					XmVector rotation = m_variables.worldRotation;
					m_variables.localPosition = m_variables.worldPosition;
					m_variables.localScale = m_variables.worldScale;

					//計算ループ
					for (auto pt = m_parent; IS_TRUE(pt.LockShared()); pt = pt->m_parent)
					{
						//matrix, local->world計算
						matrix *= pt->m_updateMatrix.ToMatrix();
						//world->local計算
						rotation = DirectX::XMQuaternionMultiply(rotation, Quaternion::Inverse(pt->m_variables.localRotation));
						m_variables.localPosition -= pt->m_variables.localPosition;
						m_variables.localScale /= pt->m_variables.localScale;
						//Lock解除
						pt.UnlockShared();
					}
					//Set
					m_variables.localRotation = rotation;


					//local行列計算
					Matrix localToWorldMatrix = Matrix::Scaleing(m_variables.localScale);
					localToWorldMatrix *= m_variables.localRotation.ToMatrix();
					localToWorldMatrix *= Matrix::Translate(m_variables.localPosition);
					//local->world
					m_updateMatrix = localToWorldMatrix * matrix;
				}
				//LocalInfoをもとにWorldInfo作成
				else
				{
					bool isStaticAll = true;
	
					//local行列計算
					matrix *= Matrix::Scaleing(m_variables.localScale);
					matrix *= m_variables.localRotation.ToMatrix();
					matrix *= Matrix::Translate(m_variables.localPosition);
					//Copy
					m_variables.worldRotation = m_variables.localRotation;
					m_variables.worldScale = m_variables.localScale;

					//計算ループ
					for (auto pt = m_parent; IS_TRUE(pt.LockShared()); pt = pt->m_parent)
					{
						//Matrix計算, local->world
						matrix *= pt->m_updateMatrix.ToMatrix();
						//world変数もlocal->world計算
						m_variables.worldScale *= pt->m_variables.localScale;
						m_variables.worldRotation *= pt->m_variables.localRotation;
						//static?
						isStaticAll &= static_cast<bool>(pt->m_flags & m_cIsStaticFlag);
						//Lock解除
						pt.UnlockShared();
					}

					//Set
					m_updateMatrix = matrix;
					m_isNotify = false;

					//自身の子供を更新
					for (auto& e : m_childrens)
						e->UpdateForChildren(0, matrix, m_variables.worldRotation,
							m_variables.worldScale, m_variables.worldPosition, m_isActive, isStaticAll, true);
				}

				//Matrix Store
				m_updateMatrix = matrix;
			}
			//----------------------------------------------------------------------------------
			//[SyncDrawMatrix]
			//GraphicsUpdate用Matrixを同期する
			inline void TransformX11::SyncDrawMatrix()
			{
				//MemoryCopy
				memcpy_s(&m_drawMatrix, sizeof(Vector4x4), &m_updateMatrix, sizeof(Vector4x4));
			}
			//----------------------------------------------------------------------------------
			//[AllocateCallback]
			//m_callbackIndexsを設定
			inline void TransformX11::AllocateCallback()
			{
				LockGuardTransform guard(m_lock);

				//i
				uint componentsIndex = 0;
				//clear
				m_callbackIndexs.clear();
				m_callbackEnabledFlags = 0;

				//探査ループ
				for (auto& e : m_components)
				{	
					//フラグ分解
					uint flags = (e->m_saveCallbackEnabledFlags - (e->m_saveCallbackEnabledFlags & ComponentX11::CallbackFunctions::m_cExclusionAllocateFlags));
					//フラグ値あり
					if (flags != 0)
					{
						//シフトしながらTrue Bit検索, Hitでインデックスリストに追加
						for (uint i = 0, shift = 1; i < ComponentX11::CallbackFunctions::m_cNumAllCallbacks; ++i, shift = 1 << i)
							if (e->m_saveCallbackEnabledFlags & shift)
							{
								if (m_callbackIndexs.find(shift) == m_callbackIndexs.end())
									m_callbackIndexs.try_emplace(shift, std::vector<uint>());
								
								m_callbackIndexs.at(shift).emplace_back(componentsIndex);
								m_callbackEnabledFlags |= shift;
							}
						//シフトしながらTrue Bit検索, Hitでインデックスリストに追加
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
				//要素あればCall
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
				//要素あればCall
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
				//要素あればCall
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
			//CopyInstantiate用, parentを設定し直す
			inline void TransformX11::ParentSettings(WeakPointer<TransformX11> parent)
			{
				m_parent = parent;

				for (auto& e : m_childrens)
					e->ParentSettings(transform);
			}



			//----------------------------------------------------------------------------------
			//[Instantiate]
			//コンポーネントをインスタンス化する
			//引数1: copy
			//引数2: 登録するTransform
			//引数3: Used mutex counter (Required for Multi thread), default = true
			//引数4: Auto gabage collection?, default = true
			template <class ComponentClass>
			inline static SharedPointer<ComponentClass> ComponentX11::Instantiate(SharedPointer<ComponentClass> copy, 
				SharedPointer<TransformX11> transform, bool isUsedMutexCounter, bool isAutoGabageCollection)
			{
				static_assert(std::is_base_of_v<ComponentX11, ComponentClass>,
					"ComponentX11::Instantiate templatre -> Component derived class only!");

				//valid?
				if (IS_FALSE(transform.getIsValid() & copy.getIsValid())) return SharedPointer<ComponentClass>();

				//作成
				SharedPointer<ComponentClass> result(new ComponentClass(),
					isUsedMutexCounter, isAutoGabageCollection);
				//コピー
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
			//コンポーネントをインスタンス化する
			//引数1: 登録するTransform
			//引数2: is Enabled, default = true
			//引数3: Used mutex counter (Required for Multi thread), default = true
			//引数4: Auto gabage collection?, default = true
			template <class ComponentClass>
			inline static SharedPointer<ComponentClass> ComponentX11::Instantiate(SharedPointer<TransformX11> transform, 
				bool isEnabled, bool isUsedMutexCounter, bool isAutoGabageCollection)
			{
				static_assert(std::is_base_of_v<ComponentX11, ComponentClass>,
					"ComponentX11::Instantiate templatre -> Component derived class only!");

				//valid?
				if (IS_FALSE(transform.getIsValid())) return SharedPointer<ComponentClass>();

				//作成
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
			//CallbackEnabledFlagsの値を変更する, Awakeなどは再度呼び出すように設定すると即時呼ばれる
			//Copy Instantiateを行うとCopy先は初期状態となる
			//引数1: flag
			//引数2: set value
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

				//再Allocate
				transform->AllocateCallback();
			}

			//----------------------------------------------------------------------------------
			//[AllocateCallback]
			//callback tableを作成する
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
			//Callback Flagsを初期状態にする
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
			//Callbackを呼び出す
			inline void ComponentX11::InvokeCallback(uint callbackType)
			{
				(this->*m_functionTable.at(callbackType))();
			}
			//----------------------------------------------------------------------------------
			//[InvokeCallbackPhysics]
			//Callbackを呼び出す (Physics)
			inline void ComponentX11::InvokeCallbackPhysics(uint callbackType, const ContactInfo& info)
			{
				(this->*m_physicsFunctionTable.at(callbackType))(info);
			}
			//----------------------------------------------------------------------------------
			//[InvokeCallbackPointer]
			//Callbackを呼び出す (Physics)
			inline void ComponentX11::InvokeCallbackPointer(uint callbackType, const WeakPointer<TransformX11>& pointer0, const WeakPointer<TransformX11>& pointer1)
			{
				(this->*m_pointerFunctionTable.at(callbackType))(pointer0, pointer1);
			}
		}
	}
}
#endif //!SGFRAMEWORK_HEADER_TRANSFORM_X11_HPP_