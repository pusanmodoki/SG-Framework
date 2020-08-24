/*----------------------------------------------------------------------------------
std::atomic�𑀍삷��Atomic namespace���L�q����Atomic.hpp
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
	//std::atomic�𑀍삷��Atomic namespace
	namespace Atomic
	{
		//----------------------------------------------------------------------------------
		//All
		//----------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------
		//[Init]
		//atomic<T>�����b�N�t���[�A���S���Y����p���ď���������
		//template: atomic type
		//return: ������� (atomic�ɑ���ς�)
		//����1: atomic (atomic type)
		//����2: ����������l
		template <class T>
		inline T Init(std::atomic<T>& atomic, T init)
		{
			T expected = atomic.load();

			while (1 ^ atomic.compare_exchange_weak(expected, init)) {}

			return init;
		}
		//----------------------------------------------------------------------------------
		//[Init]
		//atomic<T>�����b�N�t���[�A���S���Y����p���ď���������
		//template: atomic type
		//return: ������� (atomic�ɑ���ς�)
		//����1: atomic (atomic type)
		//����2: ����������l
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
		//[And]
		//atomic<bool>�����b�N�t���[�A���S���Y����p����AND���Z����
		//return: �v�Z���� (atomic�ɑ���ς�)
		//����1: atomic (bool)
		inline bool And(std::atomic<bool>& atomicBoolean, bool andFlag)
		{
			bool expected = atomicBoolean.load();

			while (1 ^ atomicBoolean.compare_exchange_weak(expected, expected & andFlag)) {}

			return atomicBoolean.load();
		}
		//----------------------------------------------------------------------------------
		//[Or]
		//atomic<bool>�����b�N�t���[�A���S���Y����p����OR���Z����
		//return: �v�Z���� (atomic�ɑ���ς�)
		//����1: atomic (bool)
		inline bool Or(std::atomic<bool>& atomicBoolean, bool orFlag)
		{
			bool expected = atomicBoolean.load();

			while (1 ^ atomicBoolean.compare_exchange_weak(expected, expected | orFlag)) {}

			return atomicBoolean.load();
		}
		//----------------------------------------------------------------------------------
		//[Xor]
		//atomic<bool>�����b�N�t���[�A���S���Y����p����OR���Z����
		//return: �v�Z���� (atomic�ɑ���ς�)
		//����1: atomic (bool)
		inline bool Xor(std::atomic<bool>& atomicBoolean, bool xorFlag)
		{
			bool expected = atomicBoolean.load();

			while (1 ^ atomicBoolean.compare_exchange_weak(expected, expected ^ xorFlag)) {}

			return atomicBoolean.load();
		}
		//----------------------------------------------------------------------------------
		//[BitInversion]
		//atomic<bool>�����b�N�t���[�A���S���Y����p���ăr�b�g���]������
		//return: �v�Z���� (atomic�ɑ���ς�)
		//����1: atomic (bool)
		inline bool BitInversion(std::atomic<bool>& atomicBoolean)
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
		//atomic<bool>��p����Lock����
		//����1: atomic (bool)
		inline void LockAtomic(std::atomic<bool>& atomicBoolean)
		{
			bool expected = false;

			while (1 ^ atomicBoolean.compare_exchange_weak(expected, true)) { expected = false; }
		}
		//----------------------------------------------------------------------------------
		//[TryLockAtomic]
		//atomic<bool>��p����Lock������
		//return: Lock�ɐ��������ꍇtrue
		//����1: atomic (bool)
		inline bool TryLockAtomic(std::atomic<bool>& atomicBoolean)
		{
			bool expected = false;

			return (1 ^ atomicBoolean.compare_exchange_weak(expected, true));
		}
		//----------------------------------------------------------------------------------
		//[TryCycleLockAtomic]
		//atomic<bool>��p����Lock��cycle�񐔎���
		//return: Lock�ɐ��������ꍇtrue
		//����1: atomic (bool)
		//����2: cycle, for���[�v��
		inline bool TryCycleLockAtomic(std::atomic<bool>& atomicBoolean, unsigned int cycle)
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
		//atomic<bool>��p����Unlock����
		//����1: atomic (bool)
		inline void UnlockAtomic(std::atomic<bool>& atomicBoolean)
		{
			bool expected = true;

			while (1 ^ atomicBoolean.compare_exchange_weak(expected, false)) { expected = true; }
		}
		//Atomic��LockGuard
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
		//Atomic��TryLockGuard
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
		//atomic<T>�����b�N�t���[�A���S���Y����p���ĉ��Z����
		//template: integral type
		//return: �v�Z���� (atomic�ɑ���ς�)
		//����1: atomic (integral type)
		//����2: ���Z����l
		template <class T>
		inline T Add(std::atomic<T>& atomicIntegral, T add)
		{
			static_assert(std::is_integral_v<T>, "Only integral type is supported");

			T expected = atomicIntegral.load();

			while (1 ^ atomicIntegral.compare_exchange_weak(expected, expected + add)) {}

			return atomicIntegral.load();
		}
		//----------------------------------------------------------------------------------
		//[Subtract]
		//atomic<T>�����b�N�t���[�A���S���Y����p���Č��Z����
		//template: integral type
		//return: �v�Z���� (atomic�ɑ���ς�)
		//����1: atomic (integral type)
		//����2: ���Z����l
		template <class T>
		inline T Subtract(std::atomic<T>& atomicIntegral, T sub)
		{
			static_assert(std::is_integral_v<T>, "Only integral type is supported");

			T expected = atomicIntegral.load();

			while (1 ^ atomicIntegral.compare_exchange_weak(expected, expected - sub)) {}

			return atomicIntegral.load();
		}
		//----------------------------------------------------------------------------------
		//[Subtract]
		//atomic<T>�����b�N�t���[�A���S���Y����p���ď�Z����
		//template: integral type
		//return: �v�Z���� (atomic�ɑ���ς�)
		//����1: atomic (integral type)
		//����2: ��Z����l
		template <class T>
		inline T Multiply(std::atomic<T>& atomicIntegral, T multi)
		{
			static_assert(std::is_integral_v<T>, "Only integral type is supported");

			T expected = atomicIntegral.load();

			while (1 ^ atomicIntegral.compare_exchange_weak(expected, expected * multi)) {}

			return atomicIntegral.load();
		}
		//----------------------------------------------------------------------------------
		//[Divide]
		//atomic<T>�����b�N�t���[�A���S���Y����p���ď��Z����
		//template: integral type
		//return: �v�Z���� (atomic�ɑ���ς�)
		//����1: atomic (integral type)
		//����2: ���Z����l
		template <class T>
		inline T Divide(std::atomic<T>& atomicIntegral, T div)
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
		//atomic<integral type>�����b�N�t���[�A���S���Y����p����AND���Z����
		//template: integral type
		//return: �v�Z���� (atomic�ɑ���ς�)
		//����1: atomic (integral type)
		template <class T>
		inline T And(std::atomic<T>& atomicIntegral, T andFlag)
		{
			static_assert(std::is_integral_v<T>, "Only integral type is supported");

			T expected = atomicIntegral.load();

			while (1 ^ atomicIntegral.compare_exchange_weak(expected, expected & andFlag)) {}

			return atomicIntegral.load();
		}
		//----------------------------------------------------------------------------------
		//[Or]
		//atomic<integral type>�����b�N�t���[�A���S���Y����p����OR���Z����
		//template: integral type
		//return: �v�Z���� (atomic�ɑ���ς�)
		//����1: atomic (integral type)
		template <class T>
		inline T Or(std::atomic<T>& atomicIntegral, T orFlag)
		{
			static_assert(std::is_integral_v<T>, "Only integral type is supported");

			T expected = atomicIntegral.load();

			while (1 ^ atomicIntegral.compare_exchange_weak(expected, expected | orFlag)) {}

			return atomicIntegral.load();
		}
		//----------------------------------------------------------------------------------
		//[Xor]
		//atomic<integral type>�����b�N�t���[�A���S���Y����p����OR���Z����
		//template: integral type
		//return: �v�Z���� (atomic�ɑ���ς�)
		//����1: atomic (integral type)
		template <class T>
		inline T Xor(std::atomic<T>& atomicIntegral, T xorFlag)
		{
			static_assert(std::is_integral_v<T>, "Only integral type is supported");

			T expected = atomicIntegral.load();

			while (1 ^ atomicIntegral.compare_exchange_weak(expected, expected ^ xorFlag)) {}

			return atomicIntegral.load();
		}
		//----------------------------------------------------------------------------------
		//[BitInversion]
		//atomic<integral type>�����b�N�t���[�A���S���Y����p���ăr�b�g���]������
		//template: integral type
		//return: �v�Z���� (atomic�ɑ���ς�)
		//����1: atomic (integral type)
		template <class T>
		inline T BitInversion(std::atomic<T>& atomicIntegral)
		{
			static_assert(std::is_integral_v<T>, "Only integral type is supported");
			static constexpr unsigned long long cMaxBit = std::numeric_limits<unsigned long long>::max();

			T expected = atomicIntegral.load();

			while (1 ^ atomicIntegral.compare_exchange_weak(expected, expected ^ static_cast<T>(cMaxBit))) {}

			return atomicIntegral.load();
		}

		//----------------------------------------------------------------------------------
		//[Add]
		//atomic<T>�����b�N�t���[�A���S���Y����p���ĉ��Z����
		//template: integral type
		//return: �v�Z���� (atomic�ɑ���ς�)
		//����1: atomic (integral type)
		//����2: ���Z����l
		template <class T, class CastType>
		inline T Add(std::atomic<T>& atomicIntegral, CastType add)
		{
			static_assert(std::is_convertible_v<CastType, T>, "Only castable type is supported");

			return Add(atomicIntegral, static_cast<T>(add));
		}
		//----------------------------------------------------------------------------------
		//[Subtract]
		//atomic<T>�����b�N�t���[�A���S���Y����p���Č��Z����
		//template: integral type
		//return: �v�Z���� (atomic�ɑ���ς�)
		//����1: atomic (integral type)
		//����2: ���Z����l
		template <class T, class CastType>
		inline T Subtract(std::atomic<T>& atomicIntegral, CastType sub)
		{
			static_assert(std::is_convertible_v<CastType, T>, "Only castable type is supported");

			return Subtract(atomicIntegral, static_cast<T>(sub));
		}
		//----------------------------------------------------------------------------------
		//[Subtract]
		//atomic<T>�����b�N�t���[�A���S���Y����p���ď�Z����
		//template: integral type
		//return: �v�Z���� (atomic�ɑ���ς�)
		//����1: atomic (integral type)
		//����2: ��Z����l
		template <class T, class CastType>
		inline T Multiply(std::atomic<T>& atomicIntegral, CastType multi)
		{
			static_assert(std::is_convertible_v<CastType, T>, "Only castable type is supported");

			return Multiply(atomicIntegral, static_cast<T>(multi));
		}
		//----------------------------------------------------------------------------------
		//[Divide]
		//atomic<T>�����b�N�t���[�A���S���Y����p���ď��Z����
		//template: integral type
		//return: �v�Z���� (atomic�ɑ���ς�)
		//����1: atomic (integral type)
		//����2: ���Z����l
		template <class T, class CastType>
		inline T Divide(std::atomic<T>& atomicIntegral, CastType div)
		{
			static_assert(std::is_convertible_v<CastType, T>, "Only castable type is supported");

			return Divide(atomicIntegral, static_cast<T>(div));
		}

		//----------------------------------------------------------------------------------
		//[And]
		//atomic<integral type>�����b�N�t���[�A���S���Y����p����AND���Z����
		//template: integral type
		//return: �v�Z���� (atomic�ɑ���ς�)
		//����1: atomic (integral type)
		template <class T, class CastType>
		inline T And(std::atomic<T>& atomicIntegral, CastType andFlag)
		{
			static_assert(std::is_convertible_v<CastType, T>, "Only castable type is supported");

			return And(atomicIntegral, static_cast<T>(andFlag));
		}
		//----------------------------------------------------------------------------------
		//[Or]
		//atomic<integral type>�����b�N�t���[�A���S���Y����p����OR���Z����
		//template: integral type
		//return: �v�Z���� (atomic�ɑ���ς�)
		//����1: atomic (integral type)
		template <class T, class CastType>
		inline T Or(std::atomic<T>& atomicIntegral, CastType orFlag)
		{
			static_assert(std::is_convertible_v<CastType, T>, "Only castable type is supported");

			return Or(atomicIntegral, static_cast<T>(orFlag));
		}
		//----------------------------------------------------------------------------------
		//[Xor]
		//atomic<integral type>�����b�N�t���[�A���S���Y����p����OR���Z����
		//template: integral type
		//return: �v�Z���� (atomic�ɑ���ς�)
		//����1: atomic (integral type)
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
		//atomic<T*>�����b�N�t���[�A���S���Y����p���ĉ��Z����
		//return: �v�Z���� (atomic�ɑ���ς�)
		//template: integral type
		//����1: atomic (integral type)
		//����2: ���Z����l
		template <class T>
		inline T* Add(std::atomic<T*>& atomicPointer, int add)
		{
			static_assert(std::is_pointer_v<T>, "Only pointer type is supported");

			T* expected = atomicPointer.load();

			while (1 ^ atomicPointer.compare_exchange_weak(expected, expected + add)) {}

			return atomicPointer.load();
		}
		//----------------------------------------------------------------------------------
		//[Subtract]
		//atomic<T>�����b�N�t���[�A���S���Y����p���Č��Z����
		//return: �v�Z���� (atomic�ɑ���ς�)
		//template: integral type
		//����1: atomic (integral type)
		//����2: ���Z����l
		template <class T>
		inline T* Subtract(std::atomic<T*>& atomicPointer, int sub)
		{
			static_assert(std::is_pointer_v<T>, "Only pointer type is supported");

			T* expected = atomicPointer.load();

			while (1 ^ atomicPointer.compare_exchange_weak(expected, expected - sub)) {}

			return atomicPointer.load();
		}
	}
}
#endif //SGFRAMEWORK_HEADER_ATOMIC_HPP_