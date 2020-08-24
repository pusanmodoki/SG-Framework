/*----------------------------------------------------------------------------------
std::wstringを拡張したsgstring class
SGFrameworkの文字列型はsgstringで統一される
------------------------------------------------------------------------------------*/
#include "SGString.hpp"
#include "../InvalidArgument/InvalidArgument.hpp"

//Framework namespace
namespace SGFramework
{
	//----------------------------------------------------------------------------------
	//[ToX16]
	//数字文字列を16進数表記に変換する
	//throw: 数字に変換できなかった場合
	//return: 変換した文字列
	sgstring sgstring::MyFunctions::ToX16() const
	{
		int x16 = 0;
		try { x16 = std::stoi(m_string); }
		catch (...)  { throw InvalidArgument(L"Error!! sgstring->ToX16", L"Invalid string: " + m_string); }
		std::wstringstream stream;
		stream << std::showbase;
		stream << std::hex << x16;
		return stream.str();
	}
	//----------------------------------------------------------------------------------
	//[ToX10]
	//数字文字列を10進数表記に変換する
	//throw: 数字に変換できなかった場合
	//return: 変換した文字列
	sgstring sgstring::MyFunctions::ToX10() const
	{
		int x10 = 0;
		try { x10 = std::stoi(m_string); }
		catch (...) { throw InvalidArgument(L"Error!! sgstring->ToX10", L"Invalid string: " + m_string); }
		std::wstringstream stream;
		stream << std::showbase;
		stream << std::dec << x10;
		return stream.str();
	}
	//----------------------------------------------------------------------------------
	//[ToX8]
	//数字文字列を8進数表記に変換する
	//throw: 数字に変換できなかった場合
	//return: 変換した文字列
	sgstring sgstring::MyFunctions::ToX8() const
	{
		int x8 = 0;
		try { x8 = std::stoi(m_string); }
		catch (...) { throw InvalidArgument(L"Error!! sgstring->ToX8", L"Invalid string: " + m_string); }
		std::wstringstream stream;
		stream << std::showbase;
		stream << std::oct << x8;
		return stream.str();
	}
	//----------------------------------------------------------------------------------
	//[ToX2]
	//数字文字列を2進数表記に変換する
	//throw: 数字に変換できなかった場合
	//return: 変換した文字列
	sgstring sgstring::MyFunctions::ToX2() const
	{
		int x2 = 0;
		try { x2 = std::stoi(m_string); }
		catch (...) { throw InvalidArgument(L"Error!! sgstring->ToX2", L"Invalid string: " + m_string); }
		std::wstringstream stream;
		stream << std::showbase;
		stream << std::bitset<32>(x2);
		return stream.str();
	}

	//----------------------------------------------------------------------------------
	//[ToInt]
	//文字列をintに変換する
	//throw: 変換できなかった場合
	//return: 変換した値
	int sgstring::MyFunctions::ToInt() const
	{
		try { return std::stoi(m_string); }
		catch (...) { throw InvalidArgument(L"Error!! sgstring->ToInt", L"Invalid string: " + m_string); }
	}
	//----------------------------------------------------------------------------------
	//[ToLong]
	//文字列をlongに変換する
	//throw: 変換できなかった場合
	//return: 変換した値
	long sgstring::MyFunctions::ToLong() const
	{
		try { return std::stol(m_string); }
		catch (...) { throw InvalidArgument(L"Error!! sgstring->ToLong", L"Invalid string: " + m_string); }
	}
	//----------------------------------------------------------------------------------
	//[ToUlong]
	//文字列をulongに変換する
	//throw: 変換できなかった場合
	//return: 変換した値
	ulong sgstring::MyFunctions::ToUlong() const
	{
		try { return std::stoul(m_string); }
		catch (...) { throw InvalidArgument(L"Error!! sgstring->ToUlong", L"Invalid string: " + m_string); }
	}
	//----------------------------------------------------------------------------------
	//[ToInt64]
	//文字列をint64に変換する
	//throw: 変換できなかった場合
	//return: 変換した値
	int64 sgstring::MyFunctions::ToInt64() const
	{
		try { return std::stoll(m_string); }
		catch (...) { throw InvalidArgument(L"Error!! sgstring->ToInt64", L"Invalid string: " + m_string); }
	}
	//----------------------------------------------------------------------------------
	//[ToUint64]
	//文字列をuint64に変換する
	//throw: 変換できなかった場合
	//return: 変換した値
	uint64 sgstring::MyFunctions::ToUint64() const
	{
		try { return std::stoull(m_string); }
		catch (...) { throw InvalidArgument(L"Error!! sgstring->ToUint64", L"Invalid string: " + m_string); }
	}
	//----------------------------------------------------------------------------------
	//[ToFloat]
	//文字列をfloatに変換する
	//throw: 変換できなかった場合
	//return: 変換した値
	float sgstring::MyFunctions::ToFloat() const
	{
		try { return std::stof(m_string); }
		catch (...) { throw InvalidArgument(L"Error!! sgstring->ToFloat", L"Invalid string: " + m_string); }
	}
	//----------------------------------------------------------------------------------
	//[ToDouble]
	//文字列をdoubleに変換する
	//throw: 変換できなかった場合
	//return: 変換した値
	double sgstring::MyFunctions::ToDouble() const
	{
		try { return std::stod(m_string); }
		catch (...) { throw InvalidArgument(L"Error!! sgstring->ToDouble", L"Invalid string: " + m_string); }
	}



	//----------------------------------------------------------------------------------
	//[ToX16]
	//数字文字列を16進数表記に変換する
	//throw: 数字に変換できなかった場合
	//return: 変換した文字列
	//引数1: 対象文字列
	sgstring sgstring::functions::ToX16(const sgstring& string)
	{
		int x16 = 0;
		try { x16 = std::stoi(string); }
		catch (...) { throw InvalidArgument(L"Error!! sgstring->ToX16", L"Invalid string: " + string); }
		std::wstringstream stream;
		stream << std::showbase;
		stream << std::hex << x16;
		return stream.str();
	}
	//----------------------------------------------------------------------------------
	//[ToX10]
	//数字文字列を10進数表記に変換する
	//throw: 数字に変換できなかった場合
	//return: 変換した文字列
	//引数1: 対象文字列
	sgstring sgstring::functions::ToX10(const sgstring& string)
	{
		int x10 = 0;
		try { x10 = std::stoi(string); }
		catch (...) { throw InvalidArgument(L"Error!! sgstring->ToX10", L"Invalid string: " + string); }
		std::wstringstream stream;
		stream << std::showbase;
		stream << std::dec << x10;
		return stream.str();
	}
	//----------------------------------------------------------------------------------
	//[ToX8]
	//数字文字列を8進数表記に変換する
	//throw: 数字に変換できなかった場合
	//return: 変換した文字列
	//引数1: 対象文字列
	sgstring sgstring::functions::ToX8(const sgstring& string)
	{
		int x8 = 0;
		try { x8 = std::stoi(string); }
		catch (...) { throw InvalidArgument(L"Error!! sgstring->ToX8", L"Invalid string: " + string); }
		std::wstringstream stream;
		stream << std::showbase;
		stream << std::oct << x8;
		return stream.str();
	}
	//----------------------------------------------------------------------------------
	//[ToX2]
	//数字文字列を2進数表記に変換する
	//throw: 数字に変換できなかった場合
	//return: 変換した文字列
	//引数1: 対象文字列
	sgstring sgstring::functions::ToX2(const sgstring& string)
	{
		int x2 = 0;
		try { x2 = std::stoi(string); }
		catch (...) { throw InvalidArgument(L"Error!! sgstring->ToX2", L"Invalid string: " + string); }
		std::wstringstream stream;
		stream << std::showbase;
		stream << std::bitset<32>(x2);
		return stream.str();
	}

	//----------------------------------------------------------------------------------
	//[ToInt]
	//文字列をintに変換する
	//throw: 変換できなかった場合
	//return: 変換した値
	//引数1: 対象文字列
	int sgstring::functions::ToInt(const sgstring& string)
	{
		try { return std::stoi(string); }
		catch (...) { throw InvalidArgument(L"Error!! sgstring->ToInt", L"Invalid string: " + string); }
	}
	//----------------------------------------------------------------------------------
	//[ToLong]
	//文字列をlongに変換する
	//throw: 変換できなかった場合
	//return: 変換した値
	//引数1: 対象文字列
	long sgstring::functions::ToLong(const sgstring& string)
	{
		try { return std::stol(string); }
		catch (...) { throw InvalidArgument(L"Error!! sgstring->ToLong", L"Invalid string: " + string); }
	}
	//----------------------------------------------------------------------------------
	//[ToUlong]
	//文字列をulongに変換する
	//throw: 変換できなかった場合
	//return: 変換した値
	//引数1: 対象文字列
	ulong sgstring::functions::ToUlong(const sgstring& string)
	{
		try { return std::stoul(string); }
		catch (...) { throw InvalidArgument(L"Error!! sgstring->ToUlong", L"Invalid string: " + string); }
	}
	//----------------------------------------------------------------------------------
	//[ToInt64]
	//文字列をint64に変換する
	//throw: 変換できなかった場合
	//return: 変換した値
	//引数1: 対象文字列
	int64 sgstring::functions::ToInt64(const sgstring& string)
	{
		try { return std::stoll(string); }
		catch (...) { throw InvalidArgument(L"Error!! sgstring->ToInt64", L"Invalid string: " + string); }
	}
	//----------------------------------------------------------------------------------
	//[ToUint64]
	//文字列をuint64に変換する
	//throw: 変換できなかった場合
	//return: 変換した値
	//引数1: 対象文字列
	uint64 sgstring::functions::ToUint64(const sgstring& string)
	{
		try { return std::stoull(string); }
		catch (...) { throw InvalidArgument(L"Error!! sgstring->ToUint64", L"Invalid string: " + string); }
	}
	//----------------------------------------------------------------------------------
	//[ToFloat]
	//文字列をfloatに変換する
	//throw: 変換できなかった場合
	//return: 変換した値
	//引数1: 対象文字列
	float sgstring::functions::ToFloat(const sgstring& string)
	{
		try { return std::stof(string); }
		catch (...) { throw InvalidArgument(L"Error!! sgstring->ToFloat", L"Invalid string: " + string); }
	}
	//----------------------------------------------------------------------------------
	//[ToDouble]
	//文字列をdoubleに変換する
	//throw: 変換できなかった場合
	//return: 変換した値
	//引数1: 対象文字列
	double sgstring::functions::ToDouble(const sgstring& string)
	{
		try { return std::stod(string); }
		catch (...) { throw InvalidArgument(L"Error!! sgstring->ToDouble", L"Invalid string: " + string); }
	}

}