/*----------------------------------------------------------------------------------
Exception namespaceを記述したException.hpp
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_EXCEPTION_HPP_
#define SGFRAMEWORK_HEADER_EXCEPTION_HPP_
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

#include "../MacroAndUsing/MacroAndUsing.hpp"

//Framework namespace
namespace SGFramework
{
	//Exception namespace
	namespace Exception
	{
		//Detail namespace
		namespace Detail
		{
			//DefaultExceptionのベースとなるExceptionInterface class
			class ExceptionInterface
			{
			public:
				//<property, virtual> exception class name
				SGF_PROPERTY virtual const std::string _exceptionName() const noexcept = 0;
				//<property, virtual> exception text
				SGF_PROPERTY virtual const std::string _exceptionText() const noexcept = 0;
			};
		}


		//throw用のDefaultException class
		//SGFrameworkのthrowはすべてこのクラスを継承したものを投げる
		//継承する際はprotected版のコンストラクタを呼び出し最後の引数をfalseにすることを推奨
		class DefaultException : public Detail::ExceptionInterface, protected std::exception
		{
		public:
			//delete
			inline DefaultException() = delete;
			//default
			inline DefaultException(const DefaultException& copy) = default;
			//----------------------------------------------------------------------------------
			//[コンストラクタ]
			//初期化を行う
			//argument 1: エラーを出した関数の場所(例: Namespace::Class)
			//argument 2: エラーを出した関数名
			//argument 3: エラー内容
			//argument 4: エラーコード(ある場合), default = 0
			//argument 
			inline DefaultException(const std::string& addressName, const std::string& functionName, const std::string& errorText, long errorCode = 0, bool isShowMessageBox = true, bool isGetBacktrace = true) noexcept
				: std::exception(("address: " + addressName + "::" + functionName + "\nerror code: " + (errorCode != 0 ? InterpretationErrorCode(errorCode) : "<empty>") + "\nmessage: " + errorText
					+ ((isGetBacktrace ^ false) ? ("\nbacktrace: " + std::string()) : "")).c_str()) {}

			//<property> std::exception::what
			SGF_PROPERTY virtual const std::string _what() const noexcept { return what(); }
			//<property, virtual> exception class name
			SGF_PROPERTY virtual const std::string _exceptionName() const noexcept override { return "DefaultException"; }
			//<property, virtual> exception text
			SGF_PROPERTY virtual const std::string _exceptionText() const noexcept override { return "Unknown exception."; }

			//<property> error window text
			SGF_PROPERTY const std::string _windowText() const noexcept { return _exceptionName() + "::" + _exceptionText() + "\n" + what(); }
			
			//error window herder
			static constexpr char cWindowHeader[] = "SGFramework error!!";

		protected:
			//----------------------------------------------------------------------------------
			//[ShowMessageBox]
			//エラーメッセージを表示する
			void ShowMessageBox()
			{
				//Windows Version
#if defined(SGF_PLATFORM_WINDOWS)
				MessageBoxA(_hwnd(), _windowText().c_str(), cWindowHeader, MB_OK | MB_ICONERROR);
#endif//endif
			}

		private:
//Windows Version
#if defined(SGF_PLATFORM_WINDOWS)
			//----------------------------------------------------------------------------------
			//[Initialize_Winx64]
			//HWNDの登録を行う
			//引数1: HWND
			inline static void Initialize_WinX64(HWND hwnd) { _hwnd() = hwnd; }
			//<property, static> hwnd
			SGF_PROPERTY static HWND& _hwnd() { static HWND instance = nullptr; return instance; }
#endif//endif

			//----------------------------------------------------------------------------------
			//[InterpretationErrorCode]
			//エラーメッセージを解釈し文字列に変換する
			//argument 1: Windows = HRESULT 
			inline std::string InterpretationErrorCode(long errorCode)
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
#endif
			}
		};

		//無効な引数が原因の際に使われるInvalidArgumentException class
		class InvalidArgumentException : public DefaultException
		{
		public:
			//----------------------------------------------------------------------------------
			//[コンストラクタ]
			//初期化を行う
			//argument 1: エラーを出した関数の場所(例: Namespace::Class)
			//argument 2: エラーを出した関数名
			//argument 3: エラー内容
			//argument 4: エラーコード(ある場合)
			inline InvalidArgumentException(const std::string& addressName, const std::string& functionName, const std::string& errorText, long errorCode = 0) noexcept
				: DefaultException(addressName, functionName, errorText, errorCode, false) { ShowMessageBox(); }

			//<property, virtual> exception class name
			SGF_PROPERTY const std::string _exceptionName() const noexcept override { return "InvalidArgumentException"; }
			//<property, virtual> exception text
			SGF_PROPERTY const std::string _exceptionText() const noexcept override { return "invalid argument."; }
		};


		//無効な関数呼び出しが原因の際に使われるInvalidFunctionCallException class
		class InvalidFunctionCallException : public DefaultException
		{
		public:
			//----------------------------------------------------------------------------------
			//[コンストラクタ]
			//初期化を行う
			//argument 1: エラーを出した関数の場所(例: Namespace::Class)
			//argument 2: エラーを出した関数名
			//argument 3: エラー内容
			//argument 4: エラーコード(ある場合)
			inline InvalidFunctionCallException(const std::string& addressName, const std::string& functionName, const std::string& errorText, long errorCode = 0) noexcept
				: DefaultException(addressName, functionName, errorText, errorCode, false) { ShowMessageBox(); }

			//<property, virtual> exception class name
			SGF_PROPERTY const std::string _exceptionName() const noexcept override { return "InvalidFunctionCallException"; }
			//<property, virtual> exception text
			SGF_PROPERTY const std::string _exceptionText() const noexcept override { return "invalid function call."; }
		};


		//テンプレート引数に関するエラー使われるInvalidTemplateArgumentException class
		class InvalidTemplateArgumentException : public DefaultException
		{
		public:
			//----------------------------------------------------------------------------------
			//[コンストラクタ]
			//初期化を行う
			//argument 1: エラーを出した関数の場所(例: Namespace::Class)
			//argument 2: エラーを出した関数名
			//argument 3: エラー内容
			//argument 4: エラーコード(ある場合)
			inline InvalidTemplateArgumentException(const std::string& addressName, const std::string& functionName, const std::string& errorText, long errorCode = 0) noexcept
				: DefaultException(addressName, functionName, errorText, errorCode, false) { ShowMessageBox(); }

			//<property, virtual> exception class name
			SGF_PROPERTY const std::string _exceptionName() const noexcept override { return "InvalidTemplateArgumentException"; }
			//<property, virtual> exception text
			SGF_PROPERTY const std::string _exceptionText() const noexcept override { return "invalid template argment."; }
		};
	}
}
#endif // !SGFRAMEWORK_HEADER_EXCEPTION_HPP_
