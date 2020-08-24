/*----------------------------------------------------------------------------------
2�̒l��������Vector2 �\����
�ł����p����\���̃V���[�Y��1�Ȃ̂ŕʎ���Vector���֌W�Ȃ����̂͑S��inline����
	���֐����L�ڂ���Vector2.cpp
------------------------------------------------------------------------------------*/
#include <math.h>
#include "Vector2.hpp"
#include "Int/Vector2Int.hpp"
#include "Ref/Vector2Ref.hpp"
#include "../Vector3/Vector3.hpp"
#include "../Vector4/Vector4.hpp"

//Framework namespace
namespace SGFramework
{
	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//�������������
	//����1: Vector2Int
	//----------------------------------------------------------------------------------
	Vector2::Vector2(const Vector2Int & vec)
	{
		this->x = static_cast<float>(vec.x);		this->y = static_cast<float>(vec.y);
	}

	//Vector3�I�y���[�^�[
	Vector2::operator Vector3() const { return Vector3(x, y, 0.0f); }
	//Vector4�I�y���[�^�[
	Vector2::operator Vector4() const { return Vector4(x, y, 0.0f, 0.0f); }
	//Vector2Int�I�y���[�^�[
	Vector2::operator Vector2Int() const { return Vector2Int(static_cast<int>(x), static_cast<int>(y)); }

	//=�I�y���[�^�[
	const Vector2& Vector2::operator=(const Vector3& vec)
	{
		this->x = vec.x;		this->y = vec.y;
		return *this;
	}
	//=�I�y���[�^�[
	const Vector2& Vector2::operator=(const Vector4& vec)
	{
		this->x = vec.x;		this->y = vec.y;
		return *this;
	}
	//=�I�y���[�^�[
	const Vector2& Vector2::operator = (const Vector2Int& vec)
	{
		this->x = static_cast<float>(vec.x);		this->y = static_cast<float>(vec.y);
		return *this;
	}


	//----------------------------------------------------------------------------------
	//Vector2Ref

	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//�������������
	//����1: Vector2Ref
	Vector2::Vector2(const Vector2Ref & value) : DirectX::XMFLOAT2(value.x, value.y) {}

	//=�I�y���[�^�[
	const Vector2 & Vector2::operator=(const Vector2Ref & vec)
	{
		x = vec.x; y = vec.y;
		return *this;
	}


	//==�I�y���[�^�[
	bool Vector2::operator ==(const Vector2Ref& vec) const 
	{
		return (fabsf(x - vec.x) <= MathAF::cEpsilon
			&& fabsf(y - vec.y) <= MathAF::cEpsilon);
	}
	//!=�I�y���[�^�[
	bool Vector2::operator !=(const Vector2Ref& vec) const
	{
		return (fabsf(x - vec.x) <= MathAF::cEpsilon
			&& fabsf(y - vec.y) <= MathAF::cEpsilon);
	}

	//���Z�I�y���[�^�[
	Vector2 Vector2::operator +(const Vector2Ref& vec) const
	{
		return Vector2(x + vec.x, y + vec.y);
	}
	//���Z�I�y���[�^�[
	Vector2 Vector2::operator -(const Vector2Ref& vec) const
	{
		return Vector2(x - vec.x, y - vec.y);
	}
	//��Z�I�y���[�^�[
	Vector2 Vector2::operator *(const Vector2Ref& vec) const 
	{
		return Vector2(x * vec.x, y * vec.y);
	}
	//���Z�I�y���[�^�[
	Vector2 Vector2::operator /(const Vector2Ref& vec) const
	{
		return Vector2(x / vec.x, y / vec.y);
	}

	//+=�I�y���[�^�[
	const Vector2& Vector2::operator +=(const Vector2Ref& vec)
	{
		this->x += vec.x;		this->y += vec.y;
		return *this;
	}
	//-=�I�y���[�^�[
	const Vector2& Vector2::operator -=(const Vector2Ref& vec)
	{
		this->x -= vec.x;		this->y -= vec.y;
		return *this;
	}
	//*=�I�y���[�^�[
	const Vector2& Vector2::operator *=(const Vector2Ref& vec)
	{
		this->x *= vec.x;		this->y *= vec.y;
		return *this;
	}
	// /=�I�y���[�^�[
	const Vector2& Vector2::operator /=(const Vector2Ref& vec)
	{
		this->x /= vec.x;		this->y /= vec.y;
		return *this;
	}
}