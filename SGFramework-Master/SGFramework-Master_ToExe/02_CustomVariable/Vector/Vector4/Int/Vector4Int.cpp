/*----------------------------------------------------------------------------------
4つのint値を扱えるVector4Int 構造体
最も多用する構造体シリーズの1つなので別自作Vectorが関係ないものは全てinline実装
	→関数を記述したVector4Int.cpp
------------------------------------------------------------------------------------*/
#include "Vector4Int.hpp"
#include "../../Vector2/Int/Vector2Int.hpp"
#include "../../Vector3/Int/Vector3Int.hpp"
#include "../Vector4.hpp"
#include "../Ref/Vector4Ref.hpp"

//Framework namespace
namespace SGFramework
{
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//代入初期化する
	//引数1: Vector4
	Vector4Int::Vector4Int(const Vector4 & vec)
	{
		this->x = static_cast<int>(vec.x);		this->y = static_cast<int>(vec.y);
		this->z = static_cast<int>(vec.z);		this->w = static_cast<int>(vec.w);
	}

	//----------------------------------------------------------------------------------
	//[ToFloat]
	//Vector4に変換を行う
	//return: Vector4に変換したthis
	Vector4 Vector4Int::ToFloat() const
	{
		return Vector4(static_cast<float>(x), static_cast<float>(y),
			static_cast<float>(z), static_cast<float>(w));
	}

	//Vector2Intオペレーター
	Vector4Int::operator Vector2Int() const { return Vector2Int(x, y); }
	//Vector3Intオペレーター
	Vector4Int::operator Vector3Int() const { return Vector3Int(x, y, z); }

	//=オペレーター
	const Vector4Int& Vector4Int::operator=(const Vector2Int& vec)
	{
		this->x = vec.x;		this->y = vec.y;
		this->z = 0;		this->w = 0;
		return *this;
	}
	//=オペレーター
	const Vector4Int& Vector4Int::operator=(const Vector3Int& vec)
	{
		this->x = vec.x;		this->y = vec.y;
		this->z = vec.z;		this->w = 0;
		return *this;
	}
	//=オペレーター
	const Vector4Int& Vector4Int::operator = (const Vector4& vec)
	{
		this->x = static_cast<int>(vec.x); this->y = static_cast<int>(vec.y);
		this->z = static_cast<int>(vec.z); this->w = static_cast<int>(vec.w);
		return *this;
	}
	//=オペレーター
	const Vector4Int & Vector4Int::operator=(const Vector4Ref & vec)
	{
		this->x = static_cast<int>(vec.x); this->y = static_cast<int>(vec.y);
		this->z = static_cast<int>(vec.z); this->w = static_cast<int>(vec.w);
		return *this;
	}
}