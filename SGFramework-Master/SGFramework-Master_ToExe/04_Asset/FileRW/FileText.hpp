/*----------------------------------------------------------------------------------
std::vector< std::vector<sgstring>>を使いやすくするFileText class
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
		//std::vector< std::vector<sgstring>>を使いやすくするFileText class
		class FileText : public std::vector< std::vector<sgstring>>
		{
		public:
			//----------------------------------------------------------------------------------
			//[コンストラクタ]
			//初期化
			inline FileText();
			//----------------------------------------------------------------------------------
			//[コンストラクタ]
			//行数をlineCounst数追加する
			//引数1: 行数
			inline FileText(uint lineCount);
			//----------------------------------------------------------------------------------
			//[コンストラクタ]
			//行数をlineCounst数追加, 列数をelementCount数追加 (全行) する
			//引数1: 行数
			//引数2: 列数
			inline FileText(uint lineCount, uint elementCount);


			//----------------------------------------------------------------------------------
			//[push_line_back]
			//末尾に行を追加する
			inline void push_line_back();
			//----------------------------------------------------------------------------------
			//[push_line_back]
			//末尾に行を追加する
			//引数1: 追加する行にpushする要素
			//引数2: 追加する行にpushする要素 (Pack)
			template <typename SgString1, typename... SgString2>
			inline void push_line_back_element(const SgString1& push, const SgString2&... pushPack);
			//----------------------------------------------------------------------------------
			//[push_line_back_multiple]
			//末尾に行をlineCount数追加する
			//引数1: 行数
			inline void push_line_back_multiple(uint lineCount);
			//----------------------------------------------------------------------------------
			//[push_line_back_multiple]
			//行数をlineCounst数追加, 列数をelementCount数追加 (全行) する
			//引数1: 行数
			//引数2: 列数
			inline void push_line_back_multiple(uint lineCount, uint elementCount);

			//----------------------------------------------------------------------------------
			//[push_line_back_multiple_element]
			//行を引数分追加する
			//引数1: 追加する行
			//引数2: 追加する行 (Pack)
			template <typename FileLine1, typename... FileLine2>
			inline void push_line_back_multiple_element(const FileLine1& push, const FileLine2&... pushPack);
		};






		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//初期化
		inline FileText::FileText() : std::vector< std::vector<sgstring>>() { }
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//行数をlineCounst数追加する
		//引数1: 行数
		inline FileText::FileText(uint lineCount)
			: std::vector< std::vector<sgstring>>(lineCount, std::vector<sgstring>()){}
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//行数をlineCounst数追加, 列数をelementCount数追加 (全行) する
		//引数1: 行数
		//引数2: 列数
		inline FileText::FileText(uint lineCount, uint elementCount)
			: std::vector< std::vector<sgstring>>(lineCount, std::vector<sgstring>(elementCount, sgstring())) {}	

		//----------------------------------------------------------------------------------
		//[push_line_back]
		//末尾に行を追加する
		inline void FileText::push_line_back() { this->emplace_back(std::vector<sgstring>()); }
		//----------------------------------------------------------------------------------
		//[push_line_back]
		//末尾に行を追加する
		//引数1: 追加する行にpushする要素
		//引数2: 追加する行にpushする要素 (Pack)
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
		//末尾に行をlineCount数追加する
		//引数1: 行数
		inline void FileText::push_line_back_multiple(uint lineCount) 
		{
			this->reserve(this->size() + lineCount);
			for (uint i = 0; i < lineCount; i++) push_line_back(); 
		}
		//----------------------------------------------------------------------------------
		//[push_line_back_multiple]
		//行数をlineCounst数追加, 列数をelementCount数追加 (全行) する
		//引数1: 行数
		//引数2: 列数
		inline void FileText::push_line_back_multiple(uint lineCount, uint elementCount)
		{
			this->reserve(this->size() + lineCount);
			for (uint i = 0; i < lineCount; i++) 
				this->emplace_back(std::vector<sgstring>(elementCount, sgstring()));
		}
		//----------------------------------------------------------------------------------
		//[push_line_back_multiple_element]
		//行を引数分追加する
		//引数1: 追加する行
		//引数2: 追加する行 (Pack)
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
