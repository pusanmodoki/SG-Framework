/*----------------------------------------------------------------------------------
計算ライブラリなMathAF namespace (math + almost float)
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_CMATHF_FLOAT_EDIT_HPP_
#define SGFRAMEWORK_HEADER_CMATHF_FLOAT_EDIT_HPP_
#include <cmath>
#include <limits>
#include "../02_CustomVariable/CustomVariable.hpp"
#include "MathAFBasis.hpp"

//Framework namespace
namespace SGFramework
{
	//計算ライブラリ
	namespace MathAF
	{
		//----------------------------------------------------------------------------------
		//[Floor]
		//return: fの切り捨て
		//引数1: f
		inline constexpr float Floor(float f) noexcept { return static_cast<float>(f < 0.0f ? static_cast<int>(f) - 1 : static_cast<int>(f)); }
		//----------------------------------------------------------------------------------
		//[Floor]
		//return: fの切り捨て(int)
		//引数1: f
		inline constexpr int FloorToInt(float f) noexcept { return f < 0.0f ? static_cast<int>(f) - 1 : static_cast<int>(f); }
		//----------------------------------------------------------------------------------
		//[Ceil]
		//return: fの切り上げ
		//引数1: f
		inline constexpr float Ceil(float f) noexcept { return static_cast<float>(f < 0.0f ? static_cast<int>(f) : static_cast<int>(f) + 1); }
		//----------------------------------------------------------------------------------
		//[CeilToInt]
		//return: fの切り上げ(int)
		//引数1: f
		inline constexpr int CeilToInt(float f) noexcept { return f < 0.0f ? static_cast<int>(f) : static_cast<int>(f) + 1; }
		//----------------------------------------------------------------------------------
		//[Round]
		//return: fの偶数丸め
		//引数1: f
		inline constexpr float Round(float f) noexcept { return static_cast<float>(static_cast<int>(f + 0.5f)); }
		//----------------------------------------------------------------------------------
		//[RoundInt]
		//return: fの偶数丸め(int)
		//引数1: f
		inline constexpr int RoundToInt(float f) noexcept { return static_cast<int>(f + 0.5f); }

		//----------------------------------------------------------------------------------
		//[Mod]
		//return: fとpのあまり算, f%p
		//引数1: f
		//引数1: p
		inline constexpr float Mod(float f, float p) noexcept
		{
			p = Absolute(p);
			float result = Absolute(f) - static_cast<float>(static_cast<int>(Absolute(f) / p)) * p;
			if (IsMinusSign(result)) result += p;
			return CopySign(result, f);
		}

		//----------------------------------------------------------------------------------
		//[PingPong]
		//tを0 ~ lengthの間で行き来させる
		//return: tを0 ~ lengthの間に制限, 行き来させた値
		//引数1: t
		//引数2: 制限値
		inline constexpr float PingPong(float t, float length) noexcept
		{
			return Mod(t / length, 2) < 1.0f ? Mod(t, length) : length - Mod(t, length);
		}
	}
}
#endif // !SGFRAMEWORK_HEADER_CMATHF_FLOAT_EDIT_HPP_
