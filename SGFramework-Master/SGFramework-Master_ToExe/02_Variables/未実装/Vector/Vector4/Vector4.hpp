/*----------------------------------------------------------------------------------
4つの値を扱えるVector4 構造体
最も多用する構造体シリーズの1つなので別自作Vectorが関係ないものは全てinline実装
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_VECTOR_VECTOR4_HPP_
#define SGFRAMEWORK_HEADER_VECTOR_VECTOR4_HPP_
#include <DirectXMath.h>
#include <math.h>
#include <iostream>
#include <PxPhysicsAPI.h>
#include "../../ConstAndUsing/ConstAndUsing.hpp"
#include "../../Property/Property.hpp"
#include "../../String/String.hpp"

//Framework namespace
namespace SGFramework
{
	//プロトタイプ宣言
	struct Vector2;
	struct Vector3;
	struct Vector4Int;
	struct Vector4Ref;
	
	//4つの値を扱えるVector4 構造体
	struct Vector4 final : public DirectX::XMFLOAT4
	{
	public:
		//----------------------------------------------------------------------------------
		//inlne実装

		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//全て0に設定される
		inline constexpr Vector4();
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化する
		//引数1: x
		//引数2: y
		//引数3: z
		//引数4: w
		inline constexpr Vector4(float x, float y, float z, float w);
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化する
		//引数1: XMFLOAT4
		inline constexpr Vector4(const DirectX::XMFLOAT4& vec);
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化する
		//引数1: PxVec4
		inline constexpr Vector4(const physx::PxVec4& vec);
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化する
		//引数1: XMVECTOR
		inline Vector4(const XmVector& vec);
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化する
		//引数1: float*
		inline Vector4(const float* array);

		//----------------------------------------------------------------------------------
		//[ToXmVector]
		//return: XmVector
		inline XmVector ToXmVector() const noexcept { return DirectX::XMLoadFloat4(this); }
		//----------------------------------------------------------------------------------
		//[ToRotationMatrix]
		//return: thisをQuaternionとした回転行列
		inline XmMatrix ToRotationMatrix() const noexcept { return DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(this)); }
		//----------------------------------------------------------------------------------
		//[ToRotationMatrix]
		//引数1: Apply
		inline XmMatrix ToRotationMatrix(XmMatrix& apply) const noexcept { apply = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(this)); }
		//----------------------------------------------------------------------------------
		//[ToPhysX]
		//return: physx::PxVec4
		inline physx::PxVec4 ToPhysX() const noexcept { return physx::PxVec4(x, y, z, w); }

		//default
		Vector4(const Vector4&) = default;
		Vector4& operator=(const Vector4&) = default;
		Vector4(Vector4&&) = default;
		Vector4& operator=(Vector4&&) = default;

		//----------------------------------------------------------------------------------
		//[CreateWithInt]
		//intでVector4を作成
		//引数1:x
		//引数2:y
		//引数3:z
		//引数4:w
		inline static constexpr Vector4 CreateWithInt(int x, int y, int z, int w);

		//----------------------------------------------------------------------------------
		//[Absolute]
		//return: 絶対値
		inline constexpr Vector4 Absolute() const noexcept;
		//----------------------------------------------------------------------------------
		//[Magnitude]
		//return: 自身とVector(0, 0, 0, 0)の距離
		inline constexpr float Magnitude() const noexcept;
		//----------------------------------------------------------------------------------
		//[ExactMagnitude]
		//return: 自身とVector(0,0,0,0)の距離 (正確な値だが負荷が高い)
		inline float ExactMagnitude() const noexcept;	
		//----------------------------------------------------------------------------------
		//[Normalize]
		//return: 自身を正規化したベクトル
		inline Vector4 Normalized() const noexcept;

		//XMVECTORオペレーター
		inline operator XmVector() const noexcept;
		//float*オペレーター
		inline operator float*();
		//const float*オペレーター
		inline operator const float*() const;

		//=オペレーター
		inline XmVector operator = (const XmVector& vec) noexcept;

		//+オペレーター  
		inline Vector4 operator +() const;
		//-オペレーター  
		inline Vector4 operator -() const;
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
		//乗算オペレーター(float)
		inline Vector4 operator *(float value) const;
		//除算オペレーター
		inline Vector4 operator /(const Vector4& vec) const;
		//除算オペレーター(float)
		inline Vector4 operator /(float value) const;

		//+=オペレーター
		inline const Vector4 &operator +=(const Vector4& vec);
		//-=オペレーター
		inline const Vector4 &operator -=(const Vector4& vec);
		//*=オペレーター
		inline const Vector4 &operator *=(const Vector4& vec);
		//*=オペレーター(float)
		inline const Vector4 &operator *=(float value);
		// /=オペレーター
		inline const Vector4 &operator /=(const Vector4& vec);
		// /=オペレーター(float)
		inline const Vector4 &operator /=(float value);

		//sgstring operator
		inline operator sgstring() const { return ToString(); }
		//----------------------------------------------------------------------------------
		//[ToString]
		//sgstring形式で出力する
		//return: sgstring表現したthis
		inline sgstring ToString() const { return (L"(" + std::to_wstring(x) + L", " + std::to_wstring(y) + L", " + std::to_wstring(z) + L", " + std::to_wstring(w) + L")"); }
		//----------------------------------------------------------------------------------
		//[ToStdString]
		//std::string形式で出力する
		//return: std::string表現したthis
		inline std::string ToStdString() const { return ("(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ", " + std::to_string(w) + ")"); }

		//----------------------------------------------------------------------------------
		//cpp実装

		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化する
		//引数1: Vector4Int
		Vector4(const Vector4Int& value);

		//----------------------------------------------------------------------------------
		//[setVector3]
		//Vector4(vec.x, vec.y, vec.z, w)の距離
		//引数1: Vector3
		//引数2: w, default = 0.0f
		void setVector3(const Vector3& vector3, float w = 0.0f);

		//Vector2オペレーター
		operator Vector2() const;
		//Vector3オペレーター
		operator Vector3() const;
		//Vector4Intオペレーター
		operator Vector4Int() const;
	
		//=オペレーター
		const Vector4&  operator = (const Vector2& vec);
		//=オペレーター
		const Vector4& operator = (const Vector3& vec);
		//=オペレーター
		const Vector4& operator = (const Vector4Int& vec);

		//----------------------------------------------------------------------------------
		//Vector4Ref

		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化する
		//引数1: Vector4Ref
		Vector4(const Vector4Ref& value);

		// =オペレーター
		const Vector4 &operator =(const Vector4Ref & vec);

		//==オペレーター
		bool operator ==(const Vector4Ref& vec) const;
		//!=オペレーター
		bool operator !=(const Vector4Ref& vec) const;

		//加算オペレーター
		Vector4 operator +(const Vector4Ref& vec) const;
		//減算オペレーター
		Vector4 operator -(const Vector4Ref& vec) const;
		//乗算オペレーター
		Vector4 operator *(const Vector4Ref& vec) const;
		//除算オペレーター
		Vector4 operator /(const Vector4Ref& vec) const;

		//+=オペレーター
		const Vector4& operator +=(const Vector4Ref& vec);
		//-=オペレーター
		const Vector4& operator -=(const Vector4Ref& vec);
		//*=オペレーター
		const Vector4& operator *=(const Vector4Ref& vec);
		// /=オペレーター
		const Vector4& operator /=(const Vector4Ref& vec);


		//計算ライブラリ


		//----------------------------------------------------------------------------------
		//[Dot]<Calculation Library Function>
		//内積を行う
		//return: 内積の結果
		//引数1: 対象ベクトル1
		//引数2: 対象ベクトル2
		inline static float Dot(const Vector4& vec1, const Vector4& vec2) noexcept;
		//----------------------------------------------------------------------------------
		//[Distance]<Calculation Library Function>
		//fromからtoの距離を測る
		//return: from-toの距離
		//引数1: 対象ベクトル1
		//引数2: 対象ベクトル2
		inline constexpr float Distance(const Vector4& from, const Vector4& to) noexcept;
		//----------------------------------------------------------------------------------
		//[ExactDistance]<Calculation Library Function>
		//fromからtoの距離を測る, Distanceより正確な値だが計算負荷が高い
		//return: from-toの距離
		//引数1: 対象ベクトル1
		//引数2: 対象ベクトル2
		inline static float ExactDistance(const Vector4& from, const Vector4& to) noexcept;
	};

	//----------------------------------------------------------------------------------
	//[wostream operator]
	//引数1: stream
	//引数2: this
	inline std::wostream& operator<<(std::wostream& stream, const Vector4& value)
	{
		const wchar* string = value.ToString();
		stream << string;
		return stream;
	}


	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//全て0に設定される
	inline constexpr Vector4::Vector4() : DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f) {}
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//代入初期化する
	//引数1: x
	//引数2: y
	//引数3: z
	//引数4: w
	inline constexpr Vector4::Vector4(float x, float y, float z, float w)
		: DirectX::XMFLOAT4(x, y, z, w) {}
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//代入初期化する
	//引数1: XMFLOAT4
	inline constexpr Vector4::Vector4(const DirectX::XMFLOAT4 & vec) : XMFLOAT4(vec) {}
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//代入初期化する
	//引数1: PxVec4
	inline constexpr Vector4::Vector4(const physx::PxVec4 & vec) : XMFLOAT4(vec.x, vec.y, vec.z, vec.w) {}
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//代入初期化する
	//引数1: XMVECTOR
	inline Vector4::Vector4(const XmVector & vec)
	{
		DirectX::XMStoreFloat4(this, vec);
	}
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//代入初期化する
	//引数1: float*
	inline Vector4::Vector4(const float * array) : DirectX::XMFLOAT4(array){}

	//----------------------------------------------------------------------------------
	//[CreateWithInt]
	//intでVector4を作成
	//引数1:x
	//引数2:y
	//引数3:z
	//引数4:w
	inline constexpr Vector4 Vector4::CreateWithInt(int x, int y, int z, int w)
	{
		return Vector4(static_cast<float>(x), static_cast<float>(y), 
			static_cast<float>(z), static_cast<float>(w));
	}

	//----------------------------------------------------------------------------------
	//[Absolute]
	//vecの絶対値を計算する
	//return: 絶対値
	inline constexpr Vector4 Vector4::Absolute() const noexcept
	{
		return std::move(Vector4(x < 0.0f ? -x : x, y < 0.0f ? -y : y, z < 0.0f ? -z : z, w < 0.0f ? -w : w));
	}
	//----------------------------------------------------------------------------------
	//[Magnitude]
	//return: 自身とVector(0,0, 0, 0)の距離
	inline constexpr float Vector4::Magnitude() const noexcept
	{
		float f = x * x + y * y + z * z + w * w;
		int i = 0x5f3759df - (*reinterpret_cast<int*>(&f) >> 1);
		float bitF = *reinterpret_cast<float*>(&i);

		bitF *= (1.5f - 0.5f * f * bitF * bitF);
		return f * bitF * (1.5f - 0.5f * f * bitF * bitF);
	}
	//----------------------------------------------------------------------------------
	//[ExactMagnitude]
	//return: 自身とVector(0,0,0)の距離 (正確な値だが負荷が高い)
	inline float Vector4::ExactMagnitude() const noexcept
	{
		return std::sqrtf(x * x + y * y + z * z + w * w);
	}
	//----------------------------------------------------------------------------------
	//[Normalize]
	//return: 自身を正規化したベクトル
	inline Vector4 Vector4::Normalized() const noexcept
	{
		float divLength = 1.0f / ExactMagnitude();
		return std::move(Vector4(x * divLength, y * divLength, z * divLength, w * divLength));
	}

	//XMVECTORオペレーター
	inline Vector4::operator XmVector() const noexcept {	return DirectX::XMLoadFloat4(this); }
	//float*オペレーター
	inline Vector4::operator float*() { return (float*)this; }
	//const float*オペレーター
	inline Vector4::operator const float*() const { return (float*)this; }

	//=オペレーター
	inline XmVector Vector4::operator=(const XmVector & vec) noexcept
	{
		DirectX::XMStoreFloat4(this, vec);
		return vec;
	}

	//+オペレーター  
	inline Vector4 Vector4::operator+() const { return Vector4(+this->x, +this->y, +this->z, +this->w); }
	//-オペレーター  
	inline Vector4 Vector4::operator-() const{	return Vector4(-this->x, -this->y , -this->z, -this->w); }
	//==オペレーター
	inline bool Vector4::operator==(const Vector4& vec) const
	{
		return (fabsf(x - vec.x) <= MathAF::cEpsilon
			&& fabsf(y - vec.y) <= MathAF::cEpsilon
			&& fabsf(z - vec.z) <= MathAF::cEpsilon
			&& fabsf(w - vec.w) <= MathAF::cEpsilon);
	}
	//!=オペレーター
	inline bool Vector4::operator!=(const Vector4& vec) const
	{
		return (fabsf(x - vec.x) > MathAF::cEpsilon
			&& fabsf(y - vec.y) > MathAF::cEpsilon
			&& fabsf(z - vec.z) > MathAF::cEpsilon
			&& fabsf(w - vec.w) > MathAF::cEpsilon);
	}

	//加算オペレーター
	inline Vector4 Vector4::operator+(const Vector4& vec) const{	return Vector4(x + vec.x, y + vec.y, z + vec.z, w + vec.w); }
	//減算オペレーター
	inline Vector4 Vector4::operator-(const Vector4& vec) const{	return Vector4(x - vec.x, y - vec.y, z - vec.z, w - vec.w); }
	//乗算オペレーター
	inline Vector4 Vector4::operator*(const Vector4& vec) const{ return Vector4(x * vec.x, y * vec.y, z * vec.z, w * vec.w); }
	//乗算オペレーター(float)
	inline Vector4 Vector4::operator*(float value) const{ return Vector4(x * value, y * value, z * value, w * value); }
	//除算オペレーター
	inline Vector4 Vector4::operator/(const Vector4& vec) const{ return Vector4(x / vec.x, y / vec.y, z / vec.z, w / vec.w); }
	//除算オペレーター(float)
	inline Vector4 Vector4::operator/(float value) const{return Vector4(x / value, y / value, z / value, w / value); }
	
	//+=オペレーター
	inline const Vector4 & Vector4::operator+=(const Vector4& vec)
	{
		this->x += vec.x;		this->y += vec.y;
		this->z += vec.z;		this->w += vec.w;
		return *this;
	}
	//-=オペレーター
	inline const Vector4 & Vector4::operator-=(const Vector4& vec)
	{
		this->x -= vec.x;		this->y -= vec.y;
		this->z -= vec.z;		this->w -= vec.w;
		return *this;
	}
	//*=オペレーター
	inline const Vector4 & Vector4::operator*=(const Vector4& vec)
	{
		this->x *= vec.x;		this->y *= vec.y;
		this->z *= vec.z;		this->w *= vec.w;
		return *this;
	}
	//*=オペレーター(float)
	inline const Vector4 & Vector4::operator*=(float value)
	{
		this->x *= value;		this->y *= value;
		this->z *= value;		this->w *= value;
		return *this;
	}
	// /=オペレーター
	inline const Vector4 & Vector4::operator/=(const Vector4& vec)
	{
		this->x /= vec.x;		this->y /= vec.y;
		this->z /= vec.z;		this->w /= vec.w;
		return *this;
	}
	// /=オペレーター(float)
	inline const Vector4 & Vector4::operator/=(float value)
	{
		this->x /= value;		this->y /= value;
		this->z /= value;		this->w /= value;
		return *this;
	}


	//----------------------------------------------------------------------------------
	//[Dot]<Calculation Library Function>
	//内積を行う
	//return: 内積の結果
	//引数1: 対象ベクトル1
	//引数2: 対象ベクトル2
	inline float Vector4::Dot(const Vector4 & vec1, const Vector4 & vec2) noexcept
	{
		return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z + vec1.w * vec2.w;
	}
	//----------------------------------------------------------------------------------
	//[Distance]<Calculation Library Function>
	//fromからtoの距離を測る
	//return: from-toの距離
	//引数1: 対象ベクトル1
	//引数2: 対象ベクトル2
	inline constexpr float Vector4::Distance(const Vector4 & from, const Vector4 & to) noexcept
	{
		float f = (from.x - to.x) * (from.x - to.x)
			+ (from.y - to.y) * (from.y - to.y)
			+ (from.z - to.z) * (from.z - to.z)
			+ (to.w - from.w) * (to.w - from.w);
		int i = 0x5f3759df - (*reinterpret_cast<int*>(&f) >> 1);
		float bitF = *reinterpret_cast<float*>(&i);

		bitF *= (1.5f - 0.5f * f * bitF * bitF);
		return f * bitF * (1.5f - 0.5f * f * bitF * bitF);
	}
	//----------------------------------------------------------------------------------
	//[ExactDistance]<Calculation Library Function>
	//fromからtoの距離を測る, Distanceより正確な値だが計算負荷が高い
	//return: from-toの距離
	//引数1: 対象ベクトル1
	//引数2: 対象ベクトル2
	inline float Vector4::ExactDistance(const Vector4& from, const Vector4& to) noexcept
	{
		return std::sqrtf((to.x - from.x) * (to.x - from.x) + (to.y - from.y) * (to.y - from.y)
			+ (to.z - from.z) * (to.z - from.z) + (to.w - from.w) * (to.w - from.w));
	}
}

#endif // !SGFRAMEWORK_HEADER_VECTOR_VECTOR4_HPP_
