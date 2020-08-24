/*----------------------------------------------------------------------------------
2つのint値を扱えるVector2Int 構造体
最も多用する構造体シリーズの1つなので別自作Vectorが関係ないものは全てinline実装
	→関数を記述したVector2Int.cpp
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
	//[コンストラクタ]
	//代入初期化する
	//引数1: Vector2
	Vector2Int::Vector2Int(const Vector2 & vec)
	{
		this->x = static_cast<int>(vec.x);		this->y = static_cast<int>(vec.y);
	}

	//----------------------------------------------------------------------------------
	//[ToFloat]
	//Vector2に変換を行う
	//return: Vector2に変換したthis
	Vector2 Vector2Int::ToFloat() const
	{
		return Vector2(static_cast<float>(x), static_cast<float>(y));
	}

	//Vector3Intオペレーター
	Vector2Int::operator Vector3Int() const { return Vector3Int(x, y, 0); }
	//Vector4Intオペレーター
	Vector2Int::operator Vector4Int() const { return Vector4Int(x, y, 0, 0); }

	//=オペレーター
	const Vector2Int& Vector2Int::operator=(const Vector3Int& vec)
	{
		this->x = vec.x;		this->y = vec.y;
		return *this;
	}
	//=オペレーター
	const Vector2Int& Vector2Int::operator=(const Vector4Int& vec)
	{
		this->x = vec.x;		this->y = vec.y;
		return *this;
	}
	//=オペレーター
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