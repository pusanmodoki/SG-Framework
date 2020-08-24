/*----------------------------------------------------------------------------------
3��int�l��������Vector3Int �\����
�ł����p����\���̃V���[�Y��1�Ȃ̂ŕʎ���Vector���֌W�Ȃ����̂͑S��inline����
	���֐����L�q����Vector3Int.cpp
------------------------------------------------------------------------------------*/
#include "Vector3Int.hpp"
#include "../../Vector2/Int/Vector2Int.hpp"
#include "../../Vector4/Int/Vector4Int.hpp"
#include "../Vector3.hpp"
#include "../Ref/Vector3Ref.hpp"

//Framework namespace
namespace SGFramework
{
	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//�������������
	//����1: Vector
	Vector3Int::Vector3Int(const Vector3 & vec)
	{
		this->x = static_cast<int>(vec.x);		this->y = static_cast<int>(vec.y);
		this->z = static_cast<int>(vec.z);
	}

	//----------------------------------------------------------------------------------
	//[ToFloat]
	//Vector3�ɕϊ����s��
	//return: Vector3�ɕϊ�����this
	Vector3 Vector3Int::ToFloat() const
	{
		return Vector3(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
	}

	//Vector2Int�I�y���[�^�[
	Vector3Int::operator Vector2Int() const { return Vector2Int(x, y); }
	//Vector4Int�I�y���[�^�[
	Vector3Int::operator Vector4Int() const { return Vector4Int(x, y, z, 0); }

	//=�I�y���[�^�[
	const Vector3Int& Vector3Int::operator=(const Vector2Int& vec)
	{
		this->x = vec.x;		this->y = vec.y;
		this->z = 0;
		return *this;
	}
	//=�I�y���[�^�[
	const Vector3Int& Vector3Int::operator=(const Vector4Int& vec)
	{
		this->x = vec.x;		this->y = vec.y;
		this->z = vec.z;
		return *this;
	}
	//=�I�y���[�^�[
	const Vector3Int& Vector3Int::operator = (const Vector3& vec)
	{
		this->x = static_cast<int>(vec.x);		this->y = static_cast<int>(vec.y);
		this->z = static_cast<int>(vec.z);
		return *this;
	}
	//=�I�y���[�^�[
	const Vector3Int & Vector3Int::operator=(const Vector3Ref & vec)
	{
		this->x = static_cast<int>(vec.x);		this->y = static_cast<int>(vec.y);
		this->z = static_cast<int>(vec.z);
		return *this;
	}
}