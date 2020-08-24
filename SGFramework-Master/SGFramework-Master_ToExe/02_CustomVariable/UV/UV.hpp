/*----------------------------------------------------------------------------------
2�̒l��������UV �\����
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_UV_HPP_
#define SGFRAMEWORK_HEADER_UV_HPP_
#include <DirectXMath.h>
#include <cmath>
#include <iostream>
#include "../ConstAndUsing/ConstAndUsing.hpp"
#include "../Property/Property.hpp"
#include "../String/String.hpp"
#include "../Vector/Vector.hpp"

//Framework namespace
namespace SGFramework
{
	//2�̒l��������UV �\����
	struct UV final
	{
	public:
		//----------------------------------------------------------------------------------
		//inlne����

		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�S��0�ɐݒ肳���
		inline constexpr UV() : u(0.0f), v(0.0f) {};
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�������������
		//����1:x
		//����2:y
		inline constexpr UV(float u, float v) : u(u), v(v) {}
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�������������
		//����1: XMFLOAT2
		inline constexpr UV(const Vector2& value) : u(value.x), v(value.y) {}
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�������������
		//����1: float*
		inline constexpr UV(const float* array) : u(array[0]), v(array[1]) {}

		//----------------------------------------------------------------------------------
		//�ϐ�
		union
		{
			struct
			{
				float u;		//U
				float v;		//V
			};
			float value[2];	//Value
		};

		//----------------------------------------------------------------------------------
		//[Absolute]
		//return: ��Βl
		inline UV Absolute() const noexcept;
		//----------------------------------------------------------------------------------
		//[Magnitude]
		//return: ���g��Vector(0,0)�̋���
		inline constexpr float Magnitude() const noexcept;
		//----------------------------------------------------------------------------------
		//[ExactMagnitude]
		//return: ���g��Vector(0,0,0,0)�̋��� (���m�Ȓl�������ׂ�����)
		inline float ExactMagnitude() const noexcept;
		//----------------------------------------------------------------------------------
		//[Normalize]
		//return: ���g�̐��K���x�N�g��
		inline UV Normalized() const noexcept;

		//float*�I�y���[�^�[
		inline operator float*() { return (float*)this->value; }
		//const float*�I�y���[�^�[
		inline operator const float*() const { return (const float*)this->value; }
		//Vector2�I�y���[�^�[
		inline operator Vector2() const { return Vector2(u, v); }
		//=�I�y���[�^�[
		inline Vector2 operator = (const Vector2& vec) { this->u = vec.x; this->v = vec.y; }
		//+�I�y���[�^�[  
		inline UV operator +() const { return UV(+u, +v); }
		//-�I�y���[�^�[  
		inline UV operator -() const { return UV(-u, -v); }
		//==�I�y���[�^�[
		inline bool operator ==(const UV& uv) const
		{
			return (fabsf(u - uv.u) <= MathAF::cEpsilon
				&& fabsf(v - uv.v) <= MathAF::cEpsilon);
		}
		//!=�I�y���[�^�[
		inline bool operator !=(const UV& uv) const
		{
			return (fabsf(u - uv.u) > MathAF::cEpsilon
				&& fabsf(v - uv.v) > MathAF::cEpsilon);
		}

		//���Z�I�y���[�^�[
		inline UV operator +(const UV& uv) const { return UV(u + uv.u, v + uv.v); }
		//���Z�I�y���[�^�[
		inline UV operator -(const UV& uv) const { return UV(u - uv.v, v - uv.v); }
		//��Z�I�y���[�^�[
		inline UV operator *(const UV& uv) const { return UV(u * uv.v, v * uv.v); }
		//��Z�I�y���[�^�[(float)
		inline UV operator *(const float value) const { return UV(u * value, v * value); }
		//���Z�I�y���[�^�[
		inline UV operator /(const UV& uv) const { return UV(u / uv.v, v / uv.v); }
		//���Z�I�y���[�^�[(float)
		inline UV operator /(const float value) const { return UV(u / value, v / value); }

		//+=�I�y���[�^�[
		inline const UV &operator +=(const UV& uv) { this->u += uv.u; this->v += uv.v; return *this; }
		//-=�I�y���[�^�[
		inline const UV &operator -=(const UV& uv) { this->u -= uv.u; this->v -= uv.v; return *this; }
		//*=�I�y���[�^�[
		inline const UV &operator *=(const UV& uv) { this->u *= uv.u; this->v *= uv.v; return *this; }
		//*=�I�y���[�^�[(float)
		inline const UV &operator *=(const float value) { this->u *= value; this->v *= value; return *this; }
		// /=�I�y���[�^�[
		inline const UV &operator /=(const UV& uv) { this->u /= u; this->v /= v; return *this; }
		// /=�I�y���[�^�[(float)
		inline const UV &operator /=(const float value) { this->u /= value; this->v /= value; return *this; }

		//sgstring operator
		inline operator sgstring() const { return ToString(); }
		//----------------------------------------------------------------------------------
		//[ToString]
		//sgstring�`���ŏo�͂���
		//return: sgstring�\������this
		inline sgstring ToString() const { return (L"(" + std::to_wstring(u) + L", " + std::to_wstring(v) + L")"); }
		//----------------------------------------------------------------------------------
		//[ToStdString]
		//std::string�`���ŏo�͂���
		//return: std::string�\������this
		inline std::string ToStdString() const { return ("(" + std::to_string(u) + ", " + std::to_string(v) + ")"); }


		//�v�Z���C�u����


		//----------------------------------------------------------------------------------
		//[Distance]<Calculation Library Function>
		//from����to�̋����𑪂�
		//return: from-to�̋���
		//����1: �Ώۃx�N�g��1
		//����2: �Ώۃx�N�g��2
		inline constexpr float Distance(const UV& from, const UV& to) noexcept
		{
			float f = (from.u - to.u) * (from.u - to.u)
				+ (from.v - to.v) * (from.v - to.v);
			int i = 0x5f3759df - (*reinterpret_cast<int*>(&f) >> 1);
			float bitF = *reinterpret_cast<float*>(&i);

			bitF *= (1.5f - 0.5f * f * bitF * bitF);
			return f * bitF * (1.5f - 0.5f * f * bitF * bitF);
		}
		//----------------------------------------------------------------------------------
		//[ExactDistance]<Calculation Library Function>
		//from����to�̋����𑪂�, Distance��萳�m�Ȓl�����v�Z���ׂ�����
		//return: from-to�̋���
		//����1: �Ώۃx�N�g��1
		//����2: �Ώۃx�N�g��2
		inline float ExactDistance(const UV& from, const UV& to) noexcept
		{
			return std::sqrtf((to.u - from.u) * (to.u - from.u) + (to.v - from.v) * (to.v - from.v));
		}
		//----------------------------------------------------------------------------------
		//[Direction]<Calculation Library Function>
		//from����to�ւ̐��K���ϕ����x�N�g�����擾����
		//return: ���K���ϕ����x�N�g��
		//����1: �Ώۃx�N�g��1
		//����2: �Ώۃx�N�g��2
		inline UV Direction(const UV& from, const UV& to) noexcept
		{
			return std::move((to - from).Normalized());
		}
		//----------------------------------------------------------------------------------
		//[Max]<Calculation Library Function>
		//2��UV�̊e�����̈�ԑ傫�Ȓl���g����UV���쐬
		//return: �ő�UV
		//����1: �Ώ�UV1
		//����2: �Ώ�UV2
		inline static UV Max(const UV& uv1, const UV& uv2)
		{
			UV ret;
			if (uv1.u >= uv2.u) ret.u = uv1.u; else ret.u = uv2.u;
			if (uv1.v >= uv2.v) ret.v = uv1.v; else ret.v = uv2.v;
			return ret;
		}
		//----------------------------------------------------------------------------------
		//[Min]<Calculation Library Function>
		//2��UV�̊e�����̈�ԏ������l���g����UV���쐬
		//return: �ŏ��x�N�g��
		//����1: �Ώ�UV1
		//����2: �Ώ�UV2
		inline static UV Min(const UV& uv1, const UV& uv2)
		{
			UV ret;
			if (uv1.u <= uv2.u) ret.u = uv1.u; else ret.u = uv2.u;
			if (uv1.v <= uv2.v) ret.v = uv1.v; else ret.v = uv2.v;
			return ret;
		}
	};



	//----------------------------------------------------------------------------------
	//[wostream operator]
	//����1: stream
	//����2: this
	inline std::wostream& operator<<(std::wostream& stream, const UV& value)
	{
		const wchar* string = value.ToString();
		stream << string;
		return stream;
	}


	//----------------------------------------------------------------------------------
	//[Absolute]
	//return: ��Βl
	inline UV UV::Absolute() const noexcept
	{
		return UV(u < 0.0f ? -u : u, v < 0.0f ? -v : v);
	}
	//----------------------------------------------------------------------------------
	//[Magnitude]
	//return: ���g��Vector(0,0)�̋���
	inline constexpr float UV::Magnitude() const  noexcept
	{
		float f = u * u + v * v;
		int i = 0x5f3759df - (*reinterpret_cast<int*>(&f) >> 1);
		float bitF = *reinterpret_cast<float*>(&i);

		bitF *= (1.5f - 0.5f * f * bitF * bitF);
		return f * bitF * (1.5f - 0.5f * f * bitF * bitF);
	}
	//----------------------------------------------------------------------------------
	//[ExactMagnitude]
	//return: ���g��Vector(0,0,0,0)�̋��� (���m�Ȓl�������ׂ�����)
	inline float UV::ExactMagnitude() const noexcept
	{
		return std::sqrtf(u * u + v * v);
	}
	//----------------------------------------------------------------------------------
	//[Normalize]
	//return: ���g�̐��K���x�N�g��
	inline UV UV::Normalized() const noexcept
	{
		float divLength = 1.0f / ExactMagnitude();
		return UV(u * divLength, v * divLength);
	}
}
#endif // !SGFRAMEWORK_HEADER_UV_HPP_
