/*----------------------------------------------------------------------------------
ConditionVariable系クラスのベースとなるBaseConditionVariable class
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
		//ConditionVariable系クラスのベースとなるBaseConditionVariable class
		class BaseConditionVariable
		{
		public:
			//----------------------------------------------------------------------------------
			//[WaitingForAllRelease]
			//waitCounter == 0になるまで待機する
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
			//[コンストラクタ]
			//new初期化
			//引数1: m_waitCounterをnewするか否か
			inline BaseConditionVariable(bool isNewWaitCounter) : m_mutex(new std::mutex()), m_waitCounter(isNewWaitCounter ? new int(0) : nullptr), m_condition() {}
			//----------------------------------------------------------------------------------
			//[コンストラクタ]
			//copy初期化
			//引数1: copy
			inline BaseConditionVariable(const BaseConditionVariable& copy) : m_mutex(copy.m_mutex), m_waitCounter(copy.m_waitCounter), m_condition() {}
			//----------------------------------------------------------------------------------
			//[CopyBase]
			//このクラスの変数をcopyする
			//引数1: copy
			inline void CopyBase(const BaseConditionVariable* copy) { m_mutex = copy->m_mutex; m_waitCounter = copy->m_waitCounter; }

			SharedPointer<std::mutex> m_mutex;		//mutex
			SharedPointer<int> m_waitCounter;			//couneter
			std::condition_variable m_condition;			//condition_variable
		};
	}

	//条件変数クラスをまとめたConditionVariable namespace
	namespace ConditionVariable
	{
		//自動で制御を行うConditionVariable->Auto class
		class Auto : public BaseClass::BaseConditionVariable
		{
		public:
			//----------------------------------------------------------------------------------
			//[コンストラクタ]
			//初期化する
			inline Auto() : BaseConditionVariable(true), m_predicate(new bool(false)) {}
			//----------------------------------------------------------------------------------
			//[コンストラクタ]
			//コピー初期化する
			//引数1: コピー
			inline Auto(const Auto& copy) : BaseConditionVariable(copy), m_predicate(copy.m_predicate) {}
			//copy operator, return this
			inline const Auto& operator = (const Auto& copy) { CopyBase(&copy); m_predicate = copy.m_predicate; return *this; }

			//----------------------------------------------------------------------------------
			//[NotifyOne]
			//停止解除命令を一つだけ送信する
			inline void NotifyOne() { m_mutex->lock(); *m_predicate = true; m_mutex->unlock(); m_condition.notify_one(); }
			//----------------------------------------------------------------------------------
			//[NotifyAll]
			//停止解除命令を全てに送信する
			inline void NotifyAll() { m_mutex->lock(); *m_predicate = true; m_mutex->unlock(); m_condition.notify_all(); }
			//----------------------------------------------------------------------------------
			//[Wait]
			//Notify関数が実行されるまでスレッドを停止する
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
			//Notify関数が実行されるまで or 指定相対時間経過までスレッドを停止する
			//引数1: 停止相対時間
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
			//Notify関数が実行されるまで or 指定絶対時間経過までスレッドを停止する
			//引数1: 停止絶対時間
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

		//condition_variable->wait関数の引数Predicateで使う値の型をテンプレートで指定、 
		//型はtemplateで指定するが, 実体は外部のものを参照して扱うConditionVariable->AutoReference class
		//template 1: Predicateで使う変数型
		//template 2: Notify関数実行時に使う値, default = 1
		//template 2: wait解除後リセットに使う値, default = 0
		template<class NotifyType, NotifyType notifyInitializer = 1, NotifyType resetInitializer = 0>
		class AutoReference : public BaseClass::BaseConditionVariable
		{
		public:
			//----------------------------------------------------------------------------------
			//[コンストラクタ]
			//初期化する
			//引数1: 条件変数とペアにする値, この実体が存在している間は必ず削除されないこと
			inline AutoReference(NotifyType& predicate) : BaseConditionVariable(true), m_predicate(&predicate) { *m_predicate = resetInitializer; }
			//----------------------------------------------------------------------------------
			//[コンストラクタ]
			//コピー初期化する
			//引数1: コピー
			inline AutoReference(const AutoReference& copy) : BaseConditionVariable(copy), m_predicate(copy.m_predicate) {}
			//copy operator, return this
			inline const AutoReference& operator = (const AutoReference& copy) { CopyBase(&copy); m_predicate = copy.m_predicate; return *this; }

			//----------------------------------------------------------------------------------
			//[NotifyOne]
			//停止解除命令を一つだけ送信する
			inline void NotifyOne() { m_mutex->lock(); *m_predicate = notifyInitializer; m_mutex->unlock(); m_condition.notify_one(); }
			//----------------------------------------------------------------------------------
			//[NotifyAll]
			//停止解除命令を全てに送信する
			inline void NotifyAll() { m_mutex->lock(); *m_predicate = notifyInitializer; m_mutex->unlock(); m_condition.notify_all(); }
			//----------------------------------------------------------------------------------
			//[Wait]
			//Notify関数が実行されるまでスレッドを停止する
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
			//Notify関数が実行されるまで or 指定相対時間経過までスレッドを停止する
			//引数1: 停止相対時間
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
			//Notify関数が実行されるまで or 指定絶対時間経過までスレッドを停止する
			//引数1: 停止絶対時間
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

		//condition_variable->wait関数の引数Predicateをfunctionとして保存, 使用するConditionVariable->Predicate class
		class Predicate : public BaseClass::BaseConditionVariable
		{
		public:
			//----------------------------------------------------------------------------------
			//[コンストラクタ]
			//初期化する
			//引数1: waitに使用する関数 (bool())
			inline Predicate(const std::function<bool()>& predicate) : BaseConditionVariable(false), m_predicate(predicate) {}
			//----------------------------------------------------------------------------------
			//[コンストラクタ]
			//コピー初期化する
			//引数1: コピー
			inline Predicate(const Predicate& copy) : BaseConditionVariable(copy), m_predicate(copy.m_predicate) {}
			//copy operator, return this
			inline const Predicate& operator = (const Predicate& copy) { CopyBase(&copy); m_predicate = copy.m_predicate; }

			//----------------------------------------------------------------------------------
			//[NotifyOne]
			//停止解除命令を一つだけ送信する
			inline void NotifyOne() { m_condition.notify_one(); }
			//----------------------------------------------------------------------------------
			//[NotifyAll]
			//停止解除命令を全てに送信する
			inline void NotifyAll() { m_condition.notify_all(); }
			//----------------------------------------------------------------------------------
			//[Wait]
			//Notify関数が実行されるまでスレッドを停止する
			inline void Wait()
			{
				//lock
				std::unique_lock<std::mutex> lock(*m_mutex);
				//wait
				m_condition.wait(lock, m_predicate);
			}
			//----------------------------------------------------------------------------------
			//[WaitFor]
			//Notify関数が実行されるまで or 指定相対時間経過までスレッドを停止する
			//引数1: 停止相対時間
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
			//Notify関数が実行されるまで or 指定絶対時間経過までスレッドを停止する
			//引数1: 停止絶対時間
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

		//condition_variable->wait関数のPredicateで使う同値判定する値をフラグ (uint) として、
		//複数値指定するConditionVariable->MultipleBit class
		class MultipleBit : public BaseClass::BaseConditionVariable
		{
		public:
			//----------------------------------------------------------------------------------
			//[コンストラクタ]
			//初期化する
			//引数1: 解除条件となるフラグ
			inline MultipleBit(uint equalBits)
				: BaseConditionVariable(true), m_predicates(new uint[3], 3)
			{
				m_predicates[m_cValueState] = 0;
				m_predicates[m_cEqualBitsState] = equalBits;
				m_predicates[m_cUnlockResultState] = 0;
			}
			//----------------------------------------------------------------------------------
			//[コンストラクタ]
			//コピー初期化する
			//引数1: コピー
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
			//停止解除命令を一つだけ送信する
			//引数1: 解除に使うフラグ
			inline void NotifyOne(uint notifyInitializer)
			{
				m_mutex->lock();
				m_predicates[m_cValueState] = notifyInitializer;
				m_mutex->unlock();
				m_condition.notify_one();
			}
			//----------------------------------------------------------------------------------
			//[NotifyAll]
			//停止解除命令を全てに送信する
			//引数1: 解除に使うフラグ
			inline void NotifyAll(uint notifyInitializer)
			{
				m_mutex->lock();
				m_predicates[m_cValueState] = notifyInitializer;
				m_mutex->unlock();
				m_condition.notify_all();
			}
			//----------------------------------------------------------------------------------
			//[Wait]
			//Notify関数が実行されるまでスレッドを停止する
			//return: 解除時の値
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
			//Notify関数が実行されるまで or 指定\/, 相対時間経過までスレッドを停止する
			//return: タイムアウトした場合はfalse
			//引数1: 停止相対時間
			//引数2: nullptrでない場合解除した時の結果が代入される
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
			//Notify関数が実行されるまで or 指定絶対時間経過までスレッドを停止する
			//引数1: 停止絶対時間
			//引数2: nullptrでない場合解除した時の結果が代入される
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

		//condition_variable->wait関数の引数Predicateで使う値の型をテンプレートで指定, 
		//同値判定する値をフラグとして複数値指定するConditionVariable->MultipleBitReference class
		//型はtemplateで指定するが, 実体は外部のものを参照して扱う
		//template 1: Predicateで使う変数型
		//template 2: wait解除後リセットに使う値, default = 0
		template<class NotifyType, NotifyType resetInitializer = 0>
		class MultipleBitReference : public BaseClass::BaseConditionVariable
		{
			static_assert(std::is_integral_v<NotifyType>, "MultipleBitReference -> integral type only");
		public:
			//----------------------------------------------------------------------------------
			//[コンストラクタ]
			//初期化する
			//引数1: 条件変数とペアにする値, この実体が存在している間は必ず削除されないこと
			//引数2: 解除条件となるフラグ (複数ある場合or演算で指定)
			inline MultipleBitReference(NotifyType& predicate, const NotifyType& equalBits)
				: BaseConditionVariable(true), m_predicateValue(&predicate), m_predicateEqualBits(equalBits), m_unlockResult(resetInitializer) {}
			//----------------------------------------------------------------------------------
			//[コンストラクタ]
			//コピー初期化する
			//引数1: コピー
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
			//停止解除命令を一つだけ送信する
			//引数1: 解除に使うフラグ
			inline void NotifyOne(const NotifyType& notifyInitializer)
			{
				m_mutex->lock();
				*m_predicateValue = notifyInitializer;
				m_mutex->unlock();
				m_condition.notify_one();
			}
			//----------------------------------------------------------------------------------
			//[NotifyAll]
			//停止解除命令を全てに送信する
			//引数1: 解除に使うフラグ
			inline void NotifyAll(const NotifyType& notifyInitializer)
			{
				m_mutex->lock();
				*m_predicateValue = notifyInitializer;
				m_mutex->unlock();
				m_condition.notify_all();
			}
			//----------------------------------------------------------------------------------
			//[Wait]
			//Notify関数が実行されるまでスレッドを停止する
			//return: 解除時の値
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
			//Notify関数が実行されるまで or 指定相対時間経過までスレッドを停止する
			//return: タイムアウトした場合はfalse
			//引数1: 停止相対時間
			//引数2: nullptrでない場合解除した時の結果が代入される
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
			//Notify関数が実行されるまで or 指定絶対時間経過までスレッドを停止する
			//引数1: 停止絶対時間
			//引数2: nullptrでない場合解除した時の結果が代入される
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