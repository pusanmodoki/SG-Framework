/*----------------------------------------------------------------------------------
4つのint値を扱えるVector4Int 構造体
最も多用する構造体シリーズの1つなので別自作Vectorが関係ないものは全てinline実装
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_VECTOR_VECTOR4INT_HPP_
#define SGFRAMEWORK_HEADER_VECTOR_VECTOR4INT_HPP_
#include <PxPhysicsAPI.h>
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
	struct Vector3Int;
	struct Vector4;
	struct Vector4Ref;

	//4つのint値を扱えるVector4Int 構造体
	struct Vector4Int final
	{
		//----------------------------------------------------------------------------------
		//inlne実装

		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//全て0に設定される
		inline constexpr Vector4Int();
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化する
		//引数1: x
		//引数2: y
		//引数3: z
		//引数4: w
		inline constexpr Vector4Int(int x, int y, int z, int w);
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化する
		//引数1: int*
		inline Vector4Int(const int* array);

		//----------------------------------------------------------------------------------
		//[ToXmVector]
		//return: XmVector
		inline XmVector ToXmVector() const noexcept { return DirectX::XMLoadFloat4(&DirectX::XMFLOAT4(
			static_cast<float>(x), static_cast<float>(y),
			static_cast<float>(z), static_cast<float>(w))); }
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
		inline physx::PxVec4 ToPhysX() const noexcept { return physx::PxVec4(
			static_cast<float>(x), static_cast<float>(y),
			static_cast<float>(z), static_cast<float>(w)); }

		//default
		Vector4Int(const Vector4Int&) = default;
		Vector4Int& operator=(const Vector4Int&) = default;
		Vector4Int(Vector4Int&&) = default;
		Vector4Int& operator=(Vector4Int&&) = default;

		//----------------------------------------------------------------------------------
		//変数
		union
		{
			struct
			{
				int x;		//X
				int y;		//Y
				int z;		//Z
				int w;	//W
			};
			int value[4];	//Value
		};
		
		//int*オペレーター
		inline operator int* ();
		//const int*オペレーター
		inline operator const int*() const;

		//+オペレーター  
		inline Vector4Int operator +() const;
		//-オペレーター  
		inline Vector4Int operator -() const;
		//==オペレーター
		inline bool operator ==(const Vector4Int& vec) const;
		//!=オペレーター
		inline bool operator !=(const Vector4Int& vec) const;

		//加算オペレーター
		inline Vector4Int operator +(const Vector4Int& vec) const;
		//減算オペレーター
		inline Vector4Int operator -(const Vector4Int& vec) const;
		//乗算オペレーター
		inline Vector4Int operator *(const Vector4Int& vec) const;
		//乗算オペレーター(int)
		inline Vector4Int operator *(int value) const;
		//除算オペレーター
		inline Vector4Int operator /(const Vector4Int& vec) const;
		//除算オペレーター(int)
		inline Vector4Int operator /(int value) const;

		//+=オペレーター
		inline const Vector4Int &operator +=(const Vector4Int & vec);
		//-=オペレーター
		inline const Vector4Int &operator -=(const Vector4Int & vec);
		//*=オペレーター
		inline const Vector4Int &operator *=(const Vector4Int & vec);
		//*=オペレーター(int)
		inline const Vector4Int &operator *=(int value);
		// /=オペレーター
		inline const Vector4Int &operator /=(const Vector4Int & vec);
		// /=オペレーター(int)
		inline const Vector4Int &operator /=(int value);

		//----------------------------------------------------------------------------------
		//[Absolute]
		//return: 絶対値
		inline constexpr Vector4Int Absolute() const noexcept;
		//----------------------------------------------------------------------------------
		//[Magnitude]
		//return: 自身とVector(0,0, 0, 0)の距離
		inline constexpr float Magnitude() const;
		//----------------------------------------------------------------------------------
		//[ExactMagnitude]
		//return: 自身とVector(0,0,0,0)の距離 (正確な値だが負荷が高い)
		inline float ExactMagnitude() const;

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
		//引数1: Vector4
		Vector4Int(const Vector4& value);

		//----------------------------------------------------------------------------------
		//[ToFloat]
		//Vector4に変換を行う
		//return: Vector4に変換したthis
		Vector4 ToFloat() const;

		//Vector2Intオペレーター
		operator Vector2Int() const;
		//Vector3Intオペレーター
		operator Vector3Int() const;

		//=オペレーター
		const Vector4Int& operator = (const Vector2Int& vec);
		//=オペレーター
		const Vector4Int& operator = (const Vector3Int& vec);
		//=オペレーター
		const Vector4Int& operator = (const Vector4& vec);
		//=オペレーター
		const Vector4Int& operator = (const Vector4Ref& vec);
	};


	//----------------------------------------------------------------------------------
	//[wostream operator]
	//引数1: stream
	//引数2: this
	inline std::wostream& operator<<(std::wostream& stream, const Vector4Int& value)
	{
		const wchar* string = value.ToString();
		stream << string;
		return stream;
	}


	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//全て0に設定される
	inline constexpr Vector4Int::Vector4Int()
		: x(0), y(0), z(0), w(0) {}
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//代入初期化する
	//引数1: x
	//引数2: y
	//引数3: z
	//引数4: w
	inline constexpr Vector4Int::Vector4Int(int x, int y, int z, int w)
		: x(x), y(y), z(z), w(w) {}
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//代入初期化する
	//引数1: int*
	inline Vector4Int::Vector4Int(const int * array)
		: x(array[0]), y(array[1]), z(array[2]), w(array[3]) {}

	//int*オペレーター
	inline Vector4Int::operator int*() { return value; }
	//const int*オペレーター
	inline Vector4Int::operator const int*() const { return value; }

	//+オペレーター  
	inline Vector4Int Vector4Int::operator+() const { return Vector4Int(+this->x, +this->y, +this->z, +this->w); }
	//-オペレーター  
	inline Vector4Int Vector4Int::operator-() const{ return Vector4Int(-this->x, -this->y, -this->z, -this->w); }
	//==オペレーター
	inline bool Vector4Int::operator==(const Vector4Int& vec) const{ return (x == vec.x && y == vec.y && z == vec.z && w == vec.w); }
	//!=オペレーター
	inline bool Vector4Int::operator!=(const Vector4Int& vec) const { return (x != vec.x || y != vec.y || z != vec.z || w != vec.w); }

	//加算オペレーター
	inline Vector4Int Vector4Int::operator+(const Vector4Int& vec) const { return Vector4Int(x + vec.x, y + vec.y, z + vec.z, w + vec.w); }
	//減算オペレーター
	inline Vector4Int Vector4Int::operator-(const Vector4Int& vec) const { return Vector4Int(x - vec.x, y - vec.y, z - vec.z, w - vec.w); }
	//乗算オペレーター
	inline Vector4Int Vector4Int::operator*(const Vector4Int& vec) const { return Vector4Int(x * vec.x, y * vec.y, z * vec.z, w * vec.w); }
	//乗算オペレーター(int)
	inline Vector4Int Vector4Int::operator*(int value) const { return Vector4Int(x * value, y * value, z * value, w * value); }
	//除算オペレーター
	inline Vector4Int Vector4Int::operator/(const Vector4Int& vec) const { return Vector4Int(x / vec.x, y / vec.y, z / vec.z, w / vec.w); }
	//除算オペレーター(int)
	inline Vector4Int Vector4Int::operator/(int value) const { return Vector4Int(x / value, y / value, z / value, w / value); }

	//+=オペレーター
	inline const Vector4Int & Vector4Int::operator+=(const Vector4Int & vec)
	{
		this->x += vec.x;		this->y += vec.y;
		this->z += vec.z;		this->w += vec.w;
		return *this;
	}
	//-=オペレーター
	inline const Vector4Int & Vector4Int::operator-=(const Vector4Int & vec)
	{
		this->x -= vec.x;		this->y -= vec.y;
		this->z -= vec.z;		this->w -= vec.w;
		return *this;
	}
	//*=オペレーター
	inline const Vector4Int & Vector4Int::operator*=(const Vector4Int & vec)
	{
		this->x *= vec.x;		this->y *= vec.y;
		this->z *= vec.z;		this->w *= vec.w;
		return *this;
	}
	//*=オペレーター(int)
	inline const Vector4Int & Vector4Int::operator*=(int value)
	{
		this->x *= value;		this->y *= value;
		this->z *= value;		this->w *= value;
		return *this;
	}
	// /=オペレーター
	inline const Vector4Int & Vector4Int::operator/=(const Vector4Int & vec)
	{
		this->x /= vec.x;		this->y /= vec.y;
		this->z /= vec.z;		this->w /= vec.w;
		return *this;
	}
	// /=オペレーター(int)
	inline const Vector4Int & Vector4Int::operator/=(int value)
	{
		this->x /= value;		this->y /= value;
		this->z /= value;		this->w /= value;
		return *this;
	}

	//----------------------------------------------------------------------------------
	//[Absolute]
	//return: 絶対値
	inline constexpr Vector4Int Vector4Int::Absolute() const noexcept
	{
		return std::move(Vector4Int(x < 0 ? -x : x, y < 0 ? -y : y, z < 0 ? -z : z, w < 0 ? -w : w));
	}
	//----------------------------------------------------------------------------------
	//[Magnitude]
	//return: 自身とVector(0,0, 0, 0)の距離
	inline constexpr float Vector4Int::Magnitude() const
	{
		float f = static_cast<float>(x * x + y * y + z * z + w * w);
		int i = 0x5f3759df - (*reinterpret_cast<int*>(&f) >> 1);
		float bitF = *reinterpret_cast<float*>(&i);

		bitF *= (1.5f - 0.5f * f * bitF * bitF);
		return f * bitF * (1.5f - 0.5f * f * bitF * bitF);
	}
	//----------------------------------------------------------------------------------
	//[ExactMagnitude]
	//return: 自身とVector(0,0,0,0)の距離 (正確な値だが負荷が高い)
	inline float Vector4Int::ExactMagnitude() const
	{
		return std::sqrtf(static_cast<float>(x * x + y * y + z * z + w * w));
	}
}

#endif // !SGFRAMEWORK_HEADER_VECTOR_VECTOR4INT_HPP_
