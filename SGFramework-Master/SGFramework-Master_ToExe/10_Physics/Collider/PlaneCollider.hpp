/*----------------------------------------------------------------------------------
無限平面状当たり判定を提供するPlaneCollider class
BuildColliderを実行するまで当たり判定は機能しないので注意
平面の法線はforward方向と同義となる
当たり判定はUnity風ではなく、
Planeの平面から上にいる(not hit)か, 下にいるか(hit)のみで判定する。注意。
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_PLANE_COLLIDER_HPP_
#define SGFRAMEWORK_HEADER_PLANE_COLLIDER_HPP_
#include "BaseCollider.hpp"
#include "../PhysicsMaterial.hpp"
#include "../Physics.hpp"

//Framework namespace
namespace SGFramework
{
	//無限平面状当たり判定を提供するPlaneCollider class
	//BuildColliderを実行するまで当たり判定は機能しないので注意
	//平面の法線はforward方向と同義となる
	//当たり判定はUnity風ではなく、
	//Planeの平面から上にいる(not hit)か, 下にいるか(hit)のみで判定する。注意。
	class PlaneCollider : public BaseClass::BaseCollider
	{
	public:
		//plane normal(transform.forward) (read function property)
		SGF_FUNCTION_PROPERTY Vector3 getNormal () 
		{
			return transform.getIsValid() ? 
				transform->getForward() : Const::Vector3::zero; 
		}
		//attachd is rigid body static or kincematic only? (read function property)
		SGF_FUNCTION_PROPERTY bool getIsStaticOnlly() override { return true; }

		//----------------------------------------------------------------------------------
		//[BuildCollider]
		//Colliderを生成する
		//平面の法線はforward方向と同義となる
		//引数1: collider is trigger?, default = false
		//引数2: used hit callback?, default = ture
		//引数3: collider center, default = zero
		//引数4: physics material, default = defaultMaterial
		inline void BuildCollider(bool isTrigger = false, bool isUsedCallback = true,
			const Vector3& center = Const::Vector3::zero,
			const PhysicsMaterial& material = Physics::defaultPhysicsMaterial())
		{
			using namespace Detail::Physics;

			m_isTrigger = isTrigger;
			m_center = center;
			m_material = material;
			m_isUsedCallback = isUsedCallback;

			//message作成
			PhysicsMessage message(MessageType::PointerMessageFlag | MessageType::RegisterColliderCL, instanceID());
			message.attachment.collider = m_thisPointer.DynamicCast<BaseClass::BaseCollider>();
			//send message
			Physics::SendPhysicsMessage(message);
		}

	private:
		//----------------------------------------------------------------------------------
		//[CreateShape]
		//実際にShapeを作成する
		inline void CreateShape() override
		{
			//flag setting
			physx::PxShapeFlags flag = physx::PxShapeFlag::eSCENE_QUERY_SHAPE;

			if (IS_TRUE(m_isTrigger)) flag |= physx::PxShapeFlag::eTRIGGER_SHAPE;
			else flag |= physx::PxShapeFlag::eSIMULATION_SHAPE;

			if (IS_TRUE(IsDebug::isValue())) flag |= physx::PxShapeFlag::eVISUALIZATION;

			//create shape
			m_shape = Physics::physicsFactory->createShape(
				physx::PxPlaneGeometry(), *m_material(), true, flag);
			//type setting
			m_type = static_cast<ColliderType>(m_shape->getGeometryType());

			//filter setting
			InitFilterMask(m_isUsedCallback);

			//userData = this instance id
			m_shape->userData = reinterpret_cast<void*>(static_cast<size_t>(instanceID()));

			//set local pose
			m_shape->setLocalPose(physx::PxTransformFromPlaneEquation(
				physx::PxPlane(
				(transform->getWorldPosition() + CalculateCenter()).ToPhysX(),
					(transform->getWorldRotation() * Const::Vector3::down).ToPhysX())));

			//attach
			m_actor->attachShape(*m_shape);
		}
		//----------------------------------------------------------------------------------
		//[ChangeScale]
		//Scaleを変更する
		inline void ChangeScale() override {}
	};
}
#endif //!SGFRAMEWORK_HEADER_PLANE_COLLIDER_HPP_