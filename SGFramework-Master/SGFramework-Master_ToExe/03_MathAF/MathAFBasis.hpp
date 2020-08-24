/*----------------------------------------------------------------------------------
計算ライブラリなMathAF namespace (math + almost float)
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_CMATHF_BASIS_HPP_
#define SGFRAMEWORK_HEADER_CMATHF_BASIS_HPP_
#include "../02_CustomVariable/ConstAndUsing/ConstAndUsing.hpp"

//Framework namespace
namespace SGFramework
{
	//計算ライブラリ
	namespace MathAF
	{
		//----------------------------------------------------------------------------------
		//[Absolute]
		//return: fの絶対値
		//引数1: f
		template<class T>
		inline constexpr T Absolute(T f) noexcept { return f < 0 ? -f : f; }
		//----------------------------------------------------------------------------------
		//[IsNaN]
		//return: fがnanならtrue
		//引数1: f
		template<class FloationgType>
		inline constexpr bool IsNaN(FloationgType f) noexcept { static_assert(std::is_floating_point_v<FloationgType>, L"Error value is Floationg"); return !(f == f); }
		//----------------------------------------------------------------------------------
		//[IsZero]
		//return: fが0(誤差含む)ならtrue
		//引数1: f
		template<class FloationgType>
		inline constexpr bool IsZero(FloationgType f) noexcept { static_assert(std::is_floating_point_v<FloationgType>, L"Error value is Floationg");  return (-cEpsilon > f && f > cEpsilon); }
		//----------------------------------------------------------------------------------
		//[IsMinusSign]
		//return: fが-の値ならtrue
		//引数1: f
		template<class FloationgType>
		inline constexpr bool IsMinusSign(FloationgType f) noexcept { static_assert(std::is_floating_point_v<FloationgType>, L"Error value is Floationg");  return (!IsNaN(f) && f < 0); }
		//----------------------------------------------------------------------------------
		//[CopySign]
		//return: valueにsignの符号をコピーしたもの
		//引数1: value
		//引数2: sign
		template<class FloationgType>
		inline constexpr float CopySign(FloationgType value, FloationgType sign) noexcept
		{
			static_assert(std::is_floating_point_v<FloationgType>, L"Error value is Floationg");
			float absoluteOrNaN= IsNaN(value) ? cNaN : Absolute(value);
			return sign >= 0 ? absoluteOrNaN : -absoluteOrNaN;
		}

		//----------------------------------------------------------------------------------
		//[Clamp]
		//fをmin ~ maxの間で制限する
		//return: min ~ max で制限させた値
		//引数1: f
		//引数2: 最小制限値
		//引数3: 最大制限値
		template<class T>
		inline constexpr T Clamp(T f, T min, T max) noexcept
		{
			if (f < min) return min;
			else if (f > max) return max;
			return f;
		}
		//----------------------------------------------------------------------------------
		//[Clamp01]
		//fを0 ~ 1の間で制限する
		//return: 0 ~ 1 で制限させた値
		//引数1: f
		//引数2: 最小制限値
		//引数3: 最大制限値
		template<class FloationgType>
		inline constexpr float Clamp01(FloationgType f) noexcept
		{
			static_assert(std::is_floating_point_v<FloationgType>, L"Error value is Floationg");
			if (f < 0.0f) return 0.0f;
			else if (f > 1.0f) return  1.0f;
			return f;
		}
		//----------------------------------------------------------------------------------
		//[Max]
		//f1とf2を比較する
		//return: 大きい方の値
		//引数1: f1
		//引数2: f2
		template<class T>
		inline constexpr T Max(T f1, T f2) noexcept { return f1 > f2 ? f1 : f2; }
		//----------------------------------------------------------------------------------
		//[MaxWithAbsolute]
		//f1とf2の絶対値を比較する
		//return: 絶対値の大きい方の値
		//引数1: f1
		//引数2: f2
		template<class T>
		inline constexpr T MaxWithAbsolute(T f1, T f2) noexcept { return Absolute(f1) > Absolute(f2) ? f1 : f2; }
		//----------------------------------------------------------------------------------
		//[Min]
		//f1とf2を比較する
		//return: 小さい方の値
		//引数1: f1
		//引数2: f2
		template<class T>
		inline constexpr T Min(T f1, T f2) noexcept { return f1 < f2 ? f1 : f2; }
		//----------------------------------------------------------------------------------
		//[MinWithAbsolute]
		//f1とf2の絶対値を比較する
		//return: 絶対値の小さい方の値
		//引数1: f1
		//引数2: f2
		template<class T>
		inline constexpr T MinWithAbsolute(T f1, T f2) noexcept { return Absolute(f1) < Absolute(f2) ? f1 : f2; }
	}
}

#endif // !SGFRAMEWORK_HEADER_CMATHF_BASIS_HPP_