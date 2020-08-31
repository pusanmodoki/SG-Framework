/*----------------------------------------------------------------------------------
<ExceptionBacktrace.cpp> contents->バックトレースを取得できるBacktrace class
制作者: Shota Uemura
------------------------------------------------------------------------------------*/
#include "ExceptionBacktrace.hpp"

//Framework namespace
namespace SGFramework
{
	//Exception namespace
	namespace Exception
	{
		//Windows Version
#if defined(SGF_PLATFORM_WINDOWS)
		//プロセスハンドル
		HANDLE Backtrace::m_processHandle = nullptr;
		//ロック
		std::atomic_bool Backtrace::m_isLock = false;
		//スタートアップ済み？
		bool Backtrace::m_isStartup = false;
#else
#error "Unsupported platform."
#endif//Windows Version

//デバッグ時のみ動作
#ifdef SGF_DEBUG
		//----------------------------------------------------------------------------------
		//[Startup]
		//初期化を行う
		void Backtrace::Startup()
		{
			//Lock
			bool expected = false;
			while (1 ^ m_isLock.compare_exchange_weak(expected, true)) { expected = false; }

			//Windows Version
	#if defined(SGF_PLATFORM_WINDOWS)
			//プロセスハンドルを取得
			m_processHandle = ::GetCurrentProcess();

			//初期化
			SymSetOptions(SYMOPT_DEFERRED_LOADS | SYMOPT_LOAD_LINES | SYMOPT_UNDNAME);
			if (SymInitialize(m_processHandle, nullptr, true) ^ true) 
				throw std::exception("Backtrace::Startup->SymInitialize failed.");

			//フラグ初期化
			m_isStartup = true;
	#else
	#error "Unsupported platform."
	#endif//Windows Version

			//Unlock
			expected = true;
			while (1 ^ m_isLock.compare_exchange_weak(expected, false)) { expected = true; }
		}
		//----------------------------------------------------------------------------------
		//[Shutdown]
		//終了処理を行う
		void Backtrace::Shutdown()
		{
			//Lock
			bool expected = false;
			while (1 ^ m_isLock.compare_exchange_weak(expected, true)) { expected = false; }

			//Windows Version
	#if defined(SGF_PLATFORM_WINDOWS)
			//初期化済みならクリーンアップ
			if (m_isStartup ^ false)
			{
				SymCleanup(m_processHandle);
				m_isStartup = false;
			}
	#else
	#error "Unsupported platform."
	#endif//Windows Version

			//Unlock
			expected = true;
			while (1 ^ m_isLock.compare_exchange_weak(expected, false)) { expected = true; }
		}
#endif //SGF_DEBUG
	}
}

