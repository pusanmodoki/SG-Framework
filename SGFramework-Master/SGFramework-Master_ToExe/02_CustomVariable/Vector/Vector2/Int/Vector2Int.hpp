/*----------------------------------------------------------------------------------
2�̒l��������Vector2Int �\����
�ł����p����\���̃V���[�Y��1�Ȃ̂ŕʎ���Vector���֌W�Ȃ����̂͑S��inline����
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_VECTOR_VECTOR2INT_HPP_
#define SGFRAMEWORK_HEADER_VECTOR_VECTOR2INT_HPP_
#include <PxPhysicsAPI.h>
#include <assimp/vector2.h>
#include <DirectXMath.h>
#include <math.h>
#include <iostream>
#include "imgui.h"
#include "../../../ConstAndUsing/ConstAndUsing.hpp"
#include "../../../Property/Property.hpp"
#include "../../../String/String.hpp"

//Framework namespace
namespace SGFramework
{
	//�v���g�^�C�v�錾
	struct Vector3Int;
	struct Vector4Int;
	struct Vector2;
	struct Vector2Ref;

	//2�̒l��������Vector2Int �\����
	struct Vector2Int final
	{
		//----------------------------------------------------------------------------------
		//inlne����

		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�S��0�ɐݒ肳���
		inline constexpr Vector2Int();
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�������������
		//����1: x
		//����2: y
		inline constexpr Vector2Int(int x, int y);
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�������������
		//����1: int*
		inline constexpr Vector2Int(const int* array);

		//----------------------------------------------------------------------------------
		//[ToXmVector]
		//return: XmVector
		inline XmVector ToXmVector() const noexcept {
			return DirectX::XMLoadFloat2(&DirectX::XMFLOAT2(static_cast<float>(x), static_cast<float>(y))); }
		//----------------------------------------------------------------------------------
		//[ToPhysX]
		//return: physx::PxVec
		inline physx::PxVec2 ToPhysX() const noexcept { return physx::PxVec2(
			static_cast<float>(x), static_cast<float>(y)); }
		//----------------------------------------------------------------------------------
		//[ToAssimp]
		//return: aiVector2D
		inline aiVector2D ToAssimp() const noexcept { return aiVector2D(
			static_cast<float>(x), static_cast<float>(y)); }
		//----------------------------------------------------------------------------------
		//[ToImGui]
		//return: ImVec2
		inline ImVec2 ToImGui() const noexcept { return ImVec2(
			static_cast<float>(x), static_cast<float>(y)); }

		//default
		Vector2Int(const Vector2Int&) = default;
		Vector2Int& operator=(const Vector2Int&) = default;
		Vector2Int(Vector2Int&&) = default;
		Vector2Int& operator=(Vector2Int&&) = default;

		//----------------------------------------------------------------------------------
		//�ϐ�
		union
		{
			struct
			{
				int x;		//X
				int y;		//Y
			};
			int value[2];	//Value
		};

		//----------------------------------------------------------------------------------
		//[Absolute]
		//return: ��Βl
		inline Vector2Int Absolute() const noexcept;
		//----------------------------------------------------------------------------------
		//[Magnitude]
		//return: ���g��Vector(0,0)�̋���
		inline constexpr float Magnitude() const noexcept;
		//----------------------------------------------------------------------------------
		//[ExactMagnitude]
		//return: ���g��Vector(0,0)�̋��� (���m�Ȓl�������ׂ�����)
		inline float ExactMagnitude() const noexcept;

		//int*�I�y���[�^�[
		inline operator int* ();
		//const int*�I�y���[�^�[
		inline operator const int*() const;

		//+�I�y���[�^�[  
		inline Vector2Int operator +() const;
		//-�I�y���[�^�[  
		inline Vector2Int operator -() const;
		//[]�I�y���[�^�[
		inline int& operator [](int i);
		//==�I�y���[�^�[
		inline bool operator ==(const Vector2Int& vec) const;
		//!=�I�y���[�^�[
		inline bool operator !=(const Vector2Int& vec) const;

		//���Z�I�y���[�^�[
		inline Vector2Int operator +(const Vector2Int& vec) const;
		//���Z�I�y���[�^�[
		inline Vector2Int operator -(const Vector2Int& vec) const;
		//��Z�I�y���[�^�[
		inline Vector2Int operator *(const Vector2Int& vec) const;
		//��Z�I�y���[�^�[(int)
		inline Vector2Int operator *(int value) const;
		//���Z�I�y���[�^�[
		inline Vector2Int operator /(const Vector2Int& vec) const;
		//���Z�I�y���[�^�[(int)
		inline Vector2Int operator /(int value) const;

		//+=�I�y���[�^�[
		inline const Vector2Int &operator +=(const Vector2Int& vec);
		//-=�I�y���[�^�[
		inline const Vector2Int &operator -=(const Vector2Int& vec);
		//*=�I�y���[�^�[
		inline const Vector2Int &operator *=(const Vector2Int& vec);
		//*=�I�y���[�^�[(int)
		inline const Vector2Int &operator *=(int value);
		// /=�I�y���[�^�[
		inline const Vector2Int &operator /=(const Vector2Int& vec);
		// /=�I�y���[�^�[(int)
		inline const Vector2Int &operator /=(int value);


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
		//����1: Vector2
		Vector2Int(const Vector2& value);

		//----------------------------------------------------------------------------------
		//[ToFloat]
		//Vector2�ɕϊ����s��
		//return: Vector2�ɕϊ�����this
		Vector2 ToFloat() const;

		//Vector3Int�I�y���[�^�[
		operator Vector3Int() const;
		//Vector4Int�I�y���[�^�[
		operator Vector4Int() const;

		//=�I�y���[�^�[
		const Vector2Int& operator = (const Vector3Int& vec);
		//=�I�y���[�^�[
		const Vector2Int& operator = (const Vector4Int& vec);
		//=�I�y���[�^�[
		const Vector2Int& operator = (const Vector2& vec);
		//=�I�y���[�^�[
		const Vector2Int& operator = (const Vector2Ref& vec);
	};


	//----------------------------------------------------------------------------------
	//[wostream operator]
	//����1: stream
	//����2: this
	inline std::wostream& operator<<(std::wostream& stream, const Vector2Int& value)
	{
		const wchar* string = value.ToString();
		stream << string;
		return stream;
	}

	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//�S��0�ɐݒ肳���
	inline constexpr Vector2Int::Vector2Int() : x(0), y(0) {}
	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//�������������
	//����1: x
	//����2: y
	inline constexpr Vector2Int::Vector2Int(int x, int y) : x(x), y(y) {}
	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//�������������
	//����1: int*
	inline constexpr Vector2Int::Vector2Int(const int * array) : x(array[0]), y(array[1]) {}


	//----------------------------------------------------------------------------------
	//[Absolute]
	//return: ��Βl
	inline Vector2Int Vector2Int::Absolute() const noexcept
	{
		return std::move(Vector2Int(x < 0 ? -x : x, y < 0 ? -y : y));
	}
	//----------------------------------------------------------------------------------
	//[Magnitude]
	//return: ���g��Vector(0,0)�̋���
	inline constexpr float Vector2Int::Magnitude() const noexcept
	{
		float f = static_cast<float>(x * x + y * y);
		int i = 0x5f3759df - (*reinterpret_cast<int*>(&f) >> 1);
		float bitF = *reinterpret_cast<float*>(&i);

		bitF *= (1.5f - 0.5f * f * bitF * bitF);
		return f * bitF * (1.5f - 0.5f * f * bitF * bitF);
	}
	//----------------------------------------------------------------------------------
	//[ExactMagnitude]
	//return: ���g��Vector(0,0)�̋��� (���m�Ȓl�������ׂ�����)
	inline float Vector2Int::ExactMagnitude() const noexcept
	{
		return std::sqrtf(static_cast<float>(x * x + y * y));
	}

	//int*�I�y���[�^�[
	inline Vector2Int::operator int*() { return value; }
	//const int*�I�y���[�^�[
	inline Vector2Int::operator const int*() const { return value; }

	//-�I�y���[�^�[  
	inline Vector2Int Vector2Int::operator+() const { return Vector2Int(+this->x, +this->y); }
	//-�I�y���[�^�[  
	inline Vector2Int Vector2Int::operator-() const { return Vector2Int(-this->x, -this->y); }
	//[]�I�y���[�^�[
	inline int& Vector2Int::operator[](int i) { return value[i]; }
	//==�I�y���[�^�[
	inline bool Vector2Int::operator==(const Vector2Int& vec) const { return (x == vec.x && y == vec.y); }
	//!=�I�y���[�^�[
	inline bool Vector2Int::operator!=(const Vector2Int& vec) const { return (x != vec.x || y != vec.y); }

	//���Z�I�y���[�^�[
	inline Vector2Int Vector2Int::operator+(const Vector2Int& vec) const { return Vector2Int(x + vec.x, y + vec.y); }
	//���Z�I�y���[�^�[
	inline Vector2Int Vector2Int::operator-(const Vector2Int& vec) const { return Vector2Int(x - vec.x, y - vec.y); }
	//��Z�I�y���[�^�[
	inline Vector2Int Vector2Int::operator*(const Vector2Int& vec) const { return Vector2Int(x * vec.x, y * vec.y); }
	//��Z�I�y���[�^�[(int)
	inline Vector2Int Vector2Int::operator*(int value) const { return Vector2Int(x * value, y * value); }
	//���Z�I�y���[�^�[
	inline Vector2Int Vector2Int::operator/(const Vector2Int& vec) const { return Vector2Int(x / vec.x, y / vec.y); }
	//���Z�I�y���[�^�[(int)
	inline Vector2Int Vector2Int::operator/(int value) const { return Vector2Int(x / value, y / value); }

	//+=�I�y���[�^�[
	inline const Vector2Int & Vector2Int::operator+=(const Vector2Int& vec)
	{
		this->x += vec.x;		this->y += vec.y;
		return *this;
	}
	//-=�I�y���[�^�[
	inline const Vector2Int & Vector2Int::operator-=(const Vector2Int& vec)
	{
		this->x -= vec.x;		this->y -= vec.y;
		return *this;
	}
	//*=�I�y���[�^�[
	inline const Vector2Int & Vector2Int::operator*=(const Vector2Int& vec)
	{
		this->x *= vec.x;		this->y *= vec.y;
		return *this;
	}
	//*=�I�y���[�^�[(int)
	inline const Vector2Int & Vector2Int::operator*=(int value)
	{
		this->x *= value;		this->y *= value;
		return *this;
	}
	// /=�I�y���[�^�[
	inline const Vector2Int& Vector2Int::operator/=(const Vector2Int& vec)
	{
		this->x /= vec.x;		this->y /= vec.y;
		return *this;
	}
	// /=�I�y���[�^�[(int)
	inline const Vector2Int & Vector2Int::operator/=(int value)
	{
		this->x /= value;		this->y /= value;
		return *this;
	}
}
#endif //!_VECTOR_VECTOR2INT_HPP_