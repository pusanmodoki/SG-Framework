/*----------------------------------------------------------------------------------
3�̒l��������Vector3 �\����
�ł����p����\���̃V���[�Y��1�Ȃ̂ŕʎ���Vector���֌W�Ȃ����̂͑S��inline����
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_VECTOR_VECTOR3_HPP_
#define SGFRAMEWORK_HEADER_VECTOR_VECTOR3_HPP_
#include <DirectXMath.h>
#include <cmath>
#include <iostream>
#include <assimp/vector3.h>
#include <PxPhysicsAPI.h>
#include "../../ConstAndUsing/ConstAndUsing.hpp"
#include "../../Property/Property.hpp"
#include "../../String/String.hpp"

//Framework namespace
namespace SGFramework
{
	//�v���g�^�C�v�錾
	struct Vector2;
	struct Vector4;
	struct Vector3Int;
	struct Vector3Ref;
	struct Quaternion;
	
	//3�̒l��������Vector3 �\����
	struct Vector3 : public DirectX::XMFLOAT3
	{
	public:
		//----------------------------------------------------------------------------------
		//inlne����

		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�S��0�ɐݒ肳���
		inline constexpr Vector3();
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�������������
		//����1:x
		//����2:y
		//����3:z
		inline constexpr Vector3(float x, float y, float z);
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�������������
		//����1: XMFLOAT3
		inline constexpr Vector3(const DirectX::XMFLOAT3& value);
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�������������
		//����1: PxVec3
		inline constexpr Vector3(const physx::PxVec3& vec);
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�������������
		//����1: aiVector3D
		inline constexpr Vector3(const aiVector3D& vec);
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�������������
		//����1: XMVECTOR
		inline Vector3(const XmVector& value);
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�������������
		//����1: float*
		inline Vector3(const float* array);

		//----------------------------------------------------------------------------------
		//[ToXmVector]
		//return: XmVector
		inline XmVector ToXmVector() const noexcept { return DirectX::XMLoadFloat3(this); }
		//----------------------------------------------------------------------------------
		//[ToPhysX]
		//return: physx::PxVec3
		inline physx::PxVec3 ToPhysX() const noexcept { return physx::PxVec3(x, y, z); }
		//----------------------------------------------------------------------------------
		//[ToAssimp]
		//return: aiVector3D
		inline aiVector3D ToAssimp() const noexcept { return aiVector3D(x, y, z); }

		//default
		Vector3(const Vector3&) = default;
		Vector3& operator=(const Vector3&) = default;
		Vector3(Vector3&&) = default;
		Vector3& operator=(Vector3&&) = default;

		//----------------------------------------------------------------------------------
		//[CreateWithInt]
		//int��Vector3���쐬
		//����1:x
		//����2:y
		//����3:z
		inline static constexpr Vector3 CreateWithInt(int x, int y, int z);

		//----------------------------------------------------------------------------------
		//[Absolute]
		//return: ��Βl
		inline Vector3 Absolute() const noexcept;
		//----------------------------------------------------------------------------------
		//[Magnitude]
		//return: ���g��Vector(0,0,0)�̋���
		inline constexpr float Magnitude() const noexcept;
		//----------------------------------------------------------------------------------
		//[ExactMagnitude]
		//return: ���g��Vector(0,0,0)�̋��� (���m�Ȓl�������ׂ�����)
		inline float ExactMagnitude() const noexcept;
		//----------------------------------------------------------------------------------
		//[ClampMagnitude]
		//Length��maxLength�ɐ�������
		//return: ��������vector
		//����1: ����Length
		inline Vector3 ClampMagnitude(const float maxLength) const noexcept;
		//----------------------------------------------------------------------------------
		//[Normalize]
		//return: ���g�𐳋K�������x�N�g��
		inline Vector3 Normalized() const noexcept;
		//----------------------------------------------------------------------------------
		//[Orthogonal]
		//return: ���g�̃x�N�g���̒����x�N�g��
		inline Vector3 Orthogonal() const noexcept;
		//----------------------------------------------------------------------------------
		//[TransformCoord]
		//Matrix��t�^�����l�ɕϊ�����
		//return: Matrix��t�^�����x�N�g��
		//����1: �Ώ�Matrix
		inline Vector3 TransformCoord(const DirectX::XMMATRIX& matrix) const noexcept;
		//----------------------------------------------------------------------------------
		//[Rotate]
		//return: ���g�̃x�N�g���̉�]�x�N�g��
		inline Vector3 Rotate(const XmVector& quaternion) const noexcept;
		//----------------------------------------------------------------------------------
		//[ProgectOnPlane]
		//vec�𕽖ʏ�x�N�g���Ɏˉe����
		//return: �ˉe�σx�N�g��
		//����1: ���ʃx�N�g���̖@��(���K���ς̂���)
		inline Vector3 ProgectOnPlane(const Vector3& normal) const noexcept;
		
		//XMVECTOR�I�y���[�^�[
		inline operator XmVector() const noexcept;
		//float*�I�y���[�^�[
		inline operator float*();
		//const float*�I�y���[�^�[
		inline operator const float*() const;

		//=�I�y���[�^�[
		inline XmVector operator = (const XmVector& vec) noexcept;

		//+�I�y���[�^�[  
		inline Vector3 operator +() const;
		//-�I�y���[�^�[  
		inline Vector3 operator -() const;
		//==�I�y���[�^�[
		inline bool operator ==(const Vector3& vec) const;
		//!=�I�y���[�^�[
		inline bool operator !=(const Vector3& vec) const;

		//���Z�I�y���[�^�[
		inline Vector3 operator +(const Vector3& vec) const;
		//���Z�I�y���[�^�[
		inline Vector3 operator -(const Vector3& vec) const;
		//��Z�I�y���[�^�[
		inline Vector3 operator *(const Vector3& vec) const;
		//��Z�I�y���[�^�[(float)
		inline Vector3 operator *(float value) const;
		//���Z�I�y���[�^�[
		inline Vector3 operator /(const Vector3& vec) const;
		//���Z�I�y���[�^�[(float)
		inline Vector3 operator /(float value) const;

		//+=�I�y���[�^�[
		inline const Vector3 &operator +=(const Vector3& vec);
		//-=�I�y���[�^�[
		inline const Vector3 &operator -=(const Vector3& vec);
		//*=�I�y���[�^�[
		inline const Vector3 &operator *=(const Vector3& vec);
		//*=�I�y���[�^�[(float)
		inline const Vector3 &operator *=(float value);
		// /=�I�y���[�^�[
		inline const Vector3 &operator /=(const Vector3& vec);
		// /=�I�y���[�^�[(float)
		inline const Vector3 &operator /=(float value);


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
		//����1: Vector3Int
		Vector3(const Vector3Int& value);

		//Vector2�I�y���[�^�[
		operator Vector2() const;
		//Vector4�I�y���[�^�[
		operator Vector4() const;
		//Vector3Int�I�y���[�^�[
		operator Vector3Int() const;

		//=�I�y���[�^�[
		const Vector3& operator = (const Vector2& vec);
		//=�I�y���[�^�[
		const Vector3& operator = (const Vector4& vec);
		//=�I�y���[�^�[
		const Vector3& operator = (const Vector3Int& vec);
		//*�I�y���[�^�[
		Vector3 operator * (const Quaternion& quaternion) const;
		//*=�I�y���[�^�[
		const Vector3& operator *= (const Quaternion& quaternion);

		//----------------------------------------------------------------------------------
		//Vector3Ref

		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�������������
		//����1: Vector3Ref
		Vector3(const Vector3Ref& value);

		// =�I�y���[�^�[
		const Vector3 &operator =(const Vector3Ref & vec);

		//==�I�y���[�^�[
		bool operator ==(const Vector3Ref& vec) const;
		//!=�I�y���[�^�[
		bool operator !=(const Vector3Ref& vec) const;

		//���Z�I�y���[�^�[
		Vector3 operator +(const Vector3Ref& vec) const;
		//���Z�I�y���[�^�[
		Vector3 operator -(const Vector3Ref& vec) const;
		//��Z�I�y���[�^�[
		Vector3 operator *(const Vector3Ref& vec) const;
		//���Z�I�y���[�^�[
		Vector3 operator /(const Vector3Ref& vec) const;

		//+=�I�y���[�^�[
		const Vector3& operator +=(const Vector3Ref& vec);
		//-=�I�y���[�^�[
		const Vector3& operator -=(const Vector3Ref& vec);
		//*=�I�y���[�^�[
		const Vector3& operator *=(const Vector3Ref& vec);
		// /=�I�y���[�^�[
		const Vector3& operator /=(const Vector3Ref& vec);


		//�v�Z���C�u����


		//----------------------------------------------------------------------------------
		//[Orthogonal]<Calculation Library Function>
		//return: ���g�̃x�N�g���̒����x�N�g��
		//����1: �Ώۃx�N�g��
		inline static Vector3 Orthogonal(const Vector3& vec) noexcept;
		//----------------------------------------------------------------------------------
		//[TransformCoord]<Calculation Library Function>
		//Matrix��t�^�����l�ɕϊ�����
		//return: Matrix��t�^�����x�N�g��
		//����1: �Ώۃx�N�g��
		//����2: �Ώ�Matrix
		inline static Vector3 TransformCoord(const Vector3& vec, const DirectX::XMMATRIX& matrix) noexcept;
		//----------------------------------------------------------------------------------
		//[Rotate]<Calculation Library Function>
		//�x�N�g��������quaternion���g���ĉ�]������
		//return: ��]�����x�N�g��
		//����1: �Ώۃx�N�g��
		//����2: ��]�Ɏg��quaternion
		inline static Vector3 Rotate(const Vector3 & vec, const XmVector & quaternion) noexcept;
		//----------------------------------------------------------------------------------
		//[ProgectOnPlane]<Calculation Library Function>
		//vec�𕽖ʏ�x�N�g���Ɏˉe����
		//return: �ˉe�σx�N�g��
		//����1: �Ώۃx�N�g��
		//����2: ���ʃx�N�g���̖@��(���K���ς̂���)
		inline static Vector3 ProgectOnPlane(const Vector3& vec, const Vector3& normal) noexcept;
		//----------------------------------------------------------------------------------
		//[Dot]<Calculation Library Function>
		//���ς��s��
		//return: ���ς̌���
		//����1: �Ώۃx�N�g��1
		//����2: �Ώۃx�N�g��2
		inline static float Dot(const Vector3& vec1, const Vector3& vec2) noexcept;
		//----------------------------------------------------------------------------------
		//[Cross]<Calculation Library Function>
		//�O�ς��s��
		//return: �O�ς̌���
		//����1: �Ώۃx�N�g��1
		//����2: �Ώۃx�N�g��2
		inline static Vector3 Cross(const Vector3& vec1, const Vector3& vec2) noexcept;
		//----------------------------------------------------------------------------------
		//[Distance]<Calculation Library Function>
		//from����to�̋����𑪂�
		//return: from-to�̋���
		//����1: �Ώۃx�N�g��1
		//����2: �Ώۃx�N�g��2
		inline static constexpr float Distance(const Vector3& from, const Vector3& to) noexcept;
		//----------------------------------------------------------------------------------
		//[ExactDistance]<Calculation Library Function>
		//from����to�̋����𑪂�, Distance��萳�m�Ȓl�����v�Z���ׂ�����
		//return: from-to�̋���
		//����1: �Ώۃx�N�g��1
		//����2: �Ώۃx�N�g��2
		inline static float ExactDistance(const Vector3& from, const Vector3& to) noexcept;
		//----------------------------------------------------------------------------------
		//[Direction]<Calculation Library Function>
		//from����to�ւ̐��K���ϕ����x�N�g�����擾����
		//return: ���K���ϕ����x�N�g��
		//����1: �Ώۃx�N�g��1
		//����2: �Ώۃx�N�g��2
		inline static Vector3 Direction(const Vector3& from, const Vector3& to) noexcept;
		//----------------------------------------------------------------------------------
		//[Lerp3D]<Calculation Library Function>
		//from��to�̊Ԃ�t�Ő��`�⊮�������̂�ԋp����, �⊮�ɂ�3���֐��⊮���g��
		//return: from����to�ւ̐��`(3���֐�)���
		//����1: from
		//����2: to
		//����3: �⊮�̓x����(0 ~ 1)
		inline static constexpr Vector3 Lerp3D(const Vector3& from, const Vector3& to, float t) noexcept;
		//----------------------------------------------------------------------------------
		//[LerpDirect]<Calculation Library Function>
		//from��to�̊Ԃ�t�Ő��`�⊮�������̂�ԋp����, �⊮�ɂ͂����̐��`��Ԃ��g��
		//return: from����to�ւ̐��`���
		//����1: from
		//����2: to
		//����3: �⊮�̓x����(0 ~ 1)
		inline static constexpr Vector3 LerpDirect(const Vector3& from, const Vector3& to, float t) noexcept;
		//----------------------------------------------------------------------------------
		//[Slerp]<Calculation Library Function>
		//from��to�̊Ԃ�t�ŋ���ɕ⊮�������̂�ԋp����
		//return: from����to�ւ̋��ʐ��`���
		//����1: from
		//����2: to
		//����3: �⊮�̓x����(0 ~ 1)
		inline static Vector3 Slerp(const Vector3& from, const Vector3& to, float t) noexcept;
		//----------------------------------------------------------------------------------
		//[Max]<Calculation Library Function>
		//2�̃x�N�g���̊e�����̈�ԑ傫�Ȓl���g����Vector���쐬
		//return: �ő�x�N�g��
		//����1: �Ώۃx�N�g��1
		//����2: �Ώۃx�N�g��2
		inline static constexpr Vector3 Max(const Vector3& vec1, const Vector3& vec2) noexcept;
		//----------------------------------------------------------------------------------
		//[Min]<Calculation Library Function>
		//2�̃x�N�g���̊e�����̈�ԏ������l���g����Vector���쐬
		//return: �ŏ��x�N�g��
		//����1: �Ώۃx�N�g��1
		//����2: �Ώۃx�N�g��2
		inline static constexpr Vector3 Min(const Vector3& vec1, const Vector3& vec2) noexcept;
	};

	//----------------------------------------------------------------------------------
	//[wostream operator]
	//����1: stream
	//����2: this
	inline std::wostream& operator<<(std::wostream& stream, const Vector3& value)
	{
		const wchar* string = value.ToString();
		stream << string;
		return stream;
	}

	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//�S��0�ɐݒ肳���
	inline constexpr Vector3::Vector3() : DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f) {}
	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//�������������
	//����1: x
	//����2: y
	//����3: z
	inline constexpr Vector3::Vector3(float x, float y, float z) : DirectX::XMFLOAT3(x, y, z) {}
	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//�������������
	//����1: XMFLOAT3
	inline constexpr Vector3::Vector3(const DirectX::XMFLOAT3 & vec) : XMFLOAT3(vec) {}
	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//�������������
	//����1: PxVec3
	inline constexpr Vector3::Vector3(const physx::PxVec3 & vec) : XMFLOAT3(vec.x, vec.y, vec.z) {}
	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//�������������
	//����1: aiVector3D
	inline constexpr Vector3::Vector3(const aiVector3D & vec) : XMFLOAT3(vec.x, vec.y, vec.z) {}
	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//�������������
	//����1: XMVECTOR
	inline Vector3::Vector3(const XmVector & vec)
	{
		DirectX::XMStoreFloat3(this, vec);
	}
	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//�������������
	//����1: float*
	inline Vector3::Vector3(const float * array) : DirectX::XMFLOAT3(array) {}

	//----------------------------------------------------------------------------------
	//[CreateWithInt]
	//int��Vector3���쐬
	//����1:x
	//����2:y
	//����3:z
	inline constexpr Vector3 Vector3::CreateWithInt(int x, int y, int z)
	{
		return Vector3(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
	}


	//----------------------------------------------------------------------------------
	//[Absolute]
	//return: ��Βl
	inline Vector3 Vector3::Absolute() const noexcept
	{
		return Vector3(x < 0.0f ? -x : x, y < 0.0f ? -y : y, z < 0.0f ? -z : z);
	}
	//----------------------------------------------------------------------------------
	//[Magnitude]
	//return: ���g��Vector(0,0,0)�̋���
	inline constexpr float Vector3::Magnitude() const noexcept
	{
		float f = x * x + y * y + z * z;
		int i = 0x5f3759df - (*reinterpret_cast<int*>(&f) >> 1);
		float bitF = *reinterpret_cast<float*>(&i);

		bitF *= (1.5f - 0.5f * f * bitF * bitF);
		return f * bitF * (1.5f - 0.5f * f * bitF * bitF);
	}
	//----------------------------------------------------------------------------------
	//[ExactMagnitude]
	//return: ���g��Vector(0,0,0)�̋��� (���m�Ȓl�������ׂ�����)
	inline float Vector3::ExactMagnitude() const noexcept
	{
		return std::sqrtf(x * x + y * y + z * z);
	}
	//----------------------------------------------------------------------------------
	//[ClampMagnitude]
	//Length��maxLength�ɐ�������
	//return: ��������vector
	//����1: ����Length
	inline Vector3 Vector3::ClampMagnitude(const float maxLength) const noexcept
	{
		return Vector3(DirectX::XMVector3ClampLength(DirectX::XMLoadFloat3(this), 0.0f, maxLength));
	}
	//----------------------------------------------------------------------------------
	//[Normalize]
	//return: ���g�𐳋K�������x�N�g��
	inline Vector3 Vector3::Normalized() const noexcept
	{
		float divLength = 1.0f / ExactMagnitude();
		return Vector3(x * divLength, y * divLength, z * divLength);
	}
	//----------------------------------------------------------------------------------
	//[Orthogonal]
	//return: ���g�̃x�N�g���̒����x�N�g��
	inline Vector3 Vector3::Orthogonal() const noexcept { return Vector3(DirectX::XMVector3Orthogonal(DirectX::XMLoadFloat3(this))); }
	//----------------------------------------------------------------------------------
	//[TransformCoord]
	//Matrix��t�^�����l�ɕϊ�����
	//return: Matrix��t�^�����x�N�g��
	//����1: �Ώ�Matrix
	inline Vector3 Vector3::TransformCoord(const DirectX::XMMATRIX & matrix) const noexcept
	{
		return DirectX::XMVector3TransformCoord(DirectX::XMLoadFloat3(this), matrix);
	}
	//----------------------------------------------------------------------------------
	//[Rotate]
	//return: ���g�̃x�N�g���̉�]�x�N�g��
	inline Vector3 Vector3::Rotate(const XmVector&  quaternion) const noexcept
	{
		return Vector3(DirectX::XMVector3Rotate(DirectX::XMLoadFloat3(this), quaternion));
	}
	//----------------------------------------------------------------------------------
	//[ProgectOnPlane]
	//vec�𕽖ʏ�x�N�g���Ɏˉe����
	//return: �ˉe�σx�N�g��
	//����1: ���ʃx�N�g���̖@��(���K���ς̂���)
	inline Vector3 Vector3::ProgectOnPlane(const Vector3 & normal) const noexcept
	{
		return std::move(*this - (*this - normal * Vector3::Dot(this->ToXmVector(), normal)));
	}

	//XMVECTOR�I�y���[�^�[
	inline Vector3::operator XmVector() const noexcept { return DirectX::XMLoadFloat3(this); }
	//float*�I�y���[�^�[
	inline Vector3::operator float*() { return (float*)this; }
	//const float*�I�y���[�^�[
	inline Vector3::operator const float*() const { return (float*)this; }

	//=�I�y���[�^�[
	inline XmVector Vector3::operator=(const XmVector & vec) noexcept
	{
		DirectX::XMStoreFloat3(this, vec);
		return vec;
	}

	//+�I�y���[�^�[  
	inline Vector3 Vector3::operator+() const { return Vector3(+this->x, +this->y, +this->z); }
	//-�I�y���[�^�[  
	inline Vector3 Vector3::operator-() const { return Vector3(-this->x, -this->y, -this->z); }
	//==�I�y���[�^�[
	inline bool Vector3::operator==(const Vector3& vec) const
	{
		return (fabsf(x - vec.x) <= MathAF::cEpsilon
			&& fabsf(y - vec.y) <= MathAF::cEpsilon
			&& fabsf(z - vec.z) <= MathAF::cEpsilon);
	}
	//!=�I�y���[�^�[
	inline bool Vector3::operator!=(const Vector3& vec) const
	{
		return (fabsf(x - vec.x) > MathAF::cEpsilon
			&& fabsf(y - vec.y) > MathAF::cEpsilon
			&& fabsf(z - vec.z) > MathAF::cEpsilon);
	}

	//���Z�I�y���[�^�[
	inline Vector3 Vector3::operator+(const Vector3& vec) const { return Vector3(x + vec.x, y + vec.y, z + vec.z); }
	//���Z�I�y���[�^�[
	inline Vector3 Vector3::operator-(const Vector3& vec) const { return Vector3(x - vec.x, y - vec.y, z - vec.z); }
	//��Z�I�y���[�^�[
	inline Vector3 Vector3::operator*(const Vector3& vec) const { return Vector3(x * vec.x, y * vec.y, z * vec.z); }
	//��Z�I�y���[�^�[(float)
	inline Vector3 Vector3::operator*(float value) const { return Vector3(x * value, y * value, z * value); }
	//���Z�I�y���[�^�[
	inline Vector3 Vector3::operator/(const Vector3& vec) const { return Vector3(x / vec.x, y / vec.y, z / vec.z); }
	//���Z�I�y���[�^�[(float)
	inline Vector3 Vector3::operator/(float value) const { return Vector3(x / value, y / value, z / value); }

	//+=�I�y���[�^�[
	inline const Vector3 & Vector3::operator+=(const Vector3& vec)
	{
		this->x += vec.x;		this->y += vec.y;
		this->z += vec.z;
		return *this;
	}
	//-=�I�y���[�^�[
	inline const Vector3 & Vector3::operator-=(const Vector3& vec)
	{
		this->x -= vec.x;		this->y -= vec.y;
		this->z -= vec.z;
		return *this;
	}
	//*=�I�y���[�^�[
	inline const Vector3 & Vector3::operator*=(const Vector3& vec)
	{
		this->x *= vec.x;		this->y *= vec.y;
		this->z *= vec.z;
		return *this;
	}
	//*=�I�y���[�^�[(float)
	inline const Vector3 & Vector3::operator*=(float value)
	{
		this->x *= value;		this->y *= value;
		this->z *= value;
		return *this;
	}
	// /=�I�y���[�^�[
	inline const Vector3 & Vector3::operator/=(const Vector3& vec)
	{
		this->x /= vec.x;		this->y /= vec.y;
		this->z /= vec.z;
		return *this;
	}
	// /=�I�y���[�^�[(float)
	inline const Vector3 & Vector3::operator/=(float value)
	{
		this->x /= value;		this->y /= value;
		this->z /= value;
		return *this;
	}



	//----------------------------------------------------------------------------------
	//[Orthogonal]<Calculation Library Function>
	//return: ���g�̃x�N�g���̒����x�N�g��
	//����1: �Ώۃx�N�g��
	inline Vector3 Vector3::Orthogonal(const Vector3& vec) noexcept { return std::move(Vector3(DirectX::XMVector3Orthogonal(DirectX::XMLoadFloat3(&vec)))); }
	//----------------------------------------------------------------------------------
	//[TransformCoord]<Calculation Library Function>
	//Matrix��t�^�����l�ɕϊ�����
	//return: Matrix��t�^�����x�N�g��
	//����1: �Ώۃx�N�g��
	//����2: �Ώ�Matrix
	inline Vector3 Vector3::TransformCoord(const Vector3& vec, const DirectX::XMMATRIX& matrix) noexcept
	{
		return std::move(DirectX::XMVector3TransformCoord(DirectX::XMLoadFloat3(&vec), matrix));
	}
	//----------------------------------------------------------------------------------
	//[Rotate]<Calculation Library Function>
	//�x�N�g��������quaternion���g���ĉ�]������
	//return: ��]�����x�N�g��
	//����1: �Ώۃx�N�g��
	//����2: ��]�Ɏg��quaternion
	inline Vector3 Vector3::Rotate(const Vector3 & vec, const XmVector & quaternion) noexcept 
	{
		return std::move(DirectX::XMVector3Rotate(DirectX::XMLoadFloat3(&vec), quaternion));
	}
	//----------------------------------------------------------------------------------
	//[ProgectOnPlane]<Calculation Library Function>
	//vec�𕽖ʏ�x�N�g���Ɏˉe����
	//return: �ˉe�σx�N�g��
	//����1: �Ώۃx�N�g��
	//����2: ���ʃx�N�g���̖@��(���K���ς̂���)
	inline Vector3 Vector3::ProgectOnPlane(const Vector3& vec, const Vector3& normal) noexcept 
	{
		return std::move(vec - (vec - normal * Vector3::Dot(vec, normal)));
	}
	//----------------------------------------------------------------------------------
	//[Dot]<Calculation Library Function>
	//���ς��s��
	//return: ���ς̌���
	//����1: �Ώۃx�N�g��1
	//����2: �Ώۃx�N�g��2
	inline float Vector3::Dot(const Vector3& vec1, const Vector3& vec2) noexcept
	{
		return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
	}
	//----------------------------------------------------------------------------------
	//[Cross]<Calculation Library Function>
	//�O�ς��s��
	//return: �O�ς̌���
	//����1: �Ώۃx�N�g��1
	//����2: �Ώۃx�N�g��2
	inline Vector3 Vector3::Cross(const Vector3& vec1, const Vector3& vec2) noexcept
	{ 
		// (y1 * z2 - z1 * y2, z1 * x2 - x1 * z2, x1 * y2 - y1 * x2) = (x3, y3, z3) =  v3
		return std::move(Vector3(vec1.y * vec2.z - vec1.z * vec2.y,
			vec1.z * vec2.x - vec1.x * vec2.z, vec1.x * vec2.y - vec1.y * vec2.x));
	}
	//----------------------------------------------------------------------------------
	//[Distance]<Calculation Library Function>
	//from����to�̋����𑪂�
	//return: from-to�̋���
	//����1: �Ώۃx�N�g��1
	//����2: �Ώۃx�N�g��2
	inline constexpr float Vector3::Distance(const Vector3& from, const Vector3& to) noexcept
	{
		float f = (from.x - to.x) * (from.x - to.x)
			+ (from.y - to.y) * (from.y - to.y)
			+ (from.z - to.z) * (from.z - to.z);
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
	inline float Vector3::ExactDistance(const Vector3& from, const Vector3& to) noexcept
	{
		return std::sqrtf((to.x - from.x) * (to.x - from.x) + (to.y - from.y) * (to.y - from.y)
		+ (to.z - from.z) * (to.z - from.z));
	}
	//----------------------------------------------------------------------------------
	//[Direction]<Calculation Library Function>
	//from����to�ւ̐��K���ϕ����x�N�g�����擾����
	//return: ���K���ϕ����x�N�g��
	//����1: �Ώۃx�N�g��1
	//����2: �Ώۃx�N�g��2
	inline Vector3 Vector3::Direction(const Vector3& from, const Vector3& to) noexcept { return std::move((to - from).Normalized()); }
	//----------------------------------------------------------------------------------
	//[Lerp3D]<Calculation Library Function>
	//from��to�̊Ԃ�t�Ő��`�⊮�������̂�ԋp����, �⊮�ɂ�3���֐��⊮���g��
	//return: from����to�ւ̐��`(3���֐�)���
	//����1: from
	//����2: to
	//����3: �⊮�̓x����(0 ~ 1)
	inline constexpr Vector3 Vector3::Lerp3D(const Vector3& from, const Vector3& to, float t) noexcept
	{
		if (t > 1.0f) t = 1.0f;	else if (t < 0.0f) t = 0.0f;

		//�O���֐��⊮
		float rate = t * t * (3.0f - 2.0f * t);
		float rateSub = (1.0f - rate);
		//���`���
		return std::move(Vector3(from.x * rateSub + to.x * rate,
			from.y * rateSub + to.y * rate, from.z * rateSub + to.z * rate));
	}
	//----------------------------------------------------------------------------------
	//[LerpDirect]<Calculation Library Function>
	//from��to�̊Ԃ�t�Ő��`�⊮�������̂�ԋp����, �⊮�ɂ͂����̐��`��Ԃ��g��
	//return: from����to�ւ̐��`���
	//����1: from
	//����2: to
	//����3: �⊮�̓x����(0 ~ 1)
	inline constexpr Vector3 Vector3::LerpDirect(const Vector3& from, const Vector3& to, float t) noexcept
	{
		if (t > 1.0f) t = 1.0f;	else if (t < 0.0f) t = 0.0f;

		//���`���
		float tSub = (1.0f - t);
		return std::move(Vector3(from.x * tSub + to.x * t, from.y * tSub + to.y * t, from.z * tSub + to.z * t));
	}
	//----------------------------------------------------------------------------------
	//[Slerp]<Calculation Library Function>
	//from��to�̊Ԃ�t�ŋ���ɕ⊮�������̂�ԋp����
	//return: from����to�ւ̋��ʐ��`���
	//����1: from
	//����2: to
	//����3: �⊮�̓x����(0 ~ 1)
	inline Vector3 Vector3::Slerp(const Vector3& from, const Vector3& to, float t) noexcept
	{
		if (t > 1.0f) t = 1.0f;	else if (t < 0.0f) t = 0.0f;
		//Normal
		Vector3 startNormal = from.Normalized(), EndNormal = to.Normalized();
		//sin��
		float angle = std::acos(Dot(startNormal, EndNormal));
		//sin(angle * ~~) = �⊮�W��, �ꉞ���K�������ʐ��`��Ԃ�
		return std::move(((startNormal * std::sin(angle * (1 - t)) + EndNormal * std::sin(angle * t)) / std::sin(angle)).Normalized());
	}
	//----------------------------------------------------------------------------------
	//[Max]<Calculation Library Function>
	//2�̃x�N�g���̊e�����̈�ԑ傫�Ȓl���g����Vector���쐬
	//return: �ő�x�N�g��
	//����1: �Ώۃx�N�g��1
	//����2: �Ώۃx�N�g��2
	inline constexpr Vector3 Vector3::Max(const Vector3& vec1, const Vector3& vec2) noexcept
	{
		Vector3 ret;
		if (vec1.x >= vec2.x) ret.x = vec1.x; else ret.x = vec2.x;
		if (vec1.y >= vec2.y) ret.y = vec1.y; else ret.y = vec2.y;
		if (vec1.z >= vec2.z) ret.z = vec1.z; else ret.z = vec2.z;
		return std::move(ret);
	}
	//----------------------------------------------------------------------------------
	//[Min]<Calculation Library Function>
	//2�̃x�N�g���̊e�����̈�ԏ������l���g����Vector���쐬
	//return: �ŏ��x�N�g��
	//����1: �Ώۃx�N�g��1
	//����2: �Ώۃx�N�g��2
	inline constexpr Vector3 Vector3::Min(const Vector3& vec1, const Vector3& vec2) noexcept
	{
		Vector3 ret;
		if (vec1.x <= vec2.x) ret.x = vec1.x; else ret.x = vec2.x;
		if (vec1.y <= vec2.y) ret.y = vec1.y; else ret.y = vec2.y;
		if (vec1.z <= vec2.z) ret.z = vec1.z; else ret.z = vec2.z;
		return std::move(ret);
	}
}
#endif // !SGFRAMEWORK_HEADER_VECTOR_VECTOR3_HPP_