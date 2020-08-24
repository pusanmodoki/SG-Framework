/*----------------------------------------------------------------------------------
物理エンジン(PhysX)を管理するPhysics class
	→Collision Filterを定義する PhysicsFilterShader.hpp
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_PHYSICS_FILTER_SHADER_HPP_
#define SGFRAMEWORK_HEADER_PHYSICS_FILTER_SHADER_HPP_
#include "../Physics.hpp"

//Framework namespace
namespace SGFramework
{
	//Filter Data
	//world0 = this layer
	//world1 = layerMask
	//world2 = used callback = 1
	//world3 = transform instance id

	//----------------------------------------------------------------------------------
	//[FilterShader]
	//Collision Filter
	inline physx::PxFilterFlags Physics::FilterShader(
		physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0, 
		physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1, 
		physx::PxPairFlags & pairFlags, const void * constantBlock, physx::PxU32 constantBlockSize)
	{
		//using physx namespace
		using namespace physx;

		//un used
		PX_UNUSED(constantBlock);
		PX_UNUSED(constantBlockSize);
		
		//レイヤー判定
		if ((filterData0.word0 & filterData1.word1) == 0)
			return PxFilterFlag::eSUPPRESS;

		//トリガーがいない->Collision設定
		if (((attributes0 | attributes1) & PxFilterObjectFlag::eTRIGGER) == 0)
		{
			pairFlags = PxPairFlag::eSOLVE_CONTACT |
				PxPairFlag::eDETECT_DISCRETE_CONTACT |
				PxPairFlag::eDETECT_CCD_CONTACT;
		}
		//トリガーがいる->Trigger設定
		else
		{
			pairFlags = PxPairFlag::eDETECT_DISCRETE_CONTACT |
				PxPairFlag::eNOTIFY_TOUCH_FOUND |
				PxPairFlag::eNOTIFY_TOUCH_LOST;
		}

		//Use Callback->Notify, Else->Default Call
		if ((filterData0.word2 | filterData1.word2) && filterData0.word3 != filterData1.word3)
			return PxFilterFlag::eNOTIFY;
		else
			return PxFilterFlag::eDEFAULT;
	}
}
#endif //!SGFRAMEWORK_HEADER_PHYSICS_FILTER_SHADER_HPP_