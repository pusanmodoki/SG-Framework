/*----------------------------------------------------------------------------------
3つの値を扱えるVector3 構造体
最も多用する構造体シリーズの1つなので別自作Vectorが関係ないものは全てinline実装
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_VECTOR_VECTOR3_HPP_
#define SGFRAMEWORK_HEADER_VECTOR_VECTOR3_HPP_
#include <DirectXMath.h>
#include <cmath>
#include <iostream>
#include <assimp/vector3.h>
#include <PxPhysicsAPI.h>
#include "../../ConstAndUsing/ConstAndUsing.hpp"
#include "../../Property/Property.hpp"
#include "../../String/String.hpp"

//Framework namespace
namespace SGFramework
{
	//プロトタイプ宣言
	struct Vector2;
	struct Vector4;
	struct Vector3Int;
	struct Vector3Ref;
	struct Quaternion;
	
	//3つの値を扱えるVector3 構造体
	struct Vector3 : public DirectX::XMFLOAT3
	{
	public:
		//----------------------------------------------------------------------------------
		//inlne実装

		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//全て0に設定される
		inline constexpr Vector3();
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化する
		//引数1:x
		//引数2:y
		//引数3:z
		inline constexpr Vector3(float x, float y, float z);
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化する
		//引数1: XMFLOAT3
		inline constexpr Vector3(const DirectX::XMFLOAT3& value);
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化する
		//引数1: PxVec3
		inline constexpr Vector3(const physx::PxVec3& vec);
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化する
		//引数1: aiVector3D
		inline constexpr Vector3(const aiVector3D& vec);
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化する
		//引数1: XMVECTOR
		inline Vector3(const XmVector& value);
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化する
		//引数1: float*
		inline Vector3(const float* array);

		//----------------------------------------------------------------------------------
		//[ToXmVector]
		//return: XmVector
		inline XmVector ToXmVector() const noexcept { return DirectX::XMLoadFloat3(this); }
		//----------------------------------------------------------------------------------
		//[ToPhysX]
		//return: physx::PxVec3
		inline physx::PxVec3 ToPhysX() const noexcept { return physx::PxVec3(x, y, z); }
		//----------------------------------------------------------------------------------
		//[ToAssimp]
		//return: aiVector3D
		inline aiVector3D ToAssimp() const noexcept { return aiVector3D(x, y, z); }

		//default
		Vector3(const Vector3&) = default;
		Vector3& operator=(const Vector3&) = default;
		Vector3(Vector3&&) = default;
		Vector3& operator=(Vector3&&) = default;

		//----------------------------------------------------------------------------------
		//[CreateWithInt]
		//intでVector3を作成
		//引数1:x
		//引数2:y
		//引数3:z
		inline static constexpr Vector3 CreateWithInt(int x, int y, int z);

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
		//float*オペレーター
		inline operator float*();
		//const float*オペレーター
		inline operator const float*() const;

		//=オペレーター
		inline XmVector operator = (const XmVector& vec) noexcept;

		//+オペレーター  
		inline Vector3 operator +() const;
		//-オペレーター  
		inline Vector3 operator -() const;
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
		//乗算オペレーター(float)
		inline Vector3 operator *(float value) const;
		//除算オペレーター
		inline Vector3 operator /(const Vector3& vec) const;
		//除算オペレーター(float)
		inline Vector3 operator /(float value) const;

		//+=オペレーター
		inline const Vector3 &operator +=(const Vector3& vec);
		//-=オペレーター
		inline const Vector3 &operator -=(const Vector3& vec);
		//*=オペレーター
		inline const Vector3 &operator *=(const Vector3& vec);
		//*=オペレーター(float)
		inline const Vector3 &operator *=(float value);
		// /=オペレーター
		inline const Vector3 &operator /=(const Vector3& vec);
		// /=オペレーター(float)
		inline const Vector3 &operator /=(float value);


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
		//cpp実装

		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化する
		//引数1: Vector3Int
		Vector3(const Vector3Int& value);

		//Vector2オペレーター
		operator Vector2() const;
		//Vector4オペレーター
		operator Vector4() const;
		//Vector3Intオペレーター
		operator Vector3Int() const;

		//=オペレーター
		const Vector3& operator = (const Vector2& vec);
		//=オペレーター
		const Vector3& operator = (const Vector4& vec);
		//=オペレーター
		const Vector3& operator = (const Vector3Int& vec);
		//*オペレーター
		Vector3 operator * (const Quaternion& quaternion) const;
		//*=オペレーター
		const Vector3& operator *= (const Quaternion& quaternion);

		//----------------------------------------------------------------------------------
		//Vector3Ref

		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化する
		//引数1: Vector3Ref
		Vector3(const Vector3Ref& value);

		// =オペレーター
		const Vector3 &operator =(const Vector3Ref & vec);

		//==オペレーター
		bool operator ==(const Vector3Ref& vec) const;
		//!=オペレーター
		bool operator !=(const Vector3Ref& vec) const;

		//加算オペレーター
		Vector3 operator +(const Vector3Ref& vec) const;
		//減算オペレーター
		Vector3 operator -(const Vector3Ref& vec) const;
		//乗算オペレーター
		Vector3 operator *(const Vector3Ref& vec) const;
		//除算オペレーター
		Vector3 operator /(const Vector3Ref& vec) const;

		//+=オペレーター
		const Vector3& operator +=(const Vector3Ref& vec);
		//-=オペレーター
		const Vector3& operator -=(const Vector3Ref& vec);
		//*=オペレーター
		const Vector3& operator *=(const Vector3Ref& vec);
		// /=オペレーター
		const Vector3& operator /=(const Vector3Ref& vec);


		//計算ライブラリ


		//----------------------------------------------------------------------------------
		//[Orthogonal]<Calculation Library Function>
		//return: 自身のベクトルの直交ベクトル
		//引数1: 対象ベクトル
		inline static Vector3 Orthogonal(const Vector3& vec) noexcept;
		//----------------------------------------------------------------------------------
		//[TransformCoord]<Calculation Library Function>
		//Matrixを付与した値に変換する
		//return: Matrixを付与したベクトル
		//引数1: 対象ベクトル
		//引数2: 対象Matrix
		inline static Vector3 TransformCoord(const Vector3& vec, const DirectX::XMMATRIX& matrix) noexcept;
		//----------------------------------------------------------------------------------
		//[Rotate]<Calculation Library Function>
		//ベクトル方向をquaternionを使って回転させる
		//return: 回転したベクトル
		//引数1: 対象ベクトル
		//引数2: 回転に使うquaternion
		inline static Vector3 Rotate(const Vector3 & vec, const XmVector & quaternion) noexcept;
		//----------------------------------------------------------------------------------
		//[ProgectOnPlane]<Calculation Library Function>
		//vecを平面上ベクトルに射影する
		//return: 射影済ベクトル
		//引数1: 対象ベクトル
		//引数2: 平面ベクトルの法線(正規化済のもの)
		inline static Vector3 ProgectOnPlane(const Vector3& vec, const Vector3& normal) noexcept;
		//----------------------------------------------------------------------------------
		//[Dot]<Calculation Library Function>
		//内積を行う
		//return: 内積の結果
		//引数1: 対象ベクトル1
		//引数2: 対象ベクトル2
		inline static float Dot(const Vector3& vec1, const Vector3& vec2) noexcept;
		//----------------------------------------------------------------------------------
		//[Cross]<Calculation Library Function>
		//外積を行う
		//return: 外積の結果
		//引数1: 対象ベクトル1
		//引数2: 対象ベクトル2
		inline static Vector3 Cross(const Vector3& vec1, const Vector3& vec2) noexcept;
		//----------------------------------------------------------------------------------
		//[Distance]<Calculation Library Function>
		//fromからtoの距離を測る
		//return: from-toの距離
		//引数1: 対象ベクトル1
		//引数2: 対象ベクトル2
		inline static constexpr float Distance(const Vector3& from, const Vector3& to) noexcept;
		//----------------------------------------------------------------------------------
		//[ExactDistance]<Calculation Library Function>
		//fromからtoの距離を測る, Distanceより正確な値だが計算負荷が高い
		//return: from-toの距離
		//引数1: 対象ベクトル1
		//引数2: 対象ベクトル2
		inline static float ExactDistance(const Vector3& from, const Vector3& to) noexcept;
		//----------------------------------------------------------------------------------
		//[Direction]<Calculation Library Function>
		//fromからtoへの正規化済方向ベクトルを取得する
		//return: 正規化済方向ベクトル
		//引数1: 対象ベクトル1
		//引数2: 対象ベクトル2
		inline static Vector3 Direction(const Vector3& from, const Vector3& to) noexcept;
		//----------------------------------------------------------------------------------
		//[Lerp3D]<Calculation Library Function>
		//fromとtoの間をtで線形補完したものを返却する, 補完には3次関数補完を使う
		//return: fromからtoへの線形(3次関数)補間
		//引数1: from
		//引数2: to
		//引数3: 補完の度合い(0 ~ 1)
		inline static constexpr Vector3 Lerp3D(const Vector3& from, const Vector3& to, float t) noexcept;
		//----------------------------------------------------------------------------------
		//[LerpDirect]<Calculation Library Function>
		//fromとtoの間をtで線形補完したものを返却する, 補完にはただの線形補間を使う
		//return: fromからtoへの線形補間
		//引数1: from
		//引数2: to
		//引数3: 補完の度合い(0 ~ 1)
		inline static constexpr Vector3 LerpDirect(const Vector3& from, const Vector3& to, float t) noexcept;
		//----------------------------------------------------------------------------------
		//[Slerp]<Calculation Library Function>
		//fromとtoの間をtで球状に補完したものを返却する
		//return: fromからtoへの球面線形補間
		//引数1: from
		//引数2: to
		//引数3: 補完の度合い(0 ~ 1)
		inline static Vector3 Slerp(const Vector3& from, const Vector3& to, float t) noexcept;
		//----------------------------------------------------------------------------------
		//[Max]<Calculation Library Function>
		//2つのベクトルの各成分の一番大きな値を使ってVectorを作成
		//return: 最大ベクトル
		//引数1: 対象ベクトル1
		//引数2: 対象ベクトル2
		inline static constexpr Vector3 Max(const Vector3& vec1, const Vector3& vec2) noexcept;
		//----------------------------------------------------------------------------------
		//[Min]<Calculation Library Function>
		//2つのベクトルの各成分の一番小さい値を使ってVectorを作成
		//return: 最小ベクトル
		//引数1: 対象ベクトル1
		//引数2: 対象ベクトル2
		inline static constexpr Vector3 Min(const Vector3& vec1, const Vector3& vec2) noexcept;
	};

	//----------------------------------------------------------------------------------
	//[wostream operator]
	//引数1: stream
	//引数2: this
	inline std::wostream& operator<<(std::wostream& stream, const Vector3& value)
	{
		const wchar* string = value.ToString();
		stream << string;
		return stream;
	}

	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//全て0に設定される
	inline constexpr Vector3::Vector3() : DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f) {}
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//代入初期化する
	//引数1: x
	//引数2: y
	//引数3: z
	inline constexpr Vector3::Vector3(float x, float y, float z) : DirectX::XMFLOAT3(x, y, z) {}
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//代入初期化する
	//引数1: XMFLOAT3
	inline constexpr Vector3::Vector3(const DirectX::XMFLOAT3 & vec) : XMFLOAT3(vec) {}
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//代入初期化する
	//引数1: PxVec3
	inline constexpr Vector3::Vector3(const physx::PxVec3 & vec) : XMFLOAT3(vec.x, vec.y, vec.z) {}
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//代入初期化する
	//引数1: aiVector3D
	inline constexpr Vector3::Vector3(const aiVector3D & vec) : XMFLOAT3(vec.x, vec.y, vec.z) {}
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//代入初期化する
	//引数1: XMVECTOR
	inline Vector3::Vector3(const XmVector & vec)
	{
		DirectX::XMStoreFloat3(this, vec);
	}
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//代入初期化する
	//引数1: float*
	inline Vector3::Vector3(const float * array) : DirectX::XMFLOAT3(array) {}

	//----------------------------------------------------------------------------------
	//[CreateWithInt]
	//intでVector3を作成
	//引数1:x
	//引数2:y
	//引数3:z
	inline constexpr Vector3 Vector3::CreateWithInt(int x, int y, int z)
	{
		return Vector3(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
	}


	//----------------------------------------------------------------------------------
	//[Absolute]
	//return: 絶対値
	inline Vector3 Vector3::Absolute() const noexcept
	{
		return Vector3(x < 0.0f ? -x : x, y < 0.0f ? -y : y, z < 0.0f ? -z : z);
	}
	//----------------------------------------------------------------------------------
	//[Magnitude]
	//return: 自身とVector(0,0,0)の距離
	inline constexpr float Vector3::Magnitude() const noexcept
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
	inline float Vector3::ExactMagnitude() const noexcept
	{
		return std::sqrtf(x * x + y * y + z * z);
	}
	//----------------------------------------------------------------------------------
	//[ClampMagnitude]
	//LengthをmaxLengthに制限する
	//return: 制限したvector
	//引数1: 制限Length
	inline Vector3 Vector3::ClampMagnitude(const float maxLength) const noexcept
	{
		return Vector3(DirectX::XMVector3ClampLength(DirectX::XMLoadFloat3(this), 0.0f, maxLength));
	}
	//----------------------------------------------------------------------------------
	//[Normalize]
	//return: 自身を正規化したベクトル
	inline Vector3 Vector3::Normalized() const noexcept
	{
		float divLength = 1.0f / ExactMagnitude();
		return Vector3(x * divLength, y * divLength, z * divLength);
	}
	//----------------------------------------------------------------------------------
	//[Orthogonal]
	//return: 自身のベクトルの直交ベクトル
	inline Vector3 Vector3::Orthogonal() const noexcept { return Vector3(DirectX::XMVector3Orthogonal(DirectX::XMLoadFloat3(this))); }
	//----------------------------------------------------------------------------------
	//[TransformCoord]
	//Matrixを付与した値に変換する
	//return: Matrixを付与したベクトル
	//引数1: 対象Matrix
	inline Vector3 Vector3::TransformCoord(const DirectX::XMMATRIX & matrix) const noexcept
	{
		return DirectX::XMVector3TransformCoord(DirectX::XMLoadFloat3(this), matrix);
	}
	//----------------------------------------------------------------------------------
	//[Rotate]
	//return: 自身のベクトルの回転ベクトル
	inline Vector3 Vector3::Rotate(const XmVector&  quaternion) const noexcept
	{
		return Vector3(DirectX::XMVector3Rotate(DirectX::XMLoadFloat3(this), quaternion));
	}
	//----------------------------------------------------------------------------------
	//[ProgectOnPlane]
	//vecを平面上ベクトルに射影する
	//return: 射影済ベクトル
	//引数1: 平面ベクトルの法線(正規化済のもの)
	inline Vector3 Vector3::ProgectOnPlane(const Vector3 & normal) const noexcept
	{
		return std::move(*this - (*this - normal * Vector3::Dot(this->ToXmVector(), normal)));
	}

	//XMVECTORオペレーター
	inline Vector3::operator XmVector() const noexcept { return DirectX::XMLoadFloat3(this); }
	//float*オペレーター
	inline Vector3::operator float*() { return (float*)this; }
	//const float*オペレーター
	inline Vector3::operator const float*() const { return (float*)this; }

	//=オペレーター
	inline XmVector Vector3::operator=(const XmVector & vec) noexcept
	{
		DirectX::XMStoreFloat3(this, vec);
		return vec;
	}

	//+オペレーター  
	inline Vector3 Vector3::operator+() const { return Vector3(+this->x, +this->y, +this->z); }
	//-オペレーター  
	inline Vector3 Vector3::operator-() const { return Vector3(-this->x, -this->y, -this->z); }
	//==オペレーター
	inline bool Vector3::operator==(const Vector3& vec) const
	{
		return (fabsf(x - vec.x) <= MathAF::cEpsilon
			&& fabsf(y - vec.y) <= MathAF::cEpsilon
			&& fabsf(z - vec.z) <= MathAF::cEpsilon);
	}
	//!=オペレーター
	inline bool Vector3::operator!=(const Vector3& vec) const
	{
		return (fabsf(x - vec.x) > MathAF::cEpsilon
			&& fabsf(y - vec.y) > MathAF::cEpsilon
			&& fabsf(z - vec.z) > MathAF::cEpsilon);
	}

	//加算オペレーター
	inline Vector3 Vector3::operator+(const Vector3& vec) const { return Vector3(x + vec.x, y + vec.y, z + vec.z); }
	//減算オペレーター
	inline Vector3 Vector3::operator-(const Vector3& vec) const { return Vector3(x - vec.x, y - vec.y, z - vec.z); }
	//乗算オペレーター
	inline Vector3 Vector3::operator*(const Vector3& vec) const { return Vector3(x * vec.x, y * vec.y, z * vec.z); }
	//乗算オペレーター(float)
	inline Vector3 Vector3::operator*(float value) const { return Vector3(x * value, y * value, z * value); }
	//除算オペレーター
	inline Vector3 Vector3::operator/(const Vector3& vec) const { return Vector3(x / vec.x, y / vec.y, z / vec.z); }
	//除算オペレーター(float)
	inline Vector3 Vector3::operator/(float value) const { return Vector3(x / value, y / value, z / value); }

	//+=オペレーター
	inline const Vector3 & Vector3::operator+=(const Vector3& vec)
	{
		this->x += vec.x;		this->y += vec.y;
		this->z += vec.z;
		return *this;
	}
	//-=オペレーター
	inline const Vector3 & Vector3::operator-=(const Vector3& vec)
	{
		this->x -= vec.x;		this->y -= vec.y;
		this->z -= vec.z;
		return *this;
	}
	//*=オペレーター
	inline const Vector3 & Vector3::operator*=(const Vector3& vec)
	{
		this->x *= vec.x;		this->y *= vec.y;
		this->z *= vec.z;
		return *this;
	}
	//*=オペレーター(float)
	inline const Vector3 & Vector3::operator*=(float value)
	{
		this->x *= value;		this->y *= value;
		this->z *= value;
		return *this;
	}
	// /=オペレーター
	inline const Vector3 & Vector3::operator/=(const Vector3& vec)
	{
		this->x /= vec.x;		this->y /= vec.y;
		this->z /= vec.z;
		return *this;
	}
	// /=オペレーター(float)
	inline const Vector3 & Vector3::operator/=(float value)
	{
		this->x /= value;		this->y /= value;
		this->z /= value;
		return *this;
	}



	//----------------------------------------------------------------------------------
	//[Orthogonal]<Calculation Library Function>
	//return: 自身のベクトルの直交ベクトル
	//引数1: 対象ベクトル
	inline Vector3 Vector3::Orthogonal(const Vector3& vec) noexcept { return std::move(Vector3(DirectX::XMVector3Orthogonal(DirectX::XMLoadFloat3(&vec)))); }
	//----------------------------------------------------------------------------------
	//[TransformCoord]<Calculation Library Function>
	//Matrixを付与した値に変換する
	//return: Matrixを付与したベクトル
	//引数1: 対象ベクトル
	//引数2: 対象Matrix
	inline Vector3 Vector3::TransformCoord(const Vector3& vec, const DirectX::XMMATRIX& matrix) noexcept
	{
		return std::move(DirectX::XMVector3TransformCoord(DirectX::XMLoadFloat3(&vec), matrix));
	}
	//----------------------------------------------------------------------------------
	//[Rotate]<Calculation Library Function>
	//ベクトル方向をquaternionを使って回転させる
	//return: 回転したベクトル
	//引数1: 対象ベクトル
	//引数2: 回転に使うquaternion
	inline Vector3 Vector3::Rotate(const Vector3 & vec, const XmVector & quaternion) noexcept 
	{
		return std::move(DirectX::XMVector3Rotate(DirectX::XMLoadFloat3(&vec), quaternion));
	}
	//----------------------------------------------------------------------------------
	//[ProgectOnPlane]<Calculation Library Function>
	//vecを平面上ベクトルに射影する
	//return: 射影済ベクトル
	//引数1: 対象ベクトル
	//引数2: 平面ベクトルの法線(正規化済のもの)
	inline Vector3 Vector3::ProgectOnPlane(const Vector3& vec, const Vector3& normal) noexcept 
	{
		return std::move(vec - (vec - normal * Vector3::Dot(vec, normal)));
	}
	//----------------------------------------------------------------------------------
	//[Dot]<Calculation Library Function>
	//内積を行う
	//return: 内積の結果
	//引数1: 対象ベクトル1
	//引数2: 対象ベクトル2
	inline float Vector3::Dot(const Vector3& vec1, const Vector3& vec2) noexcept
	{
		return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
	}
	//----------------------------------------------------------------------------------
	//[Cross]<Calculation Library Function>
	//外積を行う
	//return: 外積の結果
	//引数1: 対象ベクトル1
	//引数2: 対象ベクトル2
	inline Vector3 Vector3::Cross(const Vector3& vec1, const Vector3& vec2) noexcept
	{ 
		// (y1 * z2 - z1 * y2, z1 * x2 - x1 * z2, x1 * y2 - y1 * x2) = (x3, y3, z3) =  v3
		return std::move(Vector3(vec1.y * vec2.z - vec1.z * vec2.y,
			vec1.z * vec2.x - vec1.x * vec2.z, vec1.x * vec2.y - vec1.y * vec2.x));
	}
	//----------------------------------------------------------------------------------
	//[Distance]<Calculation Library Function>
	//fromからtoの距離を測る
	//return: from-toの距離
	//引数1: 対象ベクトル1
	//引数2: 対象ベクトル2
	inline constexpr float Vector3::Distance(const Vector3& from, const Vector3& to) noexcept
	{
		float f = (from.x - to.x) * (from.x - to.x)
			+ (from.y - to.y) * (from.y - to.y)
			+ (from.z - to.z) * (from.z - to.z);
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
	inline float Vector3::ExactDistance(const Vector3& from, const Vector3& to) noexcept
	{
		return std::sqrtf((to.x - from.x) * (to.x - from.x) + (to.y - from.y) * (to.y - from.y)
		+ (to.z - from.z) * (to.z - from.z));
	}
	//----------------------------------------------------------------------------------
	//[Direction]<Calculation Library Function>
	//fromからtoへの正規化済方向ベクトルを取得する
	//return: 正規化済方向ベクトル
	//引数1: 対象ベクトル1
	//引数2: 対象ベクトル2
	inline Vector3 Vector3::Direction(const Vector3& from, const Vector3& to) noexcept { return std::move((to - from).Normalized()); }
	//----------------------------------------------------------------------------------
	//[Lerp3D]<Calculation Library Function>
	//fromとtoの間をtで線形補完したものを返却する, 補完には3次関数補完を使う
	//return: fromからtoへの線形(3次関数)補間
	//引数1: from
	//引数2: to
	//引数3: 補完の度合い(0 ~ 1)
	inline constexpr Vector3 Vector3::Lerp3D(const Vector3& from, const Vector3& to, float t) noexcept
	{
		if (t > 1.0f) t = 1.0f;	else if (t < 0.0f) t = 0.0f;

		//三次関数補完
		float rate = t * t * (3.0f - 2.0f * t);
		float rateSub = (1.0f - rate);
		//線形補間
		return std::move(Vector3(from.x * rateSub + to.x * rate,
			from.y * rateSub + to.y * rate, from.z * rateSub + to.z * rate));
	}
	//----------------------------------------------------------------------------------
	//[LerpDirect]<Calculation Library Function>
	//fromとtoの間をtで線形補完したものを返却する, 補完にはただの線形補間を使う
	//return: fromからtoへの線形補間
	//引数1: from
	//引数2: to
	//引数3: 補完の度合い(0 ~ 1)
	inline constexpr Vector3 Vector3::LerpDirect(const Vector3& from, const Vector3& to, float t) noexcept
	{
		if (t > 1.0f) t = 1.0f;	else if (t < 0.0f) t = 0.0f;

		//線形補間
		float tSub = (1.0f - t);
		return std::move(Vector3(from.x * tSub + to.x * t, from.y * tSub + to.y * t, from.z * tSub + to.z * t));
	}
	//----------------------------------------------------------------------------------
	//[Slerp]<Calculation Library Function>
	//fromとtoの間をtで球状に補完したものを返却する
	//return: fromからtoへの球面線形補間
	//引数1: from
	//引数2: to
	//引数3: 補完の度合い(0 ~ 1)
	inline Vector3 Vector3::Slerp(const Vector3& from, const Vector3& to, float t) noexcept
	{
		if (t > 1.0f) t = 1.0f;	else if (t < 0.0f) t = 0.0f;
		//Normal
		Vector3 startNormal = from.Normalized(), EndNormal = to.Normalized();
		//sinθ
		float angle = std::acos(Dot(startNormal, EndNormal));
		//sin(angle * ~~) = 補完係数, 一応正規化し球面線形補間へ
		return std::move(((startNormal * std::sin(angle * (1 - t)) + EndNormal * std::sin(angle * t)) / std::sin(angle)).Normalized());
	}
	//----------------------------------------------------------------------------------
	//[Max]<Calculation Library Function>
	//2つのベクトルの各成分の一番大きな値を使ってVectorを作成
	//return: 最大ベクトル
	//引数1: 対象ベクトル1
	//引数2: 対象ベクトル2
	inline constexpr Vector3 Vector3::Max(const Vector3& vec1, const Vector3& vec2) noexcept
	{
		Vector3 ret;
		if (vec1.x >= vec2.x) ret.x = vec1.x; else ret.x = vec2.x;
		if (vec1.y >= vec2.y) ret.y = vec1.y; else ret.y = vec2.y;
		if (vec1.z >= vec2.z) ret.z = vec1.z; else ret.z = vec2.z;
		return std::move(ret);
	}
	//----------------------------------------------------------------------------------
	//[Min]<Calculation Library Function>
	//2つのベクトルの各成分の一番小さい値を使ってVectorを作成
	//return: 最小ベクトル
	//引数1: 対象ベクトル1
	//引数2: 対象ベクトル2
	inline constexpr Vector3 Vector3::Min(const Vector3& vec1, const Vector3& vec2) noexcept
	{
		Vector3 ret;
		if (vec1.x <= vec2.x) ret.x = vec1.x; else ret.x = vec2.x;
		if (vec1.y <= vec2.y) ret.y = vec1.y; else ret.y = vec2.y;
		if (vec1.z <= vec2.z) ret.z = vec1.z; else ret.z = vec2.z;
		return std::move(ret);
	}
}
#endif // !SGFRAMEWORK_HEADER_VECTOR_VECTOR3_HPP_