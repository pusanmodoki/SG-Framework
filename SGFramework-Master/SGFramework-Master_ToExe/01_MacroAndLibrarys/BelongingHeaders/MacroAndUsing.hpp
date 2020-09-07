/*----------------------------------------------------------------------------------
S.Game-Framework  (SG-Framework)
The MIT License (MIT)

Copyright (c) 2020 Shota Uemura

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
------------------------------------------------------------------------------------*/
/**
* @file MacroAndUsing.hpp
*  @brief using, 定数などを定義
*/
#ifndef SGFRAMEWORK_HEADER_MACRO_AND_USING_HPP_
#define SGFRAMEWORK_HEADER_MACRO_AND_USING_HPP_
#include <cmath>
#include <limits>
#include <mutex>
#include <array>

//Windows Version
#if defined(SGF_PLATFORM_WINDOWS)
#include <DirectXMath.h>
#include <Windows.h>
//else
#else
#error "Unsupported platform."
#endif//endif

//Windows Version
#if defined(SGF_PLATFORM_WINDOWS)
#undef min
#undef max
//else
#else
#error "Unsupported platform."
#endif//endif


//! @brief SG-Framework inline property macro
#define SGF_INLINE_PROPERTY inline
//! @brief SG-Framework property macro
#define SGF_PROPERTY

//----------------------------------------------------------------------------------
//汎用マクロ

//! @brief true判定高速化, 見やすさ向上マクロ(高速化は環境依存？)
#define IS_TRUE(val) ((val) ^ false)
//! @brief false判定高速化, 見やすさ向上マクロ(高速化は環境依存？)
#define IS_FALSE(val) ((val) ^ true)

//! @brief ポインタをnullptrチェック→delete & nullptr代入するマクロ
#define DELETE_POINTER(pointer) if (pointer != nullptr) { delete pointer; pointer = nullptr; }
//! @brief ポインタをnullptrチェック→delete[] & nullptr代入するマクロ
#define DELETE_ARRAY_POINTER(pointer) if (pointer != nullptr) { delete[] pointer; pointer = nullptr; }

//! @brief リスト内の全ポインタをnullptrチェック→delete & nullptr代入するマクロ
#define DELETE_POINTER_LIST(pointerList) { for (auto& e : pointerList) if (e != nullptr) delete e; pointerList.clear(); }
//! @brief リスト内の全ポインタをnullptrチェック→delete[] & nullptr代入するマクロ
#define DELETE_ARRAY_POINTER_LIST(pointerList) { for (auto& e : pointerList) if (e != nullptr) delete[] e; pointerList.clear(); }

//! @brief COMポインタをnullptrチェック→Release & nullptr代入するマクロ
#define COM_RELEASE(pointer) if (pointer != nullptr) { pointer->Release(); pointer = nullptr; }
//! @brief physx等のポインタをnullptrチェック→release & nullptr代入するマクロ
#define PHYSX_RELEASE(pointer) if (pointer != nullptr) { pointer->release(); pointer = nullptr; }

//#define MAKE_COMMAND(function) ([this]() { function; })
//#define MAKE_FOR_COMMAND(function) ([&]() { function; })

//! @brief クラスの全デフォルト定義関数を削除するマクロ
#define DELETE_ALL_DEFAULT_CLASS_FUNCTIONS(className) \
		className() = delete; \
		className(const className&) = delete; \
		className(className&&) = delete; \
		className& operator = (const className&) = delete; \
		className& operator = (className&&) = delete;

//! @brief クラスのデフォルト宣言コピー, ムーブ関数をdefault定義するマクロ
#define DEFAULT_COPY_MOVE_FUNCTIONS(className) \
		className(const className&) = default; \
		className(className&&) = default; \
		className& operator = (const className&) = default; \
		className& operator = (className&&) = default;

#if defined(SGF_DEBUG)
//! @brief デバッグ時のみtry catch, リリース時はtryBlockのみ記述するマクロ 
#define TRY_CATCH_ON_DEBUG(tryBlock, catchBlock)  try { tryBlock; } catch(...) { catchBlock; }
#else
//! @brief デバッグ時のみtry catch, リリース時はtryBlockのみ記述するマクロ 
#define TRY_CATCH_ON_DEBUG(tryBlock, catchBlock) tryBlock;
#endif

//Windows Version
#if defined(SGF_PLATFORM_WINDOWS) 
//! @brief Windows-SGFrameworkが使用するWMメッセージの最初の番号
#define WM_SGFRAMEWORK_BEGIN (WM_APP + 0x2000)
//! @brief Windows-SGFrameworkが使用するInput用メッセージ
#define WM_SGFRAMEWORK_UPDATE_INPUT (WM_SGFRAMEWORK_BEGIN)
//! @brief Windows-SGFrameworkが使用するSetWindowNameメッセージ
#define WM_SGFRAMEWORK_SET_WINDOW_NAME (WM_SGFRAMEWORK_BEGIN + 1)
//! @brief Windows-SGFrameworkが使用するInvalidメッセージ
#define WM_SGFRAMEWORK_INVALID_MESSAGE (WM_SGFRAMEWORK_BEGIN + 2)
//! @brief Windows-SGFrameworkが使用するImguiメッセージ
#define WM_SGFRAMEWORK_UPDATE_GUI (WM_SGFRAMEWORK_BEGIN + 3)
//! @brief Windows-SGFrameworkが使用するExceptionWindow Closeメッセージ
#define WM_SGFRAMEWORK_CLOSE_EXCEPTION_WINDOW (WM_SGFRAMEWORK_BEGIN + 4)
//! @brief Windows-SGFrameworkが使用するメッセージ番号の終端
#define WM_SGFRAMEWORK_END (WM_APP + 0x20010);

//! @brief Windows-CreateWindow->hMenuに使われるException子ウィンドウ用マスク
#define CREATE_WINDOW_HMENU_CHILDLEN_MASK_EXCEPTION 0x1000000000000000
//! @brief Windows-CREATE_WINDOW_HMENU_CHILDLEN_MASK_EXCEPTIONを用いたID
#define CREATE_WINDOW_HMENU_VALUE_CHILDLEN_EXCEPTION(value) (reinterpret_cast<HMENU>(0x1000000000000000 | value))
#endif//Windows Version

//! @brief SG-Framework namespace
namespace SGFramework
{
	//Windows Version
#if defined(SGF_PLATFORM_WINDOWS) && defined (SGF_USED_GRAPHICS_DIRECTX)
	//! @brief SIMD matrix
	using SimdMatrix = DirectX::XMMATRIX;
	//! @brief SIMD vector
	using SimdVector = DirectX::XMVECTOR;
#else
	//! @brief SIMD matrix
	using SimdMatrix = std::nullptr_t;
	//! @brief SIMD vector
	using SimdVector = std::nullptr_t;
#endif
	
	//! @brief byte = unsigned char
	using byte = unsigned char;
	//! @brief uchar = unsigned char(byte)
	using uchar = unsigned char;
	//! @brief wchar = wchar_t
	using wchar = wchar_t;
	//! @brief ushort = unsigned short
	using ushort = unsigned short;
	//! @brief uint = unsigned int
	using uint = unsigned int;
	//! @brief ulong = unsigned long
	using ulong = unsigned long;
	//! @brief int64 = long long
	using int64 = long long;
	//! @brief uint64 = unsigned long long
	using uint64 = unsigned long long;
	//! @brief ulonglong = unsigned long long(uint64)
	using ulonglong = unsigned long long;

	//! @brief Fowler-Noll-Vo用定数用Hash namespace
	namespace Hash
	{
		//! @brief Fowler-Noll-Vo Prime
		constexpr size_t cFowlerNollVoPrime = 1099511628211u;
		//! @brief Fowler-Noll-Vo Offset
		constexpr size_t cFowlerNollVoOffsetBasis = 14695981039346656037u;
	}

	//! @brief 計算ライブラリを提供するMathAF namespace
	namespace MathAF
	{
		//! @brief 次元を定義するDimension enum
		struct Dimension
		{
			//! @brief enum
			enum Enum
			{
				Dimension2D = 0x1,	 //! @brief 2D
				Dimension3D = 0x2,	//! @brief 3D
			};
		};

		//! @brief 最大degree
		constexpr float cMaxDegree = 180.0f;
		//! @brief 最小degree
		constexpr float cMinDegree = -180.0f;
		//! @brief 円周率
		constexpr float cPi = 3.141592654f;
		//! @brief 円周率 x 2
		constexpr float cPix2 = 6.283185307f;
		//! @brief 円周率 / 2
		constexpr float cPiDiv2 = 1.570796327f;
		//! @brief 円周率 / 4
		constexpr float cPiDiv4 = 0.785398163f;
		//! @brief 正の無限大
		constexpr float cInfinity = std::numeric_limits<float>::infinity();
		//! @brief 負の無限大
		constexpr float cNegativeInfinity = -std::numeric_limits<float>::infinity();
		//! @brief Nan
		constexpr float cNaN = std::numeric_limits<float>::quiet_NaN();
		//! @brief degree->radian, deg * this
		constexpr float cDegToRad = (cPi * 2) / 360.0f;
		//! @brief radian->degree, rad * this
		constexpr float cRadToDeg = 360 / (cPi * 2);

		//! @brief floatの誤差
		constexpr float cEpsilon = std::numeric_limits<float>::epsilon();
		//! @brief doubleの誤差
		constexpr double cDoubleEpsilon = std::numeric_limits<double>::epsilon();

		//----------------------------------------------------------------------------------
		//! @brief 誤差ありの==演算を行う
		//! @param[in] left left value
		//! @param[in] right right value
		//! @return bool left - rightが誤差以下ならtrueを返却
		inline constexpr bool IsEqual(float left, float right) { return (left - right) < 0.0f ? (-(left - right) <= cEpsilon) : ((left - right) <= cEpsilon); }

		//----------------------------------------------------------------------------------
		//! @brief 誤差ありの==演算を行う
		//! @param[in] left left value
		//! @param[in] right right value
		//! @return bool left - rightが誤差以下ならtrueを返却
		inline constexpr bool IsEqual(double left, double right) { return left - right < 0 ? (-(left - right) <= cDoubleEpsilon) : (left - right <= cDoubleEpsilon); }
		//----------------------------------------------------------------------------------
		//! @brief 誤差ありの==演算を行う
		//! @param[in] left left value
		//! @param[in] right right value
		//! @return bool left - rightが誤差超過ならtrueを返却
		inline constexpr bool IsNotEqual(float left, float right) { return left - right < 0 ? (-(left - right) > cEpsilon) : (left - right > cEpsilon); }
		//----------------------------------------------------------------------------------
		//! @brief 誤差ありの==演算を行う
		//! @param[in] left left value
		//! @param[in] right right value
		//! @return bool left - rightが誤差超過ならtrueを返却
		inline constexpr bool IsNotEqual(double left, double right) { return left - right < 0 ? (-(left - right) > cDoubleEpsilon) : (left - right > cDoubleEpsilon); }
	}

	//! @brief ファイルサイズ変換をサポートするDataVolumeConversion namespace
	namespace DataVolumeConversion
	{
		//! @brief byte->kilobyte, byte / this
		constexpr float cByteToKiloByteFloat = 1024.0f;
		//! @brief byte->megabyte, byte / this
		constexpr float cByteToMegaByteFloat = 1024.0f * 1024.0f;
		//! @brief byte->gigabyte, byte / this
		constexpr float cByteToGigaByteFloat = 1024.0f * 1024.0f * 1024.0f;

		//! @brief kilobyte->byte, byte * this
		constexpr float cKiloByteToByteFloat = 1024.0f;
		//! @brief kilobyte->megabyte, byte / this
		constexpr float cKiloByteToMegaByteFloat = 1024.0f;
		//! @brief kilobyte->gigabyte, byte / this
		constexpr float cKiloByteToGigaByteFloat = 1024.0f * 1024.0f;

		//! @brief megabyte->byte, byte * this
		constexpr float cMegaByteToByteFloat = 1024.0f * 1024.0f;
		//! @brief megabyte->kilobyte, byte * this
		constexpr float cMegaByteToKiloByteFloat = 1024.0f;
		//! @brief megabyte->gigabyte, byte / this
		constexpr float cMegaByteToGigaByteFloat = 1024.0f;

		//! @brief gigabyte->byte, byte * this
		constexpr float cGigaByteToByteFloat = 1024.0f * 1024.0f * 1024.0f;
		//! @brief gigabyte->kilobyte, byte * this
		constexpr float cGigaByteToKiloByteFloat = 1024.0f * 1024.0f;
		//! @brief gigabyte->megabyte, byte * this
		constexpr float cGigaByteToMegaByteFloat = 1024.0f;

		//! @brief byte->kilobyte, byte / this
		constexpr uint64 cByteToKiloByte = 1024;
		//! @brief byte->megabyte, byte / this
		constexpr uint64 cByteToMegaByte = 1024 * 1024;
		//! @brief byte->gigabyte, byte / this
		constexpr uint64 cByteToGigaByte = 1024 * 1024 * 1024;

		//! @brief kilobyte->byte, byte * this
		constexpr uint64 cKiloByteToByte = 1024;
		//! @brief kilobyte->megabyte, byte / this
		constexpr uint64 cKiloByteToMegaByte = 1024;
		//! @brief kilobyte->gigabyte, byte / this
		constexpr uint64 cKiloByteToGigaByte = 1024 * 1024;

		//! @brief megabyte->byte, byte * this営業す
		constexpr uint64 cMegaByteToByte = 1024 * 1024;
		//! @brief megabyte->kilobyte, byte * this
		constexpr uint64 cMegaByteToKiloByte = 1024;
		//! @brief megabyte->gigabyte, byte / this
		constexpr uint64 cMegaByteToGigaByte = 1024;

		//! @brief gigabyte->byte, byte * this
		constexpr uint64 cGigaByteToByte = 1024 * 1024 * 1024;
		//! @brief gigabyte->kilobyte, byte * this
		constexpr uint64 cGigaByteToKiloByte = 1024 * 1024;
		//! @brief gigabyte->megabyte, byte * this
		constexpr uint64 cGigaByteToMegaByte = 1024;
	}


	//! @brief フレームワークの管理をするFrameworkFoundation namespace
	namespace FrameworkFoundation
	{
		//Windows Version
#if defined(SGF_PLATFORM_WINDOWS)
		//----------------------------------------------------------------------------------
		//! @brief フレッシュレートを取得する
		//! @param[in] GetDeviceCapsで使用するHWND
		//! @return float 30, 60, 120, 144, 244の中からデバイス取得関数で取得したリフレッシュレートに最も近い値
		inline float GetRefreshRate(HWND hWnd)
		{
			//設定リフレッシュレート
			constexpr std::array<int, 5> cRefreshs = { 30, 60, 120, 144, 244 };

			//リフレッシュレートを取得
			int result;
			{
				HDC hdc = GetDC(hWnd);
				result = GetDeviceCaps(hdc, VREFRESH);
				ReleaseDC(hWnd, hdc);
			}

			//設定リフレッシュレートと合致するか確認
			bool isEqual = false;
			for (const auto& e : cRefreshs)
				if (result == e) isEqual = true;
			//一緒ならそのまま返す
			if (IS_TRUE(isEqual)) return static_cast<float>(result);

			//一番近い値を探す用
			int fasts[2] = { 100, -1 };
			//探索ループ
			for (int i = 0; i < 5; i++)
			{
				int distance = cRefreshs[i] - result;
				if (distance < 0) distance *= - 1;
				if (fasts[0] > distance)
				{
					fasts[0] = distance;
					fasts[1] = i;
				}
			}

			//一番近い値を返却
			return static_cast<float>(cRefreshs[fasts[1]]);
		}
#else
#error "GetRefreshRate->Unimplemented"
			throw std::exception("GetRefreshRate->Unimplemented");
#endif
	}
}
#endif // !SGFRAMEWORK_HEADER_MACRO_AND_USING_HPP_