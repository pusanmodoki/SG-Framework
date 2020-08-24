/*----------------------------------------------------------------------------------
物理エンジン(PhysX)を管理するPhysics class
	→ProcessingMessage系関数を定義するPhysicsProcessingMessage.hpp
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_PHYSICS_PROCESSING_MESSAGE_HPP_
#define SGFRAMEWORK_HEADER_PHYSICS_PROCESSING_MESSAGE_HPP_
#include "../Physics.hpp"
#include "../Collider/BaseCollider.hpp"
#include "../RigidBody/RigidBody.hpp"

//Framework namespace
namespace SGFramework
{
	//----------------------------------------------------------------------------------
	//[ProcessingMessage]
	//メッセージを処理する
	//引数1: message
	inline void Physics::ProcessingMessage(const Detail::Physics::PhysicsMessage & message)
	{
		using namespace Detail::Physics;

		//かなりの数参照するのでとりあえず保存して使ってみる
		uint messageType = message.type;

		//-------------------------------------------------------------------------------
		//rigid body dynamic message
		if (messageType & MessageType::RigidBodyDynamicMessageFlag)
		{
			ProcessingRigidBodyDynamicMessage(message, messageType);
		}
		//-------------------------------------------------------------------------------
		//register pointer message
		else if (messageType & MessageType::PointerMessageFlag)
		{
			ProcessingPointerMessage(message, messageType);
		}
		//-------------------------------------------------------------------------------
		//other message
		else
		{
			ProcessingOtherMessage(message, messageType);
		}
	}


	//----------------------------------------------------------------------------------
	//[ProcessingRigidBodyDynamicMessage]
	//メッセージを処理する (RigidBodyDynamic)
	//引数1: message
		//引数1: message type
	inline void Physics::ProcessingRigidBodyDynamicMessage(const Detail::Physics::PhysicsMessage & message, uint messageType)
	{
		using namespace Detail::Physics;

		auto dynamic = m_actors.at(message.instanceID).actorDynamic;
		if (dynamic == nullptr) return;

		//-------------------------------------------------------------------------------
		//add force flag
		if (messageType & MessageType::RigidBodyDynamicAddForceFlag)
		{
			//add force
			if (messageType & MessageType::RigidAddForceFO)
			{
				dynamic->addForce(message.attachment.force.force.ToPhysX(),
					static_cast<physx::PxForceMode::Enum>(message.attachment.force.forceMode),
					message.attachment.force.isAutoAwake);
			}
			//add torque
			else if (messageType & MessageType::RigidAddTorqueFO)
			{
				dynamic->addTorque(message.attachment.force.force.ToPhysX(),
					static_cast<physx::PxForceMode::Enum>(message.attachment.force.forceMode),
					message.attachment.force.isAutoAwake);
			}
			//clear force
			else if (messageType & MessageType::RigidClearForce)
			{
				dynamic->clearForce();
			}
			//clear torque
			else if (messageType & MessageType::RigidClearTorque)
			{
				dynamic->clearTorque();
			}
		}
		//-------------------------------------------------------------------------------
		//set velocity flag
		else if (messageType & MessageType::RigidBodyDynamicVelocityFlag)
		{
			//set velocity
			if (messageType & MessageType::RigidSetVelocityVL)
			{
				dynamic->setLinearVelocity(message.attachment.velocity.velocity.ToPhysX(),
					message.attachment.velocity.isAutoAwake);
			}
			//set anguler velocity
			else if (messageType & MessageType::RigidSetAngulerVelocityVL)
			{
				dynamic->setAngularVelocity(message.attachment.velocity.velocity.ToPhysX(),
					message.attachment.velocity.isAutoAwake);
			}
			//set velocity max speed
			else if (messageType & MessageType::RigidSetMaxSpeedFO)
			{
				dynamic->setMaxLinearVelocity(message.attachment.floatData);
			}
			//set anguler velocity max speed
			else if (messageType & MessageType::RigidSetAngulerMaxSpeedFO)
			{
				dynamic->setMaxAngularVelocity(message.attachment.floatData);
			}
			//set velocity damping
			else if (messageType & MessageType::RigidSetDampingFO)
			{
				dynamic->setLinearDamping(message.attachment.floatData);
			}
			//set anguler velocity damping
			else if (messageType & MessageType::RigidSetAngulerDampingFO)
			{
				dynamic->setAngularDamping(message.attachment.floatData);
			}
		}
		//-------------------------------------------------------------------------------
		//other flag
		else if (messageType & MessageType::RigidBodyDynamicOtherFlag)
		{
			//set lock flags (use lock)
			if (messageType & MessageType::RigidSetLockFlagsLO)
			{
				dynamic->setRigidDynamicLockFlag(
					static_cast<physx::PxRigidDynamicLockFlag::Enum>(message.attachment.lock.flag),
					message.attachment.lock.flag);
			}
			//set lock flags (use byte)
			else if (messageType & MessageType::RigidSetLockFlagsBY)
			{
				dynamic->setRigidDynamicLockFlags(
					physx::PxRigidDynamicLockFlags(message.attachment.byteData));
			}
			//set mass
			else if (messageType & MessageType::RigidSetMassFL)
			{
				dynamic->setMass(message.attachment.floatData);
			}
			//set Is Kinematic
			else if (messageType & MessageType::RigidSetKinematicBO)
			{
				//CCDとは併用できない
				if (IS_TRUE(message.attachment.booleanData))
					dynamic->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_CCD, false);

				dynamic->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC,
					message.attachment.booleanData);
			}
			//set Is CCD
			else if (messageType & MessageType::RigidSetCcdBO)
			{
				//Kinematicとは併用できない
				if (IS_TRUE(message.attachment.booleanData))
					dynamic->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, false);

				dynamic->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_CCD,
					message.attachment.booleanData);
			}
			//set Is Sleep
			else if (messageType & MessageType::RigidSetSleepBO)
			{
				//kinematicには無効
				if (IS_TRUE(dynamic->getRigidBodyFlags().isSet(physx::PxRigidBodyFlag::eKINEMATIC)))
					return;

				//now sleeping
				bool isNowResult = dynamic->isSleeping();
				//true->false = sleep
				if ((message.attachment.booleanData ^ isNowResult) & isNowResult)
					dynamic->putToSleep();
				//false->true = wakeUp
				else if (((message.attachment.booleanData ^ isNowResult) & message.attachment.booleanData))
					dynamic->wakeUp();
			}
			//wake up
			else if (messageType & MessageType::RigidWakeUp)
			{
				dynamic->wakeUp();
			}
		}
	}


	//----------------------------------------------------------------------------------
	//[ProcessingPointerMessage]
	//メッセージを処理する (Pointer)
	//引数1: message
		//引数1: message type
	inline void Physics::ProcessingPointerMessage(const Detail::Physics::PhysicsMessage & message, uint messageType)
	{
		using namespace Detail::Physics;
	
		//register Rigid Body
		if (messageType & MessageType::RegisterRigidBodyRG)
		{
			auto pointer = message.attachment.rigidBody;
			if (IS_TRUE(pointer.getIsRevocatoin())) return;

			//Actorを追加
			pointer->BuildRigidBody();
			if (pointer->getBaseActor() == nullptr)
				return;

			m_scene->addActor(*pointer->getBaseActor());

			m_actors.try_emplace(message.instanceID,
				pointer->transform, pointer, pointer->getBaseActor(),
				pointer->getIsStatic() ? nullptr : reinterpret_cast<physx::PxRigidDynamic*>(pointer->getBaseActor()),
				IS_FALSE(pointer->getIsStatic()));

			if (IS_FALSE(pointer->getIsStatic()))
				m_dynamicActors.try_emplace(message.instanceID, m_actors.at(message.instanceID).rigidBody);
		}
		//register Collider
		else if (messageType & MessageType::RegisterColliderCL)
		{
			auto pointer = message.attachment.collider;
			if (IS_TRUE(pointer.getIsRevocatoin())) return;

			//Colliderを追加
			pointer->GetActor();
			pointer->CreateShape();
			if (pointer->m_shape == nullptr)
				return;

			m_colliders.try_emplace(message.instanceID, pointer, pointer->m_shape);
		}
		//unregister Rigid Body
		else if (messageType & MessageType::UnregisterRigidBody)
		{
			if (m_actors.find(message.instanceID) == m_actors.end())
				return;

			auto& at = m_actors.at(message.instanceID);

			//release後マップから削除
			at.actor->release();
			if (IS_TRUE(at.isDynamic)) m_dynamicActors.erase(message.instanceID);
			m_actors.erase(message.instanceID);
		}
		//unregister Collider
		else if (messageType & MessageType::UnregisterCollider)
		{
			if (m_colliders.find(message.instanceID) == m_colliders.end())
				return;
			
			auto& at = m_colliders.at(message.instanceID);

			//release後マップから削除
			if (at.shape->getActor())
				at.shape->getActor()->detachShape(*at.shape);

			at.shape->release();
			m_colliders.erase(message.instanceID);
		}
	}


	//----------------------------------------------------------------------------------
	//[ProcessingOtherMessage]
	//メッセージを処理する (Other)
	//引数1: message
	//引数1: message type
	inline void Physics::ProcessingOtherMessage(const Detail::Physics::PhysicsMessage & message, uint messageType)
	{
		using namespace Detail::Physics;
	
		//ここから先今は1つしかメッセージがないので直に実装, 下の方に将来のため本来の実装をコメントアウト
		//-------------------------------------------------------------------------------
		//base rigid body or collider message
		if (messageType & (MessageType::BaseRigidBodyMessageFlag | MessageType::BaseColliderMessageFlag))
		{
			//base rigid body message
			//warp
			if (messageType == (MessageType::BaseRigidBodyMessageFlag | MessageType::RigidWarp))
			{
				auto& at = m_actors.at(message.instanceID);

				if (at.transform.LockShared())
				{
					m_actors.at(message.instanceID).actor->setGlobalPose(physx::PxTransform(
						at.transform->getWorldPosition().ToPhysX(), at.transform->getWorldRotation().ToPhysX()));

					at.transform.UnlockShared();
				}
			}
			//base collider message
			//set Is Trigger
			else if (messageType == (MessageType::BaseColliderMessageFlag | MessageType::SetTriggerBO))
			{
				auto& at = m_colliders.at(message.instanceID);

				if (IS_FALSE(at.collider.LockShared()))
					return;

				//simulation shapeとtriggerは共存できない
				if (IS_TRUE(message.attachment.booleanData))
				{
					at.shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
					at.shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
				}
				else
				{
					at.shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, false);
					at.shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
				}
			}
			//base collider message
			//transfer rigid body
			else if (messageType == (MessageType::BaseColliderMessageFlag | MessageType::TransferRigidBodyTR))
			{
				//参照追加
				message.attachment.transferRigidBody.shape->acquireReference();
				//detach
				message.attachment.transferRigidBody.oldActor->detachShape(*message.attachment.transferRigidBody.shape);
				//attach
				message.attachment.transferRigidBody.newActor->attachShape(*message.attachment.transferRigidBody.shape);
				//参照減算
				message.attachment.transferRigidBody.shape->release();
			}
		}
		//-------------------------------------------------------------------------------
		//box collider message
		//set values
		else if (messageType == (MessageType::BoxColliderMessageFlag | MessageType::BoxSetScaleV3))
		{
			m_colliders.at(message.instanceID).shape->setGeometry(
				physx::PxBoxGeometry(message.attachment.vector3.ToPhysX()));
		}
		//-------------------------------------------------------------------------------
		//capsule collider message
		//set values
		else if (messageType == (MessageType::CapsuleColliderMessageFlag | MessageType::CapsuleSetValuesCP))
		{
			m_colliders.at(message.instanceID).shape->setGeometry(physx::PxCapsuleGeometry(
				message.attachment.capsule.radius, message.attachment.capsule.height));
		}
		//-------------------------------------------------------------------------------
		//sphere collider message
		//set radius
		else if (messageType == (MessageType::SphereColliderMessageFlag | MessageType::SphereSetRadiusFL))
		{
			auto& at = m_colliders.at(message.instanceID);
			at.shape->setGeometry(physx::PxSphereGeometry(message.attachment.floatData));
		}

		////base rigid body message
		//else if (messageType & MessageType::BaseRigidBodyMessageFlag)
		//{
		//	//warp
		//	if (messageType & MessageType::RigidWarp)
		//	{
		//		auto& at = m_actors.at(message.instanceID);

		//		if (at.transform.LockShared())
		//		{
		//			m_actors.at(message.instanceID).actor->setGlobalPose(physx::PxTransform(
		//				at.transform->getWorldPosition().ToPhysX(), at.transform->getWorldRotation().ToPhysX()));

		//			at.transform.UnlockShared();
		//		}
		//	}
		//}

		////base collider message
		//else if (messageType & MessageType::BaseColliderMessageFlag)
		//{
		//	//set Is Trigger
		//	if (messageType & MessageType::SetTriggerBO)
		//	{
		//		auto& at = m_colliders.at(message.instanceID);
		//		
		//		if (IS_FALSE(at.collider.LockShared()))
		//			return;

		//		//simulation shapeとtriggerは共存できない
		//		if (IS_TRUE(message.attachment.booleanData))
		//		{
		//			at.shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
		//			at.shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
		//		}
		//		else
		//		{
		//			at.shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, false);
		//			at.shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
		//		}
		//	}
		//}

		////box collider message
		//else if (messageType & MessageType::BoxColliderMessageFlag)
		//{
		//	//set values
		//	if (messageType & MessageType::BoxSetScaleV3)
		//	{
		//		m_colliders.at(message.instanceID).shape->setGeometry(
		//			physx::PxBoxGeometry(message.attachment.vector3.ToPhysX()));
		//	}
		//}

		////capsule collider message
		//else if (messageType & MessageType::CapsuleColliderMessageFlag)
		//{
		//	//set values
		//	if (messageType & MessageType::CapsuleSetValuesCP)
		//	{
		//		m_colliders.at(message.instanceID).shape->setGeometry(physx::PxCapsuleGeometry(
		//			message.attachment.capsule.radius, message.attachment.capsule.height));
		//	}
		//}

		////sphere collider message
		//else if (messageType & MessageType::SphereColliderMessageFlag)
		//{
		//	//set radius
		//	if (messageType & MessageType::SphereSetRadiusFL)
		//	{
		//		auto& at = m_colliders.at(message.instanceID);
		//		at.shape->setGeometry(physx::PxSphereGeometry(message.attachment.floatData));
		//	}
		//}
	}
}

#endif //!SGFRAMEWORK_HEADER_PHYSICS_PROCESSING_MESSAGE_HPP_