/*----------------------------------------------------------------------------------
2つの値を扱えるUV 構造体
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_UV_HPP_
#define SGFRAMEWORK_HEADER_UV_HPP_
#include <DirectXMath.h>
#include <cmath>
#include <iostream>
#include "../ConstAndUsing/ConstAndUsing.hpp"
#include "../Property/Property.hpp"
#include "../String/String.hpp"
#include "../Vector/Vector.hpp"

//Framework namespace
namespace SGFramework
{
	//2つの値を扱えるUV 構造体
	struct UV final
	{
	public:
		//----------------------------------------------------------------------------------
		//inlne実装

		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//全て0に設定される
		inline constexpr UV() : u(0.0f), v(0.0f) {};
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化する
		//引数1:x
		//引数2:y
		inline constexpr UV(float u, float v) : u(u), v(v) {}
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化する
		//引数1: XMFLOAT2
		inline constexpr UV(const Vector2& value) : u(value.x), v(value.y) {}
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化する
		//引数1: float*
		inline constexpr UV(const float* array) : u(array[0]), v(array[1]) {}

		//----------------------------------------------------------------------------------
		//変数
		union
		{
			struct
			{
				float u;		//U
				float v;		//V
			};
			float value[2];	//Value
		};

		//----------------------------------------------------------------------------------
		//[Absolute]
		//return: 絶対値
		inline UV Absolute() const noexcept;
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
		inline UV Normalized() const noexcept;

		//float*オペレーター
		inline operator float*() { return (float*)this->value; }
		//const float*オペレーター
		inline operator const float*() const { return (const float*)this->value; }
		//Vector2オペレーター
		inline operator Vector2() const { return Vector2(u, v); }
		//=オペレーター
		inline Vector2 operator = (const Vector2& vec) { this->u = vec.x; this->v = vec.y; }
		//+オペレーター  
		inline UV operator +() const { return UV(+u, +v); }
		//-オペレーター  
		inline UV operator -() const { return UV(-u, -v); }
		//==オペレーター
		inline bool operator ==(const UV& uv) const
		{
			return (fabsf(u - uv.u) <= MathAF::cEpsilon
				&& fabsf(v - uv.v) <= MathAF::cEpsilon);
		}
		//!=オペレーター
		inline bool operator !=(const UV& uv) const
		{
			return (fabsf(u - uv.u) > MathAF::cEpsilon
				&& fabsf(v - uv.v) > MathAF::cEpsilon);
		}

		//加算オペレーター
		inline UV operator +(const UV& uv) const { return UV(u + uv.u, v + uv.v); }
		//減算オペレーター
		inline UV operator -(const UV& uv) const { return UV(u - uv.v, v - uv.v); }
		//乗算オペレーター
		inline UV operator *(const UV& uv) const { return UV(u * uv.v, v * uv.v); }
		//乗算オペレーター(float)
		inline UV operator *(const float value) const { return UV(u * value, v * value); }
		//除算オペレーター
		inline UV operator /(const UV& uv) const { return UV(u / uv.v, v / uv.v); }
		//除算オペレーター(float)
		inline UV operator /(const float value) const { return UV(u / value, v / value); }

		//+=オペレーター
		inline const UV &operator +=(const UV& uv) { this->u += uv.u; this->v += uv.v; return *this; }
		//-=オペレーター
		inline const UV &operator -=(const UV& uv) { this->u -= uv.u; this->v -= uv.v; return *this; }
		//*=オペレーター
		inline const UV &operator *=(const UV& uv) { this->u *= uv.u; this->v *= uv.v; return *this; }
		//*=オペレーター(float)
		inline const UV &operator *=(const float value) { this->u *= value; this->v *= value; return *this; }
		// /=オペレーター
		inline const UV &operator /=(const UV& uv) { this->u /= u; this->v /= v; return *this; }
		// /=オペレーター(float)
		inline const UV &operator /=(const float value) { this->u /= value; this->v /= value; return *this; }

		//sgstring operator
		inline operator sgstring() const { return ToString(); }
		//----------------------------------------------------------------------------------
		//[ToString]
		//sgstring形式で出力する
		//return: sgstring表現したthis
		inline sgstring ToString() const { return (L"(" + std::to_wstring(u) + L", " + std::to_wstring(v) + L")"); }
		//----------------------------------------------------------------------------------
		//[ToStdString]
		//std::string形式で出力する
		//return: std::string表現したthis
		inline std::string ToStdString() const { return ("(" + std::to_string(u) + ", " + std::to_string(v) + ")"); }


		//計算ライブラリ


		//----------------------------------------------------------------------------------
		//[Distance]<Calculation Library Function>
		//fromからtoの距離を測る
		//return: from-toの距離
		//引数1: 対象ベクトル1
		//引数2: 対象ベクトル2
		inline constexpr float Distance(const UV& from, const UV& to) noexcept
		{
			float f = (from.u - to.u) * (from.u - to.u)
				+ (from.v - to.v) * (from.v - to.v);
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
		inline float ExactDistance(const UV& from, const UV& to) noexcept
		{
			return std::sqrtf((to.u - from.u) * (to.u - from.u) + (to.v - from.v) * (to.v - from.v));
		}
		//----------------------------------------------------------------------------------
		//[Direction]<Calculation Library Function>
		//fromからtoへの正規化済方向ベクトルを取得する
		//return: 正規化済方向ベクトル
		//引数1: 対象ベクトル1
		//引数2: 対象ベクトル2
		inline UV Direction(const UV& from, const UV& to) noexcept
		{
			return std::move((to - from).Normalized());
		}
		//----------------------------------------------------------------------------------
		//[Max]<Calculation Library Function>
		//2つのUVの各成分の一番大きな値を使ってUVを作成
		//return: 最大UV
		//引数1: 対象UV1
		//引数2: 対象UV2
		inline static UV Max(const UV& uv1, const UV& uv2)
		{
			UV ret;
			if (uv1.u >= uv2.u) ret.u = uv1.u; else ret.u = uv2.u;
			if (uv1.v >= uv2.v) ret.v = uv1.v; else ret.v = uv2.v;
			return ret;
		}
		//----------------------------------------------------------------------------------
		//[Min]<Calculation Library Function>
		//2つのUVの各成分の一番小さい値を使ってUVを作成
		//return: 最小ベクトル
		//引数1: 対象UV1
		//引数2: 対象UV2
		inline static UV Min(const UV& uv1, const UV& uv2)
		{
			UV ret;
			if (uv1.u <= uv2.u) ret.u = uv1.u; else ret.u = uv2.u;
			if (uv1.v <= uv2.v) ret.v = uv1.v; else ret.v = uv2.v;
			return ret;
		}
	};



	//----------------------------------------------------------------------------------
	//[wostream operator]
	//引数1: stream
	//引数2: this
	inline std::wostream& operator<<(std::wostream& stream, const UV& value)
	{
		const wchar* string = value.ToString();
		stream << string;
		return stream;
	}


	//----------------------------------------------------------------------------------
	//[Absolute]
	//return: 絶対値
	inline UV UV::Absolute() const noexcept
	{
		return UV(u < 0.0f ? -u : u, v < 0.0f ? -v : v);
	}
	//----------------------------------------------------------------------------------
	//[Magnitude]
	//return: 自身とVector(0,0)の距離
	inline constexpr float UV::Magnitude() const  noexcept
	{
		float f = u * u + v * v;
		int i = 0x5f3759df - (*reinterpret_cast<int*>(&f) >> 1);
		float bitF = *reinterpret_cast<float*>(&i);

		bitF *= (1.5f - 0.5f * f * bitF * bitF);
		return f * bitF * (1.5f - 0.5f * f * bitF * bitF);
	}
	//----------------------------------------------------------------------------------
	//[ExactMagnitude]
	//return: 自身とVector(0,0,0,0)の距離 (正確な値だが負荷が高い)
	inline float UV::ExactMagnitude() const noexcept
	{
		return std::sqrtf(u * u + v * v);
	}
	//----------------------------------------------------------------------------------
	//[Normalize]
	//return: 自身の正規化ベクトル
	inline UV UV::Normalized() const noexcept
	{
		float divLength = 1.0f / ExactMagnitude();
		return UV(u * divLength, v * divLength);
	}
}
#endif // !SGFRAMEWORK_HEADER_UV_HPP_
