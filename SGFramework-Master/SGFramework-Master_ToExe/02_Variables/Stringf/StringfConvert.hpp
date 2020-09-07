/*----------------------------------------------------------------------------------
string_wを操作するstringf namespace
→convert関数を定義するStringfTo.hpp
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_STRINGF_CONVERT_HPP_
#define SGFRAMEWORK_HEADER_STRINGF_CONVERT_HPP_
#include <string>
#include <bitset>
#include <type_traits>
#include "StringfUsingAndStructure.hpp"
#include "../MacroAndUsing/MacroAndUsing.hpp"
#include "../Exception/Exception.hpp"

//Windows Version
#if defined(SGF_PLATFORM_WINDOWS)
#include <Windows.h>
//else
#else
#error "Unsupported platform."
#endif//endif

//Framework namespace
namespace SGFramework
{
	//std::wstring, string_aを操作するstringf namespace
	//stdクラスとの連携を前提としたもののため特例で命名規則をsnake_caseにする
	namespace stringf
	{
		//----------------------------------------------------------------------------------
		//convert functions

		//----------------------------------------------------------------------------------
		//[convert]
		//template: source type (supported templates: string_a<override>, const char*<override>, char*<override>, float, double, bool, integer types)
		//return: sourceを変換したstring
		//argument 1: convert source
		template<typename T>
		inline string_w convert(T source)
		{
			static_assert(std::is_integral_v<T>, "Unsupported template. supported templates: string_a<override>, const char*<override>, char*<override>, float, double, bool, integer types.");
			//char<n>_tなど普段使わない型はこちらで実装する, 定義未実装が起きた際はこれで防ぎたい…
			return IS_FALSE(std::is_unsigned_v<T>) ? std::to_wstring(static_cast<int64>(source)) : std::to_wstring(static_cast<uint64>(source));
		}
		//----------------------------------------------------------------------------------
		//[convert]
		//template: source type (supported templates: string_a, char*, const char*, float, double, bool, integer types)
		//return: sourceを変換したstring
		//argument 1: convert source
		inline string_w convert(const string_a& source) noexcept
		{
			//Windows Version
#if defined(SGF_PLATFORM_WINDOWS)
			int inLength = static_cast<int>(source.length());
			int outLength = MultiByteToWideChar(CP_UTF8, 0, source.c_str(), inLength, nullptr, 0);
			std::wstring result(outLength, L'\0');
			if (outLength) MultiByteToWideChar(CP_UTF8, 0, source.c_str(), inLength, &result[0], outLength);
			return std::move(result);
			//else
#else
#error "Unsupported platform."
#endif//endif
		}
		//----------------------------------------------------------------------------------
		//[convert]
		//template: source type (supported templates: string_a<override>, const char*<override>, char*<override>, float, double, bool, integer types)
		//return: sourceを変換したstring
		//argument 1: convert source
		inline string_w convert(const char* source) noexcept
		{
			//Windows Version
#if defined(SGF_PLATFORM_WINDOWS)
			int inLength = static_cast<int>(std::strlen(source));
			int outLength = MultiByteToWideChar(CP_UTF8, 0, source, inLength, nullptr, 0);
			std::wstring result(outLength, L'\0');
			if (outLength) MultiByteToWideChar(CP_UTF8, 0, source, inLength, &result[0], outLength);
			return std::move(result);
			//else
#else
#error "Unsupported platform."
#endif//endif
		}
		//----------------------------------------------------------------------------------
		//[convert]
		//template: source type (supported templates: string_a<override>, const char*<override>, char*<override>, float, double, bool, integer types)
		//return: sourceを変換したstring
		//argument 1: convert source
		template<>
		inline string_w convert<char*>(char* source) noexcept
		{
			//Windows Version
#if defined(SGF_PLATFORM_WINDOWS)
			int inLength = static_cast<int>(std::strlen(source));
			int outLength = MultiByteToWideChar(CP_UTF8, 0, source, inLength, nullptr, 0);
			std::wstring result(outLength, L'\0');
			if (outLength) MultiByteToWideChar(CP_UTF8, 0, source, inLength, &result[0], outLength);
			return std::move(result);
			//else
#else
#error "Unsupported platform."
#endif//endif
		}
		//----------------------------------------------------------------------------------
		//[convert]
		//template: source type (supported templates: string_a<override>, const char*<override>, char*<override>, float, double, bool, integer types)
		//return: sourceを変換したstring
		//argument 1: convert source
		template<>
		inline string_w convert<float>(float source) { return std::to_wstring(source); }
		//----------------------------------------------------------------------------------
		//[convert]
		//template: source type (supported templates: string_a<override>, const char*<override>, char*<override>, float, double, bool, integer types)
		//return: sourceを変換したstring
		//argument 1: convert source
		template<>
		inline string_w convert<double>(double source) { return std::to_wstring(source); }
		//----------------------------------------------------------------------------------
		//[convert]
		//template: source type (supported templates: string_a<override>, const char*<override>, char*<override>, float, double, bool, integer types)
		//return: sourceを変換したstring
		//argument 1: convert source
		template<>
		inline string_w convert<bool>(bool source) { return IS_TRUE(source) ? L"true" : L"false"; }
		//----------------------------------------------------------------------------------
		//[convert]
		//template: source type (supported templates: string_a<override>, const char*<override>, char*<override>, float, double, bool, integer types)
		//return: sourceを変換したstring
		//argument 1: convert source
		template<>
		inline string_w convert<char>(char source) { return std::to_wstring(static_cast<int>(source)); }
		//----------------------------------------------------------------------------------
		//[convert]
		//template: source type (supported templates: string_a<override>, const char*<override>, char*<override>, float, double, bool, integer types)
		//return: sourceを変換したstring
		//argument 1: convert source
		template<>
		inline string_w convert<uchar>(uchar source) { return std::to_wstring(static_cast<uint>(source)); }
		//----------------------------------------------------------------------------------
		//[convert]
		//template: source type (supported templates: string_a<override>, const char*<override>, char*<override>, float, double, bool, integer types)
		//return: sourceを変換したstring
		//argument 1: convert source
		template<>
		inline string_w convert<short>(short source) { return std::to_wstring(static_cast<int>(source)); }
		//----------------------------------------------------------------------------------
		//[convert]
		//template: source type (supported templates: string_a<override>, const char*<override>, char*<override>, float, double, bool, integer types)
		//return: sourceを変換したstring
		//argument 1: convert source
		template<>
		inline string_w convert<ushort>(ushort source) { return std::to_wstring(static_cast<uint>(source)); }
		//----------------------------------------------------------------------------------
		//[convert]
		//template: source type (supported templates: string_a<override>, const char*<override>, char*<override>, float, double, bool, integer types)
		//return: sourceを変換したstring
		//argument 1: convert source
		template<>
		inline string_w convert<wchar>(wchar source) { return std::to_wstring(static_cast<uint>(source)); }
		//----------------------------------------------------------------------------------
		//[convert]
		//template: source type (supported templates: string_a<override>, const char*<override>, char*<override>, float, double, bool, integer types)
		//return: sourceを変換したstring
		//argument 1: convert source
		template<>
		inline string_w convert<int>(int source) { return std::to_wstring(source); }
		//----------------------------------------------------------------------------------
		//[convert]
		//template: source type (supported templates: string_a<override>, const char*<override>, char*<override>, float, double, bool, integer types)
		//return: sourceを変換したstring
		//argument 1: convert source
		template<>
		inline string_w convert<uint>(uint source) { return std::to_wstring(source); }
		//----------------------------------------------------------------------------------
		//[convert]
		//template: source type (supported templates: string_a<override>, const char*<override>, char*<override>, float, double, bool, integer types)
		//return: sourceを変換したstring
		//argument 1: convert source
		template<>
		inline string_w convert<long>(long source) { return std::to_wstring(source); }
		//----------------------------------------------------------------------------------
		//[convert]
		//template: source type (supported templates: string_a<override>, const char*<override>, char*<override>, float, double, bool, integer types)
		//return: sourceを変換したstring
		//argument 1: convert source
		template<>
		inline string_w convert<ulong>(ulong source) { return std::to_wstring(source); }
		//----------------------------------------------------------------------------------
		//[convert]
		//template: source type (supported templates: string_a<override>, const char*<override>, char*<override>, float, double, bool, integer types)
		//return: sourceを変換したstring
		//argument 1: convert source
		template<>
		inline string_w convert<int64>(int64 source) { return std::to_wstring(source); }
		//----------------------------------------------------------------------------------
		//[convert]
		//template: source type (supported templates: string_a<override>, const char*<override>, char*<override>, float, double, bool, integer types)
		//return: sourceを変換したstring
		//argument 1: convert source
		template<>
		inline string_w convert<uint64>(uint64 source) { return std::to_wstring(source); }

		//----------------------------------------------------------------------------------
		//[to_pointer]
		//template: source type (supported templates: pointer types)
		//return: ポインタを16進数表現に変換したstring
		//argument 1: 基数表現に用いる基数
		template<typename T>
		inline string_w to_pointer(T source)
		{
			//ポインタ型のみ可
			static_assert(std::is_pointer_v<T>, "Unsupported template. supported templates: pointer types.");

			std::wstringstream stream;
			stream << L"0x" << std::showbase;
			stream << std::hex << source;
			return stream.str();
		}

		//----------------------------------------------------------------------------------
		//[to_radix]<possibly throw>
		//template: source type (supported templates: string_w<override>, wchar*<override>, const wchar*<override>, integer types)
		//return: sourceを基数表現に変換したstring
		//argument 1: 基数表現に用いる基数
		template<typename T>
		inline string_w to_radix(T source, radix::enum_radix radix)
		{
			//整数型のみ可
			static_assert(std::is_integral_v<T>, "Unsupported template. supported templates: string_w<override>, wchar*<override>, const wchar*<override>, integer types.");

			//文字列変換
			switch (radix)
			{
			case radix::x10:
				return convert(source);
			case radix::x16:
			{
				std::wstringstream stream;
				stream << std::showbase;
				stream << std::hex << source;
				return stream.str();
			}
			case radix::x8:
			{
				std::wstringstream stream;
				stream << std::showbase;
				stream << std::oct << source;
				return stream.str();
			}
			case radix::x2:
			{
				std::wstringstream stream;
				stream << std::showbase;
				stream << std::bitset<sizeof(source) * 8>(source) ;
				return stream.str();
			}
			default:
				throw Exception::InvalidArgumentException("stringf", "to_radix", "radix is invalid value.");
			}
		}
		//----------------------------------------------------------------------------------
		//[to_radix]<possibly throw>
		//template: source type (supported templates: string_w<override>, wchar*<override>, const wchar*<override>, integer types)
		//return: source(数字文字列)を基数表現に変換したstring
		//argument 1: 基数表現に用いる基数
		inline string_w to_radix(const string_w& source, radix::enum_radix radix)
		{
			//convert int64
			int64 convert = 0;

#if defined SGF_DEBUG
			try { convert = std::stoll(source); }
			catch (...)
			{
				//wstring->string
				int inLength = static_cast<int>(source.length());
				int outLength = WideCharToMultiByte(CP_UTF8, 0, source.c_str(), inLength, nullptr, 0, nullptr, nullptr);

				string_a sourceToAscii(outLength, L'\0');
				if (outLength > 0) WideCharToMultiByte(CP_UTF8, 0, source.c_str(), inLength, &sourceToAscii[0], outLength, nullptr, nullptr);

				throw Exception::InvalidArgumentException("stringf", "to_radix",
					"source->integer convert failed, source = " + (outLength > 0 ? sourceToAscii : "<empty>"));
			}
#else
			convert = std::stoll(source);
#endif

			//文字列変換
			switch (radix)
			{
			case radix::x10:
				return source;
			case radix::x16:
			{
				std::wstringstream stream;
				stream << std::showbase;
				stream << std::hex << convert;
				return stream.str();
			}
			case radix::x8:
			{
				std::wstringstream stream;
				stream << std::showbase;
				stream << std::oct << convert;
				return stream.str();
			}
			case radix::x2:
			{
				std::wstringstream stream;
				stream << std::showbase;
				stream << std::bitset<sizeof(convert) * 8>(convert);
				return stream.str();
			}
			default:
				throw Exception::InvalidArgumentException("stringf", "to_radix", "radix is invalid value.");
			}
		}
		//----------------------------------------------------------------------------------
		//[to_radix]
		//template: source type (supported templates: string_w<override>, wchar*<override>, const wchar*<override>, integer types)
		//return: source(数字文字列)を基数表現に変換したstring
		//argument 1: 基数表現に用いる基数
		inline string_w to_radix(wchar* source, radix::enum_radix radix)
		{
			//convert int64
			int64 convert = 0;
#if defined SGF_DEBUG
			try { convert = std::stoll(source); }
			catch (...)
			{
				//wstring->string
				int inLength = static_cast<int>(std::wcslen(source));
				int outLength = WideCharToMultiByte(CP_UTF8, 0, source, inLength, nullptr, 0, nullptr, nullptr);

				string_a sourceToAscii(outLength, L'\0');
				if (outLength > 0) WideCharToMultiByte(CP_UTF8, 0, source, inLength, &sourceToAscii[0], outLength, nullptr, nullptr);

				throw Exception::InvalidArgumentException("stringf", "to_radix",
					"source->integer convert failed, source = " + (outLength > 0 ? sourceToAscii : "<empty>"));
			}
#else
			convert = std::stoll(source);
#endif

			//文字列変換
			switch (radix)
			{
			case radix::x10:
				return source;
			case radix::x16:
			{
				std::wstringstream stream;
				stream << std::showbase;
				stream << std::hex << convert;
				return stream.str();
			}
			case radix::x8:
			{
				std::wstringstream stream;
				stream << std::showbase;
				stream << std::oct << convert;
				return stream.str();
			}
			case radix::x2:
			{
				std::wstringstream stream;
				stream << std::showbase;
				stream << std::bitset<sizeof(convert) * 8>(convert);
				return stream.str();
			}
			default:
				throw Exception::InvalidArgumentException("stringf", "to_radix", "radix is invalid value.");
			}
		}
		//----------------------------------------------------------------------------------
		//[to_radix]<possibly throw>
		//template: source type (supported templates: string_w<override>, wchar*<override>, const wchar*<override>, integer types)
		//return: source(数字文字列)を基数表現に変換したstring
		//argument 1: 基数表現に用いる基数
		inline string_w to_radix(const wchar* source, radix::enum_radix radix)
		{
			//convert int64
			int64 convert = 0;

#if defined SGF_DEBUG
			try { convert = std::stoll(source); }
			catch (...)
			{
				//wstring->string
				int inLength = static_cast<int>(std::wcslen(source));
				int outLength = WideCharToMultiByte(CP_UTF8, 0, source, inLength, nullptr, 0, nullptr, nullptr);

				string_a sourceToAscii(outLength, L'\0');
				if (outLength > 0) WideCharToMultiByte(CP_UTF8, 0, source, inLength, &sourceToAscii[0], outLength, nullptr, nullptr);

				throw Exception::InvalidArgumentException("stringf", "to_radix",
					"source->integer convert failed, source = " + (outLength > 0 ? sourceToAscii : "<empty>"));
			}
#else
			convert = std::stoll(source);
#endif

			//文字列変換
			switch (radix)
			{
			case radix::x10:
				return source;
			case radix::x16:
			{
				std::wstringstream stream;
				stream << std::showbase;
				stream << std::hex << convert;
				return stream.str();
			}
			case radix::x8:
			{
				std::wstringstream stream;
				stream << std::showbase;
				stream << std::oct << convert;
				return stream.str();
			}
			case radix::x2:
			{
				std::wstringstream stream;
				stream << std::showbase;
				stream << std::bitset<sizeof(convert) * 8>(convert);
				return stream.str();
			}
			default:
				throw Exception::InvalidArgumentException("stringf", "to_radix", "radix is invalid value.");
			}
		}
	}
}

#endif // !SGFRAMEWORK_HEADER_STRINGF_CONVERT_HPP_