/*----------------------------------------------------------------------------------
�v�Z���C�u������MathAF namespace (math + almost float)
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_CMATHF_DECLARATION_HPP_
#define SGFRAMEWORK_HEADER_CMATHF_DECLARATION_HPP_
#include <cmath>
#include <math.h>
#include "../02_CustomVariable/CustomVariable.hpp"
#include "MathAFBasis.hpp"
#include "MathAFFloatEdit.hpp"

//Framework namespace
namespace SGFramework
{
	//�v�Z���C�u����
	namespace MathAF
	{
		//----------------------------------------------------------------------------------
		//[Cos]
		//return: cosine(f)
		//����1: f (radian)
		inline float Cos(float f) noexcept { return std::cosf(f); }
		//----------------------------------------------------------------------------------
		//[Sin]
		//return: sine(f)
		//����1: f (radian)
		inline float Sin(float f) noexcept { return std::sinf(f); }
		//----------------------------------------------------------------------------------
		//[Tan]
		//return: tangent(f)
		//����1: f (radian)
		inline float Tan(float f) noexcept { return std::tanf(f); }
		//----------------------------------------------------------------------------------
		//[Acos]
		//return: arcCosine(f)
		//����1: f (radian)
		inline float Acos(float f) noexcept { return std::acosf(f); }
		//----------------------------------------------------------------------------------
		//[Asin]
		//return: arcSine(f)
		//����1: f (radian)
		inline float Asin(float f) noexcept { return std::asinf(f); }
		//----------------------------------------------------------------------------------
		//[Atan]
		//return: arcAangent(f)
		//����1: f (radian)
		inline float Atan(float f) noexcept { return std::atanf(f); }
		//----------------------------------------------------------------------------------
		//[Atan2]
		//return: arcAangent2(f)
		//����1: f (radian)
		inline float Atan2(float y, float x) noexcept { return std::atan2f(y, x); }
		//----------------------------------------------------------------------------------
		//[Sqrt]
		//return: f�̕�����, Fast inverse square root��p����̂Œᐸ�x & ����
		//����1: f
		inline constexpr float Sqrt(float f) noexcept;
		//----------------------------------------------------------------------------------
		//[SqrtLowAccuracy]
		//return: f�̕�����, Sqrt�����X�ɒᐸ�x�����ō���
		//����1: f
		inline constexpr float SqrtLowAccuracy(float f) noexcept;
		//----------------------------------------------------------------------------------
		//[SqrtHighAccuracy]
		//return: f�̕�����, Sqrt��荂���x & �ᑬ
		//����1: f
		inline float SqrtHighAccuracy(float f) noexcept { return std::sqrtf(f); }
		//----------------------------------------------------------------------------------
		//[Pow]
		//return: f��p��, p = int�̕�������
		//����1: f
		//����2: p
		inline float Pow(float f, float p) noexcept {return std::powf(f, p); }
		//----------------------------------------------------------------------------------
		//[Pow]
		//return: f��p��
		//����1: f
		//����2: p
		inline constexpr float Pow(float f, int p) noexcept;
		//----------------------------------------------------------------------------------
		//[Pow2]
		//return: f��2��
		//����1: f
		inline constexpr float Pow2(float f) noexcept { return f * f; }
		//----------------------------------------------------------------------------------
		//[Pow3]
		//return: f��3��
		//����1: f
		inline constexpr float Pow3(float f) noexcept { return f * f * f; }
		//----------------------------------------------------------------------------------
		//[Pow4]
		//return: f��4��
		//����1: f
		inline constexpr float Pow4(float f) noexcept { return f * f * f * f; }
		//----------------------------------------------------------------------------------
		//[Pow5]
		//return: f��5��
		//����1: f
		inline constexpr float Pow5(float f) noexcept { return f * f * f * f * f; }
		//----------------------------------------------------------------------------------
		//[Log]
		//return: f�̑ΐ�
		//����1: f
		inline float Log(float f) noexcept { return std::logf(f); }
		//----------------------------------------------------------------------------------
		//[Log10]
		//return: f�̒ꂪ10�ȑΐ�
		//����1: f
		inline float Log10(float f) noexcept { return std::log10f(f); }

		//----------------------------------------------------------------------------------
		//[Lerp3D]
		//from��to�̊Ԃ�t�Ő��`�⊮�������̂�ԋp����, �⊮�ɂ�3���֐��⊮���g��
		//return: from����to�ւ̐��`(3���֐�)���
		//����1: from
		//����2: to
		//����3: �⊮�̓x����(0 ~ 1)s
		inline constexpr float Lerp3D(float from, float to, float t) noexcept;
		//----------------------------------------------------------------------------------
		//[LerpDirect]
		//from��to�̊Ԃ�t�Ő��`�⊮�������̂�ԋp����, �⊮�ɂ͂����̐��`��Ԃ��g��
		//return: from����to�ւ̐��`���
		//����1: from
		//����2: to
		//����3: �⊮�̓x����(0 ~ 1)
		inline constexpr float LerpDirect(float from, float to, float t) noexcept;

		//----------------------------------------------------------------------------------
		//[GreatestCommonDenominator]
		//return: A��B�̍ő����
		//����1: ����A
		//����1: ����B
		inline constexpr int GreatestCommonDenominator(int a, int b) noexcept;
		//----------------------------------------------------------------------------------
		//[AspectRatio]
		//return: A��B�̃A�X�y�N�g��
		//����1: �A�X�y�N�g����Z�o����Vector
		inline constexpr Vector2Int AspectRatio(Vector2Int vec) noexcept;


		//----------------------------------------------------------------------------------
		//[NormalDirection]
		//return: �O�_�����ʂ̖@���x�N�g��
		//����1: ���_1
		//����2: ���_2
		//����3: ���_3
		inline SGFramework::Vector3 NormalDirection(const SGFramework::Vector3& vertex1, 
			const SGFramework::Vector3& vertex2, const SGFramework::Vector3& vertex3) noexcept { return Vector3::Cross(vertex2 - vertex1, vertex3 - vertex2).Normalized(); }
		//----------------------------------------------------------------------------------
		//[DistanceWithPointToPlane]
		//return: ���W�ƕ��ʂ̍ŒZ����
		//����1: ���W
		//����2: ���ʂ̍��W
		//����3: ���ʂ̖@��
		inline float DistanceWithPointToPlane(const SGFramework::Vector3& point, 
			const SGFramework::Vector3& planePos, const SGFramework::Vector3& planeNormal) noexcept { return Absolute(Vector3::Dot(planeNormal, point - planePos)); }

		//----------------------------------------------------------------------------------
		//[AspectRatioSettingX]
		//x����Ɏw�肵���A�X�y�N�g���Vector���Z�o����
		//return: x����Ɍv�Z�����A�X�y�N�g��
		//����1: ��ɂ���x
		//����2: �A�X�y�N�g��
		inline constexpr SGFramework::Vector2 AspectRatioSettingX(float x, const SGFramework::Vector2& aspectRadio) noexcept;
		//----------------------------------------------------------------------------------
		//[AspectRatioSettingX]
		//x����Ɏw�肵���A�X�y�N�g���Vector���Z�o����
		//return: x����Ɍv�Z�����A�X�y�N�g��
		//����1: ��ɂ���x
		//����2: �A�X�y�N�g��
		inline constexpr Vector2Int AspectRatioSettingX(int x, const Vector2Int& aspectRadio) noexcept;
		//----------------------------------------------------------------------------------
		//[AspectRatioSettingY]
		//y����Ɏw�肵���A�X�y�N�g���Vector���Z�o����
		//return: y����Ɍv�Z�����A�X�y�N�g��
		//����1: ��ɂ���y
		//����2: �A�X�y�N�g��
		inline constexpr SGFramework::Vector2 AspectRatioSettingY(float y, const SGFramework::Vector2& aspectRadio) noexcept;
		//----------------------------------------------------------------------------------
		//[AspectRatioSettingY]
		//y����Ɏw�肵���A�X�y�N�g���Vector���Z�o����
		//return: y����Ɍv�Z�����A�X�y�N�g��
		//����1: ��ɂ���y
		//����2: �A�X�y�N�g��
		inline constexpr Vector2Int AspectRatioSettingY(int y, const Vector2Int& aspectRadio) noexcept;







		//----------------------------------------------------------------------------------
		//[Sqrt]
		//return: f�̕�����, Fast inverse square root��p����̂Ő��x�͒��
		//����1: f (radian)
		inline constexpr float Sqrt(float f) noexcept
		{
			int i = 0x5f3759df - (*reinterpret_cast<int*>(&f) >> 1);
			float bitF = *reinterpret_cast<float*>(&i);

			bitF *= (1.5f - 0.5f * f * bitF * bitF);
			return f * bitF * (1.5f - 0.5f * f * bitF * bitF);
		}
		//----------------------------------------------------------------------------------
		//[SqrtLowAccuracy]
		//return: f�̕�����, Sqrt�����X�ɒᐸ�x�����ō���
		//����1: f
		constexpr float SqrtLowAccuracy(float f) noexcept
		{
			int i = 0x5f3759df - (*reinterpret_cast<int*>(&f) >> 1);
			float bitF = *reinterpret_cast<float*>(&i);

			return f * bitF * (1.5f - 0.5f * f * bitF * bitF);
		}

		//----------------------------------------------------------------------------------
		//[Pow]
		//return: f��p��
		//����1: f
		//����2: p
		inline constexpr float Pow(float f, int p) noexcept
		{
			if (IsZero(f) || p == 0) return 1.0f;

			float result = 1.0f;
			float mult = (p < 0 ? 1.0f / f : f);

			for (int i = 0; i < (p < 0 ? -p : p); i++)
				result *= mult;

			return result;
		}

		//----------------------------------------------------------------------------------
		//[Lerp3D]
		//from��to�̊Ԃ�t�Ő��`�⊮�������̂�ԋp����, �⊮�ɂ�3���֐��⊮���g��
		//return: from����to�ւ̐��`(3���֐�)���
		//����1: from
		//����2: to
		//����3: �⊮�̓x����(0 ~ 1)s
		inline constexpr float Lerp3D(float from, float to, float t) noexcept
		{
			if (t > 1.0f) t = 1.0f;	else if (t < 0.0f) t = 0.0f;
			//3���֐����
			float rate = t * t * (3.0f - 2.0f * t);

			//return
			return from * (1.0f - rate) + to * rate;
		}
		//----------------------------------------------------------------------------------
		//[LerpDirect]
		//from��to�̊Ԃ�t�Ő��`�⊮�������̂�ԋp����, �⊮�ɂ͂����̐��`��Ԃ��g��
		//return: from����to�ւ̐��`���
		//����1: from
		//����2: to
		//����3: �⊮�̓x����(0 ~ 1)
		inline constexpr float LerpDirect(float from, float to, float t) noexcept
		{
			if (t > 1.0f) t = 1.0f;	else if (t < 0.0f) t = 0.0f;
			//return
			return from * (1.0f - t) + to * t;
		}

		//----------------------------------------------------------------------------------
		//[GreatestCommonDenominator]
		//return: A��B�̍ő����
		//����1: ����A
		//����1: ����B
		inline constexpr int GreatestCommonDenominator(int a, int b) noexcept
		{
			// - �̒l�̏ꍇ�������������̂ŏI��
			if (a < 0 || b < 0)	return 0;
			//b��a���傫���ꍇ�t�����s
			else if (a < b)		return GreatestCommonDenominator(b, a);
			else
			{
				//���[�N���b�h�̌ݏ��@
				while (b != 0)
				{
					int temp = a % b;
					a = b;
					b = temp;
				}
				return a;
			}
		}
		//----------------------------------------------------------------------------------
		//[AspectRatio]
		//return: A��B�̃A�X�y�N�g��
		//����1: �A�X�y�N�g����Z�o����Vector
		inline constexpr SGFramework::Vector2Int AspectRatio(SGFramework::Vector2Int vec) noexcept
		{
			int gcd = GreatestCommonDenominator(vec.x, vec.y);
			vec.x /= gcd;
			vec.y /= gcd;
			return vec;
		}

		//----------------------------------------------------------------------------------
		//[AspectRatioSettingX]
		//x����Ɏw�肵���A�X�y�N�g���Vector���Z�o����
		//return: x����Ɍv�Z�����A�X�y�N�g��
		//����1: ��ɂ���x
		//����2: �A�X�y�N�g��
		inline constexpr SGFramework::Vector2 AspectRatioSettingX(float x, const SGFramework::Vector2& aspectRadio) noexcept
		{
			return SGFramework::Vector2(x, x * (aspectRadio.y / aspectRadio.x));
		}
		//----------------------------------------------------------------------------------
		//[AspectRatioSettingX]
		//x����Ɏw�肵���A�X�y�N�g���Vector���Z�o����
		//return: x����Ɍv�Z�����A�X�y�N�g��
		//����1: ��ɂ���x
		//����2: �A�X�y�N�g��
		inline constexpr SGFramework::Vector2Int AspectRatioSettingX(int x, const SGFramework::Vector2Int& aspectRadio) noexcept
		{
			return Vector2Int(x, static_cast<int>(static_cast<float>(x) * (static_cast<float>(aspectRadio.y) / static_cast<float>(aspectRadio.x))));
		}
		//----------------------------------------------------------------------------------
		//[AspectRatioSettingY]
		//y����Ɏw�肵���A�X�y�N�g���Vector���Z�o����
		//return: y����Ɍv�Z�����A�X�y�N�g��
		//����1: ��ɂ���y
		//����2: �A�X�y�N�g��
		inline constexpr SGFramework::Vector2 AspectRatioSettingY(float y, const SGFramework::Vector2& aspectRadio) noexcept
		{
			return SGFramework::Vector2(y, y * (aspectRadio.x / aspectRadio.y));
		}
		//----------------------------------------------------------------------------------
		//[AspectRatioSettingY]
		//y����Ɏw�肵���A�X�y�N�g���Vector���Z�o����
		//return: y����Ɍv�Z�����A�X�y�N�g��
		//����1: ��ɂ���y
		//����2: �A�X�y�N�g��
		inline constexpr SGFramework::Vector2Int AspectRatioSettingY(int y, const SGFramework::Vector2Int& aspectRadio) noexcept
		{
			return SGFramework::Vector2Int(y, static_cast<int>(static_cast<float>(y) *
				(static_cast<float>(aspectRadio.x) / static_cast<float>(aspectRadio.y))));
		}

	}
}
#endif // !SGFRAMEWORK_HEADER_CMATHF_DECLARATION_HPP_
