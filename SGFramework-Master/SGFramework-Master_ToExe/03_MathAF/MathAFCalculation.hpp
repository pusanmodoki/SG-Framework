/*----------------------------------------------------------------------------------
計算ライブラリなMathAF namespace (math + almost float)
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_CMATHF_DECLARATION_HPP_
#define SGFRAMEWORK_HEADER_CMATHF_DECLARATION_HPP_
#include <cmath>
#include <math.h>
#include "../02_CustomVariable/CustomVariable.hpp"
#include "MathAFBasis.hpp"
#include "MathAFFloatEdit.hpp"

//Framework namespace
namespace SGFramework
{
	//計算ライブラリ
	namespace MathAF
	{
		//----------------------------------------------------------------------------------
		//[Cos]
		//return: cosine(f)
		//引数1: f (radian)
		inline float Cos(float f) noexcept { return std::cosf(f); }
		//----------------------------------------------------------------------------------
		//[Sin]
		//return: sine(f)
		//引数1: f (radian)
		inline float Sin(float f) noexcept { return std::sinf(f); }
		//----------------------------------------------------------------------------------
		//[Tan]
		//return: tangent(f)
		//引数1: f (radian)
		inline float Tan(float f) noexcept { return std::tanf(f); }
		//----------------------------------------------------------------------------------
		//[Acos]
		//return: arcCosine(f)
		//引数1: f (radian)
		inline float Acos(float f) noexcept { return std::acosf(f); }
		//----------------------------------------------------------------------------------
		//[Asin]
		//return: arcSine(f)
		//引数1: f (radian)
		inline float Asin(float f) noexcept { return std::asinf(f); }
		//----------------------------------------------------------------------------------
		//[Atan]
		//return: arcAangent(f)
		//引数1: f (radian)
		inline float Atan(float f) noexcept { return std::atanf(f); }
		//----------------------------------------------------------------------------------
		//[Atan2]
		//return: arcAangent2(f)
		//引数1: f (radian)
		inline float Atan2(float y, float x) noexcept { return std::atan2f(y, x); }
		//----------------------------------------------------------------------------------
		//[Sqrt]
		//return: fの平方根, Fast inverse square rootを用いるので低精度 & 高速
		//引数1: f
		inline constexpr float Sqrt(float f) noexcept;
		//----------------------------------------------------------------------------------
		//[SqrtLowAccuracy]
		//return: fの平方根, Sqrtよりも更に低精度だが最高速
		//引数1: f
		inline constexpr float SqrtLowAccuracy(float f) noexcept;
		//----------------------------------------------------------------------------------
		//[SqrtHighAccuracy]
		//return: fの平方根, Sqrtより高精度 & 低速
		//引数1: f
		inline float SqrtHighAccuracy(float f) noexcept { return std::sqrtf(f); }
		//----------------------------------------------------------------------------------
		//[Pow]
		//return: fのp乗, p = intの方が高速
		//引数1: f
		//引数2: p
		inline float Pow(float f, float p) noexcept {return std::powf(f, p); }
		//----------------------------------------------------------------------------------
		//[Pow]
		//return: fのp乗
		//引数1: f
		//引数2: p
		inline constexpr float Pow(float f, int p) noexcept;
		//----------------------------------------------------------------------------------
		//[Pow2]
		//return: fの2乗
		//引数1: f
		inline constexpr float Pow2(float f) noexcept { return f * f; }
		//----------------------------------------------------------------------------------
		//[Pow3]
		//return: fの3乗
		//引数1: f
		inline constexpr float Pow3(float f) noexcept { return f * f * f; }
		//----------------------------------------------------------------------------------
		//[Pow4]
		//return: fの4乗
		//引数1: f
		inline constexpr float Pow4(float f) noexcept { return f * f * f * f; }
		//----------------------------------------------------------------------------------
		//[Pow5]
		//return: fの5乗
		//引数1: f
		inline constexpr float Pow5(float f) noexcept { return f * f * f * f * f; }
		//----------------------------------------------------------------------------------
		//[Log]
		//return: fの対数
		//引数1: f
		inline float Log(float f) noexcept { return std::logf(f); }
		//----------------------------------------------------------------------------------
		//[Log10]
		//return: fの底が10な対数
		//引数1: f
		inline float Log10(float f) noexcept { return std::log10f(f); }

		//----------------------------------------------------------------------------------
		//[Lerp3D]
		//fromとtoの間をtで線形補完したものを返却する, 補完には3次関数補完を使う
		//return: fromからtoへの線形(3次関数)補間
		//引数1: from
		//引数2: to
		//引数3: 補完の度合い(0 ~ 1)s
		inline constexpr float Lerp3D(float from, float to, float t) noexcept;
		//----------------------------------------------------------------------------------
		//[LerpDirect]
		//fromとtoの間をtで線形補完したものを返却する, 補完にはただの線形補間を使う
		//return: fromからtoへの線形補間
		//引数1: from
		//引数2: to
		//引数3: 補完の度合い(0 ~ 1)
		inline constexpr float LerpDirect(float from, float to, float t) noexcept;

		//----------------------------------------------------------------------------------
		//[GreatestCommonDenominator]
		//return: AとBの最大公約数
		//引数1: 数字A
		//引数1: 数字B
		inline constexpr int GreatestCommonDenominator(int a, int b) noexcept;
		//----------------------------------------------------------------------------------
		//[AspectRatio]
		//return: AとBのアスペクト比
		//引数1: アスペクト比を算出するVector
		inline constexpr Vector2Int AspectRatio(Vector2Int vec) noexcept;


		//----------------------------------------------------------------------------------
		//[NormalDirection]
		//return: 三点が作る面の法線ベクトル
		//引数1: 頂点1
		//引数2: 頂点2
		//引数3: 頂点3
		inline SGFramework::Vector3 NormalDirection(const SGFramework::Vector3& vertex1, 
			const SGFramework::Vector3& vertex2, const SGFramework::Vector3& vertex3) noexcept { return Vector3::Cross(vertex2 - vertex1, vertex3 - vertex2).Normalized(); }
		//----------------------------------------------------------------------------------
		//[DistanceWithPointToPlane]
		//return: 座標と平面の最短距離
		//引数1: 座標
		//引数2: 平面の座標
		//引数3: 平面の法線
		inline float DistanceWithPointToPlane(const SGFramework::Vector3& point, 
			const SGFramework::Vector3& planePos, const SGFramework::Vector3& planeNormal) noexcept { return Absolute(Vector3::Dot(planeNormal, point - planePos)); }

		//----------------------------------------------------------------------------------
		//[AspectRatioSettingX]
		//xを基に指定したアスペクト比のVectorを算出する
		//return: xを基に計算したアスペクト比
		//引数1: 基にするx
		//引数2: アスペクト比
		inline constexpr SGFramework::Vector2 AspectRatioSettingX(float x, const SGFramework::Vector2& aspectRadio) noexcept;
		//----------------------------------------------------------------------------------
		//[AspectRatioSettingX]
		//xを基に指定したアスペクト比のVectorを算出する
		//return: xを基に計算したアスペクト比
		//引数1: 基にするx
		//引数2: アスペクト比
		inline constexpr Vector2Int AspectRatioSettingX(int x, const Vector2Int& aspectRadio) noexcept;
		//----------------------------------------------------------------------------------
		//[AspectRatioSettingY]
		//yを基に指定したアスペクト比のVectorを算出する
		//return: yを基に計算したアスペクト比
		//引数1: 基にするy
		//引数2: アスペクト比
		inline constexpr SGFramework::Vector2 AspectRatioSettingY(float y, const SGFramework::Vector2& aspectRadio) noexcept;
		//----------------------------------------------------------------------------------
		//[AspectRatioSettingY]
		//yを基に指定したアスペクト比のVectorを算出する
		//return: yを基に計算したアスペクト比
		//引数1: 基にするy
		//引数2: アスペクト比
		inline constexpr Vector2Int AspectRatioSettingY(int y, const Vector2Int& aspectRadio) noexcept;







		//----------------------------------------------------------------------------------
		//[Sqrt]
		//return: fの平方根, Fast inverse square rootを用いるので精度は低め
		//引数1: f (radian)
		inline constexpr float Sqrt(float f) noexcept
		{
			int i = 0x5f3759df - (*reinterpret_cast<int*>(&f) >> 1);
			float bitF = *reinterpret_cast<float*>(&i);

			bitF *= (1.5f - 0.5f * f * bitF * bitF);
			return f * bitF * (1.5f - 0.5f * f * bitF * bitF);
		}
		//----------------------------------------------------------------------------------
		//[SqrtLowAccuracy]
		//return: fの平方根, Sqrtよりも更に低精度だが最高速
		//引数1: f
		constexpr float SqrtLowAccuracy(float f) noexcept
		{
			int i = 0x5f3759df - (*reinterpret_cast<int*>(&f) >> 1);
			float bitF = *reinterpret_cast<float*>(&i);

			return f * bitF * (1.5f - 0.5f * f * bitF * bitF);
		}

		//----------------------------------------------------------------------------------
		//[Pow]
		//return: fのp乗
		//引数1: f
		//引数2: p
		inline constexpr float Pow(float f, int p) noexcept
		{
			if (IsZero(f) || p == 0) return 1.0f;

			float result = 1.0f;
			float mult = (p < 0 ? 1.0f / f : f);

			for (int i = 0; i < (p < 0 ? -p : p); i++)
				result *= mult;

			return result;
		}

		//----------------------------------------------------------------------------------
		//[Lerp3D]
		//fromとtoの間をtで線形補完したものを返却する, 補完には3次関数補完を使う
		//return: fromからtoへの線形(3次関数)補間
		//引数1: from
		//引数2: to
		//引数3: 補完の度合い(0 ~ 1)s
		inline constexpr float Lerp3D(float from, float to, float t) noexcept
		{
			if (t > 1.0f) t = 1.0f;	else if (t < 0.0f) t = 0.0f;
			//3次関数補間
			float rate = t * t * (3.0f - 2.0f * t);

			//return
			return from * (1.0f - rate) + to * rate;
		}
		//----------------------------------------------------------------------------------
		//[LerpDirect]
		//fromとtoの間をtで線形補完したものを返却する, 補完にはただの線形補間を使う
		//return: fromからtoへの線形補間
		//引数1: from
		//引数2: to
		//引数3: 補完の度合い(0 ~ 1)
		inline constexpr float LerpDirect(float from, float to, float t) noexcept
		{
			if (t > 1.0f) t = 1.0f;	else if (t < 0.0f) t = 0.0f;
			//return
			return from * (1.0f - t) + to * t;
		}

		//----------------------------------------------------------------------------------
		//[GreatestCommonDenominator]
		//return: AとBの最大公約数
		//引数1: 数字A
		//引数1: 数字B
		inline constexpr int GreatestCommonDenominator(int a, int b) noexcept
		{
			// - の値の場合挙動が怪しいので終了
			if (a < 0 || b < 0)	return 0;
			//bがaより大きい場合逆を実行
			else if (a < b)		return GreatestCommonDenominator(b, a);
			else
			{
				//ユークリッドの互除法
				while (b != 0)
				{
					int temp = a % b;
					a = b;
					b = temp;
				}
				return a;
			}
		}
		//----------------------------------------------------------------------------------
		//[AspectRatio]
		//return: AとBのアスペクト比
		//引数1: アスペクト比を算出するVector
		inline constexpr SGFramework::Vector2Int AspectRatio(SGFramework::Vector2Int vec) noexcept
		{
			int gcd = GreatestCommonDenominator(vec.x, vec.y);
			vec.x /= gcd;
			vec.y /= gcd;
			return vec;
		}

		//----------------------------------------------------------------------------------
		//[AspectRatioSettingX]
		//xを基に指定したアスペクト比のVectorを算出する
		//return: xを基に計算したアスペクト比
		//引数1: 基にするx
		//引数2: アスペクト比
		inline constexpr SGFramework::Vector2 AspectRatioSettingX(float x, const SGFramework::Vector2& aspectRadio) noexcept
		{
			return SGFramework::Vector2(x, x * (aspectRadio.y / aspectRadio.x));
		}
		//----------------------------------------------------------------------------------
		//[AspectRatioSettingX]
		//xを基に指定したアスペクト比のVectorを算出する
		//return: xを基に計算したアスペクト比
		//引数1: 基にするx
		//引数2: アスペクト比
		inline constexpr SGFramework::Vector2Int AspectRatioSettingX(int x, const SGFramework::Vector2Int& aspectRadio) noexcept
		{
			return Vector2Int(x, static_cast<int>(static_cast<float>(x) * (static_cast<float>(aspectRadio.y) / static_cast<float>(aspectRadio.x))));
		}
		//----------------------------------------------------------------------------------
		//[AspectRatioSettingY]
		//yを基に指定したアスペクト比のVectorを算出する
		//return: yを基に計算したアスペクト比
		//引数1: 基にするy
		//引数2: アスペクト比
		inline constexpr SGFramework::Vector2 AspectRatioSettingY(float y, const SGFramework::Vector2& aspectRadio) noexcept
		{
			return SGFramework::Vector2(y, y * (aspectRadio.x / aspectRadio.y));
		}
		//----------------------------------------------------------------------------------
		//[AspectRatioSettingY]
		//yを基に指定したアスペクト比のVectorを算出する
		//return: yを基に計算したアスペクト比
		//引数1: 基にするy
		//引数2: アスペクト比
		inline constexpr SGFramework::Vector2Int AspectRatioSettingY(int y, const SGFramework::Vector2Int& aspectRadio) noexcept
		{
			return SGFramework::Vector2Int(y, static_cast<int>(static_cast<float>(y) *
				(static_cast<float>(aspectRadio.x) / static_cast<float>(aspectRadio.y))));
		}

	}
}
#endif // !SGFRAMEWORK_HEADER_CMATHF_DECLARATION_HPP_
