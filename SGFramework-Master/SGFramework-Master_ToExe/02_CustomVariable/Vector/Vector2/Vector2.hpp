/*----------------------------------------------------------------------------------
2つの値を扱えるVector2 構造体
最も多用する構造体シリーズの1つなので別自作Vectorが関係ないものは全てinline実装
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_VECTOR_VECTOR2_HPP_
#define SGFRAMEWORK_HEADER_VECTOR_VECTOR2_HPP_
#include <DirectXMath.h>
#include <iostream>
#include <PxPhysicsAPI.h>
#include <assimp/vector2.h>
#include "imgui.h"
#include "../../ConstAndUsing/ConstAndUsing.hpp"
#include "../../Property/Property.hpp"
#include "../../String/String.hpp"

//Framework namespace
namespace SGFramework
{
	//プロトタイプ宣言
	struct Vector3;
	struct Vector4;
	struct Vector2Int;
	struct Vector2Ref;

	//2つの値を扱えるVector2 構造体
	struct Vector2 final: public DirectX::XMFLOAT2
	{
	public:
		//----------------------------------------------------------------------------------
		//inlne実装

		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//全て0に設定される
		inline constexpr Vector2();
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化する
		//引数1:x
		//引数2:y
		inline constexpr Vector2(float x, float y);
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化する
		//引数1: XMFLOAT2
		inline constexpr Vector2(const DirectX::XMFLOAT2& value);
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化する
		//引数1: PxVec2
		inline constexpr Vector2(const physx::PxVec2& vec);
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化する
		//引数1: aiVector2D
		inline constexpr Vector2(const aiVector2D& vec);
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化する
		//引数1: ImVec2
		inline constexpr Vector2(const ImVec2& vec);
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化する
		//引数1: XMVECTOR
		inline Vector2(const XmVector& value);
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化する
		//引数1: float*
		inline Vector2(const float* array);

		//----------------------------------------------------------------------------------
		//[ToXmVector]
		//return: XmVector
		inline XmVector ToXmVector() const noexcept { return DirectX::XMLoadFloat2(this); }
		//----------------------------------------------------------------------------------
		//[ToPhysX]
		//return: physx::PxVec
		inline physx::PxVec2 ToPhysX() const noexcept { return physx::PxVec2(x, y); }
		//----------------------------------------------------------------------------------
		//[ToAssimp]
		//return: aiVector2D
		inline aiVector2D ToAssimp() const noexcept { return aiVector2D(x, y); }
		//----------------------------------------------------------------------------------
		//[ToImGui]
		//return: ImVec2
		inline ImVec2 ToImGui() const noexcept { return ImVec2(x, y); }

		//default
		Vector2(const Vector2&) = default;
		Vector2& operator=(const Vector2&) = default;
		Vector2(Vector2&&) = default;
		Vector2& operator=(Vector2&&) = default;

		//----------------------------------------------------------------------------------
		//[CreateWithInt]
		//intでVector2を作成
		//引数1:x
		//引数2:y
		inline static constexpr Vector2 CreateWithInt(int x, int y);

		//----------------------------------------------------------------------------------
		//[Absolute]
		//return: 絶対値
		inline Vector2 Absolute() const noexcept;
		//----------------------------------------------------------------------------------
		//[Magnitude]
		//return: 自身とVector(0,0)の距離
		inline constexpr float Magnitude() const noexcept;
		//----------------------------------------------------------------------------------
		//[ExactMagnitude]
		//return: 自身とVector(0,0)の距離 (正確な値だが負荷が高い)
		inline float ExactMagnitude() const noexcept;
		//----------------------------------------------------------------------------------
		//[ClampMagnitude]
		//LengthをmaxLengthに制限する
		//return: 制限したvector
		//引数1: 制限Length
		inline Vector2 ClampMagnitude(const float maxLength) const noexcept;
		//----------------------------------------------------------------------------------
		//[Normalize]
		//return: 自身の正規化ベクトル
		inline Vector2 Normalized() const noexcept;

		//XMVECTORオペレーター
		inline operator XmVector() const noexcept;
		//float*オペレーター
		inline operator float*();
		//const float*オペレーター
		inline operator const float*() const;

		//=オペレーター
		inline XmVector operator = (const XmVector& vec) noexcept;

		//+オペレーター  
		inline Vector2 operator +() const;
		//-オペレーター  
		inline Vector2 operator -() const;
		//==オペレーター
		inline bool operator ==(const Vector2& vec) const;
		//!=オペレーター
		inline bool operator !=(const Vector2& vec) const;

		//加算オペレーター
		inline Vector2 operator +(const Vector2& vec) const;
		//減算オペレーター
		inline Vector2 operator -(const Vector2& vec) const;
		//乗算オペレーター
		inline Vector2 operator *(const Vector2& vec) const;
		//乗算オペレーター(float)
		inline Vector2 operator *(float value) const;
		//除算オペレーター
		inline Vector2 operator /(const Vector2& vec) const;
		//除算オペレーター(float)
		inline Vector2 operator /(float value) const;

		//+=オペレーター
		inline const Vector2 &operator +=(const Vector2& vec);
		//-=オペレーター
		inline const Vector2 &operator -=(const Vector2& vec);
		//*=オペレーター
		inline const Vector2 &operator *=(const Vector2& vec);
		//*=オペレーター(float)
		inline const Vector2 &operator *=(float value);
		// /=オペレーター
		inline const Vector2 &operator /=(const Vector2& vec);
		// /=オペレーター(float)
		inline const Vector2 &operator /=(float value);

		//sgstring operator
		inline operator sgstring() const { return ToString(); }
		//----------------------------------------------------------------------------------
		//[ToString]
		//sgstring形式で出力する
		//return: sgstring表現したthis
		inline sgstring ToString() const { return (L"(" + std::to_wstring(x) + L", " + std::to_wstring(y) + L")"); }
		//----------------------------------------------------------------------------------
		//[ToStdString]
		//std::string形式で出力する
		//return: std::string表現したthis
		inline std::string ToStdString() const { return ("(" + std::to_string(x) + ", " + std::to_string(y) + ")"); }
		
		//----------------------------------------------------------------------------------
		//cpp実装

		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化する
		//引数1: Vector2Int
		Vector2(const Vector2Int& value);

		//Vector3オペレーター
		operator Vector3() const;
		//Vector4オペレーター
		operator Vector4() const;
		//Vector2Intオペレーター
		operator Vector2Int() const;

		//=オペレーター
		const Vector2& operator = (const Vector3& vec);
		//=オペレーター
		const Vector2& operator = (const Vector4& vec);
		//=オペレーター
		const Vector2& operator = (const Vector2Int& vec);

		//----------------------------------------------------------------------------------
		//Vector2Ref

		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化する
		//引数1: Vector2Ref
		Vector2(const Vector2Ref& value);

		// =オペレーター
		const Vector2 &operator =(const Vector2Ref & vec);

		//==オペレーター
		bool operator ==(const Vector2Ref& vec) const;
		//!=オペレーター
		bool operator !=(const Vector2Ref& vec) const;

		//加算オペレーター
		Vector2 operator +(const Vector2Ref& vec) const;
		//減算オペレーター
		Vector2 operator -(const Vector2Ref& vec) const;
		//乗算オペレーター
		Vector2 operator *(const Vector2Ref& vec) const;
		//除算オペレーター
		Vector2 operator /(const Vector2Ref& vec) const;

		//+=オペレーター
		const Vector2& operator +=(const Vector2Ref& vec);
		//-=オペレーター
		const Vector2& operator -=(const Vector2Ref& vec);
		//*=オペレーター
		const Vector2& operator *=(const Vector2Ref& vec);
		// /=オペレーター
		const Vector2& operator /=(const Vector2Ref& vec);


		//計算ライブラリ


		//----------------------------------------------------------------------------------
		//[Dot]<Calculation Library Function>
		//内積を行う
		//return: 内積の結果
		//引数1: 対象ベクトル1
		//引数2: 対象ベクトル2
		inline static float Dot(const Vector2& vec1, const Vector2& vec2) noexcept;
		//----------------------------------------------------------------------------------
		//[Cross]<Calculation Library Function>
		//外積を行う
		//return: 外積の結果
		//引数1: 対象ベクトル1
		//引数2: 対象ベクトル2
		inline static float Cross(const Vector2& vec1, const Vector2& vec2) noexcept;
		//----------------------------------------------------------------------------------
		//[Angle]<Calculation Library Function>
		//2点間の角度を測る
		//return: 2点間の角度
		//引数1: 対象ベクトル1
		//引数2: 対象ベクトル2
		inline static float Angle(const Vector2& from, const Vector2& to) noexcept;
		//----------------------------------------------------------------------------------
		//[Distance]<Calculation Library Function>
		//fromからtoの距離を測る
		//return: from-toの距離
		//引数1: 対象ベクトル1
		//引数2: 対象ベクトル2
		inline static constexpr float Distance(const Vector2& from, const Vector2& to) noexcept;
		//----------------------------------------------------------------------------------
		//[ExactDistance]<Calculation Library Function>
		//fromからtoの距離を測る, Distanceより正確な値だが計算負荷が高い
		//return: from-toの距離
		//引数1: 対象ベクトル1
		//引数2: 対象ベクトル2
		inline static float ExactDistance(const Vector2& from, const Vector2& to) noexcept;
		//----------------------------------------------------------------------------------
		//[Direction]<Calculation Library Function>
		//fromからtoへの正規化済方向ベクトルを取得する
		//return: 正規化済方向ベクトル
		//引数1: 対象ベクトル1
		//引数2: 対象ベクトル2
		inline static Vector2 Direction(const Vector2& from, const Vector2& to) noexcept;
		//----------------------------------------------------------------------------------
		//[TransformCoord]<Calculation Library Function>
		//Matrixを付与した値に変換する
		//return: Matrixを付与したベクトル
		//引数1: 対象ベクトル
		//引数2: 対象Matrix
		inline static Vector2 TransformCoord(const Vector2& vec, const DirectX::XMMATRIX& matrix) noexcept;
		//----------------------------------------------------------------------------------
		//[Max]<Calculation Library Function>
		//2つのベクトルの各成分の一番大きな値を使ってVectorを作成
		//return: 最大ベクトル
		//引数1: 対象ベクトル1
		//引数2: 対象ベクトル2
		inline static constexpr Vector2 Max(const Vector2& vec1, const Vector2& vec2) noexcept;
		//----------------------------------------------------------------------------------
		//[Min]<Calculation Library Function>
		//2つのベクトルの各成分の一番小さい値を使ってVectorを作成
		//return: 最小ベクトル
		//引数1: 対象ベクトル1
		//引数2: 対象ベクトル2
		inline static constexpr Vector2 Min(const Vector2& vec1, const Vector2& vec2) noexcept;
	};


	//----------------------------------------------------------------------------------
	//[wostream operator]
	//引数1: stream
	//引数2: this
	inline std::wostream& operator<<(std::wostream& stream, const Vector2& value)
	{
		const wchar* string = value.ToString();
		stream << string;
		return stream;
	}

	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//全て0に設定される
	inline constexpr Vector2::Vector2() : XMFLOAT2(0.0f, 0.0f) {}
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//代入初期化する
	//引数1: x
	//引数2: y
	inline constexpr Vector2::Vector2(float x, float y) : XMFLOAT2(x, y) {}
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//代入初期化する
	//引数1: XMFLOAT2
	inline constexpr Vector2::Vector2(const DirectX::XMFLOAT2 & vec) : XMFLOAT2(vec) { }
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//代入初期化する
	//引数1: PxVec2
	inline constexpr Vector2::Vector2(const physx::PxVec2 & vec) : XMFLOAT2(vec.x, vec.y) {}
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//代入初期化する
	//引数1: aiVector2D
	inline constexpr Vector2::Vector2(const aiVector2D & vec) : XMFLOAT2(vec.x, vec.y) {}
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//代入初期化する
	//引数1: ImVec2
	inline constexpr Vector2::Vector2(const ImVec2 & vec) : XMFLOAT2(vec.x, vec.y) {}
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//代入初期化する
	//引数1: XMVECTOR
	inline Vector2::Vector2(const XmVector & vec)
	{
		DirectX::XMStoreFloat2(this, vec);
	}
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//代入初期化する
	//引数1: float*
	inline Vector2::Vector2(const float * array) : DirectX::XMFLOAT2(array) {}

	//----------------------------------------------------------------------------------
	//[CreateWithInt]
	//intでVector2を作成
	//引数1:x
	//引数2:y
	inline constexpr Vector2 Vector2::CreateWithInt(int x, int y)
	{
		return Vector2(static_cast<float>(x), static_cast<float>(y));
	}
	//----------------------------------------------------------------------------------
	//[Absolute]
	//return: 絶対値
	inline Vector2 Vector2::Absolute() const noexcept
	{
		return Vector2(x < 0.0f ? -x : x, y < 0.0f ? -y : y);
	}
	//----------------------------------------------------------------------------------
	//[Magnitude]
	//return: 自身とVector(0,0)の距離
	inline constexpr float Vector2::Magnitude() const noexcept
	{
		float f = x * x + y * y;
		int i = 0x5f3759df - (*reinterpret_cast<int*>(&f) >> 1);
		float bitF = *reinterpret_cast<float*>(&i);

		bitF *= (1.5f - 0.5f * f * bitF * bitF);
		return f * bitF * (1.5f - 0.5f * f * bitF * bitF);
	}
	//----------------------------------------------------------------------------------
	//[ExactMagnitude]
	//return: 自身とVector(0,0)の距離 (正確な値だが負荷が高い)
	inline float Vector2::ExactMagnitude() const noexcept
	{		
		return std::sqrtf(x * x + y * y);
	}
	//----------------------------------------------------------------------------------
	//[ClampMagnitude]
	//vecのLengthをmaxLengthに制限する
	//return: 制限したvector
	//引数2: 制限Length
	inline Vector2 Vector2::ClampMagnitude(const float maxLength) const noexcept
	{
		return std::move(DirectX::XMVector2ClampLength(*this, 0.0f, maxLength));
	}
	//----------------------------------------------------------------------------------
	//[Normalize]
	//return: 自身の正規化ベクトル
	inline Vector2 Vector2::Normalized() const noexcept
	{
		float divLength = 1.0f / ExactMagnitude();
		return std::move(Vector2(x * divLength, y * divLength));
	}

	//XMVECTORオペレーター
	inline Vector2::operator XmVector() const noexcept { return DirectX::XMLoadFloat2(this); }
	//float*オペレーター
	inline Vector2::operator float*() { return (float*)this; }
	//const float*オペレーター
	inline Vector2::operator const float*() const { return (float*)this; }

	//=オペレーター
	inline XmVector Vector2::operator=(const XmVector& vec) noexcept
	{
		DirectX::XMStoreFloat2(this, vec);
		return vec;
	}

	//+オペレーター  
	inline Vector2 Vector2::operator+() const { return Vector2(+this->x, +this->y); }
	//-オペレーター  
	inline Vector2 Vector2::operator-() const { return Vector2(-this->x, -this->y); }
	//==オペレーター
	inline bool Vector2::operator==(const Vector2& vec) const
	{
		return (fabsf(x - vec.x) <= MathAF::cEpsilon
			&& fabsf(y - vec.y) <= MathAF::cEpsilon);
	}
	//!=オペレーター
	inline bool Vector2::operator!=(const Vector2& vec) const
	{
		return (fabsf(x - vec.x) > MathAF::cEpsilon
			&& fabsf(y - vec.y) > MathAF::cEpsilon);
	}

	//加算オペレーター
	inline Vector2 Vector2::operator+(const Vector2& vec) const { return Vector2(x + vec.x, y + vec.y); }
	//減算オペレーター
	inline Vector2 Vector2::operator-(const Vector2& vec) const { return Vector2(x - vec.x, y - vec.y); }
	//乗算オペレーター
	inline Vector2 Vector2::operator*(const Vector2& vec) const { return Vector2(x * vec.x, y * vec.y); }
	//乗算オペレーター(float)
	inline Vector2 Vector2::operator*(const float value) const { return Vector2(x * value, y * value); }
	//除算オペレーター
	inline Vector2 Vector2::operator/(const Vector2& vec) const { return Vector2(x / vec.x, y / vec.y); }
	//除算オペレーター(float)
	inline Vector2 Vector2::operator/(const float value) const { return Vector2(x / value, y / value); }

	//+=オペレーター
	inline const Vector2 & Vector2::operator+=(const Vector2& vec)
	{
		this->x += vec.x;		this->y += vec.y;
		return *this;
	}
	//-=オペレーター
	inline const Vector2 & Vector2::operator-=(const Vector2& vec)
	{
		this->x -= vec.x;		this->y -= vec.y;
		return *this;
	}
	//*=オペレーター
	inline const Vector2 & Vector2::operator*=(const Vector2& vec)
	{
		this->x *= vec.x;		this->y *= vec.y;
		return *this;
	}
	//*=オペレーター(float)
	inline const Vector2 & Vector2::operator*=(const float value)
	{
		this->x *= value;		this->y *= value;
		return *this;
	}
	// /=オペレーター
	inline const Vector2 & Vector2::operator/=(const Vector2& vec)
	{
		this->x /= vec.x;		this->y /= vec.y;
		return *this;
	}
	// /=オペレーター(float)
	inline const Vector2 & Vector2::operator/=(const float value)
	{
		this->x /= value;		this->y /= value;
		return *this;
	}




	//----------------------------------------------------------------------------------
	//[Dot]<Calculation Library Function>
	//内積を行う
	//return: 内積の結果
	//引数1: 対象ベクトル1
	//引数2: 対象ベクトル2
	inline float Vector2::Dot(const Vector2& vec1, const Vector2& vec2) noexcept
	{
		return vec1.x * vec2.x + vec1.y * vec2.y;
	}
	//----------------------------------------------------------------------------------
	//[Cross]<Calculation Library Function>
	//外積を行う
	//return: 外積の結果
	//引数1: 対象ベクトル1
	//引数2: 対象ベクトル2
	inline float Vector2::Cross(const Vector2& vec1, const Vector2& vec2) noexcept
	{
		//x1 * y2 - x2 * y1 = |v1||v2|sin(θ)
		return vec1.x * vec2.y + vec2.x * vec1.y;
	}
	//----------------------------------------------------------------------------------
	//[Angle]<Calculation Library Function>
	//2点間の角度を測る
	//return: 2点間の角度
	//引数1: 対象ベクトル1
	//引数2: 対象ベクトル2
	inline float Vector2::Angle(const Vector2& from, const Vector2& to) noexcept
	{
		float cosSita = Dot(from, to) / (from.Magnitude() * to.Magnitude());
		float ret = std::acosf(cosSita) * MathAF::cRadToDeg;
		ret += 90.0f;
		while (ret > 180.0f) ret -= 360.0f;
		return std::move(ret);
	}
	//----------------------------------------------------------------------------------
	//[Distance]<Calculation Library Function>
	//fromからtoの距離を測る
	//return: from-toの距離
	//引数1: 対象ベクトル1
	//引数2: 対象ベクトル2
	inline constexpr float Vector2::Distance(const Vector2& from, const Vector2& to) noexcept
	{
		float f = (from.x - to.x) * (from.x - to.x)
			+ (from.y - to.y) * (from.y - to.y);
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
	inline float Vector2::ExactDistance(const Vector2 & from, const Vector2 & to) noexcept
	{
		return std::sqrtf((to.x - from.x) * (to.x - from.x) + (to.y - from.y) * (to.y - from.y));
	}
	//----------------------------------------------------------------------------------
	//[Direction]<Calculation Library Function>
	//fromからtoへの正規化済方向ベクトルを取得する
	//return: 正規化済方向ベクトル
	//引数1: 対象ベクトル1
	//引数2: 対象ベクトル2
	inline Vector2 Vector2::Direction(const Vector2& from, const Vector2& to) noexcept
	{
		return std::move((to - from).Normalized());
	}
	//----------------------------------------------------------------------------------
	//[TransformCoord]<Calculation Library Function>
	//Matrixを付与した値に変換する
	//return: Matrixを付与したベクトル
	//引数1: 対象ベクトル
	//引数2: 対象Matrix
	inline Vector2 Vector2::TransformCoord(const Vector2& vec, const DirectX::XMMATRIX& matrix) noexcept
	{
		return std::move(DirectX::XMVector2TransformCoord(vec, matrix));
	}
	//----------------------------------------------------------------------------------
	//[Max]<Calculation Library Function>
	//2つのベクトルの各成分の一番大きな値を使ってVectorを作成
	//return: 最大ベクトル
	//引数1: 対象ベクトル1
	//引数2: 対象ベクトル2
	inline constexpr Vector2 Vector2::Max(const Vector2& vec1, const Vector2& vec2) noexcept
	{
		Vector2 ret;
		if (vec1.x >= vec2.x) ret.x = vec1.x; else ret.x = vec2.x;
		if (vec1.y >= vec2.y) ret.y = vec1.y; else ret.y = vec2.y;
		return std::move(ret);
	}
	//----------------------------------------------------------------------------------
	//[Min]<Calculation Library Function>
	//2つのベクトルの各成分の一番小さい値を使ってVectorを作成
	//return: 最小ベクトル
	//引数1: 対象ベクトル1
	//引数2: 対象ベクトル2
	inline constexpr Vector2 Vector2::Min(const Vector2& vec1, const Vector2& vec2) noexcept
	{
		Vector2 ret;
		if (vec1.x <= vec2.x) ret.x = vec1.x; else ret.x = vec2.x;
		if (vec1.y <= vec2.y) ret.y = vec1.y; else ret.y = vec2.y;
		return std::move(ret);
	}
}
#endif // !SGFRAMEWORK_HEADER_VECTOR_VECTOR2_HPP_