/*----------------------------------------------------------------------------------
ConditionVariable�n�N���X�̃x�[�X�ƂȂ�BaseConditionVariable class
----------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_CONDITION_VARABLE_HPP_
#define SGFRAMEWORK_HEADER_CONDITION_VARABLE_HPP_
#include <condition_variable>
#include <mutex>
#include <functional>
#include <type_traits>
#include "../Pointer/Pointer.hpp"

//Framework namespace
namespace SGFramework
{
	//Base Classes
	namespace BaseClass
	{
		//ConditionVariable�n�N���X�̃x�[�X�ƂȂ�BaseConditionVariable class
		class BaseConditionVariable
		{
		public:
			//----------------------------------------------------------------------------------
			//[WaitingForAllRelease]
			//waitCounter == 0�ɂȂ�܂őҋ@����
			inline void WaitingForAllRelease()
			{
				while (true)
				{
					m_mutex->lock();
					int result = *m_waitCounter;
					m_mutex->unlock();
					if (result == 0)  break;
					else std::this_thread::yield();
				}
			}
			//using condition_variable mutex (get function property)
			SGF_FUNCTION_PROPERTY const std::mutex& getMutex() const { return *m_mutex; }
			//using condition_variable (get function property)
			SGF_FUNCTION_PROPERTY const std::condition_variable& getVariable() const { return m_condition; }
			//using condition_variable->native_handle (get function property)
			SGF_FUNCTION_PROPERTY std::condition_variable::native_handle_type getNativeHandle() { return m_condition.native_handle(); }

		protected:
			//----------------------------------------------------------------------------------
			//[�R���X�g���N�^]
			//new������
			//����1: m_waitCounter��new���邩�ۂ�
			inline BaseConditionVariable(bool isNewWaitCounter) : m_mutex(new std::mutex()), m_waitCounter(isNewWaitCounter ? new int(0) : nullptr), m_condition() {}
			//----------------------------------------------------------------------------------
			//[�R���X�g���N�^]
			//copy������
			//����1: copy
			inline BaseConditionVariable(const BaseConditionVariable& copy) : m_mutex(copy.m_mutex), m_waitCounter(copy.m_waitCounter), m_condition() {}
			//----------------------------------------------------------------------------------
			//[CopyBase]
			//���̃N���X�̕ϐ���copy����
			//����1: copy
			inline void CopyBase(const BaseConditionVariable* copy) { m_mutex = copy->m_mutex; m_waitCounter = copy->m_waitCounter; }

			SharedPointer<std::mutex> m_mutex;		//mutex
			SharedPointer<int> m_waitCounter;			//couneter
			std::condition_variable m_condition;			//condition_variable
		};
	}

	//�����ϐ��N���X���܂Ƃ߂�ConditionVariable namespace
	namespace ConditionVariable
	{
		//�����Ő�����s��ConditionVariable->Auto class
		class Auto : public BaseClass::BaseConditionVariable
		{
		public:
			//----------------------------------------------------------------------------------
			//[�R���X�g���N�^]
			//����������
			inline Auto() : BaseConditionVariable(true), m_predicate(new bool(false)) {}
			//----------------------------------------------------------------------------------
			//[�R���X�g���N�^]
			//�R�s�[����������
			//����1: �R�s�[
			inline Auto(const Auto& copy) : BaseConditionVariable(copy), m_predicate(copy.m_predicate) {}
			//copy operator, return this
			inline const Auto& operator = (const Auto& copy) { CopyBase(&copy); m_predicate = copy.m_predicate; return *this; }

			//----------------------------------------------------------------------------------
			//[NotifyOne]
			//��~�������߂���������M����
			inline void NotifyOne() { m_mutex->lock(); *m_predicate = true; m_mutex->unlock(); m_condition.notify_one(); }
			//----------------------------------------------------------------------------------
			//[NotifyAll]
			//��~�������߂�S�Ăɑ��M����
			inline void NotifyAll() { m_mutex->lock(); *m_predicate = true; m_mutex->unlock(); m_condition.notify_all(); }
			//----------------------------------------------------------------------------------
			//[Wait]
			//Notify�֐������s�����܂ŃX���b�h���~����
			inline void Wait()
			{
				//lock
				std::unique_lock<std::mutex> lock(*m_mutex);
				//add counter
				++*m_waitCounter;
				//wait
				m_condition.wait(lock, [this]() { return *m_predicate; });

				//sub counter, counter == 0 -> flag = false
				if (--*m_waitCounter == 0)
					*m_predicate = false;
			}
			//----------------------------------------------------------------------------------
			//[WaitFor]
			//Notify�֐������s�����܂� or �w�葊�Ύ��Ԍo�߂܂ŃX���b�h���~����
			//����1: ��~���Ύ���
			template <class Rep, class Period>
			inline bool WaitFor(const std::chrono::duration<Rep, Period>& relativeWaitTime)
			{
				//lock
				std::unique_lock<std::mutex> lock(*m_mutex);
				//add counter
				++*m_waitCounter;
				//wait
				bool result = m_condition.wait_for(lock, relativeWaitTime, [this]() { return *m_predicate; });
				if (IS_FALSE(result)) return result;

				//sub counter, counter == 0 -> flag = false
				if (--*m_waitCounter == 0)
					*m_predicate = false;

				return result;
			}
			//----------------------------------------------------------------------------------
			//[WaitUntil]
			//Notify�֐������s�����܂� or �w���Ύ��Ԍo�߂܂ŃX���b�h���~����
			//����1: ��~��Ύ���
			template <class Clock, class Duration>
			inline bool WaitUntil(const std::chrono::time_point<Clock, Duration>& absoluteTime)
			{
				//lock
				std::unique_lock<std::mutex> lock(*m_mutex);
				//add counter
				++*m_waitCounter;
				//wait
				bool result = m_condition.wait_until(lock, absoluteTime, [this]() { return *m_predicate; });
				if (IS_FALSE(result)) return result;

				//sub counter, counter == 0 -> flag = false
				if (--*m_waitCounter == 0)
					*m_predicate = false;

				return result;
			}

			//using condition_variable predicate (get function property)
			SGF_FUNCTION_PROPERTY const bool& getPredicate() { return *m_predicate; }

		private:
			SharedPointer<bool> m_predicate;	//Predicate
		};

		//condition_variable->wait�֐��̈���Predicate�Ŏg���l�̌^���e���v���[�g�Ŏw��A 
		//�^��template�Ŏw�肷�邪, ���̂͊O���̂��̂��Q�Ƃ��Ĉ���ConditionVariable->AutoReference class
		//template 1: Predicate�Ŏg���ϐ��^
		//template 2: Notify�֐����s���Ɏg���l, default = 1
		//template 2: wait�����ナ�Z�b�g�Ɏg���l, default = 0
		template<class NotifyType, NotifyType notifyInitializer = 1, NotifyType resetInitializer = 0>
		class AutoReference : public BaseClass::BaseConditionVariable
		{
		public:
			//----------------------------------------------------------------------------------
			//[�R���X�g���N�^]
			//����������
			//����1: �����ϐ��ƃy�A�ɂ���l, ���̎��̂����݂��Ă���Ԃ͕K���폜����Ȃ�����
			inline AutoReference(NotifyType& predicate) : BaseConditionVariable(true), m_predicate(&predicate) { *m_predicate = resetInitializer; }
			//----------------------------------------------------------------------------------
			//[�R���X�g���N�^]
			//�R�s�[����������
			//����1: �R�s�[
			inline AutoReference(const AutoReference& copy) : BaseConditionVariable(copy), m_predicate(copy.m_predicate) {}
			//copy operator, return this
			inline const AutoReference& operator = (const AutoReference& copy) { CopyBase(&copy); m_predicate = copy.m_predicate; return *this; }

			//----------------------------------------------------------------------------------
			//[NotifyOne]
			//��~�������߂���������M����
			inline void NotifyOne() { m_mutex->lock(); *m_predicate = notifyInitializer; m_mutex->unlock(); m_condition.notify_one(); }
			//----------------------------------------------------------------------------------
			//[NotifyAll]
			//��~�������߂�S�Ăɑ��M����
			inline void NotifyAll() { m_mutex->lock(); *m_predicate = notifyInitializer; m_mutex->unlock(); m_condition.notify_all(); }
			//----------------------------------------------------------------------------------
			//[Wait]
			//Notify�֐������s�����܂ŃX���b�h���~����
			inline void Wait()
			{
				//lock
				std::unique_lock<std::mutex> lock(*m_mutex);
				//add counter
				++*m_waitCounter;
				//wait
				m_condition.wait(lock, [this]() { return (*m_predicate == notifyInitializer); });

				//sub counter, counter == 0 -> flag = false
				if (--*m_waitCounter == 0)
					*m_predicate = resetInitializer;
			}
			//----------------------------------------------------------------------------------
			//[WaitFor]
			//Notify�֐������s�����܂� or �w�葊�Ύ��Ԍo�߂܂ŃX���b�h���~����
			//����1: ��~���Ύ���
			template <class Rep, class Period>
			inline bool WaitFor(const std::chrono::duration<Rep, Period>& relativeWaitTime)
			{
				//lock
				std::unique_lock<std::mutex> lock(*m_mutex);
				//add counter
				++*m_waitCounter;
				//wait
				bool result = m_condition.wait_for(lock, relativeWaitTime, [this]() { return (*m_predicate == notifyInitializer); });
				if (IS_FALSE(result)) return result;

				//sub counter, counter == 0 -> flag = false
				if (--*m_waitCounter == 0)
					*m_predicate = resetInitializer;

				return result;
			}
			//----------------------------------------------------------------------------------
			//[WaitUntil]
			//Notify�֐������s�����܂� or �w���Ύ��Ԍo�߂܂ŃX���b�h���~����
			//����1: ��~��Ύ���
			template <class Clock, class Duration>
			inline bool WaitUntil(const std::chrono::time_point<Clock, Duration>& absoluteTime)
			{
				//lock
				std::unique_lock<std::mutex> lock(*m_mutex);
				//add counter
				++*m_waitCounter;
				//wait
				bool result = m_condition.wait_until(lock, absoluteTime, [this]() { return (*m_predicate == notifyInitializer); });
				if (IS_FALSE(result)) return result;

				//sub counter, counter == 0 -> flag = false
				if (--*m_waitCounter == 0)
					*m_predicate = resetInitializer;

				return result;
			}

			//using condition_variable predicate (get function property)
			SGF_FUNCTION_PROPERTY const bool& getPredicate() { return *m_predicate; }

		private:
			NotifyType* m_predicate;	//Predicate
		};

		//condition_variable->wait�֐��̈���Predicate��function�Ƃ��ĕۑ�, �g�p����ConditionVariable->Predicate class
		class Predicate : public BaseClass::BaseConditionVariable
		{
		public:
			//----------------------------------------------------------------------------------
			//[�R���X�g���N�^]
			//����������
			//����1: wait�Ɏg�p����֐� (bool())
			inline Predicate(const std::function<bool()>& predicate) : BaseConditionVariable(false), m_predicate(predicate) {}
			//----------------------------------------------------------------------------------
			//[�R���X�g���N�^]
			//�R�s�[����������
			//����1: �R�s�[
			inline Predicate(const Predicate& copy) : BaseConditionVariable(copy), m_predicate(copy.m_predicate) {}
			//copy operator, return this
			inline const Predicate& operator = (const Predicate& copy) { CopyBase(&copy); m_predicate = copy.m_predicate; }

			//----------------------------------------------------------------------------------
			//[NotifyOne]
			//��~�������߂���������M����
			inline void NotifyOne() { m_condition.notify_one(); }
			//----------------------------------------------------------------------------------
			//[NotifyAll]
			//��~�������߂�S�Ăɑ��M����
			inline void NotifyAll() { m_condition.notify_all(); }
			//----------------------------------------------------------------------------------
			//[Wait]
			//Notify�֐������s�����܂ŃX���b�h���~����
			inline void Wait()
			{
				//lock
				std::unique_lock<std::mutex> lock(*m_mutex);
				//wait
				m_condition.wait(lock, m_predicate);
			}
			//----------------------------------------------------------------------------------
			//[WaitFor]
			//Notify�֐������s�����܂� or �w�葊�Ύ��Ԍo�߂܂ŃX���b�h���~����
			//����1: ��~���Ύ���
			template <class Rep, class Period>
			inline bool WaitFor(const std::chrono::duration<Rep, Period>& relativeWaitTime)
			{
				//lock
				std::unique_lock<std::mutex> lock(*m_mutex);
				//wait
				bool result = m_condition.wait_for(lock, relativeWaitTime, m_predicate);
				return result;
			}
			//----------------------------------------------------------------------------------
			//[WaitUntil]
			//Notify�֐������s�����܂� or �w���Ύ��Ԍo�߂܂ŃX���b�h���~����
			//����1: ��~��Ύ���
			template <class Clock, class Duration>
			inline bool WaitUntil(const std::chrono::time_point<Clock, Duration>& absoluteTime)
			{
				//lock
				std::unique_lock<std::mutex> lock(*m_mutex);
				//wait
				bool result = m_condition.wait_until(lock, absoluteTime, m_predicate);
				return result;
			}

			//using condition_variable predicate (get function property)
			SGF_FUNCTION_PROPERTY const std::function<bool()>& getPredicate() { return m_predicate; }

		private:
			std::function<bool()> m_predicate;		//Predicate
		};

		//condition_variable->wait�֐���Predicate�Ŏg�����l���肷��l���t���O (uint) �Ƃ��āA
		//�����l�w�肷��ConditionVariable->MultipleBit class
		class MultipleBit : public BaseClass::BaseConditionVariable
		{
		public:
			//----------------------------------------------------------------------------------
			//[�R���X�g���N�^]
			//����������
			//����1: ���������ƂȂ�t���O
			inline MultipleBit(uint equalBits)
				: BaseConditionVariable(true), m_predicates(new uint[3], 3)
			{
				m_predicates[m_cValueState] = 0;
				m_predicates[m_cEqualBitsState] = equalBits;
				m_predicates[m_cUnlockResultState] = 0;
			}
			//----------------------------------------------------------------------------------
			//[�R���X�g���N�^]
			//�R�s�[����������
			//����1: �R�s�[
			inline MultipleBit(const MultipleBit& copy)
				: BaseConditionVariable(copy), m_predicates(copy.m_predicates) {}
			//copy operator, return this
			inline const MultipleBit& operator = (const MultipleBit& copy)
			{
				CopyBase(&copy);
				m_predicates = copy.m_predicates;
				return *this;
			}

			//----------------------------------------------------------------------------------
			//[NotifyOne]
			//��~�������߂���������M����
			//����1: �����Ɏg���t���O
			inline void NotifyOne(uint notifyInitializer)
			{
				m_mutex->lock();
				m_predicates[m_cValueState] = notifyInitializer;
				m_mutex->unlock();
				m_condition.notify_one();
			}
			//----------------------------------------------------------------------------------
			//[NotifyAll]
			//��~�������߂�S�Ăɑ��M����
			//����1: �����Ɏg���t���O
			inline void NotifyAll(uint notifyInitializer)
			{
				m_mutex->lock();
				m_predicates[m_cValueState] = notifyInitializer;
				m_mutex->unlock();
				m_condition.notify_all();
			}
			//----------------------------------------------------------------------------------
			//[Wait]
			//Notify�֐������s�����܂ŃX���b�h���~����
			//return: �������̒l
			inline uint Wait()
			{
				//lock
				std::unique_lock<std::mutex> lock(*m_mutex);
				//add counter
				++*m_waitCounter;
				//wait
				m_condition.wait(lock, [this]() { m_predicates[m_cUnlockResultState] = (m_predicates[m_cValueState] & m_predicates[m_cEqualBitsState]); return (m_predicates[m_cUnlockResultState] != 0); });

				//sub counter, counter == 0 -> flag = false
				if (--*m_waitCounter == 0)
					m_predicates[m_cValueState] = 0;

				return m_predicates[m_cUnlockResultState];
			}
			//----------------------------------------------------------------------------------
			//[WaitFor]
			//Notify�֐������s�����܂� or �w��\/, ���Ύ��Ԍo�߂܂ŃX���b�h���~����
			//return: �^�C���A�E�g�����ꍇ��false
			//����1: ��~���Ύ���
			//����2: nullptr�łȂ��ꍇ�����������̌��ʂ���������
			template <class Rep, class Period>
			inline bool WaitFor(const std::chrono::duration<Rep, Period>& relativeWaitTime, uint* unlockResult)
			{
				//lock
				std::unique_lock<std::mutex> lock(*m_mutex);
				//add counter
				++*m_waitCounter;
			
				//wait
				bool result = m_condition.wait_for(lock, relativeWaitTime,
					[this]() { m_predicates[m_cUnlockResultState] = (m_predicates[m_cValueState] & m_predicates[m_cEqualBitsState]); return (m_predicates[m_cUnlockResultState] != 0); });
					//result
				if (unlockResult != nullptr) unlockResult = m_predicates[m_cUnlockResultState];
				//time out?
				if (IS_FALSE(result)) return result;				
				
				//sub counter, counter == 0 -> flag = false
				if (--*m_waitCounter == 0)
					m_predicates[m_cValueState] = 0;

				return result;
			}
			//----------------------------------------------------------------------------------
			//[WaitUntil]
			//Notify�֐������s�����܂� or �w���Ύ��Ԍo�߂܂ŃX���b�h���~����
			//����1: ��~��Ύ���
			//����2: nullptr�łȂ��ꍇ�����������̌��ʂ���������
			template <class Clock, class Duration>
			inline bool WaitUntil(const std::chrono::time_point<Clock, Duration>& absoluteTime, uint* unlockResult)
			{
				//lock
				std::unique_lock<std::mutex> lock(*m_mutex);
				//add counter
				++*m_waitCounter;

				//wait
				uint notify;
				bool result = m_condition.wait_until(lock, absoluteTime, 
					[this]() { m_predicates[m_cUnlockResultState] = (m_predicates[m_cValueState] & m_predicates[m_cEqualBitsState]); return (m_predicates[m_cUnlockResultState] != 0); });

				//result
				if (unlockResult != nullptr) unlockResult = m_predicates[m_cUnlockResultState];
				//time out?
				if (IS_FALSE(result)) return result;

				//sub counter, counter == 0 -> flag = false
				if (--*m_waitCounter == 0)
					m_predicates[m_cValueState] = 0;

				return result;
			}

			//using condition_variable predicate (get function property)
			SGF_FUNCTION_PROPERTY uint getPredicate() { return m_predicates[m_cValueState]; }

		private:
			static constexpr uint m_cValueState = 0;				//Value
			static constexpr uint m_cEqualBitsState = 1;		//EqualBits
			static constexpr uint m_cUnlockResultState = 2;	//UnlockResult
			SharedPointer<uint[]> m_predicates;					//Predicates
		};

		//condition_variable->wait�֐��̈���Predicate�Ŏg���l�̌^���e���v���[�g�Ŏw��, 
		//���l���肷��l���t���O�Ƃ��ĕ����l�w�肷��ConditionVariable->MultipleBitReference class
		//�^��template�Ŏw�肷�邪, ���̂͊O���̂��̂��Q�Ƃ��Ĉ���
		//template 1: Predicate�Ŏg���ϐ��^
		//template 2: wait�����ナ�Z�b�g�Ɏg���l, default = 0
		template<class NotifyType, NotifyType resetInitializer = 0>
		class MultipleBitReference : public BaseClass::BaseConditionVariable
		{
			static_assert(std::is_integral_v<NotifyType>, "MultipleBitReference -> integral type only");
		public:
			//----------------------------------------------------------------------------------
			//[�R���X�g���N�^]
			//����������
			//����1: �����ϐ��ƃy�A�ɂ���l, ���̎��̂����݂��Ă���Ԃ͕K���폜����Ȃ�����
			//����2: ���������ƂȂ�t���O (��������ꍇor���Z�Ŏw��)
			inline MultipleBitReference(NotifyType& predicate, const NotifyType& equalBits)
				: BaseConditionVariable(true), m_predicateValue(&predicate), m_predicateEqualBits(equalBits), m_unlockResult(resetInitializer) {}
			//----------------------------------------------------------------------------------
			//[�R���X�g���N�^]
			//�R�s�[����������
			//����1: �R�s�[
			inline MultipleBitReference(const MultipleBitReference& copy)
				: BaseConditionVariable(copy), m_predicateValue(copy.m_predicateValue), 
				m_predicateEqualBits(copy.m_predicateEqualBits), m_unlockResult(resetInitializer) {}

			//copy operator, return this
			inline const MultipleBitReference& operator = (const MultipleBitReference& copy)
			{
				CopyBase(&copy);
				m_predicateValue = copy.m_predicateValue;
				m_predicateEqualBits = copy.m_predicateEqualBits;
				m_unlockResult = copy.m_unlockResult;
				return *this;
			}

			//----------------------------------------------------------------------------------
			//[NotifyOne]
			//��~�������߂���������M����
			//����1: �����Ɏg���t���O
			inline void NotifyOne(const NotifyType& notifyInitializer)
			{
				m_mutex->lock();
				*m_predicateValue = notifyInitializer;
				m_mutex->unlock();
				m_condition.notify_one();
			}
			//----------------------------------------------------------------------------------
			//[NotifyAll]
			//��~�������߂�S�Ăɑ��M����
			//����1: �����Ɏg���t���O
			inline void NotifyAll(const NotifyType& notifyInitializer)
			{
				m_mutex->lock();
				*m_predicateValue = notifyInitializer;
				m_mutex->unlock();
				m_condition.notify_all();
			}
			//----------------------------------------------------------------------------------
			//[Wait]
			//Notify�֐������s�����܂ŃX���b�h���~����
			//return: �������̒l
			inline NotifyType Wait()
			{
				//lock
				std::unique_lock<std::mutex> lock(*m_mutex);
				//add counter
				++*m_waitCounter;
				//wait
				m_condition.wait(lock, [this]() { m_unlockResult = (*m_predicateValue & m_predicateEqualBits); return (m_unlockResult != 0); });

				//sub counter, counter == 0 -> flag = false
				if (--*m_waitCounter == 0)
					*m_predicateValue = resetInitializer;

				return m_unlockResult;
			}
			//----------------------------------------------------------------------------------
			//[WaitFor]
			//Notify�֐������s�����܂� or �w�葊�Ύ��Ԍo�߂܂ŃX���b�h���~����
			//return: �^�C���A�E�g�����ꍇ��false
			//����1: ��~���Ύ���
			//����2: nullptr�łȂ��ꍇ�����������̌��ʂ���������
			template <class Rep, class Period>
			inline bool WaitFor(const std::chrono::duration<Rep, Period>& relativeWaitTime, NotifyType* unlockResult)
			{
				//lock
				std::unique_lock<std::mutex> lock(*m_mutex);
				//add counter
				++*m_waitCounter;

				//wait
				bool result = m_condition.wait_for(lock, relativeWaitTime, [this]() { m_unlockResult = (*m_predicateValue & m_predicateEqualBits); return (m_unlockResult != 0); });
				if (IS_FALSE(result)) return result;

				//sub counter, counter == 0 -> flag = false
				if (--*m_waitCounter == 0)
					*m_predicateValue = resetInitializer;

				//result
				if (unlockResult != nullptr)
					unlockResult = m_unlockResult;

				return result;
			}
			//----------------------------------------------------------------------------------
			//[WaitUntil]
			//Notify�֐������s�����܂� or �w���Ύ��Ԍo�߂܂ŃX���b�h���~����
			//����1: ��~��Ύ���
			//����2: nullptr�łȂ��ꍇ�����������̌��ʂ���������
			template <class Clock, class Duration>
			inline bool WaitUntil(const std::chrono::time_point<Clock, Duration>& absoluteTime, NotifyType* unlockResult)
			{
				//lock
				std::unique_lock<std::mutex> lock(*m_mutex);
				//add counter
				++*m_waitCounter;

				//wait
				bool result = m_condition.wait_until(lock, absoluteTime, [this]() { m_unlockResult = (*m_predicateValue & m_predicateEqualBits); return (m_unlockResult != 0); });
				if (IS_FALSE(result)) return result;

				//sub counter, counter == 0 -> flag = false
				if (--*m_waitCounter == 0)
					*m_predicateValue = resetInitializer;

				//result
				if (unlockResult != nullptr)
					unlockResult = m_unlockResult;

				return result;
			}

			//using condition_variable predicate (get function property)
			SGF_FUNCTION_PROPERTY const NotifyType& getPredicate() { return *m_predicateValue; }

		private:
			NotifyType* m_predicateValue;		//Predicate
			NotifyType m_predicateEqualBits;	//Equal
			NotifyType m_unlockResult;			//Wait Result
		};
	}
}
#endif // !SGFRAMEWORK_HEADER_CONDITION_VARABLE_HPP_