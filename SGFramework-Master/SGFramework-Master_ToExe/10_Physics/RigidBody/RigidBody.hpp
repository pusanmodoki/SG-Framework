/*----------------------------------------------------------------------------------
RigidBodyを定義するRigidBody.hpp
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_RIGID_BODY_HPP_
#define SGFRAMEWORK_HEADER_RIGID_BODY_HPP_
#include <PxPhysicsAPI.h>
#include "../../05_Graphics/Graphics.hpp"
#include "../Physics.hpp"

//Framework namespace
namespace SGFramework
{
	//prototype
	class Application;
	struct ContactInfo;

	//Base Classes
	namespace BaseClass
	{
		//Static, DynamicのBaseとなるBaseRigidBody class
		class BaseRigidBody : public Component
		{
		public:
			//friend
			friend class Physics;
			friend class BaseCollider;

			//num attached colliders (read function property)
			SGF_FUNCTION_PROPERTY uint getNumAttachedColliders() { m_numAttachColliders.load(); }
			//is static rigid body ? (read function property)
			SGF_FUNCTION_PROPERTY bool getIsStatic() { return m_isStatic; }

		protected:
			//----------------------------------------------------------------------------------
			//[BuildRigidBody]
			//RigidBody作成
			virtual void BuildRigidBody() = 0;
			//----------------------------------------------------------------------------------
			//[UpdatePhysics]
			//Physicsから呼び出される更新関数
			virtual void UpdatePhysics() = 0;
			//----------------------------------------------------------------------------------
			//[TransferColliders]
			//自身のRigidBody
			void TransferColliders();

			//----------------------------------------------------------------------------------
			//[OnDestroy]
			//インスタンスが削除される瞬間にCallbackされる関数
			inline void OnDestroy() override
			{
				using namespace Detail::Physics;

				if (getBaseActor() == nullptr) return;

				//自身にColliderが紐付いている & Transformごと削除でない場合親に移すよう試す
				if (m_numAttachColliders.load() > 0)
					TransferColliders();

				//message作成
				PhysicsMessage message(MessageType::PointerMessageFlag | MessageType::UnregisterRigidBody, instanceID());
				//send message
				Physics::SendPhysicsMessage(message);
			}

			//----------------------------------------------------------------------------------
			//[DefaultShape]
			//return: default shape (shared)
			inline static physx::PxShape* DefaultShape()
			{
				physx::PxShape* shape = nullptr;

				//Simulationなどは行わない
				shape = Physics::physicsFactory->createShape(physx::PxSphereGeometry(1.0f),
					*Physics::defaultPhysicsMaterial(), true);
				shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
				shape->setFlag(physx::PxShapeFlag::eSCENE_QUERY_SHAPE, false);
				shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, false);
				shape->setFlag(physx::PxShapeFlag::eVISUALIZATION, false);
				shape->setName(getDefaultShapeName());

				physx::PxFilterData data;
				data.word0 = 0;
				data.word1 = 0;
				data.word2 = 0;
				data.word3 = 0;

				shape->setSimulationFilterData(data);
				shape->setQueryFilterData(data);

				return shape;
			}

			//base rigid actor (read function property)
			SGF_FUNCTION_PROPERTY virtual physx::PxRigidActor* getBaseActor() = 0;

			//DefaultShape name (read function property)
			SGF_FUNCTION_PROPERTY static const char* getDefaultShapeName()
			{
				static std::string data = "Default shape only for SGFramework";
				return data.data();
			}
			
			//is rigid body static
			bool m_isStatic = false;

			//is register physics scene?
			bool m_isRegisterScene = false;

		private:
			//----------------------------------------------------------------------------------
			//[InvokeCallback]
			//Physics hit callbackをtransformに送信する
			//引数1: callback type
			//引数2: contact info
			inline void InvokeCallback(uint callbackType, const ContactInfo& info)
			{
				transform->InvokeCallbackPhysics(callbackType, info);
			}
			
			//Colliders
			std::atomic_uint m_numAttachColliders = 0;
		};
	}



	//動かないstatic objectにcolliderを適用することができるRigidBodyStatic class
	class RigidBodyStatic final : public BaseClass::BaseRigidBody
	{
	public:

		//physx pointer (read function property)
		SGF_FUNCTION_PROPERTY physx::PxRigidStatic& getPhysX() { return *m_rigidBody; }

	private:
		//----------------------------------------------------------------------------------
		//[BuildRigidBody]
		//RigidBody作成
		inline void BuildRigidBody() override
		{
			auto shape = DefaultShape();

			auto pxTransform = physx::PxTransform(transform->getWorldPosition().ToPhysX(), transform->getWorldRotation().ToPhysX());
			while (IS_FALSE(pxTransform.isValid()))
			{
				pxTransform.q = Const::Quaternion::identity.ToPhysX();
				if (IS_FALSE(pxTransform.isValid()))
					pxTransform.p = Const::Vector3::zero.ToPhysX();
			}

			//create rigid body
			m_rigidBody = physx::PxCreateStatic(*Physics::physicsFactory, pxTransform, *shape);

			shape->release();
		}
		//----------------------------------------------------------------------------------
		//[UpdatePhysics]
		//Physicsから呼び出される更新関数
		inline void UpdatePhysics() override {}
		//----------------------------------------------------------------------------------
		//[EnabledCallbacks]
		//return: 自身がCallされるCallback Flags, Component::CallbackFunctions参照
		inline uint EnableCallbacks() override { return CallbackFunctions::Awake | CallbackFunctions::OnDestroy; }

		//----------------------------------------------------------------------------------
		//[Awake]
		//インスタンス化した瞬間に呼ばれるCallback関数
		void Awake() override
		{
			using namespace Detail::Physics;
			
			//is static
			m_isStatic = true;

			//message作成
			PhysicsMessage message(MessageType::PointerMessageFlag | MessageType::RegisterRigidBodyRG, instanceID());
			message.attachment.rigidBody = m_thisPointer.DynamicCast<BaseClass::BaseRigidBody>();

			//send message
			Physics::SendPhysicsMessage(message);		
		}

		//base rigid actor (read function property)
		SGF_FUNCTION_PROPERTY physx::PxRigidActor* getBaseActor() override { return m_rigidBody; }

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
		inline void ChangeUpperHierarchy(WeakPointer<Transform> newParent, WeakPointer<Transform> oldParent) override {}

		//rigid body
		physx::PxRigidStatic* m_rigidBody;
	};



	//動的オブジェクトにアタッチするRigidBodyDynamic class
	class RigidBodyDynamic final : public BaseClass::BaseRigidBody
	{
	public:
		//----------------------------------------------------------------------------------
		//[AddForce]
		//RigidBodyに外力を与える
		//引数1: force
		//引数2: force mode, default = force
		//引数3: sleep状態時自動で起こすか否か, default = true
		inline void AddForce(Vector3 force, ForceMode::Enum forceMode = ForceMode::Force, bool isAutoAwake = true)
		{
			using namespace Detail::Physics;

			//message作成
			PhysicsMessage message(MessageType::RigidBodyDynamicMessageFlag
				| MessageType::RigidBodyDynamicAddForceFlag | MessageType::RigidAddForceFO, instanceID());
			message.attachment.force.force = force;
			message.attachment.force.forceMode = forceMode;
			message.attachment.force.isAutoAwake = isAutoAwake;
			//send message
			Physics::SendPhysicsMessage(message);
		}
		//----------------------------------------------------------------------------------
		//[ClearForce]
		//現在与えられている外力を解除する
		inline void ClearForce() { if (IS_FALSE(m_isRegisterScene)) return; Physics::LockGuardPhysics guard(true); m_rigidBody->clearForce(); }

		//----------------------------------------------------------------------------------
		//[AddTorque]
		//RigidBodyにトルク(回転)を与える
		//引数1: トルク(回転)
		//引数2: force mode, default = force
		//引数3: sleep状態時自動で起こすか否か, default = true
		void AddTorque(Vector3 torque, ForceMode::Enum forceMode = ForceMode::Force, bool isAutoAwake = true)
		{
			using namespace Detail::Physics;

			//message作成
			PhysicsMessage message(MessageType::RigidBodyDynamicMessageFlag
				| MessageType::RigidBodyDynamicAddForceFlag | MessageType::RigidAddTorqueFO, instanceID());
			message.attachment.force.force = torque;
			message.attachment.force.forceMode = forceMode;
			message.attachment.force.isAutoAwake = isAutoAwake;
			//send message
			Physics::SendPhysicsMessage(message);
		}
		//----------------------------------------------------------------------------------
		//[ClearForce]
		//現在与えられているトルクを解除する
		void ClearTorque() { if (IS_FALSE(m_isRegisterScene)) return; Physics::LockGuardPhysics guard(true); m_rigidBody->clearTorque(); }

		//----------------------------------------------------------------------------------
		//[WakeUp]
		//Rigid Bodyがスリープ状態の場合起床させる
		void WakeUp() { Physics::LockGuardPhysics guard(true); m_rigidBody->wakeUp(); }

		//now linear velocity (read function property)
		SGF_FUNCTION_PROPERTY Vector3 getVelocity() 
		{
			if (IS_FALSE(m_isRegisterScene)) return Const::Vector3::zero;
			Physics::LockGuardPhysics guard(false);
			return m_rigidBody->getLinearVelocity(); 
		}
		//now linear velocity (set function property)
		//argument 1: set velocity
		//argument 2: auto WakeUp, default = true
		SGF_FUNCTION_PROPERTY Vector3 setVelocity(const Vector3& set, bool isAutoAwake = true)
		{
			if (IS_FALSE(m_isRegisterScene)) return Const::Vector3::zero;
			Physics::LockGuardPhysics guard(true);
			m_rigidBody->setLinearVelocity(set.ToPhysX(), isAutoAwake); return set;
		}

		//max linear velocity speed (read function property)
		SGF_FUNCTION_PROPERTY float getMaxSpeed() 
		{
			if (IS_FALSE(m_isRegisterScene)) return 0.0f; 
			Physics::LockGuardPhysics guard(false);
			return m_rigidBody->getMaxLinearVelocity(); 
		}
		//max linear velocity speed (set function property)
		//argument 1: set max linear velocity speed
		SGF_FUNCTION_PROPERTY float setMaxSpeed(float set)
		{
			if (IS_FALSE(m_isRegisterScene)) return 0.0f;
			Physics::LockGuardPhysics guard(true);
			m_rigidBody->setMaxLinearVelocity(set);
			return set;
		}

		//linear velocity damping(減衰係数) (read function property)
		SGF_FUNCTION_PROPERTY float getVelocityDamping() 
		{
			if (IS_FALSE(m_isRegisterScene)) return 0.0f; 
			Physics::LockGuardPhysics guard(false); 
			return m_rigidBody->getLinearDamping(); 
		}
		//linear velocity damping(減衰係数) (set function property)
		//argument 1: set linear velocity damping
		SGF_FUNCTION_PROPERTY float setVelocityDamping(float set)
		{
			if (IS_FALSE(m_isRegisterScene)) return 0.0f;
			Physics::LockGuardPhysics guard(true);
			m_rigidBody->setLinearDamping(set);
			return set;
		}

		//now angular velocity (read function property)
		SGF_FUNCTION_PROPERTY Vector3 getAngularVelocity()
		{
			if (IS_FALSE(m_isRegisterScene)) return Const::Vector3::zero;
			Physics::LockGuardPhysics guard(false);
			return m_rigidBody->getAngularVelocity();
		}
		//now angular velocity (set function property)
		//argument 1: set angular velocity
		//argument 2: auto WakeUp, default = true
		SGF_FUNCTION_PROPERTY Vector3 setAngularVelocity(const Vector3& set, bool isAutoAwake = true)
		{
			if (IS_FALSE(m_isRegisterScene)) return Const::Vector3::zero;
			Physics::LockGuardPhysics guard(true);
			m_rigidBody->setAngularVelocity(set.ToPhysX(), isAutoAwake);
			return set;
		}
		
		//max angular velocity speed (read function property)
		SGF_FUNCTION_PROPERTY float getMaxAngularSpeed() { if (IS_FALSE(m_isRegisterScene)) return 0.0f; Physics::LockGuardPhysics guard(false); return m_rigidBody->getMaxAngularVelocity(); }
		//max angular velocity speed (set function property)
		//argument 1: set max angular velocity speed
		SGF_FUNCTION_PROPERTY float setMaxAngularSpeed(float set)
		{
			if (IS_FALSE(m_isRegisterScene)) return 0.0f;
			Physics::LockGuardPhysics guard(true);
			m_rigidBody->setMaxAngularVelocity(set);
			return set;
		}

		//angular velocity damping(減衰係数) (read function property)
		SGF_FUNCTION_PROPERTY float getAngularVelocityDamping()
		{
			if (IS_FALSE(m_isRegisterScene)) return 0.0f;
			Physics::LockGuardPhysics guard(false);
			return m_rigidBody->getAngularDamping();
		}
		//angular velocity damping(減衰係数) (set function property)
		//argument 1: set angular velocity damping
		SGF_FUNCTION_PROPERTY float setAngularVelocityDamping(float set)
		{
			if (IS_FALSE(m_isRegisterScene)) return 0.0f;
			Physics::LockGuardPhysics guard(true);
			m_rigidBody->setAngularDamping(set);
			return set;
		}

		//now mass (read function property)
		SGF_FUNCTION_PROPERTY float getMass() 
		{
			if (IS_FALSE(m_isRegisterScene)) return 0.0f; 
			Physics::LockGuardPhysics guard(false); 
			return m_rigidBody->getMass(); 
		}
		//now mass (set function property)
		//argument 1: set mass
		SGF_FUNCTION_PROPERTY float setMass(float set)
		{
			if (IS_FALSE(m_isRegisterScene)) return 0.0f; 
			Physics::LockGuardPhysics guard(true);
			m_rigidBody->setMass(set);
			return set;
		}

		//now lock flags (read function property)
		SGF_FUNCTION_PROPERTY byte getLockFlags() 
		{
			Physics::LockGuardPhysics guard(false);
			return m_rigidBody->getRigidDynamicLockFlags();
		}
		//now lock flags (set function property)
		//argument 1: RigidBodyLock flag
		//argument 2: set value
		SGF_FUNCTION_PROPERTY RigidBodyLock::Enum setLockFlag(RigidBodyLock::Enum flag, bool set)
		{
			using namespace Detail::Physics;

			//message作成
			PhysicsMessage message(MessageType::RigidBodyDynamicMessageFlag
				| MessageType::RigidBodyDynamicOtherFlag | MessageType::RigidSetLockFlagsLO, instanceID());
			message.attachment.lock.flag = flag;
			message.attachment.lock.isSet = set;
			//send message
			Physics::SendPhysicsMessage(message);
			return flag;
		}
		//now lock flags (set function property)
		//argument 1: RigidBodyLock flag (or multiple flags)
		SGF_FUNCTION_PROPERTY byte setLockFlags(byte flags)
		{
			using namespace Detail::Physics;

			//message作成
			PhysicsMessage message(MessageType::RigidBodyDynamicMessageFlag 
				| MessageType::RigidBodyDynamicOtherFlag | MessageType::RigidSetLockFlagsBY, instanceID());
			message.attachment.byteData = flags;
			//send message
			Physics::SendPhysicsMessage(message);
			return flags;
		}

		//this kinematic setting? (read function property)
		SGF_FUNCTION_PROPERTY bool getIsKinematic() { if (IS_FALSE(m_isRegisterScene)) return false; Physics::LockGuardPhysics guard(false); return m_isKinematic; }
		//this kinematic setting? (set function property)
		//argument 1: set value
		SGF_FUNCTION_PROPERTY bool setIsKinematic(bool set)
		{
			if (IS_FALSE(m_isRegisterScene)) return false;

			Physics::LockGuardPhysics guard(true);
			//設定
			m_isKinematic = set;
			m_rigidBody->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, set);
			if (IS_TRUE(m_isKinematic))
			{
				//CCDとは併用できない
				m_isCCD = false;			
				//kinematic更新
				m_rigidBody->setKinematicTarget(
					physx::PxTransform(transform->getWorldPosition().ToPhysX(), transform->getWorldRotation().ToPhysX()));
			}
			return set;
		}

		//this ccd setting?(高速でも検知します) (read function property)
		SGF_FUNCTION_PROPERTY bool getIsCCD() { if (IS_FALSE(m_isRegisterScene)) return false; Physics::LockGuardPhysics guard(false); return m_isCCD; }
		//this ccd setting?(高速でも検知します) (set function property)
		//argument 1: set value
		SGF_FUNCTION_PROPERTY bool setIsCCD(bool set)
		{
			if (IS_FALSE(m_isRegisterScene)) return false;

			Physics::LockGuardPhysics guard(true);
			//設定
			m_isCCD = set;
			m_rigidBody->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_CCD, set);
			//Kinematicとは併用できない
			if (IS_TRUE(m_isCCD)) m_isKinematic = false;
			return set;
		}

		//this sleeping? (read function property)
		SGF_FUNCTION_PROPERTY bool getIsSleep() { if (IS_FALSE(m_isRegisterScene)) return false;  Physics::LockGuardPhysics guard(false); return m_rigidBody->isSleeping(); }
		//this sleeping? (set function property)
		//kinematic には無効, 現在の値を返却
		SGF_FUNCTION_PROPERTY bool setIsSleep(bool set)
		{
			if (IS_FALSE(m_isRegisterScene)) return false;

			Physics::LockGuardPhysics guard(true);
			//kinematicには無効
			if (IS_TRUE(m_isKinematic)) return m_rigidBody->isSleeping();

			//now sleeping
			bool isNowResult = m_rigidBody->isSleeping();
			//true->false = sleep
			if ((set ^ isNowResult) & isNowResult) m_rigidBody->putToSleep();
			//false->true = wakeUp
			else if (((set ^ isNowResult) & set)) m_rigidBody->wakeUp();

			return set;
		}

		//physx pointer (read function property)
		SGF_FUNCTION_PROPERTY physx::PxRigidDynamic& getPhysX() { return *m_rigidBody; }

	private:
		//----------------------------------------------------------------------------------
		//[UpdatePhysics]
		//Physicsから呼び出される更新関数
		inline void UpdatePhysics() override
		{	
			static const physx::PxRigidDynamicLockFlags cAllLockVelocity =
				physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_X | 
				physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Y |
				physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Z;

			static const physx::PxRigidDynamicLockFlags cAllLockRotation =
				physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X |
				physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X |
				physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X;

			//kinematicでなければtransformに反映
			if (IS_FALSE(m_isKinematic))
			{
				//現在座標取得
				auto pxtransform = m_rigidBody->getGlobalPose();
				//Flags
				auto flags = m_rigidBody->getRigidDynamicLockFlags();

				if ((flags & cAllLockVelocity) != cAllLockVelocity)
				{
					if ((flags & cAllLockRotation) != cAllLockRotation)
					{
						transform->setWorldPosition(pxtransform.p);
						transform->setWorldRotation(pxtransform.q);
					}
					else
					{
						transform->setWorldPosition(pxtransform.p);
						m_rigidBody->setGlobalPose(physx::PxTransform(pxtransform.p, transform->getWorldRotation().ToPhysX()));
					}
				}
				else if ((flags & cAllLockRotation) != cAllLockRotation)
				{
					transform->setWorldRotation(pxtransform.q);
					m_rigidBody->setGlobalPose(physx::PxTransform(transform->getWorldPosition().ToPhysX(), pxtransform.q));
				}
				else
				{
					m_rigidBody->setGlobalPose(physx::PxTransform(transform->getWorldPosition().ToPhysX(), transform->getWorldRotation().ToPhysX()));
				}

				if ((m_rigidBody->getRigidDynamicLockFlags() & cAllLockRotation) != cAllLockRotation)
					transform->setWorldRotation(pxtransform.q);
			}
			//kinematicならrigid bodyに反映
			else
			{
				m_rigidBody->setKinematicTarget(
					physx::PxTransform(transform->getWorldPosition().ToPhysX(), transform->getWorldRotation().ToPhysX()));
			}
		}
		//----------------------------------------------------------------------------------
		//[BuildRigidBody]
		//RigidBody作成
		inline void BuildRigidBody() override
		{
			auto shape = DefaultShape();

			auto pxTransform = physx::PxTransform(transform->getWorldPosition().ToPhysX(), transform->getWorldRotation().ToPhysX());
			if (IS_FALSE(pxTransform.isValid()))
			{
				pxTransform.q = Const::Quaternion::identity.ToPhysX();
				if (IS_FALSE(pxTransform.isValid()))
					pxTransform.p = Const::Vector3::zero.ToPhysX();
			}

			//create rigid body
			m_rigidBody = physx::PxCreateDynamic(*Physics::physicsFactory, pxTransform, *shape, 10.0f);

			shape->release();
		}

		//----------------------------------------------------------------------------------
		//[EnabledCallbacks]
		//return: 自身がCallされるCallback Flags, Component::CallbackFunctions参照
		inline uint EnableCallbacks() override { return CallbackFunctions::Awake | CallbackFunctions::OnDestroy; }
		//----------------------------------------------------------------------------------
		//[Awake]
		//インスタンス化した瞬間に呼ばれるCallback関数
		void Awake() override
		{
			using namespace Detail::Physics;

			//not static
			m_isStatic = false;			
			//false setting
			m_isKinematic = false;
			m_isCCD = false;

			//message作成
			PhysicsMessage message(MessageType::PointerMessageFlag | MessageType::RegisterRigidBodyRG, instanceID());
			message.attachment.rigidBody = m_thisPointer.DynamicCast<BaseClass::BaseRigidBody>();
			//send message
			Physics::SendPhysicsMessage(message);
		}

		//base rigid actor (read function property)
		SGF_FUNCTION_PROPERTY physx::PxRigidActor* getBaseActor() override { return m_rigidBody; }

		//rigid body
		physx::PxRigidDynamic* m_rigidBody;
		//this kinematic setting?
		bool m_isKinematic;
		//this ccd setting?
		bool m_isCCD;
	};
}
#endif // !SGFRAMEWORK_HEADER_RIGID_BODY_HPP_