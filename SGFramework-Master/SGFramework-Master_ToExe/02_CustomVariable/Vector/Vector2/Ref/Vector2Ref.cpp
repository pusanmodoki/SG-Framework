/*----------------------------------------------------------------------------------
2つの値を扱えるVector2Ref (参照)構造体
最も多用する構造体シリーズの1つなので別自作Vectorが関係ないものは全てinline実装
------------------------------------------------------------------------------------*/
#include "Vector2Ref.hpp"
#include "../Int/Vector2Int.hpp"
#include "../../Vector3/Vector3.hpp"
#include "../../Vector4/Vector4.hpp"

//Framework namespace
namespace SGFramework
{
	//Vector3オペレーター
	inline Vector2Ref::operator Vector3() const
	{
		return Vector3(x, y, 0.0f);
	}
	//Vector4オペレーター
	Vector2Ref::operator Vector4() const
	{
		return Vector4(x, y, 0.0f, 0.0f);
	}
	//Vector2Intオペレーター
	Vector2Ref::operator Vector2Int() const
	{
		return Vector2Int(static_cast<int>(x), static_cast<int>(y));
	}
	//=オペレーター
	const Vector2Ref & Vector2Ref::operator=(const Vector3 & vec)
	{
		x = vec.x; y = vec.y;
		return *this;
	}
	//=オペレーター
	const Vector2Ref & Vector2Ref::operator=(const Vector4 & vec)
	{
		x = vec.x; y = vec.y;
		return *this;
	}
	//=オペレーター
	const Vector2Ref & Vector2Ref::operator=(const Vector2Int & vec)
	{
		x = static_cast<float>(vec.x);		y = static_cast<float>(vec.y);
		return *this;
	}
}