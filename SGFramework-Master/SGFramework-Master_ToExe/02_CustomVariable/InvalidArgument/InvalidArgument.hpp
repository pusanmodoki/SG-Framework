/*----------------------------------------------------------------------------------
throw用のInvalidArgument struct
SGFrameworkのthrowはすべてこれを投げる
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
	//throw用のInvalidArgument class
	//SGFrameworkのthrowはすべてこれを投げる
	class InvalidArgument final
	{
	public:
		//delete
		inline InvalidArgument() = delete;
		//default
		inline InvalidArgument(const InvalidArgument& copy) = default;
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//初期化を行う
		//header->L"Error!! クラス名->関数名"
		//text->L"エラー内容"
		//code->HRESULT等がある場合
		//引数1: エラーヘッダー
		//引数2: エラー内容
		//引数3: エラーコード(ある場合)
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
		sgstring m_errorHeader;		//エラーヘッダー
		sgstring m_errorText;			//エラー内容
		long m_errorCode;											//エラーコード(ある場合)
	};
}
#endif // !SGFRAMEWORK_HEADER_INVALID_ARGUMENT_HPP_
