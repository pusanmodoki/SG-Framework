/*----------------------------------------------------------------------------------
string_wを操作するstringf namespace
→convert関数を定義するStringfConvert.hpp
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_STRINGF_CONVERT_HPP_
#define SGFRAMEWORK_HEADER_STRINGF_CONVERT_HPP_
#include <string>
#include <vector>
#include "StringfUsingAndStructure.hpp"
#include "StringfOtherFunctions.hpp"
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
	//std::wstring, std::stringを操作するstringf namespace
	//stdクラスとの連携を前提としたもののため特例で命名規則をsnake_caseにする
	namespace stringf
	{
		//----------------------------------------------------------------------------------
		//convert functions


		//----------------------------------------------------------------------------------
		//[convert_ascii]
		//return: source(string_w)を変換したstring_a
		//argument 1: convert source
		inline string_a convert_ascii(const string_w& source)
		{
			int inLength = static_cast<int>(source.length());
			int outLength = WideCharToMultiByte(CP_UTF8, 0, source.c_str(), inLength, nullptr, 0, nullptr, nullptr);

			string_a result(outLength, L'\0');
			if (outLength > 0) WideCharToMultiByte(CP_UTF8, 0, source.c_str(), inLength, &result[0], outLength, nullptr, nullptr);
			return std::move(result);
		}

		//----------------------------------------------------------------------------------
		//[convert]<possibly throw(debug only)>
		//template: return type (supported templates: float, double, bool, integer types)
		//return: sourceを変換したT型変数
		//argument 1: convert source
		template<typename T>
		inline T convert(const string_w& source)
		{
			static_assert(std::is_integral_v<T>, "Unsupported template. supported templates: supported templates: float, double, bool, integer types");
			
			//char<n>_tなど普段使わない型はこちらで実装する, 定義未実装が起きた際はこれで防ぎたい…
			TRY_CATCH_ON_DEBUG(return IS_FALSE(std::is_unsigned_v<T>) ? static_cast<T>(std::stoll(source)) : static_cast<T>(std::stoull(source)),
				throw Exception::InvalidArgumentException("stringf", "convert", "source convert failed, source = " + (source.length() > 0 ? convert_ascii(source) : "<empty>")));
		}
		//----------------------------------------------------------------------------------
		//[convert]<possibly throw(debug only)>
		//template: return type (supported templates: float, double, bool, integer types)
		//return: sourceを変換したT型変数
		//argument 1: convert source
		template<>
		inline float convert<float>(const string_w& source)
		{
			TRY_CATCH_ON_DEBUG(return std::stof(source), throw Exception::InvalidArgumentException("stringf", "convert",
				"source convert failed, source = " + (source.length() > 0 ? convert_ascii(source) : "<empty>")));
		}
		//----------------------------------------------------------------------------------
		//[convert]<possibly throw(debug only)>
		//template: return type (supported templates: float, double, bool, integer types)
		//return: sourceを変換したT型変数
		//argument 1: convert source
		template<>
		inline double convert<double>(const string_w& source)
		{
			try { return std::stod(source); }
			catch (...)
			{
				throw Exception::InvalidArgumentException("stringf", "convert",
					"source convert failed, source = " + (source.length() > 0 ? convert_ascii(source) : "<empty>"));
			}
		}
		//----------------------------------------------------------------------------------
		//[convert]<possibly throw(debug only)>
		//template: return type (supported templates: float, double, bool, integer types)
		//return: sourceを変換したT型変数
		//argument 1: convert source (true, false, 0, 1の場合のみ正常動作, それ以外の場合throw)
		template<>
		inline bool convert<bool>(const string_w& source)
		{
			try 
			{ 
				if (source == L"true") return true;
				else if (source == L"false") return false;
				else if (source.length() == 1)
				{
					if (source[0] == L'1') return true;
					else if (source[0] == L'0') return true;
					else throw 0;
				}
				else throw 0;
			}
			catch (...)
			{
				throw Exception::InvalidArgumentException("stringf", "convert",
					"source convert failed, source = " + (source.length() > 0 ? convert_ascii(source) : "<empty>"));
			}
		}
		//----------------------------------------------------------------------------------
		//[convert]
		//template: return type (supported templates: float, double, bool, integer types)
		//return: sourceを変換したT型変数
		//argument 1: convert source
		template<>
		inline char convert<char>(const string_w& source)
		{
			try { return static_cast<char>(std::stoi(source)); }
			catch (...)
			{
				throw Exception::InvalidArgumentException("stringf", "convert",
					"source convert failed, source = " + (source.length() > 0 ? convert_ascii(source) : "<empty>"));
			}
		}
		//----------------------------------------------------------------------------------
		//[convert]
		//template: return type (supported templates: float, double, bool, integer types)
		//return: sourceを変換したT型変数
		//argument 1: convert source
		template<>
		inline uchar convert<uchar>(const string_w& source)
		{
			try { return static_cast<uchar>(std::stoul(source)); }
			catch (...)
			{
				throw Exception::InvalidArgumentException("stringf", "convert",
					"source convert failed, source = " + (source.length() > 0 ? convert_ascii(source) : "<empty>"));
			}
		}
		//----------------------------------------------------------------------------------
		//[convert]
		//template: return type (supported templates: float, double, bool, integer types)
		//return: sourceを変換したT型変数
		//argument 1: convert source
		template<>
		inline short convert<short>(const string_w& source)
		{
			try { return static_cast<short>(std::stoi(source)); }
			catch (...)
			{
				throw Exception::InvalidArgumentException("stringf", "convert",
					"source convert failed, source = " + (source.length() > 0 ? convert_ascii(source) : "<empty>"));
			}
		}
		//----------------------------------------------------------------------------------
		//[convert]
		//template: return type (supported templates: float, double, bool, integer types)
		//return: sourceを変換したT型変数
		//argument 1: convert source
		template<>
		inline ushort convert<ushort>(const string_w& source)
		{
			try { return static_cast<ushort>(std::stoul(source)); }
			catch (...)
			{
				throw Exception::InvalidArgumentException("stringf", "convert",
					"source convert failed, source = " + (source.length() > 0 ? convert_ascii(source) : "<empty>"));
			}
		}
		//----------------------------------------------------------------------------------
		//[convert]
		//template: return type (supported templates: float, double, bool, integer types)
		//return: sourceを変換したT型変数
		//argument 1: convert source
		template<>
		inline wchar convert<wchar>(const string_w& source)
		{
			try { return static_cast<wchar>(std::stoul(source)); }
			catch (...)
			{
				throw Exception::InvalidArgumentException("stringf", "convert",
					"source convert failed, source = " + (source.length() > 0 ? convert_ascii(source) : "<empty>"));
			}
		}
		//----------------------------------------------------------------------------------
		//[convert]
		//template: return type (supported templates: float, double, bool, integer types)
		//return: sourceを変換したT型変数
		//argument 1: convert source
		template<>
		inline int convert<int>(const string_w& source)
		{
			try { return static_cast<int>(std::stoi(source)); }
			catch (...)
			{
				throw Exception::InvalidArgumentException("stringf", "convert",
					"source convert failed, source = " + (source.length() > 0 ? convert_ascii(source) : "<empty>"));
			}
		}
		//----------------------------------------------------------------------------------
		//[convert]
		//template: return type (supported templates: float, double, bool, integer types)
		//return: sourceを変換したT型変数
		//argument 1: convert source
		template<>
		inline uint convert<uint>(const string_w& source)
		{
			try { return static_cast<uint>(std::stoul(source)); }
			catch (...)
			{
				throw Exception::InvalidArgumentException("stringf", "convert",
					"source convert failed, source = " + (source.length() > 0 ? convert_ascii(source) : "<empty>"));
			}
		}
		//----------------------------------------------------------------------------------
		//[convert]
		//template: return type (supported templates: float, double, bool, longeger types)
		//return: sourceを変換したT型変数
		//argument 1: convert source
		template<>
		inline long convert<long>(const string_w& source)
		{
			try { return static_cast<long>(std::stoi(source)); }
			catch (...)
			{
				throw Exception::InvalidArgumentException("stringf", "convert",
					"source convert failed, source = " + (source.length() > 0 ? convert_ascii(source) : "<empty>"));
			}
		}
		//----------------------------------------------------------------------------------
		//[convert]
		//template: return type (supported templates: float, double, bool, longeger types)
		//return: sourceを変換したT型変数
		//argument 1: convert source
		template<>
		inline ulong convert<ulong>(const string_w& source)
		{
			try { return static_cast<ulong>(std::stoul(source)); }
			catch (...)
			{
				throw Exception::InvalidArgumentException("stringf", "convert",
					"source convert failed, source = " + (source.length() > 0 ? convert_ascii(source) : "<empty>"));
			}
		}
		//----------------------------------------------------------------------------------
		//[convert]
		//template: return type (supported templates: float, double, bool, integer types)
		//return: sourceを変換したT型変数
		//argument 1: convert source
		template<>
		inline int64 convert<int64>(const string_w& source)
		{
			try { return static_cast<int64>(std::stoi(source)); }
			catch (...)
			{
				throw Exception::InvalidArgumentException("stringf", "convert",
					"source convert failed, source = " + (source.length() > 0 ? convert_ascii(source) : "<empty>"));
			}
		}
		//----------------------------------------------------------------------------------
		//[convert]
		//template: return type (supported templates: float, double, bool, integer types)
		//return: sourceを変換したT型変数
		//argument 1: convert source
		template<>
		inline uint64 convert<uint64>(const string_w& source)
		{
			try { return static_cast<uint64>(std::stoul(source)); }
			catch (...)
			{
				throw Exception::InvalidArgumentException("stringf", "convert",
					"source convert failed, source = " + (source.length() > 0 ? convert_ascii(source) : "<empty>"));
			}
		}

		//----------------------------------------------------------------------------------
		//[convert_array]
		//template: return type split array(supported templates: float, double, bool, integer types)
		//return: sourceをdelimiterで分割し数字変換T型配列
		//argument 1: convert source (数字, delmiterのみで構成したものに限る)
		template <typename T>
		inline std::vector<T> convert_array(const string_w& source, wchar delimiter = L',')
		{
			static_assert(std::is_integral_v<T> || std::is_same_v<bool, T> || std::is_same_v<float, T> || std::is_same_v<double, T>,
				"Unsupported template. supported templates: supported templates: float, double, bool, integer types");
			
			std::vector<string_w> split = stringf::split(source, delimiter, true);
			std::vector<T> result;

			for (auto& e : split)
			{
				try { result.emplace_back(convert<T>(e)); }
				catch (...)
				{
					throw Exception::InvalidArgumentException("stringf", "convert_array",
						"source convert failed, source = " + (source.length() > 0 ? convert_ascii(source) : "<empty>"));
				}
			}
			return std::move(result);
		}
	}
}
#endif // !SGFRAMEWORK_HEADER_STRINGF_CONVERT_HPP_