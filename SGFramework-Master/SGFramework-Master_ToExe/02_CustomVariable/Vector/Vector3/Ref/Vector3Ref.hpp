/*----------------------------------------------------------------------------------
3つの値を扱えるVector3Ref (参照)構造体
最も多用する構造体シリーズの1つなので別自作Vectorが関係ないものは全てinline実装
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_VECTOR_VECTOR3_REF_HPP_
#define SGFRAMEWORK_HEADER_VECTOR_VECTOR3_REF_HPP_
#include <DirectXMath.h>
#include <iostream>
#include "../../../ConstAndUsing/ConstAndUsing.hpp"
#include "../../../Property/Property.hpp"
#include "../../../String/String.hpp"
#include "../Vector3.hpp"

//Framework namespace
namespace SGFramework
{
	//プロトタイプ宣言
	struct Vector3;
	struct Vector4;
	struct Vector2;
	struct Vector3Int;

	//3つの値を扱えるVector3Ref (参照)構造体
	struct Vector3Ref final
	{
	public:
		//----------------------------------------------------------------------------------
		//inlne実装

		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化する
		//引数1:x
		//引数2:y
		inline Vector3Ref(float& x, float& y, float& z);
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化する
		//引数1: XMFLOAT2
		inline Vector3Ref(DirectX::XMFLOAT3& value);

		//----------------------------------------------------------------------------------
		//[ToXmVector]
		//return: XmVector
		inline XmVector ToXmVector() const noexcept { return DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(x, y, z)); }
		//----------------------------------------------------------------------------------
		//[ToPhysX]
		//return: physx::PxVec3
		inline physx::PxVec3 ToPhysX() const noexcept { return physx::PxVec3(x, y, z); }
		//----------------------------------------------------------------------------------
		//[ToAssimp]
		//return: aiVector3D
		inline aiVector3D ToAssimp() const noexcept { return aiVector3D(x, y, z); }

		//default
		Vector3Ref(const Vector3Ref&) = default;
		Vector3Ref& operator=(const Vector3Ref&) = default;
		Vector3Ref(Vector3Ref&&) = default;
		Vector3Ref& operator=(Vector3Ref&&) = default;

		//----------------------------------------------------------------------------------
		//変数
		float& x;		//X
		float& y;		//Y
		float& z;		//Z

		//----------------------------------------------------------------------------------
		//[Absolute]
		//return: 絶対値
		inline Vector3 Absolute() const noexcept;
		//----------------------------------------------------------------------------------
		//[Magnitude]
		//return: 自身とVector(0,0,0)の距離
		inline constexpr float Magnitude() const noexcept;
		//----------------------------------------------------------------------------------
		//[ExactMagnitude]
		//return: 自身とVector(0,0,0)の距離 (正確な値だが負荷が高い)
		inline float ExactMagnitude() const noexcept;
		//----------------------------------------------------------------------------------
		//[ClampMagnitude]
		//LengthをmaxLengthに制限する
		//return: 制限したvector
		//引数1: 制限Length
		inline Vector3 ClampMagnitude(const float maxLength) const noexcept;
		//----------------------------------------------------------------------------------
		//[Normalize]
		//return: 自身を正規化したベクトル
		inline Vector3 Normalized() const noexcept;
		//----------------------------------------------------------------------------------
		//[Orthogonal]
		//return: 自身のベクトルの直交ベクトル
		inline Vector3 Orthogonal() const noexcept;
		//----------------------------------------------------------------------------------
		//[TransformCoord]
		//Matrixを付与した値に変換する
		//return: Matrixを付与したベクトル
		//引数1: 対象Matrix
		inline Vector3 TransformCoord(const DirectX::XMMATRIX& matrix) const noexcept;
		//----------------------------------------------------------------------------------
		//[Rotate]
		//return: 自身のベクトルの回転ベクトル
		inline Vector3 Rotate(const XmVector& quaternion) const noexcept;
		//----------------------------------------------------------------------------------
		//[ProgectOnPlane]
		//vecを平面上ベクトルに射影する
		//return: 射影済ベクトル
		//引数1: 平面ベクトルの法線(正規化済のもの)
		inline Vector3 ProgectOnPlane(const Vector3& normal) const noexcept;

		//XMVECTORオペレーター
		inline operator XmVector() const noexcept;

		//+オペレーター  
		inline Vector3 operator +() const;
		//-オペレーター  
		inline Vector3 operator -() const;
		//==オペレーター
		inline bool operator ==(const Vector3Ref& vec) const;
		//!=オペレーター
		inline bool operator !=(const Vector3Ref& vec) const;

		//加算オペレーター
		inline Vector3 operator +(const Vector3Ref& vec) const;
		//減算オペレーター
		inline Vector3 operator -(const Vector3Ref& vec) const;
		//乗算オペレーター
		inline Vector3 operator *(const Vector3Ref& vec) const;
		//乗算オペレーター(float)
		inline Vector3 operator *(float value) const;
		//除算オペレーター
		inline Vector3 operator /(const Vector3Ref& vec) const;
		//除算オペレーター(float)
		inline Vector3 operator /(float value) const;

		//+=オペレーター
		inline const Vector3Ref &operator +=(const Vector3Ref& vec);
		//-=オペレーター
		inline const Vector3Ref &operator -=(const Vector3Ref& vec);
		//*=オペレーター
		inline const Vector3Ref &operator *=(const Vector3Ref& vec);
		//*=オペレーター(float)
		inline const Vector3Ref &operator *=(float value);
		// /=オペレーター
		inline const Vector3Ref &operator /=(const Vector3Ref& vec);
		// /=オペレーター(float)
		inline const Vector3Ref &operator /=(float value);

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
		inline std::string ToStdString() const { return ("(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")"); }

		//----------------------------------------------------------------------------------
		//Vector3

		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化する
		//引数1: Vector3
		inline Vector3Ref(Vector3& value);
		//----------------------------------------------------------------------------------
		//[ToVector3]
		//return: Vector3
		inline Vector3 ToVector3() const { return Vector3(*this); }

		//=オペレーター
		inline const Vector3Ref &operator =(const Vector3& vec);
		//==オペレーター
		inline bool operator ==(const Vector3& vec) const;
		//!=オペレーター
		inline bool operator !=(const Vector3& vec) const;

		//加算オペレーター
		inline Vector3 operator +(const Vector3& vec) const;
		//減算オペレーター
		inline Vector3 operator -(const Vector3& vec) const;
		//乗算オペレーター
		inline Vector3 operator *(const Vector3& vec) const;
		//除算オペレーター
		inline Vector3 operator /(const Vector3& vec) const;

		//+=オペレーター
		inline const Vector3Ref &operator +=(const Vector3& vec);
		//-=オペレーター
		inline const Vector3Ref &operator -=(const Vector3& vec);
		//*=オペレーター
		inline const Vector3Ref &operator *=(const Vector3& vec);
		// /=オペレーター
		inline const Vector3Ref &operator /=(const Vector3& vec);



		//=オペレーター
		inline const Vector3Ref &operator =(const DirectX::XMFLOAT3& vec);
		//==オペレーター
		inline bool operator ==(const DirectX::XMFLOAT3& vec) const;
		//!=オペレーター
		inline bool operator !=(const DirectX::XMFLOAT3& vec) const;

		//加算オペレーター
		inline Vector3 operator +(const DirectX::XMFLOAT3& vec) const;
		//減算オペレーター
		inline Vector3 operator -(const DirectX::XMFLOAT3& vec) const;
		//乗算オペレーター
		inline Vector3 operator *(const DirectX::XMFLOAT3& vec) const;
		//除算オペレーター
		inline Vector3 operator /(const DirectX::XMFLOAT3& vec) const;

		//+=オペレーター
		inline const Vector3Ref &operator +=(const DirectX::XMFLOAT3& vec);
		//-=オペレーター
		inline const Vector3Ref &operator -=(const DirectX::XMFLOAT3& vec);
		//*=オペレーター
		inline const Vector3Ref &operator *=(const DirectX::XMFLOAT3& vec);
		// /=オペレーター
		inline const Vector3Ref &operator /=(const DirectX::XMFLOAT3& vec);


		//----------------------------------------------------------------------------------
		//cpp実装

		//Vector2オペレーター
		operator Vector2() const;
		//Vector4オペレーター
		operator Vector4() const;
		//Vector3Intオペレーター
		operator Vector3Int() const;

		//=オペレーター
		const Vector3Ref& operator = (const Vector2& vec);
		//=オペレーター
		const Vector3Ref& operator = (const Vector4& vec);
		//=オペレーター
		const Vector3Ref& operator = (const Vector3Int& vec);
	};


	//----------------------------------------------------------------------------------
	//[wostream operator]
	//引数1: stream
	//引数2: this
	inline std::wostream& operator<<(std::wostream& stream, const Vector3Ref& value)
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
	inline Vector3Ref::Vector3Ref(float& x, float& y, float& z) : x(x), y(y), z(z) {};
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//代入初期化する
	//引数1: XMFLOAT2
	inline Vector3Ref::Vector3Ref(DirectX::XMFLOAT3 & vec) : x(vec.x), y(vec.y), z(vec.z) { }

	//----------------------------------------------------------------------------------
	//[Absolute]
	//return: 絶対値
	inline Vector3 Vector3Ref::Absolute() const noexcept
	{
		return Vector3(x < 0.0f ? -x : x, y < 0.0f ? -y : y, z < 0.0f ? -z : z);
	}
	//----------------------------------------------------------------------------------
	//[Magnitude]
	//return: 自身とVector(0,0,0)の距離
	inline constexpr float Vector3Ref::Magnitude() const noexcept
	{
		float f = x * x + y * y + z * z;
		int i = 0x5f3759df - (*reinterpret_cast<int*>(&f) >> 1);
		float bitF = *reinterpret_cast<float*>(&i);

		bitF *= (1.5f - 0.5f * f * bitF * bitF);
		return f * bitF * (1.5f - 0.5f * f * bitF * bitF);
	}
	//----------------------------------------------------------------------------------
	//[ExactMagnitude]
	//return: 自身とVector(0,0,0)の距離 (正確な値だが負荷が高い)
	inline float Vector3Ref::ExactMagnitude() const noexcept
	{
		return std::sqrtf(x * x + y * y + z * z);
	}
	//----------------------------------------------------------------------------------
	//[ClampMagnitude]
	//LengthをmaxLengthに制限する
	//return: 制限したvector
	//引数1: 制限Length
	inline Vector3 Vector3Ref::ClampMagnitude(const float maxLength) const noexcept
	{
		return std::move(DirectX::XMVector3ClampLength(this->ToXmVector(), 0.0f, maxLength));
	}
	//----------------------------------------------------------------------------------
	//[Normalize]
	//return: 自身を正規化したベクトル
	inline Vector3 Vector3Ref::Normalized() const noexcept
	{
		float divLength = 1.0f / ExactMagnitude();
		return Vector3(x * divLength, y * divLength, z * divLength);
	}
	//----------------------------------------------------------------------------------
	//[Orthogonal]
	//return: 自身のベクトルの直交ベクトル
	inline Vector3 Vector3Ref::Orthogonal() const noexcept { return std::move(DirectX::XMVector3Orthogonal(this->ToXmVector())); }
	//----------------------------------------------------------------------------------
	//[TransformCoord]
	//Matrixを付与した値に変換する
	//return: Matrixを付与したベクトル
	//引数1: 対象Matrix
	inline Vector3 Vector3Ref::TransformCoord(const DirectX::XMMATRIX & matrix) const noexcept
	{
		return DirectX::XMVector3TransformCoord(this->ToXmVector(), matrix);
	}
	//----------------------------------------------------------------------------------
	//[Rotate]
	//return: 自身のベクトルの回転ベクトル
	inline Vector3 Vector3Ref::Rotate(const XmVector&  quaternion) const noexcept
	{
		return std::move(DirectX::XMVector3Rotate(this->ToXmVector(), quaternion));
	}
	//----------------------------------------------------------------------------------
	//[ProgectOnPlane]
	//vecを平面上ベクトルに射影する
	//return: 射影済ベクトル
	//引数1: 平面ベクトルの法線(正規化済のもの)
	inline Vector3 Vector3Ref::ProgectOnPlane(const Vector3 & normal) const noexcept
	{
		return std::move(*this - (*this - normal * Vector3::Dot(*this, normal)));
	}

	//XMVECTORオペレーター
	inline Vector3Ref::operator XmVector() const noexcept { return DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(x, y, z)); }

	//+オペレーター  
	inline Vector3 Vector3Ref::operator+() const { return Vector3(+this->x, +this->y, +this->z); }
	//-オペレーター  
	inline Vector3 Vector3Ref::operator-() const { return Vector3(-this->x, -this->y, -this->z); }
	//==オペレーター
	inline bool Vector3Ref::operator==(const Vector3Ref& vec) const
	{
		return (fabsf(x - vec.x) <= MathAF::cEpsilon
			&& fabsf(y - vec.y) <= MathAF::cEpsilon
			&& fabsf(z - vec.z) <= MathAF::cEpsilon);
	}
	//!=オペレーター
	inline bool Vector3Ref::operator!=(const Vector3Ref& vec) const
	{
		return (fabsf(x - vec.x) <= MathAF::cEpsilon
			&& fabsf(y - vec.y) <= MathAF::cEpsilon
			&& fabsf(z - vec.z) <= MathAF::cEpsilon);
	}

	//加算オペレーター
	inline Vector3 Vector3Ref::operator+(const Vector3Ref& vec) const { return Vector3(x + vec.x, y + vec.y, z + vec.z); }
	//減算オペレーター
	inline Vector3 Vector3Ref::operator-(const Vector3Ref& vec) const { return Vector3(x - vec.x, y - vec.y, z - vec.z); }
	//乗算オペレーター
	inline Vector3 Vector3Ref::operator*(const Vector3Ref& vec) const { return Vector3(x * vec.x, y * vec.y, z * vec.z); }
	//乗算オペレーター(float)
	inline Vector3 Vector3Ref::operator*(const float value) const { return Vector3(x * value, y * value, z * value); }
	//除算オペレーター
	inline Vector3 Vector3Ref::operator/(const Vector3Ref& vec) const { return Vector3(x / vec.x, y / vec.y, z / vec.z); }
	//除算オペレーター(float)
	inline Vector3 Vector3Ref::operator/(const float value) const { return Vector3(x / value, y / value, z / value); }

	//+=オペレーター
	inline const Vector3Ref & Vector3Ref::operator+=(const Vector3Ref& vec)
	{
		this->x += vec.x;		this->y += vec.y;
		this->z += vec.z;
		return *this;
	}
	//-=オペレーター
	inline const Vector3Ref & Vector3Ref::operator-=(const Vector3Ref& vec)
	{
		this->x -= vec.x;		this->y -= vec.y;
		this->z -= vec.z;
		return *this;
	}
	//*=オペレーター
	inline const Vector3Ref & Vector3Ref::operator*=(const Vector3Ref& vec)
	{
		this->x *= vec.x;		this->y *= vec.y;
		this->z *= vec.z;
		return *this;
	}
	//*=オペレーター(float)
	inline const Vector3Ref & Vector3Ref::operator*=(const float value)
	{
		this->x *= value;		this->y *= value;
		this->z *= value;
		return *this;
	}
	// /=オペレーター
	inline const Vector3Ref & Vector3Ref::operator/=(const Vector3Ref& vec)
	{
		this->x /= vec.x;		this->y /= vec.y;
		this->z /= vec.z;
		return *this;
	}
	// /=オペレーター(float)
	inline const Vector3Ref & Vector3Ref::operator/=(const float value)
	{
		this->x /= value;		this->y /= value;
		this->z /= value;
		return *this;
	}

	//----------------------------------------------------------------------------------
	//Vector3

	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//代入初期化する
	//引数1: Vector3
	Vector3Ref::Vector3Ref(Vector3 & value) : x(value.x), y(value.y), z(value.z) {}

	//=オペレーター
	inline const Vector3Ref & Vector3Ref::operator=(const Vector3 & vec)
	{
		x = vec.x; y = vec.y;
		z = vec.z;
		return *this;
	}

	//==オペレーター
	inline bool Vector3Ref::operator==(const Vector3 & vec) const
	{
		return (fabsf(x - vec.x) <= MathAF::cEpsilon
			&& fabsf(y - vec.y) <= MathAF::cEpsilon
			&& fabsf(z - vec.z) <= MathAF::cEpsilon);
	}
	//!=オペレーター
	inline bool Vector3Ref::operator!=(const Vector3 & vec) const
	{
		return (fabsf(x - vec.x) <= MathAF::cEpsilon
			&& fabsf(y - vec.y) <= MathAF::cEpsilon
			&& fabsf(z - vec.z) <= MathAF::cEpsilon);
	}

	//加算オペレーター
	inline Vector3 Vector3Ref::operator+(const Vector3 & vec) const { return Vector3(x + vec.x, y + vec.y, z + vec.z); }
	//減算オペレーター
	inline Vector3 Vector3Ref::operator-(const Vector3 & vec) const { return Vector3(x - vec.x, y - vec.y, z - vec.z); }
	//乗算オペレーター
	inline Vector3 Vector3Ref::operator*(const Vector3 & vec) const { return Vector3(x * vec.x, y * vec.y, z * vec.z); }
	//除算オペレーター
	inline Vector3 Vector3Ref::operator/(const Vector3 & vec) const { return Vector3(x / vec.x, y / vec.y, z / vec.z); }

	//+=オペレーター
	inline const Vector3Ref & Vector3Ref::operator+=(const Vector3 & vec)
	{
		this->x += vec.x;		this->y += vec.y;
		this->z += vec.z;
		return *this;
	}
	//-=オペレーター
	inline const Vector3Ref & Vector3Ref::operator-=(const Vector3 & vec)
	{
		this->x -= vec.x;		this->y -= vec.y;
		this->z -= vec.z;
		return *this;
	}
	//*=オペレーター
	inline const Vector3Ref & Vector3Ref::operator*=(const Vector3 & vec)
	{
		this->x *= vec.x;		this->y *= vec.y;
		this->z *= vec.z;
		return *this;
	}
	// /=オペレーター
	inline const Vector3Ref & Vector3Ref::operator/=(const Vector3 & vec)
	{
		this->x /= vec.x;		this->y /= vec.y;
		this->z /= vec.z;
		return *this;
	}


	//=オペレーター
	inline const Vector3Ref & Vector3Ref::operator=(const DirectX::XMFLOAT3 & vec)
	{
		x = vec.x; y = vec.y;
		z = vec.z;
		return *this;
	}

	//==オペレーター
	inline bool Vector3Ref::operator==(const DirectX::XMFLOAT3 & vec) const
	{
		return (fabsf(x - vec.x) <= MathAF::cEpsilon
			&& fabsf(y - vec.y) <= MathAF::cEpsilon
			&& fabsf(z - vec.z) <= MathAF::cEpsilon);
	}
	//!=オペレーター
	inline bool Vector3Ref::operator!=(const DirectX::XMFLOAT3 & vec) const
	{
		return (fabsf(x - vec.x) <= MathAF::cEpsilon
			&& fabsf(y - vec.y) <= MathAF::cEpsilon
			&& fabsf(z - vec.z) <= MathAF::cEpsilon);
	}

	//加算オペレーター
	inline Vector3 Vector3Ref::operator+(const DirectX::XMFLOAT3 & vec) const { return Vector3(x + vec.x, y + vec.y, z + vec.z); }
	//減算オペレーター
	inline Vector3 Vector3Ref::operator-(const DirectX::XMFLOAT3 & vec) const { return Vector3(x - vec.x, y - vec.y, z - vec.z); }
	//乗算オペレーター
	inline Vector3 Vector3Ref::operator*(const DirectX::XMFLOAT3 & vec) const { return Vector3(x * vec.x, y * vec.y, z * vec.z); }
	//除算オペレーター
	inline Vector3 Vector3Ref::operator/(const DirectX::XMFLOAT3 & vec) const { return Vector3(x / vec.x, y / vec.y, z / vec.z); }

	//+=オペレーター
	inline const Vector3Ref & Vector3Ref::operator+=(const DirectX::XMFLOAT3 & vec)
	{
		this->x += vec.x;		this->y += vec.y;
		this->z += vec.z;
		return *this;
	}
	//-=オペレーター
	inline const Vector3Ref & Vector3Ref::operator-=(const DirectX::XMFLOAT3 & vec)
	{
		this->x -= vec.x;		this->y -= vec.y;
		this->z -= vec.z;
		return *this;
	}
	//*=オペレーター
	inline const Vector3Ref & Vector3Ref::operator*=(const DirectX::XMFLOAT3 & vec)
	{
		this->x *= vec.x;		this->y *= vec.y;
		this->z *= vec.z;
		return *this;
	}
	// /=オペレーター
	inline const Vector3Ref & Vector3Ref::operator/=(const DirectX::XMFLOAT3 & vec)
	{
		this->x /= vec.x;		this->y /= vec.y;
		this->z /= vec.z;
		return *this;
	}
}
#endif // !SGFRAMEWORK_HEADER_VECTOR_VECTOR3_REF_HPP_
