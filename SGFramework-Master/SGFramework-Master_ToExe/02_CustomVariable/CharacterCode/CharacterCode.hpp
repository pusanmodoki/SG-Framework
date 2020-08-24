/*----------------------------------------------------------------------------------
ファイルの文字コード変換定数を列挙するCharacterCode namespace
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_CHARACTER_CODE_HPP_
#define SGFRAMEWORK_HEADER_CHARACTER_CODE_HPP_
#include <codecvt>
#include <fstream>
#include <algorithm>
#include <locale>
#include "../String/String.hpp"
#include "../Property/Property.hpp"

//Framework namespace
namespace SGFramework
{
	//Character Code Constants
	//Use function
	//fstream -> fstream.imbue(std::locale(std::locale::empty(), &change);
	//FileStream -> FileStream.ChangeLocale(change)
	namespace CharacterCode
	{
		extern std::codecvt_utf8<wchar_t> defaultWide;	//UTF8
		extern std::codecvt_utf8<char> defaultChar;		//UTF8
		extern std::codecvt_utf8<wchar_t> toUTF8Wide;	//UTF8
		extern std::codecvt_utf8<char> toUTF8Char;		//UTF8

		//UTF16 little endian
		extern std::codecvt_utf16<wchar_t, 0x10ffff, std::little_endian> toUTF16LEWide;
		//UTF16 big endian
		extern std::codecvt_utf16<wchar_t> toUTF16BEWide;
		//UTF8 In Exclusive UseBOM
		extern std::codecvt_utf8<wchar_t, 0x10ffff, std::consume_header> inUTF8UseBomWide;
		//UTF16 little endian In Exclusive UseBOM
		extern std::codecvt_utf16<wchar_t, 0x10ffff, std::consume_header> inUTF16LEUseBomWide;
		//UTF16 big endian In Exclusive UseBOM
		extern std::codecvt_utf16<wchar_t, 0x10ffff, std::consume_header> inUTF16BEUseBomWide;
		//UTF8 Out Exclusive UseBOM
		extern std::codecvt_utf8<wchar_t, 0x10ffff, std::generate_header> outUTF8UseBomWide;
		//UTF16 little endian Out Exclusive UseBOM
		extern std::codecvt_utf16<wchar_t, 0x10ffff, (std::codecvt_mode)(std::generate_header | std::little_endian)> outUTF16LEUseBomWide;

		//UTF16 little endian
		extern std::codecvt_utf16<char, 0x10ffff, std::little_endian> toUTF16LEChar;
		//UTF16 big endian
		extern std::codecvt_utf16<char> toUTF16BEChar;
		//UTF8 In Exclusive UseBOM
		extern std::codecvt_utf8<char, 0x10ffff, std::consume_header> inUTF8UseBomChar;
		//UTF16 little endian In Exclusive UseBOM
		extern std::codecvt_utf16<char, 0x10ffff, std::consume_header> inUTF16LEUseBomChar;
		//UTF16 big endian In Exclusive UseBOM
		extern std::codecvt_utf16<char, 0x10ffff, std::consume_header> inUTF16BEUseBomChar;
		//UTF8 Out Exclusive UseBOM
		extern std::codecvt_utf8<char, 0x10ffff, std::generate_header> outUTF8UseBomChar;
		//UTF16 little endian Out Exclusive UseBOM
		extern std::codecvt_utf16<char, 0x10ffff, (std::codecvt_mode)(std::generate_header | std::little_endian)> outUTF16LEUseBomChar;
		
		//BOM is UTF8
		constexpr int cBomIsUTF8[3] = { 0xEF, 0xBB, 0xBF };
		//BOM is UTF16 little endian
		constexpr int cBomIsUTF16BE[2] = { 0xFE, 0xFF };
		//BOM is UTF8 big endian
		constexpr int cBomIsUTF16LE[2] = { 0xFF, 0xFE };
	}
}

#endif // !SGFRAMEWORK_HEADER_CHARACTER_CODE_HPP_
