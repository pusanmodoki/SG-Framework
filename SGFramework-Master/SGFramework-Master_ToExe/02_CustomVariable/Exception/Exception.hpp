/*----------------------------------------------------------------------------------
Exception namespace���L�q����Exception.hpp
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
			//DefaultException�̃x�[�X�ƂȂ�ExceptionInterface class
			class ExceptionInterface : std::exception
			{
			public:
				//<property, virtual> exception class name
				SGF_PROPERTY virtual const std::string& _exceptionName() const = 0;
				//<property, virtual> exception text
				SGF_PROPERTY virtual const std::string& _exceptionText() const = 0;
			};
		}

		//throw�p��DefaultException class
		//SGFramework��throw�͂��ׂĂ��̃N���X���p���������̂𓊂���
		class DefaultException : public Detail::ExceptionInterface
		{
		public:
			//delete
			inline DefaultException() = delete;
			//default
			inline DefaultException(const DefaultException& copy) = default;
			//----------------------------------------------------------------------------------
			//[�R���X�g���N�^]
			//���������s��
			//����1: �G���[���o�����֐��̏ꏊ(��: Namespace::Class)
			//����2: �G���[���o�����֐���
			//����3: �G���[���e
			//����4: �G���[�R�[�h(����ꍇ)
			inline DefaultException(const std::string& addressName, const std::string& functionName, const std::string& errorText, long errorCode = 0)
				: m_addressName(addressName), m_functionName(functionName), m_text(errorText), m_errorCode(errorCode) {}

			//std�p
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
				//x16�ɕϊ������G���[�R�[�h
				std::stringstream stream; stream << std::showbase; stream << std::hex << m_errorCode;
				//�G���[���e�ԋp
				return _exceptionName() + "::" + _exceptionText() + "\n"
#ifdef SGF_DEBUG
					 + (m_addressName != "" ? ("<Debug>class: " + m_addressName) : "") + "<Debug>function: " + m_functionName + "\n"
#endif // SGF_DEBUG
					+ "Error contents: " + (m_text != "" ? m_text : "unknown error.") + "\n"
					+ (m_errorCode != 0 ? ("Error code: " + stream.str()) : "");
			}
			
		private:
			std::string m_addressName;		//�N���X��
			std::string m_functionName;	//�֐���
			std::string m_text;					//�G���[���e
			long m_errorCode;					//�G���[�R�[�h(����ꍇ)
		};

		//�����Ȉ����������̍ۂɎg����InvalidArgumentException class
		class InvalidArgumentException : DefaultException
		{
		public:
			//----------------------------------------------------------------------------------
			//[�R���X�g���N�^]
			//���������s��
			//����1: �G���[���o�����֐��̏ꏊ(��: Namespace::Class)
			//����2: �G���[���o�����֐���
			//����3: �G���[���e
			//����4: �G���[�R�[�h(����ꍇ)
			inline InvalidArgumentException(const std::string& addressName, const std::string& functionName, const std::string& errorText, long errorCode = 0)
				: DefaultException(addressName, functionName, errorText, errorCode) {}

			//<property, virtual> exception class name
			SGF_PROPERTY const std::string& _exceptionName() const override { return "InvalidArgumentException"; }
			//<property, virtual> exception text
			SGF_PROPERTY const std::string& _exceptionText() const override { return "invalid argument."; }
		};

		//�����Ȋ֐��Ăяo���������̍ۂɎg����InvalidFunctionCallException class
		class InvalidFunctionCallException : DefaultException
		{
		public:
			//----------------------------------------------------------------------------------
			//[�R���X�g���N�^]
			//���������s��
			//����1: �G���[���o�����֐��̏ꏊ(��: Namespace::Class)
			//����2: �G���[���o�����֐���
			//����3: �G���[���e
			//����4: �G���[�R�[�h(����ꍇ)
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
