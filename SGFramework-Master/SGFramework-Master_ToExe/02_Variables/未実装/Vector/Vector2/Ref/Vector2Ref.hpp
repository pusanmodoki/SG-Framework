/*----------------------------------------------------------------------------------
2つの値を扱えるVector2Ref (参照) 構造体
最も多用する構造体シリーズの1つなので別自作Vectorが関係ないものは全てinline実装
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_VECTOR_VECTOR2_REF_HPP_
#define SGFRAMEWORK_HEADER_VECTOR_VECTOR2_REF_HPP_
#include <DirectXMath.h>
#include <PxPhysicsAPI.h>
#include <assimp/vector2.h>
#include "imgui.h"
#include <iostream>
#include "../../../ConstAndUsing/ConstAndUsing.hpp"
#include "../../../Property/Property.hpp"
#include "../../../String/String.hpp"
#include "../Vector2.hpp"

//Framework namespace
namespace SGFramework
{
	//プロトタイプ宣言
	struct Vector3;
	struct Vector4;
	struct Vector2Int;

	//2つの値を扱えるVector2RefRef 構造体
	struct Vector2Ref final
	{
	public:
		//----------------------------------------------------------------------------------
		//inlne実装

		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化する
		//引数1:x
		//引数2:y
		inline Vector2Ref(float& x, float& y);
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化する
		//引数1: Vector2
		inline Vector2Ref(DirectX::XMFLOAT2& value);

		//----------------------------------------------------------------------------------
		//[ToXmVector]
		//return: XmVector
		inline XmVector ToXmVector() const noexcept { return DirectX::XMLoadFloat2(&DirectX::XMFLOAT2(x, y)); }
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
		Vector2Ref(const Vector2Ref&) = default;
		Vector2Ref& operator=(const Vector2Ref&) = default;
		Vector2Ref(Vector2Ref&&) = default;
		Vector2Ref& operator=(Vector2Ref&&) = default;
	
		//----------------------------------------------------------------------------------
		//変数
		float& x;		//X
		float& y;		//Y

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
		
		//+オペレーター  
		inline Vector2 operator +() const;
		//-オペレーター  
		inline Vector2 operator -() const;
		//==オペレーター
		inline bool operator ==(const Vector2Ref& vec) const;
		//!=オペレーター
		inline bool operator !=(const Vector2Ref& vec) const;

		//加算オペレーター
		inline Vector2 operator +(const Vector2Ref& vec) const;
		//減算オペレーター
		inline Vector2 operator -(const Vector2Ref& vec) const;
		//乗算オペレーター
		inline Vector2 operator *(const Vector2Ref& vec) const;
		//乗算オペレーター(float)
		inline Vector2 operator *(float value) const;
		//除算オペレーター
		inline Vector2 operator /(const Vector2Ref& vec) const;
		//除算オペレーター(float)
		inline Vector2 operator /(float value) const;

		//+=オペレーター
		inline const Vector2Ref &operator +=(const Vector2Ref& vec);
		//-=オペレーター
		inline const Vector2Ref &operator -=(const Vector2Ref& vec);
		//*=オペレーター
		inline const Vector2Ref &operator *=(const Vector2Ref& vec);
		//*=オペレーター(float)
		inline const Vector2Ref &operator *=(float value);
		// /=オペレーター
		inline const Vector2Ref &operator /=(const Vector2Ref& vec);
		// /=オペレーター(float)
		inline const Vector2Ref &operator /=(float value);

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
		//Vector2

		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化する
		//引数1: Vector2
		inline Vector2Ref(Vector2& value);
		//----------------------------------------------------------------------------------
		//[ToVector2]
		//return: Vector2
		inline Vector2 ToVector2() const { return Vector2(*this); }

		//= オペレーター
		inline const Vector2Ref& operator = (const Vector2& vec);

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
		//除算オペレーター
		inline Vector2 operator /(const Vector2& vec) const;
		
		//+=オペレーター
		inline const Vector2Ref &operator +=(const Vector2& vec);
		//-=オペレーター
		inline const Vector2Ref &operator -=(const Vector2& vec);
		//*=オペレーター
		inline const Vector2Ref &operator *=(const Vector2& vec);
		// /=オペレーター
		inline const Vector2Ref &operator /=(const Vector2& vec);

		//= オペレーター
		inline const Vector2Ref& operator = (const DirectX::XMFLOAT2& vec);

		//==オペレーター
		inline bool operator ==(const DirectX::XMFLOAT2& vec) const;
		//!=オペレーター
		inline bool operator !=(const DirectX::XMFLOAT2& vec) const;

		//加算オペレーター
		inline Vector2 operator +(const DirectX::XMFLOAT2& vec) const;
		//減算オペレーター
		inline Vector2 operator -(const DirectX::XMFLOAT2& vec) const;
		//乗算オペレーター
		inline Vector2 operator *(const DirectX::XMFLOAT2& vec) const;
		//除算オペレーター
		inline Vector2 operator /(const DirectX::XMFLOAT2& vec) const;

		//+=オペレーター
		inline const Vector2Ref &operator +=(const DirectX::XMFLOAT2& vec);
		//-=オペレーター
		inline const Vector2Ref &operator -=(const DirectX::XMFLOAT2& vec);
		//*=オペレーター
		inline const Vector2Ref &operator *=(const DirectX::XMFLOAT2& vec);
		// /=オペレーター
		inline const Vector2Ref &operator /=(const DirectX::XMFLOAT2& vec);


		//----------------------------------------------------------------------------------
		//cpp実装

		//Vector3オペレーター
		operator Vector3() const;
		//Vector4オペレーター
		operator Vector4() const;
		//Vector2Intオペレーター
		operator Vector2Int() const;

		//=オペレーター
		const Vector2Ref& operator = (const Vector3& vec);
		//=オペレーター
		const Vector2Ref& operator = (const Vector4& vec);
		//=オペレーター
		const Vector2Ref& operator = (const Vector2Int& vec);
	};


	//----------------------------------------------------------------------------------
	//[wostream operator]
	//引数1: stream
	//引数2: this
	inline std::wostream& operator<<(std::wostream& stream, const Vector2Ref& value)
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
	inline Vector2Ref::Vector2Ref(float& x, float& y) : x(x), y(y) {};
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//代入初期化する
	//引数1: XMFLOAT2
	inline Vector2Ref::Vector2Ref(DirectX::XMFLOAT2 & vec) : x(vec.x), y(vec.y) { }

	//----------------------------------------------------------------------------------
	//[Absolute]
	//return: 絶対値
	inline Vector2 Vector2Ref::Absolute() const noexcept
	{
		return Vector2(x < 0.0f ? -x : x, y < 0.0f ? -y : y);
	}
	//----------------------------------------------------------------------------------
	//[Magnitude]
	//return: 自身とVector(0,0)の距離
	inline constexpr float Vector2Ref::Magnitude() const noexcept
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
	inline float Vector2Ref::ExactMagnitude() const noexcept
	{
		return std::sqrtf(x * x + y * y);
	}
	//----------------------------------------------------------------------------------
	//[ClampMagnitude]
	//vecのLengthをmaxLengthに制限する
	//return: 制限したvector
	//引数2: 制限Length
	inline Vector2 Vector2Ref::ClampMagnitude(const float maxLength) const noexcept
	{
		return std::move(Vector2(DirectX::XMVector2ClampLength(this->ToXmVector(), 0.0f, maxLength)));
	}
	//----------------------------------------------------------------------------------
	//[Normalize]
	//return: 自身の正規化ベクトル
	inline Vector2 Vector2Ref::Normalized() const noexcept
	{
		float divLength = 1.0f / ExactMagnitude();
		return std::move(Vector2(x * divLength, y * divLength));
	}

	//XMVECTORオペレーター
	inline Vector2Ref::operator XmVector() const noexcept { return DirectX::XMLoadFloat2(&DirectX::XMFLOAT2(x, y)); }

	//+オペレーター  
	inline Vector2 Vector2Ref::operator+() const { return Vector2(+this->x, +this->y); }
	//-オペレーター  
	inline Vector2 Vector2Ref::operator-() const { return Vector2(-this->x, -this->y); }
	//==オペレーター
	inline bool Vector2Ref::operator==(const Vector2Ref& vec) const
	{
		return (fabsf(x - vec.x) <= MathAF::cEpsilon
			&& fabsf(y - vec.y) <= MathAF::cEpsilon);
	}
	//!=オペレーター
	inline bool Vector2Ref::operator!=(const Vector2Ref& vec) const
	{
		return (fabsf(x - vec.x) > MathAF::cEpsilon
			&& fabsf(y - vec.y) > MathAF::cEpsilon);
	}

	//加算オペレーター
	inline Vector2 Vector2Ref::operator+(const Vector2Ref& vec) const { return Vector2(x + vec.x, y + vec.y); }
	//減算オペレーター
	inline Vector2 Vector2Ref::operator-(const Vector2Ref& vec) const { return Vector2(x - vec.x, y - vec.y); }
	//乗算オペレーター
	inline Vector2 Vector2Ref::operator*(const Vector2Ref& vec) const { return Vector2(x * vec.x, y * vec.y); }
	//乗算オペレーター(float)
	inline Vector2 Vector2Ref::operator*(const float value) const { return Vector2(x * value, y * value); }
	//除算オペレーター
	inline Vector2 Vector2Ref::operator/(const Vector2Ref& vec) const { return Vector2(x / vec.x, y / vec.y); }
	//除算オペレーター(float)
	inline Vector2 Vector2Ref::operator/(const float value) const { return Vector2(x / value, y / value); }

	//+=オペレーター
	inline const Vector2Ref & Vector2Ref::operator+=(const Vector2Ref& vec)
	{
		this->x += vec.x;		this->y += vec.y;
		return *this;
	}
	//-=オペレーター
	inline const Vector2Ref & Vector2Ref::operator-=(const Vector2Ref& vec)
	{
		this->x -= vec.x;		this->y -= vec.y;
		return *this;
	}
	//*=オペレーター
	inline const Vector2Ref & Vector2Ref::operator*=(const Vector2Ref& vec)
	{
		this->x *= vec.x;		this->y *= vec.y;
		return *this;
	}
	//*=オペレーター(float)
	inline const Vector2Ref & Vector2Ref::operator*=(const float value)
	{
		this->x *= value;		this->y *= value;
		return *this;
	}
	// /=オペレーター
	inline const Vector2Ref & Vector2Ref::operator/=(const Vector2Ref& vec)
	{
		this->x /= vec.x;		this->y /= vec.y;
		return *this;
	}
	// /=オペレーター(float)
	inline const Vector2Ref & Vector2Ref::operator/=(const float value)
	{
		this->x /= value;		this->y /= value;
		return *this;
	}

	//----------------------------------------------------------------------------------
	//Vector2

	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//代入初期化する
	//引数1: Vector2
	Vector2Ref::Vector2Ref(Vector2 & value) : x(value.x), y(value.y) {}

	//= オペレーター
	inline const Vector2Ref & Vector2Ref::operator=(const Vector2 & vec)
	{
		x = vec.x; y = vec.y;
		return *this;
	}
	
	//==オペレーター
	inline bool Vector2Ref::operator==(const Vector2 & vec) const
	{
		return (fabsf(x - vec.x) <= MathAF::cEpsilon
			&& fabsf(y - vec.y) <= MathAF::cEpsilon);
	}
	//!=オペレーター
	inline bool Vector2Ref::operator!=(const Vector2 & vec) const
	{
		return (fabsf(x - vec.x) > MathAF::cEpsilon
			&& fabsf(y - vec.y) > MathAF::cEpsilon);
	}

	//加算オペレーター
	inline Vector2 Vector2Ref::operator+(const Vector2 & vec) const { return Vector2(x + vec.x, y + vec.y);}
	//減算オペレーター
	inline Vector2 Vector2Ref::operator-(const Vector2 & vec) const { return Vector2(x - vec.x, y - vec.y); }
	//乗算オペレーター
	inline Vector2 Vector2Ref::operator*(const Vector2 & vec) const { return Vector2(x * vec.x, y * vec.y); }
	//除算オペレーター
	inline Vector2 Vector2Ref::operator/(const Vector2 & vec) const { return Vector2(x  / vec.x, y / vec.y); }

	inline const Vector2Ref & Vector2Ref::operator+=(const Vector2 & vec)
	{
		this->x += vec.x;		this->y += vec.y;
		return *this;
	}

	inline const Vector2Ref & Vector2Ref::operator-=(const Vector2 & vec)
	{
		this->x -= vec.x;		this->y -= vec.y;
		return *this;
	}

	inline const Vector2Ref & Vector2Ref::operator*=(const Vector2 & vec)
	{
		this->x *= vec.x;		this->y *= vec.y;
		return *this;
	}

	inline const Vector2Ref & Vector2Ref::operator/=(const Vector2 & vec)
	{
		this->x /= vec.x;		this->y /= vec.y;
		return *this;
	}

	//= オペレーター
	inline const Vector2Ref & Vector2Ref::operator=(const DirectX::XMFLOAT2 & vec)
	{
		x = vec.x; y = vec.y;
		return *this;
	}

	//==オペレーター
	inline bool Vector2Ref::operator==(const DirectX::XMFLOAT2& vec) const
	{
		return (fabsf(x - vec.x) <= MathAF::cEpsilon
			&& fabsf(y - vec.y) <= MathAF::cEpsilon);
	}
	//!=オペレーター
	inline bool Vector2Ref::operator!=(const DirectX::XMFLOAT2& vec) const
	{
		return (fabsf(x - vec.x) > MathAF::cEpsilon
			&& fabsf(y - vec.y) > MathAF::cEpsilon);
	}

	//加算オペレーター
	inline Vector2 Vector2Ref::operator+(const DirectX::XMFLOAT2& vec) const { return Vector2(x + vec.x, y + vec.y); }
	//減算オペレーター
	inline Vector2 Vector2Ref::operator-(const DirectX::XMFLOAT2& vec) const { return Vector2(x - vec.x, y - vec.y); }
	//乗算オペレーター
	inline Vector2 Vector2Ref::operator*(const DirectX::XMFLOAT2& vec) const { return Vector2(x * vec.x, y * vec.y); }
	//除算オペレーター
	inline Vector2 Vector2Ref::operator/(const DirectX::XMFLOAT2& vec) const { return Vector2(x / vec.x, y / vec.y); }

	inline const Vector2Ref & Vector2Ref::operator+=(const DirectX::XMFLOAT2& vec)
	{
		this->x += vec.x;		this->y += vec.y;
		return *this;
	}

	inline const Vector2Ref & Vector2Ref::operator-=(const DirectX::XMFLOAT2& vec)
	{
		this->x -= vec.x;		this->y -= vec.y;
		return *this;
	}

	inline const Vector2Ref & Vector2Ref::operator*=(const DirectX::XMFLOAT2& vec)
	{
		this->x *= vec.x;		this->y *= vec.y;
		return *this;
	}

	inline const Vector2Ref & Vector2Ref::operator/=(const DirectX::XMFLOAT2& vec)
	{
		this->x /= vec.x;		this->y /= vec.y;
		return *this;
	}
}
#endif // !SGFRAMEWORK_HEADER_VECTOR_VECTOR2_REF_HPP_
