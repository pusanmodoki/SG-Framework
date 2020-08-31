/*----------------------------------------------------------------------------------
<Color.hpp> contents->色を扱うカラー structure
制作者: Shota Uemura
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_COLOR_HPP_
#define SGFRAMEWORK_HEADER_COLOR_HPP_
#include "../MacroAndUsing/MacroAndUsing.hpp"
#include "../Exception/Exception.hpp"

//Framework namespace
namespace SGFramework
{
	//色を扱うカラー structure
	struct Color
	{
		//コンストラクタ…クリアに初期化
		inline constexpr Color() noexcept : r(0.0f) , g(0.0f) , b(0.0f) , a(0.0f) {}
		//コンストラクタ…代入初期化
		//argument 1: R
		//argument 2: G
		//argument 3: B
		//argument 4: A
		inline constexpr Color(float _r, float _g, float _b, float _a) noexcept : r(_r), g(_g), b(_b), a(_a) {}
		//コンストラクタ…代入初期化
		//argument 1: RGB
		//argument 3: A
		inline constexpr Color(const Color& color, float _a) noexcept : r(color.r), g(color.g), b(color.b), a(_a) {}

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
		inline operator float*() noexcept { return values; }
		//float
		inline operator const float*() const noexcept { return values; }
		//==オペレーター
		inline bool operator ==(const Color& color) const noexcept
		{
			return (fabsf(r - color.r) <= MathAF::cEpsilon
				&& fabsf(g - color.g) <= MathAF::cEpsilon
				&& fabsf(b - color.b) <= MathAF::cEpsilon
				&& fabsf(a - color.a) <= MathAF::cEpsilon);
		};
		//!=オペレーター
		inline bool operator !=(const Color& color) const noexcept
		{
			return (fabsf(r - color.r) > MathAF::cEpsilon
				&& fabsf(g - color.g) > MathAF::cEpsilon
				&& fabsf(b - color.b) > MathAF::cEpsilon
				&& fabsf(a - color.a) > MathAF::cEpsilon);
		};
		//+オペレーター
		inline Color operator +(const Color& color) const noexcept { return Color(r + color.r, g + color.g, b + color.b, a + color.a); }
		//+オペレーター
		inline Color operator +(float value) const noexcept { return Color(r + value, g + value, b + value, a + value); }
		//-オペレーター
		inline Color operator -(const Color& color) const noexcept { return Color(r - color.r, g - color.g, b - color.b, a - color.a); }
		//-オペレーター
		inline Color operator -(float value) const noexcept { return Color(r - value, g - value, b - value, a - value); }
		//*オペレーター
		inline Color operator *(const Color& color) const noexcept { return Color(r * color.r, g * color.g, b * color.b, a * color.a); }
		//*オペレーター
		inline Color operator *(float value) const { return Color(r * value, g * value, b * value, a * value); }
		//+=オペレーター
		inline const Color& operator +=(const Color& color) noexcept { r += color.r; g += color.g; b += color.b; a += color.a; return *this; }
		//+=オペレーター
		inline const Color& operator +=(float value) noexcept { r += value; g += value; b += value; a += value; return *this; }
		//-=オペレーター
		inline const Color& operator -=(const Color& color) noexcept { r -= color.r; g += color.g; b -= color.b; a -= color.a; return *this; }
		//-=オペレーター
		inline const Color& operator -=(float value) noexcept { r -= value; g -= value; b -= value; a -= value; return *this; }
		//*=オペレーター
		inline const Color& operator *=(const Color& color) noexcept { r *= color.r; g *= color.g; b *= color.b; a *= color.a; return *this; }
		//*=オペレーター
		inline const Color& operator *=(float value) noexcept { r *= value; g *= value; b *= value; a *= value; return *this; }
	
		// /オペレーター <possibly throw(debug only)>
		inline Color operator /(const Color& color) const
			//Non debug
#ifndef SGF_DEBUG
			noexcept
#endif // !SGF_DEBUG
		{
			//Debug
#ifdef SGF_DEBUG
			if (color.a == 0.0f || color.b == 0.0f || color.b == 0.0f || color.a == 0.0f)
				throw Exception::InvalidArgumentException("Color", "operator /", "either divisor == 0.0f.");
#endif // SGF_DEBUG
				return Color(r / color.r, g / color.g, b / color.b, a / color.a);
		}
		
		// /オペレーター <possibly throw(debug only)>
		inline Color operator /(float value) const
			//Non debug
#ifndef SGF_DEBUG
			noexcept
#endif // !SGF_DEBUG
		{
			//Debug
#ifdef SGF_DEBUG
			if (value == 0.0f) throw Exception::InvalidArgumentException("Color", "operator /", "divisor == 0.0f.");
#endif // SGF_DEBUG
			return Color(r / value, g / value, b / value, a / value); 
		}

		// /=オペレーター <possibly throw(debug only)>
		inline const Color& operator /=(const Color& color)
			//Non debug
#ifndef SGF_DEBUG
			noexcept
#endif // !SGF_DEBUG
		{
			//Debug
#ifdef SGF_DEBUG
			if (color.a == 0.0f || color.b == 0.0f || color.b == 0.0f || color.a == 0.0f)
				throw Exception::InvalidArgumentException("Color", "operator /=", "either divisor == 0.0f.");
#endif // SGF_DEBUG
			r /= color.r; g /= color.g; b /= color.b; a /= color.a;  return *this; 
		}

		// /=オペレーター <possibly throw(debug only)>
		inline const Color& operator /=(float value)
			//Non debug
#ifndef SGF_DEBUG
			noexcept
#endif // !SGF_DEBUG
		{ 
			//Debug
#ifdef SGF_DEBUG
			if (value == 0.0f) throw Exception::InvalidArgumentException("Color", "operator /=", "divisor == 0.0f.");
#endif // SGF_DEBUG
			r /= value; g /= value; b /= value; a /= value; return *this;
		}
	};
}

#endif // !SGFRAMEWORK_HEADER_COLOR_HPP_
