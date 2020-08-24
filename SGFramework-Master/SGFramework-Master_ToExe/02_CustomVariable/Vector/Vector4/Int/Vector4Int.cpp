/*----------------------------------------------------------------------------------
4��int�l��������Vector4Int �\����
�ł����p����\���̃V���[�Y��1�Ȃ̂ŕʎ���Vector���֌W�Ȃ����̂͑S��inline����
	���֐����L�q����Vector4Int.cpp
------------------------------------------------------------------------------------*/
#include "Vector4Int.hpp"
#include "../../Vector2/Int/Vector2Int.hpp"
#include "../../Vector3/Int/Vector3Int.hpp"
#include "../Vector4.hpp"
#include "../Ref/Vector4Ref.hpp"

//Framework namespace
namespace SGFramework
{
	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//�������������
	//����1: Vector4
	Vector4Int::Vector4Int(const Vector4 & vec)
	{
		this->x = static_cast<int>(vec.x);		this->y = static_cast<int>(vec.y);
		this->z = static_cast<int>(vec.z);		this->w = static_cast<int>(vec.w);
	}

	//----------------------------------------------------------------------------------
	//[ToFloat]
	//Vector4�ɕϊ����s��
	//return: Vector4�ɕϊ�����this
	Vector4 Vector4Int::ToFloat() const
	{
		return Vector4(static_cast<float>(x), static_cast<float>(y),
			static_cast<float>(z), static_cast<float>(w));
	}

	//Vector2Int�I�y���[�^�[
	Vector4Int::operator Vector2Int() const { return Vector2Int(x, y); }
	//Vector3Int�I�y���[�^�[
	Vector4Int::operator Vector3Int() const { return Vector3Int(x, y, z); }

	//=�I�y���[�^�[
	const Vector4Int& Vector4Int::operator=(const Vector2Int& vec)
	{
		this->x = vec.x;		this->y = vec.y;
		this->z = 0;		this->w = 0;
		return *this;
	}
	//=�I�y���[�^�[
	const Vector4Int& Vector4Int::operator=(const Vector3Int& vec)
	{
		this->x = vec.x;		this->y = vec.y;
		this->z = vec.z;		this->w = 0;
		return *this;
	}
	//=�I�y���[�^�[
	const Vector4Int& Vector4Int::operator = (const Vector4& vec)
	{
		this->x = static_cast<int>(vec.x); this->y = static_cast<int>(vec.y);
		this->z = static_cast<int>(vec.z); this->w = static_cast<int>(vec.w);
		return *this;
	}
	//=�I�y���[�^�[
	const Vector4Int & Vector4Int::operator=(const Vector4Ref & vec)
	{
		this->x = static_cast<int>(vec.x); this->y = static_cast<int>(vec.y);
		this->z = static_cast<int>(vec.z); this->w = static_cast<int>(vec.w);
		return *this;
	}
}