/*----------------------------------------------------------------------------------
3つの値を扱えるVector3Int 構造体
最も多用する構造体シリーズの1つなので別自作Vectorが関係ないものは全てinline実装
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_VECTOR_VECTOR3INT_HPP_
#define SGFRAMEWORK_HEADER_VECTOR_VECTOR3INT_HPP_
#include <PxPhysicsAPI.h>
#include <assimp/vector3.h>
#include <DirectXMath.h>
#include <iostream>
#include "../../../ConstAndUsing/ConstAndUsing.hpp"
#include "../../../Property/Property.hpp"
#include "../../../String/String.hpp"

//Framework namespace
namespace SGFramework
{
	//プロトタイプ宣言
	struct Vector2Int;
	struct Vector4Int;
	struct Vector3;
	struct Vector3Ref;

	//3つの値を扱えるVector3Int 構造体
	struct Vector3Int final
	{
		//----------------------------------------------------------------------------------
		//inlne実装

		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//全て0に設定される
		inline constexpr Vector3Int();
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化する
		//引数1: x
		//引数2: y
		//引数3: z
		inline constexpr Vector3Int(int x, int y, int z);
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化する
		//引数1: int*
		inline Vector3Int(const int* array);

		//----------------------------------------------------------------------------------
		//[ToXmVector]
		//return: XmVector
		inline XmVector ToXmVector() const noexcept {
			return DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(
				static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)));
		}
		//----------------------------------------------------------------------------------
		//[ToPhysX]
		//return: physx::PxVec3
		inline physx::PxVec3 ToPhysX() const noexcept { return physx::PxVec3(
			static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)); }
		//----------------------------------------------------------------------------------
		//[ToAssimp]
		//return: aiVector3D
		inline aiVector3D ToAssimp() const noexcept { return aiVector3D(
			static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)); }

		//default
		Vector3Int(const Vector3Int&) = default;
		Vector3Int& operator=(const Vector3Int&) = default;
		Vector3Int(Vector3Int&&) = default;
		Vector3Int& operator=(Vector3Int&&) = default;

		//----------------------------------------------------------------------------------
		//変数
		union
		{
			struct
			{
				int x;		//X
				int y;		//Y
				int z;		//Z
			};
			int value[3];	//Value
		};

		//----------------------------------------------------------------------------------
		//[Absolute]
		//return: 絶対値
		inline Vector3Int Absolute() const noexcept;
		//----------------------------------------------------------------------------------
		//[Magnitude]
		//return: 自身とVector(0,0,0)の距離
		inline constexpr float Magnitude() const noexcept;
		//----------------------------------------------------------------------------------
		//[ExactMagnitude]
		//return: 自身とVector(0,0,0)の距離 (正確な値だが負荷が高い)
		inline float ExactMagnitude() const noexcept;

		//int*オペレーター
		inline operator int* ();
		//const int*オペレーター
		inline operator const int*() const;

		//-オペレーター  
		inline Vector3Int operator +() const;
		//-オペレーター  
		inline Vector3Int operator -() const;
		//==オペレーター
		inline bool operator ==(const Vector3Int& vec) const;
		//!=オペレーター
		inline bool operator !=(const Vector3Int& vec) const;

		//加算オペレーター
		inline Vector3Int operator +(const Vector3Int& vec) const;
		//減算オペレーター
		inline Vector3Int operator -(const Vector3Int& vec) const;
		//乗算オペレーター
		inline Vector3Int operator *(const Vector3Int& vec) const;
		//乗算オペレーター(int)
		inline Vector3Int operator *(int value) const;
		//除算オペレーター
		inline Vector3Int operator /(const Vector3Int& vec) const;
		//除算オペレーター(int)
		inline Vector3Int operator /(int value) const;

		//+=オペレーター
		inline const Vector3Int &operator +=(const Vector3Int& vec);
		//-=オペレーター
		inline const Vector3Int &operator -=(const Vector3Int& vec);
		//*=オペレーター
		inline const Vector3Int &operator *=(const Vector3Int& vec);
		//*=オペレーター(int)
		inline const Vector3Int &operator *=(int value);
		// /=オペレーター
		inline const Vector3Int &operator /=(const Vector3Int& vec);
		// /=オペレーター(int)
		inline const Vector3Int &operator /=(int value);


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
		//引数1: Vector3
		Vector3Int(const Vector3& value);

		//----------------------------------------------------------------------------------
		//[ToFloat]
		//Vector3に変換を行う
		//return: Vector3に変換したthis
		Vector3 ToFloat() const;

		//Vector2Intオペレーター
		operator Vector2Int() const;
		//Vector4Intオペレーター
		operator Vector4Int() const;

		//=オペレーター
		const Vector3Int& operator = (const Vector2Int& vec);
		//=オペレーター
		const Vector3Int& operator = (const Vector4Int& vec);
		//=オペレーター
		const Vector3Int& operator = (const Vector3& vec);
		//=オペレーター
		const Vector3Int& operator = (const Vector3Ref& vec);
	};

	//----------------------------------------------------------------------------------
	//[wostream operator]
	//引数1: stream
	//引数2: this
	inline std::wostream& operator<<(std::wostream& stream, const Vector3Int& value)
	{
		const wchar* string = value.ToString();
		stream << string;
		return stream;
	}

	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//全て0に設定される
	inline constexpr Vector3Int::Vector3Int() : x(0), y(0), z(0) {}
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//代入初期化する
	//引数1: x
	//引数2: y
	//引数3: z
	inline constexpr Vector3Int::Vector3Int(int x, int y, int z) : x(x), y(y), z(z) {}
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//代入初期化する
	//引数1: int*
	inline Vector3Int::Vector3Int(const int * array)
	{
		value[0] = array[0];	value[1] = array[1];
		value[2] = array[2];
	}

	//----------------------------------------------------------------------------------
	//[Absolute]
	//return: 絶対値
	inline Vector3Int Vector3Int::Absolute() const noexcept
	{
		return std::move(Vector3Int(x < 0 ? -x : x, y < 0 ? -y : y, z < 0 ? -z : z));
	}
	//----------------------------------------------------------------------------------
	//[Magnitude]
	//return: 自身とVector(0,0,0)の距離
	inline constexpr float Vector3Int::Magnitude() const noexcept
	{
		float f = static_cast<float>(x * x + y * y + z * z);
		int i = 0x5f3759df - (*reinterpret_cast<int*>(&f) >> 1);
		float bitF = *reinterpret_cast<float*>(&i);

		bitF *= (1.5f - 0.5f * f * bitF * bitF);
		return f * bitF * (1.5f - 0.5f * f * bitF * bitF);
	}
	//----------------------------------------------------------------------------------
	//[ExactMagnitude]
	//return: 自身とVector(0,0,0)の距離 (正確な値だが負荷が高い)
	inline float Vector3Int::ExactMagnitude() const noexcept
	{
		return std::sqrtf(static_cast<float>(x * x + y * y + z * z));
	}

	//int*オペレーター
	inline Vector3Int::operator int*() { return value; }
	//const int*オペレーター
	inline Vector3Int::operator const int*() const { return value; }

	//+オペレーター  
	inline Vector3Int Vector3Int::operator+() const { return Vector3Int(+this->x, +this->y, +this->z); }
	//-オペレーター  
	inline Vector3Int Vector3Int::operator-() const { return Vector3Int(-this->x, -this->y, -this->z); }
	//==オペレーター
	inline bool Vector3Int::operator==(const Vector3Int& vec) const { return (x == vec.x && y == vec.y && z == vec.z); }
	//!=オペレーター
	inline bool Vector3Int::operator!=(const Vector3Int& vec) const { return (x != vec.x || y != vec.y || z != vec.z); }

	//加算オペレーター
	inline Vector3Int Vector3Int::operator+(const Vector3Int& vec) const { return Vector3Int(x + vec.x, y + vec.y, z + vec.z); }
	//減算オペレーター
	inline Vector3Int Vector3Int::operator-(const Vector3Int& vec) const { return Vector3Int(x - vec.x, y - vec.y, z - vec.z); }
	//乗算オペレーター
	inline Vector3Int Vector3Int::operator*(const Vector3Int& vec) const { return Vector3Int(x * vec.x, y * vec.y, z * vec.z); }
	//乗算オペレーター(int)
	inline Vector3Int Vector3Int::operator*(int value) const { return Vector3Int(x * value, y * value, z * value); }
	//除算オペレーター
	inline Vector3Int Vector3Int::operator/(const Vector3Int& vec) const { return Vector3Int(x / vec.x, y / vec.y, z / vec.z); }
	//除算オペレーター(int)
	inline Vector3Int Vector3Int::operator/(int value) const { return Vector3Int(x / value, y / value, z / value); }

	//+=オペレーター
	inline const Vector3Int & Vector3Int::operator+=(const Vector3Int& vec)
	{
		this->x += vec.x;		this->y += vec.y;
		this->z += vec.z;
		return *this;
	}
	//-=オペレーター
	inline const Vector3Int & Vector3Int::operator-=(const Vector3Int& vec)
	{
		this->x -= vec.x;		this->y -= vec.y;
		this->z -= vec.z;
		return *this;
	}
	//*=オペレーター
	inline const Vector3Int & Vector3Int::operator*=(const Vector3Int& vec)
	{
		this->x *= vec.x;		this->y *= vec.y;
		this->z *= vec.z;
		return *this;
	}
	//*=オペレーター(int)
	inline const Vector3Int & Vector3Int::operator*=(int value)
	{
		this->x *= value;		this->y *= value;
		this->z *= value;
		return *this;
	}
	// /=オペレーター
	inline const Vector3Int & Vector3Int::operator/=(const Vector3Int& vec)
	{
		this->x /= vec.x;		this->y /= vec.y;
		this->z /= vec.z;
		return *this;
	}
	// /=オペレーター(int)
	inline const Vector3Int & Vector3Int::operator/=(int value)
	{
		this->x /= value;		this->y /= value;
		this->z /= value;
		return *this;
	}
}
#endif //!_VECTOR_VECTOR3INT_HPP_