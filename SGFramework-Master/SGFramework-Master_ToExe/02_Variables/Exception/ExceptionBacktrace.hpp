/*----------------------------------------------------------------------------------
<ExceptionBacktrace.hpp> contents->バックトレースを取得できるBacktrace class
制作者: Shota Uemura
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_EXCEPTION_BACKTRACE_HPP_
#define SGFRAMEWORK_HEADER_EXCEPTION_BACKTRACE_HPP_
#include <string>
#include <sstream>
#include <memory>
#include <atomic>
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
		//prototype
		namespace Detail { class BaseException; }

		//バックトレースを取得できるBacktrace class
		class Backtrace
		{
		public:
			//friend 
			friend class Detail::BaseException;
			//delete
			Backtrace() = delete;
			Backtrace(const Backtrace&) = delete;
			Backtrace(Backtrace&&) = delete;
			Backtrace& operator = (const Backtrace&) = delete;
			Backtrace& operator = (Backtrace&&) = delete;

			//----------------------------------------------------------------------------------
			//[Capture]<static>
			//バックトレースをキャプチャする
			//return: backtrace infomations
			inline static std::string Capture()
			{
				//Lock
				bool expected = false;
				while (1 ^ m_isLock.compare_exchange_weak(expected, true)) { expected = false; }

				//Startup確認
				if (m_isStartup ^ true) return m_cInvalidSymbol;

				//Windows Version
#if defined(SGF_PLATFORM_WINDOWS)
				std::unique_ptr<void*[]> capture = std::make_unique<void*[]>(m_cStackSize);
				std::string result = "";
				
				//キャプチャ
				int numCapture = static_cast<int>(RtlCaptureStackBackTrace(0, 62, capture.get(), nullptr));
				
				//シンボルをまとめて解決する
				for (int i = 0; i < numCapture; ++i)
				{
					void* address = capture[i];
					if (address == nullptr || (AddressToSymbol(address, result,
						(i + 1 < 10 ? "line 0" : "line ") + std::to_string(i + 1)) ^ true)) break;
				}

				//Unlock
				expected = true;
				while (1 ^ m_isLock.compare_exchange_weak(expected, false)) { expected = true; }

				return result;
#else
#error "Unsupported platform."
#endif//Windows Version
			}

		private:
			//----------------------------------------------------------------------------------
			//[Startup]<static>
			//初期化を行う
#if defined(SGF_DEBUG)
			static void Startup();
#else // SGF_DEBUG
			inline static void Startup() {}
#endif // !SGF_DEBUG

			//----------------------------------------------------------------------------------
			//[Shutdown]<static>
			//終了処理を行う
#if defined(SGF_DEBUG)
			static void Shutdown();
#else // SGF_DEBUG
			inline static void Shutdown() {}
#endif // !SGF_DEBUG

			//Windows Version
#if defined(SGF_PLATFORM_WINDOWS)
			//----------------------------------------------------------------------------------
			//[AddressToSymbol]<static>
			//アドレスからシンボル情報を取得する
			//return: 取得に成功し文字列を追加できた場合はtrue
			//argument 1: address
			//argument 2: result (add)
			inline static bool AddressToSymbol(void* address, std::string& writeResult, const std::string& header)
			{
				DWORD64 disp = 0;

				////モジュール名を取得
				//IMAGEHLP_MODULE64 imageModule = { sizeof(IMAGEHLP_MODULE64) };
				//if (true ^ SymGetModuleInfo64(m_processHandle, reinterpret_cast<DWORD64>(address), &imageModule))
				//	return false;

				//シンボル情報格納バッファ
				IMAGEHLP_SYMBOL64* imageSymbol = nullptr;
				char symbolBuffer[MAX_PATH + sizeof(IMAGEHLP_SYMBOL64)] = {};
				imageSymbol = reinterpret_cast<IMAGEHLP_SYMBOL64*>(symbolBuffer);
				imageSymbol->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL64);
				imageSymbol->MaxNameLength = MAX_PATH;

				//関数名を取得
				if (true ^ SymGetSymFromAddr64(m_processHandle, reinterpret_cast<DWORD64>(address), &disp, imageSymbol))
					return false;

				//行番号の取得
				IMAGEHLP_LINE64 line = { sizeof(IMAGEHLP_LINE64) };
				if (true ^ SymGetLineFromAddr64(m_processHandle, reinterpret_cast<DWORD64>(address), reinterpret_cast<DWORD*>(&disp), &line))
					return false;

				//アドレス追加したい場合はこんな漢字で
				//std::stringstream stream;
				//stream << "address: " << std::showbase << std::hex << address << std::noshowbase;
				//stream << ", module: " << imageModule.ModuleName;
				//stream << ", function: " << imageSymbol->Name;
				//stream << ", source: " << line.FileName << "-" << line.LineNumber;
				//モジュール追加したい場合はこんな漢字で
				//writeResult += (header + ":: module: " + imageModule.ModuleName + ", function: "
				//	+ imageSymbol->Name + ", source: " + line.FileName + "-" + std::to_string(line.LineNumber) + "\n");

				//文字列として加算
				writeResult += (header + "::\tfunction: " + imageSymbol->Name + 
					"\n\tsource: " + line.FileName + "-" + std::to_string(line.LineNumber) + "\n");
				return true;
			}

			//Stack size
			static constexpr std::size_t m_cStackSize = 10;
			//Invalid symbol infomation
			static constexpr char m_cInvalidSymbol[] = "module: ???, function: ???, source: ???";

			static HANDLE m_processHandle;	//プロセスハンドル
			static std::atomic_bool m_isLock;	//ロック
			static bool m_isStartup;					//スタートアップ済み？
#else
#error "Unsupported platform."
#endif//Windows Version
		};
	}
}

#endif // !SGFRAMEWORK_HEADER_EXCEPTION_BACKTRACE_HPP_