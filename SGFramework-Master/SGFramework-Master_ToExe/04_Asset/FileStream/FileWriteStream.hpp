/*----------------------------------------------------------------------------------
File�o�͂��s��FileWriteStream, WFileWriteStream class
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_FILE_WRITE_STREAM_HPP_
#define SGFRAMEWORK_HEADER_FILE_WRITE_STREAM_HPP_
#include <fstream>
#include <memory>
#include <sstream>
#include <iterator>
#include <codecvt>
#include <locale>
#include "../../02_CustomVariable/CustomVariable.hpp"

// Framework namespace
namespace SGFramework
{
	//File Edit
	namespace File
	{
		//File�o�͂��s��FileWriteStream
		//�Ή�������char����, �K�蕶���R�[�h��UTF-8
		class FileWriteStream final
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
			inline std::ofstream& operator () ();
			//-> operator
			inline std::ofstream& operator -> ();
			// << operator (1 wchar -> 1 char)
			inline void operator << (const sgstring& string);
			// << operator
			inline void operator << (const std::string& string);

		private:
			//file stream
			std::ofstream m_fileStream;
			//is isBinary
			bool m_isBinary;
		};

		//File�o�͂��s��WFileWriteStream
		//�Ή�������wchar����, �K�蕶���R�[�h��UTF-8
		class WFileWriteStream final
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
			inline std::wofstream& operator () ();
			//-> operator
			inline std::wofstream& operator -> ();
			// << operator 
			inline void operator << (const sgstring& string);
			// << operator
			inline void operator << (const std::wstring& string);

		private:
			//file stream
			std::wofstream m_fileStream;
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
		inline void FileWriteStream::Open(const sgstring & path,
			bool isBinary, bool isAppend, bool isInvalidUtf8)
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
		inline void FileWriteStream::Close() { m_fileStream.close(); }
		//----------------------------------------------------------------------------------
		//[Write]
		//�t�@�C����write����������
		//����1: �t�@�C���ɏ�������string
		inline void FileWriteStream::Write(const sgstring & write)
		{
			//�ϊ�
			int inLength = static_cast<int>(write.length());
			int outLength = WideCharToMultiByte(CP_ACP, 0, write.c_str(), inLength, 0, 0, nullptr, nullptr);
			std::string result(outLength, L'\0');
			if (outLength) WideCharToMultiByte(CP_UTF8, 0, write.c_str(), inLength, &result[0], outLength, nullptr, nullptr);
			//��������
			if (IS_FALSE(m_isBinary)) m_fileStream << result;
			else m_fileStream.write(result.c_str(), outLength);
		}
		//----------------------------------------------------------------------------------
		//[WriteSplit]
		//�t�@�C����write���������� (�ϊ��ł͂Ȃ�, wchar1�����𕪊�������char�Ƃ��Ĉ���)
		//����1: �t�@�C���ɏ�������string
		inline void FileWriteStream::WriteSplit(const sgstring & write)
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
		inline void FileWriteStream::Write(const std::string & write) { IS_FALSE(m_isBinary) ? m_fileStream << write : m_fileStream.write(write.c_str(), write.size()); }
		//----------------------------------------------------------------------------------
		//[Write]
		//�t�@�C����write���������� , std��
		//����1: �t�@�C���ɏ�������string
		inline void FileWriteStream::Write(const std::vector<byte>& write) { m_fileStream.write(reinterpret_cast<const char*>(write.data()), write.size()); }
		//----------------------------------------------------------------------------------
		//[IsOpen]
		//return: �t�@�C����this��p���ĊJ���Ă��邩�ۂ�
		inline bool FileWriteStream::IsOpen() { return m_fileStream.is_open(); }

		//----------------------------------------------------------------------------------
		//[ChangeLocale]
		//Locale��ύX����
		//����1: �ύX����locale , ���ӁIthis�̃f�X�g���N�^���Ă΂��܂ŗL���ȃ|�C���^�ł��邱��
		template<class _Elem, unsigned long _Mymax, std::codecvt_mode _Mymode>
		inline void FileWriteStream::ChangeLocale(std::codecvt_utf8<_Elem, _Mymax, _Mymode>& convert) { m_fileStream->imbue(std::locale(std::locale::empty(), &convert)); }
		//----------------------------------------------------------------------------------
		//[ChangeLocale]
		//Locale��ύX����
		//����1: �ύX����locale , ���ӁIthis�̃f�X�g���N�^���Ă΂��܂ŗL���ȃ|�C���^�ł��邱��
		template<class _Elem, unsigned long _Mymax, std::codecvt_mode _Mymode>
		inline void FileWriteStream::ChangeLocale(std::codecvt_utf16<_Elem, _Mymax, _Mymode>& convert) { m_fileStream->imbue(std::locale(std::locale::empty(), &convert)); }

		//() operator
		inline std::ofstream & FileWriteStream::operator()() { return m_fileStream; }
		//-> operator
		inline std::ofstream & FileWriteStream::operator->() { return m_fileStream; }
		// << operator (1 wchar -> 1 char)
		inline void FileWriteStream::operator<<(const sgstring & string) { Write(string); }
		// << operator
		inline void FileWriteStream::operator<<(const std::string & string) { Write(string); }



		//----------------------------------------------------------------------------------
		//[Open]
		//�t�@�C�����J��,  �{�֐����s���_�őΉ��R�[�h��UTF-8�ƂȂ�
		//throw: �t�@�����J���Ȃ������ꍇ
		//����1: �t�@�C���p�X(�g���q�܂�)
		//����2: �o�C�i���ŊJ�����ۂ� : default = false
			//����4: UTF-8�w��𖳌��ɂ��� : default = false
		inline void WFileWriteStream::Open(const sgstring & path, 
			bool isBinary, bool isAppend, bool isInvalidUtf8)
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
		inline void WFileWriteStream::Close() { m_fileStream.close(); }
		//----------------------------------------------------------------------------------
		//[Write]
		//�t�@�C����write����������
		//����1: �t�@�C���ɏ�������string
		inline void WFileWriteStream::Write(const sgstring & write) { IS_FALSE(m_isBinary) ? m_fileStream << write : m_fileStream.write(write.c_str(), write.size()); }
		//----------------------------------------------------------------------------------
		//[Write]
		//�t�@�C����write����������
		//����1: �t�@�C���ɏ�������string
		inline void WFileWriteStream::Write(const std::wstring & write) { IS_FALSE(m_isBinary) ? m_fileStream << write : m_fileStream.write(write.c_str(), write.size()); }
		//----------------------------------------------------------------------------------
		//[IsOpen]
		//return: �t�@�C����this��p���ĊJ���Ă��邩�ۂ�
		inline bool WFileWriteStream::IsOpen() { return m_fileStream.is_open(); }

		//----------------------------------------------------------------------------------
		//[ChangeLocale]
		//Locale��ύX����
		//����1: �ύX����locale , ���ӁIthis�̃f�X�g���N�^���Ă΂��܂ŗL���ȃ|�C���^�ł��邱��
		template<class _Elem, unsigned long _Mymax, std::codecvt_mode _Mymode>
		inline void WFileWriteStream::ChangeLocale(std::codecvt_utf8<_Elem, _Mymax, _Mymode>& convert)
		{
			m_fileStream->imbue(std::locale(std::locale::empty(), &convert));
		}
		//----------------------------------------------------------------------------------
		//[ChangeLocale]
		//Locale��ύX����
		//����1: �ύX����locale , ���ӁIthis�̃f�X�g���N�^���Ă΂��܂ŗL���ȃ|�C���^�ł��邱��
		template<class _Elem, unsigned long _Mymax, std::codecvt_mode _Mymode>
		inline void WFileWriteStream::ChangeLocale(std::codecvt_utf16<_Elem, _Mymax, _Mymode>& convert)
		{
			m_fileStream->imbue(std::locale(std::locale::empty(), &convert));
		}

		//() operator
		inline std::wofstream & WFileWriteStream::operator()() { return m_fileStream; }
		//-> operator
		inline std::wofstream & WFileWriteStream::operator->() { return m_fileStream; }
		// << operator 
		inline void WFileWriteStream::operator<<(const sgstring & string) { Write(string); }
		// << operator
		inline void WFileWriteStream::operator<<(const std::wstring & string) { Write(string); }
	}
}

#endif // !SGFRAMEWORK_HEADER_FILE_WRITE_STREAM_HPP_
