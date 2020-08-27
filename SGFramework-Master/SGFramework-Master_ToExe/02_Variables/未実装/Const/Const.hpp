/*----------------------------------------------------------------------------------
定数を纏めたConst namespace
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_CONST_HPP_
#define SGFRAMEWORK_HEADER_CONST_HPP_
#include "ConstVector.hpp"
#include "../Color/Color.hpp"
#include "../Quarernion/Quaternion.hpp"
#include "../UV/UV.hpp"

//Framework namespace
namespace SGFramework
{
	//Constant Values
	namespace Const
	{
		//Quaternion Constant Values
		namespace Quaternion
		{
			//0.0f,0.0f,0.0f,1.0f
			constexpr SGFramework::Quaternion identity = { 0.0f, 0.0f, 0.0f, 1.0f };	
			//infinity, infinity, infinity, infinity 
			constexpr SGFramework::Quaternion infinite(MathAF::cInfinity, MathAF::cInfinity, MathAF::cInfinity, MathAF::cInfinity);
			//negativeInfinity, negativeInfinity, negativeInfinity, negativeInfinity
			constexpr SGFramework::Quaternion negativeInfinite(MathAF::cNegativeInfinity, MathAF::cNegativeInfinity, MathAF::cNegativeInfinity, MathAF::cNegativeInfinity);
		}

		//UV Constant Values
		namespace UV
		{
			//UV(0,0)
			constexpr SGFramework::UV zero(0.0f, 0.0f);
			//UV(1,1)
			constexpr SGFramework::UV one(1.0f, 1.0f);
			//UV(0,0)
			constexpr SGFramework::UV leftTop(0.0f, 0.0f);
			//UV(0,1)
			constexpr SGFramework::UV leftDown(0.0f, 1.0f);
			//UV(1,0)
			constexpr SGFramework::UV rightTop(1.0f, 0.0f);
			//UV(1,1)
			constexpr SGFramework::UV rightDown(1.0f, 1.0f);
		}
	}
}

#endif // !SGFRAMEWORK_HEADER_CONST_HPP_
