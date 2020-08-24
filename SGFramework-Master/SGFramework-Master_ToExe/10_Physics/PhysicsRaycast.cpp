/*----------------------------------------------------------------------------------
�����G���W��(PhysX)���Ǘ�����Physics class
	��Raycast�n�֐����`����PhysicsRaycast.cpp
------------------------------------------------------------------------------------*/
#include "../07_Thread/Thread.hpp"
#include "Physics.hpp"
#include "RigidBody/RigidBody.hpp"
#include "ContactInfo.hpp"
#include "RaycastHit/RaycastHit.hpp"
#include "RaycastHit/OverlapHit.hpp"
#include "RaycastHit/SweepHit.hpp"

namespace SGFramework
{
	//Collision Filter Data
	//world0 = this layer
	//world1 = layerMask
	//world2 = used callback = 1
	//world3 = transform instance id
	//Query Filter Data
	//world0 = layerMask
	//world1 = exclude instance id

	//----------------------------------------------------------------------------------
	//[Raycast]
	//Ray�œ����蔻����s��
	//����1: ���_
	//����2: ����
	//����3: ����, default = infinity
	//����4: RaycastHit, nullptr�łȂ��ꍇ��Hit����������, default = nullptr
	//����4: LayerMask, default = all layer mask
	//����5: true = ray�ɓ�����������S�ĕԋp
	bool Physics::Raycast(const Vector3& origin, const Vector3& direction, float distance,
		RaycastHit* raycastHit, const LayerMask& layerMask, bool isMultiHit)
	{
		physx::PxRaycastBuffer buffer;
		physx::PxHitFlags hitFlag = physx::PxHitFlag::eDEFAULT | physx::PxHitFlag::eUV;

		//Multi Hit
		if (IS_TRUE(isMultiHit)) 
			hitFlag |= physx::PxHitFlag::eMESH_MULTIPLE;

		//PhysX->Raycast
		if (IS_TRUE(m_scene->raycast(origin.ToPhysX(), direction.ToPhysX(), distance, buffer, hitFlag,
			physx::PxQueryFilterData(physx::PxFilterData(layerMask(), isMultiHit ? 1 : 0, 0, 0),
				(physx::PxQueryFlag::eDYNAMIC | physx::PxQueryFlag::ePREFILTER | physx::PxQueryFlag::eSTATIC)), &m_queryCallback)))
		{
			//raycastHit���
			if (raycastHit != nullptr) raycastHit->Init(buffer);
			return true;
		}
		else return false;
	}

	//----------------------------------------------------------------------------------
	//[BoxCast]
	//BoxRay�œ����蔻����s��
	//����1: ���_
	//����2: ����
	//����3: �T�C�Y
	//����4: ��]
	//����5: ����, default = infinity
	//����6: SweepHit, nullptr�łȂ��ꍇ��Hit����������, default = nullptr
	//����7: LayerMask, default = all layer mask
	bool Physics::BoxCast(const Vector3 & origin, const Vector3 & direction, const Vector3 & size, 
		const Quaternion& rotation, float distance, SweepHit* hit, const LayerMask& layerMask)
	{
		physx::PxSweepBuffer buffer;

		//Sweep
		if (IS_TRUE(m_scene->sweep(physx::PxBoxGeometry((size * 0.5f).ToPhysX()), physx::PxTransform(origin.ToPhysX(), rotation.ToPhysX()),
			direction.ToPhysX(), distance, buffer, physx::PxHitFlag::eDEFAULT | physx::PxHitFlag::eUV,
			physx::PxQueryFilterData(physx::PxFilterData(layerMask(), 0, 0, 0),
				physx::PxQueryFlag::ePREFILTER | physx::PxQueryFlag::eDYNAMIC | physx::PxQueryFlag::eSTATIC), &m_queryCallback)))
		{
			//sweepHit���
			if (hit != nullptr) hit->Init(buffer);
			return true;
		}
		else return false;
	}
	//----------------------------------------------------------------------------------
	//[SphereCast]
	//SphereRay�œ����蔻����s��
	//����1: ���_
	//����2: ����
	//����3: ���a
	//����4: ����, default = infinity
	//����5: SweepHit, nullptr�łȂ��ꍇ��Hit����������, default = nullptr
	//����6: LayerMask, default = all layer mask
	bool Physics::SphereCast(const Vector3& origin, const Vector3& direction, float radius,
		float distance, SweepHit* hit, const LayerMask& layerMask)
	{
		physx::PxSweepBuffer buffer;

		//Sweep
		if (IS_TRUE(m_scene->sweep(physx::PxSphereGeometry(radius), physx::PxTransform(origin.ToPhysX(), Const::Quaternion::identity.ToPhysX()),
			direction.ToPhysX(), distance, buffer, physx::PxHitFlag::eDEFAULT | physx::PxHitFlag::eUV,
			physx::PxQueryFilterData(physx::PxFilterData(layerMask(), 0, 0, 0),
				physx::PxQueryFlag::ePREFILTER | physx::PxQueryFlag::eDYNAMIC | physx::PxQueryFlag::eSTATIC), &m_queryCallback)))
		{
			//sweepHit���
			if (hit != nullptr) hit->Init(buffer);
			return true;
		}
		else return false;
	}
	//----------------------------------------------------------------------------------
	//[CapsuleCast]
	//CapsuleRay�œ����蔻����s��
	//����1: ���_
	//����2: ����
	//����3: ���a
	//����4: ����
	//����5: ��]
	//����6: ����, default = infinity
	//����7: SweepHit, nullptr�łȂ��ꍇ��Hit����������, default = nullptr
	//����8: LayerMask, default = all layer mask
	bool Physics::CapsuleCast(const Vector3& origin, const Vector3& direction, float radius, 
		float height, const Quaternion& rotation, float distance, SweepHit* hit, const LayerMask& layerMask)
	{
		physx::PxSweepBuffer buffer;

		//Sweep
		if (IS_TRUE(m_scene->sweep(physx::PxCapsuleGeometry(radius, height * 0.5f), physx::PxTransform(origin.ToPhysX(), rotation.ToPhysX()),
			direction.ToPhysX(), distance, buffer, physx::PxHitFlag::eDEFAULT | physx::PxHitFlag::eUV,
			physx::PxQueryFilterData(physx::PxFilterData(layerMask(), 0, 0, 0),
				physx::PxQueryFlag::ePREFILTER | physx::PxQueryFlag::eDYNAMIC | physx::PxQueryFlag::eSTATIC), &m_queryCallback)))
		{
			//sweepHit���
			if (hit != nullptr) hit->Init(buffer);
			return true;
		}
		else return false;
	}
	//----------------------------------------------------------------------------------
	//[OverlapBox]
	//Box�ɃI�u�W�F�N�g���ڐG���Ă��邩�m�F����
	//����1: ���W
	//����2: ��]
	//����3: �T�C�Y
	//����4: OverlapHit, nullptr�łȂ��ꍇ��Hit����������, default = nullptr
	//����5: LayerMask, default = all layer mask
	bool Physics::OverlapBox(const Vector3& position, const Quaternion& rotation,
		const Vector3& size, OverlapHit* hit, const LayerMask& layerMask)
	{
		physx::PxOverlapBuffer buffer;
		
		//Overlap
		if (IS_TRUE(m_scene->overlap(physx::PxBoxGeometry((size * 0.5f).ToPhysX()),
			physx::PxTransform(position.ToPhysX(), rotation.ToPhysX()), buffer, 
			physx::PxQueryFilterData(physx::PxFilterData(layerMask(), 0, 0, 0),
				physx::PxQueryFlag::ePREFILTER | physx::PxQueryFlag::eDYNAMIC | physx::PxQueryFlag::eSTATIC), &m_queryCallback)))
		{
			//overlapHit���
			if (hit != nullptr) hit->Init(buffer);
			return true;
		}
		else return false;
	}		
	//----------------------------------------------------------------------------------
	//[OverlapSphere]
	//Sphere�ɃI�u�W�F�N�g���ڐG���Ă��邩�m�F����
	//����1: ���W
	//����2: radius
	//����3: OverlapHit, nullptr�łȂ��ꍇ��Hit����������, default = nullptr
	//����4: LayerMask, default = all layer mask
	bool Physics::OverlapSphere(const Vector3 & position, float radius, 
		OverlapHit * hit, const LayerMask& layerMask)
	{
		physx::PxOverlapBuffer buffer;

		//Overlap
		if (IS_TRUE(m_scene->overlap(physx::PxSphereGeometry(radius),
			physx::PxTransform(position.ToPhysX(), Const::Quaternion::identity.ToPhysX()), buffer,
			physx::PxQueryFilterData(physx::PxFilterData(layerMask(), 0, 0, 0),
				physx::PxQueryFlag::ePREFILTER | physx::PxQueryFlag::eDYNAMIC | physx::PxQueryFlag::eSTATIC), &m_queryCallback)))
		{
			//overlapHit���
			if (hit != nullptr) hit->Init(buffer);
			return true;
		}
		else return false;
	}
	//----------------------------------------------------------------------------------
	//[OverlapCapsule]
	//Capsule�ɃI�u�W�F�N�g���ڐG���Ă��邩�m�F����
	//����1: ���W
	//����2: ��]
	//����3: radius
	//����4: height
	//����5: OverlapHit, nullptr�łȂ��ꍇ��Hit����������, default = nullptr
	//����6: LayerMask, default = all layer mask
	bool Physics::OverlapCapsule(const Vector3 & position, const Quaternion & rotation, 
		float radius, float height, OverlapHit * hit, const LayerMask& layerMask)
	{
		physx::PxOverlapBuffer buffer;

		//Overlap
		if (IS_TRUE(m_scene->overlap(physx::PxCapsuleGeometry(radius, height * 0.5f),
			physx::PxTransform(position.ToPhysX(), rotation.ToPhysX()), buffer,
			physx::PxQueryFilterData(physx::PxFilterData(layerMask(), 0, 0, 0),
				physx::PxQueryFlag::ePREFILTER | physx::PxQueryFlag::eDYNAMIC | physx::PxQueryFlag::eSTATIC), &m_queryCallback)))
		{
			//overlapHit���
			if (hit != nullptr) hit->Init(buffer);
			return true;
		}
		else return false;
	}
}