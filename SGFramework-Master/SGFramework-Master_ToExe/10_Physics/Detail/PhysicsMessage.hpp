/*----------------------------------------------------------------------------------
Physics Message������PhysicsMessage structure
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

	//�p�[�c
	namespace Detail
	{
		//Physics
		namespace Physics
		{
			//Physics Message������PhysicsMessage structure
			struct PhysicsMessage
			{
				//�t�я����i�[����Attachment union
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

				//[�R���X�g���N�^]
				inline PhysicsMessage(uint messageType, uint instanceID)
					: attachment(), type(messageType), instanceID(instanceID) {}
				//[�R���X�g���N�^]->memcpy
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
				//Pointer�Ȃǂ��t�т���Ă���ꍇ�J������
				inline void Release();

				Attachment attachment;			//�t�я��
				uint type;								//message type
				uint instanceID;						//instance id
			};
			//56�o�C�g�Ɏ��߂����B��΁B
			static constexpr size_t cMessageSize = sizeof(PhysicsMessage);

			//----------------------------------------------------------------------------------
			//[Release]
			//Pointer�Ȃǂ��t�т���Ă���ꍇ�J������
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