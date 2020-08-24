/*----------------------------------------------------------------------------------
Collider classのBaseとなるBaseCollider class
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_BASE_COLLIDER_HPP_
#define SGFRAMEWORK_HEADER_BASE_COLLIDER_HPP_
#include "../Physics.hpp"
#include "../RigidBody/RigidBody.hpp"
#include "../../05_Graphics/TransformUsing.hpp"

//Framework namespace
namespace SGFramework
{
	//Base Classes
	namespace BaseClass
	{
		//Collider classのBaseとなるBaseCollider class
		class BaseCollider : public Component
		{
		public:
			//friend
			friend class BaseRigidBody;
			friend class Physics;

			//引数が違うのでvirtualにはしない
			//virtual BuildCollider() = 0;	

			//physx pointer (read function property, not lokced)
			SGF_FUNCTION_PROPERTY physx::PxShape& getPhysX() const { return *m_shape; }
			//collider center (read function property, not lokced)
			SGF_FUNCTION_PROPERTY Vector3 getCenter() { return m_center; }
			//collider center (set function property, not lokced)
			//argument 1: set value
			SGF_FUNCTION_PROPERTY Vector3 setCenter(const Vector3& set)  { return m_center = set; }

			//collider type (read function property, not lokced)
			SGF_FUNCTION_PROPERTY ColliderType getColliderType() { return m_type; }

			//builded is collider? (read function property, not lokced)
			SGF_FUNCTION_PROPERTY bool getIsBuilded() { return m_shape ? true : false; }
			//linked rigid body? (read function property, not lokced)
			SGF_FUNCTION_PROPERTY bool getIsLinkedRigidBody() { return m_isLinkRigidBody; }
			//collider is trigger? (read function property, not lokced)
			SGF_FUNCTION_PROPERTY bool getIsTrigger()
			{
				Physics::LockGuardPhysics guard(false);
				return m_shape ?
					m_shape->getFlags() & physx::PxShapeFlag::eTRIGGER_SHAPE ? true : false
					: false;
			}
			//collider is trigger? (set function property, not lokced)
			//argument 1: set value
			SGF_FUNCTION_PROPERTY bool setIsTrigger(bool set)
			{
				Physics::LockGuardPhysics guard(true);
				
				//simulation shapeとtriggerは共存できない
				if (IS_TRUE(set))
				{
					m_shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
					m_shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
				}
				else
				{
					m_shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, false);
					m_shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
				}
				return set;
			}

			//attachd is rigid body static or kincematic only? (read function property)
			SGF_FUNCTION_PROPERTY virtual bool getIsStaticOnlly() = 0;
		
			//attachd rigid body transform
			GetOnlyProperty<WeakPointer<Transform>> attachedRigidBodyTransform;
			//linked rigid body
			GetOnlyProperty<WeakPointer<BaseClass::BaseRigidBody>> rigidBody;

		protected:
			//all setting constructor
			inline BaseCollider() : 
				attachedRigidBodyTransform(m_rigidBodyTransform), rigidBody(m_rigidBody) {}

			//----------------------------------------------------------------------------------
			//[EnabledCallbacks]
			//return: 自身がCallされるCallback Flags, Component::CallbackFunctions参照
			inline uint EnableCallbacks() override { return CallbackFunctions::OnDestroy | CallbackFunctions::ChangeUpperHierarchy; }

			//----------------------------------------------------------------------------------
			//[GetActor]
			//Actorを検索
			inline void GetActor() 
			{
				//find dynamic rigid body
				auto rigidDynamic = IS_FALSE(getIsStaticOnlly()) ?
					transform->FindComponentInParent<RigidBodyDynamic>() : WeakPointer<RigidBodyDynamic>::getNull();
				//find static rigid body
				auto rigidStatic = transform->FindComponentInParent<RigidBodyStatic>();

				//find?
				bool isFindDynamic = rigidDynamic.getIsValid();
				bool isFindStatic = rigidStatic.getIsValid();

				//find dynamic
				if ((isFindDynamic ^ isFindStatic) & isFindDynamic)
				{
					m_rigidBodyTransform = rigidDynamic->transform;
					m_rigidBody = rigidDynamic.DownCast<BaseClass::BaseRigidBody>();
					m_actor = &rigidDynamic->getPhysX();
				}
				//find static
				else if ((isFindDynamic ^ isFindStatic) & isFindStatic)
				{
					m_rigidBodyTransform = rigidStatic->transform;
					m_rigidBody = rigidStatic.DownCast<BaseClass::BaseRigidBody>();
					m_actor = &rigidStatic->getPhysX();
				}
				//Both Find->どちらが直近か判定
				else if (isFindDynamic & isFindStatic)
				{
					//parent検索ループ
					for (auto parent = transform; 
						IS_TRUE(parent.getIsValid()); parent = parent->getParent())
					{
						//どちらか見つかれば終了
						if (rigidStatic->transform->instanceID() == parent->instanceID()
							|| rigidDynamic->transform->instanceID() == parent->instanceID())
						{
							m_rigidBodyTransform = parent;
							m_actor = &rigidStatic->getPhysX();
							break;
						}
					}
				}

				//linked ?
				m_isLinkRigidBody = m_rigidBodyTransform.getIsValid();
				//linked->equal判定
				if (IS_TRUE(m_isLinkRigidBody))
					m_isEqualRigidTransform = (transform->instanceID() == m_rigidBodyTransform->instanceID());
				else
					m_isEqualRigidTransform = false;

				//init
				m_isUnLinked = false;
				AtomicOperation::Add(m_rigidBody->m_numAttachColliders, 1U);
			}

			//----------------------------------------------------------------------------------
			//[OnDestroy]
			//インスタンスが削除される瞬間にCallbackされる関数
			inline void OnDestroy() override
			{
				using namespace Detail::Physics;
	
				m_rigidBodyTransform.Reset();
				m_isEqualRigidTransform = m_isLinkRigidBody = false;

				if (m_shape != nullptr)
				{
					//message作成
					PhysicsMessage message(MessageType::PointerMessageFlag | MessageType::UnregisterCollider, instanceID());

					//send message
					Physics::SendPhysicsMessage(message);

					m_shape = nullptr;
				}
			}

			//----------------------------------------------------------------------------------
			//[ChangeUpperHierarchy]
			//parent以上の階層関係に変更が行われた(parentの変更など)瞬間にCallbackされる関数
			//仕様上UpdateFlameで呼び出される可能性があることに留意すること (parentの変更などを行ったスレッドで呼び出し)
			//引数1: new parent (nullの可能性あり)
			//引数2: old parent (nullの可能性あり)
			inline virtual void ChangeUpperHierarchy(WeakPointer<Transform> newParent, WeakPointer<Transform> oldParent) override
			{
				//transform直付け or linkそもそもしていない or まだ親にRigid Bodyが存在しているなら終了
				if (m_isEqualRigidTransform | IS_FALSE(m_isLinkRigidBody)
					| transform->IsTargetMyUpperHierarchy(m_rigidBodyTransform)) 
					return;

				//new parentが存在しない場合は自身を無効化
				if (IS_FALSE(newParent.getIsValid()))
				{
					m_rigidBodyTransform.Reset();
					return;
				}

				//get component
				WeakPointer<BaseClass::BaseRigidBody> getComponent;

				//static onlyな場合->Staticのみで検索, そうでない場合は両方検索
				if (IS_FALSE(getIsStaticOnlly()))
					getComponent = transform->FindComponentInParent<RigidBodyStatic>(false).DownCast<BaseClass::BaseRigidBody>();
				else
					getComponent = transform->FindComponentInParent<BaseClass::BaseRigidBody>(false);

				//見つけたら追加, 見つからなければ無効化
				if (IS_TRUE(getComponent.getIsValid()))
					TransferRigidBody(getComponent->transform, getComponent);
				else
					m_rigidBodyTransform.Reset();
			}


			//----------------------------------------------------------------------------------
			//[CreateShape]
			//実際にShapeを作成する
			inline virtual void CreateShape() = 0;
			//----------------------------------------------------------------------------------
			//[ChangeScale]
			//Scaleを変更する
			inline virtual void ChangeScale() = 0;

			//----------------------------------------------------------------------------------
			//[InitFilterMask]
			//FilterDataを設定する
			inline void InitFilterMask(bool isUsedCallback)
			{
				//Filter Data
				//world0 = this layer
				//world1 = layerMask
				//world2 = used callback = 1
				//world3 = transform instance id
				//Query Data
				//world0 = layerMask
				//world1 = exclude instance id

				physx::PxFilterData data;
				data.word0 = transform->getLayer().getLayerValue();
				data.word1 = Layer::getLayerCollisionMask(transform->getLayer().getLayerValue());
				data.word2 = IS_TRUE(isUsedCallback) ? 1 : 0;
				data.word3 = m_rigidBody->instanceID();

				m_shape->setSimulationFilterData(data);
				m_shape->setQueryFilterData(data);
			}

			//----------------------------------------------------------------------------------
			//[CalculateCenter]
			//return: direction * center
			inline Vector3 CalculateCenter()
			{
				return transform->getRight() * m_center.x
					 + transform->getUp() * m_center.y,
					 + transform->getForward() * m_center.z;
			}

			//----------------------------------------------------------------------------------
			//[UpdatePhysics]
			//PhysicsからUpdateとしてCallbackされる関数
			void UpdatePhysics()
			{
				//un linked
				if ((m_isLinkRigidBody & m_rigidBodyTransform.getIsValid()) ^ true)
				{
					//not simulation
					if (m_shape) m_shape->setFlags(static_cast<physx::PxShapeFlags>(0));
					m_isUnLinked = true;
					m_isLinkRigidBody = false;
					return;
				}

				//un linked->linked
				if (IS_TRUE(m_isUnLinked))
				{
					//set flags
					m_shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, !m_isTrigger);
					m_shape->setFlag(physx::PxShapeFlag::eSCENE_QUERY_SHAPE, true);
					m_shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, m_isTrigger);
					m_shape->setFlag(physx::PxShapeFlag::eVISUALIZATION, IsDebug::isValue());
					//linked
					m_isUnLinked = false;
				}
				//not shared & not equal attach
				else if (m_shape->isExclusive() & (m_isEqualRigidTransform ^ 1))
				{
					//not plane
					if (m_type != ColliderType::Plane)
					{
						//Change scale
						ChangeScale();

						//equal rigidbody transfoem
						if (IS_TRUE(m_isEqualRigidTransform))
							m_shape->setLocalPose(physx::PxTransform(CalculateCenter().ToPhysX(), Const::Quaternion::identity.ToPhysX()));
						//not equal rigidbody transfoem
						else
							m_shape->setLocalPose(physx::PxTransform(
							(transform->getWorldPosition() - transform->getParent()->getWorldPosition() + CalculateCenter()).ToPhysX(),
								(transform->getWorldRotation() * transform->getParent()->getWorldRotation().getInverse()).ToPhysX()));
					}
					//plane
					else
					{
						m_shape->setLocalPose(physx::PxTransformFromPlaneEquation(
							physx::PxPlane(
							(transform->getWorldPosition() + CalculateCenter()).ToPhysX(),
								(transform->getWorldRotation() * Const::Vector3::down).ToPhysX())));
					}
				}
			}

			//----------------------------------------------------------------------------------
			//[TransferRigidBody]
			//NewRigidBodyへColliderを移動する
			//引数1: Transform
			//引数2: RigidBody
			inline void TransferRigidBody(const WeakPointer<Transform>& newRigidBodyTransform,
				const WeakPointer<BaseClass::BaseRigidBody>& newRigidBody)
			{
				using namespace Detail::Physics;

				//ShapeがNullでない
				if (m_shape != nullptr)
				{
					//message作成
					PhysicsMessage message(MessageType::BaseColliderMessageFlag | MessageType::TransferRigidBodyTR, instanceID());
					message.attachment.transferRigidBody.shape = m_shape;
					message.attachment.transferRigidBody.oldActor = m_actor;
					message.attachment.transferRigidBody.newActor = newRigidBody->getBaseActor();
					//send message
					Physics::SendPhysicsMessage(message);
				}

				//代入
				m_rigidBodyTransform = newRigidBodyTransform;
				m_rigidBody = newRigidBody;
				m_actor = newRigidBody->getBaseActor();
				//初期化
				m_isUnLinked = false;
				//Flag設定
				m_isLinkRigidBody = m_rigidBodyTransform.getIsValid();
				if (IS_TRUE(m_isLinkRigidBody))
					m_isEqualRigidTransform = (transform->instanceID() == m_rigidBodyTransform->instanceID());
				else
					m_isEqualRigidTransform = false;

				//更新
				//UpdatePhysics();
			}

			//----------------------------------------------------------------------------------
			//privateに隠すため空宣言
			inline void OnEnable() override {}
			inline void Start() override {}
			inline void Update() override {}
			inline void Sync() override {}
			inline void LateUpdate() override {}
			inline void FixedUpdate() override {}
			inline void MakeDrawCommand() override {}
			inline void OnDisable() override {}
			inline void OnCollisionEnter(const ContactInfo& contactInfo) override {}
			inline void OnCollisionStay(const ContactInfo& contactInfo) override {}
			inline void OnCollisionExit(const ContactInfo& contactInfo) override {}
			inline void OnTriggerEnter(const ContactInfo& contactInfo) override {}
			inline void OnTriggerStay(const ContactInfo& contactInfo) override {}
			inline void OnTriggerExit(const ContactInfo& contactInfo) override {}

			//RigidBody attach transform
			WeakPointer<Transform> m_rigidBodyTransform;
			//RigidBody
			WeakPointer<BaseClass::BaseRigidBody> m_rigidBody;
			//Shape
			physx::PxShape* m_shape = nullptr;
			//Actor
			physx::PxRigidActor* m_actor = nullptr;		
			PhysicsMaterial m_material;							//Material
			ColliderType m_type = ColliderType::Invalid;	//Collider type
			Vector3 m_center = Const::Vector3::zero;		//Center
			bool m_isEqualRigidTransform = false;				//Equal rigid body attach transform
			bool m_isLinkRigidBody = false;						//Linked rigid body ?
			bool m_isUnLinked = false;								//Linked -> UnLinked
			bool m_isTrigger = false;								//Collider is trigger?
			bool m_isUsedCallback= false;						//Used callback?
		};
	}
}
#endif //!SGFRAMEWORK_HEADER_BASE_COLLIDER_HPP_