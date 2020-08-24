/*----------------------------------------------------------------------------------
4�̒l��������Vector4Ref (�Q��)�\����
�ł����p����\���̃V���[�Y��1�Ȃ̂ŕʎ���Vector���֌W�Ȃ����̂͑S��inline����
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_VECTOR_VECTOR4_REF_HPP_
#define SGFRAMEWORK_HEADER_VECTOR_VECTOR4_REF_HPP_
#include <DirectXMath.h>
#include <iostream>
#include "../../../ConstAndUsing/ConstAndUsing.hpp"
#include "../../../Property/Property.hpp"
#include "../../../String/String.hpp"
#include "../Vector4.hpp"

//Framework namespace
namespace SGFramework
{
	//�v���g�^�C�v�錾
	struct Vector3;
	struct Vector4;
	struct Vector2;
	struct Vector4Int;

	//4�̒l��������Vector4Ref (�Q��)�\����
	struct Vector4Ref final
	{
	public:
		//----------------------------------------------------------------------------------
		//inlne����

		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�������������
		//����1:x
		//����2:y
		inline Vector4Ref(float& x, float& y, float& z, float& w);
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�������������
		//����1: XMFLOAT2
		inline Vector4Ref(DirectX::XMFLOAT4& value);

		//----------------------------------------------------------------------------------
		//[ToXmVector]
		//return: XmVector
		inline XmVector ToXmVector() const noexcept { return DirectX::XMLoadFloat4(&DirectX::XMFLOAT4(x, y, z, w)); }
		//----------------------------------------------------------------------------------
		//[ToRotationMatrix]
		//return: this��Quaternion�Ƃ�����]�s��
		inline XmMatrix ToRotationMatrix() const noexcept { return DirectX::XMMatrixRotationQuaternion(this->ToXmVector()); }
		//----------------------------------------------------------------------------------
		//[ToRotationMatrix]
		//����1: Apply
		inline XmMatrix ToRotationMatrix(XmMatrix& apply) const noexcept { apply = DirectX::XMMatrixRotationQuaternion(this->ToXmVector()); }
		//----------------------------------------------------------------------------------
		//[ToPhysX]
		//return: physx::PxVec4
		inline physx::PxVec4 ToPhysX() const noexcept { return physx::PxVec4(x, y, z, w); }

		//default
		Vector4Ref(const Vector4Ref&) = default;
		Vector4Ref& operator=(const Vector4Ref&) = default;
		Vector4Ref(Vector4Ref&&) = default;
		Vector4Ref& operator=(Vector4Ref&&) = default;

		//----------------------------------------------------------------------------------
		//�ϐ�
		float& x;		//X
		float& y;		//Y
		float& z;		//Z
		float& w;		//W

		//----------------------------------------------------------------------------------
		//[Absolute]
		//return: ��Βl
		inline constexpr Vector4 Absolute() const noexcept;
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
		inline constexpr Vector4 Normalized() const noexcept;

		//XMVECTOR�I�y���[�^�[
		inline operator XmVector() const noexcept;

		//+�I�y���[�^�[  
		inline Vector4 operator +() const;
		//-�I�y���[�^�[  
		inline Vector4 operator -() const;
		//==�I�y���[�^�[
		inline bool operator ==(const Vector4Ref& vec) const;
		//!=�I�y���[�^�[
		inline bool operator !=(const Vector4Ref& vec) const;

		//���Z�I�y���[�^�[
		inline Vector4 operator +(const Vector4Ref& vec) const;
		//���Z�I�y���[�^�[
		inline Vector4 operator -(const Vector4Ref& vec) const;
		//��Z�I�y���[�^�[
		inline Vector4 operator *(const Vector4Ref& vec) const;
		//��Z�I�y���[�^�[(float)
		inline Vector4 operator *(float value) const;
		//���Z�I�y���[�^�[
		inline Vector4 operator /(const Vector4Ref& vec) const;
		//���Z�I�y���[�^�[(float)
		inline Vector4 operator /(float value) const;

		//+=�I�y���[�^�[
		inline const Vector4Ref &operator +=(const Vector4Ref& vec);
		//-=�I�y���[�^�[
		inline const Vector4Ref &operator -=(const Vector4Ref& vec);
		//*=�I�y���[�^�[
		inline const Vector4Ref &operator *=(const Vector4Ref& vec);
		//*=�I�y���[�^�[(float)
		inline const Vector4Ref &operator *=(float value);
		// /=�I�y���[�^�[
		inline const Vector4Ref &operator /=(const Vector4Ref& vec);
		// /=�I�y���[�^�[(float)
		inline const Vector4Ref &operator /=(float value);

		//sgstring operator
		inline operator sgstring() const { return ToString(); }
		//----------------------------------------------------------------------------------
		//[ToString]
		//sgstring�`���ŏo�͂���
		//return: sgstring�\������this
		inline sgstring ToString() const { return (L"(" + std::to_wstring(x) + L", " + std::to_wstring(y) + L", " + std::to_wstring(z) + L")"); }
		//----------------------------------------------------------------------------------
		//[ToStdString]
		//std::string�`���ŏo�͂���
		//return: std::string�\������this
		inline std::string ToStdString() const { return ("(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ", " + std::to_string(w) + ")"); }

		//----------------------------------------------------------------------------------
		//Vector4

		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�������������
		//����1: Vector4
		inline Vector4Ref(Vector4& value);
		//----------------------------------------------------------------------------------
		//[ToVector4]
		//return: Vector4
		inline Vector4 ToVector4() const { return Vector4(*this); }

		//=�I�y���[�^�[
		inline const Vector4Ref &operator =(const Vector4& vec);
		//==�I�y���[�^�[
		inline bool operator ==(const Vector4& vec) const;
		//!=�I�y���[�^�[
		inline bool operator !=(const Vector4& vec) const;

		//���Z�I�y���[�^�[
		inline Vector4 operator +(const Vector4& vec) const;
		//���Z�I�y���[�^�[
		inline Vector4 operator -(const Vector4& vec) const;
		//��Z�I�y���[�^�[
		inline Vector4 operator *(const Vector4& vec) const;
		//���Z�I�y���[�^�[
		inline Vector4 operator /(const Vector4& vec) const;

		//+=�I�y���[�^�[
		inline const Vector4Ref &operator +=(const Vector4& vec);
		//-=�I�y���[�^�[
		inline const Vector4Ref &operator -=(const Vector4& vec);
		//*=�I�y���[�^�[
		inline const Vector4Ref &operator *=(const Vector4& vec);
		// /=�I�y���[�^�[
		inline const Vector4Ref &operator /=(const Vector4& vec);


		//=�I�y���[�^�[
		inline const Vector4Ref &operator =(const DirectX::XMFLOAT4& vec);
		//==�I�y���[�^�[
		inline bool operator ==(const DirectX::XMFLOAT4& vec) const;
		//!=�I�y���[�^�[
		inline bool operator !=(const DirectX::XMFLOAT4& vec) const;

		//���Z�I�y���[�^�[
		inline Vector4 operator +(const DirectX::XMFLOAT4& vec) const;
		//���Z�I�y���[�^�[
		inline Vector4 operator -(const DirectX::XMFLOAT4& vec) const;
		//��Z�I�y���[�^�[
		inline Vector4 operator *(const DirectX::XMFLOAT4& vec) const;
		//���Z�I�y���[�^�[
		inline Vector4 operator /(const DirectX::XMFLOAT4& vec) const;

		//+=�I�y���[�^�[
		inline const Vector4Ref &operator +=(const DirectX::XMFLOAT4& vec);
		//-=�I�y���[�^�[
		inline const Vector4Ref &operator -=(const DirectX::XMFLOAT4& vec);
		//*=�I�y���[�^�[
		inline const Vector4Ref &operator *=(const DirectX::XMFLOAT4& vec);
		// /=�I�y���[�^�[
		inline const Vector4Ref &operator /=(const DirectX::XMFLOAT4& vec);

		//----------------------------------------------------------------------------------
		//cpp����

		//Vector2�I�y���[�^�[
		operator Vector2() const;
		//Vector3�I�y���[�^�[
		operator Vector3() const;
		//Vector4Int�I�y���[�^�[
		operator Vector4Int() const;

		//=�I�y���[�^�[
		const Vector4Ref& operator = (const Vector2& vec);
		//=�I�y���[�^�[
		const Vector4Ref& operator = (const Vector3& vec);
		//=�I�y���[�^�[
		const Vector4Ref& operator = (const Vector4Int& vec);
	};


	//----------------------------------------------------------------------------------
	//[wostream operator]
	//����1: stream
	//����2: this
	inline std::wostream& operator<<(std::wostream& stream, const Vector4Ref& value)
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
	inline Vector4Ref::Vector4Ref(float& x, float& y, float& z, float& w) : x(x), y(y), z(z), w(w) {};
	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//�������������
	//����1: XMFLOAT2
	inline Vector4Ref::Vector4Ref(DirectX::XMFLOAT4 & vec) : x(vec.x), y(vec.y), z(vec.z), w(vec.w) { }

	//----------------------------------------------------------------------------------
	//[Absolute]
	//return: ��Βl
	inline constexpr Vector4 Vector4Ref::Absolute() const noexcept
	{
		return Vector4(x < 0.0f ? -x : x, y < 0.0f ? -y : y, z < 0.0f ? -z : z, w < 0.0f ? -w : w);
	}
	//----------------------------------------------------------------------------------
	//[Magnitude]
	//return: ���g��Vector(0,0)�̋���
	inline constexpr float Vector4Ref::Magnitude() const noexcept
	{
		float f = x * x + y * y + z * z + w * w;
		int i = 0x5f3759df - (*reinterpret_cast<int*>(&f) >> 1);
		float bitF = *reinterpret_cast<float*>(&i);

		bitF *= (1.5f - 0.5f * f * bitF * bitF);
		return f * bitF * (1.5f - 0.5f * f * bitF * bitF);
	}
	//----------------------------------------------------------------------------------
	//[ExactMagnitude]
	//return: ���g��Vector(0,0,0,0)�̋��� (���m�Ȓl�������ׂ�����)
	inline float Vector4Ref::ExactMagnitude() const noexcept
	{
		return std::sqrtf(x * x + y * y + z * z + w * w);
	}
	//----------------------------------------------------------------------------------
	//[Normalize]
	//return: ���g�̐��K���x�N�g��
	inline constexpr Vector4 Vector4Ref::Normalized() const noexcept
	{
		float divLength = 1.0f / Magnitude();
		return Vector4(x * divLength, y * divLength, z * divLength, w * divLength);
	}

	//XMVECTOR�I�y���[�^�[
	inline Vector4Ref::operator XmVector() const noexcept { return DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(x, y, z)); }

	//+�I�y���[�^�[  
	inline Vector4 Vector4Ref::operator+() const { return Vector4(+this->x, +this->y, +this->z, +this->w); }
	//-�I�y���[�^�[  
	inline Vector4 Vector4Ref::operator-() const { return Vector4(-this->x, -this->y, -this->z, -this->w); }
	//==�I�y���[�^�[
	inline bool Vector4Ref::operator==(const Vector4Ref& vec) const
	{
		return (fabsf(x - vec.x) <= MathAF::cEpsilon
			&& fabsf(y - vec.y) <= MathAF::cEpsilon
			&& fabsf(z - vec.z) <= MathAF::cEpsilon
			&& fabsf(w - vec.w) <= MathAF::cEpsilon);
	}
	//!=�I�y���[�^�[
	inline bool Vector4Ref::operator!=(const Vector4Ref& vec) const
	{
		return (fabsf(x - vec.x) <= MathAF::cEpsilon
			&& fabsf(y - vec.y) <= MathAF::cEpsilon
			&& fabsf(z - vec.z) <= MathAF::cEpsilon
			&& fabsf(w - vec.w) <= MathAF::cEpsilon);
	}

	//���Z�I�y���[�^�[
	inline Vector4 Vector4Ref::operator+(const Vector4Ref& vec) const { return Vector4(x + vec.x, y + vec.y, z + vec.z, w + vec.w); }
	//���Z�I�y���[�^�[
	inline Vector4 Vector4Ref::operator-(const Vector4Ref& vec) const { return Vector4(x - vec.x, y - vec.y, z - vec.z, w - vec.w); }
	//��Z�I�y���[�^�[
	inline Vector4 Vector4Ref::operator*(const Vector4Ref& vec) const { return Vector4(x * vec.x, y * vec.y, z * vec.z, w * vec.w); }
	//��Z�I�y���[�^�[(float)
	inline Vector4 Vector4Ref::operator*(const float value) const { return Vector4(x * value, y * value, z * value, w * value); }
	//���Z�I�y���[�^�[
	inline Vector4 Vector4Ref::operator/(const Vector4Ref& vec) const { return Vector4(x / vec.x, y / vec.y, z / vec.z, w / vec.w); }
	//���Z�I�y���[�^�[(float)
	inline Vector4 Vector4Ref::operator/(const float value) const { return Vector4(x / value, y / value, z / value, w / value); }

	//+=�I�y���[�^�[
	inline const Vector4Ref & Vector4Ref::operator+=(const Vector4Ref& vec)
	{
		this->x += vec.x;		this->y += vec.y;
		this->z += vec.z;		this->w += vec.w;
		return *this;
	}
	//-=�I�y���[�^�[
	inline const Vector4Ref & Vector4Ref::operator-=(const Vector4Ref& vec)
	{
		this->x -= vec.x;		this->y -= vec.y;
		this->z -= vec.z;		this->w -= vec.w;
		return *this;
	}
	//*=�I�y���[�^�[
	inline const Vector4Ref & Vector4Ref::operator*=(const Vector4Ref& vec)
	{
		this->x *= vec.x;		this->y *= vec.y;
		this->z *= vec.z;		this->w *= vec.w;
		return *this;
	}
	//*=�I�y���[�^�[(float)
	inline const Vector4Ref & Vector4Ref::operator*=(const float value)
	{
		this->x *= value;		this->y *= value;
		this->z *= value;		this->w *= value;
		return *this;
	}
	// /=�I�y���[�^�[
	inline const Vector4Ref & Vector4Ref::operator/=(const Vector4Ref& vec)
	{
		this->x /= vec.x;		this->y /= vec.y;
		this->z /= vec.z;		this->w /= vec.w;
		return *this;
	}
	// /=�I�y���[�^�[(float)
	inline const Vector4Ref & Vector4Ref::operator/=(const float value)
	{
		this->x /= value;		this->y /= value;
		this->z /= value;		this->w /= value;
		return *this;
	}

	//----------------------------------------------------------------------------------
	//Vector4

	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//�������������
	//����1: Vector4
	Vector4Ref::Vector4Ref(Vector4 & value) : x(value.x), y(value.y), z(value.z), w(value.w) {}

	//=�I�y���[�^�[
	inline const Vector4Ref & Vector4Ref::operator=(const Vector4 & vec)
	{
		x = vec.x; y = vec.y;
		z = vec.z; w = vec.w;
		return *this;
	}

	//==�I�y���[�^�[
	inline bool Vector4Ref::operator==(const Vector4 & vec) const
	{
		return (fabsf(x - vec.x) <= MathAF::cEpsilon
			&& fabsf(y - vec.y) <= MathAF::cEpsilon
			&& fabsf(z - vec.z) <= MathAF::cEpsilon
			&& fabsf(w - vec.w) <= MathAF::cEpsilon);
	}
	//!=�I�y���[�^�[
	inline bool Vector4Ref::operator!=(const Vector4 & vec) const
	{
		return (fabsf(x - vec.x) <= MathAF::cEpsilon
			&& fabsf(y - vec.y) <= MathAF::cEpsilon
			&& fabsf(z - vec.z) <= MathAF::cEpsilon
			&& fabsf(w - vec.w) <= MathAF::cEpsilon);
	}

	//���Z�I�y���[�^�[
	inline Vector4 Vector4Ref::operator+(const Vector4 & vec) const { return Vector4(x + vec.x, y + vec.y, z + vec.z, w + vec.w); }
	//���Z�I�y���[�^�[
	inline Vector4 Vector4Ref::operator-(const Vector4 & vec) const { return Vector4(x - vec.x, y - vec.y, z - vec.z, w - vec.w); }
	//��Z�I�y���[�^�[
	inline Vector4 Vector4Ref::operator*(const Vector4 & vec) const { return Vector4(x * vec.x, y * vec.y, z * vec.z, w * vec.w); }
	//���Z�I�y���[�^�[
	inline Vector4 Vector4Ref::operator/(const Vector4 & vec) const { return Vector4(x / vec.x, y / vec.y, z / vec.z, w / vec.w); }

	//+=�I�y���[�^�[
	inline const Vector4Ref & Vector4Ref::operator+=(const Vector4 & vec)
	{
		this->x += vec.x;		this->y += vec.y;
		this->z += vec.z;		this->w += vec.w;
		return *this;
	}
	//-=�I�y���[�^�[
	inline const Vector4Ref & Vector4Ref::operator-=(const Vector4 & vec)
	{
		this->x -= vec.x;		this->y -= vec.y;
		this->z -= vec.z;		this->w -= vec.w;
		return *this;
	}
	//*=�I�y���[�^�[
	inline const Vector4Ref & Vector4Ref::operator*=(const Vector4 & vec)
	{
		this->x *= vec.x;		this->y *= vec.y;
		this->z *= vec.z;		this->w *= vec.w;
		return *this;
	}
	// /=�I�y���[�^�[
	inline const Vector4Ref & Vector4Ref::operator/=(const Vector4 & vec)
	{
		this->x /= vec.x;		this->y /= vec.y;
		this->z /= vec.z;		this->w /= vec.w;
		return *this;
	}


	//=�I�y���[�^�[
	inline const Vector4Ref & Vector4Ref::operator=(const DirectX::XMFLOAT4 & vec)
	{
		x = vec.x; y = vec.y;
		z = vec.z; w = vec.w;
		return *this;
	}

	//==�I�y���[�^�[
	inline bool Vector4Ref::operator==(const DirectX::XMFLOAT4 & vec) const
	{
		return (fabsf(x - vec.x) <= MathAF::cEpsilon
			&& fabsf(y - vec.y) <= MathAF::cEpsilon
			&& fabsf(z - vec.z) <= MathAF::cEpsilon
			&& fabsf(w - vec.w) <= MathAF::cEpsilon);
	}
	//!=�I�y���[�^�[
	inline bool Vector4Ref::operator!=(const DirectX::XMFLOAT4 & vec) const
	{
		return (fabsf(x - vec.x) <= MathAF::cEpsilon
			&& fabsf(y - vec.y) <= MathAF::cEpsilon
			&& fabsf(z - vec.z) <= MathAF::cEpsilon
			&& fabsf(w - vec.w) <= MathAF::cEpsilon);
	}

	//���Z�I�y���[�^�[
	inline Vector4 Vector4Ref::operator+(const DirectX::XMFLOAT4 & vec) const { return Vector4(x + vec.x, y + vec.y, z + vec.z, w + vec.w); }
	//���Z�I�y���[�^�[
	inline Vector4 Vector4Ref::operator-(const DirectX::XMFLOAT4 & vec) const { return Vector4(x - vec.x, y - vec.y, z - vec.z, w - vec.w); }
	//��Z�I�y���[�^�[
	inline Vector4 Vector4Ref::operator*(const DirectX::XMFLOAT4 & vec) const { return Vector4(x * vec.x, y * vec.y, z * vec.z, w * vec.w); }
	//���Z�I�y���[�^�[
	inline Vector4 Vector4Ref::operator/(const DirectX::XMFLOAT4 & vec) const { return Vector4(x / vec.x, y / vec.y, z / vec.z, w / vec.w); }

	//+=�I�y���[�^�[
	inline const Vector4Ref & Vector4Ref::operator+=(const DirectX::XMFLOAT4 & vec)
	{
		this->x += vec.x;		this->y += vec.y;
		this->z += vec.z;		this->w += vec.w;
		return *this;
	}
	//-=�I�y���[�^�[
	inline const Vector4Ref & Vector4Ref::operator-=(const DirectX::XMFLOAT4 & vec)
	{
		this->x -= vec.x;		this->y -= vec.y;
		this->z -= vec.z;		this->w -= vec.w;
		return *this;
	}
	//*=�I�y���[�^�[
	inline const Vector4Ref & Vector4Ref::operator*=(const DirectX::XMFLOAT4 & vec)
	{
		this->x *= vec.x;		this->y *= vec.y;
		this->z *= vec.z;		this->w *= vec.w;
		return *this;
	}
	// /=�I�y���[�^�[
	inline const Vector4Ref & Vector4Ref::operator/=(const DirectX::XMFLOAT4 & vec)
	{
		this->x /= vec.x;		this->y /= vec.y;
		this->z /= vec.z;		this->w /= vec.w;
		return *this;
	}
}
#endif // !SGFRAMEWORK_HEADER_VECTOR_VECTOR3_REF_HPP_
