/*----------------------------------------------------------------------------------
Overlap(OverlapBox, OverlapSphere, OverlapCapsule)で使用するOverlapHit structure
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_OVERLAP_HIT_HPP_
#define SGFRAMEWORK_HEADER_OVERLAP_HIT_HPP_
#include "../Physics.hpp"
#include "../Collider/BaseCollider.hpp"

//Framework namespace
namespace SGFramework
{
	//Overlap(OverlapBox, OverlapSphere, OverlapCapsule)で使用するOverlapHit structure
	struct OverlapHit
	{
		//friend
		friend class Physics;
		//all setting constructor
		inline OverlapHit() : isValid(false) { }
		//buffer setting constructor
		inline OverlapHit(physx::PxRaycastBuffer* buffer) : isValid(buffer && buffer->getNbAnyHits() ? true : false)
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

		//hit object surface -> polygon triangle index(read function property)
		SGF_FUNCTION_PROPERTY int getTriangleIndex() { return IS_TRUE(isValid) ? m_buffer.getAnyHit(0).faceIndex : 0; }

		//isValid Infos?
		const bool isValid;

	private:
		//----------------------------------------------------------------------------------
		//[Init]
		//Bufferをコピーする
		void Init(physx::PxOverlapBuffer& buffer)
		{
			(*const_cast<bool*>(&this->isValid)) = buffer.getNbAnyHits() ? true : false;

			memcpy_s(&m_buffer, sizeof(physx::PxOverlapBuffer), &buffer, sizeof(physx::PxOverlapBuffer));
		}

		//buffer
		physx::PxOverlapBuffer m_buffer;
	};
}

#endif //!SGFRAMEWORK_HEADER_OVERLAP_HIT_HPP_