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
#include <array>

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
		//! @attention newされたポインタが返されます, std::localeのコンストラクタで使用する場合std::localeが自動でdeleteします
		SGF_PROPERTY const std::codecvt_utf8<wchar_t>* _default() { return new  std::codecvt_utf8<wchar_t>; }
		//! @brief_property_get CharacterCode UTF-8N
		//! @attention newされたポインタが返されます, std::localeのコンストラクタで使用する場合std::localeが自動でdeleteします
		SGF_PROPERTY const std::codecvt_utf8<wchar_t>* _utf8() { return new  std::codecvt_utf8<wchar_t>; }

		//! @brief_property_get CharacterCode UTF16 little endian
		//! @attention newされたポインタが返されます, std::localeのコンストラクタで使用する場合std::localeが自動でdeleteします
		SGF_PROPERTY const std::codecvt_utf16<wchar_t, 0x10ffff, std::little_endian>* _utf16Le() { return new  std::codecvt_utf16<wchar_t, 0x10ffff, std::little_endian>; }
		//! @brief_property_get CharacterCode UTF16 big endian
		//! @attention newされたポインタが返されます, std::localeのコンストラクタで使用する場合std::localeが自動でdeleteします
		SGF_PROPERTY const std::codecvt_utf16<wchar_t>* _utf16Be() { return new  std::codecvt_utf16<wchar_t>; }

		//! @brief_property_get CharacterCode UTF-8(BOM)
		//! @attention newされたポインタが返されます, std::localeのコンストラクタで使用する場合std::localeが自動でdeleteします
		SGF_PROPERTY const std::codecvt_utf8<wchar_t, 0x10ffff, std::consume_header>* _utf8WithBom() { return new  std::codecvt_utf8<wchar_t, 0x10ffff, std::consume_header>; }
		//! @brief_property_get CharacterCode UTF16 little endian(BOM)
		//! @attention newされたポインタが返されます, std::localeのコンストラクタで使用する場合std::localeが自動でdeleteします
		SGF_PROPERTY const std::codecvt_utf16<wchar_t, 0x10ffff, std::consume_header>* _utf16LeWithBom() { return new  std::codecvt_utf16<wchar_t, 0x10ffff, std::consume_header>; }
		//! @brief_property_get CharacterCode UTF16 big endian(BOM)
		//! @attention newされたポインタが返されます, std::localeのコンストラクタで使用する場合std::localeが自動でdeleteします
		SGF_PROPERTY const std::codecvt_utf16<wchar_t, 0x10ffff, std::consume_header>* _utf16BeWithBom() { return new  std::codecvt_utf16<wchar_t, 0x10ffff, std::consume_header>; }

		//! @brief_property_get UTF-8 BOM identifier
		SGF_PROPERTY const std::array<int, 3>& _utf8BomIdentifier() { static std::array<int, 3> instance = { 0xEF, 0xBB, 0xBF }; return instance; }
		//! @brief_property_get UTF-16 little endian BOM identifier
		SGF_PROPERTY const std::array<int, 2>& _utf16LeBomIdentifier() { static std::array<int, 2> instance = { 0xFF, 0xFE }; return instance; }
		//! @brief_property_get UTF-16 big endian BOM identifier
		SGF_PROPERTY const std::array<int, 2>& _utf16BeBomIdentifier() { static std::array<int, 2> instance = { 0xFE, 0xFF }; return instance; }
	}
}

#endif // !SGFRAMEWORK_HEADER_CHARACTER_CODE_HPP_
