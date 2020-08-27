/*----------------------------------------------------------------------------------
3つの値を扱えるVector3Ref (Ref) 構造体
最も多用する構造体シリーズの1つなので別自作Vectorが関係ないものは全てinline実装
------------------------------------------------------------------------------------*/
#include "Vector3Ref.hpp"
#include "../Int/Vector3Int.hpp"
#include "../../Vector2/Vector2.hpp"
#include "../../Vector4/Vector4.hpp"

//Framework namespace
namespace SGFramework
{
	//Vector2オペレーター
	inline Vector3Ref::operator Vector2() const
	{
		return Vector2(x, y);
	}
	//Vector4オペレーター
	Vector3Ref::operator Vector4() const
	{
		return Vector4(x, y, z, 0.0f);
	}
	//Vector3Intオペレーター
	Vector3Ref::operator Vector3Int() const
	{
		return Vector3Int(static_cast<int>(x), static_cast<int>(y), static_cast<int>(z));
	}
	//=オペレーター
	const Vector3Ref & Vector3Ref::operator=(const Vector2 & vec)
	{
		x = vec.x; y = vec.y;
		z = 0.0f;
		return *this;
	}
	//=オペレーター
	const Vector3Ref & Vector3Ref::operator=(const Vector4 & vec)
	{
		x = vec.x; y = vec.y;
		z = vec.z;
		return *this;
	}
	//=オペレーター
	const Vector3Ref & Vector3Ref::operator=(const Vector3Int & vec)
	{
		x = static_cast<float>(vec.x); y = static_cast<float>(vec.y);
		z = static_cast<float>(vec.z);
		return *this;
	}
}