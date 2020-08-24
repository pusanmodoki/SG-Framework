/*----------------------------------------------------------------------------------
File���o�͂��s��FileStream, WFileStream class
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_FILE_STREAM_HPP_
#define SGFRAMEWORK_HEADER_FILE_STREAM_HPP_
#include <fstream>
#include <memory>
#include <sstream>
#include <iterator>
#include <codecvt>
#include <locale>
#include "../../02_CustomVariable/CustomVariable.hpp"
#include "FileReadStream.hpp"
#include "FileWriteStream.hpp"

// Framework namespace
namespace SGFramework
{
	//File Edit
	namespace File
	{
		//File���o�͂��s��FileStream
		//�Ή�������char����, �K�蕶���R�[�h��UTF-8
		class FileStream final
		{
		public:
			//----------------------------------------------------------------------------------
			//[Open]
			//�t�@�C�����J��,  �{�֐����s���_�őΉ��R�[�h��UTF-8�ƂȂ�
			//throw: �t�@�����J���Ȃ������ꍇ
			//����1: �t�@�C���p�X(�g���q�܂�)
			//����2: �o�C�i���ŊJ�����ۂ� : default = false
			//����3: �ǉ��������ݎd�l�ŊJ�����ۂ� : default = false
			//����4: UTF-8�w��𖳌��ɂ��� : default = false
			inline void Open(const sgstring& path, bool isBinary = false, 
				bool isAppend = false, bool isInvalidUtf8 = false);
			//----------------------------------------------------------------------------------
			//[Close]
			//�t�@�C�������
			inline void Close();
			//----------------------------------------------------------------------------------
			//[Read]
			//�t�@�C���̒��g��S���ǂݍ��� (char->wide�ϊ�)
			//return: �t�@�C���̒��̕�����
			inline sgstring Read();
			//----------------------------------------------------------------------------------
			//[ReadUnite]
			//�t�@�C���̒��g��S���ǂݍ��� (�ϊ��ł͂Ȃ�, char2���������̂�����wchar�Ƃ��Ĉ���)
			//return: �t�@�C���̒��̕�����
			inline sgstring ReadUnite();
			//----------------------------------------------------------------------------------
			//[ReadStd]
			//�t�@�C���̒��g��S���ǂݍ���, std��
			//return: �t�@�C���̒��̕�����
			inline std::string ReadStd();
			//----------------------------------------------------------------------------------
			//[Read]
			//�t�@�C���̒��g��S���ǂݍ��� (char->wide�ϊ�)
			//����1: �t�@�C���̒��̕������������string
			inline void Read(sgstring& string);
			//----------------------------------------------------------------------------------
			//[ReadUnite]
			//�t�@�C���̒��g��S���ǂݍ��� (�ϊ��ł͂Ȃ�, char2���������̂�����wchar�Ƃ��Ĉ���)
			//����1: �t�@�C���̒��̕������������string
			inline sgstring ReadUnite(sgstring& string);
			//----------------------------------------------------------------------------------
			//[Read]
			//�t�@�C���̒��g��S���ǂݍ���, std��
			//����1: �t�@�C���̒��̕������������string
			inline void ReadStd(std::string& string);
			//----------------------------------------------------------------------------------
			//[ReadVector]
			//�t�@�C���̒��g��S���ǂݍ���, std::vector��
			//return: �t�@�C���̒��̕�����
			inline std::vector<byte> ReadVector();
			//----------------------------------------------------------------------------------
			//[ReadVector]
			//�t�@�C���̒��g��S���ǂݍ���, std::vector��
			//����1: �t�@�C���̒��̕������������vector
			inline void ReadVector(std::vector<byte>& vector);
			//----------------------------------------------------------------------------------
			//[Write]
			//�t�@�C����write���������� (wide->char�ϊ�)
			//����1: �t�@�C���ɏ�������string
			inline void Write(const sgstring& write);
			//----------------------------------------------------------------------------------
			//[WriteSplit]
			//�t�@�C����write���������� (�ϊ��ł͂Ȃ�, wchar1�����𕪊�������char�Ƃ��Ĉ���)
			//����1: �t�@�C���ɏ�������string
			inline void WriteSplit(const sgstring& write);
			//----------------------------------------------------------------------------------
			//[Write]
			//�t�@�C����write���������� , std��
			//����1: �t�@�C���ɏ�������string
			inline void Write(const std::string& write);
			//----------------------------------------------------------------------------------
			//[Write]
			//�t�@�C����write���������� , std��
			//����1: �t�@�C���ɏ�������string
			inline void Write(const std::vector<byte>& write);
			//----------------------------------------------------------------------------------
			//[IsOpen]
			//return: �t�@�C����this��p���ĊJ���Ă��邩�ۂ�
			inline bool IsOpen();
			//----------------------------------------------------------------------------------
			//[IsBinary]
			//return: �o�C�i���J���Ă��邩�ۂ�
			inline bool IsBinary() { return m_isBinary; }

			//----------------------------------------------------------------------------------
			//[ChangeLocale]
			//Locale��ύX����
			//����1: �ύX����locale , ���ӁIthis�̃f�X�g���N�^���Ă΂��܂ŗL���ȃ|�C���^�ł��邱��
			template <class _Elem, unsigned long _Mymax, std::codecvt_mode _Mymode>
			inline void ChangeLocale(std::codecvt_utf8<_Elem, _Mymax, _Mymode>& convert);
			//----------------------------------------------------------------------------------
			//[ChangeLocale]
			//Locale��ύX����
			//����1: �ύX����locale , ���ӁIthis�̃f�X�g���N�^���Ă΂��܂ŗL���ȃ|�C���^�ł��邱��
			template <class _Elem, unsigned long _Mymax, std::codecvt_mode _Mymode>
			inline void ChangeLocale(std::codecvt_utf16<_Elem, _Mymax, _Mymode>& convert);
			
			//() operator
			inline std::fstream& operator () ();
			//-> operator
			inline std::fstream& operator -> ();
			// << operator (1 wchar -> 1 char)
			inline void operator << (const sgstring& string);
			// << operator
			inline void operator << (const std::string& string);

		private:
			//file stream
			std::fstream m_fileStream;
			//is isBinary
			bool m_isBinary;
		};

		//File���o�͂��s��WFileStream
		//�Ή�������wchar����, �K�蕶���R�[�h��UTF-8
		class WFileStream final
		{
		public:
			//----------------------------------------------------------------------------------
			//[Open]
			//�t�@�C�����J��,  �{�֐����s���_�őΉ��R�[�h��UTF-8�ƂȂ�
			//throw: �t�@�����J���Ȃ������ꍇ
			//����1: �t�@�C���p�X(�g���q�܂�)
			//����2: �o�C�i���ŊJ�����ۂ� : default = false
			//����3: �ǉ��������ݎd�l�ŊJ�����ۂ� : default = false
			//����4: UTF-8�w��𖳌��ɂ��� : default = false
			inline void Open(const sgstring& path, bool isBinary = false,
				bool isAppend = false, bool isInvalidUtf8 = false);
			//----------------------------------------------------------------------------------
			//[Close]
			//�t�@�C�������
			inline void Close();
			//----------------------------------------------------------------------------------
			//[Read]
			//�t�@�C���̒��g��S���ǂݍ���
			//return: �t�@�C���̒��̕�����
			inline sgstring Read();
			//----------------------------------------------------------------------------------
			//[ReadStd]
			//�t�@�C���̒��g��S���ǂݍ���, std��
			//return: �t�@�C���̒��̕�����
			inline std::string ReadStd();
			//----------------------------------------------------------------------------------
			//[ReadStd]
			//�t�@�C���̒��g��S���ǂݍ���
			//return: �t�@�C���̒��̕�����
			inline void Read(sgstring& string);
			//----------------------------------------------------------------------------------
			//[ReadStd]
			//�t�@�C���̒��g��S���ǂݍ���, std��
			//����1: �t�@�C���̒��̕������������string
			inline void ReadStd(std::wstring& string);
			//----------------------------------------------------------------------------------
			//[Write]
			//�t�@�C����write����������
			//����1: �t�@�C���ɏ�������string
			inline void Write(const sgstring& write);
			//----------------------------------------------------------------------------------
			//[Write]
			//�t�@�C����write����������
			//����1: �t�@�C���ɏ�������string
			inline void Write(const std::wstring& write);
			//----------------------------------------------------------------------------------
			//[IsOpen]
			//return: �t�@�C����this��p���ĊJ���Ă��邩�ۂ�
			inline bool IsOpen();
			//----------------------------------------------------------------------------------
			//[IsBinary]
			//return: �o�C�i���J���Ă��邩�ۂ�
			inline bool IsBinary() { return m_isBinary; }

			//----------------------------------------------------------------------------------
			//[ChangeLocale]
			//Locale��ύX����
			//����1: �ύX����locale , ���ӁIthis�̃f�X�g���N�^���Ă΂��܂ŗL���ȃ|�C���^�ł��邱��
			template <class _Elem, unsigned long _Mymax, std::codecvt_mode _Mymode>
			inline void ChangeLocale(std::codecvt_utf8<_Elem, _Mymax, _Mymode>& convert);
			//----------------------------------------------------------------------------------
			//[ChangeLocale]
			//Locale��ύX����
			//����1: �ύX����locale , ���ӁIthis�̃f�X�g���N�^���Ă΂��܂ŗL���ȃ|�C���^�ł��邱��
			template <class _Elem, unsigned long _Mymax, std::codecvt_mode _Mymode>
			inline void ChangeLocale(std::codecvt_utf16<_Elem, _Mymax, _Mymode>& convert);

			//() operator
			inline std::wfstream& operator () ();
			//-> operator
			inline std::wfstream& operator -> ();
			// << operator 
			inline void operator << (const sgstring& string);
			// << operator
			inline void operator << (const std::wstring& string);

		private:
			//file stream
			std::wfstream m_fileStream;
			//is isBinary
			bool m_isBinary;
		};


		//----------------------------------------------------------------------------------
		//[Open]
		//�t�@�C�����J��,  �{�֐����s���_�őΉ��R�[�h��UTF-8�ƂȂ�
		//throw: �t�@�����J���Ȃ������ꍇ
		//����1: �t�@�C���p�X(�g���q�܂�)
		//����2: �o�C�i���ŊJ�����ۂ� : default = false
		//����3: �ǉ��������ݎd�l�ŊJ�����ۂ� : default = false
		//����4: UTF-8�w��𖳌��ɂ��� : default = false
		inline void FileStream::Open(const sgstring & path, bool isBinary, bool isAppend, bool isInvalidUtf8)
		{
			std::ios::openmode openMode = 0;
			//app, binary
			if (isAppend) openMode |= std::ios::app;
			else openMode |= std::ios::out;
			
			if (isBinary)
			{
				openMode |= std::ios::binary;
				m_isBinary = true;
			}
			else m_isBinary = false;

			//io
			openMode |= std::ios::in;

			//open
			m_fileStream.open(path, openMode);

			//successes
			if (IS_TRUE(m_fileStream.is_open()) & IS_FALSE(isInvalidUtf8))
				m_fileStream.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<char>));
			//failed
			else if (IS_FALSE(m_fileStream.is_open()))
				throw InvalidArgument(L"Error!! FileStream->Open", L"File NotFound: " + path);
		}
		//----------------------------------------------------------------------------------
		//[Close]
		//�t�@�C�������
		inline void FileStream::Close() { m_fileStream.close(); }
		//----------------------------------------------------------------------------------
		//[Read]
		//�t�@�C���̒��g��S���ǂݍ��� (char->wide�ϊ�)
		//return: �t�@�C���̒��̕�����
		inline sgstring FileStream::Read() { return sgstring(std::string(std::istreambuf_iterator<char>(m_fileStream), std::istreambuf_iterator<char>())); }
		//----------------------------------------------------------------------------------
		//[ReadUnite]
		//�t�@�C���̒��g��S���ǂݍ��� (�ϊ��ł͂Ȃ�, char2���������̂�����wchar�Ƃ��Ĉ���)
		//return: �t�@�C���̒��̕�����
		inline sgstring FileStream::ReadUnite()
		{
			//1�o�C�g������
			std::string read1Byte = std::string(std::istreambuf_iterator<char>(m_fileStream), std::istreambuf_iterator<char>());
			//�Ԃ�l
			sgstring read2Byte;
			//wchar�ϊ�
			for (auto it = read1Byte.begin(); it != read1Byte.end();)
			{
				read2Byte.push_back(static_cast<wchar>(*(it + 1) << 8) | *it);
				++it;
				if (it != read1Byte.end()) ++it;
			}
		}
		//----------------------------------------------------------------------------------
		//[ReadStd]
		//�t�@�C���̒��g��S���ǂݍ���, std��
		//return: �t�@�C���̒��̕�����s
		inline std::string FileStream::ReadStd() { return std::string(std::istreambuf_iterator<char>(m_fileStream), std::istreambuf_iterator<char>()); }
		//----------------------------------------------------------------------------------
		//[Read]
		//�t�@�C���̒��g��S���ǂݍ���
		//����1: �t�@�C���̒��̕������������string
		inline void FileStream::Read(sgstring & string) { string.assign(sgstring(std::string(std::istreambuf_iterator<char>(m_fileStream), std::istreambuf_iterator<char>()))); }
		//----------------------------------------------------------------------------------
		//[ReadUnite]
		//�t�@�C���̒��g��S���ǂݍ��� (�ϊ��ł͂Ȃ�, char2���������̂�����wchar�Ƃ��Ĉ���)
		//����1: �t�@�C���̒��̕������������string
		inline sgstring FileStream::ReadUnite(sgstring & string)
		{
			//1�o�C�g������
			std::string read1Byte = std::string(std::istreambuf_iterator<char>(m_fileStream), std::istreambuf_iterator<char>());
			//clear
			string.clear();
			//wchar�ϊ�
			for (auto it = read1Byte.begin(); it != read1Byte.end();)
			{
				string.push_back(static_cast<wchar>(*(it + 1) << 8) | *it);
				++it;
				if (it != read1Byte.end()) ++it;
			}
		}
		//----------------------------------------------------------------------------------
		//[Read]
		//�t�@�C���̒��g��S���ǂݍ���, std��
		//����1: �t�@�C���̒��̕������������string
		inline void FileStream::ReadStd(std::string & string) { string.assign(std::istreambuf_iterator<char>(m_fileStream), std::istreambuf_iterator<char>()); }
		//----------------------------------------------------------------------------------
		//[ReadVector]
		//�t�@�C���̒��g��S���ǂݍ���, std::vector��
		//return: �t�@�C���̒��̕�����
		inline std::vector<byte> FileStream::ReadVector() 
		{
			m_fileStream.seekg(0, std::ios::end);
			auto eofPos = m_fileStream.tellg();
			m_fileStream.clear();
			m_fileStream.seekg(0, std::ios::beg);
			
			std::vector<byte> result(static_cast<uint64>(eofPos - m_fileStream.tellg()));
			return std::move(std::vector<byte>(std::istream_iterator<byte>(m_fileStream), std::istream_iterator<byte>())); 
		}
		//----------------------------------------------------------------------------------
		//[ReadVector]
		//�t�@�C���̒��g��S���ǂݍ���, std::vector��
		//����1: �t�@�C���̒��̕������������vector
		inline void FileStream::ReadVector(std::vector<byte>& vector)
		{
			m_fileStream.seekg(0, std::ios::end);
			auto eofPos = m_fileStream.tellg();
			m_fileStream.clear();
			m_fileStream.seekg(0, std::ios::beg);

			vector.resize(static_cast<uint64>(eofPos - m_fileStream.tellg()));
			vector.assign(std::istream_iterator<byte>(m_fileStream), std::istream_iterator<byte>()); 
		}
		//----------------------------------------------------------------------------------
		//[Write]
		//�t�@�C����write����������
		//����1: �t�@�C���ɏ�������string
		inline void FileStream::Write(const sgstring & write)
		{
			//�ϊ�
			int inLength = static_cast<int>(write.length());
			int outLength = WideCharToMultiByte(CP_ACP, 0, write.c_str(), inLength, 0, 0, nullptr, nullptr);
			std::string result(outLength, L'\0');
			if (outLength) WideCharToMultiByte(CP_UTF8, 0, write.c_str(), inLength, &result[0], outLength, nullptr, nullptr);
			
			//��������
			if (IS_FALSE(m_isBinary)) m_fileStream << result;
			else m_fileStream.write(result.c_str(), result.size());
		}
		//----------------------------------------------------------------------------------
		//[WriteSplit]
		//�t�@�C����write���������� (�ϊ��ł͂Ȃ�, wchar1�����𕪊�������char�Ƃ��Ĉ���)
		//����1: �t�@�C���ɏ�������string
		inline void FileStream::WriteSplit(const sgstring & write)
		{
			std::string join;
			//string�ϊ�
			for (auto& e : write)
			{
				join.push_back((char)(e >> 8));
				join.push_back((char)(e & 0xff));
			}
			//��������
			if (IS_FALSE(m_isBinary)) m_fileStream << join;
			else m_fileStream.write(join.c_str(), join.size());
		}
		//----------------------------------------------------------------------------------
		//[Write]
		//�t�@�C����write���������� , std��
		//����1: �t�@�C���ɏ�������string
		inline void FileStream::Write(const std::string & write) { IS_FALSE(m_isBinary) ? m_fileStream << write : m_fileStream.write(write.c_str(), write.size()); }
		//----------------------------------------------------------------------------------
		//[Write]
		//�t�@�C����write���������� , std��
		//����1: �t�@�C���ɏ�������string
		inline void FileStream::Write(const std::vector<byte>& write) { m_fileStream.write(reinterpret_cast<const char*>(write.data()), write.size()); }
		//----------------------------------------------------------------------------------
		//[IsOpen]
		//return: �t�@�C����this��p���ĊJ���Ă��邩�ۂ�
		inline bool FileStream::IsOpen() { return m_fileStream.is_open(); }

		//----------------------------------------------------------------------------------
		//[ChangeLocale]
		//Locale��ύX����
		//����1: �ύX����locale , ���ӁIthis�̃f�X�g���N�^���Ă΂��܂ŗL���ȃ|�C���^�ł��邱��
		template<class _Elem, unsigned long _Mymax, std::codecvt_mode _Mymode>
		inline void FileStream::ChangeLocale(std::codecvt_utf8<_Elem, _Mymax, _Mymode>& convert) { m_fileStream->imbue(std::locale(std::locale::empty(), &convert)); }
		//----------------------------------------------------------------------------------
		//[ChangeLocale]
		//Locale��ύX����
		//����1: �ύX����locale , ���ӁIthis�̃f�X�g���N�^���Ă΂��܂ŗL���ȃ|�C���^�ł��邱��
		template<class _Elem, unsigned long _Mymax, std::codecvt_mode _Mymode>
		inline void FileStream::ChangeLocale(std::codecvt_utf16<_Elem, _Mymax, _Mymode>& convert) { m_fileStream->imbue(std::locale(std::locale::empty(), &convert)); }
	
		//() operator
		inline std::fstream & FileStream::operator()() { return m_fileStream; }
		//-> operator
		inline std::fstream & FileStream::operator->() { return m_fileStream; }
		// << operator (1 wchar -> 1 char)
		inline void FileStream::operator<<(const sgstring & string) { Write(string); }
		// << operator
		inline void FileStream::operator<<(const std::string & string) { Write(string); }

		//----------------------------------------------------------------------------------
		//[Open]
		//�t�@�C�����J��,  �{�֐����s���_�őΉ��R�[�h��UTF-8�ƂȂ�
		//throw: �t�@�����J���Ȃ������ꍇ
		//����1: �t�@�C���p�X(�g���q�܂�)
		//����2: �o�C�i���ŊJ�����ۂ� : default = false
		//����3: �ǉ��������ݎd�l�ŊJ�����ۂ� : default = false
		//����4: UTF-8�w��𖳌��ɂ��� : default = false
		inline void WFileStream::Open(const sgstring & path, bool isBinary, bool isAppend, bool isInvalidUtf8)
		{
			std::ios::openmode openMode = 0;
			//app, binary
			if (isAppend) openMode |= std::ios::app;
			else openMode |= std::ios::out;

			if (isBinary)
			{
				openMode |= std::ios::binary;
				m_isBinary = true;
			}
			else m_isBinary = false;
			
			//io
			openMode |= std::ios::in;

			//open
			m_fileStream.open(path, openMode);

			//successes
			if (IS_TRUE(m_fileStream.is_open()) & IS_FALSE(isInvalidUtf8))
				m_fileStream.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar>));
			//failed
			else if (IS_FALSE(m_fileStream.is_open()))
				throw InvalidArgument(L"Error!! FileStream->Open", L"File NotFound: " + path);
		}
		//----------------------------------------------------------------------------------
		//[Close]
		//�t�@�C�������
		inline void WFileStream::Close() { m_fileStream.close(); }
		//----------------------------------------------------------------------------------
		//[Read]
		//�t�@�C���̒��g��S���ǂݍ���
		//return: �t�@�C���̒��̕�����
		inline sgstring WFileStream::Read() { return sgstring(std::wstring(std::istreambuf_iterator<wchar>(m_fileStream), std::istreambuf_iterator<wchar>())); }
		//----------------------------------------------------------------------------------
		//[ReadStd]
		//�t�@�C���̒��g��S���ǂݍ���, std��
		//return: �t�@�C���̒��̕�����
		inline std::string WFileStream::ReadStd() { return std::string(std::istreambuf_iterator<wchar>(m_fileStream), std::istreambuf_iterator<wchar>()); }
		//----------------------------------------------------------------------------------
		//[ReadStd]
		//�t�@�C���̒��g��S���ǂݍ���
		//return: �t�@�C���̒��̕�����
		inline void WFileStream::Read(sgstring & string) { string.assign(sgstring(std::wstring(std::istreambuf_iterator<wchar>(m_fileStream), std::istreambuf_iterator<wchar>()))); }
		//----------------------------------------------------------------------------------
		//[ReadStd]
		//�t�@�C���̒��g��S���ǂݍ���, std��
		//����1: �t�@�C���̒��̕������������string
		inline void WFileStream::ReadStd(std::wstring & string) { string.assign(std::istreambuf_iterator<wchar>(m_fileStream), std::istreambuf_iterator<wchar>()); }
		//----------------------------------------------------------------------------------
		//[Write]
		//�t�@�C����write����������
		//����1: �t�@�C���ɏ�������string
		inline void WFileStream::Write(const sgstring & write) { IS_FALSE(m_isBinary) ? m_fileStream << write : m_fileStream.write(write.c_str(), write.size()); }
		//----------------------------------------------------------------------------------
		//[Write]
		//�t�@�C����write����������
		//����1: �t�@�C���ɏ�������string
		inline void WFileStream::Write(const std::wstring & write) { IS_FALSE(m_isBinary) ? m_fileStream << write : m_fileStream.write(write.c_str(), write.size()); }
		//----------------------------------------------------------------------------------
		//[IsOpen]
		//return: �t�@�C����this��p���ĊJ���Ă��邩�ۂ�
		inline bool WFileStream::IsOpen() { return m_fileStream.is_open(); }

		//----------------------------------------------------------------------------------
		//[ChangeLocale]
		//Locale��ύX����
		//����1: �ύX����locale , ���ӁIthis�̃f�X�g���N�^���Ă΂��܂ŗL���ȃ|�C���^�ł��邱��
		template<class _Elem, unsigned long _Mymax, std::codecvt_mode _Mymode>
		inline void WFileStream::ChangeLocale(std::codecvt_utf8<_Elem, _Mymax, _Mymode>& convert)
		{
			m_fileStream->imbue(std::locale(std::locale::empty(), &convert));
		}
		//----------------------------------------------------------------------------------
		//[ChangeLocale]
		//Locale��ύX����
		//����1: �ύX����locale , ���ӁIthis�̃f�X�g���N�^���Ă΂��܂ŗL���ȃ|�C���^�ł��邱��
		template<class _Elem, unsigned long _Mymax, std::codecvt_mode _Mymode>
		inline void WFileStream::ChangeLocale(std::codecvt_utf16<_Elem, _Mymax, _Mymode>& convert)
		{
			m_fileStream->imbue(std::locale(std::locale::empty(), &convert));
		}
	
		//() operator
		inline std::wfstream & WFileStream::operator()() { return m_fileStream; }
		//-> operator
		inline std::wfstream & WFileStream::operator->() { return m_fileStream; }
		// << operator 
		inline void WFileStream::operator<<(const sgstring & string) { Write(string); }
		// << operator
		inline void WFileStream::operator<<(const std::wstring & string) { Write(string); }
	}
}

#endif // !SGFRAMEWORK_HEADER_FILE_STREAM_HPP_