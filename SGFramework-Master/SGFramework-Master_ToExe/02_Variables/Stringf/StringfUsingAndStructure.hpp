/*----------------------------------------------------------------------------------
string_wを操作するstringf namespace
→using, 構造体定義をするStringfUsingAndStructure.hpp
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_STRINGF_USING_AND_STRUCTURE_HPP_
#define SGFRAMEWORK_HEADER_STRINGF_USING_AND_STRUCTURE_HPP_
#include <string>

//Framework namespace
namespace SGFramework
{
	//string ascii
	using string_a = std::string;
	//SG-Framework default string, string wide
	using string_w = std::wstring;

	//std::wstring, std::stringを操作するstringf namespace
	//stdクラスとの連携を前提としたもののため特例で命名規則をsnake_caseにする
	namespace stringf
	{
		//find系で見つからなかった場合のinvalid_pos
		constexpr std::size_t c_invalid_pos = std::wstring::npos;

		//基数指定に使うradix structure
		struct radix
		{
			//enum
			enum enum_radix
			{
				x10 = 0x1,
				x2 = 0x2,
				x8 = 0x4,
				x16 = 0x8,
			};
		};
	}
}
#endif // !SGFRAMEWORK_HEADER_STRINGF_USING_AND_STRUCTURE_HPP_