/*----------------------------------------------------------------------------------
3つのint値を扱えるVector3Int 構造体
最も多用する構造体シリーズの1つなので別自作Vectorが関係ないものは全てinline実装
	→関数を記述したVector3Int.cpp
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
	//[コンストラクタ]
	//代入初期化する
	//引数1: Vector
	Vector3Int::Vector3Int(const Vector3 & vec)
	{
		this->x = static_cast<int>(vec.x);		this->y = static_cast<int>(vec.y);
		this->z = static_cast<int>(vec.z);
	}

	//----------------------------------------------------------------------------------
	//[ToFloat]
	//Vector3に変換を行う
	//return: Vector3に変換したthis
	Vector3 Vector3Int::ToFloat() const
	{
		return Vector3(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
	}

	//Vector2Intオペレーター
	Vector3Int::operator Vector2Int() const { return Vector2Int(x, y); }
	//Vector4Intオペレーター
	Vector3Int::operator Vector4Int() const { return Vector4Int(x, y, z, 0); }

	//=オペレーター
	const Vector3Int& Vector3Int::operator=(const Vector2Int& vec)
	{
		this->x = vec.x;		this->y = vec.y;
		this->z = 0;
		return *this;
	}
	//=オペレーター
	const Vector3Int& Vector3Int::operator=(const Vector4Int& vec)
	{
		this->x = vec.x;		this->y = vec.y;
		this->z = vec.z;
		return *this;
	}
	//=オペレーター
	const Vector3Int& Vector3Int::operator = (const Vector3& vec)
	{
		this->x = static_cast<int>(vec.x);		this->y = static_cast<int>(vec.y);
		this->z = static_cast<int>(vec.z);
		return *this;
	}
	//=オペレーター
	const Vector3Int & Vector3Int::operator=(const Vector3Ref & vec)
	{
		this->x = static_cast<int>(vec.x);		this->y = static_cast<int>(vec.y);
		this->z = static_cast<int>(vec.z);
		return *this;
	}
}