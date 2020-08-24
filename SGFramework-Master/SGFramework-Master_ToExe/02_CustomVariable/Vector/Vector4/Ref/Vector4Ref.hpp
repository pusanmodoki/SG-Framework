/*----------------------------------------------------------------------------------
4つの値を扱えるVector4Ref (参照)構造体
最も多用する構造体シリーズの1つなので別自作Vectorが関係ないものは全てinline実装
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_VECTOR_VECTOR4_REF_HPP_
#define SGFRAMEWORK_HEADER_VECTOR_VECTOR4_REF_HPP_
#include <DirectXMath.h>
#include <iostream>
#include "../../../ConstAndUsing/ConstAndUsing.hpp"
#include "../../../Property/Property.hpp"
#include "../../../String/String.hpp"
#include "../Vector4.hpp"

//Framework namespace
namespace SGFramework
{
	//プロトタイプ宣言
	struct Vector3;
	struct Vector4;
	struct Vector2;
	struct Vector4Int;

	//4つの値を扱えるVector4Ref (参照)構造体
	struct Vector4Ref final
	{
	public:
		//----------------------------------------------------------------------------------
		//inlne実装

		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化する
		//引数1:x
		//引数2:y
		inline Vector4Ref(float& x, float& y, float& z, float& w);
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化する
		//引数1: XMFLOAT2
		inline Vector4Ref(DirectX::XMFLOAT4& value);

		//----------------------------------------------------------------------------------
		//[ToXmVector]
		//return: XmVector
		inline XmVector ToXmVector() const noexcept { return DirectX::XMLoadFloat4(&DirectX::XMFLOAT4(x, y, z, w)); }
		//----------------------------------------------------------------------------------
		//[ToRotationMatrix]
		//return: thisをQuaternionとした回転行列
		inline XmMatrix ToRotationMatrix() const noexcept { return DirectX::XMMatrixRotationQuaternion(this->ToXmVector()); }
		//----------------------------------------------------------------------------------
		//[ToRotationMatrix]
		//引数1: Apply
		inline XmMatrix ToRotationMatrix(XmMatrix& apply) const noexcept { apply = DirectX::XMMatrixRotationQuaternion(this->ToXmVector()); }
		//----------------------------------------------------------------------------------
		//[ToPhysX]
		//return: physx::PxVec4
		inline physx::PxVec4 ToPhysX() const noexcept { return physx::PxVec4(x, y, z, w); }

		//default
		Vector4Ref(const Vector4Ref&) = default;
		Vector4Ref& operator=(const Vector4Ref&) = default;
		Vector4Ref(Vector4Ref&&) = default;
		Vector4Ref& operator=(Vector4Ref&&) = default;

		//----------------------------------------------------------------------------------
		//変数
		float& x;		//X
		float& y;		//Y
		float& z;		//Z
		float& w;		//W

		//----------------------------------------------------------------------------------
		//[Absolute]
		//return: 絶対値
		inline constexpr Vector4 Absolute() const noexcept;
		//----------------------------------------------------------------------------------
		//[Magnitude]
		//return: 自身とVector(0,0)の距離
		inline constexpr float Magnitude() const noexcept;
		//----------------------------------------------------------------------------------
		//[ExactMagnitude]
		//return: 自身とVector(0,0,0,0)の距離 (正確な値だが負荷が高い)
		inline float ExactMagnitude() const noexcept;
		//----------------------------------------------------------------------------------
		//[Normalize]
		//return: 自身の正規化ベクトル
		inline constexpr Vector4 Normalized() const noexcept;

		//XMVECTORオペレーター
		inline operator XmVector() const noexcept;

		//+オペレーター  
		inline Vector4 operator +() const;
		//-オペレーター  
		inline Vector4 operator -() const;
		//==オペレーター
		inline bool operator ==(const Vector4Ref& vec) const;
		//!=オペレーター
		inline bool operator !=(const Vector4Ref& vec) const;

		//加算オペレーター
		inline Vector4 operator +(const Vector4Ref& vec) const;
		//減算オペレーター
		inline Vector4 operator -(const Vector4Ref& vec) const;
		//乗算オペレーター
		inline Vector4 operator *(const Vector4Ref& vec) const;
		//乗算オペレーター(float)
		inline Vector4 operator *(float value) const;
		//除算オペレーター
		inline Vector4 operator /(const Vector4Ref& vec) const;
		//除算オペレーター(float)
		inline Vector4 operator /(float value) const;

		//+=オペレーター
		inline const Vector4Ref &operator +=(const Vector4Ref& vec);
		//-=オペレーター
		inline const Vector4Ref &operator -=(const Vector4Ref& vec);
		//*=オペレーター
		inline const Vector4Ref &operator *=(const Vector4Ref& vec);
		//*=オペレーター(float)
		inline const Vector4Ref &operator *=(float value);
		// /=オペレーター
		inline const Vector4Ref &operator /=(const Vector4Ref& vec);
		// /=オペレーター(float)
		inline const Vector4Ref &operator /=(float value);

		//sgstring operator
		inline operator sgstring() const { return ToString(); }
		//----------------------------------------------------------------------------------
		//[ToString]
		//sgstring形式で出力する
		//return: sgstring表現したthis
		inline sgstring ToString() const { return (L"(" + std::to_wstring(x) + L", " + std::to_wstring(y) + L", " + std::to_wstring(z) + L")"); }
		//----------------------------------------------------------------------------------
		//[ToStdString]
		//std::string形式で出力する
		//return: std::string表現したthis
		inline std::string ToStdString() const { return ("(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ", " + std::to_string(w) + ")"); }

		//----------------------------------------------------------------------------------
		//Vector4

		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化する
		//引数1: Vector4
		inline Vector4Ref(Vector4& value);
		//----------------------------------------------------------------------------------
		//[ToVector4]
		//return: Vector4
		inline Vector4 ToVector4() const { return Vector4(*this); }

		//=オペレーター
		inline const Vector4Ref &operator =(const Vector4& vec);
		//==オペレーター
		inline bool operator ==(const Vector4& vec) const;
		//!=オペレーター
		inline bool operator !=(const Vector4& vec) const;

		//加算オペレーター
		inline Vector4 operator +(const Vector4& vec) const;
		//減算オペレーター
		inline Vector4 operator -(const Vector4& vec) const;
		//乗算オペレーター
		inline Vector4 operator *(const Vector4& vec) const;
		//除算オペレーター
		inline Vector4 operator /(const Vector4& vec) const;

		//+=オペレーター
		inline const Vector4Ref &operator +=(const Vector4& vec);
		//-=オペレーター
		inline const Vector4Ref &operator -=(const Vector4& vec);
		//*=オペレーター
		inline const Vector4Ref &operator *=(const Vector4& vec);
		// /=オペレーター
		inline const Vector4Ref &operator /=(const Vector4& vec);


		//=オペレーター
		inline const Vector4Ref &operator =(const DirectX::XMFLOAT4& vec);
		//==オペレーター
		inline bool operator ==(const DirectX::XMFLOAT4& vec) const;
		//!=オペレーター
		inline bool operator !=(const DirectX::XMFLOAT4& vec) const;

		//加算オペレーター
		inline Vector4 operator +(const DirectX::XMFLOAT4& vec) const;
		//減算オペレーター
		inline Vector4 operator -(const DirectX::XMFLOAT4& vec) const;
		//乗算オペレーター
		inline Vector4 operator *(const DirectX::XMFLOAT4& vec) const;
		//除算オペレーター
		inline Vector4 operator /(const DirectX::XMFLOAT4& vec) const;

		//+=オペレーター
		inline const Vector4Ref &operator +=(const DirectX::XMFLOAT4& vec);
		//-=オペレーター
		inline const Vector4Ref &operator -=(const DirectX::XMFLOAT4& vec);
		//*=オペレーター
		inline const Vector4Ref &operator *=(const DirectX::XMFLOAT4& vec);
		// /=オペレーター
		inline const Vector4Ref &operator /=(const DirectX::XMFLOAT4& vec);

		//----------------------------------------------------------------------------------
		//cpp実装

		//Vector2オペレーター
		operator Vector2() const;
		//Vector3オペレーター
		operator Vector3() const;
		//Vector4Intオペレーター
		operator Vector4Int() const;

		//=オペレーター
		const Vector4Ref& operator = (const Vector2& vec);
		//=オペレーター
		const Vector4Ref& operator = (const Vector3& vec);
		//=オペレーター
		const Vector4Ref& operator = (const Vector4Int& vec);
	};


	//----------------------------------------------------------------------------------
	//[wostream operator]
	//引数1: stream
	//引数2: this
	inline std::wostream& operator<<(std::wostream& stream, const Vector4Ref& value)
	{
		const wchar* string = value.ToString();
		stream << string;
		return stream;
	}

	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//代入初期化する
	//引数1:x
	//引数2:y
	inline Vector4Ref::Vector4Ref(float& x, float& y, float& z, float& w) : x(x), y(y), z(z), w(w) {};
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//代入初期化する
	//引数1: XMFLOAT2
	inline Vector4Ref::Vector4Ref(DirectX::XMFLOAT4 & vec) : x(vec.x), y(vec.y), z(vec.z), w(vec.w) { }

	//----------------------------------------------------------------------------------
	//[Absolute]
	//return: 絶対値
	inline constexpr Vector4 Vector4Ref::Absolute() const noexcept
	{
		return Vector4(x < 0.0f ? -x : x, y < 0.0f ? -y : y, z < 0.0f ? -z : z, w < 0.0f ? -w : w);
	}
	//----------------------------------------------------------------------------------
	//[Magnitude]
	//return: 自身とVector(0,0)の距離
	inline constexpr float Vector4Ref::Magnitude() const noexcept
	{
		float f = x * x + y * y + z * z + w * w;
		int i = 0x5f3759df - (*reinterpret_cast<int*>(&f) >> 1);
		float bitF = *reinterpret_cast<float*>(&i);

		bitF *= (1.5f - 0.5f * f * bitF * bitF);
		return f * bitF * (1.5f - 0.5f * f * bitF * bitF);
	}
	//----------------------------------------------------------------------------------
	//[ExactMagnitude]
	//return: 自身とVector(0,0,0,0)の距離 (正確な値だが負荷が高い)
	inline float Vector4Ref::ExactMagnitude() const noexcept
	{
		return std::sqrtf(x * x + y * y + z * z + w * w);
	}
	//----------------------------------------------------------------------------------
	//[Normalize]
	//return: 自身の正規化ベクトル
	inline constexpr Vector4 Vector4Ref::Normalized() const noexcept
	{
		float divLength = 1.0f / Magnitude();
		return Vector4(x * divLength, y * divLength, z * divLength, w * divLength);
	}

	//XMVECTORオペレーター
	inline Vector4Ref::operator XmVector() const noexcept { return DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(x, y, z)); }

	//+オペレーター  
	inline Vector4 Vector4Ref::operator+() const { return Vector4(+this->x, +this->y, +this->z, +this->w); }
	//-オペレーター  
	inline Vector4 Vector4Ref::operator-() const { return Vector4(-this->x, -this->y, -this->z, -this->w); }
	//==オペレーター
	inline bool Vector4Ref::operator==(const Vector4Ref& vec) const
	{
		return (fabsf(x - vec.x) <= MathAF::cEpsilon
			&& fabsf(y - vec.y) <= MathAF::cEpsilon
			&& fabsf(z - vec.z) <= MathAF::cEpsilon
			&& fabsf(w - vec.w) <= MathAF::cEpsilon);
	}
	//!=オペレーター
	inline bool Vector4Ref::operator!=(const Vector4Ref& vec) const
	{
		return (fabsf(x - vec.x) <= MathAF::cEpsilon
			&& fabsf(y - vec.y) <= MathAF::cEpsilon
			&& fabsf(z - vec.z) <= MathAF::cEpsilon
			&& fabsf(w - vec.w) <= MathAF::cEpsilon);
	}

	//加算オペレーター
	inline Vector4 Vector4Ref::operator+(const Vector4Ref& vec) const { return Vector4(x + vec.x, y + vec.y, z + vec.z, w + vec.w); }
	//減算オペレーター
	inline Vector4 Vector4Ref::operator-(const Vector4Ref& vec) const { return Vector4(x - vec.x, y - vec.y, z - vec.z, w - vec.w); }
	//乗算オペレーター
	inline Vector4 Vector4Ref::operator*(const Vector4Ref& vec) const { return Vector4(x * vec.x, y * vec.y, z * vec.z, w * vec.w); }
	//乗算オペレーター(float)
	inline Vector4 Vector4Ref::operator*(const float value) const { return Vector4(x * value, y * value, z * value, w * value); }
	//除算オペレーター
	inline Vector4 Vector4Ref::operator/(const Vector4Ref& vec) const { return Vector4(x / vec.x, y / vec.y, z / vec.z, w / vec.w); }
	//除算オペレーター(float)
	inline Vector4 Vector4Ref::operator/(const float value) const { return Vector4(x / value, y / value, z / value, w / value); }

	//+=オペレーター
	inline const Vector4Ref & Vector4Ref::operator+=(const Vector4Ref& vec)
	{
		this->x += vec.x;		this->y += vec.y;
		this->z += vec.z;		this->w += vec.w;
		return *this;
	}
	//-=オペレーター
	inline const Vector4Ref & Vector4Ref::operator-=(const Vector4Ref& vec)
	{
		this->x -= vec.x;		this->y -= vec.y;
		this->z -= vec.z;		this->w -= vec.w;
		return *this;
	}
	//*=オペレーター
	inline const Vector4Ref & Vector4Ref::operator*=(const Vector4Ref& vec)
	{
		this->x *= vec.x;		this->y *= vec.y;
		this->z *= vec.z;		this->w *= vec.w;
		return *this;
	}
	//*=オペレーター(float)
	inline const Vector4Ref & Vector4Ref::operator*=(const float value)
	{
		this->x *= value;		this->y *= value;
		this->z *= value;		this->w *= value;
		return *this;
	}
	// /=オペレーター
	inline const Vector4Ref & Vector4Ref::operator/=(const Vector4Ref& vec)
	{
		this->x /= vec.x;		this->y /= vec.y;
		this->z /= vec.z;		this->w /= vec.w;
		return *this;
	}
	// /=オペレーター(float)
	inline const Vector4Ref & Vector4Ref::operator/=(const float value)
	{
		this->x /= value;		this->y /= value;
		this->z /= value;		this->w /= value;
		return *this;
	}

	//----------------------------------------------------------------------------------
	//Vector4

	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//代入初期化する
	//引数1: Vector4
	Vector4Ref::Vector4Ref(Vector4 & value) : x(value.x), y(value.y), z(value.z), w(value.w) {}

	//=オペレーター
	inline const Vector4Ref & Vector4Ref::operator=(const Vector4 & vec)
	{
		x = vec.x; y = vec.y;
		z = vec.z; w = vec.w;
		return *this;
	}

	//==オペレーター
	inline bool Vector4Ref::operator==(const Vector4 & vec) const
	{
		return (fabsf(x - vec.x) <= MathAF::cEpsilon
			&& fabsf(y - vec.y) <= MathAF::cEpsilon
			&& fabsf(z - vec.z) <= MathAF::cEpsilon
			&& fabsf(w - vec.w) <= MathAF::cEpsilon);
	}
	//!=オペレーター
	inline bool Vector4Ref::operator!=(const Vector4 & vec) const
	{
		return (fabsf(x - vec.x) <= MathAF::cEpsilon
			&& fabsf(y - vec.y) <= MathAF::cEpsilon
			&& fabsf(z - vec.z) <= MathAF::cEpsilon
			&& fabsf(w - vec.w) <= MathAF::cEpsilon);
	}

	//加算オペレーター
	inline Vector4 Vector4Ref::operator+(const Vector4 & vec) const { return Vector4(x + vec.x, y + vec.y, z + vec.z, w + vec.w); }
	//減算オペレーター
	inline Vector4 Vector4Ref::operator-(const Vector4 & vec) const { return Vector4(x - vec.x, y - vec.y, z - vec.z, w - vec.w); }
	//乗算オペレーター
	inline Vector4 Vector4Ref::operator*(const Vector4 & vec) const { return Vector4(x * vec.x, y * vec.y, z * vec.z, w * vec.w); }
	//除算オペレーター
	inline Vector4 Vector4Ref::operator/(const Vector4 & vec) const { return Vector4(x / vec.x, y / vec.y, z / vec.z, w / vec.w); }

	//+=オペレーター
	inline const Vector4Ref & Vector4Ref::operator+=(const Vector4 & vec)
	{
		this->x += vec.x;		this->y += vec.y;
		this->z += vec.z;		this->w += vec.w;
		return *this;
	}
	//-=オペレーター
	inline const Vector4Ref & Vector4Ref::operator-=(const Vector4 & vec)
	{
		this->x -= vec.x;		this->y -= vec.y;
		this->z -= vec.z;		this->w -= vec.w;
		return *this;
	}
	//*=オペレーター
	inline const Vector4Ref & Vector4Ref::operator*=(const Vector4 & vec)
	{
		this->x *= vec.x;		this->y *= vec.y;
		this->z *= vec.z;		this->w *= vec.w;
		return *this;
	}
	// /=オペレーター
	inline const Vector4Ref & Vector4Ref::operator/=(const Vector4 & vec)
	{
		this->x /= vec.x;		this->y /= vec.y;
		this->z /= vec.z;		this->w /= vec.w;
		return *this;
	}


	//=オペレーター
	inline const Vector4Ref & Vector4Ref::operator=(const DirectX::XMFLOAT4 & vec)
	{
		x = vec.x; y = vec.y;
		z = vec.z; w = vec.w;
		return *this;
	}

	//==オペレーター
	inline bool Vector4Ref::operator==(const DirectX::XMFLOAT4 & vec) const
	{
		return (fabsf(x - vec.x) <= MathAF::cEpsilon
			&& fabsf(y - vec.y) <= MathAF::cEpsilon
			&& fabsf(z - vec.z) <= MathAF::cEpsilon
			&& fabsf(w - vec.w) <= MathAF::cEpsilon);
	}
	//!=オペレーター
	inline bool Vector4Ref::operator!=(const DirectX::XMFLOAT4 & vec) const
	{
		return (fabsf(x - vec.x) <= MathAF::cEpsilon
			&& fabsf(y - vec.y) <= MathAF::cEpsilon
			&& fabsf(z - vec.z) <= MathAF::cEpsilon
			&& fabsf(w - vec.w) <= MathAF::cEpsilon);
	}

	//加算オペレーター
	inline Vector4 Vector4Ref::operator+(const DirectX::XMFLOAT4 & vec) const { return Vector4(x + vec.x, y + vec.y, z + vec.z, w + vec.w); }
	//減算オペレーター
	inline Vector4 Vector4Ref::operator-(const DirectX::XMFLOAT4 & vec) const { return Vector4(x - vec.x, y - vec.y, z - vec.z, w - vec.w); }
	//乗算オペレーター
	inline Vector4 Vector4Ref::operator*(const DirectX::XMFLOAT4 & vec) const { return Vector4(x * vec.x, y * vec.y, z * vec.z, w * vec.w); }
	//除算オペレーター
	inline Vector4 Vector4Ref::operator/(const DirectX::XMFLOAT4 & vec) const { return Vector4(x / vec.x, y / vec.y, z / vec.z, w / vec.w); }

	//+=オペレーター
	inline const Vector4Ref & Vector4Ref::operator+=(const DirectX::XMFLOAT4 & vec)
	{
		this->x += vec.x;		this->y += vec.y;
		this->z += vec.z;		this->w += vec.w;
		return *this;
	}
	//-=オペレーター
	inline const Vector4Ref & Vector4Ref::operator-=(const DirectX::XMFLOAT4 & vec)
	{
		this->x -= vec.x;		this->y -= vec.y;
		this->z -= vec.z;		this->w -= vec.w;
		return *this;
	}
	//*=オペレーター
	inline const Vector4Ref & Vector4Ref::operator*=(const DirectX::XMFLOAT4 & vec)
	{
		this->x *= vec.x;		this->y *= vec.y;
		this->z *= vec.z;		this->w *= vec.w;
		return *this;
	}
	// /=オペレーター
	inline const Vector4Ref & Vector4Ref::operator/=(const DirectX::XMFLOAT4 & vec)
	{
		this->x /= vec.x;		this->y /= vec.y;
		this->z /= vec.z;		this->w /= vec.w;
		return *this;
	}
}
#endif // !SGFRAMEWORK_HEADER_VECTOR_VECTOR3_REF_HPP_
