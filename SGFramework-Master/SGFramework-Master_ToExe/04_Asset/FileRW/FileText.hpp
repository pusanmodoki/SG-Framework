/*----------------------------------------------------------------------------------
std::vector< std::vector<sgstring>>���g���₷������FileText class
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_FILE_TEXT_HPP_
#define SGFRAMEWORK_HEADER_FILE_TEXT_HPP_
#include <vector>
#include "../../02_CustomVariable/CustomVariable.hpp"

// Framework namespace
namespace SGFramework
{
	//File Edit
	namespace File
	{
		//std::vector< std::vector<sgstring>>���g���₷������FileText class
		class FileText : public std::vector< std::vector<sgstring>>
		{
		public:
			//----------------------------------------------------------------------------------
			//[�R���X�g���N�^]
			//������
			inline FileText();
			//----------------------------------------------------------------------------------
			//[�R���X�g���N�^]
			//�s����lineCounst���ǉ�����
			//����1: �s��
			inline FileText(uint lineCount);
			//----------------------------------------------------------------------------------
			//[�R���X�g���N�^]
			//�s����lineCounst���ǉ�, �񐔂�elementCount���ǉ� (�S�s) ����
			//����1: �s��
			//����2: ��
			inline FileText(uint lineCount, uint elementCount);


			//----------------------------------------------------------------------------------
			//[push_line_back]
			//�����ɍs��ǉ�����
			inline void push_line_back();
			//----------------------------------------------------------------------------------
			//[push_line_back]
			//�����ɍs��ǉ�����
			//����1: �ǉ�����s��push����v�f
			//����2: �ǉ�����s��push����v�f (Pack)
			template <typename SgString1, typename... SgString2>
			inline void push_line_back_element(const SgString1& push, const SgString2&... pushPack);
			//----------------------------------------------------------------------------------
			//[push_line_back_multiple]
			//�����ɍs��lineCount���ǉ�����
			//����1: �s��
			inline void push_line_back_multiple(uint lineCount);
			//----------------------------------------------------------------------------------
			//[push_line_back_multiple]
			//�s����lineCounst���ǉ�, �񐔂�elementCount���ǉ� (�S�s) ����
			//����1: �s��
			//����2: ��
			inline void push_line_back_multiple(uint lineCount, uint elementCount);

			//----------------------------------------------------------------------------------
			//[push_line_back_multiple_element]
			//�s���������ǉ�����
			//����1: �ǉ�����s
			//����2: �ǉ�����s (Pack)
			template <typename FileLine1, typename... FileLine2>
			inline void push_line_back_multiple_element(const FileLine1& push, const FileLine2&... pushPack);
		};






		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//������
		inline FileText::FileText() : std::vector< std::vector<sgstring>>() { }
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�s����lineCounst���ǉ�����
		//����1: �s��
		inline FileText::FileText(uint lineCount)
			: std::vector< std::vector<sgstring>>(lineCount, std::vector<sgstring>()){}
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�s����lineCounst���ǉ�, �񐔂�elementCount���ǉ� (�S�s) ����
		//����1: �s��
		//����2: ��
		inline FileText::FileText(uint lineCount, uint elementCount)
			: std::vector< std::vector<sgstring>>(lineCount, std::vector<sgstring>(elementCount, sgstring())) {}	

		//----------------------------------------------------------------------------------
		//[push_line_back]
		//�����ɍs��ǉ�����
		inline void FileText::push_line_back() { this->emplace_back(std::vector<sgstring>()); }
		//----------------------------------------------------------------------------------
		//[push_line_back]
		//�����ɍs��ǉ�����
		//����1: �ǉ�����s��push����v�f
		//����2: �ǉ�����s��push����v�f (Pack)
		template<typename SgString1, typename ...SgString2>
		inline void FileText::push_line_back_element(const SgString1 & push, const SgString2 & ...pushPack)
		{
			uint index = static_cast<uint>(this->size());
			this->emplace_back(std::vector<sgstring>()); 			
			this->operator[](index).emplace_back(push);
			for (const auto& e : { pushPack... } )
				this->operator[](index).emplace_back(e);
		}

		//----------------------------------------------------------------------------------
		//[push_line_back_multiple]
		//�����ɍs��lineCount���ǉ�����
		//����1: �s��
		inline void FileText::push_line_back_multiple(uint lineCount) 
		{
			this->reserve(this->size() + lineCount);
			for (uint i = 0; i < lineCount; i++) push_line_back(); 
		}
		//----------------------------------------------------------------------------------
		//[push_line_back_multiple]
		//�s����lineCounst���ǉ�, �񐔂�elementCount���ǉ� (�S�s) ����
		//����1: �s��
		//����2: ��
		inline void FileText::push_line_back_multiple(uint lineCount, uint elementCount)
		{
			this->reserve(this->size() + lineCount);
			for (uint i = 0; i < lineCount; i++) 
				this->emplace_back(std::vector<sgstring>(elementCount, sgstring()));
		}
		//----------------------------------------------------------------------------------
		//[push_line_back_multiple_element]
		//�s���������ǉ�����
		//����1: �ǉ�����s
		//����2: �ǉ�����s (Pack)
		template <typename FileLine1, typename... FileLine2>
		inline void FileText::push_line_back_multiple_element(const FileLine1& push, const FileLine2&... pushPack)
		{
			this->emplace_back(push);
			for (const auto& e : { pushPack... } )
				this->emplace_back(e);
		}
	}
}
#endif // !SGFRAMEWORK_HEADER_FILE_TEXT_HPP_
