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
* @file Atomic.hpp
*  @brief std::atomicを操作するAtomic namespace
*/
#ifndef SGFRAMEWORK_HEADER_ATOMIC_HPP_
#define SGFRAMEWORK_HEADER_ATOMIC_HPP_
#include <atomic>
#include <type_traits>
#include "../../01_MacroAndLibrarys/MacroAndLibrarys.hpp"
#include "../Exception/Exception.hpp"

//Windows version
#if defined(SGF_PLATFORM_WINDOWS)
#undef max
#endif

//! @brief SG-Framework namespace
namespace SGFramework
{
	//! @brief std::atomicを操作するAtomic namespace
	namespace Atomic
	{
		//----------------------------------------------------------------------------------
		//All
		//----------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------
		//! @brief std::atomicをロックフリーアルゴリズムを用いて初期化する
		//! @param[out] atomic 初期化するstd::atomic
		//! @param[in] init 初期化する値
		//! @return T 代入結果 (atomicに代入済み)
		template <class T>
		inline T Init(std::atomic<T>& atomic, T init) noexcept
		{
			T expected = atomic.load();

			while (1 ^ atomic.compare_exchange_weak(expected, init)) {}

			return init;
		}

		//----------------------------------------------------------------------------------
		//! @brief std::atomicをロックフリーアルゴリズムを用いて初期化する
		//! @param[out] atomic 初期化するstd::atomic
		//! @param[in] init 初期化する値
		//! @return T 代入結果 (atomicに代入済み)
		//! @attention CastTypeがT型にキャストできない場合static_assert
		template <class T, class CastType>
		inline T Init(std::atomic<T>& atomic, CastType init) noexcept
		{
			static_assert(std::is_convertible_v<CastType, T>, "Only castable type is supported");

			return Init(atomic, static_cast<T>(init));
		}



		//----------------------------------------------------------------------------------
		//Bool
		//----------------------------------------------------------------------------------


		//----------------------------------------------------------------------------------
		//! @brief std::atomic_boolをロックフリーアルゴリズムを用いてAND演算し代入する
		//! @param[in, out] atomicBoolean AND, 代入するstd::atomic
		//! @param[in] andFlag AND Flag
		//! @return bool 代入結果 (atomicに代入済み)
		inline bool And(std::atomic<bool>& atomicBoolean, bool andFlag) noexcept
		{
			bool expected = atomicBoolean.load();

			while (1 ^ atomicBoolean.compare_exchange_weak(expected, expected & andFlag)) {}

			return atomicBoolean.load();
		}
		//----------------------------------------------------------------------------------
		//! @brief std::atomic_boolをロックフリーアルゴリズムを用いてOR演算し代入する
		//! @param[in, out] atomicBoolean OR, 代入するstd::atomic
		//! @param[in] orFlag OR Flag
		//! @return bool 代入結果 (atomicに代入済み)
		inline bool Or(std::atomic<bool>& atomicBoolean, bool orFlag) noexcept
		{
			bool expected = atomicBoolean.load();

			while (1 ^ atomicBoolean.compare_exchange_weak(expected, expected | orFlag)) {}

			return atomicBoolean.load();
		}
		//----------------------------------------------------------------------------------
		//! @brief std::atomic_boolをロックフリーアルゴリズムを用いてXOR演算し代入する
		//! @param[in, out] atomicBoolean XOR, 代入するstd::atomic
		//! @param[in] xorFlag XOR Flag
		//! @return bool 代入結果 (atomicに代入済み)
		inline bool Xor(std::atomic<bool>& atomicBoolean, bool xorFlag) noexcept
		{
			bool expected = atomicBoolean.load();

			while (1 ^ atomicBoolean.compare_exchange_weak(expected, expected ^ xorFlag)) {}

			return atomicBoolean.load();
		}
		//----------------------------------------------------------------------------------
		//! @brief std::atomic_boolをロックフリーアルゴリズムを用いてビット反転させる
		//! @param[in, out] atomicBoolean XOR, 代入するstd::atomic
		//! @return bool 代入結果 (atomicに代入済み)
		inline bool BitInversion(std::atomic<bool>& atomicBoolean) noexcept
		{
			bool expected = atomicBoolean.load();

			while (1 ^ atomicBoolean.compare_exchange_weak(expected, expected ^ 1)) {}

			return atomicBoolean.load();
		}



		//----------------------------------------------------------------------------------
		//Bool(Lock)
		//----------------------------------------------------------------------------------


		//----------------------------------------------------------------------------------
		//! @brief std::atomic_boolをロックさせ排他制御を可能にする
		//! @param[in, out] atomicBoolean Lockするstd::atomic
		inline void LockAtomic(std::atomic<bool>& atomicBoolean) noexcept
		{
			bool expected = false;

			while (1 ^ atomicBoolean.compare_exchange_weak(expected, true)) { expected = false; }
		}
		//----------------------------------------------------------------------------------
		//! @brief std::atomic_boolをロックを試し, 成功した場合排他制御を可能にする
		//! @param[in, out] atomicBoolean Lockを試すstd::atomic
		//! @return bool Lockに成功した場合true
		inline bool TryLockAtomic(std::atomic<bool>& atomicBoolean) noexcept
		{
			bool expected = false;

			return (1 ^ atomicBoolean.compare_exchange_weak(expected, true));
		}
		//----------------------------------------------------------------------------------
		//! @brief std::atomic_boolをロックcycle回数を試し, 成功した場合排他制御を可能にする
		//! @param[in, out] atomicBoolean Lockを試すstd::atomic
		//! @param[in] cycle, forループ回数
		//! @return bool Lockに成功した場合true
		inline bool TryCycleLockAtomic(std::atomic<bool>& atomicBoolean, unsigned int cycle) noexcept
		{
			bool expected = false;

			for (unsigned int i = 0; i < cycle; expected = false)
			{
				expected = false;

				if (1 ^ atomicBoolean.compare_exchange_weak(expected, true)) ++i;
				else return true;
			}
			return false;
		}
		//----------------------------------------------------------------------------------
		//! @brief std::atomic_boolをアンロックし排他制御を終了する, 必ずLock後に同じスレッドで実行すること
		//! @param[in, out] atomicBoolean Unlockするstd::atomic
		inline void UnlockAtomic(std::atomic<bool>& atomicBoolean) noexcept
		{
			bool expected = true;

			while (1 ^ atomicBoolean.compare_exchange_weak(expected, false)) { expected = true; }
		}

		//! @brief std::atomic_bool版のロックガードを提供するAtomic版LockGuard class
		class LockGuard
		{
		public:
			//----------------------------------------------------------------------------------
			//! @brief ロックを行う
			//! @param[in, out] atomic 参照するstd::atomic
			LockGuard(std::atomic<bool>& atomic) : m_atomic(atomic) { LockAtomic(m_atomic); }
			//----------------------------------------------------------------------------------
			//! @brief アンロックを行う
			~LockGuard() { UnlockAtomic(m_atomic); }
		private:
			std::atomic<bool>& m_atomic; //! @brief atomic reference
		};
		//! @brief std::atomic_bool版のトライロックガードを提供するTryLockGuard class
		class TryLockGuard
		{
		public:
			//----------------------------------------------------------------------------------
			//! @brief ロックを行う
			//! @param[in, out] atomic 参照するstd::atomic
			TryLockGuard(std::atomic<bool>& atomic) : m_atomic(atomic) { TryLockAtomic(m_atomic); }
			//----------------------------------------------------------------------------------
			//! @brief アンロックを行う
			~TryLockGuard() { UnlockAtomic(m_atomic); }
		private:
			std::atomic<bool>& m_atomic; //! @brief atomic reference
		};



		//----------------------------------------------------------------------------------
		//Integral
		//----------------------------------------------------------------------------------


		//----------------------------------------------------------------------------------
		//! @brief std::atomicをロックフリーアルゴリズムを用いて加算する
		//! @param[in, out] atomicIntegral 計算, 代入するstd::atomic
		//! @param[in] add 加算する値
		//! @return T 計算結果 (atomicに代入済み)
		//! @attention integal typeのみ対応
		template <class T>
		inline T Add(std::atomic<T>& atomicIntegral, T add) noexcept
		{
			static_assert(std::is_integral_v<T>, "Only integral type is supported");

			T expected = atomicIntegral.load();

			while (1 ^ atomicIntegral.compare_exchange_weak(expected, expected + add)) {}

			return atomicIntegral.load();
		}
		//----------------------------------------------------------------------------------
		//! @brief std::atomicをロックフリーアルゴリズムを用いて加算する
		//! @param[in, out] atomicIntegral 計算, 代入するstd::atomic
		//! @param[in] add 加算する値
		//! @return T 計算結果 (atomicに代入済み)
		//! @attention integal typeのみ対応, CastTypeがT型にキャストできない場合static_assert
		template <class T, class CastType>
		inline T Add(std::atomic<T>& atomicIntegral, CastType add) noexcept
		{
			static_assert(std::is_convertible_v<CastType, T>, "Only castable type is supported");

			return Add(atomicIntegral, static_cast<T>(add));
		}
		//----------------------------------------------------------------------------------
		//! @brief std::atomicをロックフリーアルゴリズムを用いて減算する
		//! @param[in, out] atomicIntegral 計算, 代入するstd::atomic
		//! @param[in] sub 減算する値
		//! @return T 計算結果 (atomicに代入済み)
		//! @attention integal typeのみ対応
		template <class T>
		inline T Subtract(std::atomic<T>& atomicIntegral, T sub) noexcept
		{
			static_assert(std::is_integral_v<T>, "Only integral type is supported");

			T expected = atomicIntegral.load();

			while (1 ^ atomicIntegral.compare_exchange_weak(expected, expected - sub)) {}

			return atomicIntegral.load();
		}
		//----------------------------------------------------------------------------------
		//! @brief std::atomicをロックフリーアルゴリズムを用いて減算する
		//! @param[in, out] atomicIntegral 計算, 代入するstd::atomic
		//! @param[in] sub 減算する値
		//! @return T 計算結果 (atomicに代入済み)
		//! @attention integal typeのみ対応, CastTypeがT型にキャストできない場合static_assert
		template <class T, class CastType>
		inline T Subtract(std::atomic<T>& atomicIntegral, CastType sub) noexcept
		{
			static_assert(std::is_convertible_v<CastType, T>, "Only castable type is supported");

			return Subtract(atomicIntegral, static_cast<T>(sub));
		}
		//----------------------------------------------------------------------------------
		//! @brief std::atomicをロックフリーアルゴリズムを用いて乗算する
		//! @param[in, out] atomicIntegral 計算, 代入するstd::atomic
		//! @param[in] multi 乗算する値
		//! @return T 計算結果 (atomicに代入済み)
		//! @attention integal typeのみ対応
		template <class T>
		inline T Multiply(std::atomic<T>& atomicIntegral, T multi) noexcept
		{
			static_assert(std::is_integral_v<T>, "Only integral type is supported");

			T expected = atomicIntegral.load();

			while (1 ^ atomicIntegral.compare_exchange_weak(expected, expected * multi)) {}

			return atomicIntegral.load();
		}
		//----------------------------------------------------------------------------------
		//! @brief std::atomicをロックフリーアルゴリズムを用いて乗算する
		//! @param[in, out] atomicIntegral 計算, 代入するstd::atomic
		//! @param[in] multi 乗算する値
		//! @return T 計算結果 (atomicに代入済み)
		//! @attention integal typeのみ対応, CastTypeがT型にキャストできない場合static_assert
		template <class T, class CastType>
		inline T Multiply(std::atomic<T>& atomicIntegral, CastType multi) noexcept
		{
			static_assert(std::is_convertible_v<CastType, T>, "Only castable type is supported");

			return Multiply(atomicIntegral, static_cast<T>(multi));
		}
		//----------------------------------------------------------------------------------
		//! @brief std::atomicをロックフリーアルゴリズムを用いて除算する
		//! @param[in, out] atomicIntegral 計算, 代入するstd::atomic
		//! @param[in] div 除算する値
		//! @return T 計算結果 (atomicに代入済み)
		//! @throw div == 0な場合
		//! @attention integal typeのみ対応
		template <class T>
		inline T Divide(std::atomic<T>& atomicIntegral, T div) noexcept
		{
			static_assert(std::is_integral_v<T>, "Only integral type is supported");
#ifdef SGF_DEBUG
			if (div == 0) throw Exception::InvalidArgumentException("Atomic", "Divide", "div == 0");
#endif // SGF_DEBUG

			T expected = atomicIntegral.load();

			while (1 ^ atomicIntegral.compare_exchange_weak(expected, expected / div)) {}

			return atomicIntegral.load();
		}
		//----------------------------------------------------------------------------------
		//! @brief std::atomicをロックフリーアルゴリズムを用いて除算する
		//! @param[in, out] atomicIntegral 計算, 代入するstd::atomic
		//! @param[in] div 除算する値
		//! @return T 計算結果 (atomicに代入済み)
		//! @throw div == 0な場合
		//! @attention integal typeのみ対応, CastTypeがT型にキャストできない場合static_assert
		template <class T, class CastType>
		inline T Divide(std::atomic<T>& atomicIntegral, CastType div) noexcept
		{
			static_assert(std::is_convertible_v<CastType, T>, "Only castable type is supported");

			TRY_CATCH_ON_DEBUG(return Divide(atomicIntegral, static_cast<T>(div)), throw);
		}

		//----------------------------------------------------------------------------------
		//! @brief std::atomicをロックフリーアルゴリズムを用いてAND演算する
		//! @param[in, out] atomicIntegral 計算, 代入するstd::atomic
		//! @param[in]  andFlag AND Flag
		//! @return T 計算結果 (atomicに代入済み)
		//! @attention integal typeのみ対応
		template <class T>
		inline T And(std::atomic<T>& atomicIntegral, T andFlag) noexcept
		{
			static_assert(std::is_integral_v<T>, "Only integral type is supported");

			T expected = atomicIntegral.load();

			while (1 ^ atomicIntegral.compare_exchange_weak(expected, expected & andFlag)) {}

			return atomicIntegral.load();
		}
		//----------------------------------------------------------------------------------
		//! @brief std::atomicをロックフリーアルゴリズムを用いてAND演算する
		//! @param[in, out] atomicIntegral 計算, 代入するstd::atomic
		//! @param[in]  andFlag AND Flag
		//! @return T 計算結果 (atomicに代入済み)
		//! @attention integal typeのみ対応, CastTypeがT型にキャストできない場合static_assert
		template <class T, class CastType>
		inline T And(std::atomic<T>& atomicIntegral, CastType andFlag) noexcept
		{
			static_assert(std::is_convertible_v<CastType, T>, "Only castable type is supported");

			return And(atomicIntegral, static_cast<T>(andFlag));
		}
		//----------------------------------------------------------------------------------
		//! @brief std::atomicをロックフリーアルゴリズムを用いてOR演算する
		//! @param[in, out] atomicIntegral 計算, 代入するstd::atomic
		//! @param[in]  orFlag OR Flag
		//! @return T 計算結果 (atomicに代入済み)
		//! @attention integal typeのみ対応
		template <class T>
		inline T Or(std::atomic<T>& atomicIntegral, T orFlag) noexcept
		{
			static_assert(std::is_integral_v<T>, "Only integral type is supported");

			T expected = atomicIntegral.load();

			while (1 ^ atomicIntegral.compare_exchange_weak(expected, expected | orFlag)) {}

			return atomicIntegral.load();
		}
		//----------------------------------------------------------------------------------
		//! @brief std::atomicをロックフリーアルゴリズムを用いてOR演算する
		//! @param[in, out] atomicIntegral 計算, 代入するstd::atomic
		//! @param[in]  orFlag OR Flag
		//! @return T 計算結果 (atomicに代入済み)
		//! @attention integal typeのみ対応, CastTypeがT型にキャストできない場合static_assert
		template <class T, class CastType>
		inline T Or(std::atomic<T>& atomicIntegral, CastType orFlag) noexcept
		{
			static_assert(std::is_convertible_v<CastType, T>, "Only castable type is supported");

			return Or(atomicIntegral, static_cast<T>(orFlag));
		}
		//----------------------------------------------------------------------------------
		//! @brief std::atomicをロックフリーアルゴリズムを用いてXOR演算する
		//! @param[in, out] atomicIntegral 計算, 代入するstd::atomic
		//! @param[in]  xorFlag XOR Flag
		//! @return T 計算結果 (atomicに代入済み)
		//! @attention integal typeのみ対応
		template <class T>
		inline T Xor(std::atomic<T>& atomicIntegral, T xorFlag) noexcept
		{
			static_assert(std::is_integral_v<T>, "Only integral type is supported");

			T expected = atomicIntegral.load();

			while (1 ^ atomicIntegral.compare_exchange_weak(expected, expected ^ xorFlag)) {}

			return atomicIntegral.load();
		}
		//----------------------------------------------------------------------------------
		//! @brief std::atomicをロックフリーアルゴリズムを用いてXOR演算する
		//! @param[in, out] atomicIntegral 計算, 代入するstd::atomic
		//! @param[in]  xorFlag XOR Flag
		//! @return T 計算結果 (atomicに代入済み)
		//! @attention integal typeのみ対応, CastTypeがT型にキャストできない場合static_assert
		template <class T, class CastType>
		inline T Xor(std::atomic<T>& atomicIntegral, CastType xorFlag) noexcept
		{
			static_assert(std::is_convertible_v<CastType, T>, "Only castable type is supported");

			return Xor(atomicIntegral, static_cast<T>(xorFlag));
		}
		//----------------------------------------------------------------------------------
		//! @brief std::atomic_boolをロックフリーアルゴリズムを用いてビット反転させる
		//! @param[in, out] atomicIntegral 計算, 代入するstd::atomic
		//! @return T 計算結果 (atomicに代入済み)
		//! @attention integal typeのみ対応
		template <class T>
		inline T BitInversion(std::atomic<T>& atomicIntegral) noexcept
		{
			static_assert(std::is_integral_v<T>, "Only integral type is supported");
			static constexpr unsigned long long cMaxBit = std::numeric_limits<unsigned long long>::max();

			T expected = atomicIntegral.load();

			while (1 ^ atomicIntegral.compare_exchange_weak(expected, expected ^ static_cast<T>(cMaxBit))) {}

			return atomicIntegral.load();
		}


		//----------------------------------------------------------------------------------
		//Pointer
		//----------------------------------------------------------------------------------


		//----------------------------------------------------------------------------------
		//! @brief std::atomic_pointerをロックフリーアルゴリズムを用いて加算する
		//! @param[in, out] atomicPointer 計算, 代入するstd::atomic
		//! @param[in] add 加算する値
		//! @return T 計算結果 (atomicに代入済み)
		//! @attention pointer typeのみ対応
		template <class T>
		inline T* Add(std::atomic<T*>& atomicPointer, int add) noexcept
		{
			static_assert(std::is_pointer_v<T>, "Only pointer type is supported");

			T* expected = atomicPointer.load();

			while (1 ^ atomicPointer.compare_exchange_weak(expected, expected + add)) {}

			return atomicPointer.load();
		}
		//----------------------------------------------------------------------------------
		//! @brief std::atomic_pointerをロックフリーアルゴリズムを用いて減算する
		//! @param[in, out] atomicPointer 計算, 代入するstd::atomic
		//! @param[in] sub 減算する値
		//! @return T 計算結果 (atomicに代入済み)
		//! @attention pointer typeのみ対応
		template <class T>
		inline T* Subtract(std::atomic<T*>& atomicPointer, int sub) noexcept
		{
			static_assert(std::is_pointer_v<T>, "Only pointer type is supported");

			T* expected = atomicPointer.load();

			while (1 ^ atomicPointer.compare_exchange_weak(expected, expected - sub)) {}

			return atomicPointer.load();
		}
	}
}
#endif //SGFRAMEWORK_HEADER_ATOMIC_HPP_