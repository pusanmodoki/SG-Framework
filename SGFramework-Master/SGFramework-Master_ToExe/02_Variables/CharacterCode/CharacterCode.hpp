/*----------------------------------------------------------------------------------
S.Game-Framework  (SG-Framework)
The MIT License (MIT)

Copyright (c) 2020 Shota Uemura

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
------------------------------------------------------------------------------------*/
/**
* @file CharacterCode.hpp
*  @brief ファイルの文字コード変換定数をプロパティで提供するCharacterCode namespace
*/
#ifndef SGFRAMEWORK_HEADER_CHARACTER_CODE_HPP_
#define SGFRAMEWORK_HEADER_CHARACTER_CODE_HPP_
#include <codecvt>
#include <fstream>
#include <algorithm>
#include <locale>

//Framework namespace
namespace SGFramework
{
	//Character Code Constants
	//Use function
	//fstream -> fstream.imbue(std::locale(std::locale::empty(), &change);
	//FileStream -> FileStream.ChangeLocale(change)

	//! @brief ファイルの文字コード変換定数をプロパティで提供するCharacterCode namespace
	//! @par How to use
	//! std::wfstream -> fstream.imbue(std::locale(std::locale::empty(), &change)\n
	//! FileStream -> FileStream.ChangeLocale(change)
	namespace CharacterCode
	{
		//! @brief_property_get CharacterCode used by default(UTF-8N)
		inline const std::codecvt_utf8<wchar_t>& _default() { static std::codecvt_utf8<wchar_t> instance; return instance; }
		//! @brief_property_get CharacterCode UTF-8N
		inline const std::codecvt_utf8<wchar_t>& _utf8() { static std::codecvt_utf8<wchar_t> instance; return instance; }

		namespace Narrow
		{
			//! @brief_property_get CharacterCode used by default(Narrow-UTF8)
			inline const std::codecvt_utf8<char>& _default() { static std::codecvt_utf8<char> instance; return instance; }
			//! @brief_property_get CharacterCode Narrow-UTF8
			inline const std::codecvt_utf8<char>& _utf8() { static std::codecvt_utf8<char> instance; return instance; }
		}

		//! @brief_property_get CharacterCode Wide-UTF16 little endian
		inline const std::codecvt_utf16<wchar_t, 0x10ffff, std::little_endian>& _utf16LeWide() { static std::codecvt_utf16<wchar_t, 0x10ffff, std::little_endian> instance; return instance; }
		//! @brief_property_get CharacterCode Wide-UTF16 big endian
		inline const std::codecvt_utf16<wchar_t>& _utf16BeWide() { static std::codecvt_utf16<wchar_t> instance; return instance; }

		//! @brief_property_get CharacterCode Wide-UTF8 with BOM
		inline const std::codecvt_utf8<wchar_t, 0x10ffff, std::consume_header>& _utf8WithBomWide() { static std::codecvt_utf8<wchar_t, 0x10ffff, std::consume_header> instance; return instance; }
		//! @brief_property_get CharacterCode Wide-UTF16 little endian with BOM
		inline const std::codecvt_utf16<wchar_t, 0x10ffff, std::consume_header>& _utf16LeWithBomWide() { static std::codecvt_utf16<wchar_t, 0x10ffff, std::consume_header> instance; return instance; }


		//UTF8 with BOM
		extern std::codecvt_utf8<wchar_t, 0x10ffff, std::consume_header> g_inUTF8UseBomWide;
		//UTF16 little endian In Exclusive UseBOM
		extern std::codecvt_utf16<wchar_t, 0x10ffff, std::consume_header> g_inUTF16LEUseBomWide;
		//UTF16 big endian In Exclusive UseBOM
		extern std::codecvt_utf16<wchar_t, 0x10ffff, std::consume_header> g_inUTF16BEUseBomWide;
		//UTF8 Out Exclusive UseBOM
		extern std::codecvt_utf8<wchar_t, 0x10ffff, std::generate_header> g_outUTF8UseBomWide;
		//UTF16 little endian Out Exclusive UseBOM
		extern std::codecvt_utf16<wchar_t, 0x10ffff, (std::codecvt_mode)(std::generate_header | std::little_endian)> g_outUTF16LEUseBomWide;

		//UTF16 little endian
		extern std::codecvt_utf16<char, 0x10ffff, std::little_endian> g_toUTF16LEChar;
		//UTF16 big endian
		extern std::codecvt_utf16<char> g_toUTF16BEChar;
		//UTF8 In Exclusive UseBOM
		extern std::codecvt_utf8<char, 0x10ffff, std::consume_header> g_inUTF8UseBomChar;
		//UTF16 little endian In Exclusive UseBOM
		extern std::codecvt_utf16<char, 0x10ffff, std::consume_header> g_inUTF16LEUseBomChar;
		//UTF16 big endian In Exclusive UseBOM
		extern std::codecvt_utf16<char, 0x10ffff, std::consume_header> g_inUTF16BEUseBomChar;
		//UTF8 Out Exclusive UseBOM
		extern std::codecvt_utf8<char, 0x10ffff, std::generate_header> g_outUTF8UseBomChar;
		//UTF16 little endian Out Exclusive UseBOM
		extern std::codecvt_utf16<char, 0x10ffff, (std::codecvt_mode)(std::generate_header | std::little_endian)> g_outUTF16LEUseBomChar;
		
		//BOM is UTF8
		constexpr int g_cBomIsUTF8[3] = { 0xEF, 0xBB, 0xBF };
		//BOM is UTF16 little endian
		constexpr int g_cBomIsUTF16BE[2] = { 0xFE, 0xFF };
		//BOM is UTF8 big endian
		constexpr int g_cBomIsUTF16LE[2] = { 0xFF, 0xFE };
	}
}

#endif // !SGFRAMEWORK_HEADER_CHARACTER_CODE_HPP_
