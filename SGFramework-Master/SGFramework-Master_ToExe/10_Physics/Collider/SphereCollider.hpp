/*----------------------------------------------------------------------------------
Sphere�󓖂��蔻���񋟂���SphereCollider class
BuildCollider�����s����܂œ����蔻��͋@�\���Ȃ��̂Œ���
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_SPHERE_COLLIDER_HPP_
#define SGFRAMEWORK_HEADER_SPHERE_COLLIDER_HPP_
#include "BaseCollider.hpp"
#include "../PhysicsMaterial.hpp"
#include "../Physics.hpp"

//Framework namespace
namespace SGFramework
{
	//Sphere�󓖂��蔻���񋟂���SphereCollider class
	//BuildCollider�����s����܂œ����蔻��͋@�\���Ȃ��̂Œ���
	class SphereCollider final : public BaseClass::BaseCollider
	{
	public:
		//sphere radius (transform->getWorldScale.x * this value) (read function property)
		SGF_FUNCTION_PROPERTY float getRadius() { return m_radius; }
		//sphere radius (transform->getWorldScale.x * this value) (set function property)
		//argument 1: set value
		SGF_FUNCTION_PROPERTY float setRadius(float set)
		{
			m_radius = set;
			if (m_shape)
			{
				Physics::LockGuardPhysics guard(true);
				m_shape->setGeometry(physx::PxSphereGeometry(m_radius * m_transformScaleX));
			}
			return set;
		}
		//attachd is rigid body static or kincematic only? (read function property)
		SGF_FUNCTION_PROPERTY bool getIsStaticOnlly() override { return false; }

		//----------------------------------------------------------------------------------
		//[BuildCollider]
		//Collider�𐶐�����
		//����1: radius (transform->getWorldScale.x * this value)
		//����2: collider is trigger?, default = false
		//����3: used hit callback?, default = ture
		//����4: collider center, default = zero
		//����5: physics material, default = defaultMaterial
		inline void BuildCollider(float radius, bool isTrigger = false, bool isUsedCallback = true,
			const Vector3& center = Const::Vector3::zero,
			const PhysicsMaterial& material = Physics::defaultPhysicsMaterial())
		{
			using namespace Detail::Physics;

			//Set
			m_isTrigger = isTrigger;
			m_radius = radius;
			m_transformScaleX = transform->getWorldScale().x;
			m_center = center;
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
			m_shape = Physics::physicsFactory->createShape(
				physx::PxSphereGeometry(m_radius * m_transformScaleX),
				*m_material(), true, flag);
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
			float get = transform->getWorldScale().x;

			//buf�ƈႦ�ΕύX
			if (m_transformScaleX != get)
				m_shape->setGeometry(physx::PxSphereGeometry(m_radius * m_transformScaleX));

			//buf�ۑ�
			m_transformScaleX = get;
		}

		float m_transformScaleX = 0.0f;	//transform scale.x
		float m_radius = 0.0f;					//collider radius
	};
}
#endif //!SGFRAMEWORK_HEADER_SPHERE_COLLIDER_HPP_