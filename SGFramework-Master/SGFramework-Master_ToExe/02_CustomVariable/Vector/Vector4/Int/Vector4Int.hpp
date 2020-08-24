/*----------------------------------------------------------------------------------
4��int�l��������Vector4Int �\����
�ł����p����\���̃V���[�Y��1�Ȃ̂ŕʎ���Vector���֌W�Ȃ����̂͑S��inline����
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_VECTOR_VECTOR4INT_HPP_
#define SGFRAMEWORK_HEADER_VECTOR_VECTOR4INT_HPP_
#include <PxPhysicsAPI.h>
#include <DirectXMath.h>
#include <iostream>
#include "../../../ConstAndUsing/ConstAndUsing.hpp"
#include "../../../Property/Property.hpp"
#include "../../../String/String.hpp"

//Framework namespace
namespace SGFramework
{
	//�v���g�^�C�v�錾
	struct Vector2Int;
	struct Vector3Int;
	struct Vector4;
	struct Vector4Ref;

	//4��int�l��������Vector4Int �\����
	struct Vector4Int final
	{
		//----------------------------------------------------------------------------------
		//inlne����

		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�S��0�ɐݒ肳���
		inline constexpr Vector4Int();
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�������������
		//����1: x
		//����2: y
		//����3: z
		//����4: w
		inline constexpr Vector4Int(int x, int y, int z, int w);
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�������������
		//����1: int*
		inline Vector4Int(const int* array);

		//----------------------------------------------------------------------------------
		//[ToXmVector]
		//return: XmVector
		inline XmVector ToXmVector() const noexcept { return DirectX::XMLoadFloat4(&DirectX::XMFLOAT4(
			static_cast<float>(x), static_cast<float>(y),
			static_cast<float>(z), static_cast<float>(w))); }
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
		inline physx::PxVec4 ToPhysX() const noexcept { return physx::PxVec4(
			static_cast<float>(x), static_cast<float>(y),
			static_cast<float>(z), static_cast<float>(w)); }

		//default
		Vector4Int(const Vector4Int&) = default;
		Vector4Int& operator=(const Vector4Int&) = default;
		Vector4Int(Vector4Int&&) = default;
		Vector4Int& operator=(Vector4Int&&) = default;

		//----------------------------------------------------------------------------------
		//�ϐ�
		union
		{
			struct
			{
				int x;		//X
				int y;		//Y
				int z;		//Z
				int w;	//W
			};
			int value[4];	//Value
		};
		
		//int*�I�y���[�^�[
		inline operator int* ();
		//const int*�I�y���[�^�[
		inline operator const int*() const;

		//+�I�y���[�^�[  
		inline Vector4Int operator +() const;
		//-�I�y���[�^�[  
		inline Vector4Int operator -() const;
		//==�I�y���[�^�[
		inline bool operator ==(const Vector4Int& vec) const;
		//!=�I�y���[�^�[
		inline bool operator !=(const Vector4Int& vec) const;

		//���Z�I�y���[�^�[
		inline Vector4Int operator +(const Vector4Int& vec) const;
		//���Z�I�y���[�^�[
		inline Vector4Int operator -(const Vector4Int& vec) const;
		//��Z�I�y���[�^�[
		inline Vector4Int operator *(const Vector4Int& vec) const;
		//��Z�I�y���[�^�[(int)
		inline Vector4Int operator *(int value) const;
		//���Z�I�y���[�^�[
		inline Vector4Int operator /(const Vector4Int& vec) const;
		//���Z�I�y���[�^�[(int)
		inline Vector4Int operator /(int value) const;

		//+=�I�y���[�^�[
		inline const Vector4Int &operator +=(const Vector4Int & vec);
		//-=�I�y���[�^�[
		inline const Vector4Int &operator -=(const Vector4Int & vec);
		//*=�I�y���[�^�[
		inline const Vector4Int &operator *=(const Vector4Int & vec);
		//*=�I�y���[�^�[(int)
		inline const Vector4Int &operator *=(int value);
		// /=�I�y���[�^�[
		inline const Vector4Int &operator /=(const Vector4Int & vec);
		// /=�I�y���[�^�[(int)
		inline const Vector4Int &operator /=(int value);

		//----------------------------------------------------------------------------------
		//[Absolute]
		//return: ��Βl
		inline constexpr Vector4Int Absolute() const noexcept;
		//----------------------------------------------------------------------------------
		//[Magnitude]
		//return: ���g��Vector(0,0, 0, 0)�̋���
		inline constexpr float Magnitude() const;
		//----------------------------------------------------------------------------------
		//[ExactMagnitude]
		//return: ���g��Vector(0,0,0,0)�̋��� (���m�Ȓl�������ׂ�����)
		inline float ExactMagnitude() const;

		//sgstring operator
		inline operator sgstring() const { return ToString(); }
		//----------------------------------------------------------------------------------
		//[ToString]
		//sgstring�`���ŏo�͂���
		//return: sgstring�\������this
		inline sgstring ToString() const { return (L"(" + std::to_wstring(x) + L", " + std::to_wstring(y) + L", " + std::to_wstring(z) + L", " + std::to_wstring(w) + L")"); }
		//----------------------------------------------------------------------------------
		//[ToStdString]
		//std::string�`���ŏo�͂���
		//return: std::string�\������this
		inline std::string ToStdString() const { return ("(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ", " + std::to_string(w) + ")"); }

		//----------------------------------------------------------------------------------
		//cpp����
		
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�������������
		//����1: Vector4
		Vector4Int(const Vector4& value);

		//----------------------------------------------------------------------------------
		//[ToFloat]
		//Vector4�ɕϊ����s��
		//return: Vector4�ɕϊ�����this
		Vector4 ToFloat() const;

		//Vector2Int�I�y���[�^�[
		operator Vector2Int() const;
		//Vector3Int�I�y���[�^�[
		operator Vector3Int() const;

		//=�I�y���[�^�[
		const Vector4Int& operator = (const Vector2Int& vec);
		//=�I�y���[�^�[
		const Vector4Int& operator = (const Vector3Int& vec);
		//=�I�y���[�^�[
		const Vector4Int& operator = (const Vector4& vec);
		//=�I�y���[�^�[
		const Vector4Int& operator = (const Vector4Ref& vec);
	};


	//----------------------------------------------------------------------------------
	//[wostream operator]
	//����1: stream
	//����2: this
	inline std::wostream& operator<<(std::wostream& stream, const Vector4Int& value)
	{
		const wchar* string = value.ToString();
		stream << string;
		return stream;
	}


	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//�S��0�ɐݒ肳���
	inline constexpr Vector4Int::Vector4Int()
		: x(0), y(0), z(0), w(0) {}
	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//�������������
	//����1: x
	//����2: y
	//����3: z
	//����4: w
	inline constexpr Vector4Int::Vector4Int(int x, int y, int z, int w)
		: x(x), y(y), z(z), w(w) {}
	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//�������������
	//����1: int*
	inline Vector4Int::Vector4Int(const int * array)
		: x(array[0]), y(array[1]), z(array[2]), w(array[3]) {}

	//int*�I�y���[�^�[
	inline Vector4Int::operator int*() { return value; }
	//const int*�I�y���[�^�[
	inline Vector4Int::operator const int*() const { return value; }

	//+�I�y���[�^�[  
	inline Vector4Int Vector4Int::operator+() const { return Vector4Int(+this->x, +this->y, +this->z, +this->w); }
	//-�I�y���[�^�[  
	inline Vector4Int Vector4Int::operator-() const{ return Vector4Int(-this->x, -this->y, -this->z, -this->w); }
	//==�I�y���[�^�[
	inline bool Vector4Int::operator==(const Vector4Int& vec) const{ return (x == vec.x && y == vec.y && z == vec.z && w == vec.w); }
	//!=�I�y���[�^�[
	inline bool Vector4Int::operator!=(const Vector4Int& vec) const { return (x != vec.x || y != vec.y || z != vec.z || w != vec.w); }

	//���Z�I�y���[�^�[
	inline Vector4Int Vector4Int::operator+(const Vector4Int& vec) const { return Vector4Int(x + vec.x, y + vec.y, z + vec.z, w + vec.w); }
	//���Z�I�y���[�^�[
	inline Vector4Int Vector4Int::operator-(const Vector4Int& vec) const { return Vector4Int(x - vec.x, y - vec.y, z - vec.z, w - vec.w); }
	//��Z�I�y���[�^�[
	inline Vector4Int Vector4Int::operator*(const Vector4Int& vec) const { return Vector4Int(x * vec.x, y * vec.y, z * vec.z, w * vec.w); }
	//��Z�I�y���[�^�[(int)
	inline Vector4Int Vector4Int::operator*(int value) const { return Vector4Int(x * value, y * value, z * value, w * value); }
	//���Z�I�y���[�^�[
	inline Vector4Int Vector4Int::operator/(const Vector4Int& vec) const { return Vector4Int(x / vec.x, y / vec.y, z / vec.z, w / vec.w); }
	//���Z�I�y���[�^�[(int)
	inline Vector4Int Vector4Int::operator/(int value) const { return Vector4Int(x / value, y / value, z / value, w / value); }

	//+=�I�y���[�^�[
	inline const Vector4Int & Vector4Int::operator+=(const Vector4Int & vec)
	{
		this->x += vec.x;		this->y += vec.y;
		this->z += vec.z;		this->w += vec.w;
		return *this;
	}
	//-=�I�y���[�^�[
	inline const Vector4Int & Vector4Int::operator-=(const Vector4Int & vec)
	{
		this->x -= vec.x;		this->y -= vec.y;
		this->z -= vec.z;		this->w -= vec.w;
		return *this;
	}
	//*=�I�y���[�^�[
	inline const Vector4Int & Vector4Int::operator*=(const Vector4Int & vec)
	{
		this->x *= vec.x;		this->y *= vec.y;
		this->z *= vec.z;		this->w *= vec.w;
		return *this;
	}
	//*=�I�y���[�^�[(int)
	inline const Vector4Int & Vector4Int::operator*=(int value)
	{
		this->x *= value;		this->y *= value;
		this->z *= value;		this->w *= value;
		return *this;
	}
	// /=�I�y���[�^�[
	inline const Vector4Int & Vector4Int::operator/=(const Vector4Int & vec)
	{
		this->x /= vec.x;		this->y /= vec.y;
		this->z /= vec.z;		this->w /= vec.w;
		return *this;
	}
	// /=�I�y���[�^�[(int)
	inline const Vector4Int & Vector4Int::operator/=(int value)
	{
		this->x /= value;		this->y /= value;
		this->z /= value;		this->w /= value;
		return *this;
	}

	//----------------------------------------------------------------------------------
	//[Absolute]
	//return: ��Βl
	inline constexpr Vector4Int Vector4Int::Absolute() const noexcept
	{
		return std::move(Vector4Int(x < 0 ? -x : x, y < 0 ? -y : y, z < 0 ? -z : z, w < 0 ? -w : w));
	}
	//----------------------------------------------------------------------------------
	//[Magnitude]
	//return: ���g��Vector(0,0, 0, 0)�̋���
	inline constexpr float Vector4Int::Magnitude() const
	{
		float f = static_cast<float>(x * x + y * y + z * z + w * w);
		int i = 0x5f3759df - (*reinterpret_cast<int*>(&f) >> 1);
		float bitF = *reinterpret_cast<float*>(&i);

		bitF *= (1.5f - 0.5f * f * bitF * bitF);
		return f * bitF * (1.5f - 0.5f * f * bitF * bitF);
	}
	//----------------------------------------------------------------------------------
	//[ExactMagnitude]
	//return: ���g��Vector(0,0,0,0)�̋��� (���m�Ȓl�������ׂ�����)
	inline float Vector4Int::ExactMagnitude() const
	{
		return std::sqrtf(static_cast<float>(x * x + y * y + z * z + w * w));
	}
}

#endif // !SGFRAMEWORK_HEADER_VECTOR_VECTOR4INT_HPP_
