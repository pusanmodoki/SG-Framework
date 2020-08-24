/*----------------------------------------------------------------------------------
Box状当たり判定を提供するBoxCollider class
BuildColliderを実行するまで当たり判定は機能しないので注意
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_BOX_COLLIDER_HPP_
#define SGFRAMEWORK_HEADER_BOX_COLLIDER_HPP_
#include "BaseCollider.hpp"
#include "../PhysicsMaterial.hpp"
#include "../Physics.hpp"

//Framework namespace
namespace SGFramework
{
	//Box状当たり判定を提供するBoxCollider class
	//BuildColliderを実行するまで当たり判定は機能しないので注意
	class BoxCollider : public BaseClass::BaseCollider
	{
	public:
		//box scale(transform->getWorldScale * this value) (read function property)
		SGF_FUNCTION_PROPERTY Vector3 getScale() { return m_scale * 2.0f; }
		//box scale(transform->getWorldScale * this value) (read function property)
		//argument 1: set value
		SGF_FUNCTION_PROPERTY Vector3 setScale(const Vector3& set)
		{
			using namespace Detail::Physics;
			
			m_scale = set * 0.5f;
			if (m_shape)
			{
				//message作成
				PhysicsMessage message(MessageType::BoxColliderMessageFlag | MessageType::BoxSetScaleV3, instanceID());
				message.attachment.vector3 = m_scale * transform->getWorldScale();
				//send message
				Physics::SendPhysicsMessage(message);
			}
			return set;
		}
		//attachd is rigid body static or kinematic only? (read function property)
		SGF_FUNCTION_PROPERTY bool getIsStaticOnlly() override { return false; }

		//----------------------------------------------------------------------------------
		//[BuildCollider]
		//Colliderを生成する
		//引数1: box scale (transform->getWorldScale * this value)
		//引数2: collider is trigger?, default = false
		//引数3: used hit callback?, default = ture
		//引数4: collider center, default = zero
		//引数5: physics material, default = defaultMaterial
		inline void BuildCollider(const Vector3& scale, bool isTrigger = false, bool isUsedCallback = true,
			const Vector3& center = Const::Vector3::zero,
			const PhysicsMaterial& material = Physics::defaultPhysicsMaterial())
		{
			using namespace Detail::Physics;

			//Set
			m_isTrigger = isTrigger;
			m_scale = scale;
			m_transformScale = transform->getWorldScale();
			m_center = center;
			m_material = material;
			m_isUsedCallback = isUsedCallback;

			//message作成
			PhysicsMessage message(MessageType::PointerMessageFlag | MessageType::RegisterColliderCL, instanceID());
			message.attachment.collider = m_thisPointer.DynamicCast<BaseClass::BaseCollider>();
			//send message
			Physics::SendPhysicsMessage(message);
			//message.Release();
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
			m_shape = Physics::physicsFactory->createShape(physx::PxBoxGeometry(
				(m_scale * m_transformScale).ToPhysX()), *m_material(), true, flag);
			//type setting
			m_type = static_cast<ColliderType>(m_shape->getGeometryType());

			//filter setting
			InitFilterMask(m_isUsedCallback);

			//userData = this instance id
			m_shape->userData = reinterpret_cast<void*>(static_cast<size_t>(instanceID()));

			//equal rigidbody transfoem
			if (IS_TRUE(m_isEqualRigidTransform))
				m_shape->setLocalPose(physx::PxTransform(CalculateCenter().ToPhysX(), Const::Quaternion::identity.ToPhysX()));
			//not equal rigidbody transfoem
			else
				m_shape->setLocalPose(physx::PxTransform(
				(transform->getWorldPosition() - transform->getParent()->getWorldPosition() + CalculateCenter()).ToPhysX(),
					(transform->getWorldRotation() * transform->getParent()->getWorldRotation().getInverse()).ToPhysX()));

			//attach
			m_actor->attachShape(*m_shape);
		}
		//----------------------------------------------------------------------------------
		//[ChangeScale]
		//Scaleを変更する
		inline void ChangeScale() override
		{
			//get
			Vector3 get = transform->getWorldScale();

			//bufと違えば変更
			if (m_transformScale != get)
				m_shape->setGeometry(physx::PxBoxGeometry((m_scale  * get).ToPhysX()));

			//buf保存
			m_transformScale = get;
		}

		Vector3 m_transformScale = Const::Vector3::zero;	//transform scale
		Vector3 m_scale = Const::Vector3::zero;					//collider scale
	};

}
#endif //!SGFRAMEWORK_HEADER_BOX_COLLIDER_HPP_