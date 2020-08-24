/*----------------------------------------------------------------------------------
2つの値を扱えるVector2Int 構造体
最も多用する構造体シリーズの1つなので別自作Vectorが関係ないものは全てinline実装
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_VECTOR_VECTOR2INT_HPP_
#define SGFRAMEWORK_HEADER_VECTOR_VECTOR2INT_HPP_
#include <PxPhysicsAPI.h>
#include <assimp/vector2.h>
#include <DirectXMath.h>
#include <math.h>
#include <iostream>
#include "imgui.h"
#include "../../../ConstAndUsing/ConstAndUsing.hpp"
#include "../../../Property/Property.hpp"
#include "../../../String/String.hpp"

//Framework namespace
namespace SGFramework
{
	//プロトタイプ宣言
	struct Vector3Int;
	struct Vector4Int;
	struct Vector2;
	struct Vector2Ref;

	//2つの値を扱えるVector2Int 構造体
	struct Vector2Int final
	{
		//----------------------------------------------------------------------------------
		//inlne実装

		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//全て0に設定される
		inline constexpr Vector2Int();
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化する
		//引数1: x
		//引数2: y
		inline constexpr Vector2Int(int x, int y);
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化する
		//引数1: int*
		inline constexpr Vector2Int(const int* array);

		//----------------------------------------------------------------------------------
		//[ToXmVector]
		//return: XmVector
		inline XmVector ToXmVector() const noexcept {
			return DirectX::XMLoadFloat2(&DirectX::XMFLOAT2(static_cast<float>(x), static_cast<float>(y))); }
		//----------------------------------------------------------------------------------
		//[ToPhysX]
		//return: physx::PxVec
		inline physx::PxVec2 ToPhysX() const noexcept { return physx::PxVec2(
			static_cast<float>(x), static_cast<float>(y)); }
		//----------------------------------------------------------------------------------
		//[ToAssimp]
		//return: aiVector2D
		inline aiVector2D ToAssimp() const noexcept { return aiVector2D(
			static_cast<float>(x), static_cast<float>(y)); }
		//----------------------------------------------------------------------------------
		//[ToImGui]
		//return: ImVec2
		inline ImVec2 ToImGui() const noexcept { return ImVec2(
			static_cast<float>(x), static_cast<float>(y)); }

		//default
		Vector2Int(const Vector2Int&) = default;
		Vector2Int& operator=(const Vector2Int&) = default;
		Vector2Int(Vector2Int&&) = default;
		Vector2Int& operator=(Vector2Int&&) = default;

		//----------------------------------------------------------------------------------
		//変数
		union
		{
			struct
			{
				int x;		//X
				int y;		//Y
			};
			int value[2];	//Value
		};

		//----------------------------------------------------------------------------------
		//[Absolute]
		//return: 絶対値
		inline Vector2Int Absolute() const noexcept;
		//----------------------------------------------------------------------------------
		//[Magnitude]
		//return: 自身とVector(0,0)の距離
		inline constexpr float Magnitude() const noexcept;
		//----------------------------------------------------------------------------------
		//[ExactMagnitude]
		//return: 自身とVector(0,0)の距離 (正確な値だが負荷が高い)
		inline float ExactMagnitude() const noexcept;

		//int*オペレーター
		inline operator int* ();
		//const int*オペレーター
		inline operator const int*() const;

		//+オペレーター  
		inline Vector2Int operator +() const;
		//-オペレーター  
		inline Vector2Int operator -() const;
		//[]オペレーター
		inline int& operator [](int i);
		//==オペレーター
		inline bool operator ==(const Vector2Int& vec) const;
		//!=オペレーター
		inline bool operator !=(const Vector2Int& vec) const;

		//加算オペレーター
		inline Vector2Int operator +(const Vector2Int& vec) const;
		//減算オペレーター
		inline Vector2Int operator -(const Vector2Int& vec) const;
		//乗算オペレーター
		inline Vector2Int operator *(const Vector2Int& vec) const;
		//乗算オペレーター(int)
		inline Vector2Int operator *(int value) const;
		//除算オペレーター
		inline Vector2Int operator /(const Vector2Int& vec) const;
		//除算オペレーター(int)
		inline Vector2Int operator /(int value) const;

		//+=オペレーター
		inline const Vector2Int &operator +=(const Vector2Int& vec);
		//-=オペレーター
		inline const Vector2Int &operator -=(const Vector2Int& vec);
		//*=オペレーター
		inline const Vector2Int &operator *=(const Vector2Int& vec);
		//*=オペレーター(int)
		inline const Vector2Int &operator *=(int value);
		// /=オペレーター
		inline const Vector2Int &operator /=(const Vector2Int& vec);
		// /=オペレーター(int)
		inline const Vector2Int &operator /=(int value);


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
		//引数1: Vector2
		Vector2Int(const Vector2& value);

		//----------------------------------------------------------------------------------
		//[ToFloat]
		//Vector2に変換を行う
		//return: Vector2に変換したthis
		Vector2 ToFloat() const;

		//Vector3Intオペレーター
		operator Vector3Int() const;
		//Vector4Intオペレーター
		operator Vector4Int() const;

		//=オペレーター
		const Vector2Int& operator = (const Vector3Int& vec);
		//=オペレーター
		const Vector2Int& operator = (const Vector4Int& vec);
		//=オペレーター
		const Vector2Int& operator = (const Vector2& vec);
		//=オペレーター
		const Vector2Int& operator = (const Vector2Ref& vec);
	};


	//----------------------------------------------------------------------------------
	//[wostream operator]
	//引数1: stream
	//引数2: this
	inline std::wostream& operator<<(std::wostream& stream, const Vector2Int& value)
	{
		const wchar* string = value.ToString();
		stream << string;
		return stream;
	}

	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//全て0に設定される
	inline constexpr Vector2Int::Vector2Int() : x(0), y(0) {}
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//代入初期化する
	//引数1: x
	//引数2: y
	inline constexpr Vector2Int::Vector2Int(int x, int y) : x(x), y(y) {}
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//代入初期化する
	//引数1: int*
	inline constexpr Vector2Int::Vector2Int(const int * array) : x(array[0]), y(array[1]) {}


	//----------------------------------------------------------------------------------
	//[Absolute]
	//return: 絶対値
	inline Vector2Int Vector2Int::Absolute() const noexcept
	{
		return std::move(Vector2Int(x < 0 ? -x : x, y < 0 ? -y : y));
	}
	//----------------------------------------------------------------------------------
	//[Magnitude]
	//return: 自身とVector(0,0)の距離
	inline constexpr float Vector2Int::Magnitude() const noexcept
	{
		float f = static_cast<float>(x * x + y * y);
		int i = 0x5f3759df - (*reinterpret_cast<int*>(&f) >> 1);
		float bitF = *reinterpret_cast<float*>(&i);

		bitF *= (1.5f - 0.5f * f * bitF * bitF);
		return f * bitF * (1.5f - 0.5f * f * bitF * bitF);
	}
	//----------------------------------------------------------------------------------
	//[ExactMagnitude]
	//return: 自身とVector(0,0)の距離 (正確な値だが負荷が高い)
	inline float Vector2Int::ExactMagnitude() const noexcept
	{
		return std::sqrtf(static_cast<float>(x * x + y * y));
	}

	//int*オペレーター
	inline Vector2Int::operator int*() { return value; }
	//const int*オペレーター
	inline Vector2Int::operator const int*() const { return value; }

	//-オペレーター  
	inline Vector2Int Vector2Int::operator+() const { return Vector2Int(+this->x, +this->y); }
	//-オペレーター  
	inline Vector2Int Vector2Int::operator-() const { return Vector2Int(-this->x, -this->y); }
	//[]オペレーター
	inline int& Vector2Int::operator[](int i) { return value[i]; }
	//==オペレーター
	inline bool Vector2Int::operator==(const Vector2Int& vec) const { return (x == vec.x && y == vec.y); }
	//!=オペレーター
	inline bool Vector2Int::operator!=(const Vector2Int& vec) const { return (x != vec.x || y != vec.y); }

	//加算オペレーター
	inline Vector2Int Vector2Int::operator+(const Vector2Int& vec) const { return Vector2Int(x + vec.x, y + vec.y); }
	//減算オペレーター
	inline Vector2Int Vector2Int::operator-(const Vector2Int& vec) const { return Vector2Int(x - vec.x, y - vec.y); }
	//乗算オペレーター
	inline Vector2Int Vector2Int::operator*(const Vector2Int& vec) const { return Vector2Int(x * vec.x, y * vec.y); }
	//乗算オペレーター(int)
	inline Vector2Int Vector2Int::operator*(int value) const { return Vector2Int(x * value, y * value); }
	//除算オペレーター
	inline Vector2Int Vector2Int::operator/(const Vector2Int& vec) const { return Vector2Int(x / vec.x, y / vec.y); }
	//除算オペレーター(int)
	inline Vector2Int Vector2Int::operator/(int value) const { return Vector2Int(x / value, y / value); }

	//+=オペレーター
	inline const Vector2Int & Vector2Int::operator+=(const Vector2Int& vec)
	{
		this->x += vec.x;		this->y += vec.y;
		return *this;
	}
	//-=オペレーター
	inline const Vector2Int & Vector2Int::operator-=(const Vector2Int& vec)
	{
		this->x -= vec.x;		this->y -= vec.y;
		return *this;
	}
	//*=オペレーター
	inline const Vector2Int & Vector2Int::operator*=(const Vector2Int& vec)
	{
		this->x *= vec.x;		this->y *= vec.y;
		return *this;
	}
	//*=オペレーター(int)
	inline const Vector2Int & Vector2Int::operator*=(int value)
	{
		this->x *= value;		this->y *= value;
		return *this;
	}
	// /=オペレーター
	inline const Vector2Int& Vector2Int::operator/=(const Vector2Int& vec)
	{
		this->x /= vec.x;		this->y /= vec.y;
		return *this;
	}
	// /=オペレーター(int)
	inline const Vector2Int & Vector2Int::operator/=(int value)
	{
		this->x /= value;		this->y /= value;
		return *this;
	}
}
#endif //!_VECTOR_VECTOR2INT_HPP_