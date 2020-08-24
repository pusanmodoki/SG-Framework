/*----------------------------------------------------------------------------------
Exception namespaceを記述したException.hpp
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_EXCEPTION_HPP_
#define SGFRAMEWORK_HEADER_EXCEPTION_HPP_
#include <memory>
#include <string>
#include <sstream>
#include <exception>
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
			class ExceptionInterface : std::exception
			{
			public:
				//<property, virtual> exception class name
				SGF_PROPERTY virtual const std::string& _exceptionName() const = 0;
				//<property, virtual> exception text
				SGF_PROPERTY virtual const std::string& _exceptionText() const = 0;
			};
		}

		//throw用のDefaultException class
		//SGFrameworkのthrowはすべてこのクラスを継承したものを投げる
		class DefaultException : public Detail::ExceptionInterface
		{
		public:
			//delete
			inline DefaultException() = delete;
			//default
			inline DefaultException(const DefaultException& copy) = default;
			//----------------------------------------------------------------------------------
			//[コンストラクタ]
			//初期化を行う
			//引数1: エラーを出した関数の場所(例: Namespace::Class)
			//引数2: エラーを出した関数名
			//引数3: エラー内容
			//引数4: エラーコード(ある場合)
			inline DefaultException(const std::string& addressName, const std::string& functionName, const std::string& errorText, long errorCode = 0)
				: m_addressName(addressName), m_functionName(functionName), m_text(errorText), m_errorCode(errorCode) {}

			//std用
			const char* what() const override { return _allText().c_str(); }

			//<property> error class name
			SGF_PROPERTY const std::string& _addressName() const { return m_addressName; }
			//<property> error function name
			SGF_PROPERTY const std::string& _functionName() const { return m_functionName; }
			//<property> error text
			SGF_PROPERTY const std::string& _text() const { return m_text; }
			//<property> error code
			SGF_PROPERTY long _errorCode() const { return m_errorCode; }

			//<property, virtual> exception class name
			SGF_PROPERTY virtual const std::string& _exceptionName() const override { return "DefaultException"; }
			//<property, virtual> exception text
			SGF_PROPERTY virtual const std::string& _exceptionText() const override { return "Unknown exception."; }

			//<property> all error text
			SGF_PROPERTY const std::string& _allText() const { return _windowHeader() + "  " + _windowText(); }
			//<property> error window herder
			SGF_PROPERTY const std::string& _windowHeader() const { return "SGFramework error!!"; }
			//<property> error window text
			SGF_PROPERTY const std::string& _windowText() const 
			{ 
				//x16に変換したエラーコード
				std::stringstream stream; stream << std::showbase; stream << std::hex << m_errorCode;
				//エラー内容返却
				return _exceptionName() + "::" + _exceptionText() + "\n"
#ifdef SGF_DEBUG
					 + (m_addressName != "" ? ("<Debug>class: " + m_addressName) : "") + "<Debug>function: " + m_functionName + "\n"
#endif // SGF_DEBUG
					+ "Error contents: " + (m_text != "" ? m_text : "unknown error.") + "\n"
					+ (m_errorCode != 0 ? ("Error code: " + stream.str()) : "");
			}
			
		private:
			std::string m_addressName;		//クラス名
			std::string m_functionName;	//関数名
			std::string m_text;					//エラー内容
			long m_errorCode;					//エラーコード(ある場合)
		};

		//無効な引数が原因の際に使われるInvalidArgumentException class
		class InvalidArgumentException : DefaultException
		{
		public:
			//----------------------------------------------------------------------------------
			//[コンストラクタ]
			//初期化を行う
			//引数1: エラーを出した関数の場所(例: Namespace::Class)
			//引数2: エラーを出した関数名
			//引数3: エラー内容
			//引数4: エラーコード(ある場合)
			inline InvalidArgumentException(const std::string& addressName, const std::string& functionName, const std::string& errorText, long errorCode = 0)
				: DefaultException(addressName, functionName, errorText, errorCode) {}

			//<property, virtual> exception class name
			SGF_PROPERTY const std::string& _exceptionName() const override { return "InvalidArgumentException"; }
			//<property, virtual> exception text
			SGF_PROPERTY const std::string& _exceptionText() const override { return "invalid argument."; }
		};

		//無効な関数呼び出しが原因の際に使われるInvalidFunctionCallException class
		class InvalidFunctionCallException : DefaultException
		{
		public:
			//----------------------------------------------------------------------------------
			//[コンストラクタ]
			//初期化を行う
			//引数1: エラーを出した関数の場所(例: Namespace::Class)
			//引数2: エラーを出した関数名
			//引数3: エラー内容
			//引数4: エラーコード(ある場合)
			inline InvalidFunctionCallException(const std::string& addressName, const std::string& functionName, const std::string& errorText, long errorCode = 0)
				: DefaultException(addressName, functionName, errorText, errorCode) {}

			//<property, virtual> exception class name
			SGF_PROPERTY const std::string& _exceptionName() const override { return "InvalidFunctionCallException"; }
			//<property, virtual> exception text
			SGF_PROPERTY const std::string& _exceptionText() const override { return "invalid function call."; }
		};
	}
}
#endif // !SGFRAMEWORK_HEADER_EXCEPTION_HPP_
