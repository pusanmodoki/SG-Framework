#ifndef SGFRAMEWORK_HEADER_ATOMIC_HPP_
#define SGFRAMEWORK_HEADER_ATOMIC_HPP_
#include <atomic>
#include <type_traits>

#undef max

//Framework namespace
namespace SGFramework
{
	//std::atomicを操作するAtomicOperation
	namespace AtomicOperation
	{
		//----------------------------------------------------------------------------------
		//All
		//----------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------
		//[Init]
		//atomic<T>をロックフリーアルゴリズムを用いて初期化する
		//template: atomic type
		//return: 代入結果 (atomicに代入済み)
		//引数1: atomic (atomic type)
		//引数2: 初期化する値
		template <class T>
		inline T Init(std::atomic<T>& atomic, T init)
		{
			T oldAtomic, newAtomic;

			do
			{
				oldAtomic = atomic.load();
				newAtomic = init;
			} while (1 ^ atomic.compare_exchange_weak(oldAtomic, newAtomic));

			return newAtomic;
		}
		//----------------------------------------------------------------------------------
		//[Init]
		//atomic<T>をロックフリーアルゴリズムを用いて初期化する
		//template: atomic type
		//return: 代入結果 (atomicに代入済み)
		//引数1: atomic (atomic type)
		//引数2: 初期化する値
		template <class T, class CastType>
		inline T Init(std::atomic<T>& atomic, CastType init)
		{
			static_assert(std::is_convertible_v<CastType, T>, "Only castable type is supported");

			return Init(atomic, static_cast<T>(init));
		}

		//----------------------------------------------------------------------------------
		//Bool
		//----------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------
		//[LockAtomic]
		//atomic<bool>を用いてLockする
		//引数1: atomic (bool)
		inline void LockAtomic(std::atomic<bool>& atomicBoolean)
		{
			bool oldFlag = false, newFlag = true;

			while (1 ^ atomicBoolean.compare_exchange_weak(oldFlag, newFlag)) {}
		}
		//----------------------------------------------------------------------------------
		//[TryLockAtomic]
		//atomic<bool>を用いてLockを試す
		//return: Lockに成功した場合true
		//引数1: atomic (bool)
		inline bool TryLockAtomic(std::atomic<bool>& atomicBoolean)
		{
			bool oldFlag = false, newFlag = true;

			return (1 ^ atomicBoolean.compare_exchange_weak(oldFlag, newFlag));
		}
		//----------------------------------------------------------------------------------
		//[TryCycleLockAtomic]
		//atomic<bool>を用いてLockをcycle回数試す
		//return: Lockに成功した場合true
		//引数1: atomic (bool)
		//引数2: cycle, forループ回数
		inline bool TryCycleLockAtomic(std::atomic<bool>& atomicBoolean, unsigned int cycle)
		{
			bool oldFlag = false, newFlag = true;

			for (unsigned int i = 0; i < cycle;)
			{
				if (1 ^ atomicBoolean.compare_exchange_weak(oldFlag, newFlag))
					++i;
				else
					return true;
			}
			return false;
		}
		//----------------------------------------------------------------------------------
		//[UnlockAtomic]
		//atomic<bool>を用いてUnlockする
		//引数1: atomic (bool)
		inline void UnlockAtomic(std::atomic<bool>& atomicBoolean)
		{
			bool oldFlag = true, newFlag = false;

			while (1 ^ atomicBoolean.compare_exchange_weak(oldFlag, newFlag)) {}
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
		//[And]
		//atomic<bool>をロックフリーアルゴリズムを用いてAND演算する
		//return: 計算結果 (atomicに代入済み)
		//引数1: atomic (bool)
		inline bool And(std::atomic<bool>& atomicBoolean, bool andFlag)
		{
			bool oldFlag = false, newFlag = false;

			do
			{
				oldFlag = atomicBoolean.load();
				newFlag = oldFlag & andFlag;
			} while (1 ^ atomicBoolean.compare_exchange_weak(oldFlag, newFlag));

			return newFlag;
		}
		//----------------------------------------------------------------------------------
		//[Or]
		//atomic<bool>をロックフリーアルゴリズムを用いてOR演算する
		//return: 計算結果 (atomicに代入済み)
		//引数1: atomic (bool)
		inline bool Or(std::atomic<bool>& atomicBoolean, bool orFlag)
		{
			bool oldFlag = false, newFlag = false;

			do
			{
				oldFlag = atomicBoolean.load();
				newFlag = oldFlag | orFlag;
			} while (1 ^ atomicBoolean.compare_exchange_weak(oldFlag, newFlag));

			return newFlag;
		}
		//----------------------------------------------------------------------------------
		//[Xor]
		//atomic<bool>をロックフリーアルゴリズムを用いてOR演算する
		//return: 計算結果 (atomicに代入済み)
		//引数1: atomic (bool)
		inline bool Xor(std::atomic<bool>& atomicBoolean, bool xorFlag)
		{
			bool oldFlag = false, newFlag = false;

			do
			{
				oldFlag = atomicBoolean.load();
				newFlag = oldFlag ^ xorFlag;
			} while (1 ^ atomicBoolean.compare_exchange_weak(oldFlag, newFlag));

			return newFlag;
		}
		//----------------------------------------------------------------------------------
		//[BitInversion]
		//atomic<bool>をロックフリーアルゴリズムを用いてビット反転させる
		//return: 計算結果 (atomicに代入済み)
		//引数1: atomic (bool)
		inline bool BitInversion(std::atomic<bool>& atomicBoolean)
		{
			bool oldFlag = false, newFlag = false;

			do
			{
				oldFlag = atomicBoolean.load();
				newFlag = oldFlag ^ 1;
			} while (1 ^ atomicBoolean.compare_exchange_weak(oldFlag, newFlag));

			return newFlag;
		}

		//----------------------------------------------------------------------------------
		//Integral
		//----------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------
		//[Add]
		//atomic<T>をロックフリーアルゴリズムを用いて加算する
		//template: integral type
		//return: 計算結果 (atomicに代入済み)
		//引数1: atomic (integral type)
		//引数2: 加算する値
		template <class T>
		inline T Add(std::atomic<T>& atomicIntegral, T add)
		{
			static_assert(std::is_integral_v<T>, "Only integral type is supported");

			T oldCounter = 0, newCounter = 0;

			do
			{
				oldCounter = atomicIntegral.load();
				newCounter = oldCounter + add;
			} while (1 ^ atomicIntegral.compare_exchange_weak(oldCounter, newCounter));

			return newCounter;
		}
		//----------------------------------------------------------------------------------
		//[Subtract]
		//atomic<T>をロックフリーアルゴリズムを用いて減算する
		//template: integral type
		//return: 計算結果 (atomicに代入済み)
		//引数1: atomic (integral type)
		//引数2: 減算する値
		template <class T>
		inline T Subtract(std::atomic<T>& atomicIntegral, T sub)
		{
			static_assert(std::is_integral_v<T>, "Only integral type is supported");

			T oldCounter = 0, newCounter = 0;

			do
			{
				oldCounter = atomicIntegral.load();
				newCounter = oldCounter - sub;
			} while (1 ^ atomicIntegral.compare_exchange_weak(oldCounter, newCounter));

			return newCounter;
		}
		//----------------------------------------------------------------------------------
		//[Subtract]
		//atomic<T>をロックフリーアルゴリズムを用いて乗算する
		//template: integral type
		//return: 計算結果 (atomicに代入済み)
		//引数1: atomic (integral type)
		//引数2: 乗算する値
		template <class T>
		inline T Multiply(std::atomic<T>& atomicIntegral, T multi)
		{
			static_assert(std::is_integral_v<T>, "Only integral type is supported");

			T oldCounter = 0, newCounter = 0;

			do
			{
				oldCounter = atomicIntegral.load();
				newCounter = oldCounter * multi;
			} while (1 ^ atomicIntegral.compare_exchange_weak(oldCounter, newCounter));

			return newCounter;
		}
		//----------------------------------------------------------------------------------
		//[Divide]
		//atomic<T>をロックフリーアルゴリズムを用いて除算する
		//template: integral type
		//return: 計算結果 (atomicに代入済み)
		//引数1: atomic (integral type)
		//引数2: 除算する値
		template <class T>
		inline T Divide(std::atomic<T>& atomicIntegral, T div)
		{
			static_assert(std::is_integral_v<T>, "Only integral type is supported");

			T oldCounter = 0, newCounter = 0;

			do
			{
				oldCounter = atomicIntegral.load();
				if (div != 0) newCounter = oldCounter / div;
			} while (1 ^ atomicIntegral.compare_exchange_weak(oldCounter, newCounter));

			return newCounter;
		}

		//----------------------------------------------------------------------------------
		//[And]
		//atomic<integral type>をロックフリーアルゴリズムを用いてAND演算する
		//template: integral type
		//return: 計算結果 (atomicに代入済み)
		//引数1: atomic (integral type)
		template <class T>
		inline T And(std::atomic<T>& atomicIntegral, T andFlag)
		{
			static_assert(std::is_integral_v<T>, "Only integral type is supported");

			T oldFlag = 0, newFlag = 0;

			do
			{
				oldFlag = atomicIntegral.load();
				newFlag = oldFlag & andFlag;
			} while (1 ^ atomicIntegral.compare_exchange_weak(oldFlag, newFlag));

			return newFlag;
		}
		//----------------------------------------------------------------------------------
		//[Or]
		//atomic<integral type>をロックフリーアルゴリズムを用いてOR演算する
		//template: integral type
		//return: 計算結果 (atomicに代入済み)
		//引数1: atomic (integral type)
		template <class T>
		inline T Or(std::atomic<T>& atomicIntegral, T orFlag)
		{
			static_assert(std::is_integral_v<T>, "Only integral type is supported");

			T oldFlag = 0, newFlag = 0;

			do
			{
				oldFlag = atomicIntegral.load();
				newFlag = oldFlag | orFlag;
			} while (1 ^ atomicIntegral.compare_exchange_weak(oldFlag, newFlag));

			return newFlag;
		}
		//----------------------------------------------------------------------------------
		//[Xor]
		//atomic<integral type>をロックフリーアルゴリズムを用いてOR演算する
		//template: integral type
		//return: 計算結果 (atomicに代入済み)
		//引数1: atomic (integral type)
		template <class T>
		inline T Xor(std::atomic<T>& atomicIntegral, T xorFlag)
		{
			static_assert(std::is_integral_v<T>, "Only integral type is supported");

			T oldFlag = 0, newFlag = 0;

			do
			{
				oldFlag = atomicIntegral.load();
				newFlag = oldFlag ^ xorFlag;
			} while (1 ^ atomicIntegral.compare_exchange_weak(oldFlag, newFlag));

			return newFlag;
		}
		//----------------------------------------------------------------------------------
		//[BitInversion]
		//atomic<integral type>をロックフリーアルゴリズムを用いてビット反転させる
		//template: integral type
		//return: 計算結果 (atomicに代入済み)
		//引数1: atomic (integral type)
		template <class T>
		inline T BitInversion(std::atomic<T>& atomicIntegral)
		{
			static_assert(std::is_integral_v<T>, "Only integral type is supported");

			T oldFlag = 0, newFlag = 0;

			do
			{
				oldFlag = atomicIntegral.load();
				newFlag = oldFlag ^ 
					static_cast<T>(std::numeric_limits<unsigned long long>::max());
			} while (1 ^ atomicIntegral.compare_exchange_weak(oldFlag, newFlag));

			return newFlag;
		}

		//----------------------------------------------------------------------------------
		//[Add]
		//atomic<T>をロックフリーアルゴリズムを用いて加算する
		//template: integral type
		//return: 計算結果 (atomicに代入済み)
		//引数1: atomic (integral type)
		//引数2: 加算する値
		template <class T, class CastType>
		inline T Add(std::atomic<T>& atomicIntegral, CastType add)
		{
			static_assert(std::is_convertible_v<CastType, T>, "Only castable type is supported");
		
			return Add(atomicIntegral, static_cast<T>(add));
		}
		//----------------------------------------------------------------------------------
		//[Subtract]
		//atomic<T>をロックフリーアルゴリズムを用いて減算する
		//template: integral type
		//return: 計算結果 (atomicに代入済み)
		//引数1: atomic (integral type)
		//引数2: 減算する値
		template <class T, class CastType>
		inline T Subtract(std::atomic<T>& atomicIntegral, CastType sub)
		{
			static_assert(std::is_convertible_v<CastType, T>, "Only castable type is supported");

			return Subtract(atomicIntegral, static_cast<T>(sub));
		}
		//----------------------------------------------------------------------------------
		//[Subtract]
		//atomic<T>をロックフリーアルゴリズムを用いて乗算する
		//template: integral type
		//return: 計算結果 (atomicに代入済み)
		//引数1: atomic (integral type)
		//引数2: 乗算する値
		template <class T, class CastType>
		inline T Multiply(std::atomic<T>& atomicIntegral, CastType multi)
		{
			static_assert(std::is_convertible_v<CastType, T>, "Only castable type is supported");

			return Multiply(atomicIntegral, static_cast<T>(multi));
		}
		//----------------------------------------------------------------------------------
		//[Divide]
		//atomic<T>をロックフリーアルゴリズムを用いて除算する
		//template: integral type
		//return: 計算結果 (atomicに代入済み)
		//引数1: atomic (integral type)
		//引数2: 除算する値
		template <class T, class CastType>
		inline T Divide(std::atomic<T>& atomicIntegral, CastType div)
		{
			static_assert(std::is_convertible_v<CastType, T>, "Only castable type is supported");

			return Divide(atomicIntegral, static_cast<T>(div));
		}

		//----------------------------------------------------------------------------------
		//[And]
		//atomic<integral type>をロックフリーアルゴリズムを用いてAND演算する
		//template: integral type
		//return: 計算結果 (atomicに代入済み)
		//引数1: atomic (integral type)
		template <class T, class CastType>
		inline T And(std::atomic<T>& atomicIntegral, CastType andFlag)
		{
			static_assert(std::is_convertible_v<CastType, T>, "Only castable type is supported");

			return And(atomicIntegral, static_cast<T>(andFlag));
		}
		//----------------------------------------------------------------------------------
		//[Or]
		//atomic<integral type>をロックフリーアルゴリズムを用いてOR演算する
		//template: integral type
		//return: 計算結果 (atomicに代入済み)
		//引数1: atomic (integral type)
		template <class T, class CastType>
		inline T Or(std::atomic<T>& atomicIntegral, CastType orFlag)
		{
			static_assert(std::is_convertible_v<CastType, T>, "Only castable type is supported");

			return Or(atomicIntegral, static_cast<T>(orFlag));
		}
		//----------------------------------------------------------------------------------
		//[Xor]
		//atomic<integral type>をロックフリーアルゴリズムを用いてOR演算する
		//template: integral type
		//return: 計算結果 (atomicに代入済み)
		//引数1: atomic (integral type)
		template <class T, class CastType>
		inline T Xor(std::atomic<T>& atomicIntegral, CastType xorFlag)
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
		//引数1: atomic (integral type)
		//引数2: 加算する値
		template <class T>
		inline T* Add(std::atomic<T*>& atomicPointer, int add)
		{
			static_assert(std::is_pointer_v<T>, "Only pointer type is supported");

			T* oldPointer = nullptr, newPointer = nullptr;

			do
			{
				oldPointer = atomicPointer.load();
				newPointer = oldPointer + add;
			} while (1 ^ atomicPointer.compare_exchange_weak(oldPointer, newPointer));

			return newPointer;
		}
		//----------------------------------------------------------------------------------
		//[Subtract]
		//atomic<T>をロックフリーアルゴリズムを用いて減算する
		//return: 計算結果 (atomicに代入済み)
		//template: integral type
		//引数1: atomic (integral type)
		//引数2: 減算する値
		template <class T>
		inline T* Subtract(std::atomic<T*>& atomicPointer, int sub)
		{
			static_assert(std::is_pointer_v<T>, "Only pointer type is supported");

			T* oldPointer = nullptr, newPointer = nullptr;

			do
			{
				oldPointer = atomicPointer.load();
				newPointer = oldPointer - sub;
			} while (1 ^ atomicPointer.compare_exchange_weak(oldPointer, newPointer));

			return newPointer;
		}
	}
}
#endif //SGFRAMEWORK_HEADER_ATOMIC_HPP_