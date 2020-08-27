/*----------------------------------------------------------------------------------
std::atomicを操作するAtomic namespaceを記述したAtomic.hpp
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_ATOMIC_HPP_
#define SGFRAMEWORK_HEADER_ATOMIC_HPP_
#include <atomic>
#include <type_traits>
#ifdef SGF_DEBUG
#include "../Exception/Exception.hpp"
#endif // SGF_DEBUG


//Windows version
#if defined(SGF_PLATFORM_WINDOWS)
#undef max
#endif

//Framework namespace
namespace SGFramework
{
	//std::atomicを操作するAtomic namespace
	namespace Atomic
	{
		//----------------------------------------------------------------------------------
		//All
		//----------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------
		//[Init]
		//atomic<T>をロックフリーアルゴリズムを用いて初期化する
		//template: atomic type
		//return: 代入結果 (atomicに代入済み)
		//argument 1: atomic (atomic type)
		//argument 2: 初期化する値
		template <class T>
		inline T Init(std::atomic<T>& atomic, T init) noexcept
		{
			T expected = atomic.load();

			while (1 ^ atomic.compare_exchange_weak(expected, init)) {}

			return init;
		}
		//----------------------------------------------------------------------------------
		//[Init]
		//atomic<T>をロックフリーアルゴリズムを用いて初期化する
		//template: atomic type
		//return: 代入結果 (atomicに代入済み)
		//argument 1: atomic (atomic type)
		//argument 2: 初期化する値
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
		//[And]
		//atomic<bool>をロックフリーアルゴリズムを用いてAND演算する
		//return: 計算結果 (atomicに代入済み)
		//argument 1: atomic (bool)
		inline bool And(std::atomic<bool>& atomicBoolean, bool andFlag) noexcept
		{
			bool expected = atomicBoolean.load();

			while (1 ^ atomicBoolean.compare_exchange_weak(expected, expected & andFlag)) {}

			return atomicBoolean.load();
		}
		//----------------------------------------------------------------------------------
		//[Or]
		//atomic<bool>をロックフリーアルゴリズムを用いてOR演算する
		//return: 計算結果 (atomicに代入済み)
		//argument 1: atomic (bool)
		inline bool Or(std::atomic<bool>& atomicBoolean, bool orFlag) noexcept
		{
			bool expected = atomicBoolean.load();

			while (1 ^ atomicBoolean.compare_exchange_weak(expected, expected | orFlag)) {}

			return atomicBoolean.load();
		}
		//----------------------------------------------------------------------------------
		//[Xor]
		//atomic<bool>をロックフリーアルゴリズムを用いてOR演算する
		//return: 計算結果 (atomicに代入済み)
		//argument 1: atomic (bool)
		inline bool Xor(std::atomic<bool>& atomicBoolean, bool xorFlag) noexcept
		{
			bool expected = atomicBoolean.load();

			while (1 ^ atomicBoolean.compare_exchange_weak(expected, expected ^ xorFlag)) {}

			return atomicBoolean.load();
		}
		//----------------------------------------------------------------------------------
		//[BitInversion]
		//atomic<bool>をロックフリーアルゴリズムを用いてビット反転させる
		//return: 計算結果 (atomicに代入済み)
		//argument 1: atomic (bool)
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
		//[LockAtomic]
		//atomic<bool>を用いてLockする
		//argument 1: atomic (bool)
		inline void LockAtomic(std::atomic<bool>& atomicBoolean) noexcept
		{
			bool expected = false;

			while (1 ^ atomicBoolean.compare_exchange_weak(expected, true)) { expected = false; }
		}
		//----------------------------------------------------------------------------------
		//[TryLockAtomic]
		//atomic<bool>を用いてLockを試す
		//return: Lockに成功した場合true
		//argument 1: atomic (bool)
		inline bool TryLockAtomic(std::atomic<bool>& atomicBoolean) noexcept
		{
			bool expected = false;

			return (1 ^ atomicBoolean.compare_exchange_weak(expected, true));
		}
		//----------------------------------------------------------------------------------
		//[TryCycleLockAtomic]
		//atomic<bool>を用いてLockをcycle回数試す
		//return: Lockに成功した場合true
		//argument 1: atomic (bool)
		//argument 2: cycle, forループ回数
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
		//[UnlockAtomic]
		//atomic<bool>を用いてUnlockする
		//argument 1: atomic (bool)
		inline void UnlockAtomic(std::atomic<bool>& atomicBoolean) noexcept
		{
			bool expected = true;

			while (1 ^ atomicBoolean.compare_exchange_weak(expected, false)) { expected = true; }
		}

		//Atomic版LockGuard
		class LockGuard
		{
		public:
			LockGuard(std::atomic<bool>& atomic) : m_atomic(atomic)
			{
				LockAtomic(m_atomic);
			}
			~LockGuard() { UnlockAtomic(m_atomic); }
		private:
			std::atomic<bool>& m_atomic;
		};
		//Atomic版TryLockGuard
		class TryLockGuard
		{
		public:
			TryLockGuard(std::atomic<bool>& atomic) : m_atomic(atomic)
			{
				TryLockAtomic(m_atomic);
			}
			~TryLockGuard() { UnlockAtomic(m_atomic); }
		private:
			std::atomic<bool>& m_atomic;
		};



		//----------------------------------------------------------------------------------
		//Integral
		//----------------------------------------------------------------------------------


		//----------------------------------------------------------------------------------
		//[Add]
		//atomic<T>をロックフリーアルゴリズムを用いて加算する
		//template: integral type
		//return: 計算結果 (atomicに代入済み)
		//argument 1: atomic (integral type)
		//argument 2: 加算する値
		template <class T>
		inline T Add(std::atomic<T>& atomicIntegral, T add) noexcept
		{
			static_assert(std::is_integral_v<T>, "Only integral type is supported");

			T expected = atomicIntegral.load();

			while (1 ^ atomicIntegral.compare_exchange_weak(expected, expected + add)) {}

			return atomicIntegral.load();
		}
		//----------------------------------------------------------------------------------
		//[Subtract]
		//atomic<T>をロックフリーアルゴリズムを用いて減算する
		//template: integral type
		//return: 計算結果 (atomicに代入済み)
		//argument 1: atomic (integral type)
		//argument 2: 減算する値
		template <class T>
		inline T Subtract(std::atomic<T>& atomicIntegral, T sub) noexcept
		{
			static_assert(std::is_integral_v<T>, "Only integral type is supported");

			T expected = atomicIntegral.load();

			while (1 ^ atomicIntegral.compare_exchange_weak(expected, expected - sub)) {}

			return atomicIntegral.load();
		}
		//----------------------------------------------------------------------------------
		//[Subtract]
		//atomic<T>をロックフリーアルゴリズムを用いて乗算する
		//template: integral type
		//return: 計算結果 (atomicに代入済み)
		//argument 1: atomic (integral type)
		//argument 2: 乗算する値
		template <class T>
		inline T Multiply(std::atomic<T>& atomicIntegral, T multi) noexcept
		{
			static_assert(std::is_integral_v<T>, "Only integral type is supported");

			T expected = atomicIntegral.load();

			while (1 ^ atomicIntegral.compare_exchange_weak(expected, expected * multi)) {}

			return atomicIntegral.load();
		}
		//----------------------------------------------------------------------------------
		//[Divide]<possibly throw>
		//atomic<T>をロックフリーアルゴリズムを用いて除算する
		//template: integral type
		//return: 計算結果 (atomicに代入済み)
		//argument 1: atomic (integral type)
		//argument 2: 除算する値
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
		//[And]
		//atomic<integral type>をロックフリーアルゴリズムを用いてAND演算する
		//template: integral type
		//return: 計算結果 (atomicに代入済み)
		//argument 1: atomic (integral type)
		template <class T>
		inline T And(std::atomic<T>& atomicIntegral, T andFlag) noexcept
		{
			static_assert(std::is_integral_v<T>, "Only integral type is supported");

			T expected = atomicIntegral.load();

			while (1 ^ atomicIntegral.compare_exchange_weak(expected, expected & andFlag)) {}

			return atomicIntegral.load();
		}
		//----------------------------------------------------------------------------------
		//[Or]
		//atomic<integral type>をロックフリーアルゴリズムを用いてOR演算する
		//template: integral type
		//return: 計算結果 (atomicに代入済み)
		//argument 1: atomic (integral type)
		template <class T>
		inline T Or(std::atomic<T>& atomicIntegral, T orFlag) noexcept
		{
			static_assert(std::is_integral_v<T>, "Only integral type is supported");

			T expected = atomicIntegral.load();

			while (1 ^ atomicIntegral.compare_exchange_weak(expected, expected | orFlag)) {}

			return atomicIntegral.load();
		}
		//----------------------------------------------------------------------------------
		//[Xor]
		//atomic<integral type>をロックフリーアルゴリズムを用いてOR演算する
		//template: integral type
		//return: 計算結果 (atomicに代入済み)
		//argument 1: atomic (integral type)
		template <class T>
		inline T Xor(std::atomic<T>& atomicIntegral, T xorFlag) noexcept
		{
			static_assert(std::is_integral_v<T>, "Only integral type is supported");

			T expected = atomicIntegral.load();

			while (1 ^ atomicIntegral.compare_exchange_weak(expected, expected ^ xorFlag)) {}

			return atomicIntegral.load();
		}
		//----------------------------------------------------------------------------------
		//[BitInversion]
		//atomic<integral type>をロックフリーアルゴリズムを用いてビット反転させる
		//template: integral type
		//return: 計算結果 (atomicに代入済み)
		//argument 1: atomic (integral type)
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
		//[Add]
		//atomic<T>をロックフリーアルゴリズムを用いて加算する
		//template: integral type
		//return: 計算結果 (atomicに代入済み)
		//argument 1: atomic (integral type)
		//argument 2: 加算する値
		template <class T, class CastType>
		inline T Add(std::atomic<T>& atomicIntegral, CastType add) noexcept
		{
			static_assert(std::is_convertible_v<CastType, T>, "Only castable type is supported");

			return Add(atomicIntegral, static_cast<T>(add));
		}
		//----------------------------------------------------------------------------------
		//[Subtract]
		//atomic<T>をロックフリーアルゴリズムを用いて減算する
		//template: integral type
		//return: 計算結果 (atomicに代入済み)
		//argument 1: atomic (integral type)
		//argument 2: 減算する値
		template <class T, class CastType>
		inline T Subtract(std::atomic<T>& atomicIntegral, CastType sub) noexcept
		{
			static_assert(std::is_convertible_v<CastType, T>, "Only castable type is supported");

			return Subtract(atomicIntegral, static_cast<T>(sub));
		}
		//----------------------------------------------------------------------------------
		//[Subtract]
		//atomic<T>をロックフリーアルゴリズムを用いて乗算する
		//template: integral type
		//return: 計算結果 (atomicに代入済み)
		//argument 1: atomic (integral type)
		//argument 2: 乗算する値
		template <class T, class CastType>
		inline T Multiply(std::atomic<T>& atomicIntegral, CastType multi) noexcept
		{
			static_assert(std::is_convertible_v<CastType, T>, "Only castable type is supported");

			return Multiply(atomicIntegral, static_cast<T>(multi));
		}
		//----------------------------------------------------------------------------------
		//[Divide]<possibly throw>
		//atomic<T>をロックフリーアルゴリズムを用いて除算する
		//template: integral type
		//return: 計算結果 (atomicに代入済み)
		//argument 1: atomic (integral type)
		//argument 2: 除算する値
		template <class T, class CastType>
		inline T Divide(std::atomic<T>& atomicIntegral, CastType div) noexcept
		{
			static_assert(std::is_convertible_v<CastType, T>, "Only castable type is supported");

			return TRY_CATCH_ON_DEBUG(Divide(atomicIntegral, static_cast<T>(div)), throw);
		}

		//----------------------------------------------------------------------------------
		//[And]
		//atomic<integral type>をロックフリーアルゴリズムを用いてAND演算する
		//template: integral type
		//return: 計算結果 (atomicに代入済み)
		//argument 1: atomic (integral type)
		template <class T, class CastType>
		inline T And(std::atomic<T>& atomicIntegral, CastType andFlag) noexcept
		{
			static_assert(std::is_convertible_v<CastType, T>, "Only castable type is supported");

			return And(atomicIntegral, static_cast<T>(andFlag));
		}
		//----------------------------------------------------------------------------------
		//[Or]
		//atomic<integral type>をロックフリーアルゴリズムを用いてOR演算する
		//template: integral type
		//return: 計算結果 (atomicに代入済み)
		//argument 1: atomic (integral type)
		template <class T, class CastType>
		inline T Or(std::atomic<T>& atomicIntegral, CastType orFlag) noexcept
		{
			static_assert(std::is_convertible_v<CastType, T>, "Only castable type is supported");

			return Or(atomicIntegral, static_cast<T>(orFlag));
		}
		//----------------------------------------------------------------------------------
		//[Xor]
		//atomic<integral type>をロックフリーアルゴリズムを用いてOR演算する
		//template: integral type
		//return: 計算結果 (atomicに代入済み)
		//argument 1: atomic (integral type)
		template <class T, class CastType>
		inline T Xor(std::atomic<T>& atomicIntegral, CastType xorFlag) noexcept
		{
			static_assert(std::is_convertible_v<CastType, T>, "Only castable type is supported");

			return Xor(atomicIntegral, static_cast<T>(xorFlag));
		}



		//----------------------------------------------------------------------------------
		//Pointer
		//----------------------------------------------------------------------------------


		//----------------------------------------------------------------------------------
		//[Add]
		//atomic<T*>をロックフリーアルゴリズムを用いて加算する
		//return: 計算結果 (atomicに代入済み)
		//template: integral type
		//argument 1: atomic (integral type)
		//argument 2: 加算する値
		template <class T>
		inline T* Add(std::atomic<T*>& atomicPointer, int add) noexcept
		{
			static_assert(std::is_pointer_v<T>, "Only pointer type is supported");

			T* expected = atomicPointer.load();

			while (1 ^ atomicPointer.compare_exchange_weak(expected, expected + add)) {}

			return atomicPointer.load();
		}
		//----------------------------------------------------------------------------------
		//[Subtract]
		//atomic<T>をロックフリーアルゴリズムを用いて減算する
		//return: 計算結果 (atomicに代入済み)
		//template: integral type
		//argument 1: atomic (integral type)
		//argument 2: 減算する値
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