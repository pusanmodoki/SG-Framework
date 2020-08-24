/*----------------------------------------------------------------------------------
色を扱うカラー構造体
----------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_COLOR_HPP_
#define SGFRAMEWORK_HEADER_COLOR_HPP_
#include "../Vector/Vector.hpp"
#include "../ConstAndUsing/ConstAndUsing.hpp"
#include "../Property/Property.hpp"

//Framework namespace
namespace SGFramework
{
	//色を扱うカラー構造体
	struct Color final
	{
		//コンストラクタ…クリアに初期化
		inline constexpr Color() : r(0.0f) , g(0.0f) , b(0.0f) , a(0.0f) {}
		//コンストラクタ…代入初期化
		//引数1: R
		//引数2: G
		//引数3: B
		//引数4: A
		inline constexpr Color(float _r, float _g, float _b, float _a) : r(_r), g(_g), b(_b), a(_a) {}
		//コンストラクタ…代入初期化
		//引数1: RGB
		//引数3: A
		inline constexpr Color(const Color& color, float _a) : r(color.r), g(color.g), b(color.b), a(_a) {}

		//変数
		float r;		//R
		float g;		//G
		float b;		//B
		float a;		//A
		
		
		//floatオペレーター
		inline operator float*(void) { return (float*)this; }
		//float
		inline operator const float*(void) const { return (const float*)this; }
		//==オペレーター
		inline bool operator ==(const Color& color) const
		{
			return (fabsf(r - color.r) <= MathAF::cEpsilon
				&& fabsf(g - color.g) <= MathAF::cEpsilon
				&& fabsf(b - color.b) <= MathAF::cEpsilon
				&& fabsf(a - color.a) <= MathAF::cEpsilon);
		};
		//!=オペレーター
		inline bool operator !=(const Color& color) const
		{
			return (fabsf(r - color.r) > MathAF::cEpsilon
				&& fabsf(g - color.g) > MathAF::cEpsilon
				&& fabsf(b - color.b) > MathAF::cEpsilon
				&& fabsf(a - color.a) > MathAF::cEpsilon);
		};
		//+オペレーター
		inline Color operator +(const Color& color) const { return Color(r + color.r, g + color.g, b + color.b, a + color.a); }
		//+オペレーター
		inline Color operator +(float color) const { return Color(r + color, g + color, b + color, a + color); }
		//-オペレーター
		inline Color operator -(const Color& color) const { return Color(r - color.r, g - color.g, b - color.b, a - color.a); }
		//-オペレーター
		inline Color operator -(float color) const { return Color(r - color, g - color, b - color, a - color); }
		//*オペレーター
		inline Color operator *(const Color& color) const { return Color(r * color.r, g * color.g, b * color.b, a * color.a); }
		//*オペレーター
		inline Color operator *(float color) const { return Color(r * color, g * color, b * color, a * color); }
		// /オペレーター
		inline Color operator /(const Color& color) const { return Color(r / color.r, g / color.g, b / color.b, a / color.a); }
		// /オペレーター
		inline Color operator /(float color) const { return Color(r / color, g / color, b / color, a / color); }
		//+=オペレーター
		inline const Color& operator +=(const Color& color) { r += color.r; g += color.g; b += color.b; a += color.a; return *this; }
		//+=オペレーター
		inline const Color& operator +=(float color) { r += color; g += color; b += color; a += color; return *this; }
		//-=オペレーター
		inline const Color& operator -=(const Color& color) { r -= color.r; g += color.g; b -= color.b; a -= color.a; return *this; }
		//-=オペレーター
		inline const Color& operator -=(float color) { r -= color; g -= color; b -= color; a -= color; return *this; }
		//*=オペレーター
		inline const Color& operator *=(const Color& color) { r *= color.r; g *= color.g; b *= color.b; a *= color.a; return *this; }
		//*=オペレーター
		inline const Color& operator *=(float color) { r *= color; g *= color; b *= color; a *= color; return *this; }
		// /=オペレーター
		inline const Color& operator /=(const Color& color) { r /= color.r; g /= color.g; b /= color.b; a /= color.a; return *this; }
		// /=オペレーター
		inline const Color& operator /=(float color) { r /= color; g /= color; b /= color; a /= color; return *this; }
	};
}

#endif // !SGFRAMEWORK_HEADER_COLOR_HPP_
