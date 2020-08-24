/*----------------------------------------------------------------------------------
RigidBodyを定義するRigidBody.cpp
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
		//自身のRigidBodyに紐付いているColliderを親に移す
		void BaseRigidBody::TransferColliders()
		{
			Physics::LockGuardPhysics guard(false);

			//現在のCollider数取得, 0なら終了
			uint numColliders = m_numAttachColliders.load();

			if (numColliders == 0 || IS_TRUE(transform->getIsDestroy())) return;

			//Collider components
			std::vector<WeakPointer<BaseClass::BaseCollider>*> colliders(numColliders);
			//New RigidBody (parent)
			WeakPointer<BaseClass::BaseRigidBody> newRigidBody;

			//まずはもっているColliderを取得
			physx::PxShape** shapes = new physx::PxShape*[numColliders];
			getBaseActor()->getShapes(shapes, numColliders, 0);
			
			//Collider components取得
			for (uint i = 0, max = getBaseActor()->getNbShapes(); i < max; ++i)
				colliders[i] = reinterpret_cast<WeakPointer<BaseClass::BaseCollider>*>(shapes[i]->userData);

			//Parent検索ループ
			for (Transform* parent = transform->m_parent.TryGetInstance();
				parent != nullptr || IS_FALSE(newRigidBody.getIsValid()); parent = parent->m_parent.TryGetInstance())
			{
				//static-> static rigid bodyのみ検索
				if (IS_FALSE(m_isStatic))
					newRigidBody = parent->FindComponent<RigidBodyStatic>().DownCast<BaseClass::BaseRigidBody>();
				//not static-> static or dynamic rigid bodyを検索
				else
					newRigidBody = parent->FindComponent<BaseClass::BaseRigidBody>();
			}

			//親に新しいRigidBodyを見つけた->Colliderを移す
			if (IS_TRUE(newRigidBody.getIsValid()))
			{
				for (auto& e : colliders)
					(*e)->TransferRigidBody(newRigidBody->transform, newRigidBody);
			}
			//親に新しいRigidBodyはいません->Component削除
			else
			{
				for (auto& e : colliders)
					(*e)->transform->RemoveComponent<BaseClass::BaseCollider>();
			}
		}
	}
}