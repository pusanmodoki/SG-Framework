/*----------------------------------------------------------------------------------
�v�Z���C�u������MathAF namespace (math + almost float)
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_CMATHF_BASIS_HPP_
#define SGFRAMEWORK_HEADER_CMATHF_BASIS_HPP_
#include "../02_CustomVariable/ConstAndUsing/ConstAndUsing.hpp"

//Framework namespace
namespace SGFramework
{
	//�v�Z���C�u����
	namespace MathAF
	{
		//----------------------------------------------------------------------------------
		//[Absolute]
		//return: f�̐�Βl
		//����1: f
		template<class T>
		inline constexpr T Absolute(T f) noexcept { return f < 0 ? -f : f; }
		//----------------------------------------------------------------------------------
		//[IsNaN]
		//return: f��nan�Ȃ�true
		//����1: f
		template<class FloationgType>
		inline constexpr bool IsNaN(FloationgType f) noexcept { static_assert(std::is_floating_point_v<FloationgType>, L"Error value is Floationg"); return !(f == f); }
		//----------------------------------------------------------------------------------
		//[IsZero]
		//return: f��0(�덷�܂�)�Ȃ�true
		//����1: f
		template<class FloationgType>
		inline constexpr bool IsZero(FloationgType f) noexcept { static_assert(std::is_floating_point_v<FloationgType>, L"Error value is Floationg");  return (-cEpsilon > f && f > cEpsilon); }
		//----------------------------------------------------------------------------------
		//[IsMinusSign]
		//return: f��-�̒l�Ȃ�true
		//����1: f
		template<class FloationgType>
		inline constexpr bool IsMinusSign(FloationgType f) noexcept { static_assert(std::is_floating_point_v<FloationgType>, L"Error value is Floationg");  return (!IsNaN(f) && f < 0); }
		//----------------------------------------------------------------------------------
		//[CopySign]
		//return: value��sign�̕������R�s�[��������
		//����1: value
		//����2: sign
		template<class FloationgType>
		inline constexpr float CopySign(FloationgType value, FloationgType sign) noexcept
		{
			static_assert(std::is_floating_point_v<FloationgType>, L"Error value is Floationg");
			float absoluteOrNaN= IsNaN(value) ? cNaN : Absolute(value);
			return sign >= 0 ? absoluteOrNaN : -absoluteOrNaN;
		}

		//----------------------------------------------------------------------------------
		//[Clamp]
		//f��min ~ max�̊ԂŐ�������
		//return: min ~ max �Ő����������l
		//����1: f
		//����2: �ŏ������l
		//����3: �ő吧���l
		template<class T>
		inline constexpr T Clamp(T f, T min, T max) noexcept
		{
			if (f < min) return min;
			else if (f > max) return max;
			return f;
		}
		//----------------------------------------------------------------------------------
		//[Clamp01]
		//f��0 ~ 1�̊ԂŐ�������
		//return: 0 ~ 1 �Ő����������l
		//����1: f
		//����2: �ŏ������l
		//����3: �ő吧���l
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
		//f1��f2���r����
		//return: �傫�����̒l
		//����1: f1
		//����2: f2
		template<class T>
		inline constexpr T Max(T f1, T f2) noexcept { return f1 > f2 ? f1 : f2; }
		//----------------------------------------------------------------------------------
		//[MaxWithAbsolute]
		//f1��f2�̐�Βl���r����
		//return: ��Βl�̑傫�����̒l
		//����1: f1
		//����2: f2
		template<class T>
		inline constexpr T MaxWithAbsolute(T f1, T f2) noexcept { return Absolute(f1) > Absolute(f2) ? f1 : f2; }
		//----------------------------------------------------------------------------------
		//[Min]
		//f1��f2���r����
		//return: ���������̒l
		//����1: f1
		//����2: f2
		template<class T>
		inline constexpr T Min(T f1, T f2) noexcept { return f1 < f2 ? f1 : f2; }
		//----------------------------------------------------------------------------------
		//[MinWithAbsolute]
		//f1��f2�̐�Βl���r����
		//return: ��Βl�̏��������̒l
		//����1: f1
		//����2: f2
		template<class T>
		inline constexpr T MinWithAbsolute(T f1, T f2) noexcept { return Absolute(f1) < Absolute(f2) ? f1 : f2; }
	}
}

#endif // !SGFRAMEWORK_HEADER_CMATHF_BASIS_HPP_