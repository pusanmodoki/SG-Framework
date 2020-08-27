/*----------------------------------------------------------------------------------
2つの値を扱えるVector4Ref (Ref) 構造体
最も多用する構造体シリーズの1つなので別自作Vectorが関係ないものは全てinline実装
------------------------------------------------------------------------------------*/
#include "Vector4Ref.hpp"
#include "../Int/Vector4Int.hpp"
#include "../../Vector2/Vector2.hpp"
#include "../../Vector3/Vector3.hpp"

//Framework namespace
namespace SGFramework
{
	//Vector3オペレーター
	inline Vector4Ref::operator Vector2() const
	{
		return Vector2(x, y);
	}
	//Vector3オペレーター
	Vector4Ref::operator Vector3() const
	{
		return Vector3(x, y, z);
	}
	//Vector4Intオペレーター
	Vector4Ref::operator Vector4Int() const
	{
		return Vector4Int(static_cast<int>(x), static_cast<int>(y),
			static_cast<int>(z), static_cast<int>(w));
	}
	//=オペレーター
	const Vector4Ref & Vector4Ref::operator=(const Vector2 & vec)
	{
		x = vec.x; y = vec.y;
		z = 0.0f; w = 0.0f;
		return *this;
	}
	//=オペレーター
	const Vector4Ref & Vector4Ref::operator=(const Vector3 & vec)
	{
		x = vec.x; y = vec.y;
		z = vec.z; w = 0.0f;
		return *this;
	}
	//=オペレーター
	const Vector4Ref & Vector4Ref::operator=(const Vector4Int & vec)
	{
		x = static_cast<float>(vec.x); y = static_cast<float>(vec.y);
		z = static_cast<float>(vec.z); w = static_cast<float>(vec.w);
		return *this;
	}
}