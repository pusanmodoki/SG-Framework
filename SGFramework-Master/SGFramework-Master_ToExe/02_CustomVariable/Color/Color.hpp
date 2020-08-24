/*----------------------------------------------------------------------------------
�F�������J���[�\����
----------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_COLOR_HPP_
#define SGFRAMEWORK_HEADER_COLOR_HPP_
#include "../Vector/Vector.hpp"
#include "../ConstAndUsing/ConstAndUsing.hpp"
#include "../Property/Property.hpp"

//Framework namespace
namespace SGFramework
{
	//�F�������J���[�\����
	struct Color final
	{
		//�R���X�g���N�^�c�N���A�ɏ�����
		inline constexpr Color() : r(0.0f) , g(0.0f) , b(0.0f) , a(0.0f) {}
		//�R���X�g���N�^�c���������
		//����1: R
		//����2: G
		//����3: B
		//����4: A
		inline constexpr Color(float _r, float _g, float _b, float _a) : r(_r), g(_g), b(_b), a(_a) {}
		//�R���X�g���N�^�c���������
		//����1: RGB
		//����3: A
		inline constexpr Color(const Color& color, float _a) : r(color.r), g(color.g), b(color.b), a(_a) {}

		//�ϐ�
		float r;		//R
		float g;		//G
		float b;		//B
		float a;		//A
		
		
		//float�I�y���[�^�[
		inline operator float*(void) { return (float*)this; }
		//float
		inline operator const float*(void) const { return (const float*)this; }
		//==�I�y���[�^�[
		inline bool operator ==(const Color& color) const
		{
			return (fabsf(r - color.r) <= MathAF::cEpsilon
				&& fabsf(g - color.g) <= MathAF::cEpsilon
				&& fabsf(b - color.b) <= MathAF::cEpsilon
				&& fabsf(a - color.a) <= MathAF::cEpsilon);
		};
		//!=�I�y���[�^�[
		inline bool operator !=(const Color& color) const
		{
			return (fabsf(r - color.r) > MathAF::cEpsilon
				&& fabsf(g - color.g) > MathAF::cEpsilon
				&& fabsf(b - color.b) > MathAF::cEpsilon
				&& fabsf(a - color.a) > MathAF::cEpsilon);
		};
		//+�I�y���[�^�[
		inline Color operator +(const Color& color) const { return Color(r + color.r, g + color.g, b + color.b, a + color.a); }
		//+�I�y���[�^�[
		inline Color operator +(float color) const { return Color(r + color, g + color, b + color, a + color); }
		//-�I�y���[�^�[
		inline Color operator -(const Color& color) const { return Color(r - color.r, g - color.g, b - color.b, a - color.a); }
		//-�I�y���[�^�[
		inline Color operator -(float color) const { return Color(r - color, g - color, b - color, a - color); }
		//*�I�y���[�^�[
		inline Color operator *(const Color& color) const { return Color(r * color.r, g * color.g, b * color.b, a * color.a); }
		//*�I�y���[�^�[
		inline Color operator *(float color) const { return Color(r * color, g * color, b * color, a * color); }
		// /�I�y���[�^�[
		inline Color operator /(const Color& color) const { return Color(r / color.r, g / color.g, b / color.b, a / color.a); }
		// /�I�y���[�^�[
		inline Color operator /(float color) const { return Color(r / color, g / color, b / color, a / color); }
		//+=�I�y���[�^�[
		inline const Color& operator +=(const Color& color) { r += color.r; g += color.g; b += color.b; a += color.a; return *this; }
		//+=�I�y���[�^�[
		inline const Color& operator +=(float color) { r += color; g += color; b += color; a += color; return *this; }
		//-=�I�y���[�^�[
		inline const Color& operator -=(const Color& color) { r -= color.r; g += color.g; b -= color.b; a -= color.a; return *this; }
		//-=�I�y���[�^�[
		inline const Color& operator -=(float color) { r -= color; g -= color; b -= color; a -= color; return *this; }
		//*=�I�y���[�^�[
		inline const Color& operator *=(const Color& color) { r *= color.r; g *= color.g; b *= color.b; a *= color.a; return *this; }
		//*=�I�y���[�^�[
		inline const Color& operator *=(float color) { r *= color; g *= color; b *= color; a *= color; return *this; }
		// /=�I�y���[�^�[
		inline const Color& operator /=(const Color& color) { r /= color.r; g /= color.g; b /= color.b; a /= color.a; return *this; }
		// /=�I�y���[�^�[
		inline const Color& operator /=(float color) { r /= color; g /= color; b /= color; a /= color; return *this; }
	};
}

#endif // !SGFRAMEWORK_HEADER_COLOR_HPP_
