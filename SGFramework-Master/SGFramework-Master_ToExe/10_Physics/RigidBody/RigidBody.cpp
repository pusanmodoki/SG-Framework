/*----------------------------------------------------------------------------------
RigidBody���`����RigidBody.cpp
------------------------------------------------------------------------------------*/
#include "RigidBody.hpp"
#include "../Collider/BaseCollider.hpp"

//Framework namespace
namespace SGFramework
{
	//Base Classes
	namespace BaseClass
	{
		//----------------------------------------------------------------------------------
		//[TransferColliders]
		//���g��RigidBody�ɕR�t���Ă���Collider��e�Ɉڂ�
		void BaseRigidBody::TransferColliders()
		{
			Physics::LockGuardPhysics guard(false);

			//���݂�Collider���擾, 0�Ȃ�I��
			uint numColliders = m_numAttachColliders.load();

			if (numColliders == 0 || IS_TRUE(transform->getIsDestroy())) return;

			//Collider components
			std::vector<WeakPointer<BaseClass::BaseCollider>*> colliders(numColliders);
			//New RigidBody (parent)
			WeakPointer<BaseClass::BaseRigidBody> newRigidBody;

			//�܂��͂����Ă���Collider���擾
			physx::PxShape** shapes = new physx::PxShape*[numColliders];
			getBaseActor()->getShapes(shapes, numColliders, 0);
			
			//Collider components�擾
			for (uint i = 0, max = getBaseActor()->getNbShapes(); i < max; ++i)
				colliders[i] = reinterpret_cast<WeakPointer<BaseClass::BaseCollider>*>(shapes[i]->userData);

			//Parent�������[�v
			for (Transform* parent = transform->m_parent.TryGetInstance();
				parent != nullptr || IS_FALSE(newRigidBody.getIsValid()); parent = parent->m_parent.TryGetInstance())
			{
				//static-> static rigid body�̂݌���
				if (IS_FALSE(m_isStatic))
					newRigidBody = parent->FindComponent<RigidBodyStatic>().DownCast<BaseClass::BaseRigidBody>();
				//not static-> static or dynamic rigid body������
				else
					newRigidBody = parent->FindComponent<BaseClass::BaseRigidBody>();
			}

			//�e�ɐV����RigidBody��������->Collider���ڂ�
			if (IS_TRUE(newRigidBody.getIsValid()))
			{
				for (auto& e : colliders)
					(*e)->TransferRigidBody(newRigidBody->transform, newRigidBody);
			}
			//�e�ɐV����RigidBody�͂��܂���->Component�폜
			else
			{
				for (auto& e : colliders)
					(*e)->transform->RemoveComponent<BaseClass::BaseCollider>();
			}
		}
	}
}