/*----------------------------------------------------------------------------------
2つの値を扱えるVector2 構造体
最も多用する構造体シリーズの1つなので別自作Vectorが関係ないものは全てinline実装
	→関数を記載したVector2.cpp
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
	//[コンストラクタ]
	//代入初期化する
	//引数1: Vector2Int
	//----------------------------------------------------------------------------------
	Vector2::Vector2(const Vector2Int & vec)
	{
		this->x = static_cast<float>(vec.x);		this->y = static_cast<float>(vec.y);
	}

	//Vector3オペレーター
	Vector2::operator Vector3() const { return Vector3(x, y, 0.0f); }
	//Vector4オペレーター
	Vector2::operator Vector4() const { return Vector4(x, y, 0.0f, 0.0f); }
	//Vector2Intオペレーター
	Vector2::operator Vector2Int() const { return Vector2Int(static_cast<int>(x), static_cast<int>(y)); }

	//=オペレーター
	const Vector2& Vector2::operator=(const Vector3& vec)
	{
		this->x = vec.x;		this->y = vec.y;
		return *this;
	}
	//=オペレーター
	const Vector2& Vector2::operator=(const Vector4& vec)
	{
		this->x = vec.x;		this->y = vec.y;
		return *this;
	}
	//=オペレーター
	const Vector2& Vector2::operator = (const Vector2Int& vec)
	{
		this->x = static_cast<float>(vec.x);		this->y = static_cast<float>(vec.y);
		return *this;
	}


	//----------------------------------------------------------------------------------
	//Vector2Ref

	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//代入初期化する
	//引数1: Vector2Ref
	Vector2::Vector2(const Vector2Ref & value) : DirectX::XMFLOAT2(value.x, value.y) {}

	//=オペレーター
	const Vector2 & Vector2::operator=(const Vector2Ref & vec)
	{
		x = vec.x; y = vec.y;
		return *this;
	}


	//==オペレーター
	bool Vector2::operator ==(const Vector2Ref& vec) const 
	{
		return (fabsf(x - vec.x) <= MathAF::cEpsilon
			&& fabsf(y - vec.y) <= MathAF::cEpsilon);
	}
	//!=オペレーター
	bool Vector2::operator !=(const Vector2Ref& vec) const
	{
		return (fabsf(x - vec.x) <= MathAF::cEpsilon
			&& fabsf(y - vec.y) <= MathAF::cEpsilon);
	}

	//加算オペレーター
	Vector2 Vector2::operator +(const Vector2Ref& vec) const
	{
		return Vector2(x + vec.x, y + vec.y);
	}
	//減算オペレーター
	Vector2 Vector2::operator -(const Vector2Ref& vec) const
	{
		return Vector2(x - vec.x, y - vec.y);
	}
	//乗算オペレーター
	Vector2 Vector2::operator *(const Vector2Ref& vec) const 
	{
		return Vector2(x * vec.x, y * vec.y);
	}
	//除算オペレーター
	Vector2 Vector2::operator /(const Vector2Ref& vec) const
	{
		return Vector2(x / vec.x, y / vec.y);
	}

	//+=オペレーター
	const Vector2& Vector2::operator +=(const Vector2Ref& vec)
	{
		this->x += vec.x;		this->y += vec.y;
		return *this;
	}
	//-=オペレーター
	const Vector2& Vector2::operator -=(const Vector2Ref& vec)
	{
		this->x -= vec.x;		this->y -= vec.y;
		return *this;
	}
	//*=オペレーター
	const Vector2& Vector2::operator *=(const Vector2Ref& vec)
	{
		this->x *= vec.x;		this->y *= vec.y;
		return *this;
	}
	// /=オペレーター
	const Vector2& Vector2::operator /=(const Vector2Ref& vec)
	{
		this->x /= vec.x;		this->y /= vec.y;
		return *this;
	}
}