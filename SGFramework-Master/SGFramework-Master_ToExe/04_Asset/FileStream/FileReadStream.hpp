/*----------------------------------------------------------------------------------
File���͂��s��FileReadStream , WFileReadStream class
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_FILE_READ_STREAM_HPP_
#define SGFRAMEWORK_HEADER_FILE_READ_STREAM_HPP_
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
		//File���͂��s��FileReadStream 
		//�Ή�������char����, �K�蕶���R�[�h��UTF-8
		class FileReadStream final
		{
		public:
			//----------------------------------------------------------------------------------
			//[Open]
			//�t�@�C�����J��,  �{�֐����s���_�őΉ��R�[�h��UTF-8�ƂȂ�
			//throw: �t�@�����J���Ȃ������ꍇ
			//����1: �t�@�C���p�X(�g���q�܂�)
			//����2: �o�C�i���ŊJ�����ۂ� : default = false
			//����3: UTF-8�w��𖳌��ɂ��� : default = false
			inline void Open(const sgstring& path, bool isBinary = false, bool isInvalidUtf8 = false);
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
			//[Read]
			//�t�@�C���̒��g��S���ǂݍ��� (char->wide�ϊ�)
			//����1: �t�@�C���̒��̕������������string
			inline void Read(sgstring& string);
			//----------------------------------------------------------------------------------
			//[ReadUnite]
			//�t�@�C���̒��g��S���ǂݍ��� (�ϊ��ł͂Ȃ�, char2���������̂�����wchar�Ƃ��Ĉ���)
			//����1: �t�@�C���̒��̕������������string
			inline void ReadUnite(sgstring& string);
			//----------------------------------------------------------------------------------
			//[Read]
			//�t�@�C���̒��g��S���ǂݍ���, std��
			//����1: �t�@�C���̒��̕������������string
			inline void ReadStd(std::string& string);
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
			inline std::ifstream& operator () ();
			//-> operator
			inline std::ifstream& operator -> ();

		private:
			//file stream
			std::ifstream m_fileStream;
			//is isBinary
			bool m_isBinary;
		};

		//File���͂��s��WFileReadStream
		//�Ή�������wchar����, �K�蕶���R�[�h��UTF-8
		class WFileReadStream final
		{
		public:
			//----------------------------------------------------------------------------------
			//[Open]
			//�t�@�C�����J��,  �{�֐����s���_�őΉ��R�[�h��UTF-8�ƂȂ�
			//throw: �t�@�����J���Ȃ������ꍇ
			//����1: �t�@�C���p�X(�g���q�܂�)
			//����2: �o�C�i���ŊJ�����ۂ� : default = false
			//����3: UTF-8�w��𖳌��ɂ��� : default = false
			inline void Open(const sgstring& path, bool isBinary = false, bool isInvalidUtf8 = false);
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
			inline std::wstring ReadStd();
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
			inline std::wifstream& operator () ();
			//-> operator
			inline std::wifstream& operator -> ();

		private:
			//file stream
			std::wifstream m_fileStream;
			//is isBinary
			bool m_isBinary;
		};


		//----------------------------------------------------------------------------------
		//[Open]
		//�t�@�C�����J��,  �{�֐����s���_�őΉ��R�[�h��UTF-8�ƂȂ�
		//throw: �t�@�����J���Ȃ������ꍇ
		//����1: �t�@�C���p�X(�g���q�܂�)
		//����2: �o�C�i���ŊJ�����ۂ� : default = false
		//����3: UTF-8�w��𖳌��ɂ��� : default = false
		inline void FileReadStream::Open(const sgstring & path, bool isBinary, bool isInvalidUtf8)
		{
			std::ios::openmode openMode = 0;
			//binary
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
		inline void FileReadStream::Close() { m_fileStream.close(); }
		//----------------------------------------------------------------------------------
		//[Read]
		//�t�@�C���̒��g��S���ǂݍ��� (char->wide�ϊ�)
		//return: �t�@�C���̒��̕�����
		inline sgstring FileReadStream::Read() { return sgstring(std::string(std::istreambuf_iterator<char>(m_fileStream), std::istreambuf_iterator<char>())); }
		//----------------------------------------------------------------------------------
		//[ReadUnite]
		//�t�@�C���̒��g��S���ǂݍ��� (�ϊ��ł͂Ȃ�, char2���������̂�����wchar�Ƃ��Ĉ���)
		//return: �t�@�C���̒��̕�����
		inline sgstring FileReadStream::ReadUnite()
		{
			//1�o�C�g������
			std::string read1Byte = std::string(std::istreambuf_iterator<char>(m_fileStream), std::istreambuf_iterator<char>());
			//�Ԃ�l
			sgstring read2Byte;
			//wchar�ϊ�
			for (auto it = read1Byte.begin(); it != read1Byte.end();)
			{
				read2Byte.push_back(static_cast<wchar>(*it << 8 | *(it+ 1)));
				++it;
				if (it != read1Byte.end()) ++it;
			}
			return read2Byte;
		}
		//----------------------------------------------------------------------------------
		//[ReadStd]
		//�t�@�C���̒��g��S���ǂݍ���, std��
		//return: �t�@�C���̒��̕�����s
		inline std::string FileReadStream::ReadStd() { return std::string(std::istreambuf_iterator<char>(m_fileStream), std::istreambuf_iterator<char>()); }
		//----------------------------------------------------------------------------------
		//[ReadVector]
		//�t�@�C���̒��g��S���ǂݍ���, std::vector��
		//return: �t�@�C���̒��̕�����
		inline std::vector<byte> FileReadStream::ReadVector()
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
		inline void FileReadStream::ReadVector(std::vector<byte>& vector)
		{
			m_fileStream.seekg(0, std::ios::end);
			auto eofPos = m_fileStream.tellg();
			m_fileStream.clear();
			m_fileStream.seekg(0, std::ios::beg);

			vector.resize(static_cast<uint64>(eofPos - m_fileStream.tellg()));
			vector.assign(std::istream_iterator<byte>(m_fileStream), std::istream_iterator<byte>());
		}
		//----------------------------------------------------------------------------------
		//[Read]
		//�t�@�C���̒��g��S���ǂݍ���
		//����1: �t�@�C���̒��̕������������string
		inline void FileReadStream::Read(sgstring & string) { string.assign(sgstring(std::string(std::istreambuf_iterator<char>(m_fileStream), std::istreambuf_iterator<char>()))); }
		//----------------------------------------------------------------------------------
		//[ReadUnite]
		//�t�@�C���̒��g��S���ǂݍ��� (�ϊ��ł͂Ȃ�, char2���������̂�����wchar�Ƃ��Ĉ���)
		//����1: �t�@�C���̒��̕������������string
		inline void FileReadStream::ReadUnite(sgstring & string)
		{
			//1�o�C�g������
			std::string read1Byte = std::string(std::istreambuf_iterator<char>(m_fileStream), std::istreambuf_iterator<char>());
			//clear
			string.clear();
			//wchar�ϊ�
			for (auto it = read1Byte.begin(); it != read1Byte.end();)
			{
				string.push_back(static_cast<wchar>(*it << 8 | *(it + 1)));
				++it;
				if (it != read1Byte.end()) ++it;
			}
		}
		//----------------------------------------------------------------------------------
		//[Read]
		//�t�@�C���̒��g��S���ǂݍ���, std��
		//����1: �t�@�C���̒��̕������������string
		inline void FileReadStream::ReadStd(std::string & string) { string.assign(std::istreambuf_iterator<char>(m_fileStream), std::istreambuf_iterator<char>()); }
		//----------------------------------------------------------------------------------
		//[IsOpen]
		//return: �t�@�C����this��p���ĊJ���Ă��邩�ۂ�
		inline bool FileReadStream::IsOpen() { return m_fileStream.is_open(); }

		//----------------------------------------------------------------------------------
		//[ChangeLocale]
		//Locale��ύX����
		//����1: �ύX����locale , ���ӁIthis�̃f�X�g���N�^���Ă΂��܂ŗL���ȃ|�C���^�ł��邱��
		template<class _Elem, unsigned long _Mymax, std::codecvt_mode _Mymode>
		inline void FileReadStream::ChangeLocale(std::codecvt_utf8<_Elem, _Mymax, _Mymode>& convert) { m_fileStream->imbue(std::locale(std::locale::empty(), &convert)); }
		//----------------------------------------------------------------------------------
		//[ChangeLocale]
		//Locale��ύX����
		//����1: �ύX����locale , ���ӁIthis�̃f�X�g���N�^���Ă΂��܂ŗL���ȃ|�C���^�ł��邱��
		template<class _Elem, unsigned long _Mymax, std::codecvt_mode _Mymode>
		inline void FileReadStream::ChangeLocale(std::codecvt_utf16<_Elem, _Mymax, _Mymode>& convert) { m_fileStream->imbue(std::locale(std::locale::empty(), &convert)); }

		//() operator
		inline std::ifstream & FileReadStream::operator()() { return m_fileStream; }
		//-> operator
		inline std::ifstream & FileReadStream::operator->() { return m_fileStream; }



		//----------------------------------------------------------------------------------
		//[Open]
		//�t�@�C�����J��,  �{�֐����s���_�őΉ��R�[�h��UTF-8�ƂȂ�
		//throw: �t�@�����J���Ȃ������ꍇ
		//����1: �t�@�C���p�X(�g���q�܂�)
		//����2: �o�C�i���ŊJ�����ۂ� : default = false
		//����3: UTF-8�w��𖳌��ɂ��� : default = false
		inline void WFileReadStream::Open(const sgstring & path,
			bool isBinary, bool isInvalidUtf8)
		{
			std::ios::openmode openMode = 0;
			//binary
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
		inline void WFileReadStream::Close() { m_fileStream.close(); }
		//----------------------------------------------------------------------------------
		//[Read]
		//�t�@�C���̒��g��S���ǂݍ���
		//return: �t�@�C���̒��̕�����
		inline sgstring WFileReadStream::Read() { return sgstring(std::wstring(std::istreambuf_iterator<wchar>(m_fileStream), std::istreambuf_iterator<wchar>())); }
		//----------------------------------------------------------------------------------
		//[ReadStd]
		//�t�@�C���̒��g��S���ǂݍ���, std��
		//return: �t�@�C���̒��̕�����
		inline std::wstring WFileReadStream::ReadStd() { return std::wstring(std::istreambuf_iterator<wchar>(m_fileStream), std::istreambuf_iterator<wchar>()); }
		//----------------------------------------------------------------------------------
		//[ReadStd]
		//�t�@�C���̒��g��S���ǂݍ���
		//return: �t�@�C���̒��̕�����
		inline void WFileReadStream::Read(sgstring & string) { string.assign(sgstring(std::wstring(std::istreambuf_iterator<wchar>(m_fileStream), std::istreambuf_iterator<wchar>()))); }
		//----------------------------------------------------------------------------------
		//[ReadStd]
		//�t�@�C���̒��g��S���ǂݍ���, std��
		//����1: �t�@�C���̒��̕������������string
		inline void WFileReadStream::ReadStd(std::wstring & string) { string.assign(std::istreambuf_iterator<wchar>(m_fileStream), std::istreambuf_iterator<wchar>()); }
		//----------------------------------------------------------------------------------
		//[IsOpen]
		//return: �t�@�C����this��p���ĊJ���Ă��邩�ۂ�
		inline bool WFileReadStream::IsOpen() { return m_fileStream.is_open(); }

		//----------------------------------------------------------------------------------
		//[ChangeLocale]
		//Locale��ύX����
		//����1: �ύX����locale , ���ӁIthis�̃f�X�g���N�^���Ă΂��܂ŗL���ȃ|�C���^�ł��邱��
		template<class _Elem, unsigned long _Mymax, std::codecvt_mode _Mymode>
		inline void WFileReadStream::ChangeLocale(std::codecvt_utf8<_Elem, _Mymax, _Mymode>& convert)
		{
			m_fileStream->imbue(std::locale(std::locale::empty(), &convert));
		}
		//----------------------------------------------------------------------------------
		//[ChangeLocale]
		//Locale��ύX����
		//����1: �ύX����locale , ���ӁIthis�̃f�X�g���N�^���Ă΂��܂ŗL���ȃ|�C���^�ł��邱��
		template<class _Elem, unsigned long _Mymax, std::codecvt_mode _Mymode>
		inline void WFileReadStream::ChangeLocale(std::codecvt_utf16<_Elem, _Mymax, _Mymode>& convert)
		{
			m_fileStream->imbue(std::locale(std::locale::empty(), &convert));
		}

		//() operator
		inline std::wifstream & WFileReadStream::operator()() { return m_fileStream; }
		//-> operator
		inline std::wifstream & WFileReadStream::operator->() { return m_fileStream; }
	}
}

#endif // !SGFRAMEWORK_HEADER_FILE_READ_STREAM_HPP_