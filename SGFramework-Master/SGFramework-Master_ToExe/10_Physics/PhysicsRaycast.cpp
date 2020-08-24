/*----------------------------------------------------------------------------------
物理エンジン(PhysX)を管理するPhysics class
	→Raycast系関数を定義するPhysicsRaycast.cpp
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
	//Rayで当たり判定を行う
	//引数1: 原点
	//引数2: 方向
	//引数3: 距離, default = infinity
	//引数4: RaycastHit, nullptrでない場合はHit情報を代入する, default = nullptr
	//引数4: LayerMask, default = all layer mask
	//引数5: true = rayに当たった情報を全て返却
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
			//raycastHit代入
			if (raycastHit != nullptr) raycastHit->Init(buffer);
			return true;
		}
		else return false;
	}

	//----------------------------------------------------------------------------------
	//[BoxCast]
	//BoxRayで当たり判定を行う
	//引数1: 原点
	//引数2: 方向
	//引数3: サイズ
	//引数4: 回転
	//引数5: 距離, default = infinity
	//引数6: SweepHit, nullptrでない場合はHit情報を代入する, default = nullptr
	//引数7: LayerMask, default = all layer mask
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
			//sweepHit代入
			if (hit != nullptr) hit->Init(buffer);
			return true;
		}
		else return false;
	}
	//----------------------------------------------------------------------------------
	//[SphereCast]
	//SphereRayで当たり判定を行う
	//引数1: 原点
	//引数2: 方向
	//引数3: 半径
	//引数4: 距離, default = infinity
	//引数5: SweepHit, nullptrでない場合はHit情報を代入する, default = nullptr
	//引数6: LayerMask, default = all layer mask
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
			//sweepHit代入
			if (hit != nullptr) hit->Init(buffer);
			return true;
		}
		else return false;
	}
	//----------------------------------------------------------------------------------
	//[CapsuleCast]
	//CapsuleRayで当たり判定を行う
	//引数1: 原点
	//引数2: 方向
	//引数3: 半径
	//引数4: 高さ
	//引数5: 回転
	//引数6: 距離, default = infinity
	//引数7: SweepHit, nullptrでない場合はHit情報を代入する, default = nullptr
	//引数8: LayerMask, default = all layer mask
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
			//sweepHit代入
			if (hit != nullptr) hit->Init(buffer);
			return true;
		}
		else return false;
	}
	//----------------------------------------------------------------------------------
	//[OverlapBox]
	//Boxにオブジェクトが接触しているか確認する
	//引数1: 座標
	//引数2: 回転
	//引数3: サイズ
	//引数4: OverlapHit, nullptrでない場合はHit情報を代入する, default = nullptr
	//引数5: LayerMask, default = all layer mask
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
			//overlapHit代入
			if (hit != nullptr) hit->Init(buffer);
			return true;
		}
		else return false;
	}		
	//----------------------------------------------------------------------------------
	//[OverlapSphere]
	//Sphereにオブジェクトが接触しているか確認する
	//引数1: 座標
	//引数2: radius
	//引数3: OverlapHit, nullptrでない場合はHit情報を代入する, default = nullptr
	//引数4: LayerMask, default = all layer mask
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
			//overlapHit代入
			if (hit != nullptr) hit->Init(buffer);
			return true;
		}
		else return false;
	}
	//----------------------------------------------------------------------------------
	//[OverlapCapsule]
	//Capsuleにオブジェクトが接触しているか確認する
	//引数1: 座標
	//引数2: 回転
	//引数3: radius
	//引数4: height
	//引数5: OverlapHit, nullptrでない場合はHit情報を代入する, default = nullptr
	//引数6: LayerMask, default = all layer mask
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
			//overlapHit代入
			if (hit != nullptr) hit->Init(buffer);
			return true;
		}
		else return false;
	}
}