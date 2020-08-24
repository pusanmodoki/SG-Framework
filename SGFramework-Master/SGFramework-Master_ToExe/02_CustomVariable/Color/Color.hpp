/*----------------------------------------------------------------------------------
色を扱うカラー structureを記述したColor.hpp
----------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_COLOR_HPP_
#define SGFRAMEWORK_HEADER_COLOR_HPP_
#include "../Vector/Vector.hpp"
#include "../MacroAndUsing/MacroAndUsing.hpp"

//Framework namespace
namespace SGFramework
{
	//色を扱うカラー structure
	struct Color
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

		//Const colors
		static const Color black;		//black (0.0f, 0.0f, 0.0f, 1.0f)
		static const Color blue;		//blue (0.0f, 0.0f, 1.0f, 1.0f)
		static const Color clear;		//clear (0.0f, 0.0f, 0.0f, 0.0f)
		static const Color cyan;		//cyan (0.0f, 1.0f, 1.0f, 1.0f)
		static const Color gray;		//gray (0.5f, 0.5f, 0.5f, 1.0f)
		static const Color green;		//green (0.0f, 1.0f, 0.0f, 1.0f)
		static const Color magenta;	//magenta (1.0f, 0.0f, 1.0f, 1.0f)
		static const Color red;			//red (1.0f, 0.0f, 0.0f, 1.0f)
		static const Color white;		//white (1.0f, 1.0f, 1.0f, 1.0f)
		static const Color yellow;	//yellow (1.0f, 1.0f, 0.0f, 1.0f)

		//変数
		union
		{
			struct
			{
				float r;		//R
				float g;		//G
				float b;		//B
				float a;		//A
			};
			float values[4];	//values
		};
		
		//floatオペレーター
		inline operator float*() { return values; }
		//float
		inline operator const float*() const { return values; }
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
