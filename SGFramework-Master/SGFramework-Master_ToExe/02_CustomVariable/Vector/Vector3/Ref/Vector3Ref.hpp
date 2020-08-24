/*----------------------------------------------------------------------------------
3�̒l��������Vector3Ref (�Q��)�\����
�ł����p����\���̃V���[�Y��1�Ȃ̂ŕʎ���Vector���֌W�Ȃ����̂͑S��inline����
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_VECTOR_VECTOR3_REF_HPP_
#define SGFRAMEWORK_HEADER_VECTOR_VECTOR3_REF_HPP_
#include <DirectXMath.h>
#include <iostream>
#include "../../../ConstAndUsing/ConstAndUsing.hpp"
#include "../../../Property/Property.hpp"
#include "../../../String/String.hpp"
#include "../Vector3.hpp"

//Framework namespace
namespace SGFramework
{
	//�v���g�^�C�v�錾
	struct Vector3;
	struct Vector4;
	struct Vector2;
	struct Vector3Int;

	//3�̒l��������Vector3Ref (�Q��)�\����
	struct Vector3Ref final
	{
	public:
		//----------------------------------------------------------------------------------
		//inlne����

		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�������������
		//����1:x
		//����2:y
		inline Vector3Ref(float& x, float& y, float& z);
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�������������
		//����1: XMFLOAT2
		inline Vector3Ref(DirectX::XMFLOAT3& value);

		//----------------------------------------------------------------------------------
		//[ToXmVector]
		//return: XmVector
		inline XmVector ToXmVector() const noexcept { return DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(x, y, z)); }
		//----------------------------------------------------------------------------------
		//[ToPhysX]
		//return: physx::PxVec3
		inline physx::PxVec3 ToPhysX() const noexcept { return physx::PxVec3(x, y, z); }
		//----------------------------------------------------------------------------------
		//[ToAssimp]
		//return: aiVector3D
		inline aiVector3D ToAssimp() const noexcept { return aiVector3D(x, y, z); }

		//default
		Vector3Ref(const Vector3Ref&) = default;
		Vector3Ref& operator=(const Vector3Ref&) = default;
		Vector3Ref(Vector3Ref&&) = default;
		Vector3Ref& operator=(Vector3Ref&&) = default;

		//----------------------------------------------------------------------------------
		//�ϐ�
		float& x;		//X
		float& y;		//Y
		float& z;		//Z

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

		//+�I�y���[�^�[  
		inline Vector3 operator +() const;
		//-�I�y���[�^�[  
		inline Vector3 operator -() const;
		//==�I�y���[�^�[
		inline bool operator ==(const Vector3Ref& vec) const;
		//!=�I�y���[�^�[
		inline bool operator !=(const Vector3Ref& vec) const;

		//���Z�I�y���[�^�[
		inline Vector3 operator +(const Vector3Ref& vec) const;
		//���Z�I�y���[�^�[
		inline Vector3 operator -(const Vector3Ref& vec) const;
		//��Z�I�y���[�^�[
		inline Vector3 operator *(const Vector3Ref& vec) const;
		//��Z�I�y���[�^�[(float)
		inline Vector3 operator *(float value) const;
		//���Z�I�y���[�^�[
		inline Vector3 operator /(const Vector3Ref& vec) const;
		//���Z�I�y���[�^�[(float)
		inline Vector3 operator /(float value) const;

		//+=�I�y���[�^�[
		inline const Vector3Ref &operator +=(const Vector3Ref& vec);
		//-=�I�y���[�^�[
		inline const Vector3Ref &operator -=(const Vector3Ref& vec);
		//*=�I�y���[�^�[
		inline const Vector3Ref &operator *=(const Vector3Ref& vec);
		//*=�I�y���[�^�[(float)
		inline const Vector3Ref &operator *=(float value);
		// /=�I�y���[�^�[
		inline const Vector3Ref &operator /=(const Vector3Ref& vec);
		// /=�I�y���[�^�[(float)
		inline const Vector3Ref &operator /=(float value);

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
		//Vector3

		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�������������
		//����1: Vector3
		inline Vector3Ref(Vector3& value);
		//----------------------------------------------------------------------------------
		//[ToVector3]
		//return: Vector3
		inline Vector3 ToVector3() const { return Vector3(*this); }

		//=�I�y���[�^�[
		inline const Vector3Ref &operator =(const Vector3& vec);
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
		//���Z�I�y���[�^�[
		inline Vector3 operator /(const Vector3& vec) const;

		//+=�I�y���[�^�[
		inline const Vector3Ref &operator +=(const Vector3& vec);
		//-=�I�y���[�^�[
		inline const Vector3Ref &operator -=(const Vector3& vec);
		//*=�I�y���[�^�[
		inline const Vector3Ref &operator *=(const Vector3& vec);
		// /=�I�y���[�^�[
		inline const Vector3Ref &operator /=(const Vector3& vec);



		//=�I�y���[�^�[
		inline const Vector3Ref &operator =(const DirectX::XMFLOAT3& vec);
		//==�I�y���[�^�[
		inline bool operator ==(const DirectX::XMFLOAT3& vec) const;
		//!=�I�y���[�^�[
		inline bool operator !=(const DirectX::XMFLOAT3& vec) const;

		//���Z�I�y���[�^�[
		inline Vector3 operator +(const DirectX::XMFLOAT3& vec) const;
		//���Z�I�y���[�^�[
		inline Vector3 operator -(const DirectX::XMFLOAT3& vec) const;
		//��Z�I�y���[�^�[
		inline Vector3 operator *(const DirectX::XMFLOAT3& vec) const;
		//���Z�I�y���[�^�[
		inline Vector3 operator /(const DirectX::XMFLOAT3& vec) const;

		//+=�I�y���[�^�[
		inline const Vector3Ref &operator +=(const DirectX::XMFLOAT3& vec);
		//-=�I�y���[�^�[
		inline const Vector3Ref &operator -=(const DirectX::XMFLOAT3& vec);
		//*=�I�y���[�^�[
		inline const Vector3Ref &operator *=(const DirectX::XMFLOAT3& vec);
		// /=�I�y���[�^�[
		inline const Vector3Ref &operator /=(const DirectX::XMFLOAT3& vec);


		//----------------------------------------------------------------------------------
		//cpp����

		//Vector2�I�y���[�^�[
		operator Vector2() const;
		//Vector4�I�y���[�^�[
		operator Vector4() const;
		//Vector3Int�I�y���[�^�[
		operator Vector3Int() const;

		//=�I�y���[�^�[
		const Vector3Ref& operator = (const Vector2& vec);
		//=�I�y���[�^�[
		const Vector3Ref& operator = (const Vector4& vec);
		//=�I�y���[�^�[
		const Vector3Ref& operator = (const Vector3Int& vec);
	};


	//----------------------------------------------------------------------------------
	//[wostream operator]
	//����1: stream
	//����2: this
	inline std::wostream& operator<<(std::wostream& stream, const Vector3Ref& value)
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
	inline Vector3Ref::Vector3Ref(float& x, float& y, float& z) : x(x), y(y), z(z) {};
	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//�������������
	//����1: XMFLOAT2
	inline Vector3Ref::Vector3Ref(DirectX::XMFLOAT3 & vec) : x(vec.x), y(vec.y), z(vec.z) { }

	//----------------------------------------------------------------------------------
	//[Absolute]
	//return: ��Βl
	inline Vector3 Vector3Ref::Absolute() const noexcept
	{
		return Vector3(x < 0.0f ? -x : x, y < 0.0f ? -y : y, z < 0.0f ? -z : z);
	}
	//----------------------------------------------------------------------------------
	//[Magnitude]
	//return: ���g��Vector(0,0,0)�̋���
	inline constexpr float Vector3Ref::Magnitude() const noexcept
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
	inline float Vector3Ref::ExactMagnitude() const noexcept
	{
		return std::sqrtf(x * x + y * y + z * z);
	}
	//----------------------------------------------------------------------------------
	//[ClampMagnitude]
	//Length��maxLength�ɐ�������
	//return: ��������vector
	//����1: ����Length
	inline Vector3 Vector3Ref::ClampMagnitude(const float maxLength) const noexcept
	{
		return std::move(DirectX::XMVector3ClampLength(this->ToXmVector(), 0.0f, maxLength));
	}
	//----------------------------------------------------------------------------------
	//[Normalize]
	//return: ���g�𐳋K�������x�N�g��
	inline Vector3 Vector3Ref::Normalized() const noexcept
	{
		float divLength = 1.0f / ExactMagnitude();
		return Vector3(x * divLength, y * divLength, z * divLength);
	}
	//----------------------------------------------------------------------------------
	//[Orthogonal]
	//return: ���g�̃x�N�g���̒����x�N�g��
	inline Vector3 Vector3Ref::Orthogonal() const noexcept { return std::move(DirectX::XMVector3Orthogonal(this->ToXmVector())); }
	//----------------------------------------------------------------------------------
	//[TransformCoord]
	//Matrix��t�^�����l�ɕϊ�����
	//return: Matrix��t�^�����x�N�g��
	//����1: �Ώ�Matrix
	inline Vector3 Vector3Ref::TransformCoord(const DirectX::XMMATRIX & matrix) const noexcept
	{
		return DirectX::XMVector3TransformCoord(this->ToXmVector(), matrix);
	}
	//----------------------------------------------------------------------------------
	//[Rotate]
	//return: ���g�̃x�N�g���̉�]�x�N�g��
	inline Vector3 Vector3Ref::Rotate(const XmVector&  quaternion) const noexcept
	{
		return std::move(DirectX::XMVector3Rotate(this->ToXmVector(), quaternion));
	}
	//----------------------------------------------------------------------------------
	//[ProgectOnPlane]
	//vec�𕽖ʏ�x�N�g���Ɏˉe����
	//return: �ˉe�σx�N�g��
	//����1: ���ʃx�N�g���̖@��(���K���ς̂���)
	inline Vector3 Vector3Ref::ProgectOnPlane(const Vector3 & normal) const noexcept
	{
		return std::move(*this - (*this - normal * Vector3::Dot(*this, normal)));
	}

	//XMVECTOR�I�y���[�^�[
	inline Vector3Ref::operator XmVector() const noexcept { return DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(x, y, z)); }

	//+�I�y���[�^�[  
	inline Vector3 Vector3Ref::operator+() const { return Vector3(+this->x, +this->y, +this->z); }
	//-�I�y���[�^�[  
	inline Vector3 Vector3Ref::operator-() const { return Vector3(-this->x, -this->y, -this->z); }
	//==�I�y���[�^�[
	inline bool Vector3Ref::operator==(const Vector3Ref& vec) const
	{
		return (fabsf(x - vec.x) <= MathAF::cEpsilon
			&& fabsf(y - vec.y) <= MathAF::cEpsilon
			&& fabsf(z - vec.z) <= MathAF::cEpsilon);
	}
	//!=�I�y���[�^�[
	inline bool Vector3Ref::operator!=(const Vector3Ref& vec) const
	{
		return (fabsf(x - vec.x) <= MathAF::cEpsilon
			&& fabsf(y - vec.y) <= MathAF::cEpsilon
			&& fabsf(z - vec.z) <= MathAF::cEpsilon);
	}

	//���Z�I�y���[�^�[
	inline Vector3 Vector3Ref::operator+(const Vector3Ref& vec) const { return Vector3(x + vec.x, y + vec.y, z + vec.z); }
	//���Z�I�y���[�^�[
	inline Vector3 Vector3Ref::operator-(const Vector3Ref& vec) const { return Vector3(x - vec.x, y - vec.y, z - vec.z); }
	//��Z�I�y���[�^�[
	inline Vector3 Vector3Ref::operator*(const Vector3Ref& vec) const { return Vector3(x * vec.x, y * vec.y, z * vec.z); }
	//��Z�I�y���[�^�[(float)
	inline Vector3 Vector3Ref::operator*(const float value) const { return Vector3(x * value, y * value, z * value); }
	//���Z�I�y���[�^�[
	inline Vector3 Vector3Ref::operator/(const Vector3Ref& vec) const { return Vector3(x / vec.x, y / vec.y, z / vec.z); }
	//���Z�I�y���[�^�[(float)
	inline Vector3 Vector3Ref::operator/(const float value) const { return Vector3(x / value, y / value, z / value); }

	//+=�I�y���[�^�[
	inline const Vector3Ref & Vector3Ref::operator+=(const Vector3Ref& vec)
	{
		this->x += vec.x;		this->y += vec.y;
		this->z += vec.z;
		return *this;
	}
	//-=�I�y���[�^�[
	inline const Vector3Ref & Vector3Ref::operator-=(const Vector3Ref& vec)
	{
		this->x -= vec.x;		this->y -= vec.y;
		this->z -= vec.z;
		return *this;
	}
	//*=�I�y���[�^�[
	inline const Vector3Ref & Vector3Ref::operator*=(const Vector3Ref& vec)
	{
		this->x *= vec.x;		this->y *= vec.y;
		this->z *= vec.z;
		return *this;
	}
	//*=�I�y���[�^�[(float)
	inline const Vector3Ref & Vector3Ref::operator*=(const float value)
	{
		this->x *= value;		this->y *= value;
		this->z *= value;
		return *this;
	}
	// /=�I�y���[�^�[
	inline const Vector3Ref & Vector3Ref::operator/=(const Vector3Ref& vec)
	{
		this->x /= vec.x;		this->y /= vec.y;
		this->z /= vec.z;
		return *this;
	}
	// /=�I�y���[�^�[(float)
	inline const Vector3Ref & Vector3Ref::operator/=(const float value)
	{
		this->x /= value;		this->y /= value;
		this->z /= value;
		return *this;
	}

	//----------------------------------------------------------------------------------
	//Vector3

	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//�������������
	//����1: Vector3
	Vector3Ref::Vector3Ref(Vector3 & value) : x(value.x), y(value.y), z(value.z) {}

	//=�I�y���[�^�[
	inline const Vector3Ref & Vector3Ref::operator=(const Vector3 & vec)
	{
		x = vec.x; y = vec.y;
		z = vec.z;
		return *this;
	}

	//==�I�y���[�^�[
	inline bool Vector3Ref::operator==(const Vector3 & vec) const
	{
		return (fabsf(x - vec.x) <= MathAF::cEpsilon
			&& fabsf(y - vec.y) <= MathAF::cEpsilon
			&& fabsf(z - vec.z) <= MathAF::cEpsilon);
	}
	//!=�I�y���[�^�[
	inline bool Vector3Ref::operator!=(const Vector3 & vec) const
	{
		return (fabsf(x - vec.x) <= MathAF::cEpsilon
			&& fabsf(y - vec.y) <= MathAF::cEpsilon
			&& fabsf(z - vec.z) <= MathAF::cEpsilon);
	}

	//���Z�I�y���[�^�[
	inline Vector3 Vector3Ref::operator+(const Vector3 & vec) const { return Vector3(x + vec.x, y + vec.y, z + vec.z); }
	//���Z�I�y���[�^�[
	inline Vector3 Vector3Ref::operator-(const Vector3 & vec) const { return Vector3(x - vec.x, y - vec.y, z - vec.z); }
	//��Z�I�y���[�^�[
	inline Vector3 Vector3Ref::operator*(const Vector3 & vec) const { return Vector3(x * vec.x, y * vec.y, z * vec.z); }
	//���Z�I�y���[�^�[
	inline Vector3 Vector3Ref::operator/(const Vector3 & vec) const { return Vector3(x / vec.x, y / vec.y, z / vec.z); }

	//+=�I�y���[�^�[
	inline const Vector3Ref & Vector3Ref::operator+=(const Vector3 & vec)
	{
		this->x += vec.x;		this->y += vec.y;
		this->z += vec.z;
		return *this;
	}
	//-=�I�y���[�^�[
	inline const Vector3Ref & Vector3Ref::operator-=(const Vector3 & vec)
	{
		this->x -= vec.x;		this->y -= vec.y;
		this->z -= vec.z;
		return *this;
	}
	//*=�I�y���[�^�[
	inline const Vector3Ref & Vector3Ref::operator*=(const Vector3 & vec)
	{
		this->x *= vec.x;		this->y *= vec.y;
		this->z *= vec.z;
		return *this;
	}
	// /=�I�y���[�^�[
	inline const Vector3Ref & Vector3Ref::operator/=(const Vector3 & vec)
	{
		this->x /= vec.x;		this->y /= vec.y;
		this->z /= vec.z;
		return *this;
	}


	//=�I�y���[�^�[
	inline const Vector3Ref & Vector3Ref::operator=(const DirectX::XMFLOAT3 & vec)
	{
		x = vec.x; y = vec.y;
		z = vec.z;
		return *this;
	}

	//==�I�y���[�^�[
	inline bool Vector3Ref::operator==(const DirectX::XMFLOAT3 & vec) const
	{
		return (fabsf(x - vec.x) <= MathAF::cEpsilon
			&& fabsf(y - vec.y) <= MathAF::cEpsilon
			&& fabsf(z - vec.z) <= MathAF::cEpsilon);
	}
	//!=�I�y���[�^�[
	inline bool Vector3Ref::operator!=(const DirectX::XMFLOAT3 & vec) const
	{
		return (fabsf(x - vec.x) <= MathAF::cEpsilon
			&& fabsf(y - vec.y) <= MathAF::cEpsilon
			&& fabsf(z - vec.z) <= MathAF::cEpsilon);
	}

	//���Z�I�y���[�^�[
	inline Vector3 Vector3Ref::operator+(const DirectX::XMFLOAT3 & vec) const { return Vector3(x + vec.x, y + vec.y, z + vec.z); }
	//���Z�I�y���[�^�[
	inline Vector3 Vector3Ref::operator-(const DirectX::XMFLOAT3 & vec) const { return Vector3(x - vec.x, y - vec.y, z - vec.z); }
	//��Z�I�y���[�^�[
	inline Vector3 Vector3Ref::operator*(const DirectX::XMFLOAT3 & vec) const { return Vector3(x * vec.x, y * vec.y, z * vec.z); }
	//���Z�I�y���[�^�[
	inline Vector3 Vector3Ref::operator/(const DirectX::XMFLOAT3 & vec) const { return Vector3(x / vec.x, y / vec.y, z / vec.z); }

	//+=�I�y���[�^�[
	inline const Vector3Ref & Vector3Ref::operator+=(const DirectX::XMFLOAT3 & vec)
	{
		this->x += vec.x;		this->y += vec.y;
		this->z += vec.z;
		return *this;
	}
	//-=�I�y���[�^�[
	inline const Vector3Ref & Vector3Ref::operator-=(const DirectX::XMFLOAT3 & vec)
	{
		this->x -= vec.x;		this->y -= vec.y;
		this->z -= vec.z;
		return *this;
	}
	//*=�I�y���[�^�[
	inline const Vector3Ref & Vector3Ref::operator*=(const DirectX::XMFLOAT3 & vec)
	{
		this->x *= vec.x;		this->y *= vec.y;
		this->z *= vec.z;
		return *this;
	}
	// /=�I�y���[�^�[
	inline const Vector3Ref & Vector3Ref::operator/=(const DirectX::XMFLOAT3 & vec)
	{
		this->x /= vec.x;		this->y /= vec.y;
		this->z /= vec.z;
		return *this;
	}
}
#endif // !SGFRAMEWORK_HEADER_VECTOR_VECTOR3_REF_HPP_
