/*----------------------------------------------------------------------------------
SweepCast(BoxCast, SphereCast, CapsuleCast)で使用するSweepHit structure
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_SWEEP_HIT_HPP_
#define SGFRAMEWORK_HEADER_SWEEP_HIT_HPP_
#include "../Physics.hpp"
#include "../Collider/BaseCollider.hpp"

//Framework namespace
namespace SGFramework
{
	//SweepCast(BoxCast, SphereCast, CapsuleCast)で使用するSweepHit structure
	struct SweepHit
	{
		//friend
		friend class Physics;

		//all setting constructor
		inline SweepHit() : isValid(false) { }
		//buffer setting constructor
		inline SweepHit(physx::PxRaycastBuffer* buffer) : isValid(buffer && buffer->getNbAnyHits() ? true : false)
		{
			if (buffer) memcpy_s(&m_buffer, sizeof(physx::PxRaycastBuffer), buffer, sizeof(physx::PxRaycastBuffer));
		}

		//hit transform (read function property)
		SGF_FUNCTION_PROPERTY WeakPointer<Transform> getTransform()
		{
			return IS_TRUE(isValid) ? (*reinterpret_cast<WeakPointer<BaseClass::BaseCollider>*>(m_buffer.getAnyHit(0).shape->userData))->transform
				: WeakPointer<Transform>();
		}
		//hit rigid body (read function property)
		SGF_FUNCTION_PROPERTY WeakPointer<BaseClass::BaseRigidBody> getRigidBody()
		{
			return IS_TRUE(isValid) ? (*reinterpret_cast<WeakPointer<BaseClass::BaseCollider>*>(m_buffer.getAnyHit(0).shape->userData))->rigidBody
				: WeakPointer<BaseClass::BaseRigidBody>();
		}
		//hit object collider (read function property)
		SGF_FUNCTION_PROPERTY WeakPointer<BaseClass::BaseCollider> getCollider()
		{
			return IS_TRUE(isValid) ? *reinterpret_cast<WeakPointer<BaseClass::BaseCollider>*>(m_buffer.getAnyHit(0).shape->userData)
				: WeakPointer<BaseClass::BaseCollider>();
		}
		//hit object position (read function property)
		SGF_FUNCTION_PROPERTY Vector3 getPosition()
		{
			return IS_TRUE(isValid) ? (*reinterpret_cast<WeakPointer<BaseClass::BaseCollider>*>(m_buffer.getAnyHit(0).shape->userData))->transform->getWorldPosition()
				: Const::Vector3::zero;
		}

		//hit object surface normal (read function property)
		SGF_FUNCTION_PROPERTY Vector3 getNormal(uint index = 0) { return IS_TRUE(isValid) ? m_buffer.getAnyHit(index).normal : Const::Vector3::zero; }
		//distance : hit object surface <-> origin (read function property)
		SGF_FUNCTION_PROPERTY float getDistance(uint index = 0) { return IS_TRUE(isValid) ? m_buffer.getAnyHit(index).distance : 0.0f; }
		//hit object surface -> polygon triangle index(read function property)
		SGF_FUNCTION_PROPERTY int getTriangleIndex(uint index = 0) { return IS_TRUE(isValid) ? m_buffer.getAnyHit(index).faceIndex : 0; }

		//isValid Infos?
		const bool isValid;

	private:
		//----------------------------------------------------------------------------------
		//[Init]
		//Bufferをコピーする
		void Init(physx::PxSweepBuffer& buffer)
		{
			(*const_cast<bool*>(&this->isValid)) = buffer.getNbAnyHits() ? true : false;

			memcpy_s(&m_buffer, sizeof(physx::PxRaycastBuffer), &buffer, sizeof(physx::PxRaycastBuffer));
		}

		//buffer
		physx::PxSweepBuffer m_buffer;
	};
}

#endif //!SGFRAMEWORK_HEADER_SWEEP_HIT_HPP_