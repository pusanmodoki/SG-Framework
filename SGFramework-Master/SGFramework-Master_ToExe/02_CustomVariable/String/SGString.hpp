/*----------------------------------------------------------------------------------
std::wstring���g������sgstring class
SGFramework�̕�����^��sgstring�œ��ꂳ���
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
	//std::wstring���g������sgstring class
	//SGFramework�̕�����^��sgstring�œ��ꂳ���
	class sgstring final : public std::wstring
	{
	public:
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�u""�v���Z�b�g
		inline sgstring() : my_functions(*this) , std::wstring(L""){}
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//������������Z�b�g, wchar�`���ɕϊ����s���̂ŏd��
		//����1: char������
		inline sgstring(const char* string) : my_functions(*this), std::wstring(to_unicode(string)) {}
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//������������Z�b�g
		//����1: wchar������
		inline sgstring(const wchar* string) : my_functions(*this), std::wstring(string) {}
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//������������Z�b�g, wchar�`���ɕϊ����s���̂ŏd��
		//����1: string������
		inline sgstring(const std::string& string) : my_functions(*this), std::wstring(to_unicode(string)) {}
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//������������Z�b�g
		//����1: wstring������
		inline sgstring(const std::wstring& string) : my_functions(*this), std::wstring(string) {}
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//������������Z�b�g
		//����1: sgstring������
		inline sgstring(const sgstring& string) : my_functions(*this), std::wstring(string) {}
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//������������Z�b�g
		//����1: sgstring������
		inline sgstring(sgstring&& string) noexcept : my_functions(*this), std::wstring(std::move(string)) {}
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//���������𕶎���ϊ����ăZ�b�g
		//����1: int����
		//����2: 16�i���\�L�ɂ��邩�ۂ�, default = false
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
		//[�R���X�g���N�^]
		//���������𕶎���ϊ����ăZ�b�g
		//����1: uint����
		//����2: 16�i���\�L�ɂ��邩�ۂ�, default = false
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
		//[�R���X�g���N�^]
		//���������𕶎���ϊ����ăZ�b�g
		//����1: long����
		//����2: 16�i���\�L�ɂ��邩�ۂ�, default = false
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
		//[�R���X�g���N�^]
		//���������𕶎���ϊ����ăZ�b�g
		//����1: ulong����
		//����2: 16�i���\�L�ɂ��邩�ۂ�, default = false
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
		//[�R���X�g���N�^]
		//���������𕶎���ϊ����ăZ�b�g
		//����1: int64����
		//����2: 16�i���\�L�ɂ��邩�ۂ�, default = false
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
		//[�R���X�g���N�^]
		//���������𕶎���ϊ����ăZ�b�g
		//����1: uint64����
		//����2: 16�i���\�L�ɂ��邩�ۂ�, default = false
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
		//[�R���X�g���N�^]
		//���������𕶎���ϊ����ăZ�b�g
		//����1: float����
		inline sgstring(float value) : my_functions(*this),  std::wstring(std::to_wstring(value)) {}
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//���������𕶎���ϊ����ăZ�b�g
		//����1: float����
		inline sgstring(double value) : my_functions(*this), std::wstring(std::to_wstring(value)) {}

		//----------------------------------------------------------------------------------
		//[to_unicode]
		//char�������wchar�`���ɕϊ�
		//return: �ϊ�������
		//����1: char������
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
		//char�������wchar�`���ɕϊ�
		//return: �ϊ�������
		//����1: char������
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
		//���������𕶎���ϊ�����
		//return: �ϊ�������
		//����1: int����
		inline static sgstring to_sgstring(int value) { return std::to_wstring(value); }
		//----------------------------------------------------------------------------------
		//[to_sgstring]
		//���������𕶎���ϊ�����
		//return: �ϊ�������
		//����1: uint����
		inline static sgstring to_sgstring(uint value) { return std::to_wstring(value); }
		//----------------------------------------------------------------------------------
		//[to_sgstring]
		//���������𕶎���ϊ�����
		//return: �ϊ�������
		//����1: long����
		inline static sgstring to_sgstring(long value) { return std::to_wstring(value); }
		//----------------------------------------------------------------------------------
		//[to_sgstring]
		//���������𕶎���ϊ�����
		//return: �ϊ�������
		//����1: ulong����
		inline static sgstring to_sgstring(ulong value) { return std::to_wstring(value); }
		//----------------------------------------------------------------------------------
		//[to_sgstring]
		//���������𕶎���ϊ�����
		//return: �ϊ�������
		//����1: int64����
		inline static sgstring to_sgstring(int64 value) { return std::to_wstring(value); }
		//----------------------------------------------------------------------------------
		//[to_sgstring]
		//���������𕶎���ϊ�����
		//return: �ϊ�������
		//����1: uint64����
		inline static sgstring to_sgstring(uint64 value) { return std::to_wstring(value); }
		//----------------------------------------------------------------------------------
		//[to_sgstring]
		//���������𕶎���ϊ�����
		//return: �ϊ�������
		//����1: float����
		inline static sgstring to_sgstring(float value) { return std::to_wstring(value); }
		//----------------------------------------------------------------------------------
		//[to_sgstring]
		//���������𕶎���ϊ�����
		//return: �ϊ�������
		//����1: double����
		inline static sgstring to_sgstring(double value) { return std::to_wstring(value); }


		//----------------------------------------------------------------------------------
		//[to_std_string]
		//���g��std::2string�ɕϊ�
		//return: �ϊ�������
		inline std::wstring to_std_wstring() const
		{
			return *this;
		}
		//----------------------------------------------------------------------------------
		//[to_std_string]
		//���g��std::string�ɕϊ�
		//return: �ϊ�������
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
		//= operator(�Ȃ�ׂ�wcahr���g������)
		inline std::wstring  operator = (const char* string) { return (this->assign(to_unicode(string))); }
		//= operator(�Ȃ�ׂ�wcahr���g������)
		inline std::wstring  operator = (const wchar* string) { return (this->assign(string)); }
		//= operator(�Ȃ�ׂ�wcahr���g������)
		inline std::wstring  operator = (const wchar string) { this->clear(); this->push_back(string); return *this; }
		//= operator(�Ȃ�ׂ�wcahr���g������)
		inline std::wstring  operator = (const std::string& string) { return (this->assign(to_unicode(string))); }
		//= operator(�Ȃ�ׂ�wcahr���g������)
		inline std::wstring  operator = (const std::wstring& string) { return (this->assign(string)); }
		//= operator(�Ȃ�ׂ�wcahr���g������)
		inline std::wstring  operator = (const sgstring& string) { return (this->assign(string)); }
		//= operator(�Ȃ�ׂ�wcahr���g������)
		inline std::wstring  operator = (int value) { return (this->assign(std::to_wstring(value))); }
		//= operator(�Ȃ�ׂ�wcahr���g������)
		inline std::wstring  operator = (uint value) { return (this->assign(std::to_wstring(value))); }
		//= operator(�Ȃ�ׂ�wcahr���g������)
		inline std::wstring  operator = (long value) { return (this->assign(std::to_wstring(value))); }
		//= operator(�Ȃ�ׂ�wcahr���g������)
		inline std::wstring  operator = (ulong value) { return (this->assign(std::to_wstring(value))); }
		//= operator(�Ȃ�ׂ�wcahr���g������)
		inline std::wstring  operator = (int64 value) { return (this->assign(std::to_wstring(value))); }
		//= operator(�Ȃ�ׂ�wcahr���g������)
		inline std::wstring  operator = (uint64 value) { return (this->assign(std::to_wstring(value))); }
		//= operator(�Ȃ�ׂ�wcahr���g������)
		inline std::wstring  operator = (float value) { return (this->assign(std::to_wstring(value))); }
		//= operator(�Ȃ�ׂ�wcahr���g������)
		inline std::wstring  operator = (double value) { return (this->assign(std::to_wstring(value))); }

		//+ operator(wchar�`���ɕϊ����s���̂ŏd��)
		inline std::wstring  operator + (const char* string) const { return (*this + to_unicode(string)); }
		//+ operator(wchar�`���ɕϊ����s���̂ŏd��)
		inline std::wstring  operator + (const std::string& string) const { return (*this + to_unicode(string)); }
		//+ operator
		inline std::wstring  operator + (wchar string) const { wchar add[] = { string, L'\0' }; return (*this + string); }

		//+= operator(wchar�`���ɕϊ����s���̂ŏd��)
		inline std::wstring operator += (const char* string) { return (this->append(to_unicode(string))); }
		//+= operator(wchar�`���ɕϊ����s���̂ŏd��)
		inline std::wstring operator += (const std::string& string) { return (this->append(to_unicode(string))); }
		//+= operator
		inline std::wstring operator += (const wchar* string) { return (this->append(string)); }
		//+= operator
		inline std::wstring operator += (const wchar string) { this->push_back(string); return *this; }
		//+= operator
		inline std::wstring operator += (const std::wstring& string) { return (this->append(string)); }
		//+= operator
		inline std::wstring operator += (const sgstring& string) { return (this->append(string)); }

		//+ operator(wchar�`���ɕϊ����s���̂ŏd��)
		inline std::wstring operator + (int value) const { return *this + std::to_wstring(value); }
		//+ operator(wchar�`���ɕϊ����s���̂ŏd��)
		inline std::wstring operator + (uint value) const { return *this + std::to_wstring(value); };
		//+ operator(wchar�`���ɕϊ����s���̂ŏd��)
		inline std::wstring operator + (long value) const { return *this + std::to_wstring(value); };
		//+ operator(wchar�`���ɕϊ����s���̂ŏd��)
		inline std::wstring operator + (ulong value) const { return *this + std::to_wstring(value); };
		//+ operator(wchar�`���ɕϊ����s���̂ŏd��)
		inline std::wstring operator + (int64 value) const { return *this + std::to_wstring(value); };
		//+ operator(wchar�`���ɕϊ����s���̂ŏd��)
		inline std::wstring operator + (uint64 value) const { return *this + std::to_wstring(value); };
		//+ operator(wchar�`���ɕϊ����s���̂ŏd��)
		inline std::wstring operator + (float value) const { return *this + std::to_wstring(value); };
		//+ operator(wchar�`���ɕϊ����s���̂ŏd��)
		inline std::wstring operator + (double value) const { return *this + std::to_wstring(value); };

		//+= operator(wchar�`���ɕϊ����s���̂ŏd��)
		inline std::wstring operator += (int value) { return (this->append(std::to_wstring(value))); }
		//+= operator(wchar�`���ɕϊ����s���̂ŏd��)
		inline std::wstring operator += (uint value) { return (this->append(std::to_wstring(value))); }
		//+= operator(wchar�`���ɕϊ����s���̂ŏd��)
		inline std::wstring operator += (long value) { return (this->append(std::to_wstring(value))); }
		//+= operator(wchar�`���ɕϊ����s���̂ŏd��)
		inline std::wstring operator += (ulong value) { return (this->append(std::to_wstring(value))); }
		//+= operator(wchar�`���ɕϊ����s���̂ŏd��)
		inline std::wstring operator += (int64 value) { return (this->append(std::to_wstring(value))); }
		//+= operator(wchar�`���ɕϊ����s���̂ŏd��)
		inline std::wstring operator += (uint64 value) { return (this->append(std::to_wstring(value))); }
		//+= operator(wchar�`���ɕϊ����s���̂ŏd��)
		inline std::wstring operator += (float value) { return (this->append(std::to_wstring(value))); }
		//+= operator(wchar�`���ɕϊ����s���̂ŏd��)
		inline std::wstring operator += (double value) { return (this->append(std::to_wstring(value))); }

		//��= operator(wchar�`���ɕϊ����s���̂ŏd��)
		inline bool operator == (const char* string) const { return (*this == to_unicode(string)); }
		//��= operator(wchar�`���ɕϊ����s���̂ŏd��)
		inline bool operator == (const std::string& string) const { return (*this == to_unicode(string)); }
		
		//!= operator(wchar�`���ɕϊ����s���̂ŏd��)
		inline bool operator != (const char* string) const { return (*this != to_unicode(string)); }
		//!= operator(wchar�`���ɕϊ����s���̂ŏd��)
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
				//�������dellimiter����؂�Ƃ��ĕ�������
				//return: ���������������������vector
				//����1: �Ώە�����
				//����2 : delimiter = Default: ','
				//����3 : \0�����Ō�̗v�f��\n�������ꍇtrue�Ȃ�폜 = Default: false 
				inline static std::vector<sgstring> Split(const sgstring& string, wchar delimiter = L',', bool isNewLineDelete = false)
				{
					if (string.empty()) return std::vector<sgstring>();

					std::vector<sgstring> ret;						//�Ԃ�l
					std::wstring getString = string;				//��������string
					std::wstring buf;									//getline�p�̃o�b�t�@
					std::wstringstream sStream(getString);	//getline�p��stream

					//���s�폜 = true �ōŌ�̕��������s�Ȃ�폜
					if (isNewLineDelete && getString[getString.size() - 1] == L'\n')
						getString.erase(getString.size() - 1);

					//��؂蕶�������Ƃɗv�f�擾���s���v�f��string vector�ɒǉ�
					while (std::getline(sStream, buf, delimiter))
						ret.emplace_back(buf);

					return std::move(ret);
				}
				//----------------------------------------------------------------------------------
				//[Split]
				//�������dellimiter����؂�Ƃ��ĕ�������
				//����1: ���������������������vector
				//����2: �Ώە�����
				//����3: delimiter = Default: ','
				//����4: \0�����Ō�̗v�f��\n�������ꍇtrue�Ȃ�폜 = Default: false 
				inline static void Split(std::vector<sgstring>& write, const sgstring& string, wchar delimiter = L',', bool isNewLineDelete = false)
				{
					std::wstring getString = string;				//��������string
					std::wstring buf;									//getline�p�̃o�b�t�@
					std::wstringstream sStream(getString);	//getline�p��stream

					//���s�폜 = true �ōŌ�̕��������s�Ȃ�폜
					if (isNewLineDelete && getString[getString.size() - 1] == L'\n')
						getString.erase(getString.size() - 1);

					//��؂蕶�������Ƃɗv�f�擾���s���v�f��string vector�ɒǉ�
					while (std::getline(sStream, buf, delimiter))
						write.emplace_back(buf);
				}

				//----------------------------------------------------------------------------------
				//[Join]
				//joinstring��delimiter����؂�Ƃ��A������
				//return: �A������sgstring
				//����1: �Ώە�����vector
				//����2 : delimiter = Default: ','
				//����3: ���ʂ̍Ō�ɉ��s��ǉ����邩
				inline static sgstring Join(const std::vector<sgstring>& joinString, wchar delimiter, bool isNewLine)
				{
					sgstring result;	 //�Ԃ�l
					for (const auto& e : joinString)
					{
						result.append(e);
						result.push_back(delimiter);
					}

					//ret���v�f����Ȃ�Ō�̋�؂蕶���폜
					if (result.size())		result.erase(result.end() - 1);
					//ret���v�f���� & ���ʂ̍Ō�ɉ��s = true�Ȃ�Ō�ɉ��s�ǉ�
					if (result.size() && isNewLine) result.push_back(L'\n');
					return std::move(result);
				}
				//----------------------------------------------------------------------------------
				//[Join]
				//joinstring��delimiter����؂�Ƃ��A������
				//����1: �A���������sgstring
				//����2: �Ώە�����vector
				//����3: delimiter = Default: ','
				//����4: ���ʂ̍Ō�ɉ��s��ǉ����邩
				inline static void Join(sgstring& write, const std::vector<sgstring>& joinString, wchar delimiter, bool isNewLine)
				{
					for (const auto& e : joinString)
					{
						write.append(e);
						write.push_back(delimiter);
					}

					//ret���v�f����Ȃ�Ō�̋�؂蕶���폜
					if (write.size())		write.erase(write.end() - 1);
					//ret���v�f���� & ���ʂ̍Ō�ɉ��s = true�Ȃ�Ō�ɉ��s�ǉ�
					if (write.size() && isNewLine) write.push_back(L'\n');
				}

				//----------------------------------------------------------------------------------
				//[ToUnicode]
				//char�������wchar�`���ɕϊ�
				//return: �ϊ�������
				//����1: char������
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
				//char�������wchar�`���ɕϊ�
				//return: �ϊ�������
				//����1: char������
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
				//���g�̕�����̐擪����string����������, IndexOf == find
				//return: �������ꍇ�͐擪�̓Y��, �Ȃ����npos
				//����1: �Ώە�����
				//����2: ����������
				inline static size_t IndexOf(const sgstring& string1, const sgstring& string2) { return string1.find_first_of(string2); }
				//----------------------------------------------------------------------------------
				//[IndexFirstOf]
				//���g�̕�����̐擪����string����������, IndexFirstOf == find
				//return: �������ꍇ�͐擪�̓Y��, �Ȃ����npos
				//����1: �Ώە�����
				//����2: ����������
				inline static size_t IndexFirstOf(const sgstring& string1, const sgstring& string2) { return string1.find(string2); }
				//----------------------------------------------------------------------------------
				//[IndexLastOf]
				//���g�̕�����̍Ōォ��string����������, IndexLastOf == rfind
				//return: �������ꍇ�͐擪�̓Y��, �Ȃ����npos
				//����1: �Ώە�����
				//����2: ����������
				inline static size_t IndexLastOf(const sgstring& string1, const sgstring& string2) { return string1.rfind(string2); }
				//----------------------------------------------------------------------------------
				//[IsStartWith]
				//���g�̕�����̍ŏ���string���r����
				//return: ���g�̕�����string����n�܂��Ă����true
				//����1: �Ώە�����
				//����2: ����������
				inline static bool IsStartWith(const sgstring& string1, const sgstring& string2) { return (string1.length() >= string2.length() && std::equal(string2.begin(), string2.end(), string1.begin())); }
				//----------------------------------------------------------------------------------
				//[IsEndWith]
				//���g�̕�����̍Ō��string���r����
				//return: ���g�̕�����string�ŏI����Ă����true
				//����1: �Ώە�����
				//����2: ����������
				inline static bool IsEndWith(const sgstring& string1, const sgstring& string2) { return (string1.length() >= string2.length() && std::equal(string2.rbegin(), string2.rend(), string1.rbegin())); }
				//----------------------------------------------------------------------------------
				//[IsContains]
				//���g�̕������string�����邩�m�F����
				//return: ���g�̕������string���܂܂�Ă����true
				//����1: �Ώە�����
				//����2: ����������
				inline static bool IsContains(const sgstring& string1, const sgstring& string2) {
					auto result = (string1.find(string2));
					return (result >= 0 && result != sgstring::npos);
				}
				//----------------------------------------------------------------------------------
				//[IsFirstOfNumbers]
				//string�̕�����̐擪�����������񂩊m�F����
				//return: string�̐擪�����������Ȃ��true
				//����1: �Ώە�����
				//����2: �󔒕������擪�ɂ���A���̌オ����������̏ꍇ�󔒍폜����true?, default = true
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
				//�����������16�i���\�L�ɕϊ�����
				//throw: �����ɕϊ��ł��Ȃ������ꍇ
				//return: �ϊ�����������
				//����1: �Ώە�����
				static sgstring ToX16(const sgstring& string);
				//----------------------------------------------------------------------------------
				//[ToX10]
				//�����������10�i���\�L�ɕϊ�����
				//throw: �����ɕϊ��ł��Ȃ������ꍇ
				//return: �ϊ�����������
				//����1: �Ώە�����
				static sgstring ToX10(const sgstring& string);
				//----------------------------------------------------------------------------------
				//[ToX8]
				//�����������8�i���\�L�ɕϊ�����
				//throw: �����ɕϊ��ł��Ȃ������ꍇ
				//return: �ϊ�����������
				//����1: �Ώە�����
				static sgstring ToX8(const sgstring& string);
				//----------------------------------------------------------------------------------
				//[ToX2]
				//�����������2�i���\�L�ɕϊ�����
				//throw: �����ɕϊ��ł��Ȃ������ꍇ
				//return: �ϊ�����������
				//����1: �Ώە�����
				static sgstring ToX2(const sgstring& string);

				//----------------------------------------------------------------------------------
				//[ToX16String]
				//������16�i���\�L������ɕϊ�����
				//return: �ϊ�����������
				//����1: �Ώې���
				inline static sgstring ToX16String(int value)
				{
					std::wstringstream stream;
					stream << std::showbase;
					stream << std::hex << value;
					return stream.str();
				}
				//----------------------------------------------------------------------------------
				//[ToX16String]
				//������16�i���\�L������ɕϊ�����
				//return: �ϊ�����������
				//����1: �Ώې���
				inline static sgstring ToX16String(uint value)
				{
					std::wstringstream stream;
					stream << std::showbase;
					stream << std::hex << value;
					return stream.str();
				}
				//----------------------------------------------------------------------------------
				//[ToX16String]
				//������16�i���\�L������ɕϊ�����
				//return: �ϊ�����������
				//����1: �Ώې���
				inline static sgstring ToX16String(long value)
				{
					std::wstringstream stream;
					stream << std::showbase;
					stream << std::hex << value;
					return stream.str();
				}
				//----------------------------------------------------------------------------------
				//[ToX16String]
				//������16�i���\�L������ɕϊ�����
				//return: �ϊ�����������
				//����1: �Ώې���
				inline static sgstring ToX16String(ulong value)
				{
					std::wstringstream stream;
					stream << std::showbase;
					stream << std::hex << value;
					return stream.str();
				}
				//----------------------------------------------------------------------------------
				//[ToX16String]
				//������16�i���\�L������ɕϊ�����
				//return: �ϊ�����������
				//����1: �Ώې���
				inline static sgstring ToX16String(int64 value)
				{
					std::wstringstream stream;
					stream << std::showbase;
					stream << std::hex << value;
					return stream.str();
				}
				//----------------------------------------------------------------------------------
				//[ToX16String]
				//������16�i���\�L������ɕϊ�����
				//return: �ϊ�����������
				//����1: �Ώې���
				inline static sgstring ToX16String(uint64 value)
				{
					std::wstringstream stream;
					stream << std::showbase;
					stream << std::hex << value;
					return stream.str();
				}

				//----------------------------------------------------------------------------------
				//[ToX8String]
				//������8�i���\�L������ɕϊ�����
				//return: �ϊ�����������
				//����1: �Ώې���
				inline static sgstring ToX8String(int value)
				{
					std::wstringstream stream;
					stream << std::showbase;
					stream << std::oct << value;
					return stream.str();
				}
				//----------------------------------------------------------------------------------
				//[ToX8String]
				//������8�i���\�L������ɕϊ�����
				//return: �ϊ�����������
				//����1: �Ώې���
				inline static sgstring ToX8String(uint value)
				{
					std::wstringstream stream;
					stream << std::showbase;
					stream << std::oct << value;
					return stream.str();
				}
				//----------------------------------------------------------------------------------
				//[ToX8String]
				//������8�i���\�L������ɕϊ�����
				//return: �ϊ�����������
				//����1: �Ώې���
				inline static sgstring ToX8String(long value)
				{
					std::wstringstream stream;
					stream << std::showbase;
					stream << std::oct << value;
					return stream.str();
				}
				//----------------------------------------------------------------------------------
				//[ToX8String]
				//������8�i���\�L������ɕϊ�����
				//return: �ϊ�����������
				//����1: �Ώې���
				inline static sgstring ToX8String(ulong value)
				{
					std::wstringstream stream;
					stream << std::showbase;
					stream << std::oct << value;
					return stream.str();
				}
				//----------------------------------------------------------------------------------
				//[ToX8String]
				//������8�i���\�L������ɕϊ�����
				//return: �ϊ�����������
				//����1: �Ώې���
				inline static sgstring ToX8String(int64 value)
				{
					std::wstringstream stream;
					stream << std::showbase;
					stream << std::oct << value;
					return stream.str();
				}
				//----------------------------------------------------------------------------------
				//[ToX8String]
				//������8�i���\�L������ɕϊ�����
				//return: �ϊ�����������
				//����1: �Ώې���
				inline static sgstring ToX8String(uint64 value)
				{
					std::wstringstream stream;
					stream << std::showbase;
					stream << std::oct << value;
					return stream.str();
				}

				//----------------------------------------------------------------------------------
				//[ToX2String]
				//������2�i���\�L������ɕϊ�����
				//return: �ϊ�����������
				//����1: �Ώې���
				inline static sgstring ToX2String(int value)
				{
					std::wstringstream stream;
					stream << std::showbase;
					stream << std::bitset<sizeof(value) * 8>(value);
					return stream.str();
				}
				//----------------------------------------------------------------------------------
				//[ToX2String]
				//������2�i���\�L������ɕϊ�����
				//return: �ϊ�����������
				//����1: �Ώې���
				inline static sgstring ToX2String(uint value)
				{
					std::wstringstream stream;
					stream << std::showbase;
					stream << std::bitset<sizeof(value) * 8>(value);
					return stream.str();
				}
				//----------------------------------------------------------------------------------
				//[ToX2String]
				//������2�i���\�L������ɕϊ�����
				//return: �ϊ�����������
				//����1: �Ώې���
				inline static sgstring ToX2String(long value)
				{
					std::wstringstream stream;
					stream << std::showbase;
					stream << std::bitset<sizeof(value) * 8>(value);
					return stream.str();
				}
				//----------------------------------------------------------------------------------
				//[ToX2String]
				//������2�i���\�L������ɕϊ�����
				//return: �ϊ�����������
				//����1: �Ώې���
				inline static sgstring ToX2String(ulong value)
				{
					std::wstringstream stream;
					stream << std::showbase;
					stream << std::bitset<sizeof(value) * 8>(value);
					return stream.str();
				}
				//----------------------------------------------------------------------------------
				//[ToX2String]
				//������2�i���\�L������ɕϊ�����
				//return: �ϊ�����������
				//����1: �Ώې���
				inline static sgstring ToX2String(int64 value)
				{
					std::wstringstream stream;
					stream << std::showbase;
					stream << std::bitset<sizeof(value) * 8>(value);
					return stream.str();
				}
				//----------------------------------------------------------------------------------
				//[ToX2String]
				//������2�i���\�L������ɕϊ�����
				//return: �ϊ�����������
				//����1: �Ώې���
				inline static sgstring ToX2String(uint64 value)
				{
					std::wstringstream stream;
					stream << std::showbase;
					stream << std::bitset<sizeof(value) * 8>(value);
					return stream.str();
				}


				//----------------------------------------------------------------------------------
				//[ToInt]
				//�������int�ɕϊ�����
				//throw: �ϊ��ł��Ȃ������ꍇ
				//return: �ϊ������l
				//����1: �Ώە�����
				static int ToInt(const sgstring& string);
				//----------------------------------------------------------------------------------
				//[ToLong]
				//�������long�ɕϊ�����
				//throw: �ϊ��ł��Ȃ������ꍇ
				//return: �ϊ������l
				//����1: �Ώە�����
				static long ToLong(const sgstring& string);
				//----------------------------------------------------------------------------------
				//[ToUlong]
				//�������ulong�ɕϊ�����
				//throw: �ϊ��ł��Ȃ������ꍇ
				//return: �ϊ������l
				//����1: �Ώە�����
				static ulong ToUlong(const sgstring& string);
				//----------------------------------------------------------------------------------
				//[ToInt64]
				//�������int64�ɕϊ�����
				//throw: �ϊ��ł��Ȃ������ꍇ
				//return: �ϊ������l
				//����1: �Ώە�����
				static int64 ToInt64(const sgstring& string);
				//----------------------------------------------------------------------------------
				//[ToUint64]
				//�������uint64�ɕϊ�����
				//throw: �ϊ��ł��Ȃ������ꍇ
				//return: �ϊ������l
				//����1: �Ώە�����
				static uint64 ToUint64(const sgstring& string);
				//----------------------------------------------------------------------------------
				//[ToFloat]
				//�������float�ɕϊ�����
				//throw: �ϊ��ł��Ȃ������ꍇ
				//return: �ϊ������l
				//����1: �Ώە�����
				static float ToFloat(const sgstring& string);
				//----------------------------------------------------------------------------------
				//[ToDouble]
				//�������double�ɕϊ�����
				//throw: �ϊ��ł��Ȃ������ꍇ
				//return: �ϊ������l
				//����1: �Ώە�����
				static double ToDouble(const sgstring& string);


				//----------------------------------------------------------------------------------
				//[ToString]
				//������string�ɕϊ�����
				//return: �ϊ�����������
				//����1: �Ώې���
				inline static sgstring ToString(int value) { return std::to_wstring(value); }
				//----------------------------------------------------------------------------------
				//[ToString]
				//������string�ɕϊ�����
				//return: �ϊ�����������
				//����1: �Ώې���
				inline static sgstring ToString(uint value) { return std::to_wstring(value); }
				//----------------------------------------------------------------------------------
				//[ToString]
				//������string�ɕϊ�����
				//return: �ϊ�����������
				//����1: �Ώې���
				inline static sgstring ToString(long value) { return std::to_wstring(value); }
				//----------------------------------------------------------------------------------
				//[ToString]
				//������string�ɕϊ�����
				//return: �ϊ�����������
				//����1: �Ώې���
				inline static sgstring ToString(ulong value) { return std::to_wstring(value); }
				//----------------------------------------------------------------------------------
				//[ToString]
				//������string�ɕϊ�����
				//return: �ϊ�����������
				//����1: �Ώې���
				inline static sgstring ToString(int64 value) { return std::to_wstring(value); }
				//----------------------------------------------------------------------------------
				//[ToString]
				//������string�ɕϊ�����
				//return: �ϊ�����������
				//����1: �Ώې���
				inline static sgstring ToString(uint64 value) { return std::to_wstring(value); }
				//----------------------------------------------------------------------------------
				//[ToString]
				//������string�ɕϊ�����
				//return: �ϊ�����������
				//����1: �Ώې���
				inline static sgstring ToString(float value) { return std::to_wstring(value); }
				//----------------------------------------------------------------------------------
				//[ToString]
				//������string�ɕϊ�����
				//return: �ϊ�����������
				//����1: �Ώې���
				inline static sgstring ToString(double value) { return std::to_wstring(value); }


				//----------------------------------------------------------------------------------
				//[ToArrayInt]
				//�������delimiter�ŕ�������vector�ɕϊ�����
				//return: �ϊ�����vector, �����Ȃ�size = 0
				//����1: �Ώە�����
				//����2: delimiter = default L','
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
				//�������delimiter�ŕ�������vector�ɕϊ�����
				//return: �ϊ�����vector, �����Ȃ�size = 0
				//����1: �Ώە�����
				//����2: delimiter = default L','
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
				//�������delimiter�ŕ�������vector�ɕϊ�����
				//return: �ϊ�����vector, �����Ȃ�size = 0
				//����1: �Ώە�����
				//����2: delimiter = default L','
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
				//�������delimiter�ŕ�������vector�ɕϊ�����
				//return: �ϊ�����vector, �����Ȃ�size = 0
				//����1: �Ώە�����
				//����2: delimiter = default L','
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
				//�������delimiter�ŕ�������vector�ɕϊ�����
				//return: �ϊ�����vector, �����Ȃ�size = 0
				//����1: �Ώە�����
				//����2: delimiter = default L','
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
				//�������delimiter�ŕ�������vector�ɕϊ�����
				//return: �ϊ�����vector, �����Ȃ�size = 0
				//����1: �Ώە�����
				//����2: delimiter = default L','
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
				//�������delimiter�ŕ�������vector�ɕϊ�����
				//return: �ϊ�����vector, �����Ȃ�size = 0
				//����1: �Ώە�����
				//����2: delimiter = default L','
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

		//string�ǉ��֐��S���̔�
		class MyFunctions
		{
		public:
			//----------------------------------------------------------------------------------
			//[�R���X�g���N�^]
			//�Q�Ƃ��Z�b�g
			//����1: this
			inline MyFunctions(sgstring& string) : m_string(string) { }

			//----------------------------------------------------------------------------------
			//[Clear]
			//������N���A, Clear() == wstring::clear()
			inline void Clear() { m_string.clear(); }
			//----------------------------------------------------------------------------------
			//[Split]
			//���g�̕������dellimiter����؂�Ƃ��ĕ�������
			//return: ���������������������vector
			//����1 : delimiter = Default: ','
			//����2 : \0�����Ō�̗v�f��\n�������ꍇtrue�Ȃ�폜 = Default: false 
			inline std::vector<sgstring> Split(wchar delimiter = L',', bool isNewLineDelete = false) const
			{
				std::vector<sgstring> ret;						//�Ԃ�l
				std::wstring getString = m_string;			//��������string
				std::wstring buf;									//getline�p�̃o�b�t�@
				std::wstringstream sStream(getString);	//getline�p��stream

				//���s�폜 = true �ōŌ�̕��������s�Ȃ�폜
				if (isNewLineDelete && getString[getString.size() - 1] == L'\n')
					getString.erase(getString.size() - 1);

				//��؂蕶�������Ƃɗv�f�擾���s���v�f��string vector�ɒǉ�
				while (std::getline(sStream, buf, delimiter))
					ret.emplace_back(buf);

				return std::move(ret);
			}
			//----------------------------------------------------------------------------------
			//[Split]
			//���g�̕������dellimiter����؂�Ƃ��ĕ�������
			//����1: ���������������������vector
			//����2: delimiter = Default: ','
			//����3: \0�����Ō�̗v�f��\n�������ꍇtrue�Ȃ�폜 = Default: false 
			inline void Split(std::vector<sgstring>& write, wchar delimiter = L',', bool isNewLineDelete = false) const
			{
				if (this->IsEmpty()) return;

				std::wstring getString = m_string;			//��������string
				std::wstring buf;									//getline�p�̃o�b�t�@
				std::wstringstream sStream(getString);	//getline�p��stream

				//�N���A
				write.clear();

				//���s�폜 = true �ōŌ�̕��������s�Ȃ�폜
				if (isNewLineDelete && getString[getString.size() - 1] == L'\n')
					getString.erase(getString.size() - 1);

				//��؂蕶�������Ƃɗv�f�擾���s���v�f��string vector�ɒǉ�
				while (std::getline(sStream, buf, delimiter))
					write.emplace_back(buf);
			}
			//----------------------------------------------------------------------------------
			//[IndexOf]
			//���g�̕�����̐擪����string����������, IndexOf == find
			//return: �������ꍇ�͐擪�̓Y��, �Ȃ����-1
			//����1: ����������
			inline size_t IndexOf(const sgstring& string) const { return m_string.find(string); }
			//----------------------------------------------------------------------------------
			//[IndexFirstOf]
			//���g�̕�����̐擪����string����������, IndexFirstOf == find
			//return: �������ꍇ�͐擪�̓Y��, �Ȃ����-1
			//����1: ����������
			inline size_t IndexFirstOf(const sgstring& string) const { return m_string.find(string); }
			//----------------------------------------------------------------------------------
			//[IndexLastOf]
			//���g�̕�����̍Ōォ��string����������, IndexLastOf == rfind
			//return: �������ꍇ�͐擪�̓Y��, �Ȃ����-1
			//����1: ����������
			inline size_t IndexLastOf(const sgstring& string) const { return m_string.rfind(string); }
			//----------------------------------------------------------------------------------
			//[IsStartWith]
			//���g�̕�����̍ŏ���string���r����
			//return: ���g�̕�����string����n�܂��Ă����true
			//����1: ����������
			inline bool IsStartWith(const sgstring& string) const { return (m_string.length() >= string.length() && std::equal(string.begin(), string.end(), m_string.begin())); }
			//----------------------------------------------------------------------------------
			//[IsEndWith]
			//���g�̕�����̍Ō��string���r����
			//return: ���g�̕�����string�ŏI����Ă����true
			//����1: ����������
			inline bool IsEndWith(const sgstring& string) const { return (m_string.length() >= string.length() && std::equal(string.rbegin(), string.rend(), m_string.rbegin())); }
			//----------------------------------------------------------------------------------
			//[IsContains]
			//���g�̕������string�����邩�m�F����
			//return: ���g�̕������string���܂܂�Ă����true
			//����1: ����������
			inline bool IsContains(const sgstring& string) const {
				auto result = (m_string.find(string));
				return (result >= 0 && result != sgstring::npos); }
			//----------------------------------------------------------------------------------
			//[IsEmpty]
			//���g���󂩊m�F����, IsEnpty == empty
			//return: ��Ȃ�true
			inline bool IsEmpty() const { return m_string.empty(); }
			//----------------------------------------------------------------------------------
			//[IsFirstOfNumbers]
			//���g�̂̕�����̐擪�����������񂩊m�F����
			//����1: �󔒕������擪�ɂ���A���̌オ����������̏ꍇ�󔒍폜����true?, default = true
			//return: ���g�̂̐擪�����������Ȃ��true
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
			//return: �g���q�𔭌������ꍇ�͊g���q( . �܂�), ������Ȃ���� ""
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
			//�����������16�i���\�L�ɕϊ�����
			//throw: �����ɕϊ��ł��Ȃ������ꍇ
			//return: �ϊ�����������
			sgstring ToX16() const;
			//----------------------------------------------------------------------------------
			//[ToX10]
			//�����������10�i���\�L�ɕϊ�����
			//throw: �����ɕϊ��ł��Ȃ������ꍇ
			//return: �ϊ�����������
			sgstring ToX10() const;
			//----------------------------------------------------------------------------------
			//[ToX8]
			//�����������8�i���\�L�ɕϊ�����
			//throw: �����ɕϊ��ł��Ȃ������ꍇ
			//return: �ϊ�����������
			sgstring ToX8() const;
			//----------------------------------------------------------------------------------
			//[ToX2]
			//�����������2�i���\�L�ɕϊ�����
			//throw: �����ɕϊ��ł��Ȃ������ꍇ
			//return: �ϊ�����������
			sgstring ToX2() const;

			//----------------------------------------------------------------------------------
			//[ToInt]
			//�������int�ɕϊ�����
			//throw: �ϊ��ł��Ȃ������ꍇ
			//return: �ϊ������l
			int ToInt() const;
			//----------------------------------------------------------------------------------
			//[ToLong]
			//�������long�ɕϊ�����
			//throw: �ϊ��ł��Ȃ������ꍇ
			//return: �ϊ������l
			long ToLong() const;
			//----------------------------------------------------------------------------------
			//[ToUlong]
			//�������ulong�ɕϊ�����
			//throw: �ϊ��ł��Ȃ������ꍇ
			//return: �ϊ������l
			ulong ToUlong() const;
			//----------------------------------------------------------------------------------
			//[ToInt64]
			//�������int64�ɕϊ�����
			//throw: �ϊ��ł��Ȃ������ꍇ
			//return: �ϊ������l
			int64 ToInt64() const;
			//----------------------------------------------------------------------------------
			//[ToUint64]
			//�������uint64�ɕϊ�����
			//throw: �ϊ��ł��Ȃ������ꍇ
			//return: �ϊ������l
			uint64 ToUint64() const;
			//----------------------------------------------------------------------------------
			//[ToFloat]
			//�������float�ɕϊ�����
			//throw: �ϊ��ł��Ȃ������ꍇ
			//return: �ϊ������l
			float ToFloat() const;
			//----------------------------------------------------------------------------------
			//[ToDouble]
			//�������double�ɕϊ�����
			//throw: �ϊ��ł��Ȃ������ꍇ
			//return: �ϊ������l
			double ToDouble() const;

			//----------------------------------------------------------------------------------
			//[ToArrayInt]
			//�������delimiter�ŕ�������vector�ɕϊ�����
			//return: �ϊ�����vector, �����Ȃ�size = 0
			//����1: delimiter = default L','
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
			//�������delimiter�ŕ�������vector�ɕϊ�����
			//return: �ϊ�����vector, �����Ȃ�size = 0
			//����1: delimiter = default L','
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
			//�������delimiter�ŕ�������vector�ɕϊ�����
			//return: �ϊ�����vector, �����Ȃ�size = 0
			//����1: delimiter = default L','
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
			//�������delimiter�ŕ�������vector�ɕϊ�����
			//return: �ϊ�����vector, �����Ȃ�size = 0
			//����1: delimiter = default L','
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
			//�������delimiter�ŕ�������vector�ɕϊ�����
			//return: �ϊ�����vector, �����Ȃ�size = 0
			//����1: delimiter = default L','
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
			//�������delimiter�ŕ�������vector�ɕϊ�����
			//return: �ϊ�����vector, �����Ȃ�size = 0
			//����1: delimiter = default L','
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
			//�������delimiter�ŕ�������vector�ɕϊ�����
			//return: �ϊ�����vector, �����Ȃ�size = 0
			//����1: delimiter = default L','
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