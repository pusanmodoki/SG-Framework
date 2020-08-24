/*----------------------------------------------------------------------------------
std::wstring���g������sgstring class
SGFramework�̕�����^��sgstring�œ��ꂳ���
------------------------------------------------------------------------------------*/
#include "SGString.hpp"
#include "../InvalidArgument/InvalidArgument.hpp"

//Framework namespace
namespace SGFramework
{
	//----------------------------------------------------------------------------------
	//[ToX16]
	//�����������16�i���\�L�ɕϊ�����
	//throw: �����ɕϊ��ł��Ȃ������ꍇ
	//return: �ϊ�����������
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
	//�����������10�i���\�L�ɕϊ�����
	//throw: �����ɕϊ��ł��Ȃ������ꍇ
	//return: �ϊ�����������
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
	//�����������8�i���\�L�ɕϊ�����
	//throw: �����ɕϊ��ł��Ȃ������ꍇ
	//return: �ϊ�����������
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
	//�����������2�i���\�L�ɕϊ�����
	//throw: �����ɕϊ��ł��Ȃ������ꍇ
	//return: �ϊ�����������
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
	//�������int�ɕϊ�����
	//throw: �ϊ��ł��Ȃ������ꍇ
	//return: �ϊ������l
	int sgstring::MyFunctions::ToInt() const
	{
		try { return std::stoi(m_string); }
		catch (...) { throw InvalidArgument(L"Error!! sgstring->ToInt", L"Invalid string: " + m_string); }
	}
	//----------------------------------------------------------------------------------
	//[ToLong]
	//�������long�ɕϊ�����
	//throw: �ϊ��ł��Ȃ������ꍇ
	//return: �ϊ������l
	long sgstring::MyFunctions::ToLong() const
	{
		try { return std::stol(m_string); }
		catch (...) { throw InvalidArgument(L"Error!! sgstring->ToLong", L"Invalid string: " + m_string); }
	}
	//----------------------------------------------------------------------------------
	//[ToUlong]
	//�������ulong�ɕϊ�����
	//throw: �ϊ��ł��Ȃ������ꍇ
	//return: �ϊ������l
	ulong sgstring::MyFunctions::ToUlong() const
	{
		try { return std::stoul(m_string); }
		catch (...) { throw InvalidArgument(L"Error!! sgstring->ToUlong", L"Invalid string: " + m_string); }
	}
	//----------------------------------------------------------------------------------
	//[ToInt64]
	//�������int64�ɕϊ�����
	//throw: �ϊ��ł��Ȃ������ꍇ
	//return: �ϊ������l
	int64 sgstring::MyFunctions::ToInt64() const
	{
		try { return std::stoll(m_string); }
		catch (...) { throw InvalidArgument(L"Error!! sgstring->ToInt64", L"Invalid string: " + m_string); }
	}
	//----------------------------------------------------------------------------------
	//[ToUint64]
	//�������uint64�ɕϊ�����
	//throw: �ϊ��ł��Ȃ������ꍇ
	//return: �ϊ������l
	uint64 sgstring::MyFunctions::ToUint64() const
	{
		try { return std::stoull(m_string); }
		catch (...) { throw InvalidArgument(L"Error!! sgstring->ToUint64", L"Invalid string: " + m_string); }
	}
	//----------------------------------------------------------------------------------
	//[ToFloat]
	//�������float�ɕϊ�����
	//throw: �ϊ��ł��Ȃ������ꍇ
	//return: �ϊ������l
	float sgstring::MyFunctions::ToFloat() const
	{
		try { return std::stof(m_string); }
		catch (...) { throw InvalidArgument(L"Error!! sgstring->ToFloat", L"Invalid string: " + m_string); }
	}
	//----------------------------------------------------------------------------------
	//[ToDouble]
	//�������double�ɕϊ�����
	//throw: �ϊ��ł��Ȃ������ꍇ
	//return: �ϊ������l
	double sgstring::MyFunctions::ToDouble() const
	{
		try { return std::stod(m_string); }
		catch (...) { throw InvalidArgument(L"Error!! sgstring->ToDouble", L"Invalid string: " + m_string); }
	}



	//----------------------------------------------------------------------------------
	//[ToX16]
	//�����������16�i���\�L�ɕϊ�����
	//throw: �����ɕϊ��ł��Ȃ������ꍇ
	//return: �ϊ�����������
	//����1: �Ώە�����
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
	//�����������10�i���\�L�ɕϊ�����
	//throw: �����ɕϊ��ł��Ȃ������ꍇ
	//return: �ϊ�����������
	//����1: �Ώە�����
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
	//�����������8�i���\�L�ɕϊ�����
	//throw: �����ɕϊ��ł��Ȃ������ꍇ
	//return: �ϊ�����������
	//����1: �Ώە�����
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
	//�����������2�i���\�L�ɕϊ�����
	//throw: �����ɕϊ��ł��Ȃ������ꍇ
	//return: �ϊ�����������
	//����1: �Ώە�����
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
	//�������int�ɕϊ�����
	//throw: �ϊ��ł��Ȃ������ꍇ
	//return: �ϊ������l
	//����1: �Ώە�����
	int sgstring::functions::ToInt(const sgstring& string)
	{
		try { return std::stoi(string); }
		catch (...) { throw InvalidArgument(L"Error!! sgstring->ToInt", L"Invalid string: " + string); }
	}
	//----------------------------------------------------------------------------------
	//[ToLong]
	//�������long�ɕϊ�����
	//throw: �ϊ��ł��Ȃ������ꍇ
	//return: �ϊ������l
	//����1: �Ώە�����
	long sgstring::functions::ToLong(const sgstring& string)
	{
		try { return std::stol(string); }
		catch (...) { throw InvalidArgument(L"Error!! sgstring->ToLong", L"Invalid string: " + string); }
	}
	//----------------------------------------------------------------------------------
	//[ToUlong]
	//�������ulong�ɕϊ�����
	//throw: �ϊ��ł��Ȃ������ꍇ
	//return: �ϊ������l
	//����1: �Ώە�����
	ulong sgstring::functions::ToUlong(const sgstring& string)
	{
		try { return std::stoul(string); }
		catch (...) { throw InvalidArgument(L"Error!! sgstring->ToUlong", L"Invalid string: " + string); }
	}
	//----------------------------------------------------------------------------------
	//[ToInt64]
	//�������int64�ɕϊ�����
	//throw: �ϊ��ł��Ȃ������ꍇ
	//return: �ϊ������l
	//����1: �Ώە�����
	int64 sgstring::functions::ToInt64(const sgstring& string)
	{
		try { return std::stoll(string); }
		catch (...) { throw InvalidArgument(L"Error!! sgstring->ToInt64", L"Invalid string: " + string); }
	}
	//----------------------------------------------------------------------------------
	//[ToUint64]
	//�������uint64�ɕϊ�����
	//throw: �ϊ��ł��Ȃ������ꍇ
	//return: �ϊ������l
	//����1: �Ώە�����
	uint64 sgstring::functions::ToUint64(const sgstring& string)
	{
		try { return std::stoull(string); }
		catch (...) { throw InvalidArgument(L"Error!! sgstring->ToUint64", L"Invalid string: " + string); }
	}
	//----------------------------------------------------------------------------------
	//[ToFloat]
	//�������float�ɕϊ�����
	//throw: �ϊ��ł��Ȃ������ꍇ
	//return: �ϊ������l
	//����1: �Ώە�����
	float sgstring::functions::ToFloat(const sgstring& string)
	{
		try { return std::stof(string); }
		catch (...) { throw InvalidArgument(L"Error!! sgstring->ToFloat", L"Invalid string: " + string); }
	}
	//----------------------------------------------------------------------------------
	//[ToDouble]
	//�������double�ɕϊ�����
	//throw: �ϊ��ł��Ȃ������ꍇ
	//return: �ϊ������l
	//����1: �Ώە�����
	double sgstring::functions::ToDouble(const sgstring& string)
	{
		try { return std::stod(string); }
		catch (...) { throw InvalidArgument(L"Error!! sgstring->ToDouble", L"Invalid string: " + string); }
	}

}