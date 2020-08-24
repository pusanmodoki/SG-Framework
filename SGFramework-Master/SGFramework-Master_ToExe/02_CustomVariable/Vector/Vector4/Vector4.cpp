/*----------------------------------------------------------------------------------
4つの値を扱えるVector4 構造体
最も多用する構造体シリーズの1つなので別自作Vectorが関係ないものは全てinline実装
→関数, inline関数を実装するVector4.cpp	
------------------------------------------------------------------------------------*/
#include "Vector4.hpp"
#include "Int/Vector4Int.hpp"
#include "Ref/Vector4Ref.hpp"
#include "../Vector2/Vector2.hpp"
#include "../Vector3/Vector3.hpp"

//Framework namespace
namespace SGFramework
{
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//代入初期化する
	//引数1: Vector4Int
	Vector4::Vector4(const Vector4Int & vec)
	{
		this->x = static_cast<float>(vec.x);		this->y = static_cast<float>(vec.y);
		this->z = static_cast<float>(vec.z);		this->w = static_cast<float>(vec.w);
	}

	//----------------------------------------------------------------------------------
	//[setVector3]
	//Vector4(vec.x, vec.y, vec.z, w)の距離
	//引数1: Vector3
	//引数2: w, default = 0.0f
	void Vector4::setVector3(const Vector3 & vector3, float w)
	{
		this->x = vector3.x;
		this->y = vector3.y;
		this->z = vector3.z;
		this->w = w;
	}

	//Vector2オペレーター
	Vector4::operator Vector2() const { return Vector2(x, y); }
	//Vector3オペレーター
	Vector4::operator Vector3() const { return Vector3(x, y, z); }
	//Vector4Intオペレーター
	Vector4::operator Vector4Int() const { return Vector4Int(static_cast<int>(x), 
		static_cast<int>(y), static_cast<int>(z), static_cast<int>(w)); }

	//=オペレーター
	const Vector4& Vector4::operator=(const Vector2& vec)
	{
		this->x = vec.x;		this->y = vec.y;
		this->z = 0.0f;		this->w = 0.0f;
		return *this;
	}
	//=オペレーター
	const Vector4& Vector4::operator=(const Vector3& vec)
	{
		this->x = vec.x;		this->y = vec.y;
		this->z = vec.z;		this->w = 0.0f;
		return *this;
	}
	//=オペレーター
	const Vector4& Vector4::operator = (const Vector4Int& vec)
	{
		this->x = static_cast<float>(vec.x);		this->y = static_cast<float>(vec.y);
		this->z = static_cast<float>(vec.z);		this->w = static_cast<float>(vec.w);
		return *this;
	}



	//----------------------------------------------------------------------------------
	//Vector4Ref

	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//代入初期化する
	//引数1: Vector4Ref
	Vector4::Vector4(const Vector4Ref & value) : DirectX::XMFLOAT4(value.x, value.y, value.z, value.w) {}

	//=オペレーター
	const Vector4 & Vector4::operator=(const Vector4Ref & vec)
	{
		x = vec.x;		y = vec.y;
		z = vec.z;		w = vec.w;
		return *this;
	}

	//==オペレーター
	bool Vector4::operator ==(const Vector4Ref& vec) const
	{
		return (fabsf(x - vec.x) <= MathAF::cEpsilon
			&& fabsf(y - vec.y) <= MathAF::cEpsilon
			&& fabsf(z - vec.z) <= MathAF::cEpsilon
			&& fabsf(w - vec.w) <= MathAF::cEpsilon);
	}
	//!=オペレーター
	bool Vector4::operator !=(const Vector4Ref& vec) const
	{
		return (fabsf(x - vec.x) <= MathAF::cEpsilon
			&& fabsf(y - vec.y) <= MathAF::cEpsilon
			&& fabsf(z - vec.z) <= MathAF::cEpsilon
			&& fabsf(w - vec.w) <= MathAF::cEpsilon);
	}

	//加算オペレーター
	Vector4 Vector4::operator +(const Vector4Ref& vec) const
	{
		return Vector4(x + vec.x, y + vec.y, z + vec.z, w + vec.w);
	}
	//減算オペレーター
	Vector4 Vector4::operator -(const Vector4Ref& vec) const
	{
		return Vector4(x - vec.x, y - vec.y, z - vec.z, w - vec.w);
	}
	//乗算オペレーター
	Vector4 Vector4::operator *(const Vector4Ref& vec) const
	{
		return Vector4(x * vec.x, y * vec.y, z * vec.z, w * vec.w);
	}
	//除算オペレーター
	Vector4 Vector4::operator /(const Vector4Ref& vec) const
	{
		return Vector4(x / vec.x, y / vec.y, z / vec.z, w / vec.w);
	}

	//+=オペレーター
	const Vector4& Vector4::operator +=(const Vector4Ref& vec)
	{
		this->x += vec.x;		this->y += vec.y;
		this->z += vec.z;		this->w += vec.w;
		return *this;
	}
	//-=オペレーター
	const Vector4& Vector4::operator -=(const Vector4Ref& vec)
	{
		this->x -= vec.x;		this->y -= vec.y;
		this->z -= vec.z;		this->w -= vec.w;
		return *this;
	}
	//*=オペレーター
	const Vector4& Vector4::operator *=(const Vector4Ref& vec)
	{
		this->x *= vec.x;		this->y *= vec.y;
		this->z *= vec.z;		this->w *= vec.w;
		return *this;
	}
	// /=オペレーター
	const Vector4& Vector4::operator /=(const Vector4Ref& vec)
	{
		this->x /= vec.x;		this->y /= vec.y;
		this->z /= vec.z;		this->w /= vec.w;
		return *this;
	}
}