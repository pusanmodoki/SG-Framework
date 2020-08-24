/*----------------------------------------------------------------------------------
�����}�e���A�����`����PhysicsMaterial
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_PHYSICS_MATERIAL_HPP_
#define SGFRAMEWORK_HEADER_PHYSICS_MATERIAL_HPP_
#include "../02_CustomVariable/CustomVariable.hpp"
#include "PhysXInclude.hpp"

//Framework namespace
namespace SGFramework
{
	//friend
	class Physics;

	//�����}�e���A�����`����PhysicsMaterial
	class PhysicsMaterial
	{
	public:
		//friend
		friend class Physics;
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//nullptr�ɐݒ肳���
		inline PhysicsMaterial() : m_material(nullptr) {}
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//������������s��
		//����1: �Î~���C�W��
		//����2: ���I���C�W��
		//����3: �����W��
		inline PhysicsMaterial(float staticFriction, float dynamicFriction, float restitution) : m_material(nullptr)
		{
			m_material = Physics()->createMaterial(staticFriction, dynamicFriction, restitution);
			if (m_material == nullptr)
				throw InvalidArgument(L"Error!! PhysicsMaterial->Constructor", L"createMaterial failed");
		}
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�R�s�[���s��
		//����1: copy
		inline PhysicsMaterial(const PhysicsMaterial& copy) : m_material(nullptr)
		{
			PHYSX_RELEASE(m_material);
			m_material = copy.m_material;
			if (m_material) m_material->acquireReference();
		}
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//���[�u���s��
		//����1: move
		inline PhysicsMaterial(PhysicsMaterial&& move) : m_material(move.m_material)
		{
			move.m_material = nullptr;
		}
		//----------------------------------------------------------------------------------
		//[�f�X�g���N�^]
		inline ~PhysicsMaterial() { PHYSX_RELEASE(m_material); }
		//----------------------------------------------------------------------------------
		//[Release]
		//�����|�C���^�̊J�����s��
		inline void Release() { PHYSX_RELEASE(m_material); }

		//PxMaterial cast operator
		inline operator physx::PxMaterial*() const { return m_material; }
		//() operator, return->PxMaterial*
		inline physx::PxMaterial* operator () () const { return m_material; }
		//-> operator, return->PxMaterial*
		inline physx::PxMaterial* operator -> () const { return m_material; }
		//* operator, return->PxMaterial&
		inline const physx::PxMaterial& operator * () const { return *m_material; }
		//copy operator, return->this
		inline const PhysicsMaterial& operator = (const PhysicsMaterial& copy)
		{
			PHYSX_RELEASE(m_material);
			m_material = copy.m_material;
			if (m_material) m_material->acquireReference();
			return *this;
		}
		//move operator, return->this
		inline PhysicsMaterial& operator = (PhysicsMaterial&& move)
		{
			PHYSX_RELEASE(m_material);
			m_material = move.m_material;
			move.m_material = nullptr;
			return *this;
		}

	private:
		//----------------------------------------------------------------------------------
		//[Physics]
		//return: PxPhysics*
		static physx::PxPhysics*& Physics() { static physx::PxPhysics* pointer = nullptr; return pointer; }

		//material
		physx::PxMaterial* m_material = nullptr;
	};
}

#endif // !SGFRAMEWORK_HEADER_PHYSICS_MATERIAL_HPP_