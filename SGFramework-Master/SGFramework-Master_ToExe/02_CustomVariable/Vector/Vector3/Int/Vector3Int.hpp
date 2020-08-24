/*----------------------------------------------------------------------------------
3�̒l��������Vector3Int �\����
�ł����p����\���̃V���[�Y��1�Ȃ̂ŕʎ���Vector���֌W�Ȃ����̂͑S��inline����
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_VECTOR_VECTOR3INT_HPP_
#define SGFRAMEWORK_HEADER_VECTOR_VECTOR3INT_HPP_
#include <PxPhysicsAPI.h>
#include <assimp/vector3.h>
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
	struct Vector4Int;
	struct Vector3;
	struct Vector3Ref;

	//3�̒l��������Vector3Int �\����
	struct Vector3Int final
	{
		//----------------------------------------------------------------------------------
		//inlne����

		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�S��0�ɐݒ肳���
		inline constexpr Vector3Int();
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�������������
		//����1: x
		//����2: y
		//����3: z
		inline constexpr Vector3Int(int x, int y, int z);
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�������������
		//����1: int*
		inline Vector3Int(const int* array);

		//----------------------------------------------------------------------------------
		//[ToXmVector]
		//return: XmVector
		inline XmVector ToXmVector() const noexcept {
			return DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(
				static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)));
		}
		//----------------------------------------------------------------------------------
		//[ToPhysX]
		//return: physx::PxVec3
		inline physx::PxVec3 ToPhysX() const noexcept { return physx::PxVec3(
			static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)); }
		//----------------------------------------------------------------------------------
		//[ToAssimp]
		//return: aiVector3D
		inline aiVector3D ToAssimp() const noexcept { return aiVector3D(
			static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)); }

		//default
		Vector3Int(const Vector3Int&) = default;
		Vector3Int& operator=(const Vector3Int&) = default;
		Vector3Int(Vector3Int&&) = default;
		Vector3Int& operator=(Vector3Int&&) = default;

		//----------------------------------------------------------------------------------
		//�ϐ�
		union
		{
			struct
			{
				int x;		//X
				int y;		//Y
				int z;		//Z
			};
			int value[3];	//Value
		};

		//----------------------------------------------------------------------------------
		//[Absolute]
		//return: ��Βl
		inline Vector3Int Absolute() const noexcept;
		//----------------------------------------------------------------------------------
		//[Magnitude]
		//return: ���g��Vector(0,0,0)�̋���
		inline constexpr float Magnitude() const noexcept;
		//----------------------------------------------------------------------------------
		//[ExactMagnitude]
		//return: ���g��Vector(0,0,0)�̋��� (���m�Ȓl�������ׂ�����)
		inline float ExactMagnitude() const noexcept;

		//int*�I�y���[�^�[
		inline operator int* ();
		//const int*�I�y���[�^�[
		inline operator const int*() const;

		//-�I�y���[�^�[  
		inline Vector3Int operator +() const;
		//-�I�y���[�^�[  
		inline Vector3Int operator -() const;
		//==�I�y���[�^�[
		inline bool operator ==(const Vector3Int& vec) const;
		//!=�I�y���[�^�[
		inline bool operator !=(const Vector3Int& vec) const;

		//���Z�I�y���[�^�[
		inline Vector3Int operator +(const Vector3Int& vec) const;
		//���Z�I�y���[�^�[
		inline Vector3Int operator -(const Vector3Int& vec) const;
		//��Z�I�y���[�^�[
		inline Vector3Int operator *(const Vector3Int& vec) const;
		//��Z�I�y���[�^�[(int)
		inline Vector3Int operator *(int value) const;
		//���Z�I�y���[�^�[
		inline Vector3Int operator /(const Vector3Int& vec) const;
		//���Z�I�y���[�^�[(int)
		inline Vector3Int operator /(int value) const;

		//+=�I�y���[�^�[
		inline const Vector3Int &operator +=(const Vector3Int& vec);
		//-=�I�y���[�^�[
		inline const Vector3Int &operator -=(const Vector3Int& vec);
		//*=�I�y���[�^�[
		inline const Vector3Int &operator *=(const Vector3Int& vec);
		//*=�I�y���[�^�[(int)
		inline const Vector3Int &operator *=(int value);
		// /=�I�y���[�^�[
		inline const Vector3Int &operator /=(const Vector3Int& vec);
		// /=�I�y���[�^�[(int)
		inline const Vector3Int &operator /=(int value);


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
		inline std::string ToStdString() const { return ("(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")"); }

		//----------------------------------------------------------------------------------
		//cpp����

		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�������������
		//����1: Vector3
		Vector3Int(const Vector3& value);

		//----------------------------------------------------------------------------------
		//[ToFloat]
		//Vector3�ɕϊ����s��
		//return: Vector3�ɕϊ�����this
		Vector3 ToFloat() const;

		//Vector2Int�I�y���[�^�[
		operator Vector2Int() const;
		//Vector4Int�I�y���[�^�[
		operator Vector4Int() const;

		//=�I�y���[�^�[
		const Vector3Int& operator = (const Vector2Int& vec);
		//=�I�y���[�^�[
		const Vector3Int& operator = (const Vector4Int& vec);
		//=�I�y���[�^�[
		const Vector3Int& operator = (const Vector3& vec);
		//=�I�y���[�^�[
		const Vector3Int& operator = (const Vector3Ref& vec);
	};

	//----------------------------------------------------------------------------------
	//[wostream operator]
	//����1: stream
	//����2: this
	inline std::wostream& operator<<(std::wostream& stream, const Vector3Int& value)
	{
		const wchar* string = value.ToString();
		stream << string;
		return stream;
	}

	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//�S��0�ɐݒ肳���
	inline constexpr Vector3Int::Vector3Int() : x(0), y(0), z(0) {}
	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//�������������
	//����1: x
	//����2: y
	//����3: z
	inline constexpr Vector3Int::Vector3Int(int x, int y, int z) : x(x), y(y), z(z) {}
	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//�������������
	//����1: int*
	inline Vector3Int::Vector3Int(const int * array)
	{
		value[0] = array[0];	value[1] = array[1];
		value[2] = array[2];
	}

	//----------------------------------------------------------------------------------
	//[Absolute]
	//return: ��Βl
	inline Vector3Int Vector3Int::Absolute() const noexcept
	{
		return std::move(Vector3Int(x < 0 ? -x : x, y < 0 ? -y : y, z < 0 ? -z : z));
	}
	//----------------------------------------------------------------------------------
	//[Magnitude]
	//return: ���g��Vector(0,0,0)�̋���
	inline constexpr float Vector3Int::Magnitude() const noexcept
	{
		float f = static_cast<float>(x * x + y * y + z * z);
		int i = 0x5f3759df - (*reinterpret_cast<int*>(&f) >> 1);
		float bitF = *reinterpret_cast<float*>(&i);

		bitF *= (1.5f - 0.5f * f * bitF * bitF);
		return f * bitF * (1.5f - 0.5f * f * bitF * bitF);
	}
	//----------------------------------------------------------------------------------
	//[ExactMagnitude]
	//return: ���g��Vector(0,0,0)�̋��� (���m�Ȓl�������ׂ�����)
	inline float Vector3Int::ExactMagnitude() const noexcept
	{
		return std::sqrtf(static_cast<float>(x * x + y * y + z * z));
	}

	//int*�I�y���[�^�[
	inline Vector3Int::operator int*() { return value; }
	//const int*�I�y���[�^�[
	inline Vector3Int::operator const int*() const { return value; }

	//+�I�y���[�^�[  
	inline Vector3Int Vector3Int::operator+() const { return Vector3Int(+this->x, +this->y, +this->z); }
	//-�I�y���[�^�[  
	inline Vector3Int Vector3Int::operator-() const { return Vector3Int(-this->x, -this->y, -this->z); }
	//==�I�y���[�^�[
	inline bool Vector3Int::operator==(const Vector3Int& vec) const { return (x == vec.x && y == vec.y && z == vec.z); }
	//!=�I�y���[�^�[
	inline bool Vector3Int::operator!=(const Vector3Int& vec) const { return (x != vec.x || y != vec.y || z != vec.z); }

	//���Z�I�y���[�^�[
	inline Vector3Int Vector3Int::operator+(const Vector3Int& vec) const { return Vector3Int(x + vec.x, y + vec.y, z + vec.z); }
	//���Z�I�y���[�^�[
	inline Vector3Int Vector3Int::operator-(const Vector3Int& vec) const { return Vector3Int(x - vec.x, y - vec.y, z - vec.z); }
	//��Z�I�y���[�^�[
	inline Vector3Int Vector3Int::operator*(const Vector3Int& vec) const { return Vector3Int(x * vec.x, y * vec.y, z * vec.z); }
	//��Z�I�y���[�^�[(int)
	inline Vector3Int Vector3Int::operator*(int value) const { return Vector3Int(x * value, y * value, z * value); }
	//���Z�I�y���[�^�[
	inline Vector3Int Vector3Int::operator/(const Vector3Int& vec) const { return Vector3Int(x / vec.x, y / vec.y, z / vec.z); }
	//���Z�I�y���[�^�[(int)
	inline Vector3Int Vector3Int::operator/(int value) const { return Vector3Int(x / value, y / value, z / value); }

	//+=�I�y���[�^�[
	inline const Vector3Int & Vector3Int::operator+=(const Vector3Int& vec)
	{
		this->x += vec.x;		this->y += vec.y;
		this->z += vec.z;
		return *this;
	}
	//-=�I�y���[�^�[
	inline const Vector3Int & Vector3Int::operator-=(const Vector3Int& vec)
	{
		this->x -= vec.x;		this->y -= vec.y;
		this->z -= vec.z;
		return *this;
	}
	//*=�I�y���[�^�[
	inline const Vector3Int & Vector3Int::operator*=(const Vector3Int& vec)
	{
		this->x *= vec.x;		this->y *= vec.y;
		this->z *= vec.z;
		return *this;
	}
	//*=�I�y���[�^�[(int)
	inline const Vector3Int & Vector3Int::operator*=(int value)
	{
		this->x *= value;		this->y *= value;
		this->z *= value;
		return *this;
	}
	// /=�I�y���[�^�[
	inline const Vector3Int & Vector3Int::operator/=(const Vector3Int& vec)
	{
		this->x /= vec.x;		this->y /= vec.y;
		this->z /= vec.z;
		return *this;
	}
	// /=�I�y���[�^�[(int)
	inline const Vector3Int & Vector3Int::operator/=(int value)
	{
		this->x /= value;		this->y /= value;
		this->z /= value;
		return *this;
	}
}
#endif //!_VECTOR_VECTOR3INT_HPP_