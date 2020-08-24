/*----------------------------------------------------------------------------------
2�̒l��������Vector2Ref (�Q��) �\����
�ł����p����\���̃V���[�Y��1�Ȃ̂ŕʎ���Vector���֌W�Ȃ����̂͑S��inline����
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_VECTOR_VECTOR2_REF_HPP_
#define SGFRAMEWORK_HEADER_VECTOR_VECTOR2_REF_HPP_
#include <DirectXMath.h>
#include <PxPhysicsAPI.h>
#include <assimp/vector2.h>
#include "imgui.h"
#include <iostream>
#include "../../../ConstAndUsing/ConstAndUsing.hpp"
#include "../../../Property/Property.hpp"
#include "../../../String/String.hpp"
#include "../Vector2.hpp"

//Framework namespace
namespace SGFramework
{
	//�v���g�^�C�v�錾
	struct Vector3;
	struct Vector4;
	struct Vector2Int;

	//2�̒l��������Vector2RefRef �\����
	struct Vector2Ref final
	{
	public:
		//----------------------------------------------------------------------------------
		//inlne����

		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�������������
		//����1:x
		//����2:y
		inline Vector2Ref(float& x, float& y);
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�������������
		//����1: Vector2
		inline Vector2Ref(DirectX::XMFLOAT2& value);

		//----------------------------------------------------------------------------------
		//[ToXmVector]
		//return: XmVector
		inline XmVector ToXmVector() const noexcept { return DirectX::XMLoadFloat2(&DirectX::XMFLOAT2(x, y)); }
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
		Vector2Ref(const Vector2Ref&) = default;
		Vector2Ref& operator=(const Vector2Ref&) = default;
		Vector2Ref(Vector2Ref&&) = default;
		Vector2Ref& operator=(Vector2Ref&&) = default;
	
		//----------------------------------------------------------------------------------
		//�ϐ�
		float& x;		//X
		float& y;		//Y

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
		
		//+�I�y���[�^�[  
		inline Vector2 operator +() const;
		//-�I�y���[�^�[  
		inline Vector2 operator -() const;
		//==�I�y���[�^�[
		inline bool operator ==(const Vector2Ref& vec) const;
		//!=�I�y���[�^�[
		inline bool operator !=(const Vector2Ref& vec) const;

		//���Z�I�y���[�^�[
		inline Vector2 operator +(const Vector2Ref& vec) const;
		//���Z�I�y���[�^�[
		inline Vector2 operator -(const Vector2Ref& vec) const;
		//��Z�I�y���[�^�[
		inline Vector2 operator *(const Vector2Ref& vec) const;
		//��Z�I�y���[�^�[(float)
		inline Vector2 operator *(float value) const;
		//���Z�I�y���[�^�[
		inline Vector2 operator /(const Vector2Ref& vec) const;
		//���Z�I�y���[�^�[(float)
		inline Vector2 operator /(float value) const;

		//+=�I�y���[�^�[
		inline const Vector2Ref &operator +=(const Vector2Ref& vec);
		//-=�I�y���[�^�[
		inline const Vector2Ref &operator -=(const Vector2Ref& vec);
		//*=�I�y���[�^�[
		inline const Vector2Ref &operator *=(const Vector2Ref& vec);
		//*=�I�y���[�^�[(float)
		inline const Vector2Ref &operator *=(float value);
		// /=�I�y���[�^�[
		inline const Vector2Ref &operator /=(const Vector2Ref& vec);
		// /=�I�y���[�^�[(float)
		inline const Vector2Ref &operator /=(float value);

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
		//Vector2

		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�������������
		//����1: Vector2
		inline Vector2Ref(Vector2& value);
		//----------------------------------------------------------------------------------
		//[ToVector2]
		//return: Vector2
		inline Vector2 ToVector2() const { return Vector2(*this); }

		//= �I�y���[�^�[
		inline const Vector2Ref& operator = (const Vector2& vec);

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
		//���Z�I�y���[�^�[
		inline Vector2 operator /(const Vector2& vec) const;
		
		//+=�I�y���[�^�[
		inline const Vector2Ref &operator +=(const Vector2& vec);
		//-=�I�y���[�^�[
		inline const Vector2Ref &operator -=(const Vector2& vec);
		//*=�I�y���[�^�[
		inline const Vector2Ref &operator *=(const Vector2& vec);
		// /=�I�y���[�^�[
		inline const Vector2Ref &operator /=(const Vector2& vec);

		//= �I�y���[�^�[
		inline const Vector2Ref& operator = (const DirectX::XMFLOAT2& vec);

		//==�I�y���[�^�[
		inline bool operator ==(const DirectX::XMFLOAT2& vec) const;
		//!=�I�y���[�^�[
		inline bool operator !=(const DirectX::XMFLOAT2& vec) const;

		//���Z�I�y���[�^�[
		inline Vector2 operator +(const DirectX::XMFLOAT2& vec) const;
		//���Z�I�y���[�^�[
		inline Vector2 operator -(const DirectX::XMFLOAT2& vec) const;
		//��Z�I�y���[�^�[
		inline Vector2 operator *(const DirectX::XMFLOAT2& vec) const;
		//���Z�I�y���[�^�[
		inline Vector2 operator /(const DirectX::XMFLOAT2& vec) const;

		//+=�I�y���[�^�[
		inline const Vector2Ref &operator +=(const DirectX::XMFLOAT2& vec);
		//-=�I�y���[�^�[
		inline const Vector2Ref &operator -=(const DirectX::XMFLOAT2& vec);
		//*=�I�y���[�^�[
		inline const Vector2Ref &operator *=(const DirectX::XMFLOAT2& vec);
		// /=�I�y���[�^�[
		inline const Vector2Ref &operator /=(const DirectX::XMFLOAT2& vec);


		//----------------------------------------------------------------------------------
		//cpp����

		//Vector3�I�y���[�^�[
		operator Vector3() const;
		//Vector4�I�y���[�^�[
		operator Vector4() const;
		//Vector2Int�I�y���[�^�[
		operator Vector2Int() const;

		//=�I�y���[�^�[
		const Vector2Ref& operator = (const Vector3& vec);
		//=�I�y���[�^�[
		const Vector2Ref& operator = (const Vector4& vec);
		//=�I�y���[�^�[
		const Vector2Ref& operator = (const Vector2Int& vec);
	};


	//----------------------------------------------------------------------------------
	//[wostream operator]
	//����1: stream
	//����2: this
	inline std::wostream& operator<<(std::wostream& stream, const Vector2Ref& value)
	{
		const wchar* string = value.ToString();
		stream << string;
		return stream;
	}

	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//�������������
	//����1:x
	//����2:y
	inline Vector2Ref::Vector2Ref(float& x, float& y) : x(x), y(y) {};
	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//�������������
	//����1: XMFLOAT2
	inline Vector2Ref::Vector2Ref(DirectX::XMFLOAT2 & vec) : x(vec.x), y(vec.y) { }

	//----------------------------------------------------------------------------------
	//[Absolute]
	//return: ��Βl
	inline Vector2 Vector2Ref::Absolute() const noexcept
	{
		return Vector2(x < 0.0f ? -x : x, y < 0.0f ? -y : y);
	}
	//----------------------------------------------------------------------------------
	//[Magnitude]
	//return: ���g��Vector(0,0)�̋���
	inline constexpr float Vector2Ref::Magnitude() const noexcept
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
	inline float Vector2Ref::ExactMagnitude() const noexcept
	{
		return std::sqrtf(x * x + y * y);
	}
	//----------------------------------------------------------------------------------
	//[ClampMagnitude]
	//vec��Length��maxLength�ɐ�������
	//return: ��������vector
	//����2: ����Length
	inline Vector2 Vector2Ref::ClampMagnitude(const float maxLength) const noexcept
	{
		return std::move(Vector2(DirectX::XMVector2ClampLength(this->ToXmVector(), 0.0f, maxLength)));
	}
	//----------------------------------------------------------------------------------
	//[Normalize]
	//return: ���g�̐��K���x�N�g��
	inline Vector2 Vector2Ref::Normalized() const noexcept
	{
		float divLength = 1.0f / ExactMagnitude();
		return std::move(Vector2(x * divLength, y * divLength));
	}

	//XMVECTOR�I�y���[�^�[
	inline Vector2Ref::operator XmVector() const noexcept { return DirectX::XMLoadFloat2(&DirectX::XMFLOAT2(x, y)); }

	//+�I�y���[�^�[  
	inline Vector2 Vector2Ref::operator+() const { return Vector2(+this->x, +this->y); }
	//-�I�y���[�^�[  
	inline Vector2 Vector2Ref::operator-() const { return Vector2(-this->x, -this->y); }
	//==�I�y���[�^�[
	inline bool Vector2Ref::operator==(const Vector2Ref& vec) const
	{
		return (fabsf(x - vec.x) <= MathAF::cEpsilon
			&& fabsf(y - vec.y) <= MathAF::cEpsilon);
	}
	//!=�I�y���[�^�[
	inline bool Vector2Ref::operator!=(const Vector2Ref& vec) const
	{
		return (fabsf(x - vec.x) > MathAF::cEpsilon
			&& fabsf(y - vec.y) > MathAF::cEpsilon);
	}

	//���Z�I�y���[�^�[
	inline Vector2 Vector2Ref::operator+(const Vector2Ref& vec) const { return Vector2(x + vec.x, y + vec.y); }
	//���Z�I�y���[�^�[
	inline Vector2 Vector2Ref::operator-(const Vector2Ref& vec) const { return Vector2(x - vec.x, y - vec.y); }
	//��Z�I�y���[�^�[
	inline Vector2 Vector2Ref::operator*(const Vector2Ref& vec) const { return Vector2(x * vec.x, y * vec.y); }
	//��Z�I�y���[�^�[(float)
	inline Vector2 Vector2Ref::operator*(const float value) const { return Vector2(x * value, y * value); }
	//���Z�I�y���[�^�[
	inline Vector2 Vector2Ref::operator/(const Vector2Ref& vec) const { return Vector2(x / vec.x, y / vec.y); }
	//���Z�I�y���[�^�[(float)
	inline Vector2 Vector2Ref::operator/(const float value) const { return Vector2(x / value, y / value); }

	//+=�I�y���[�^�[
	inline const Vector2Ref & Vector2Ref::operator+=(const Vector2Ref& vec)
	{
		this->x += vec.x;		this->y += vec.y;
		return *this;
	}
	//-=�I�y���[�^�[
	inline const Vector2Ref & Vector2Ref::operator-=(const Vector2Ref& vec)
	{
		this->x -= vec.x;		this->y -= vec.y;
		return *this;
	}
	//*=�I�y���[�^�[
	inline const Vector2Ref & Vector2Ref::operator*=(const Vector2Ref& vec)
	{
		this->x *= vec.x;		this->y *= vec.y;
		return *this;
	}
	//*=�I�y���[�^�[(float)
	inline const Vector2Ref & Vector2Ref::operator*=(const float value)
	{
		this->x *= value;		this->y *= value;
		return *this;
	}
	// /=�I�y���[�^�[
	inline const Vector2Ref & Vector2Ref::operator/=(const Vector2Ref& vec)
	{
		this->x /= vec.x;		this->y /= vec.y;
		return *this;
	}
	// /=�I�y���[�^�[(float)
	inline const Vector2Ref & Vector2Ref::operator/=(const float value)
	{
		this->x /= value;		this->y /= value;
		return *this;
	}

	//----------------------------------------------------------------------------------
	//Vector2

	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//�������������
	//����1: Vector2
	Vector2Ref::Vector2Ref(Vector2 & value) : x(value.x), y(value.y) {}

	//= �I�y���[�^�[
	inline const Vector2Ref & Vector2Ref::operator=(const Vector2 & vec)
	{
		x = vec.x; y = vec.y;
		return *this;
	}
	
	//==�I�y���[�^�[
	inline bool Vector2Ref::operator==(const Vector2 & vec) const
	{
		return (fabsf(x - vec.x) <= MathAF::cEpsilon
			&& fabsf(y - vec.y) <= MathAF::cEpsilon);
	}
	//!=�I�y���[�^�[
	inline bool Vector2Ref::operator!=(const Vector2 & vec) const
	{
		return (fabsf(x - vec.x) > MathAF::cEpsilon
			&& fabsf(y - vec.y) > MathAF::cEpsilon);
	}

	//���Z�I�y���[�^�[
	inline Vector2 Vector2Ref::operator+(const Vector2 & vec) const { return Vector2(x + vec.x, y + vec.y);}
	//���Z�I�y���[�^�[
	inline Vector2 Vector2Ref::operator-(const Vector2 & vec) const { return Vector2(x - vec.x, y - vec.y); }
	//��Z�I�y���[�^�[
	inline Vector2 Vector2Ref::operator*(const Vector2 & vec) const { return Vector2(x * vec.x, y * vec.y); }
	//���Z�I�y���[�^�[
	inline Vector2 Vector2Ref::operator/(const Vector2 & vec) const { return Vector2(x  / vec.x, y / vec.y); }

	inline const Vector2Ref & Vector2Ref::operator+=(const Vector2 & vec)
	{
		this->x += vec.x;		this->y += vec.y;
		return *this;
	}

	inline const Vector2Ref & Vector2Ref::operator-=(const Vector2 & vec)
	{
		this->x -= vec.x;		this->y -= vec.y;
		return *this;
	}

	inline const Vector2Ref & Vector2Ref::operator*=(const Vector2 & vec)
	{
		this->x *= vec.x;		this->y *= vec.y;
		return *this;
	}

	inline const Vector2Ref & Vector2Ref::operator/=(const Vector2 & vec)
	{
		this->x /= vec.x;		this->y /= vec.y;
		return *this;
	}

	//= �I�y���[�^�[
	inline const Vector2Ref & Vector2Ref::operator=(const DirectX::XMFLOAT2 & vec)
	{
		x = vec.x; y = vec.y;
		return *this;
	}

	//==�I�y���[�^�[
	inline bool Vector2Ref::operator==(const DirectX::XMFLOAT2& vec) const
	{
		return (fabsf(x - vec.x) <= MathAF::cEpsilon
			&& fabsf(y - vec.y) <= MathAF::cEpsilon);
	}
	//!=�I�y���[�^�[
	inline bool Vector2Ref::operator!=(const DirectX::XMFLOAT2& vec) const
	{
		return (fabsf(x - vec.x) > MathAF::cEpsilon
			&& fabsf(y - vec.y) > MathAF::cEpsilon);
	}

	//���Z�I�y���[�^�[
	inline Vector2 Vector2Ref::operator+(const DirectX::XMFLOAT2& vec) const { return Vector2(x + vec.x, y + vec.y); }
	//���Z�I�y���[�^�[
	inline Vector2 Vector2Ref::operator-(const DirectX::XMFLOAT2& vec) const { return Vector2(x - vec.x, y - vec.y); }
	//��Z�I�y���[�^�[
	inline Vector2 Vector2Ref::operator*(const DirectX::XMFLOAT2& vec) const { return Vector2(x * vec.x, y * vec.y); }
	//���Z�I�y���[�^�[
	inline Vector2 Vector2Ref::operator/(const DirectX::XMFLOAT2& vec) const { return Vector2(x / vec.x, y / vec.y); }

	inline const Vector2Ref & Vector2Ref::operator+=(const DirectX::XMFLOAT2& vec)
	{
		this->x += vec.x;		this->y += vec.y;
		return *this;
	}

	inline const Vector2Ref & Vector2Ref::operator-=(const DirectX::XMFLOAT2& vec)
	{
		this->x -= vec.x;		this->y -= vec.y;
		return *this;
	}

	inline const Vector2Ref & Vector2Ref::operator*=(const DirectX::XMFLOAT2& vec)
	{
		this->x *= vec.x;		this->y *= vec.y;
		return *this;
	}

	inline const Vector2Ref & Vector2Ref::operator/=(const DirectX::XMFLOAT2& vec)
	{
		this->x /= vec.x;		this->y /= vec.y;
		return *this;
	}
}
#endif // !SGFRAMEWORK_HEADER_VECTOR_VECTOR2_REF_HPP_
