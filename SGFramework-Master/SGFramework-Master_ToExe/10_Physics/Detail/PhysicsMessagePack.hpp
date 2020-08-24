/*----------------------------------------------------------------------------------
物理エンジン(PhysX)を管理するPhysics class
	->Message内のUnionで使用する構造体等を定義するPhysicsMessagePack.hpp
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_PHYSICS_MESSAGE_PACK_HPP_
#define SGFRAMEWORK_HEADER_PHYSICS_MESSAGE_PACK_HPP_
#include "../../02_CustomVariable/CustomVariable.hpp"

//Framework namespace
namespace SGFramework
{
	//パーツ
	namespace Detail
	{
		//Physics
		namespace Physics
		{
			//Messageの種類を定義したMessageType enum一般的用途
			//メッセージ種類が多すぎる & 汎用変数を多用するため、汎用変数を使用する各メンバの最後に使用変数型を記載
			//rigidBody -> RG, collider -> CL, force -> FO, velocity -> VL, lock = LO, capsule = CP
			//transferRigidBody -> TR vector3 -> V3, floatData = FL, byteData = BY, booleanData = BO
			struct MessageType
			{
				enum Enum
				{
					//base Collider Message Flag
					BaseColliderMessageFlag = 0x00100000,
					SetTriggerBO = 0x1,					//set Is Trigger
					TransferRigidBodyTR = 0x2,		//transfer rigid body

					//box Collider Message Flag
					BoxColliderMessageFlag = 0x00200000,
					BoxSetScaleV3 = 0x1,				//set Scale

					//capsule Collider Message Flag
					CapsuleColliderMessageFlag = 0x00400000,
					CapsuleSetValuesCP = 0x1,			//set values

					//mesh Collider Message Flag (Not use, only reservation)
					MeshColliderMessageFlag = 0x00800000,

					//plane Collider Message Flag (Not use, only reservation)
					PlaneColliderMessageFlag = 0x01000000,

					//sphere Collider Message Flag
					SphereColliderMessageFlag = 0x02000000,
					SphereSetRadiusFL = 0x1,			//set radius

					//base Rigid Body Message Flag
					BaseRigidBodyMessageFlag = 0x04000000,
					RigidWarp = 0x1,						//warp

					//base Rigid Body Message Flag (Not use, only reservation)
					RigidBodyStaticMessageFlag = 0x08000000,



					//rigid Body Dynamic Message Flag
					RigidBodyDynamicMessageFlag = 0x10000000,	
					//add force flag
					RigidBodyDynamicAddForceFlag = 0x10000,
					RigidAddForceFO = 0x1,				//add force
					RigidClearForce = 0x2,				//clear force
					RigidAddTorqueFO = 0x4,			//add torque
					RigidClearTorque = 0x8,				//clear torque
					
					//set velocity flag
					RigidBodyDynamicVelocityFlag = 0x20000,
					RigidSetVelocityVL = 0x01,			//set velocity
					RigidSetMaxSpeedFO = 0x02,		//set velocity max speed
					RigidSetDampingFO = 0x04,		//set velocity damping
					//set anguler velocity
					RigidSetAngulerVelocityVL = 0x08,
					//set anguler velocity max speed
					RigidSetAngulerMaxSpeedFO = 0x10,
					//set anguler velocity damping
					RigidSetAngulerDampingFO = 0x20,

					//other flag
					RigidBodyDynamicOtherFlag = 0x40000,
					RigidSetLockFlagsLO = 0x01,		//set lock flags (use lock)
					RigidSetLockFlagsBY = 0x02,		//set lock flags (use byte)
					RigidSetMassFL = 0x04,				//set mass
					RigidSetKinematicBO = 0x08,		//set Is Kinematic
					RigidSetCcdBO = 0x10,				//set Is CCD
					RigidSetSleepBO = 0x20,			//set Is Sleep
					RigidWakeUp = 0x40,					//wake up

					//register Pointer Message Flag
					PointerMessageFlag = 0x20000000,
					RegisterRigidBodyRG = 0x01,		//register Rigid Body
					RegisterColliderCL = 0x02,			//register Collider
					UnregisterRigidBody = 0x04,		//unregister Rigid Body
					UnregisterCollider = 0x08,			//unregister Collider
				};
			};

			//AddForce OR AddTprque messageで使用するPackForce structure
			struct PackForce
			{
				Vector3 force;						//Force
				ForceMode::Enum forceMode;	//Force mode
				bool isAutoAwake;					//auto awake?
			};
			//SetVelocity OR SetAngulerVelocity messageで使用するPackVelocity structure
			struct PackVelocity
			{
				Vector3 velocity;					//Set velocity
				bool isAutoAwake;					//auto awake?
			};
			//SetLockFlags messageで使用するPackLock structure
			struct PackLock
			{
				RigidBodyLock::Enum flag;		//Lock flag
				bool isSet;								//set Value
			};
			//CapsuleCollider messagesで使用するPackCapsule structure
			struct PackCapsule
			{
				float height;	//height
				float radius;	//radius
			};
			//TransferRigidBody messageで使用するPackTransferRigidBody structure
			struct PackTransferRigidBody
			{
				physx::PxShape* shape;
				physx::PxRigidActor* oldActor;
				physx::PxRigidActor* newActor;
			};
		}
	}
}
#endif // !SGFRAMEWORK_HEADER_PHYSICS_MESSAGE_PACK_HPP_