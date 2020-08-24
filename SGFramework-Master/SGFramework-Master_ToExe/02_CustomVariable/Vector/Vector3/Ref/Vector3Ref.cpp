/*----------------------------------------------------------------------------------
3�̒l��������Vector3Ref (Ref) �\����
�ł����p����\���̃V���[�Y��1�Ȃ̂ŕʎ���Vector���֌W�Ȃ����̂͑S��inline����
------------------------------------------------------------------------------------*/
#include "Vector3Ref.hpp"
#include "../Int/Vector3Int.hpp"
#include "../../Vector2/Vector2.hpp"
#include "../../Vector4/Vector4.hpp"

//Framework namespace
namespace SGFramework
{
	//Vector2�I�y���[�^�[
	inline Vector3Ref::operator Vector2() const
	{
		return Vector2(x, y);
	}
	//Vector4�I�y���[�^�[
	Vector3Ref::operator Vector4() const
	{
		return Vector4(x, y, z, 0.0f);
	}
	//Vector3Int�I�y���[�^�[
	Vector3Ref::operator Vector3Int() const
	{
		return Vector3Int(static_cast<int>(x), static_cast<int>(y), static_cast<int>(z));
	}
	//=�I�y���[�^�[
	const Vector3Ref & Vector3Ref::operator=(const Vector2 & vec)
	{
		x = vec.x; y = vec.y;
		z = 0.0f;
		return *this;
	}
	//=�I�y���[�^�[
	const Vector3Ref & Vector3Ref::operator=(const Vector4 & vec)
	{
		x = vec.x; y = vec.y;
		z = vec.z;
		return *this;
	}
	//=�I�y���[�^�[
	const Vector3Ref & Vector3Ref::operator=(const Vector3Int & vec)
	{
		x = static_cast<float>(vec.x); y = static_cast<float>(vec.y);
		z = static_cast<float>(vec.z);
		return *this;
	}
}