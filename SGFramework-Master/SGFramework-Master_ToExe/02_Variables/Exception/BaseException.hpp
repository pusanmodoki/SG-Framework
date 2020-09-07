/*----------------------------------------------------------------------------------
<BaseException.hpp> contents->ExceptionのベースとなるBaseException class
制作者: Shota Uemura
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_BASE_EXCEPTION_HPP_
#define SGFRAMEWORK_HEADER_BASE_EXCEPTION_HPP_
#include <memory>
#include <string>
#include <sstream>
#include <exception>

//Windows Version
#if defined(SGF_PLATFORM_WINDOWS)
#include <Windows.h>
//else
#else
#error "Unsupported platform."
#endif//endif

#include "../../01_MacroAndLibrarys/MacroAndLibrarys.hpp"
#include "ExceptionBacktrace.hpp"
#include "ExceptionWindow.hpp"

//Framework namespace
namespace SGFramework
{
	//Exception namespace
	namespace Exception
	{
		//Detail namespace
		namespace Detail
		{
			//ExceptionのベースとなるBaseException class
			//SGFrameworkのthrowはすべてこのクラスを継承したものを投げる
			class BaseException : public std::exception
			{
			public:
				//friend
				friend int WINAPI::WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow);
				//delete
				BaseException() = delete;
				//default
				BaseException(const BaseException&) = default;
				BaseException(BaseException&&) = default;
				BaseException& operator =(const BaseException&) = default;
				BaseException& operator =(BaseException&&) = default;

				//----------------------------------------------------------------------------------
				//[コンストラクタ]
				//初期化を行う
				//argument 1: エラーを出した関数の場所(例: Namespace::Class)
				//argument 2: エラーを出した関数名
				//argument 3: エラー内容
				//argument 4: エラーコード(ある場合), default = 0
				//argument 5: バックトレースを取得する？, default = true
				inline BaseException(const std::string& addressName, const std::string& functionName, 
					const std::string& errorText, long errorCode = 0, bool isGetBacktrace = true) noexcept
					: std::exception(("address: " + addressName + "::" + functionName + "\nmessage: " 
						+ errorText + "\nerror code: " + (errorCode != 0 ? InterpretationErrorCode(errorCode) : "<empty>")
						+ ((isGetBacktrace ^ false) ? ("\n\nbacktrace:\n" + Backtrace::Capture()) : "<empty>")).c_str()) {}

				//----------------------------------------------------------------------------------
				//[ShowMessageBox]
				//エラーメッセージを表示する
				void ShowMessageBox()
				{
					//Windows Version
#if defined(SGF_PLATFORM_WINDOWS)
					Exception::ExceptionWindow::ShowWindow(cWindowHeader, _windowText());
#else
#error "Unsupported platform."
#endif//Windows Version
				}

				//<property> std::exception::what
				SGF_INLINE_PROPERTY virtual const std::string _what() const noexcept { return what(); }
				//<property> _exceptionName() + _exceptionText() + std::exception::what(); 
				SGF_INLINE_PROPERTY virtual const std::string _whatAll() const noexcept { return _exceptionName() + "::" + _exceptionText() + "\n\n" + what(); }

				//<property, virtual> exception class name
				SGF_INLINE_PROPERTY virtual const std::string _exceptionName() const noexcept = 0;
				//<property, virtual> exception text
				SGF_INLINE_PROPERTY virtual const std::string _exceptionText() const noexcept = 0;

				//<property> error window text
				SGF_INLINE_PROPERTY const std::string _windowText() const noexcept { return _exceptionName() + "::" + _exceptionText() + "\n\n" + what(); }
			
				//error window herder
				static constexpr char cWindowHeader[] = "SGFramework error!!";

			private:
				//Windows Version
#if defined(SGF_PLATFORM_WINDOWS)
				//----------------------------------------------------------------------------------
				//[FirstStartupWin64]<static>
				//起動後すぐにする必要のある初期化処理を行う
				inline static void FirstStartupWin64() { Exception::Backtrace::Startup(); }
				//----------------------------------------------------------------------------------
				//[StartupWin64]<static>
				//初期化処理を行う
				//引数1: ウィンドウハンドル
				inline static void StartupWin64(HWND hwnd) { _hwnd() = hwnd; }
				//<property, static> hwnd
				SGF_INLINE_PROPERTY static HWND& _hwnd() { static HWND instance = nullptr; return instance; }
#else
#error "Unsupported platform."
#endif//Windows Version
			
				//----------------------------------------------------------------------------------
				//[Shutdown]<static>
				//終了処理を行う
				inline static void Shutdown() { Exception::Backtrace::Shutdown(); }

				//----------------------------------------------------------------------------------
				//[InterpretationErrorCode]<static>
				//エラーメッセージを解釈し文字列に変換する
				//argument 1: Windows = HRESULT 
				inline static std::string InterpretationErrorCode(long errorCode)
				{
					//Windows Version
#if defined(SGF_PLATFORM_WINDOWS)
					void* message;
					std::string result;

					FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER |
						FORMAT_MESSAGE_FROM_SYSTEM,
						nullptr,
						errorCode,
						MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
						reinterpret_cast<LPSTR>(&message),
						0,
						nullptr);

					if (message != nullptr)
					{
						result = (LPCSTR)message;
						LocalFree(message);
					}

					return result;
#else
#error "Unsupported platform."
#endif//Windows Version
				}
			};
		}
	}
}
#endif // !SGFRAMEWORK_HEADER_BASE_EXCEPTION_HPP_
