/*----------------------------------------------------------------------------------
物理マテリアルを定義するPhysicsMaterial
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

	//物理マテリアルを定義するPhysicsMaterial
	class PhysicsMaterial
	{
	public:
		//friend
		friend class Physics;
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//nullptrに設定される
		inline PhysicsMaterial() : m_material(nullptr) {}
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化を行う
		//引数1: 静止摩擦係数
		//引数2: 動的摩擦係数
		//引数3: 反発係数
		inline PhysicsMaterial(float staticFriction, float dynamicFriction, float restitution) : m_material(nullptr)
		{
			m_material = Physics()->createMaterial(staticFriction, dynamicFriction, restitution);
			if (m_material == nullptr)
				throw InvalidArgument(L"Error!! PhysicsMaterial->Constructor", L"createMaterial failed");
		}
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//コピーを行う
		//引数1: copy
		inline PhysicsMaterial(const PhysicsMaterial& copy) : m_material(nullptr)
		{
			PHYSX_RELEASE(m_material);
			m_material = copy.m_material;
			if (m_material) m_material->acquireReference();
		}
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//ムーブを行う
		//引数1: move
		inline PhysicsMaterial(PhysicsMaterial&& move) : m_material(move.m_material)
		{
			move.m_material = nullptr;
		}
		//----------------------------------------------------------------------------------
		//[デストラクタ]
		inline ~PhysicsMaterial() { PHYSX_RELEASE(m_material); }
		//----------------------------------------------------------------------------------
		//[Release]
		//内部ポインタの開放を行う
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