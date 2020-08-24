/*----------------------------------------------------------------------------------
Physics Messageを扱うPhysicsMessage structure
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_PHYSICS_MESSAGE_HPP_
#define SGFRAMEWORK_HEADER_PHYSICS_MESSAGE_HPP_
#include <xaudio2.h>
#include "../../02_CustomVariable/CustomVariable.hpp"
#include "PhysicsMessagePack.hpp"

// Framework namespace
namespace SGFramework
{
	//Base Classes
	namespace BaseClass
	{
		class BaseRigidBody;
		class BaseCollider;
	}

	//パーツ
	namespace Detail
	{
		//Physics
		namespace Physics
		{
			//Physics Messageを扱うPhysicsMessage structure
			struct PhysicsMessage
			{
				//付帯情報を格納するAttachment union
				union Attachment
				{
					inline Attachment() { ZeroMemory(this, sizeof(Attachment)); }
					inline ~Attachment() {}

					//rigid body pointer
					WeakPointer<BaseClass::BaseRigidBody> rigidBody;
					//collider pointer
					WeakPointer<BaseClass::BaseCollider> collider;
					PackForce force;			//using add force and torque
					PackVelocity velocity;	//using set velocity
					PackLock lock;			//using set lock flags
					PackCapsule capsule;	//using capsule messages
					//using TransferRigidBody message
					PackTransferRigidBody transferRigidBody;
					Vector3 vector3;			//vector3 data
					float floatData;			//float data
					byte byteData;			//byte data
					bool booleanData;		//bool data
				};

				//[コンストラクタ]
				inline PhysicsMessage(uint messageType, uint instanceID)
					: attachment(), type(messageType), instanceID(instanceID) {}
				//[コンストラクタ]->memcpy
				inline PhysicsMessage(const PhysicsMessage& copy)
				{
					memcpy_s(this, sizeof(PhysicsMessage), &copy, sizeof(PhysicsMessage));
				}
				//operator = ->memcpy
				inline const PhysicsMessage& operator = (const PhysicsMessage& copy)
				{
					memcpy_s(this, sizeof(PhysicsMessage), &copy, sizeof(PhysicsMessage));
					return *this;
				}
				//----------------------------------------------------------------------------------
				//[Release]
				//Pointerなどが付帯されている場合開放する
				inline void Release();

				Attachment attachment;			//付帯情報
				uint type;								//message type
				uint instanceID;						//instance id
			};
			//56バイトに収めたい。絶対。
			static constexpr size_t cMessageSize = sizeof(PhysicsMessage);

			//----------------------------------------------------------------------------------
			//[Release]
			//Pointerなどが付帯されている場合開放する
			inline void PhysicsMessage::Release()
			{
				if (type == (MessageType::PointerMessageFlag | MessageType::RegisterRigidBodyRG))
					attachment.rigidBody.Reset();
				else if (type == (MessageType::PointerMessageFlag | MessageType::RegisterColliderCL))
					attachment.collider.Reset();
			}
		}
	}
}
#endif //!SGFRAMEWORK_HEADER_PHYSICS_MESSAGE_HPP_