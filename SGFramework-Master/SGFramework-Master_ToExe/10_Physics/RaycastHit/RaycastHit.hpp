/*----------------------------------------------------------------------------------
Raycastで使用するRaycastHit structure
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_RAYCAST_HIT_HPP_
#define SGFRAMEWORK_HEADER_RAYCAST_HIT_HPP_
#include "../Physics.hpp"
#include "../Collider/BaseCollider.hpp"

//Framework namespace
namespace SGFramework
{
	//Raycastで使用するRaycastHit structure
	struct RaycastHit
	{
		//friend
		friend class Physics;

		//all setting constructor
		inline RaycastHit() : numHits(0), isValid(false) { }
		//buffer setting constructor
		inline RaycastHit(physx::PxRaycastBuffer* buffer)
			: numHits(buffer ? buffer->getNbAnyHits() : 0), isValid(buffer && buffer->getNbAnyHits() ? true : false)
		{
			if (buffer) memcpy_s(&m_buffer, sizeof(physx::PxRaycastBuffer), buffer, sizeof(physx::PxRaycastBuffer));
		}

		//hit transform (read function property)
		//argument 1: hit index, default = 0
		SGF_FUNCTION_PROPERTY WeakPointer<Transform> getTransform(uint index = 0)
		{
			return index < numHits ? (*reinterpret_cast<WeakPointer<BaseClass::BaseCollider>*>(m_buffer.getAnyHit(index).shape->userData))->transform
				: WeakPointer<Transform>();
		}
		//hit rigid body (read function property)
		//argument 1: hit index, default = 0
		SGF_FUNCTION_PROPERTY WeakPointer<BaseClass::BaseRigidBody> getRigidBody(uint index = 0)
		{
			return index < numHits ? (*reinterpret_cast<WeakPointer<BaseClass::BaseCollider>*>(m_buffer.getAnyHit(index).shape->userData))->rigidBody
				: WeakPointer<BaseClass::BaseRigidBody>();
		}
		//hit object collider (read function property)
		//argument 1: hit index, default = 0
		SGF_FUNCTION_PROPERTY WeakPointer<BaseClass::BaseCollider> getCollider(uint index = 0)
		{
			return index < numHits ? *reinterpret_cast<WeakPointer<BaseClass::BaseCollider>*>(m_buffer.getAnyHit(index).shape->userData)
				: WeakPointer<BaseClass::BaseCollider>();
		}
		//hit object position (read function property)
		//argument 1: hit index, default = 0
		SGF_FUNCTION_PROPERTY Vector3 getPosition(uint index = 0)
		{
			return index < numHits ? (*reinterpret_cast<WeakPointer<BaseClass::BaseCollider>*>(m_buffer.getAnyHit(index).shape->userData))->transform->getWorldPosition()
				: Const::Vector3::zero;
		}
		
		//hit object surface uv (read function property)
		//argument 1: hit index, default = 0
		SGF_FUNCTION_PROPERTY UV getUV(uint index = 0)
		{
			if (index < numHits)
			{
				auto hit = m_buffer.getAnyHit(index);
				return UV(hit.u, hit.v);
			}
			else return Const::UV::zero;
		}
		//hit object surface normal (read function property)
		//argument 1: hit index, default = 0
		SGF_FUNCTION_PROPERTY Vector3 getNormal(uint index = 0) { return index < numHits ? m_buffer.getAnyHit(index).normal : Const::Vector3::zero; }
		//distance : hit object surface <-> origin (read function property)
		//argument 1: hit index, default = 0
		SGF_FUNCTION_PROPERTY float getDistance(uint index = 0) { return index < numHits ? m_buffer.getAnyHit(index).distance : 0.0f; }
		//hit object surface -> polygon triangle index(read function property)
		//argument 1: hit index, default = 0
		SGF_FUNCTION_PROPERTY int getTriangleIndex(uint index = 0) { return index < numHits ? m_buffer.getAnyHit(index).faceIndex : 0; }

		//num hit infos
		const uint numHits;
		//isValid Infos?
		const bool isValid;

	private:
		//----------------------------------------------------------------------------------
		//[Init]
		//Bufferをコピーする
		void Init(physx::PxRaycastBuffer& buffer)
		{
			(*const_cast<uint*>(&this->numHits)) = buffer.getNbAnyHits();
			(*const_cast<bool*>(&this->isValid)) = buffer.getNbAnyHits() ? true : false;

			memcpy_s(&m_buffer, sizeof(physx::PxRaycastBuffer), &buffer, sizeof(physx::PxRaycastBuffer));
		}

		//buffer
		physx::PxRaycastBuffer m_buffer;
	};
}

#endif //!SGFRAMEWORK_HEADER_RAYCAST_HIT_HPP_