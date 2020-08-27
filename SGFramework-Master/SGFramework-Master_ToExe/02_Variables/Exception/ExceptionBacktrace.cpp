#include "ExceptionBacktrace.hpp"


//Framework namespace
namespace SGFramework
{
	//Exception namespace
	namespace Exception
	{
		void Backtrace::Startup()
		{
			m_processHandle = ::GetCurrentProcess();

			SymSetOptions(SYMOPT_DEFERRED_LOADS | SYMOPT_LOAD_LINES | SYMOPT_UNDNAME);
			if (SymInitialize(m_processHandle, nullptr, true) ^ true) throw 1;

			m_isStartup = true;
		}

		void Backtrace::Shutdown()
		{
			if (m_isStartup ^ false)
				SymCleanup(m_processHandle);
		}
	}
}

