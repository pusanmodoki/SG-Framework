/*----------------------------------------------------------------------------------
throw�p��InvalidArgument struct
SGFramework��throw�͂��ׂĂ���𓊂���
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_INVALID_ARGUMENT_HPP_
#define SGFRAMEWORK_HEADER_INVALID_ARGUMENT_HPP_
#include <memory>
#include <string>
#include "../Property/Property.hpp"
#include "../String/SGString.hpp"

//Framework namespace
namespace SGFramework
{
	//throw�p��InvalidArgument class
	//SGFramework��throw�͂��ׂĂ���𓊂���
	class InvalidArgument final
	{
	public:
		//delete
		inline InvalidArgument() = delete;
		//default
		inline InvalidArgument(const InvalidArgument& copy) = default;
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//���������s��
		//header->L"Error!! �N���X��->�֐���"
		//text->L"�G���[���e"
		//code->HRESULT��������ꍇ
		//����1: �G���[�w�b�_�[
		//����2: �G���[���e
		//����3: �G���[�R�[�h(����ꍇ)
		inline InvalidArgument(const sgstring& errorHeader, const sgstring& errorText, const long errorCode = 0)
		{
			this->m_errorHeader = errorHeader;
			this->m_errorText = errorText;
			this->m_errorCode = errorCode;
		}

		inline const sgstring& Header() const { return m_errorHeader; }
		inline const sgstring& Text() const { return m_errorText; }
		inline long Code() const { return m_errorCode; }

	private:
		sgstring m_errorHeader;		//�G���[�w�b�_�[
		sgstring m_errorText;			//�G���[���e
		long m_errorCode;											//�G���[�R�[�h(����ꍇ)
	};
}
#endif // !SGFRAMEWORK_HEADER_INVALID_ARGUMENT_HPP_
