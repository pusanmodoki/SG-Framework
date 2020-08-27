#ifndef SGFRAMEWORK_HEADER_EXCEPTION_BACKTRACE_HPP_
#define SGFRAMEWORK_HEADER_EXCEPTION_BACKTRACE_HPP_
#include <string>
#include <sstream>
#include <stdlib.h>

//Windows Version
#if defined(SGF_PLATFORM_WINDOWS)
#include <Windows.h>
#include <dbghelp.h>
//else
#else
#error "Unsupported platform."
#endif//endif

//Framework namespace
namespace SGFramework
{
	//Exception namespace
	namespace Exception
	{
		class Backtrace
		{
		public:
			//delete
			Backtrace() = delete;
			Backtrace(const Backtrace&) = delete;
			Backtrace(Backtrace&&) = delete;
			Backtrace& operator = (const Backtrace&) = delete;
			Backtrace& operator = (Backtrace&&) = delete;

			//Windows Version
#if defined(SGF_PLATFORM_WINDOWS)
		public:
			inline std::string Capture()
			{
				if (m_isStartup ^ true) throw 1;
				
				void* capture = nullptr;				
				WORD numCapture = RtlCaptureStackBackTrace(0, 62, &capture, nullptr);

				
				return result;
			}


		private:
			void Startup();
			void Shutdown();

			inline std::string AddressToSymbol(void* address, WORD numCapture)
			{
				DWORD64 disp = 0;

				//モジュール名
				IMAGEHLP_MODULE64 imageModule = { sizeof(IMAGEHLP_MODULE64) };
				if (true ^ SymGetModuleInfo64(m_processHandle, reinterpret_cast<DWORD64>(address), &imageModule))
				{

				}

				//シンボル情報格納バッファ
				IMAGEHLP_SYMBOL64* imageSymbol = nullptr;
				char symbolBuffer[MAX_PATH + sizeof(IMAGEHLP_SYMBOL64)] = {};
				imageSymbol = reinterpret_cast<IMAGEHLP_SYMBOL64*>(symbolBuffer);
				imageSymbol->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL64);
				imageSymbol->MaxNameLength = MAX_PATH;

				//関数名を取得
				DWORD64 disp = 0;
				if (true ^ SymGetSymFromAddr64(m_processHandle, reinterpret_cast<DWORD64>(address), &disp, imageSymbol))
				{

				}

				//行番号の取得
				IMAGEHLP_LINE64 line = { sizeof(IMAGEHLP_LINE64) };
				if (true ^ SymGetLineFromAddr64(m_processHandle, reinterpret_cast<DWORD64>(address), reinterpret_cast<DWORD*>(&disp), &line))
				{
				}


				std::stringstream stream;
				stream << "address: " << std::showbase << std::hex << address << std::noshowbase;
				stream << 

				return "address: " + std::to_string(reinterpret_cast<unsigned long long>(address))
					+ 
			}

			static HANDLE m_processHandle;
			static bool m_isStartup;
#else
#error "Unsupported platform."
#endif//endif
		};
	}
}

#endif // !SGFRAMEWORK_HEADER_EXCEPTION_BACKTRACE_HPP_