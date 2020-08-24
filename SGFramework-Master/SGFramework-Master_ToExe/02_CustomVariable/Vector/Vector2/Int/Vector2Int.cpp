/*----------------------------------------------------------------------------------
2��int�l��������Vector2Int �\����
�ł����p����\���̃V���[�Y��1�Ȃ̂ŕʎ���Vector���֌W�Ȃ����̂͑S��inline����
	���֐����L�q����Vector2Int.cpp
------------------------------------------------------------------------------------*/
#include "Vector2Int.hpp"
#include "../../Vector3/Int/Vector3Int.hpp"
#include "../../Vector4/Int/Vector4Int.hpp"
#include "../Vector2.hpp"
#include "../Ref/Vector2Ref.hpp"

//Framework namespace
namespace SGFramework
{
	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//�������������
	//����1: Vector2
	Vector2Int::Vector2Int(const Vector2 & vec)
	{
		this->x = static_cast<int>(vec.x);		this->y = static_cast<int>(vec.y);
	}

	//----------------------------------------------------------------------------------
	//[ToFloat]
	//Vector2�ɕϊ����s��
	//return: Vector2�ɕϊ�����this
	Vector2 Vector2Int::ToFloat() const
	{
		return Vector2(static_cast<float>(x), static_cast<float>(y));
	}

	//Vector3Int�I�y���[�^�[
	Vector2Int::operator Vector3Int() const { return Vector3Int(x, y, 0); }
	//Vector4Int�I�y���[�^�[
	Vector2Int::operator Vector4Int() const { return Vector4Int(x, y, 0, 0); }

	//=�I�y���[�^�[
	const Vector2Int& Vector2Int::operator=(const Vector3Int& vec)
	{
		this->x = vec.x;		this->y = vec.y;
		return *this;
	}
	//=�I�y���[�^�[
	const Vector2Int& Vector2Int::operator=(const Vector4Int& vec)
	{
		this->x = vec.x;		this->y = vec.y;
		return *this;
	}
	//=�I�y���[�^�[
	const Vector2Int& Vector2Int::operator = (const Vector2 &vec)
	{
		this->x = static_cast<int>(vec.x);		this->y = static_cast<int>(vec.y);
		return *this;
	}
	const Vector2Int & Vector2Int::operator=(const Vector2Ref & vec)
	{
		this->x = static_cast<int>(vec.x);		this->y = static_cast<int>(vec.y);
		return *this;
	}
}