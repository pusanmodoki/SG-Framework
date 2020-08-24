/*----------------------------------------------------------------------------------
4�̒l��������Vector4 �\����
�ł����p����\���̃V���[�Y��1�Ȃ̂ŕʎ���Vector���֌W�Ȃ����̂͑S��inline����
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_VECTOR_VECTOR4_HPP_
#define SGFRAMEWORK_HEADER_VECTOR_VECTOR4_HPP_
#include <DirectXMath.h>
#include <math.h>
#include <iostream>
#include <PxPhysicsAPI.h>
#include "../../ConstAndUsing/ConstAndUsing.hpp"
#include "../../Property/Property.hpp"
#include "../../String/String.hpp"

//Framework namespace
namespace SGFramework
{
	//�v���g�^�C�v�錾
	struct Vector2;
	struct Vector3;
	struct Vector4Int;
	struct Vector4Ref;
	
	//4�̒l��������Vector4 �\����
	struct Vector4 final : public DirectX::XMFLOAT4
	{
	public:
		//----------------------------------------------------------------------------------
		//inlne����

		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�S��0�ɐݒ肳���
		inline constexpr Vector4();
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�������������
		//����1: x
		//����2: y
		//����3: z
		//����4: w
		inline constexpr Vector4(float x, float y, float z, float w);
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�������������
		//����1: XMFLOAT4
		inline constexpr Vector4(const DirectX::XMFLOAT4& vec);
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�������������
		//����1: PxVec4
		inline constexpr Vector4(const physx::PxVec4& vec);
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�������������
		//����1: XMVECTOR
		inline Vector4(const XmVector& vec);
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�������������
		//����1: float*
		inline Vector4(const float* array);

		//----------------------------------------------------------------------------------
		//[ToXmVector]
		//return: XmVector
		inline XmVector ToXmVector() const noexcept { return DirectX::XMLoadFloat4(this); }
		//----------------------------------------------------------------------------------
		//[ToRotationMatrix]
		//return: this��Quaternion�Ƃ�����]�s��
		inline XmMatrix ToRotationMatrix() const noexcept { return DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(this)); }
		//----------------------------------------------------------------------------------
		//[ToRotationMatrix]
		//����1: Apply
		inline XmMatrix ToRotationMatrix(XmMatrix& apply) const noexcept { apply = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(this)); }
		//----------------------------------------------------------------------------------
		//[ToPhysX]
		//return: physx::PxVec4
		inline physx::PxVec4 ToPhysX() const noexcept { return physx::PxVec4(x, y, z, w); }

		//default
		Vector4(const Vector4&) = default;
		Vector4& operator=(const Vector4&) = default;
		Vector4(Vector4&&) = default;
		Vector4& operator=(Vector4&&) = default;

		//----------------------------------------------------------------------------------
		//[CreateWithInt]
		//int��Vector4���쐬
		//����1:x
		//����2:y
		//����3:z
		//����4:w
		inline static constexpr Vector4 CreateWithInt(int x, int y, int z, int w);

		//----------------------------------------------------------------------------------
		//[Absolute]
		//return: ��Βl
		inline constexpr Vector4 Absolute() const noexcept;
		//----------------------------------------------------------------------------------
		//[Magnitude]
		//return: ���g��Vector(0, 0, 0, 0)�̋���
		inline constexpr float Magnitude() const noexcept;
		//----------------------------------------------------------------------------------
		//[ExactMagnitude]
		//return: ���g��Vector(0,0,0,0)�̋��� (���m�Ȓl�������ׂ�����)
		inline float ExactMagnitude() const noexcept;	
		//----------------------------------------------------------------------------------
		//[Normalize]
		//return: ���g�𐳋K�������x�N�g��
		inline Vector4 Normalized() const noexcept;

		//XMVECTOR�I�y���[�^�[
		inline operator XmVector() const noexcept;
		//float*�I�y���[�^�[
		inline operator float*();
		//const float*�I�y���[�^�[
		inline operator const float*() const;

		//=�I�y���[�^�[
		inline XmVector operator = (const XmVector& vec) noexcept;

		//+�I�y���[�^�[  
		inline Vector4 operator +() const;
		//-�I�y���[�^�[  
		inline Vector4 operator -() const;
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
		//��Z�I�y���[�^�[(float)
		inline Vector4 operator *(float value) const;
		//���Z�I�y���[�^�[
		inline Vector4 operator /(const Vector4& vec) const;
		//���Z�I�y���[�^�[(float)
		inline Vector4 operator /(float value) const;

		//+=�I�y���[�^�[
		inline const Vector4 &operator +=(const Vector4& vec);
		//-=�I�y���[�^�[
		inline const Vector4 &operator -=(const Vector4& vec);
		//*=�I�y���[�^�[
		inline const Vector4 &operator *=(const Vector4& vec);
		//*=�I�y���[�^�[(float)
		inline const Vector4 &operator *=(float value);
		// /=�I�y���[�^�[
		inline const Vector4 &operator /=(const Vector4& vec);
		// /=�I�y���[�^�[(float)
		inline const Vector4 &operator /=(float value);

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
		//����1: Vector4Int
		Vector4(const Vector4Int& value);

		//----------------------------------------------------------------------------------
		//[setVector3]
		//Vector4(vec.x, vec.y, vec.z, w)�̋���
		//����1: Vector3
		//����2: w, default = 0.0f
		void setVector3(const Vector3& vector3, float w = 0.0f);

		//Vector2�I�y���[�^�[
		operator Vector2() const;
		//Vector3�I�y���[�^�[
		operator Vector3() const;
		//Vector4Int�I�y���[�^�[
		operator Vector4Int() const;
	
		//=�I�y���[�^�[
		const Vector4&  operator = (const Vector2& vec);
		//=�I�y���[�^�[
		const Vector4& operator = (const Vector3& vec);
		//=�I�y���[�^�[
		const Vector4& operator = (const Vector4Int& vec);

		//----------------------------------------------------------------------------------
		//Vector4Ref

		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�������������
		//����1: Vector4Ref
		Vector4(const Vector4Ref& value);

		// =�I�y���[�^�[
		const Vector4 &operator =(const Vector4Ref & vec);

		//==�I�y���[�^�[
		bool operator ==(const Vector4Ref& vec) const;
		//!=�I�y���[�^�[
		bool operator !=(const Vector4Ref& vec) const;

		//���Z�I�y���[�^�[
		Vector4 operator +(const Vector4Ref& vec) const;
		//���Z�I�y���[�^�[
		Vector4 operator -(const Vector4Ref& vec) const;
		//��Z�I�y���[�^�[
		Vector4 operator *(const Vector4Ref& vec) const;
		//���Z�I�y���[�^�[
		Vector4 operator /(const Vector4Ref& vec) const;

		//+=�I�y���[�^�[
		const Vector4& operator +=(const Vector4Ref& vec);
		//-=�I�y���[�^�[
		const Vector4& operator -=(const Vector4Ref& vec);
		//*=�I�y���[�^�[
		const Vector4& operator *=(const Vector4Ref& vec);
		// /=�I�y���[�^�[
		const Vector4& operator /=(const Vector4Ref& vec);


		//�v�Z���C�u����


		//----------------------------------------------------------------------------------
		//[Dot]<Calculation Library Function>
		//���ς��s��
		//return: ���ς̌���
		//����1: �Ώۃx�N�g��1
		//����2: �Ώۃx�N�g��2
		inline static float Dot(const Vector4& vec1, const Vector4& vec2) noexcept;
		//----------------------------------------------------------------------------------
		//[Distance]<Calculation Library Function>
		//from����to�̋����𑪂�
		//return: from-to�̋���
		//����1: �Ώۃx�N�g��1
		//����2: �Ώۃx�N�g��2
		inline constexpr float Distance(const Vector4& from, const Vector4& to) noexcept;
		//----------------------------------------------------------------------------------
		//[ExactDistance]<Calculation Library Function>
		//from����to�̋����𑪂�, Distance��萳�m�Ȓl�����v�Z���ׂ�����
		//return: from-to�̋���
		//����1: �Ώۃx�N�g��1
		//����2: �Ώۃx�N�g��2
		inline static float ExactDistance(const Vector4& from, const Vector4& to) noexcept;
	};

	//----------------------------------------------------------------------------------
	//[wostream operator]
	//����1: stream
	//����2: this
	inline std::wostream& operator<<(std::wostream& stream, const Vector4& value)
	{
		const wchar* string = value.ToString();
		stream << string;
		return stream;
	}


	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//�S��0�ɐݒ肳���
	inline constexpr Vector4::Vector4() : DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f) {}
	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//�������������
	//����1: x
	//����2: y
	//����3: z
	//����4: w
	inline constexpr Vector4::Vector4(float x, float y, float z, float w)
		: DirectX::XMFLOAT4(x, y, z, w) {}
	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//�������������
	//����1: XMFLOAT4
	inline constexpr Vector4::Vector4(const DirectX::XMFLOAT4 & vec) : XMFLOAT4(vec) {}
	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//�������������
	//����1: PxVec4
	inline constexpr Vector4::Vector4(const physx::PxVec4 & vec) : XMFLOAT4(vec.x, vec.y, vec.z, vec.w) {}
	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//�������������
	//����1: XMVECTOR
	inline Vector4::Vector4(const XmVector & vec)
	{
		DirectX::XMStoreFloat4(this, vec);
	}
	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//�������������
	//����1: float*
	inline Vector4::Vector4(const float * array) : DirectX::XMFLOAT4(array){}

	//----------------------------------------------------------------------------------
	//[CreateWithInt]
	//int��Vector4���쐬
	//����1:x
	//����2:y
	//����3:z
	//����4:w
	inline constexpr Vector4 Vector4::CreateWithInt(int x, int y, int z, int w)
	{
		return Vector4(static_cast<float>(x), static_cast<float>(y), 
			static_cast<float>(z), static_cast<float>(w));
	}

	//----------------------------------------------------------------------------------
	//[Absolute]
	//vec�̐�Βl���v�Z����
	//return: ��Βl
	inline constexpr Vector4 Vector4::Absolute() const noexcept
	{
		return std::move(Vector4(x < 0.0f ? -x : x, y < 0.0f ? -y : y, z < 0.0f ? -z : z, w < 0.0f ? -w : w));
	}
	//----------------------------------------------------------------------------------
	//[Magnitude]
	//return: ���g��Vector(0,0, 0, 0)�̋���
	inline constexpr float Vector4::Magnitude() const noexcept
	{
		float f = x * x + y * y + z * z + w * w;
		int i = 0x5f3759df - (*reinterpret_cast<int*>(&f) >> 1);
		float bitF = *reinterpret_cast<float*>(&i);

		bitF *= (1.5f - 0.5f * f * bitF * bitF);
		return f * bitF * (1.5f - 0.5f * f * bitF * bitF);
	}
	//----------------------------------------------------------------------------------
	//[ExactMagnitude]
	//return: ���g��Vector(0,0,0)�̋��� (���m�Ȓl�������ׂ�����)
	inline float Vector4::ExactMagnitude() const noexcept
	{
		return std::sqrtf(x * x + y * y + z * z + w * w);
	}
	//----------------------------------------------------------------------------------
	//[Normalize]
	//return: ���g�𐳋K�������x�N�g��
	inline Vector4 Vector4::Normalized() const noexcept
	{
		float divLength = 1.0f / ExactMagnitude();
		return std::move(Vector4(x * divLength, y * divLength, z * divLength, w * divLength));
	}

	//XMVECTOR�I�y���[�^�[
	inline Vector4::operator XmVector() const noexcept {	return DirectX::XMLoadFloat4(this); }
	//float*�I�y���[�^�[
	inline Vector4::operator float*() { return (float*)this; }
	//const float*�I�y���[�^�[
	inline Vector4::operator const float*() const { return (float*)this; }

	//=�I�y���[�^�[
	inline XmVector Vector4::operator=(const XmVector & vec) noexcept
	{
		DirectX::XMStoreFloat4(this, vec);
		return vec;
	}

	//+�I�y���[�^�[  
	inline Vector4 Vector4::operator+() const { return Vector4(+this->x, +this->y, +this->z, +this->w); }
	//-�I�y���[�^�[  
	inline Vector4 Vector4::operator-() const{	return Vector4(-this->x, -this->y , -this->z, -this->w); }
	//==�I�y���[�^�[
	inline bool Vector4::operator==(const Vector4& vec) const
	{
		return (fabsf(x - vec.x) <= MathAF::cEpsilon
			&& fabsf(y - vec.y) <= MathAF::cEpsilon
			&& fabsf(z - vec.z) <= MathAF::cEpsilon
			&& fabsf(w - vec.w) <= MathAF::cEpsilon);
	}
	//!=�I�y���[�^�[
	inline bool Vector4::operator!=(const Vector4& vec) const
	{
		return (fabsf(x - vec.x) > MathAF::cEpsilon
			&& fabsf(y - vec.y) > MathAF::cEpsilon
			&& fabsf(z - vec.z) > MathAF::cEpsilon
			&& fabsf(w - vec.w) > MathAF::cEpsilon);
	}

	//���Z�I�y���[�^�[
	inline Vector4 Vector4::operator+(const Vector4& vec) const{	return Vector4(x + vec.x, y + vec.y, z + vec.z, w + vec.w); }
	//���Z�I�y���[�^�[
	inline Vector4 Vector4::operator-(const Vector4& vec) const{	return Vector4(x - vec.x, y - vec.y, z - vec.z, w - vec.w); }
	//��Z�I�y���[�^�[
	inline Vector4 Vector4::operator*(const Vector4& vec) const{ return Vector4(x * vec.x, y * vec.y, z * vec.z, w * vec.w); }
	//��Z�I�y���[�^�[(float)
	inline Vector4 Vector4::operator*(float value) const{ return Vector4(x * value, y * value, z * value, w * value); }
	//���Z�I�y���[�^�[
	inline Vector4 Vector4::operator/(const Vector4& vec) const{ return Vector4(x / vec.x, y / vec.y, z / vec.z, w / vec.w); }
	//���Z�I�y���[�^�[(float)
	inline Vector4 Vector4::operator/(float value) const{return Vector4(x / value, y / value, z / value, w / value); }
	
	//+=�I�y���[�^�[
	inline const Vector4 & Vector4::operator+=(const Vector4& vec)
	{
		this->x += vec.x;		this->y += vec.y;
		this->z += vec.z;		this->w += vec.w;
		return *this;
	}
	//-=�I�y���[�^�[
	inline const Vector4 & Vector4::operator-=(const Vector4& vec)
	{
		this->x -= vec.x;		this->y -= vec.y;
		this->z -= vec.z;		this->w -= vec.w;
		return *this;
	}
	//*=�I�y���[�^�[
	inline const Vector4 & Vector4::operator*=(const Vector4& vec)
	{
		this->x *= vec.x;		this->y *= vec.y;
		this->z *= vec.z;		this->w *= vec.w;
		return *this;
	}
	//*=�I�y���[�^�[(float)
	inline const Vector4 & Vector4::operator*=(float value)
	{
		this->x *= value;		this->y *= value;
		this->z *= value;		this->w *= value;
		return *this;
	}
	// /=�I�y���[�^�[
	inline const Vector4 & Vector4::operator/=(const Vector4& vec)
	{
		this->x /= vec.x;		this->y /= vec.y;
		this->z /= vec.z;		this->w /= vec.w;
		return *this;
	}
	// /=�I�y���[�^�[(float)
	inline const Vector4 & Vector4::operator/=(float value)
	{
		this->x /= value;		this->y /= value;
		this->z /= value;		this->w /= value;
		return *this;
	}


	//----------------------------------------------------------------------------------
	//[Dot]<Calculation Library Function>
	//���ς��s��
	//return: ���ς̌���
	//����1: �Ώۃx�N�g��1
	//����2: �Ώۃx�N�g��2
	inline float Vector4::Dot(const Vector4 & vec1, const Vector4 & vec2) noexcept
	{
		return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z + vec1.w * vec2.w;
	}
	//----------------------------------------------------------------------------------
	//[Distance]<Calculation Library Function>
	//from����to�̋����𑪂�
	//return: from-to�̋���
	//����1: �Ώۃx�N�g��1
	//����2: �Ώۃx�N�g��2
	inline constexpr float Vector4::Distance(const Vector4 & from, const Vector4 & to) noexcept
	{
		float f = (from.x - to.x) * (from.x - to.x)
			+ (from.y - to.y) * (from.y - to.y)
			+ (from.z - to.z) * (from.z - to.z)
			+ (to.w - from.w) * (to.w - from.w);
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
	inline float Vector4::ExactDistance(const Vector4& from, const Vector4& to) noexcept
	{
		return std::sqrtf((to.x - from.x) * (to.x - from.x) + (to.y - from.y) * (to.y - from.y)
			+ (to.z - from.z) * (to.z - from.z) + (to.w - from.w) * (to.w - from.w));
	}
}

#endif // !SGFRAMEWORK_HEADER_VECTOR_VECTOR4_HPP_
