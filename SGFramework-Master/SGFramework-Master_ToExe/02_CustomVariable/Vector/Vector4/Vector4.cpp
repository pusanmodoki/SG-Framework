/*----------------------------------------------------------------------------------
4�̒l��������Vector4 �\����
�ł����p����\���̃V���[�Y��1�Ȃ̂ŕʎ���Vector���֌W�Ȃ����̂͑S��inline����
���֐�, inline�֐�����������Vector4.cpp	
------------------------------------------------------------------------------------*/
#include "Vector4.hpp"
#include "Int/Vector4Int.hpp"
#include "Ref/Vector4Ref.hpp"
#include "../Vector2/Vector2.hpp"
#include "../Vector3/Vector3.hpp"

//Framework namespace
namespace SGFramework
{
	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//�������������
	//����1: Vector4Int
	Vector4::Vector4(const Vector4Int & vec)
	{
		this->x = static_cast<float>(vec.x);		this->y = static_cast<float>(vec.y);
		this->z = static_cast<float>(vec.z);		this->w = static_cast<float>(vec.w);
	}

	//----------------------------------------------------------------------------------
	//[setVector3]
	//Vector4(vec.x, vec.y, vec.z, w)�̋���
	//����1: Vector3
	//����2: w, default = 0.0f
	void Vector4::setVector3(const Vector3 & vector3, float w)
	{
		this->x = vector3.x;
		this->y = vector3.y;
		this->z = vector3.z;
		this->w = w;
	}

	//Vector2�I�y���[�^�[
	Vector4::operator Vector2() const { return Vector2(x, y); }
	//Vector3�I�y���[�^�[
	Vector4::operator Vector3() const { return Vector3(x, y, z); }
	//Vector4Int�I�y���[�^�[
	Vector4::operator Vector4Int() const { return Vector4Int(static_cast<int>(x), 
		static_cast<int>(y), static_cast<int>(z), static_cast<int>(w)); }

	//=�I�y���[�^�[
	const Vector4& Vector4::operator=(const Vector2& vec)
	{
		this->x = vec.x;		this->y = vec.y;
		this->z = 0.0f;		this->w = 0.0f;
		return *this;
	}
	//=�I�y���[�^�[
	const Vector4& Vector4::operator=(const Vector3& vec)
	{
		this->x = vec.x;		this->y = vec.y;
		this->z = vec.z;		this->w = 0.0f;
		return *this;
	}
	//=�I�y���[�^�[
	const Vector4& Vector4::operator = (const Vector4Int& vec)
	{
		this->x = static_cast<float>(vec.x);		this->y = static_cast<float>(vec.y);
		this->z = static_cast<float>(vec.z);		this->w = static_cast<float>(vec.w);
		return *this;
	}



	//----------------------------------------------------------------------------------
	//Vector4Ref

	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//�������������
	//����1: Vector4Ref
	Vector4::Vector4(const Vector4Ref & value) : DirectX::XMFLOAT4(value.x, value.y, value.z, value.w) {}

	//=�I�y���[�^�[
	const Vector4 & Vector4::operator=(const Vector4Ref & vec)
	{
		x = vec.x;		y = vec.y;
		z = vec.z;		w = vec.w;
		return *this;
	}

	//==�I�y���[�^�[
	bool Vector4::operator ==(const Vector4Ref& vec) const
	{
		return (fabsf(x - vec.x) <= MathAF::cEpsilon
			&& fabsf(y - vec.y) <= MathAF::cEpsilon
			&& fabsf(z - vec.z) <= MathAF::cEpsilon
			&& fabsf(w - vec.w) <= MathAF::cEpsilon);
	}
	//!=�I�y���[�^�[
	bool Vector4::operator !=(const Vector4Ref& vec) const
	{
		return (fabsf(x - vec.x) <= MathAF::cEpsilon
			&& fabsf(y - vec.y) <= MathAF::cEpsilon
			&& fabsf(z - vec.z) <= MathAF::cEpsilon
			&& fabsf(w - vec.w) <= MathAF::cEpsilon);
	}

	//���Z�I�y���[�^�[
	Vector4 Vector4::operator +(const Vector4Ref& vec) const
	{
		return Vector4(x + vec.x, y + vec.y, z + vec.z, w + vec.w);
	}
	//���Z�I�y���[�^�[
	Vector4 Vector4::operator -(const Vector4Ref& vec) const
	{
		return Vector4(x - vec.x, y - vec.y, z - vec.z, w - vec.w);
	}
	//��Z�I�y���[�^�[
	Vector4 Vector4::operator *(const Vector4Ref& vec) const
	{
		return Vector4(x * vec.x, y * vec.y, z * vec.z, w * vec.w);
	}
	//���Z�I�y���[�^�[
	Vector4 Vector4::operator /(const Vector4Ref& vec) const
	{
		return Vector4(x / vec.x, y / vec.y, z / vec.z, w / vec.w);
	}

	//+=�I�y���[�^�[
	const Vector4& Vector4::operator +=(const Vector4Ref& vec)
	{
		this->x += vec.x;		this->y += vec.y;
		this->z += vec.z;		this->w += vec.w;
		return *this;
	}
	//-=�I�y���[�^�[
	const Vector4& Vector4::operator -=(const Vector4Ref& vec)
	{
		this->x -= vec.x;		this->y -= vec.y;
		this->z -= vec.z;		this->w -= vec.w;
		return *this;
	}
	//*=�I�y���[�^�[
	const Vector4& Vector4::operator *=(const Vector4Ref& vec)
	{
		this->x *= vec.x;		this->y *= vec.y;
		this->z *= vec.z;		this->w *= vec.w;
		return *this;
	}
	// /=�I�y���[�^�[
	const Vector4& Vector4::operator /=(const Vector4Ref& vec)
	{
		this->x /= vec.x;		this->y /= vec.y;
		this->z /= vec.z;		this->w /= vec.w;
		return *this;
	}
}