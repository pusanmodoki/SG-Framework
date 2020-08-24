/*----------------------------------------------------------------------------------
�v�Z���C�u������MathAF namespace (math + almost float)
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
	//�v�Z���C�u����
	namespace MathAF
	{
		//----------------------------------------------------------------------------------
		//[Floor]
		//return: f�̐؂�̂�
		//����1: f
		inline constexpr float Floor(float f) noexcept { return static_cast<float>(f < 0.0f ? static_cast<int>(f) - 1 : static_cast<int>(f)); }
		//----------------------------------------------------------------------------------
		//[Floor]
		//return: f�̐؂�̂�(int)
		//����1: f
		inline constexpr int FloorToInt(float f) noexcept { return f < 0.0f ? static_cast<int>(f) - 1 : static_cast<int>(f); }
		//----------------------------------------------------------------------------------
		//[Ceil]
		//return: f�̐؂�グ
		//����1: f
		inline constexpr float Ceil(float f) noexcept { return static_cast<float>(f < 0.0f ? static_cast<int>(f) : static_cast<int>(f) + 1); }
		//----------------------------------------------------------------------------------
		//[CeilToInt]
		//return: f�̐؂�グ(int)
		//����1: f
		inline constexpr int CeilToInt(float f) noexcept { return f < 0.0f ? static_cast<int>(f) : static_cast<int>(f) + 1; }
		//----------------------------------------------------------------------------------
		//[Round]
		//return: f�̋����ۂ�
		//����1: f
		inline constexpr float Round(float f) noexcept { return static_cast<float>(static_cast<int>(f + 0.5f)); }
		//----------------------------------------------------------------------------------
		//[RoundInt]
		//return: f�̋����ۂ�(int)
		//����1: f
		inline constexpr int RoundToInt(float f) noexcept { return static_cast<int>(f + 0.5f); }

		//----------------------------------------------------------------------------------
		//[Mod]
		//return: f��p�̂��܂�Z, f%p
		//����1: f
		//����1: p
		inline constexpr float Mod(float f, float p) noexcept
		{
			p = Absolute(p);
			float result = Absolute(f) - static_cast<float>(static_cast<int>(Absolute(f) / p)) * p;
			if (IsMinusSign(result)) result += p;
			return CopySign(result, f);
		}

		//----------------------------------------------------------------------------------
		//[PingPong]
		//t��0 ~ length�̊Ԃōs����������
		//return: t��0 ~ length�̊Ԃɐ���, �s�����������l
		//����1: t
		//����2: �����l
		inline constexpr float PingPong(float t, float length) noexcept
		{
			return Mod(t / length, 2) < 1.0f ? Mod(t, length) : length - Mod(t, length);
		}
	}
}
#endif // !SGFRAMEWORK_HEADER_CMATHF_FLOAT_EDIT_HPP_
