/*----------------------------------------------------------------------------------
Capsule�󓖂��蔻���񋟂���CapsuleCollider class
BuildCollider�����s����܂œ����蔻��͋@�\���Ȃ��̂Œ���
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_CAPSULE_COLLIDER_HPP_
#define SGFRAMEWORK_HEADER_CAPSULE_COLLIDER_HPP_
#include "BaseCollider.hpp"
#include "../PhysicsMaterial.hpp"
#include "../Physics.hpp"

//Framework namespace
namespace SGFramework
{
	//Capsule�󓖂��蔻���񋟂���CapsuleCollider class
	//BuildCollider�����s����܂œ����蔻��͋@�\���Ȃ��̂Œ���
	class CapsuleCollider : public BaseClass::BaseCollider
	{
	public:
		//capsule radius (transform->getWorldScale.x * this value) (read function property)
		SGF_FUNCTION_PROPERTY float getRadius() { return m_radius; }
		//capsule radius (transform->getWorldScale.x * this value) (set function property)
		//argument 1: set value
		SGF_FUNCTION_PROPERTY float setRadius(float set)
		{
			m_radius = set;
			if (m_shape)
			{
				Physics::LockGuardPhysics guard(true);
			
				m_shape->setGeometry(physx::PxCapsuleGeometry(
					m_radius * m_transformScaleXY.x,
					m_height * m_transformScaleXY.y));
			}
			return set;
		}
		//capsule height (transform->getWorldScale.y * this value) (read function property)
		SGF_FUNCTION_PROPERTY float getHeight() { return m_height * 2.0f; }
		//capsule height (transform->getWorldScale.y * this value) (set function property)
		//argument 1: set value
		SGF_FUNCTION_PROPERTY float setHeight(float set)
		{
			m_height = set * 0.5f;
			if (m_shape)
			{
				Physics::LockGuardPhysics guard(true);
			
				m_shape->setGeometry(physx::PxCapsuleGeometry(
					m_radius * m_transformScaleXY.x,
					m_height * m_transformScaleXY.y));
			}
			return set;
		}
		//attachd is rigid body static or kincematic only? (read function property)
		SGF_FUNCTION_PROPERTY bool getIsStaticOnlly() override { return false; }

		//----------------------------------------------------------------------------------
		//[BuildCollider]
		//Collider�𐶐�����
		//����1: radius (transform->getWorldScale.x * this value)
		//����2: height (transform->getWorldScale.y * this value)
		//����3: collider is trigger?, default = false
		//����4: used hit callback?, default = ture
		//����5: collider center, default = zero
		//����6: physics material, default = defaultMaterial
		inline void BuildCollider(float radius, float height, bool isTrigger = false, 
			bool isUsedCallback = true, const Vector3& center = Const::Vector3::zero,
			const PhysicsMaterial& material = Physics::defaultPhysicsMaterial())
		{
			using namespace Detail::Physics;

			//Set
			m_isTrigger = isTrigger;
			m_center = center;
			m_transformScaleXY = transform->getWorldScale();
			m_radius = radius;
			m_height = height * 0.5f;
			m_material = material;
			m_isUsedCallback = isUsedCallback;

			//message�쐬
			PhysicsMessage message(MessageType::PointerMessageFlag | MessageType::RegisterColliderCL, instanceID());
			message.attachment.collider = m_thisPointer.DynamicCast<BaseClass::BaseCollider>();
			//send message
			Physics::SendPhysicsMessage(message);
		}

	private:
		//----------------------------------------------------------------------------------
		//[CreateShape]
		//���ۂ�Shape���쐬����
		inline void CreateShape() override
		{
			//flag setting
			physx::PxShapeFlags flag = physx::PxShapeFlag::eSCENE_QUERY_SHAPE;

			if (IS_TRUE(m_isTrigger)) flag |= physx::PxShapeFlag::eTRIGGER_SHAPE;
			else flag |= physx::PxShapeFlag::eSIMULATION_SHAPE;

			if (IS_TRUE(IsDebug::isValue())) flag |= physx::PxShapeFlag::eVISUALIZATION;

			//create shape
			m_shape = Physics::physicsFactory->createShape(physx::PxCapsuleGeometry(
				m_radius * m_transformScaleXY.x, m_height * m_transformScaleXY.y), *m_material(), true, flag);
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
		//Scale��ύX����
		inline void ChangeScale() override
		{
			//get
			Vector2 get = transform->getWorldScale();

			//buf�ƈႦ�ΕύX
			if (m_transformScaleXY != get)
				m_shape->setGeometry(physx::PxCapsuleGeometry(
					m_radius * m_transformScaleXY.x,
					m_height * m_transformScaleXY.y));

			//buf�ۑ�
			m_transformScaleXY = get;
		}

		//transform scale.x, y
		Vector2 m_transformScaleXY = Const::Vector2::zero;
		float m_radius = 0.0f;					//collider radius
		float m_height = 0.0f;				//collider height
	};
}
#endif //!SGFRAMEWORK_HEADER_CAPSULE_COLLIDER_HPP_