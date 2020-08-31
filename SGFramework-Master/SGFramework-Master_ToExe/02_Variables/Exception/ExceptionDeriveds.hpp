/*----------------------------------------------------------------------------------
<ExceptionDeriveds.hpp> contents->BaseExceptionの継承クラス
制作者: Shota Uemura
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_EXCEPTION_DERIVEDS_HPP_
#define SGFRAMEWORK_HEADER_EXCEPTION_DERIVEDS_HPP_
#include "BaseException.hpp"

//Framework namespace
namespace SGFramework
{
	//Exception namespace
	namespace Exception
	{
		//throw用のDefaultException class
		//SGFrameworkのthrowはすべてこのクラスを継承したものを投げる
		class DefaultException : public Detail::BaseException
		{
		public:
			//delete
			DefaultException() = delete;
			//default
			DefaultException(const DefaultException&) = default;
			DefaultException(DefaultException&&) = default;
			DefaultException& operator =(const DefaultException&) = default;
			DefaultException& operator =(DefaultException&&) = default;

			//----------------------------------------------------------------------------------
			//[コンストラクタ]
			//初期化を行う
			//argument 1: エラーを出した関数の場所(例: Namespace::Class)
			//argument 2: エラーを出した関数名
			//argument 3: エラー内容
			//argument 4: エラーコード(ある場合), default = 0
			//argument 5: バックトレースを取得する？, default = true
			inline DefaultException(const std::string& addressName, const std::string& functionName,
				const std::string& errorText, long errorCode = 0, bool isGetBacktrace = true) noexcept
				: BaseException(addressName, functionName, errorText, errorCode, isGetBacktrace) {}

			//<property, virtual> exception class name
			SGF_PROPERTY virtual const std::string _exceptionName() const noexcept override { return "DefaultException"; }
			//<property, virtual> exception text
			SGF_PROPERTY virtual const std::string _exceptionText() const noexcept override { return "Unknown exception."; }
		};

		//無効な引数が原因の際に使われるInvalidArgumentException class
		class InvalidArgumentException : public Detail::BaseException
		{
		public:
			//delete
			InvalidArgumentException() = delete;
			//default
			InvalidArgumentException(const InvalidArgumentException&) = default;
			InvalidArgumentException(InvalidArgumentException&&) = default;
			InvalidArgumentException& operator =(const InvalidArgumentException&) = default;
			InvalidArgumentException& operator =(InvalidArgumentException&&) = default;

			//----------------------------------------------------------------------------------
			//[コンストラクタ]
			//初期化を行う
			//argument 1: エラーを出した関数の場所(例: Namespace::Class)
			//argument 2: エラーを出した関数名
			//argument 3: エラー内容
			//argument 4: エラーコード(ある場合), default = 0
			//argument 5: バックトレースを取得する？, default = true
			inline InvalidArgumentException(const std::string& addressName, const std::string& functionName,
				const std::string& errorText, long errorCode = 0, bool isGetBacktrace = true) noexcept
				: BaseException(addressName, functionName, errorText, errorCode, isGetBacktrace) {}

			//<property, virtual> exception class name
			SGF_PROPERTY const std::string _exceptionName() const noexcept override { return "InvalidArgumentException"; }
			//<property, virtual> exception text
			SGF_PROPERTY const std::string _exceptionText() const noexcept override { return "invalid argument."; }
		};


		//無効な関数呼び出しが原因の際に使われるInvalidFunctionCallException class
		class InvalidFunctionCallException : public Detail::BaseException
		{
		public:
			//delete
			InvalidFunctionCallException() = delete;
			//default
			InvalidFunctionCallException(const InvalidFunctionCallException&) = default;
			InvalidFunctionCallException(InvalidFunctionCallException&&) = default;
			InvalidFunctionCallException& operator =(const InvalidFunctionCallException&) = default;
			InvalidFunctionCallException& operator =(InvalidFunctionCallException&&) = default;

			//----------------------------------------------------------------------------------
			//[コンストラクタ]
			//初期化を行う
			//argument 1: エラーを出した関数の場所(例: Namespace::Class)
			//argument 2: エラーを出した関数名
			//argument 3: エラー内容
			//argument 4: エラーコード(ある場合), default = 0
			//argument 5: バックトレースを取得する？, default = true
			inline InvalidFunctionCallException(const std::string& addressName, const std::string& functionName,
				const std::string& errorText, long errorCode = 0, bool isGetBacktrace = true) noexcept
				: BaseException(addressName, functionName, errorText, errorCode, isGetBacktrace) {}

			//<property, virtual> exception class name
			SGF_PROPERTY const std::string _exceptionName() const noexcept override { return "InvalidFunctionCallException"; }
			//<property, virtual> exception text
			SGF_PROPERTY const std::string _exceptionText() const noexcept override { return "invalid function call."; }
		};


		//テンプレート引数に関するエラー使われるInvalidTemplateArgumentException class
		class InvalidTemplateArgumentException : public Detail::BaseException
		{
		public:
			//delete
			InvalidTemplateArgumentException() = delete;
			//default
			InvalidTemplateArgumentException(const InvalidTemplateArgumentException&) = default;
			InvalidTemplateArgumentException(InvalidTemplateArgumentException&&) = default;
			InvalidTemplateArgumentException& operator =(const InvalidTemplateArgumentException&) = default;
			InvalidTemplateArgumentException& operator =(InvalidTemplateArgumentException&&) = default;

			//----------------------------------------------------------------------------------
			//[コンストラクタ]
			//初期化を行う
			//argument 1: エラーを出した関数の場所(例: Namespace::Class)
			//argument 2: エラーを出した関数名
			//argument 3: エラー内容
			//argument 4: エラーコード(ある場合), default = 0
			//argument 5: バックトレースを取得する？, default = true
			inline InvalidTemplateArgumentException(const std::string& addressName, const std::string& functionName,
				const std::string& errorText, long errorCode = 0, bool isGetBacktrace = true) noexcept
				: BaseException(addressName, functionName, errorText, errorCode, isGetBacktrace) {}

			//<property, virtual> exception class name
			SGF_PROPERTY const std::string _exceptionName() const noexcept override { return "InvalidTemplateArgumentException"; }
			//<property, virtual> exception text
			SGF_PROPERTY const std::string _exceptionText() const noexcept override { return "invalid template argment."; }
		};
	}
}

#endif // !SGFRAMEWORK_HEADER_EXCEPTION_DERIVEDS_HPP_