/*----------------------------------------------------------------------------------
std::wstringを拡張したsgstring class
SGFrameworkの文字列型はsgstringで統一される
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_SSF_STRING_HPP_
#define SGFRAMEWORK_HEADER_SSF_STRING_HPP_
#include <string>
#include <algorithm>
#include <Windows.h>
#include <iterator>
#include <ostream>
#include <sstream>
#include <bitset> 
#include <functional>
#include "../ConstAndUsing/ConstAndUsing.hpp"

//Framework namespace
namespace SGFramework
{
	//std::wstringを拡張したsgstring class
	//SGFrameworkの文字列型はsgstringで統一される
	class sgstring final : public std::wstring
	{
	public:
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//「""」をセット
		inline sgstring() : my_functions(*this) , std::wstring(L""){}
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//引数文字列をセット, wchar形式に変換を行うので重め
		//引数1: char文字列
		inline sgstring(const char* string) : my_functions(*this), std::wstring(to_unicode(string)) {}
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//引数文字列をセット
		//引数1: wchar文字列
		inline sgstring(const wchar* string) : my_functions(*this), std::wstring(string) {}
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//引数文字列をセット, wchar形式に変換を行うので重め
		//引数1: string文字列
		inline sgstring(const std::string& string) : my_functions(*this), std::wstring(to_unicode(string)) {}
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//引数文字列をセット
		//引数1: wstring文字列
		inline sgstring(const std::wstring& string) : my_functions(*this), std::wstring(string) {}
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//引数文字列をセット
		//引数1: sgstring文字列
		inline sgstring(const sgstring& string) : my_functions(*this), std::wstring(string) {}
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//引数文字列をセット
		//引数1: sgstring文字列
		inline sgstring(sgstring&& string) noexcept : my_functions(*this), std::wstring(std::move(string)) {}
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//引数数字を文字列変換してセット
		//引数1: int整数
		//引数2: 16進数表記にするか否か, default = false
		inline sgstring(int value, bool isHexadecimal = false) : my_functions(*this)
		{
			if (!isHexadecimal) this->assign(std::to_wstring(value)); 
			else 
			{ 
				std::wstringstream stream;
				stream << std::showbase;
				stream << std::hex << value;
				this->assign(stream.str()); 
			}
		}
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//引数数字を文字列変換してセット
		//引数1: uint整数
		//引数2: 16進数表記にするか否か, default = false
		inline sgstring(uint value, bool isHexadecimal = false) : my_functions(*this)
		{
			if (!isHexadecimal) this->assign(std::to_wstring(value));
			else
			{
				std::wstringstream stream;
				stream << std::showbase;
				stream << std::hex << value;
				this->assign(stream.str());
			}
		}
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//引数数字を文字列変換してセット
		//引数1: long整数
		//引数2: 16進数表記にするか否か, default = false
		inline sgstring(long value, bool isHexadecimal = false) : my_functions(*this)
		{
			if (!isHexadecimal) this->assign(std::to_wstring(value));
			else
			{
				std::wstringstream stream;
				stream << std::showbase;
				stream << std::hex << value;
				this->assign(stream.str());
			}
		}
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//引数数字を文字列変換してセット
		//引数1: ulong整数
		//引数2: 16進数表記にするか否か, default = false
		inline sgstring(ulong value, bool isHexadecimal = false) : my_functions(*this)
		{
			if (!isHexadecimal) this->assign(std::to_wstring(value));
			else
			{
				std::wstringstream stream;
				stream << std::showbase;
				stream << std::hex << value;
				this->assign(stream.str());
			}
		}
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//引数数字を文字列変換してセット
		//引数1: int64整数
		//引数2: 16進数表記にするか否か, default = false
		inline sgstring(int64 value, bool isHexadecimal = false) : my_functions(*this)
		{
			if (!isHexadecimal) this->assign(std::to_wstring(value));
			else
			{
				std::wstringstream stream;
				stream << std::showbase;
				stream << std::hex << value;
				this->assign(stream.str());
			}
		}
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//引数数字を文字列変換してセット
		//引数1: uint64整数
		//引数2: 16進数表記にするか否か, default = false
		inline sgstring(uint64 value, bool isHexadecimal = false) : my_functions(*this)
		{
			if (!isHexadecimal) this->assign(std::to_wstring(value));
			else
			{
				std::wstringstream stream;
				stream << std::showbase;
				stream << std::hex << value;
				this->assign(stream.str());
			}
		}
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//引数数字を文字列変換してセット
		//引数1: float実数
		inline sgstring(float value) : my_functions(*this),  std::wstring(std::to_wstring(value)) {}
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//引数数字を文字列変換してセット
		//引数1: float実数
		inline sgstring(double value) : my_functions(*this), std::wstring(std::to_wstring(value)) {}

		//----------------------------------------------------------------------------------
		//[to_unicode]
		//char文字列をwchar形式に変換
		//return: 変換文字列
		//引数1: char文字列
		inline static sgstring to_unicode(const char* string)
		{
			std::string toStd = string;
			int inLength = static_cast<int>(toStd.length());
			int outLength = MultiByteToWideChar(CP_UTF8, 0, toStd.c_str(), inLength, nullptr, 0);
			std::wstring result(outLength, L'\0');
			if (outLength) MultiByteToWideChar(CP_UTF8, 0, toStd.c_str(), inLength, &result[0], outLength);
			return std::move(result);
		}
		//----------------------------------------------------------------------------------
		//[to_unicode]
		//char文字列をwchar形式に変換
		//return: 変換文字列
		//引数1: char文字列
		inline static sgstring to_unicode(const std::string& string)
		{
			int inLength = static_cast<int>(string.length());
			int outLength = MultiByteToWideChar(CP_UTF8, 0, string.c_str(), inLength, nullptr, 0);
			std::wstring result(outLength, L'\0');
			if (outLength) MultiByteToWideChar(CP_UTF8, 0, string.c_str(), inLength, &result[0], outLength);
			return std::move(result);
		}

		//----------------------------------------------------------------------------------
		//[to_sgstring]
		//引数数字を文字列変換する
		//return: 変換文字列
		//引数1: int整数
		inline static sgstring to_sgstring(int value) { return std::to_wstring(value); }
		//----------------------------------------------------------------------------------
		//[to_sgstring]
		//引数数字を文字列変換する
		//return: 変換文字列
		//引数1: uint整数
		inline static sgstring to_sgstring(uint value) { return std::to_wstring(value); }
		//----------------------------------------------------------------------------------
		//[to_sgstring]
		//引数数字を文字列変換する
		//return: 変換文字列
		//引数1: long整数
		inline static sgstring to_sgstring(long value) { return std::to_wstring(value); }
		//----------------------------------------------------------------------------------
		//[to_sgstring]
		//引数数字を文字列変換する
		//return: 変換文字列
		//引数1: ulong整数
		inline static sgstring to_sgstring(ulong value) { return std::to_wstring(value); }
		//----------------------------------------------------------------------------------
		//[to_sgstring]
		//引数数字を文字列変換する
		//return: 変換文字列
		//引数1: int64整数
		inline static sgstring to_sgstring(int64 value) { return std::to_wstring(value); }
		//----------------------------------------------------------------------------------
		//[to_sgstring]
		//引数数字を文字列変換する
		//return: 変換文字列
		//引数1: uint64整数
		inline static sgstring to_sgstring(uint64 value) { return std::to_wstring(value); }
		//----------------------------------------------------------------------------------
		//[to_sgstring]
		//引数数字を文字列変換する
		//return: 変換文字列
		//引数1: float整数
		inline static sgstring to_sgstring(float value) { return std::to_wstring(value); }
		//----------------------------------------------------------------------------------
		//[to_sgstring]
		//引数数字を文字列変換する
		//return: 変換文字列
		//引数1: double整数
		inline static sgstring to_sgstring(double value) { return std::to_wstring(value); }


		//----------------------------------------------------------------------------------
		//[to_std_string]
		//自身をstd::2stringに変換
		//return: 変換文字列
		inline std::wstring to_std_wstring() const
		{
			return *this;
		}
		//----------------------------------------------------------------------------------
		//[to_std_string]
		//自身をstd::stringに変換
		//return: 変換文字列
		inline std::string to_std_string() const
		{
			int inLength = static_cast<int>(this->length());
			int outLength = WideCharToMultiByte(CP_UTF8, 0, this->c_str(), inLength, nullptr, 0, nullptr, nullptr);
			
			std::string result(outLength, L'\0');
			if (outLength) WideCharToMultiByte(CP_UTF8, 0, this->c_str(), inLength, &result[0], outLength, nullptr, nullptr);
			return std::move(result);
		}
		
		//wchar opeartor
		inline operator const wchar* () { return this->c_str(); }

		//() operatpr
		inline std::wstring& operator () () { return *this; }
		//= operator
		inline std::wstring  operator = (const std::wstring&& string) { return (this->assign(string)); }
		//= operator
		inline sgstring& operator = (sgstring&& string) = default;
		//= operator(なるべるwcahrを使うこと)
		inline std::wstring  operator = (const char* string) { return (this->assign(to_unicode(string))); }
		//= operator(なるべるwcahrを使うこと)
		inline std::wstring  operator = (const wchar* string) { return (this->assign(string)); }
		//= operator(なるべるwcahrを使うこと)
		inline std::wstring  operator = (const wchar string) { this->clear(); this->push_back(string); return *this; }
		//= operator(なるべるwcahrを使うこと)
		inline std::wstring  operator = (const std::string& string) { return (this->assign(to_unicode(string))); }
		//= operator(なるべるwcahrを使うこと)
		inline std::wstring  operator = (const std::wstring& string) { return (this->assign(string)); }
		//= operator(なるべるwcahrを使うこと)
		inline std::wstring  operator = (const sgstring& string) { return (this->assign(string)); }
		//= operator(なるべるwcahrを使うこと)
		inline std::wstring  operator = (int value) { return (this->assign(std::to_wstring(value))); }
		//= operator(なるべるwcahrを使うこと)
		inline std::wstring  operator = (uint value) { return (this->assign(std::to_wstring(value))); }
		//= operator(なるべるwcahrを使うこと)
		inline std::wstring  operator = (long value) { return (this->assign(std::to_wstring(value))); }
		//= operator(なるべるwcahrを使うこと)
		inline std::wstring  operator = (ulong value) { return (this->assign(std::to_wstring(value))); }
		//= operator(なるべるwcahrを使うこと)
		inline std::wstring  operator = (int64 value) { return (this->assign(std::to_wstring(value))); }
		//= operator(なるべるwcahrを使うこと)
		inline std::wstring  operator = (uint64 value) { return (this->assign(std::to_wstring(value))); }
		//= operator(なるべるwcahrを使うこと)
		inline std::wstring  operator = (float value) { return (this->assign(std::to_wstring(value))); }
		//= operator(なるべるwcahrを使うこと)
		inline std::wstring  operator = (double value) { return (this->assign(std::to_wstring(value))); }

		//+ operator(wchar形式に変換を行うので重め)
		inline std::wstring  operator + (const char* string) const { return (*this + to_unicode(string)); }
		//+ operator(wchar形式に変換を行うので重め)
		inline std::wstring  operator + (const std::string& string) const { return (*this + to_unicode(string)); }
		//+ operator
		inline std::wstring  operator + (wchar string) const { wchar add[] = { string, L'\0' }; return (*this + string); }

		//+= operator(wchar形式に変換を行うので重め)
		inline std::wstring operator += (const char* string) { return (this->append(to_unicode(string))); }
		//+= operator(wchar形式に変換を行うので重め)
		inline std::wstring operator += (const std::string& string) { return (this->append(to_unicode(string))); }
		//+= operator
		inline std::wstring operator += (const wchar* string) { return (this->append(string)); }
		//+= operator
		inline std::wstring operator += (const wchar string) { this->push_back(string); return *this; }
		//+= operator
		inline std::wstring operator += (const std::wstring& string) { return (this->append(string)); }
		//+= operator
		inline std::wstring operator += (const sgstring& string) { return (this->append(string)); }

		//+ operator(wchar形式に変換を行うので重め)
		inline std::wstring operator + (int value) const { return *this + std::to_wstring(value); }
		//+ operator(wchar形式に変換を行うので重め)
		inline std::wstring operator + (uint value) const { return *this + std::to_wstring(value); };
		//+ operator(wchar形式に変換を行うので重め)
		inline std::wstring operator + (long value) const { return *this + std::to_wstring(value); };
		//+ operator(wchar形式に変換を行うので重め)
		inline std::wstring operator + (ulong value) const { return *this + std::to_wstring(value); };
		//+ operator(wchar形式に変換を行うので重め)
		inline std::wstring operator + (int64 value) const { return *this + std::to_wstring(value); };
		//+ operator(wchar形式に変換を行うので重め)
		inline std::wstring operator + (uint64 value) const { return *this + std::to_wstring(value); };
		//+ operator(wchar形式に変換を行うので重め)
		inline std::wstring operator + (float value) const { return *this + std::to_wstring(value); };
		//+ operator(wchar形式に変換を行うので重め)
		inline std::wstring operator + (double value) const { return *this + std::to_wstring(value); };

		//+= operator(wchar形式に変換を行うので重め)
		inline std::wstring operator += (int value) { return (this->append(std::to_wstring(value))); }
		//+= operator(wchar形式に変換を行うので重め)
		inline std::wstring operator += (uint value) { return (this->append(std::to_wstring(value))); }
		//+= operator(wchar形式に変換を行うので重め)
		inline std::wstring operator += (long value) { return (this->append(std::to_wstring(value))); }
		//+= operator(wchar形式に変換を行うので重め)
		inline std::wstring operator += (ulong value) { return (this->append(std::to_wstring(value))); }
		//+= operator(wchar形式に変換を行うので重め)
		inline std::wstring operator += (int64 value) { return (this->append(std::to_wstring(value))); }
		//+= operator(wchar形式に変換を行うので重め)
		inline std::wstring operator += (uint64 value) { return (this->append(std::to_wstring(value))); }
		//+= operator(wchar形式に変換を行うので重め)
		inline std::wstring operator += (float value) { return (this->append(std::to_wstring(value))); }
		//+= operator(wchar形式に変換を行うので重め)
		inline std::wstring operator += (double value) { return (this->append(std::to_wstring(value))); }

		//＝= operator(wchar形式に変換を行うので重め)
		inline bool operator == (const char* string) const { return (*this == to_unicode(string)); }
		//＝= operator(wchar形式に変換を行うので重め)
		inline bool operator == (const std::string& string) const { return (*this == to_unicode(string)); }
		
		//!= operator(wchar形式に変換を行うので重め)
		inline bool operator != (const char* string) const { return (*this != to_unicode(string)); }
		//!= operator(wchar形式に変換を行うので重め)
		inline bool operator != (const std::string& string) const { return (*this != to_unicode(string)); }
	

		//numbers
		static constexpr wchar cNumberStrings[10] = { L'0', L'1',  L'2',  L'3',  L'4',  L'5',  L'6',  L'7',  L'8',  L'9' };


		public:

			//static functions
			class functions final
			{
			public:
				//----------------------------------------------------------------------------------
				//[Split]
				//文字列をdellimiterを区切りとして分割する
				//return: 分割した文字列を代入したvector
				//引数1: 対象文字列
				//引数2 : delimiter = Default: ','
				//引数3 : \0除く最後の要素が\nだった場合trueなら削除 = Default: false 
				inline static std::vector<sgstring> Split(const sgstring& string, wchar delimiter = L',', bool isNewLineDelete = false)
				{
					if (string.empty()) return std::vector<sgstring>();

					std::vector<sgstring> ret;						//返り値
					std::wstring getString = string;				//分割するstring
					std::wstring buf;									//getline用のバッファ
					std::wstringstream sStream(getString);	//getline用のstream

					//改行削除 = true で最後の文字が改行なら削除
					if (isNewLineDelete && getString[getString.size() - 1] == L'\n')
						getString.erase(getString.size() - 1);

					//区切り文字をもとに要素取得を行い要素をstring vectorに追加
					while (std::getline(sStream, buf, delimiter))
						ret.emplace_back(buf);

					return std::move(ret);
				}
				//----------------------------------------------------------------------------------
				//[Split]
				//文字列をdellimiterを区切りとして分割する
				//引数1: 分割した文字列を代入するvector
				//引数2: 対象文字列
				//引数3: delimiter = Default: ','
				//引数4: \0除く最後の要素が\nだった場合trueなら削除 = Default: false 
				inline static void Split(std::vector<sgstring>& write, const sgstring& string, wchar delimiter = L',', bool isNewLineDelete = false)
				{
					std::wstring getString = string;				//分割するstring
					std::wstring buf;									//getline用のバッファ
					std::wstringstream sStream(getString);	//getline用のstream

					//改行削除 = true で最後の文字が改行なら削除
					if (isNewLineDelete && getString[getString.size() - 1] == L'\n')
						getString.erase(getString.size() - 1);

					//区切り文字をもとに要素取得を行い要素をstring vectorに追加
					while (std::getline(sStream, buf, delimiter))
						write.emplace_back(buf);
				}

				//----------------------------------------------------------------------------------
				//[Join]
				//joinstringをdelimiterを区切りとし連結する
				//return: 連結したsgstring
				//引数1: 対象文字列vector
				//引数2 : delimiter = Default: ','
				//引数3: 結果の最後に改行を追加するか
				inline static sgstring Join(const std::vector<sgstring>& joinString, wchar delimiter, bool isNewLine)
				{
					sgstring result;	 //返り値
					for (const auto& e : joinString)
					{
						result.append(e);
						result.push_back(delimiter);
					}

					//retが要素ありなら最後の区切り文字削除
					if (result.size())		result.erase(result.end() - 1);
					//retが要素あり & 結果の最後に改行 = trueなら最後に改行追加
					if (result.size() && isNewLine) result.push_back(L'\n');
					return std::move(result);
				}
				//----------------------------------------------------------------------------------
				//[Join]
				//joinstringをdelimiterを区切りとし連結する
				//引数1: 連結代入するsgstring
				//引数2: 対象文字列vector
				//引数3: delimiter = Default: ','
				//引数4: 結果の最後に改行を追加するか
				inline static void Join(sgstring& write, const std::vector<sgstring>& joinString, wchar delimiter, bool isNewLine)
				{
					for (const auto& e : joinString)
					{
						write.append(e);
						write.push_back(delimiter);
					}

					//retが要素ありなら最後の区切り文字削除
					if (write.size())		write.erase(write.end() - 1);
					//retが要素あり & 結果の最後に改行 = trueなら最後に改行追加
					if (write.size() && isNewLine) write.push_back(L'\n');
				}

				//----------------------------------------------------------------------------------
				//[ToUnicode]
				//char文字列をwchar形式に変換
				//return: 変換文字列
				//引数1: char文字列
				inline static sgstring ToUnicode(const char* string)
				{
					std::string toStd = string;
					int inLength = static_cast<int>(toStd.length());
					int outLength = MultiByteToWideChar(CP_UTF8, 0, toStd.c_str(), inLength, 0, 0);
					std::wstring result(outLength, L'\0');
					if (outLength) MultiByteToWideChar(CP_UTF8, 0, toStd.c_str(), inLength, &result[0], outLength);
					return std::move(result);
				}
				//----------------------------------------------------------------------------------
				//[ToUnicode]
				//char文字列をwchar形式に変換
				//return: 変換文字列
				//引数1: char文字列
				inline static sgstring ToUnicode(const std::string& string)
				{
					int inLength = static_cast<int>(string.length());
					int outLength = MultiByteToWideChar(CP_UTF8, 0, string.c_str(), inLength, 0, 0);
					std::wstring result(outLength, L'\0');
					if (outLength) MultiByteToWideChar(CP_UTF8, 0, string.c_str(), inLength, &result[0], outLength);
					return std::move(result);
				}
				//----------------------------------------------------------------------------------
				//[IndexOf]
				//自身の文字列の先頭からstringを検索する, IndexOf == find
				//return: あった場合は先頭の添字, なければnpos
				//引数1: 対象文字列
				//引数2: 検索文字列
				inline static size_t IndexOf(const sgstring& string1, const sgstring& string2) { return string1.find_first_of(string2); }
				//----------------------------------------------------------------------------------
				//[IndexFirstOf]
				//自身の文字列の先頭からstringを検索する, IndexFirstOf == find
				//return: あった場合は先頭の添字, なければnpos
				//引数1: 対象文字列
				//引数2: 検索文字列
				inline static size_t IndexFirstOf(const sgstring& string1, const sgstring& string2) { return string1.find(string2); }
				//----------------------------------------------------------------------------------
				//[IndexLastOf]
				//自身の文字列の最後からstringを検索する, IndexLastOf == rfind
				//return: あった場合は先頭の添字, なければnpos
				//引数1: 対象文字列
				//引数2: 検索文字列
				inline static size_t IndexLastOf(const sgstring& string1, const sgstring& string2) { return string1.rfind(string2); }
				//----------------------------------------------------------------------------------
				//[IsStartWith]
				//自身の文字列の最初とstringを比較する
				//return: 自身の文字列がstringから始まっていればtrue
				//引数1: 対象文字列
				//引数2: 検索文字列
				inline static bool IsStartWith(const sgstring& string1, const sgstring& string2) { return (string1.length() >= string2.length() && std::equal(string2.begin(), string2.end(), string1.begin())); }
				//----------------------------------------------------------------------------------
				//[IsEndWith]
				//自身の文字列の最後とstringを比較する
				//return: 自身の文字列がstringで終わっていればtrue
				//引数1: 対象文字列
				//引数2: 検索文字列
				inline static bool IsEndWith(const sgstring& string1, const sgstring& string2) { return (string1.length() >= string2.length() && std::equal(string2.rbegin(), string2.rend(), string1.rbegin())); }
				//----------------------------------------------------------------------------------
				//[IsContains]
				//自身の文字列にstringがあるか確認する
				//return: 自身の文字列にstringが含まれていればtrue
				//引数1: 対象文字列
				//引数2: 検索文字列
				inline static bool IsContains(const sgstring& string1, const sgstring& string2) {
					auto result = (string1.find(string2));
					return (result >= 0 && result != sgstring::npos);
				}
				//----------------------------------------------------------------------------------
				//[IsFirstOfNumbers]
				//stringの文字列の先頭が数字文字列か確認する
				//return: stringの先頭文字が数字ならばtrue
				//引数1: 対象文字列
				//引数2: 空白文字が先頭にあり、その後が数字文字列の場合空白削除してtrue?, default = true
				inline static bool IsFirstOfNumbers(sgstring& string, bool ifBlankDelete = true)
				{
					if (string.empty()) return false;
					for (const auto& e : cNumberStrings)
					{
						if (string[0] == e) return true;
						else if (string[0] == L'-' && string[1] == e) return true;
					}

					if (IS_TRUE(ifBlankDelete) && string[0] == L' ')
					{
						for (int i = 1, length = static_cast<int>(string.length()); i < length; ++i)
						{
							if (string[i] != L' ')
							{
								int result = -1;
								for (const auto& e : cNumberStrings)
								{
									if (string[i] == e || (i < length - 1 && string[i] == L'-' && string[i + 1] == e))
									{
										result = i;
										break;
									}
								}
								if (result >= 0)
								{
									string.erase(string.begin(), string.begin() + result);
									return true;
								}
								else return false;
							}
						}
					}
					return false;
				}

				//----------------------------------------------------------------------------------
				//[ToX16]
				//数字文字列を16進数表記に変換する
				//throw: 数字に変換できなかった場合
				//return: 変換した文字列
				//引数1: 対象文字列
				static sgstring ToX16(const sgstring& string);
				//----------------------------------------------------------------------------------
				//[ToX10]
				//数字文字列を10進数表記に変換する
				//throw: 数字に変換できなかった場合
				//return: 変換した文字列
				//引数1: 対象文字列
				static sgstring ToX10(const sgstring& string);
				//----------------------------------------------------------------------------------
				//[ToX8]
				//数字文字列を8進数表記に変換する
				//throw: 数字に変換できなかった場合
				//return: 変換した文字列
				//引数1: 対象文字列
				static sgstring ToX8(const sgstring& string);
				//----------------------------------------------------------------------------------
				//[ToX2]
				//数字文字列を2進数表記に変換する
				//throw: 数字に変換できなかった場合
				//return: 変換した文字列
				//引数1: 対象文字列
				static sgstring ToX2(const sgstring& string);

				//----------------------------------------------------------------------------------
				//[ToX16String]
				//数字を16進数表記文字列に変換する
				//return: 変換した文字列
				//引数1: 対象数字
				inline static sgstring ToX16String(int value)
				{
					std::wstringstream stream;
					stream << std::showbase;
					stream << std::hex << value;
					return stream.str();
				}
				//----------------------------------------------------------------------------------
				//[ToX16String]
				//数字を16進数表記文字列に変換する
				//return: 変換した文字列
				//引数1: 対象数字
				inline static sgstring ToX16String(uint value)
				{
					std::wstringstream stream;
					stream << std::showbase;
					stream << std::hex << value;
					return stream.str();
				}
				//----------------------------------------------------------------------------------
				//[ToX16String]
				//数字を16進数表記文字列に変換する
				//return: 変換した文字列
				//引数1: 対象数字
				inline static sgstring ToX16String(long value)
				{
					std::wstringstream stream;
					stream << std::showbase;
					stream << std::hex << value;
					return stream.str();
				}
				//----------------------------------------------------------------------------------
				//[ToX16String]
				//数字を16進数表記文字列に変換する
				//return: 変換した文字列
				//引数1: 対象数字
				inline static sgstring ToX16String(ulong value)
				{
					std::wstringstream stream;
					stream << std::showbase;
					stream << std::hex << value;
					return stream.str();
				}
				//----------------------------------------------------------------------------------
				//[ToX16String]
				//数字を16進数表記文字列に変換する
				//return: 変換した文字列
				//引数1: 対象数字
				inline static sgstring ToX16String(int64 value)
				{
					std::wstringstream stream;
					stream << std::showbase;
					stream << std::hex << value;
					return stream.str();
				}
				//----------------------------------------------------------------------------------
				//[ToX16String]
				//数字を16進数表記文字列に変換する
				//return: 変換した文字列
				//引数1: 対象数字
				inline static sgstring ToX16String(uint64 value)
				{
					std::wstringstream stream;
					stream << std::showbase;
					stream << std::hex << value;
					return stream.str();
				}

				//----------------------------------------------------------------------------------
				//[ToX8String]
				//数字を8進数表記文字列に変換する
				//return: 変換した文字列
				//引数1: 対象数字
				inline static sgstring ToX8String(int value)
				{
					std::wstringstream stream;
					stream << std::showbase;
					stream << std::oct << value;
					return stream.str();
				}
				//----------------------------------------------------------------------------------
				//[ToX8String]
				//数字を8進数表記文字列に変換する
				//return: 変換した文字列
				//引数1: 対象数字
				inline static sgstring ToX8String(uint value)
				{
					std::wstringstream stream;
					stream << std::showbase;
					stream << std::oct << value;
					return stream.str();
				}
				//----------------------------------------------------------------------------------
				//[ToX8String]
				//数字を8進数表記文字列に変換する
				//return: 変換した文字列
				//引数1: 対象数字
				inline static sgstring ToX8String(long value)
				{
					std::wstringstream stream;
					stream << std::showbase;
					stream << std::oct << value;
					return stream.str();
				}
				//----------------------------------------------------------------------------------
				//[ToX8String]
				//数字を8進数表記文字列に変換する
				//return: 変換した文字列
				//引数1: 対象数字
				inline static sgstring ToX8String(ulong value)
				{
					std::wstringstream stream;
					stream << std::showbase;
					stream << std::oct << value;
					return stream.str();
				}
				//----------------------------------------------------------------------------------
				//[ToX8String]
				//数字を8進数表記文字列に変換する
				//return: 変換した文字列
				//引数1: 対象数字
				inline static sgstring ToX8String(int64 value)
				{
					std::wstringstream stream;
					stream << std::showbase;
					stream << std::oct << value;
					return stream.str();
				}
				//----------------------------------------------------------------------------------
				//[ToX8String]
				//数字を8進数表記文字列に変換する
				//return: 変換した文字列
				//引数1: 対象数字
				inline static sgstring ToX8String(uint64 value)
				{
					std::wstringstream stream;
					stream << std::showbase;
					stream << std::oct << value;
					return stream.str();
				}

				//----------------------------------------------------------------------------------
				//[ToX2String]
				//数字を2進数表記文字列に変換する
				//return: 変換した文字列
				//引数1: 対象数字
				inline static sgstring ToX2String(int value)
				{
					std::wstringstream stream;
					stream << std::showbase;
					stream << std::bitset<sizeof(value) * 8>(value);
					return stream.str();
				}
				//----------------------------------------------------------------------------------
				//[ToX2String]
				//数字を2進数表記文字列に変換する
				//return: 変換した文字列
				//引数1: 対象数字
				inline static sgstring ToX2String(uint value)
				{
					std::wstringstream stream;
					stream << std::showbase;
					stream << std::bitset<sizeof(value) * 8>(value);
					return stream.str();
				}
				//----------------------------------------------------------------------------------
				//[ToX2String]
				//数字を2進数表記文字列に変換する
				//return: 変換した文字列
				//引数1: 対象数字
				inline static sgstring ToX2String(long value)
				{
					std::wstringstream stream;
					stream << std::showbase;
					stream << std::bitset<sizeof(value) * 8>(value);
					return stream.str();
				}
				//----------------------------------------------------------------------------------
				//[ToX2String]
				//数字を2進数表記文字列に変換する
				//return: 変換した文字列
				//引数1: 対象数字
				inline static sgstring ToX2String(ulong value)
				{
					std::wstringstream stream;
					stream << std::showbase;
					stream << std::bitset<sizeof(value) * 8>(value);
					return stream.str();
				}
				//----------------------------------------------------------------------------------
				//[ToX2String]
				//数字を2進数表記文字列に変換する
				//return: 変換した文字列
				//引数1: 対象数字
				inline static sgstring ToX2String(int64 value)
				{
					std::wstringstream stream;
					stream << std::showbase;
					stream << std::bitset<sizeof(value) * 8>(value);
					return stream.str();
				}
				//----------------------------------------------------------------------------------
				//[ToX2String]
				//数字を2進数表記文字列に変換する
				//return: 変換した文字列
				//引数1: 対象数字
				inline static sgstring ToX2String(uint64 value)
				{
					std::wstringstream stream;
					stream << std::showbase;
					stream << std::bitset<sizeof(value) * 8>(value);
					return stream.str();
				}


				//----------------------------------------------------------------------------------
				//[ToInt]
				//文字列をintに変換する
				//throw: 変換できなかった場合
				//return: 変換した値
				//引数1: 対象文字列
				static int ToInt(const sgstring& string);
				//----------------------------------------------------------------------------------
				//[ToLong]
				//文字列をlongに変換する
				//throw: 変換できなかった場合
				//return: 変換した値
				//引数1: 対象文字列
				static long ToLong(const sgstring& string);
				//----------------------------------------------------------------------------------
				//[ToUlong]
				//文字列をulongに変換する
				//throw: 変換できなかった場合
				//return: 変換した値
				//引数1: 対象文字列
				static ulong ToUlong(const sgstring& string);
				//----------------------------------------------------------------------------------
				//[ToInt64]
				//文字列をint64に変換する
				//throw: 変換できなかった場合
				//return: 変換した値
				//引数1: 対象文字列
				static int64 ToInt64(const sgstring& string);
				//----------------------------------------------------------------------------------
				//[ToUint64]
				//文字列をuint64に変換する
				//throw: 変換できなかった場合
				//return: 変換した値
				//引数1: 対象文字列
				static uint64 ToUint64(const sgstring& string);
				//----------------------------------------------------------------------------------
				//[ToFloat]
				//文字列をfloatに変換する
				//throw: 変換できなかった場合
				//return: 変換した値
				//引数1: 対象文字列
				static float ToFloat(const sgstring& string);
				//----------------------------------------------------------------------------------
				//[ToDouble]
				//文字列をdoubleに変換する
				//throw: 変換できなかった場合
				//return: 変換した値
				//引数1: 対象文字列
				static double ToDouble(const sgstring& string);


				//----------------------------------------------------------------------------------
				//[ToString]
				//数字をstringに変換する
				//return: 変換した文字列
				//引数1: 対象数字
				inline static sgstring ToString(int value) { return std::to_wstring(value); }
				//----------------------------------------------------------------------------------
				//[ToString]
				//数字をstringに変換する
				//return: 変換した文字列
				//引数1: 対象数字
				inline static sgstring ToString(uint value) { return std::to_wstring(value); }
				//----------------------------------------------------------------------------------
				//[ToString]
				//数字をstringに変換する
				//return: 変換した文字列
				//引数1: 対象数字
				inline static sgstring ToString(long value) { return std::to_wstring(value); }
				//----------------------------------------------------------------------------------
				//[ToString]
				//数字をstringに変換する
				//return: 変換した文字列
				//引数1: 対象数字
				inline static sgstring ToString(ulong value) { return std::to_wstring(value); }
				//----------------------------------------------------------------------------------
				//[ToString]
				//数字をstringに変換する
				//return: 変換した文字列
				//引数1: 対象数字
				inline static sgstring ToString(int64 value) { return std::to_wstring(value); }
				//----------------------------------------------------------------------------------
				//[ToString]
				//数字をstringに変換する
				//return: 変換した文字列
				//引数1: 対象数字
				inline static sgstring ToString(uint64 value) { return std::to_wstring(value); }
				//----------------------------------------------------------------------------------
				//[ToString]
				//数字をstringに変換する
				//return: 変換した文字列
				//引数1: 対象数字
				inline static sgstring ToString(float value) { return std::to_wstring(value); }
				//----------------------------------------------------------------------------------
				//[ToString]
				//数字をstringに変換する
				//return: 変換した文字列
				//引数1: 対象数字
				inline static sgstring ToString(double value) { return std::to_wstring(value); }


				//----------------------------------------------------------------------------------
				//[ToArrayInt]
				//文字列をdelimiterで分割したvectorに変換する
				//return: 変換したvector, 無効ならsize = 0
				//引数1: 対象文字列
				//引数2: delimiter = default L','
				inline static std::vector<int> ToArrayInt(const sgstring& string, wchar delimiter = L',')
				{
					std::vector<sgstring> vec = Split(string, delimiter, true);
					std::vector<int> ret;
					for (auto& e : vec)
					{
						try { ret.emplace_back(std::stoi(e)); }
						catch (...) {}
					}
					return ret;
				}
				//----------------------------------------------------------------------------------
				//[ToArrayLong]
				//文字列をdelimiterで分割したvectorに変換する
				//return: 変換したvector, 無効ならsize = 0
				//引数1: 対象文字列
				//引数2: delimiter = default L','
				inline static std::vector<long> ToArrayLong(const sgstring& string, wchar delimiter = L',')
				{
					std::vector<sgstring> vec = Split(string, delimiter, true);
					std::vector<long> ret;
					for (auto& e : vec)
					{
						try { ret.emplace_back(std::stol(e)); }
						catch (...) {}
					}
					return ret;
				}
				//----------------------------------------------------------------------------------
				//[ToArrayUlong]
				//文字列をdelimiterで分割したvectorに変換する
				//return: 変換したvector, 無効ならsize = 0
				//引数1: 対象文字列
				//引数2: delimiter = default L','
				inline static std::vector<ulong> ToArrayUlong(const sgstring& string, wchar delimiter = L',')
				{
					std::vector<sgstring> vec = Split(string, delimiter, true);
					std::vector <ulong> ret;
					for (auto& e : vec)
					{
						try { ret.emplace_back(std::stoul(e)); }
						catch (...) {}
					}
					return ret;
				}
				//----------------------------------------------------------------------------------
				//[ToArrayInt64]
				//文字列をdelimiterで分割したvectorに変換する
				//return: 変換したvector, 無効ならsize = 0
				//引数1: 対象文字列
				//引数2: delimiter = default L','
				inline static std::vector<int64> ToArrayInt64(const sgstring& string, wchar delimiter = L',')
				{
					std::vector<sgstring> vec = Split(string, delimiter, true);
					std::vector <int64> ret;
					for (auto& e : vec)
					{
						try { ret.emplace_back(std::stoll(e)); }
						catch (...) {}
					}
					return ret;
				}
				//----------------------------------------------------------------------------------
				//[ToArrayUInt64]
				//文字列をdelimiterで分割したvectorに変換する
				//return: 変換したvector, 無効ならsize = 0
				//引数1: 対象文字列
				//引数2: delimiter = default L','
				inline static std::vector<uint64> ToArrayUint64(const sgstring& string, wchar delimiter = L',')
				{
					std::vector<sgstring> vec = Split(string, delimiter, true);
					std::vector <uint64> ret;
					for (auto& e : vec)
					{
						try { ret.emplace_back(std::stoull(e)); }
						catch (...) {}
					}
					return ret;
				}
				//----------------------------------------------------------------------------------
				//[ToArrayFloat]
				//文字列をdelimiterで分割したvectorに変換する
				//return: 変換したvector, 無効ならsize = 0
				//引数1: 対象文字列
				//引数2: delimiter = default L','
				inline static std::vector<float> ToArrayFloat(const sgstring& string, wchar delimiter = L',')
				{
					std::vector<sgstring> vec = Split(string, delimiter, true);
					std::vector <float> ret;
					for (auto& e : vec)
					{
						try { ret.emplace_back(std::stof(e)); }
						catch (...) {}
					}
					return ret;
				}
				//----------------------------------------------------------------------------------
				//[ToArrayDouble]
				//文字列をdelimiterで分割したvectorに変換する
				//return: 変換したvector, 無効ならsize = 0
				//引数1: 対象文字列
				//引数2: delimiter = default L','
				inline static std::vector<double> ToArrayDouble(const sgstring& string, wchar delimiter = L',')
				{
					std::vector<sgstring> vec = Split(string, delimiter, true);
					std::vector <double> ret;
					for (auto& e : vec)
					{
						try { ret.emplace_back(std::stod(e)); }
						catch (...) {}
					}
					return ret;
				}
			};

	private:

		//string追加関数郡実体版
		class MyFunctions
		{
		public:
			//----------------------------------------------------------------------------------
			//[コンストラクタ]
			//参照をセット
			//引数1: this
			inline MyFunctions(sgstring& string) : m_string(string) { }

			//----------------------------------------------------------------------------------
			//[Clear]
			//文字列クリア, Clear() == wstring::clear()
			inline void Clear() { m_string.clear(); }
			//----------------------------------------------------------------------------------
			//[Split]
			//自身の文字列をdellimiterを区切りとして分割する
			//return: 分割した文字列を代入したvector
			//引数1 : delimiter = Default: ','
			//引数2 : \0除く最後の要素が\nだった場合trueなら削除 = Default: false 
			inline std::vector<sgstring> Split(wchar delimiter = L',', bool isNewLineDelete = false) const
			{
				std::vector<sgstring> ret;						//返り値
				std::wstring getString = m_string;			//分割するstring
				std::wstring buf;									//getline用のバッファ
				std::wstringstream sStream(getString);	//getline用のstream

				//改行削除 = true で最後の文字が改行なら削除
				if (isNewLineDelete && getString[getString.size() - 1] == L'\n')
					getString.erase(getString.size() - 1);

				//区切り文字をもとに要素取得を行い要素をstring vectorに追加
				while (std::getline(sStream, buf, delimiter))
					ret.emplace_back(buf);

				return std::move(ret);
			}
			//----------------------------------------------------------------------------------
			//[Split]
			//自身の文字列をdellimiterを区切りとして分割する
			//引数1: 分割した文字列を代入するvector
			//引数2: delimiter = Default: ','
			//引数3: \0除く最後の要素が\nだった場合trueなら削除 = Default: false 
			inline void Split(std::vector<sgstring>& write, wchar delimiter = L',', bool isNewLineDelete = false) const
			{
				if (this->IsEmpty()) return;

				std::wstring getString = m_string;			//分割するstring
				std::wstring buf;									//getline用のバッファ
				std::wstringstream sStream(getString);	//getline用のstream

				//クリア
				write.clear();

				//改行削除 = true で最後の文字が改行なら削除
				if (isNewLineDelete && getString[getString.size() - 1] == L'\n')
					getString.erase(getString.size() - 1);

				//区切り文字をもとに要素取得を行い要素をstring vectorに追加
				while (std::getline(sStream, buf, delimiter))
					write.emplace_back(buf);
			}
			//----------------------------------------------------------------------------------
			//[IndexOf]
			//自身の文字列の先頭からstringを検索する, IndexOf == find
			//return: あった場合は先頭の添字, なければ-1
			//引数1: 検索文字列
			inline size_t IndexOf(const sgstring& string) const { return m_string.find(string); }
			//----------------------------------------------------------------------------------
			//[IndexFirstOf]
			//自身の文字列の先頭からstringを検索する, IndexFirstOf == find
			//return: あった場合は先頭の添字, なければ-1
			//引数1: 検索文字列
			inline size_t IndexFirstOf(const sgstring& string) const { return m_string.find(string); }
			//----------------------------------------------------------------------------------
			//[IndexLastOf]
			//自身の文字列の最後からstringを検索する, IndexLastOf == rfind
			//return: あった場合は先頭の添字, なければ-1
			//引数1: 検索文字列
			inline size_t IndexLastOf(const sgstring& string) const { return m_string.rfind(string); }
			//----------------------------------------------------------------------------------
			//[IsStartWith]
			//自身の文字列の最初とstringを比較する
			//return: 自身の文字列がstringから始まっていればtrue
			//引数1: 検索文字列
			inline bool IsStartWith(const sgstring& string) const { return (m_string.length() >= string.length() && std::equal(string.begin(), string.end(), m_string.begin())); }
			//----------------------------------------------------------------------------------
			//[IsEndWith]
			//自身の文字列の最後とstringを比較する
			//return: 自身の文字列がstringで終わっていればtrue
			//引数1: 検索文字列
			inline bool IsEndWith(const sgstring& string) const { return (m_string.length() >= string.length() && std::equal(string.rbegin(), string.rend(), m_string.rbegin())); }
			//----------------------------------------------------------------------------------
			//[IsContains]
			//自身の文字列にstringがあるか確認する
			//return: 自身の文字列にstringが含まれていればtrue
			//引数1: 検索文字列
			inline bool IsContains(const sgstring& string) const {
				auto result = (m_string.find(string));
				return (result >= 0 && result != sgstring::npos); }
			//----------------------------------------------------------------------------------
			//[IsEmpty]
			//自身が空か確認する, IsEnpty == empty
			//return: 空ならtrue
			inline bool IsEmpty() const { return m_string.empty(); }
			//----------------------------------------------------------------------------------
			//[IsFirstOfNumbers]
			//自身のの文字列の先頭が数字文字列か確認する
			//引数1: 空白文字が先頭にあり、その後が数字文字列の場合空白削除してtrue?, default = true
			//return: 自身のの先頭文字が数字ならばtrue
			inline bool IsFirstOfNumbers(bool ifBlankDelete = true)
			{
				if (m_string.empty()) return false;
				for (const auto& e : cNumberStrings)
				{
					if (m_string[0] == e) return true;
					else if (m_string[0] == L'-' && m_string[1] == e) return true;
				}

				if (IS_TRUE(ifBlankDelete) && m_string[0] == L' ')
				{
					for (int i = 1, length = static_cast<int>(m_string.length()); i < length; ++i)
					{
						if (m_string[i] != L' ')
						{
							int result = -1;
							for (const auto& e : cNumberStrings)
							{
								if (m_string[i] == e 
									|| (i < length - 1 && m_string[i] == L'-' && m_string[i + 1] == e))
								{
									result = i;
									break;
								}
							}
							if (result >= 0)
							{
								m_string.erase(m_string.begin(), m_string.begin() + result);
								return true;
							}
							else return false;
						}
					}
				}
				return false;
	
				if (m_string.empty()) return false;
				for (const auto& e : cNumberStrings)
				{
					if (m_string[0] == e) return true;
					else if ((m_string[0] == L'-' || m_string[0] == L' ')
						&& m_string[1] == e) return true;
				}
				return false;
			}
			//----------------------------------------------------------------------------------
			//[FindExtension]
			//return: 拡張子を発見した場合は拡張子( . 含む), そうれなければ ""
			inline sgstring FindExtension() const 
			{
				size_t find = m_string.rfind(L'.');
				
				if (find != m_string.npos)
					return m_string.substr(find, m_string.size() - find);
				else
					return sgstring();
			}


			//----------------------------------------------------------------------------------
			//[ToX16]
			//数字文字列を16進数表記に変換する
			//throw: 数字に変換できなかった場合
			//return: 変換した文字列
			sgstring ToX16() const;
			//----------------------------------------------------------------------------------
			//[ToX10]
			//数字文字列を10進数表記に変換する
			//throw: 数字に変換できなかった場合
			//return: 変換した文字列
			sgstring ToX10() const;
			//----------------------------------------------------------------------------------
			//[ToX8]
			//数字文字列を8進数表記に変換する
			//throw: 数字に変換できなかった場合
			//return: 変換した文字列
			sgstring ToX8() const;
			//----------------------------------------------------------------------------------
			//[ToX2]
			//数字文字列を2進数表記に変換する
			//throw: 数字に変換できなかった場合
			//return: 変換した文字列
			sgstring ToX2() const;

			//----------------------------------------------------------------------------------
			//[ToInt]
			//文字列をintに変換する
			//throw: 変換できなかった場合
			//return: 変換した値
			int ToInt() const;
			//----------------------------------------------------------------------------------
			//[ToLong]
			//文字列をlongに変換する
			//throw: 変換できなかった場合
			//return: 変換した値
			long ToLong() const;
			//----------------------------------------------------------------------------------
			//[ToUlong]
			//文字列をulongに変換する
			//throw: 変換できなかった場合
			//return: 変換した値
			ulong ToUlong() const;
			//----------------------------------------------------------------------------------
			//[ToInt64]
			//文字列をint64に変換する
			//throw: 変換できなかった場合
			//return: 変換した値
			int64 ToInt64() const;
			//----------------------------------------------------------------------------------
			//[ToUint64]
			//文字列をuint64に変換する
			//throw: 変換できなかった場合
			//return: 変換した値
			uint64 ToUint64() const;
			//----------------------------------------------------------------------------------
			//[ToFloat]
			//文字列をfloatに変換する
			//throw: 変換できなかった場合
			//return: 変換した値
			float ToFloat() const;
			//----------------------------------------------------------------------------------
			//[ToDouble]
			//文字列をdoubleに変換する
			//throw: 変換できなかった場合
			//return: 変換した値
			double ToDouble() const;

			//----------------------------------------------------------------------------------
			//[ToArrayInt]
			//文字列をdelimiterで分割したvectorに変換する
			//return: 変換したvector, 無効ならsize = 0
			//引数1: delimiter = default L','
			inline std::vector<int> ToArrayInt(wchar delimiter = L',') const
			{
				std::vector<sgstring> vec = Split(delimiter, true);
				std::vector<int> ret;
				for (auto& e : vec)
				{
					try { ret.emplace_back(std::stoi(e)); }
					catch (...) {}
				}
				return ret;
			}
			//----------------------------------------------------------------------------------
			//[ToArrayLong]
			//文字列をdelimiterで分割したvectorに変換する
			//return: 変換したvector, 無効ならsize = 0
			//引数1: delimiter = default L','
			inline std::vector<long> ToArrayLong(wchar delimiter = L',') const
			{
				std::vector<sgstring> vec = Split(delimiter, true);
				std::vector<long> ret;
				for (auto& e : vec)
				{
					try { ret.emplace_back(std::stol(e)); }
					catch (...) {}
				}
				return ret;
			}
			//----------------------------------------------------------------------------------
			//[ToArrayUlong]
			//文字列をdelimiterで分割したvectorに変換する
			//return: 変換したvector, 無効ならsize = 0
			//引数1: delimiter = default L','
			inline std::vector<ulong> ToArrayUlong(wchar delimiter = L',') const
			{
				std::vector<sgstring> vec = Split(delimiter, true);
				std::vector <ulong> ret;
				for (auto& e : vec)
				{
					try { ret.emplace_back(std::stoul(e)); }
					catch (...) {}
				}
				return ret;
			}
			//----------------------------------------------------------------------------------
			//[ToArrayInt64]
			//文字列をdelimiterで分割したvectorに変換する
			//return: 変換したvector, 無効ならsize = 0
			//引数1: delimiter = default L','
			inline std::vector<int64> ToArrayInt64(wchar delimiter = L',') const
			{
				std::vector<sgstring> vec = Split(delimiter, true);
				std::vector <int64> ret;
				for (auto& e : vec)
				{
					try { ret.emplace_back(std::stoll(e)); }
					catch (...) {}
				}
				return ret;
			}
			//----------------------------------------------------------------------------------
			//[ToArrayUInt64]
			//文字列をdelimiterで分割したvectorに変換する
			//return: 変換したvector, 無効ならsize = 0
			//引数1: delimiter = default L','
			inline std::vector<uint64> ToArrayUint64(wchar delimiter = L',') const
			{
				std::vector<sgstring> vec = Split(delimiter, true);
				std::vector <uint64> ret;
				for (auto& e : vec)
				{
					try { ret.emplace_back(std::stoull(e)); }
					catch (...) {}
				}
				return ret;
			}
			//----------------------------------------------------------------------------------
			//[ToArrayFloat]
			//文字列をdelimiterで分割したvectorに変換する
			//return: 変換したvector, 無効ならsize = 0
			//引数1: delimiter = default L','
			inline std::vector<float> ToArrayFloat(wchar delimiter = L',') const
			{
				std::vector<sgstring> vec = Split(delimiter, true);
				std::vector <float> ret;
				for (auto& e : vec)
				{
					try { ret.emplace_back(std::stof(e)); }
					catch (...) {}
				}
				return ret;
			}
			//----------------------------------------------------------------------------------
			//[ToArrayDouble]
			//文字列をdelimiterで分割したvectorに変換する
			//return: 変換したvector, 無効ならsize = 0
			//引数1: delimiter = default L','
			inline std::vector<double> ToArrayDouble(wchar delimiter = L',') const
			{
				std::vector<sgstring> vec = Split(delimiter, true);
				std::vector <double> ret;
				for (auto& e : vec)
				{
					try { ret.emplace_back(std::stod(e)); }
					catch (...) {}
				}
				return ret;
			}

		private:
			sgstring& m_string;
		};

	public:
		//self functions
		MyFunctions my_functions;
	};
}


template <>
struct std::hash<SGFramework::sgstring>
{
	size_t operator()(const SGFramework::sgstring& k) const
	{
		std::size_t hash = SGFramework::Hash::cFowlerNollVoOffsetBasis;
		for (auto& e : k)
		{
			hash ^= e;
			hash *= SGFramework::Hash::cFowlerNollVoPrime;
		}
		return hash;
	}
};
#endif // !SGFRAMEWORK_HEADER_SSF_STRING_HPP_