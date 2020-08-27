/*----------------------------------------------------------------------------------
ファイルの文字コード変換定数を列挙するCharacterCode namespace
------------------------------------------------------------------------------------*/
#include "CharacterCode.hpp"

//Framework namespace
namespace SGFramework
{
	//Character Code Constants
	//Use function
	//fstream -> fstream.imbue(std::locale(std::locale::empty(), &change);
	//FileStream -> FileStream.ChangeLocale(change)
	namespace CharacterCode
	{
		std::codecvt_utf8<wchar_t> g_defaultWide;	//UTF8
		std::codecvt_utf8<char> g_defaultChar;			//UTF8
		std::codecvt_utf8<wchar_t> g_toUTF8Wide;	//UTF8
		std::codecvt_utf8<char> g_toUTF8Char;			//UTF8

		//UTF16 little endian
		std::codecvt_utf16<wchar_t, 0x10ffff, std::little_endian> g_toUTF16LEWide;
		//UTF16 big endian
		std::codecvt_utf16<wchar_t> g_toUTF16BEWide;
		//UTF8 In Exclusive UseBOM
		std::codecvt_utf8<wchar_t, 0x10ffff, std::consume_header> g_inUTF8UseBomWide;
		//UTF16 little endian In Exclusive UseBOM
		std::codecvt_utf16<wchar_t, 0x10ffff, std::consume_header> g_inUTF16LEUseBomWide;
		//UTF16 big endian In Exclusive UseBOM
		std::codecvt_utf16<wchar_t, 0x10ffff, std::consume_header> g_inUTF16BEUseBomWide;
		//UTF8 Out Exclusive UseBOM
		std::codecvt_utf8<wchar_t, 0x10ffff, std::generate_header> g_outUTF8UseBomWide;
		//UTF16 little endian Out Exclusive UseBOM
		std::codecvt_utf16<wchar_t, 0x10ffff, (std::codecvt_mode)(std::generate_header | std::little_endian)> g_outUTF16LEUseBomWide;

		//UTF16 little endian
		std::codecvt_utf16<char, 0x10ffff, std::little_endian> g_toUTF16LEChar;
		//UTF16 big endian
		std::codecvt_utf16<char> g_toUTF16BEChar;
		//UTF8 In Exclusive UseBOM
		std::codecvt_utf8<char, 0x10ffff, std::consume_header> g_inUTF8UseBomChar;
		//UTF16 little endian In Exclusive UseBOM
		std::codecvt_utf16<char, 0x10ffff, std::consume_header> g_inUTF16LEUseBomChar;
		//UTF16 big endian In Exclusive UseBOM
		std::codecvt_utf16<char, 0x10ffff, std::consume_header> g_inUTF16BEUseBomChar;
		//UTF8 Out Exclusive UseBOM
		std::codecvt_utf8<char, 0x10ffff, std::generate_header> g_outUTF8UseBomChar;
		//UTF16 little endian Out Exclusive UseBOM
		std::codecvt_utf16<char, 0x10ffff, (std::codecvt_mode)(std::generate_header | std::little_endian)> g_outUTF16LEUseBomChar;
	}
}