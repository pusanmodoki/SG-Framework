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
#include "../Exception/Exception.hpp"

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

			//<property> mutex
			SGF_PROPERTY const std::mutex& _mutex() const { return *m_mutex; }
			//<property> condition_variable
			SGF_PROPERTY const std::condition_variable& _conditionVariable() const { return m_condition; }
			//<property> condition_variable->native_handle
			SGF_PROPERTY std::condition_variable::native_handle_type _nativeHandle() { return m_condition.native_handle(); }

		protected:
			//----------------------------------------------------------------------------------
			//[コンストラクタ]
			//new初期化
			inline BaseConditionVariable() : m_mutex(new std::mutex()), m_condition() {}
			//----------------------------------------------------------------------------------
			//[コンストラクタ]
			//copy初期化
			//引数1: copy
			inline BaseConditionVariable(const BaseConditionVariable& copy) : m_mutex(copy.m_mutex), m_condition() {}
		
			SharedPointer<std::mutex> m_mutex;		//mutex
			std::condition_variable m_condition;			//condition_variable
		};
	}

	//ConditionVariable->NotifyOne限定で行えるConditionVariableOne class
	class ConditionVariableOne : public BaseClass::BaseConditionVariable
	{
	public:
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//初期化する
		inline ConditionVariableOne() : BaseConditionVariable(), m_isPredicate(false) {}
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//コピー初期化する
		//引数1: コピー
		inline ConditionVariableOne(const ConditionVariableOne& copy) : BaseConditionVariable(copy), m_isPredicate(false) {}
		//delete
		ConditionVariableOne& operator = (const ConditionVariableOne&) = delete;

		//----------------------------------------------------------------------------------
		//[NotifyOne]
		//停止解除命令を一つだけ送信する
		inline void NotifyOne() { m_mutex->lock(); m_isPredicate = true; m_mutex->unlock(); m_condition.notify_one(); }

		//----------------------------------------------------------------------------------
		//[Wait]
		//Notify関数が実行されるまでスレッドを停止する
		inline void Wait()
		{
			//lock
			std::unique_lock<std::mutex> lock(*m_mutex);

#if defined(SGF_DEBUG)
			//安全装置
			if (m_isWait) throw Exception::InvalidFunctionCallException("ConditionVariableOne", "Wait", "this function already running");
#endif // SGF_DEBUG
			//待ちフラグtrueにする
			m_isWait = true;

			//wait
			m_condition.wait(lock, [this]() { return m_isPredicate; });
			//flag = false
			m_isPredicate = false;
			m_isWait = false;
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

#if defined(SGF_DEBUG)
			//安全装置
			if (m_isWait) throw Exception::InvalidFunctionCallException("ConditionVariableOne", "WaitFor", "this function already running");
#endif // SGF_DEBUG
			//待ちフラグtrueにする
			m_isWait = true;

			//wait
			std::cv_status result = m_condition.wait_for(lock, relativeWaitTime, [this]() { return m_isPredicate; });

			//flag = false
			m_isWait = false;
			m_isPredicate = false;

			//result返却
			return result == std::cv_status::no_timeout;
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

#if defined(SGF_DEBUG)
			//安全装置
			if (m_isWait) throw Exception::InvalidFunctionCallException("ConditionVariableOne", "WaitUntil", "this function already running");
#endif // SGF_DEBUG
			//待ちフラグtrueにする
			m_isWait = true;

			//wait
			std::cv_status result = m_condition.wait_for(lock, relativeWaitTime, [this]() { return m_isPredicate; });

			//flag = false
			m_isWait = false;
			m_isPredicate = false;

			//result返却
			return result == std::cv_status::no_timeout;
		}

		//<property> condition_variable predicate (used lock)
		SGF_PROPERTY bool _isPredicate() { std::lock_guard<std::mutex> guard(*m_mutex); return m_isPredicate; }

	private:
		bool m_isPredicate;	//条件用
		bool m_isWait;			//安全装置
	};

	//ConditionVariable->NotifyAll, Oneを行えるConditionVariableAdvance class
	class ConditionVariableAdvance : public BaseClass::BaseConditionVariable
	{
	public:
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//初期化する
		inline ConditionVariableAdvance() : BaseConditionVariable(), m_isPredicates() {}
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//コピー初期化する
		//引数1: コピー
		inline ConditionVariableAdvance(const ConditionVariableAdvance& copy) : BaseConditionVariable(copy), m_isPredicates(false) {}
		//delete
		ConditionVariableOne& operator = (const ConditionVariableOne&) = delete;

		//----------------------------------------------------------------------------------
		//[RegisterThread]
		//Waitするスレッドを登録する, NotifyAll呼び出し前に登録すること
		//引数1: thread::id
		inline void RegisterThread(const std::thread::id& threadID) 
		{ 
			m_mutex->lock(); 
			if (threadID != std::thread::id() && m_isPredicates.find(threadID) == m_isPredicates.end()) 
				m_isPredicates.try_emplace(threadID, false); 
			m_mutex->unlock();
		}
		//----------------------------------------------------------------------------------
		//[NotifyOne]
		//停止解除命令を特定の待機スレッドに送信する
		//引数1: 起床させるThreadのID
		inline void NotifyOne(std::thread::id threadID)
		{
			m_mutex->lock();
#if defined(SGF_DEBUG)
			//安全装置
			if (threadID == std::thread::id() || m_isPredicates.find(threadID) == m_isPredicates.end())
				throw Exception::InvalidFunctionCallException("ConditionVariableAdvance", "NotifyOne", "threadID unregistered");
#endif // SGF_DEBUG
			m_isPredicates.at(threadID) = true;
			m_mutex->unlock(); 
			m_condition.notify_all(); 
		}
		//----------------------------------------------------------------------------------
		//[NotifyAll]
		//停止解除命令を全ての待機スレッドに送信する
		inline void NotifyAll() { m_mutex->lock(); for (auto& e : m_isPredicates) e.second = true; m_mutex->unlock(); m_condition.notify_all(); }

		//----------------------------------------------------------------------------------
		//[Wait]
		//Notify関数が実行されるまでスレッドを停止する
		inline void Wait()
		{
			//lock
			std::unique_lock<std::mutex> lock(*m_mutex);

#if defined(SGF_DEBUG)
			//安全装置
			if (std::this_thread::get_id() == std::thread::id() || m_isPredicates.find(std::this_thread::get_id()) == m_isPredicates.end())
				throw Exception::InvalidFunctionCallException("ConditionVariableAdvance", "Wait", "This thread unregistered");
#endif // SGF_DEBUG

			
			//wait
			m_condition.wait(lock, [this]() { return m_isPredicates.at(std::this_thread::get_id()); });
			//flag = false
			m_isPredicates.at(std::this_thread::get_id()) = false;
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
			
#if defined(SGF_DEBUG)
			//安全装置
			if (std::this_thread::get_id() == std::thread::id() || m_isPredicates.find(std::this_thread::get_id()) == m_isPredicates.end())
				throw Exception::InvalidFunctionCallException("ConditionVariableAdvance", "WaitFor", "This thread unregistered");
#endif // SGF_DEBUG

			//wait
			std::cv_status result = m_condition.wait_for(lock, relativeWaitTime, [this]() { return m_isPredicates.at(std::this_thread::get_id()); });

			//flag = false
			m_isPredicates.at(std::this_thread::get_id()) = false;

			//result返却
			return result == std::cv_status::no_timeout;
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

#if defined(SGF_DEBUG)
			//安全装置
			if (std::this_thread::get_id() == std::thread::id() || m_isPredicates.find(std::this_thread::get_id()) == m_isPredicates.end())
				throw Exception::InvalidFunctionCallException("ConditionVariableAdvance", "WaitUntil", "This thread unregistered");
#endif // SGF_DEBUG

			//wait
			std::cv_status result = m_condition.wait_for(lock, relativeWaitTime, [this]() { return m_isPredicates.at(std::this_thread::get_id()); });

			//flag = false
			m_isPredicates.at(std::this_thread::get_id()) = false;

			//result返却
			return result == std::cv_status::no_timeout;
		}

		//<property> condition_variable predicate (used lock)
		SGF_PROPERTY bool _isPredicate(std::thread::id threadID) 
		{ 
			std::lock_guard<std::mutex> guard(*m_mutex); 
			auto find = m_isPredicates.find(threadID); 
			return find != m_isPredicates.end() ?  find->second : false; 
		}

	private:
		//predicate map
		std::unordered_map<std::thread::id, bool> m_isPredicates;
	};
}
#endif // !SGFRAMEWORK_HEADER_CONDITION_VARABLE_HPP_