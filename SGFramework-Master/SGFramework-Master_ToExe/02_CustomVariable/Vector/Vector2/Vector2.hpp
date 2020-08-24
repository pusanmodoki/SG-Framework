/*----------------------------------------------------------------------------------
2�̒l��������Vector2 �\����
�ł����p����\���̃V���[�Y��1�Ȃ̂ŕʎ���Vector���֌W�Ȃ����̂͑S��inline����
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_VECTOR_VECTOR2_HPP_
#define SGFRAMEWORK_HEADER_VECTOR_VECTOR2_HPP_
#include <DirectXMath.h>
#include <iostream>
#include <PxPhysicsAPI.h>
#include <assimp/vector2.h>
#include "imgui.h"
#include "../../ConstAndUsing/ConstAndUsing.hpp"
#include "../../Property/Property.hpp"
#include "../../String/String.hpp"

//Framework namespace
namespace SGFramework
{
	//�v���g�^�C�v�錾
	struct Vector3;
	struct Vector4;
	struct Vector2Int;
	struct Vector2Ref;

	//2�̒l��������Vector2 �\����
	struct Vector2 final: public DirectX::XMFLOAT2
	{
	public:
		//----------------------------------------------------------------------------------
		//inlne����

		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�S��0�ɐݒ肳���
		inline constexpr Vector2();
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�������������
		//����1:x
		//����2:y
		inline constexpr Vector2(float x, float y);
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�������������
		//����1: XMFLOAT2
		inline constexpr Vector2(const DirectX::XMFLOAT2& value);
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�������������
		//����1: PxVec2
		inline constexpr Vector2(const physx::PxVec2& vec);
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�������������
		//����1: aiVector2D
		inline constexpr Vector2(const aiVector2D& vec);
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�������������
		//����1: ImVec2
		inline constexpr Vector2(const ImVec2& vec);
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�������������
		//����1: XMVECTOR
		inline Vector2(const XmVector& value);
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�������������
		//����1: float*
		inline Vector2(const float* array);

		//----------------------------------------------------------------------------------
		//[ToXmVector]
		//return: XmVector
		inline XmVector ToXmVector() const noexcept { return DirectX::XMLoadFloat2(this); }
		//----------------------------------------------------------------------------------
		//[ToPhysX]
		//return: physx::PxVec
		inline physx::PxVec2 ToPhysX() const noexcept { return physx::PxVec2(x, y); }
		//----------------------------------------------------------------------------------
		//[ToAssimp]
		//return: aiVector2D
		inline aiVector2D ToAssimp() const noexcept { return aiVector2D(x, y); }
		//----------------------------------------------------------------------------------
		//[ToImGui]
		//return: ImVec2
		inline ImVec2 ToImGui() const noexcept { return ImVec2(x, y); }

		//default
		Vector2(const Vector2&) = default;
		Vector2& operator=(const Vector2&) = default;
		Vector2(Vector2&&) = default;
		Vector2& operator=(Vector2&&) = default;

		//----------------------------------------------------------------------------------
		//[CreateWithInt]
		//int��Vector2���쐬
		//����1:x
		//����2:y
		inline static constexpr Vector2 CreateWithInt(int x, int y);

		//----------------------------------------------------------------------------------
		//[Absolute]
		//return: ��Βl
		inline Vector2 Absolute() const noexcept;
		//----------------------------------------------------------------------------------
		//[Magnitude]
		//return: ���g��Vector(0,0)�̋���
		inline constexpr float Magnitude() const noexcept;
		//----------------------------------------------------------------------------------
		//[ExactMagnitude]
		//return: ���g��Vector(0,0)�̋��� (���m�Ȓl�������ׂ�����)
		inline float ExactMagnitude() const noexcept;
		//----------------------------------------------------------------------------------
		//[ClampMagnitude]
		//Length��maxLength�ɐ�������
		//return: ��������vector
		//����1: ����Length
		inline Vector2 ClampMagnitude(const float maxLength) const noexcept;
		//----------------------------------------------------------------------------------
		//[Normalize]
		//return: ���g�̐��K���x�N�g��
		inline Vector2 Normalized() const noexcept;

		//XMVECTOR�I�y���[�^�[
		inline operator XmVector() const noexcept;
		//float*�I�y���[�^�[
		inline operator float*();
		//const float*�I�y���[�^�[
		inline operator const float*() const;

		//=�I�y���[�^�[
		inline XmVector operator = (const XmVector& vec) noexcept;

		//+�I�y���[�^�[  
		inline Vector2 operator +() const;
		//-�I�y���[�^�[  
		inline Vector2 operator -() const;
		//==�I�y���[�^�[
		inline bool operator ==(const Vector2& vec) const;
		//!=�I�y���[�^�[
		inline bool operator !=(const Vector2& vec) const;

		//���Z�I�y���[�^�[
		inline Vector2 operator +(const Vector2& vec) const;
		//���Z�I�y���[�^�[
		inline Vector2 operator -(const Vector2& vec) const;
		//��Z�I�y���[�^�[
		inline Vector2 operator *(const Vector2& vec) const;
		//��Z�I�y���[�^�[(float)
		inline Vector2 operator *(float value) const;
		//���Z�I�y���[�^�[
		inline Vector2 operator /(const Vector2& vec) const;
		//���Z�I�y���[�^�[(float)
		inline Vector2 operator /(float value) const;

		//+=�I�y���[�^�[
		inline const Vector2 &operator +=(const Vector2& vec);
		//-=�I�y���[�^�[
		inline const Vector2 &operator -=(const Vector2& vec);
		//*=�I�y���[�^�[
		inline const Vector2 &operator *=(const Vector2& vec);
		//*=�I�y���[�^�[(float)
		inline const Vector2 &operator *=(float value);
		// /=�I�y���[�^�[
		inline const Vector2 &operator /=(const Vector2& vec);
		// /=�I�y���[�^�[(float)
		inline const Vector2 &operator /=(float value);

		//sgstring operator
		inline operator sgstring() const { return ToString(); }
		//----------------------------------------------------------------------------------
		//[ToString]
		//sgstring�`���ŏo�͂���
		//return: sgstring�\������this
		inline sgstring ToString() const { return (L"(" + std::to_wstring(x) + L", " + std::to_wstring(y) + L")"); }
		//----------------------------------------------------------------------------------
		//[ToStdString]
		//std::string�`���ŏo�͂���
		//return: std::string�\������this
		inline std::string ToStdString() const { return ("(" + std::to_string(x) + ", " + std::to_string(y) + ")"); }
		
		//----------------------------------------------------------------------------------
		//cpp����

		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�������������
		//����1: Vector2Int
		Vector2(const Vector2Int& value);

		//Vector3�I�y���[�^�[
		operator Vector3() const;
		//Vector4�I�y���[�^�[
		operator Vector4() const;
		//Vector2Int�I�y���[�^�[
		operator Vector2Int() const;

		//=�I�y���[�^�[
		const Vector2& operator = (const Vector3& vec);
		//=�I�y���[�^�[
		const Vector2& operator = (const Vector4& vec);
		//=�I�y���[�^�[
		const Vector2& operator = (const Vector2Int& vec);

		//----------------------------------------------------------------------------------
		//Vector2Ref

		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�������������
		//����1: Vector2Ref
		Vector2(const Vector2Ref& value);

		// =�I�y���[�^�[
		const Vector2 &operator =(const Vector2Ref & vec);

		//==�I�y���[�^�[
		bool operator ==(const Vector2Ref& vec) const;
		//!=�I�y���[�^�[
		bool operator !=(const Vector2Ref& vec) const;

		//���Z�I�y���[�^�[
		Vector2 operator +(const Vector2Ref& vec) const;
		//���Z�I�y���[�^�[
		Vector2 operator -(const Vector2Ref& vec) const;
		//��Z�I�y���[�^�[
		Vector2 operator *(const Vector2Ref& vec) const;
		//���Z�I�y���[�^�[
		Vector2 operator /(const Vector2Ref& vec) const;

		//+=�I�y���[�^�[
		const Vector2& operator +=(const Vector2Ref& vec);
		//-=�I�y���[�^�[
		const Vector2& operator -=(const Vector2Ref& vec);
		//*=�I�y���[�^�[
		const Vector2& operator *=(const Vector2Ref& vec);
		// /=�I�y���[�^�[
		const Vector2& operator /=(const Vector2Ref& vec);


		//�v�Z���C�u����


		//----------------------------------------------------------------------------------
		//[Dot]<Calculation Library Function>
		//���ς��s��
		//return: ���ς̌���
		//����1: �Ώۃx�N�g��1
		//����2: �Ώۃx�N�g��2
		inline static float Dot(const Vector2& vec1, const Vector2& vec2) noexcept;
		//----------------------------------------------------------------------------------
		//[Cross]<Calculation Library Function>
		//�O�ς��s��
		//return: �O�ς̌���
		//����1: �Ώۃx�N�g��1
		//����2: �Ώۃx�N�g��2
		inline static float Cross(const Vector2& vec1, const Vector2& vec2) noexcept;
		//----------------------------------------------------------------------------------
		//[Angle]<Calculation Library Function>
		//2�_�Ԃ̊p�x�𑪂�
		//return: 2�_�Ԃ̊p�x
		//����1: �Ώۃx�N�g��1
		//����2: �Ώۃx�N�g��2
		inline static float Angle(const Vector2& from, const Vector2& to) noexcept;
		//----------------------------------------------------------------------------------
		//[Distance]<Calculation Library Function>
		//from����to�̋����𑪂�
		//return: from-to�̋���
		//����1: �Ώۃx�N�g��1
		//����2: �Ώۃx�N�g��2
		inline static constexpr float Distance(const Vector2& from, const Vector2& to) noexcept;
		//----------------------------------------------------------------------------------
		//[ExactDistance]<Calculation Library Function>
		//from����to�̋����𑪂�, Distance��萳�m�Ȓl�����v�Z���ׂ�����
		//return: from-to�̋���
		//����1: �Ώۃx�N�g��1
		//����2: �Ώۃx�N�g��2
		inline static float ExactDistance(const Vector2& from, const Vector2& to) noexcept;
		//----------------------------------------------------------------------------------
		//[Direction]<Calculation Library Function>
		//from����to�ւ̐��K���ϕ����x�N�g�����擾����
		//return: ���K���ϕ����x�N�g��
		//����1: �Ώۃx�N�g��1
		//����2: �Ώۃx�N�g��2
		inline static Vector2 Direction(const Vector2& from, const Vector2& to) noexcept;
		//----------------------------------------------------------------------------------
		//[TransformCoord]<Calculation Library Function>
		//Matrix��t�^�����l�ɕϊ�����
		//return: Matrix��t�^�����x�N�g��
		//����1: �Ώۃx�N�g��
		//����2: �Ώ�Matrix
		inline static Vector2 TransformCoord(const Vector2& vec, const DirectX::XMMATRIX& matrix) noexcept;
		//----------------------------------------------------------------------------------
		//[Max]<Calculation Library Function>
		//2�̃x�N�g���̊e�����̈�ԑ傫�Ȓl���g����Vector���쐬
		//return: �ő�x�N�g��
		//����1: �Ώۃx�N�g��1
		//����2: �Ώۃx�N�g��2
		inline static constexpr Vector2 Max(const Vector2& vec1, const Vector2& vec2) noexcept;
		//----------------------------------------------------------------------------------
		//[Min]<Calculation Library Function>
		//2�̃x�N�g���̊e�����̈�ԏ������l���g����Vector���쐬
		//return: �ŏ��x�N�g��
		//����1: �Ώۃx�N�g��1
		//����2: �Ώۃx�N�g��2
		inline static constexpr Vector2 Min(const Vector2& vec1, const Vector2& vec2) noexcept;
	};


	//----------------------------------------------------------------------------------
	//[wostream operator]
	//����1: stream
	//����2: this
	inline std::wostream& operator<<(std::wostream& stream, const Vector2& value)
	{
		const wchar* string = value.ToString();
		stream << string;
		return stream;
	}

	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//�S��0�ɐݒ肳���
	inline constexpr Vector2::Vector2() : XMFLOAT2(0.0f, 0.0f) {}
	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//�������������
	//����1: x
	//����2: y
	inline constexpr Vector2::Vector2(float x, float y) : XMFLOAT2(x, y) {}
	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//�������������
	//����1: XMFLOAT2
	inline constexpr Vector2::Vector2(const DirectX::XMFLOAT2 & vec) : XMFLOAT2(vec) { }
	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//�������������
	//����1: PxVec2
	inline constexpr Vector2::Vector2(const physx::PxVec2 & vec) : XMFLOAT2(vec.x, vec.y) {}
	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//�������������
	//����1: aiVector2D
	inline constexpr Vector2::Vector2(const aiVector2D & vec) : XMFLOAT2(vec.x, vec.y) {}
	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//�������������
	//����1: ImVec2
	inline constexpr Vector2::Vector2(const ImVec2 & vec) : XMFLOAT2(vec.x, vec.y) {}
	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//�������������
	//����1: XMVECTOR
	inline Vector2::Vector2(const XmVector & vec)
	{
		DirectX::XMStoreFloat2(this, vec);
	}
	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//�������������
	//����1: float*
	inline Vector2::Vector2(const float * array) : DirectX::XMFLOAT2(array) {}

	//----------------------------------------------------------------------------------
	//[CreateWithInt]
	//int��Vector2���쐬
	//����1:x
	//����2:y
	inline constexpr Vector2 Vector2::CreateWithInt(int x, int y)
	{
		return Vector2(static_cast<float>(x), static_cast<float>(y));
	}
	//----------------------------------------------------------------------------------
	//[Absolute]
	//return: ��Βl
	inline Vector2 Vector2::Absolute() const noexcept
	{
		return Vector2(x < 0.0f ? -x : x, y < 0.0f ? -y : y);
	}
	//----------------------------------------------------------------------------------
	//[Magnitude]
	//return: ���g��Vector(0,0)�̋���
	inline constexpr float Vector2::Magnitude() const noexcept
	{
		float f = x * x + y * y;
		int i = 0x5f3759df - (*reinterpret_cast<int*>(&f) >> 1);
		float bitF = *reinterpret_cast<float*>(&i);

		bitF *= (1.5f - 0.5f * f * bitF * bitF);
		return f * bitF * (1.5f - 0.5f * f * bitF * bitF);
	}
	//----------------------------------------------------------------------------------
	//[ExactMagnitude]
	//return: ���g��Vector(0,0)�̋��� (���m�Ȓl�������ׂ�����)
	inline float Vector2::ExactMagnitude() const noexcept
	{		
		return std::sqrtf(x * x + y * y);
	}
	//----------------------------------------------------------------------------------
	//[ClampMagnitude]
	//vec��Length��maxLength�ɐ�������
	//return: ��������vector
	//����2: ����Length
	inline Vector2 Vector2::ClampMagnitude(const float maxLength) const noexcept
	{
		return std::move(DirectX::XMVector2ClampLength(*this, 0.0f, maxLength));
	}
	//----------------------------------------------------------------------------------
	//[Normalize]
	//return: ���g�̐��K���x�N�g��
	inline Vector2 Vector2::Normalized() const noexcept
	{
		float divLength = 1.0f / ExactMagnitude();
		return std::move(Vector2(x * divLength, y * divLength));
	}

	//XMVECTOR�I�y���[�^�[
	inline Vector2::operator XmVector() const noexcept { return DirectX::XMLoadFloat2(this); }
	//float*�I�y���[�^�[
	inline Vector2::operator float*() { return (float*)this; }
	//const float*�I�y���[�^�[
	inline Vector2::operator const float*() const { return (float*)this; }

	//=�I�y���[�^�[
	inline XmVector Vector2::operator=(const XmVector& vec) noexcept
	{
		DirectX::XMStoreFloat2(this, vec);
		return vec;
	}

	//+�I�y���[�^�[  
	inline Vector2 Vector2::operator+() const { return Vector2(+this->x, +this->y); }
	//-�I�y���[�^�[  
	inline Vector2 Vector2::operator-() const { return Vector2(-this->x, -this->y); }
	//==�I�y���[�^�[
	inline bool Vector2::operator==(const Vector2& vec) const
	{
		return (fabsf(x - vec.x) <= MathAF::cEpsilon
			&& fabsf(y - vec.y) <= MathAF::cEpsilon);
	}
	//!=�I�y���[�^�[
	inline bool Vector2::operator!=(const Vector2& vec) const
	{
		return (fabsf(x - vec.x) > MathAF::cEpsilon
			&& fabsf(y - vec.y) > MathAF::cEpsilon);
	}

	//���Z�I�y���[�^�[
	inline Vector2 Vector2::operator+(const Vector2& vec) const { return Vector2(x + vec.x, y + vec.y); }
	//���Z�I�y���[�^�[
	inline Vector2 Vector2::operator-(const Vector2& vec) const { return Vector2(x - vec.x, y - vec.y); }
	//��Z�I�y���[�^�[
	inline Vector2 Vector2::operator*(const Vector2& vec) const { return Vector2(x * vec.x, y * vec.y); }
	//��Z�I�y���[�^�[(float)
	inline Vector2 Vector2::operator*(const float value) const { return Vector2(x * value, y * value); }
	//���Z�I�y���[�^�[
	inline Vector2 Vector2::operator/(const Vector2& vec) const { return Vector2(x / vec.x, y / vec.y); }
	//���Z�I�y���[�^�[(float)
	inline Vector2 Vector2::operator/(const float value) const { return Vector2(x / value, y / value); }

	//+=�I�y���[�^�[
	inline const Vector2 & Vector2::operator+=(const Vector2& vec)
	{
		this->x += vec.x;		this->y += vec.y;
		return *this;
	}
	//-=�I�y���[�^�[
	inline const Vector2 & Vector2::operator-=(const Vector2& vec)
	{
		this->x -= vec.x;		this->y -= vec.y;
		return *this;
	}
	//*=�I�y���[�^�[
	inline const Vector2 & Vector2::operator*=(const Vector2& vec)
	{
		this->x *= vec.x;		this->y *= vec.y;
		return *this;
	}
	//*=�I�y���[�^�[(float)
	inline const Vector2 & Vector2::operator*=(const float value)
	{
		this->x *= value;		this->y *= value;
		return *this;
	}
	// /=�I�y���[�^�[
	inline const Vector2 & Vector2::operator/=(const Vector2& vec)
	{
		this->x /= vec.x;		this->y /= vec.y;
		return *this;
	}
	// /=�I�y���[�^�[(float)
	inline const Vector2 & Vector2::operator/=(const float value)
	{
		this->x /= value;		this->y /= value;
		return *this;
	}




	//----------------------------------------------------------------------------------
	//[Dot]<Calculation Library Function>
	//���ς��s��
	//return: ���ς̌���
	//����1: �Ώۃx�N�g��1
	//����2: �Ώۃx�N�g��2
	inline float Vector2::Dot(const Vector2& vec1, const Vector2& vec2) noexcept
	{
		return vec1.x * vec2.x + vec1.y * vec2.y;
	}
	//----------------------------------------------------------------------------------
	//[Cross]<Calculation Library Function>
	//�O�ς��s��
	//return: �O�ς̌���
	//����1: �Ώۃx�N�g��1
	//����2: �Ώۃx�N�g��2
	inline float Vector2::Cross(const Vector2& vec1, const Vector2& vec2) noexcept
	{
		//x1 * y2 - x2 * y1 = |v1||v2|sin(��)
		return vec1.x * vec2.y + vec2.x * vec1.y;
	}
	//----------------------------------------------------------------------------------
	//[Angle]<Calculation Library Function>
	//2�_�Ԃ̊p�x�𑪂�
	//return: 2�_�Ԃ̊p�x
	//����1: �Ώۃx�N�g��1
	//����2: �Ώۃx�N�g��2
	inline float Vector2::Angle(const Vector2& from, const Vector2& to) noexcept
	{
		float cosSita = Dot(from, to) / (from.Magnitude() * to.Magnitude());
		float ret = std::acosf(cosSita) * MathAF::cRadToDeg;
		ret += 90.0f;
		while (ret > 180.0f) ret -= 360.0f;
		return std::move(ret);
	}
	//----------------------------------------------------------------------------------
	//[Distance]<Calculation Library Function>
	//from����to�̋����𑪂�
	//return: from-to�̋���
	//����1: �Ώۃx�N�g��1
	//����2: �Ώۃx�N�g��2
	inline constexpr float Vector2::Distance(const Vector2& from, const Vector2& to) noexcept
	{
		float f = (from.x - to.x) * (from.x - to.x)
			+ (from.y - to.y) * (from.y - to.y);
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
	inline float Vector2::ExactDistance(const Vector2 & from, const Vector2 & to) noexcept
	{
		return std::sqrtf((to.x - from.x) * (to.x - from.x) + (to.y - from.y) * (to.y - from.y));
	}
	//----------------------------------------------------------------------------------
	//[Direction]<Calculation Library Function>
	//from����to�ւ̐��K���ϕ����x�N�g�����擾����
	//return: ���K���ϕ����x�N�g��
	//����1: �Ώۃx�N�g��1
	//����2: �Ώۃx�N�g��2
	inline Vector2 Vector2::Direction(const Vector2& from, const Vector2& to) noexcept
	{
		return std::move((to - from).Normalized());
	}
	//----------------------------------------------------------------------------------
	//[TransformCoord]<Calculation Library Function>
	//Matrix��t�^�����l�ɕϊ�����
	//return: Matrix��t�^�����x�N�g��
	//����1: �Ώۃx�N�g��
	//����2: �Ώ�Matrix
	inline Vector2 Vector2::TransformCoord(const Vector2& vec, const DirectX::XMMATRIX& matrix) noexcept
	{
		return std::move(DirectX::XMVector2TransformCoord(vec, matrix));
	}
	//----------------------------------------------------------------------------------
	//[Max]<Calculation Library Function>
	//2�̃x�N�g���̊e�����̈�ԑ傫�Ȓl���g����Vector���쐬
	//return: �ő�x�N�g��
	//����1: �Ώۃx�N�g��1
	//����2: �Ώۃx�N�g��2
	inline constexpr Vector2 Vector2::Max(const Vector2& vec1, const Vector2& vec2) noexcept
	{
		Vector2 ret;
		if (vec1.x >= vec2.x) ret.x = vec1.x; else ret.x = vec2.x;
		if (vec1.y >= vec2.y) ret.y = vec1.y; else ret.y = vec2.y;
		return std::move(ret);
	}
	//----------------------------------------------------------------------------------
	//[Min]<Calculation Library Function>
	//2�̃x�N�g���̊e�����̈�ԏ������l���g����Vector���쐬
	//return: �ŏ��x�N�g��
	//����1: �Ώۃx�N�g��1
	//����2: �Ώۃx�N�g��2
	inline constexpr Vector2 Vector2::Min(const Vector2& vec1, const Vector2& vec2) noexcept
	{
		Vector2 ret;
		if (vec1.x <= vec2.x) ret.x = vec1.x; else ret.x = vec2.x;
		if (vec1.y <= vec2.y) ret.y = vec1.y; else ret.y = vec2.y;
		return std::move(ret);
	}
}
#endif // !SGFRAMEWORK_HEADER_VECTOR_VECTOR2_HPP_