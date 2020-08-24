/*----------------------------------------------------------------------------------
íËêîÇìZÇﬂÇΩConst namespace
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
		//Color Constant Values
		namespace Color
		{
			constexpr SGFramework::Color black(0.0f, 0.0f, 0.0f, 1.0f);		//black (0.0f, 0.0f, 0.0f, 1.0f)
			constexpr SGFramework::Color blue(0.0f, 0.0f, 1.0f, 1.0f);		//blue (0.0f, 0.0f, 1.0f, 1.0f)
			constexpr SGFramework::Color clear(0.0f, 0.0f, 0.0f, 0.0f);		//clear (0.0f, 0.0f, 0.0f, 0.0f)
			constexpr SGFramework::Color cyan(0.0f, 1.0f, 1.0f, 1.0f);		//cyan (0.0f, 1.0f, 1.0f, 1.0f)
			constexpr SGFramework::Color gray(0.5f, 0.5f, 0.5f, 1.0f);		//gray (0.5f, 0.5f, 0.5f, 1.0f)
			constexpr SGFramework::Color green(0.0f, 1.0f, 0.0f, 1.0f);		//green (0.0f, 1.0f, 0.0f, 1.0f)
			constexpr SGFramework::Color magenta(1.0f, 0.0f, 1.0f, 1.0f);	//magenta (1.0f, 0.0f, 1.0f, 1.0f)
			constexpr SGFramework::Color red(1.0f, 0.0f, 0.0f, 1.0f);			//red (1.0f, 0.0f, 0.0f, 1.0f)
			constexpr SGFramework::Color white(1.0f, 1.0f, 1.0f, 1.0f);		//white (1.0f, 1.0f, 1.0f, 1.0f)
			constexpr SGFramework::Color yellow(1.0f, 1.0f, 0.0f, 1.0f);		//yellow (1.0f, 1.0f, 0.0f, 1.0f)
		}

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
