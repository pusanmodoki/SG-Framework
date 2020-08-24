/*----------------------------------------------------------------------------------
Hit Callback時に使用するContactInfo structure
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_CONTACT_INFO_HPP_
#define SGFRAMEWORK_HEADER_CONTACT_INFO_HPP_
#include "../02_CustomVariable/CustomVariable.hpp"
#include "../05_Graphics/TransformUsing.hpp"
#include "RigidBody/RigidBody.hpp"
#include "Collider/BaseCollider.hpp"

//Framework namespace
namespace SGFramework
{
	//Hit Callback時に使用するContactInfo structure
	struct ContactInfo final
	{
	public:
		//friend
		friend class Physics;

		DELETE_ALL_DEFAULT_CLASS_FUNCTIONS(ContactInfo);
		
		//hit object->transform
		const WeakPointer<Transform> cHitTransform;
		//hit object->rigid body
		const WeakPointer<BaseClass::BaseRigidBody> cHitRigidBody;
		//hit object->hit collider
		const WeakPointer<BaseClass::BaseCollider> cHitCollider;
		//my object->hit collider
		const WeakPointer<BaseClass::BaseCollider> cMyCollider;
		//hit object->transform instanceID
		const uint cHitInstanceID;
		//weak pointers is valid?
		const bool cIsValidPointers;
		//hit object->hit collider is trigger?
		const bool cIsHitTrigger;
		//hit object->rigid body is dynamic?
		const bool cIsHitRigidDynamic;

	private:
		//all setting constructor
		inline ContactInfo(const Physics::ActorInfo* actorInfo, const Physics::HitPair* pair, bool isUsed0,
			const WeakPointer<BaseClass::BaseCollider>& ifActorInfoNullMyCollider = WeakPointer<BaseClass::BaseCollider>())
			: cHitTransform(actorInfo ? actorInfo->transform : WeakPointer<Transform>()),
			cHitRigidBody(actorInfo ? actorInfo->rigidBody : WeakPointer<BaseClass::BaseRigidBody>()),
			cHitCollider(actorInfo && pair ? IS_TRUE(isUsed0) ? pair->collider1 : pair->collider0 : WeakPointer<BaseClass::BaseCollider>()),
			cMyCollider(actorInfo && pair ? IS_TRUE(isUsed0) ? pair->collider0 : pair->collider1 : ifActorInfoNullMyCollider),
			cHitInstanceID(pair ? IS_TRUE(isUsed0) ? pair->pairInstanceID1 : pair->pairInstanceID0 : false),
			cIsValidPointers(actorInfo ? true : false), 
			cIsHitTrigger(pair ? IS_TRUE(isUsed0) ? pair->isTrigger1 : pair->isTrigger0 : false),
			cIsHitRigidDynamic(actorInfo ? actorInfo->isDynamic : false) {}
	};
}

#endif //!SGFRAMEWORK_HEADER_CONTACT_INFO_HPP_