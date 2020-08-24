/*----------------------------------------------------------------------------------
2�̒l��������Vector2Ref (�Q��)�\����
�ł����p����\���̃V���[�Y��1�Ȃ̂ŕʎ���Vector���֌W�Ȃ����̂͑S��inline����
------------------------------------------------------------------------------------*/
#include "Vector2Ref.hpp"
#include "../Int/Vector2Int.hpp"
#include "../../Vector3/Vector3.hpp"
#include "../../Vector4/Vector4.hpp"

//Framework namespace
namespace SGFramework
{
	//Vector3�I�y���[�^�[
	inline Vector2Ref::operator Vector3() const
	{
		return Vector3(x, y, 0.0f);
	}
	//Vector4�I�y���[�^�[
	Vector2Ref::operator Vector4() const
	{
		return Vector4(x, y, 0.0f, 0.0f);
	}
	//Vector2Int�I�y���[�^�[
	Vector2Ref::operator Vector2Int() const
	{
		return Vector2Int(static_cast<int>(x), static_cast<int>(y));
	}
	//=�I�y���[�^�[
	const Vector2Ref & Vector2Ref::operator=(const Vector3 & vec)
	{
		x = vec.x; y = vec.y;
		return *this;
	}
	//=�I�y���[�^�[
	const Vector2Ref & Vector2Ref::operator=(const Vector4 & vec)
	{
		x = vec.x; y = vec.y;
		return *this;
	}
	//=�I�y���[�^�[
	const Vector2Ref & Vector2Ref::operator=(const Vector2Int & vec)
	{
		x = static_cast<float>(vec.x);		y = static_cast<float>(vec.y);
		return *this;
	}
}