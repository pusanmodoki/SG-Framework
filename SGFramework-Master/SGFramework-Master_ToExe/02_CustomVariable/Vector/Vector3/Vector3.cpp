/*----------------------------------------------------------------------------------
3つの値を扱えるVector3 構造体
最も多用する構造体シリーズの1つなので別自作Vectorが関係ないものは全てinline実装
	→関数を記載したVector3.cpp
------------------------------------------------------------------------------------*/
#include <math.h>
#include "Vector3.hpp"
#include "Int/Vector3Int.hpp"
#include "Ref/Vector3Ref.hpp"
#include "../Vector2/Vector2.hpp"
#include "../Vector4/Vector4.hpp"
#include "../../Quarernion/Quaternion.hpp"

//Framework namespace
namespace SGFramework
{
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//代入初期化する
	//引数1: Vector3Int
	Vector3::Vector3(const Vector3Int & vec)
	{
		this->x = static_cast<float>(vec.x);		this->y = static_cast<float>(vec.y);
		this->z = static_cast<float>(vec.z);
	}

	//Vector2オペレーター
	Vector3::operator Vector2() const { return Vector2(x, y); }
	//Vector4オペレーター
	Vector3::operator Vector4() const { return Vector4(x, y, z, 0.0f); }
	//Vector3Intオペレーター
	Vector3::operator Vector3Int() const 
	{ return Vector3Int(static_cast<int>(x), static_cast<int>(y), static_cast<int>(z)); }

	//=オペレーター
	const Vector3& Vector3::operator=(const Vector2& vec)
	{
		this->x = vec.x;		this->y = vec.y;
		this->z = 0.0f;
		return *this;
	}
	//=オペレーター
	const Vector3& Vector3::operator=(const Vector4& vec)
	{
		this->x = vec.x;		this->y = vec.y;
		this->z = vec.z;
		return *this;
	}
	//=オペレーター
	const Vector3& Vector3::operator = (const Vector3Int& vec)
	{
		this->x = static_cast<float>(vec.x);		this->y = static_cast<float>(vec.y);
		this->z = static_cast<float>(vec.z);
		return *this;
	}
	//*オペレーター
	Vector3 Vector3::operator * (const Quaternion& quaternion) const
	{
		return DirectX::XMVector3Rotate(*this, quaternion);
	}
	//*=オペレーター
	const Vector3& Vector3::operator *= (const Quaternion& quaternion)
	{
		*this = DirectX::XMVector3Rotate(*this, quaternion);
		return *this;
	}


	//----------------------------------------------------------------------------------
	//Vector3Ref

	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//代入初期化する
	//引数1: Vector3Ref
	Vector3::Vector3(const Vector3Ref & value) : DirectX::XMFLOAT3(value.x, value.y, value.z) {}
	
	//=オペレーター
	const Vector3 & Vector3::operator=(const Vector3Ref & vec)
	{
		x = vec.x; y = vec.y;
		z = vec.z;
		return *this;
	}


	//==オペレーター
	bool Vector3::operator ==(const Vector3Ref& vec) const
	{
		return (fabsf(x - vec.x) <= MathAF::cEpsilon
			&& fabsf(y - vec.y) <= MathAF::cEpsilon
			&& fabsf(z - vec.z) <= MathAF::cEpsilon);
	}
	//!=オペレーター
	bool Vector3::operator !=(const Vector3Ref& vec) const
	{
		return (fabsf(x - vec.x) <= MathAF::cEpsilon
			&& fabsf(y - vec.y) <= MathAF::cEpsilon
			&& fabsf(z - vec.z) <= MathAF::cEpsilon);
	}

	//加算オペレーター
	Vector3 Vector3::operator +(const Vector3Ref& vec) const
	{
		return Vector3(x + vec.x, y + vec.y, z + vec.z);
	}
	//減算オペレーター
	Vector3 Vector3::operator -(const Vector3Ref& vec) const
	{
		return Vector3(x - vec.x, y - vec.y, z - vec.z);
	}
	//乗算オペレーター
	Vector3 Vector3::operator *(const Vector3Ref& vec) const
	{
		return Vector3(x * vec.x, y * vec.y, z * vec.z);
	}
	//除算オペレーター
	Vector3 Vector3::operator /(const Vector3Ref& vec) const
	{
		return Vector3(x / vec.x, y / vec.y, z / vec.z);
	}

	//+=オペレーター
	const Vector3& Vector3::operator +=(const Vector3Ref& vec)
	{
		this->x += vec.x;		this->y += vec.y;
		this->z += vec.z;
		return *this;
	}
	//-=オペレーター
	const Vector3& Vector3::operator -=(const Vector3Ref& vec)
	{
		this->x -= vec.x;		this->y -= vec.y;
		this->z -= vec.z;
		return *this;
	}
	//*=オペレーター
	const Vector3& Vector3::operator *=(const Vector3Ref& vec)
	{
		this->x *= vec.x;		this->y *= vec.y;
		this->z *= vec.z;
		return *this;
	}
	// /=オペレーター
	const Vector3& Vector3::operator /=(const Vector3Ref& vec)
	{
		this->x /= vec.x;		this->y /= vec.y;
		this->z /= vec.z;
		return *this;
	}
}