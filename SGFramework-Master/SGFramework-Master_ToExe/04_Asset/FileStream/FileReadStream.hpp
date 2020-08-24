/*----------------------------------------------------------------------------------
File入力を行うFileReadStream , WFileReadStream class
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
		//File入力を行うFileReadStream 
		//対応文字はchar文字, 規定文字コードはUTF-8
		class FileReadStream final
		{
		public:
			//----------------------------------------------------------------------------------
			//[Open]
			//ファイルを開く,  本関数実行時点で対応コードはUTF-8となる
			//throw: ファルが開けなかった場合
			//引数1: ファイルパス(拡張子含む)
			//引数2: バイナリで開くか否か : default = false
			//引数3: UTF-8指定を無効にする : default = false
			inline void Open(const sgstring& path, bool isBinary = false, bool isInvalidUtf8 = false);
			//----------------------------------------------------------------------------------
			//[Close]
			//ファイルを閉じる
			inline void Close();
			//----------------------------------------------------------------------------------
			//[Read]
			//ファイルの中身を全文読み込む (char->wide変換)
			//return: ファイルの中の文字列
			inline sgstring Read();
			//----------------------------------------------------------------------------------
			//[ReadUnite]
			//ファイルの中身を全文読み込む (変換ではなく, char2文字を合体させてwcharとして扱う)
			//return: ファイルの中の文字列
			inline sgstring ReadUnite();
			//----------------------------------------------------------------------------------
			//[ReadStd]
			//ファイルの中身を全文読み込む, std版
			//return: ファイルの中の文字列
			inline std::string ReadStd();
			//----------------------------------------------------------------------------------
			//[ReadVector]
			//ファイルの中身を全文読み込む, std::vector版
			//return: ファイルの中の文字列
			inline std::vector<byte> ReadVector();
			//----------------------------------------------------------------------------------
			//[ReadVector]
			//ファイルの中身を全文読み込む, std::vector版
			//引数1: ファイルの中の文字列を代入するvector
			inline void ReadVector(std::vector<byte>& vector);
			//----------------------------------------------------------------------------------
			//[Read]
			//ファイルの中身を全文読み込む (char->wide変換)
			//引数1: ファイルの中の文字列を代入するstring
			inline void Read(sgstring& string);
			//----------------------------------------------------------------------------------
			//[ReadUnite]
			//ファイルの中身を全文読み込む (変換ではなく, char2文字を合体させてwcharとして扱う)
			//引数1: ファイルの中の文字列を代入するstring
			inline void ReadUnite(sgstring& string);
			//----------------------------------------------------------------------------------
			//[Read]
			//ファイルの中身を全文読み込む, std版
			//引数1: ファイルの中の文字列を代入するstring
			inline void ReadStd(std::string& string);
			//----------------------------------------------------------------------------------
			//[IsOpen]
			//return: ファイルがthisを用いて開いているか否か
			inline bool IsOpen();
			//----------------------------------------------------------------------------------
			//[IsBinary]
			//return: バイナリ開いているか否か
			inline bool IsBinary() { return m_isBinary; }

			//----------------------------------------------------------------------------------
			//[ChangeLocale]
			//Localeを変更する
			//引数1: 変更するlocale , 注意！thisのデストラクタが呼ばれるまで有効なポインタであること
			template <class _Elem, unsigned long _Mymax, std::codecvt_mode _Mymode>
			inline void ChangeLocale(std::codecvt_utf8<_Elem, _Mymax, _Mymode>& convert);
			//----------------------------------------------------------------------------------
			//[ChangeLocale]
			//Localeを変更する
			//引数1: 変更するlocale , 注意！thisのデストラクタが呼ばれるまで有効なポインタであること
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

		//File入力を行うWFileReadStream
		//対応文字はwchar文字, 規定文字コードはUTF-8
		class WFileReadStream final
		{
		public:
			//----------------------------------------------------------------------------------
			//[Open]
			//ファイルを開く,  本関数実行時点で対応コードはUTF-8となる
			//throw: ファルが開けなかった場合
			//引数1: ファイルパス(拡張子含む)
			//引数2: バイナリで開くか否か : default = false
			//引数3: UTF-8指定を無効にする : default = false
			inline void Open(const sgstring& path, bool isBinary = false, bool isInvalidUtf8 = false);
			//----------------------------------------------------------------------------------
			//[Close]
			//ファイルを閉じる
			inline void Close();
			//----------------------------------------------------------------------------------
			//[Read]
			//ファイルの中身を全文読み込む
			//return: ファイルの中の文字列
			inline sgstring Read();
			//----------------------------------------------------------------------------------
			//[ReadStd]
			//ファイルの中身を全文読み込む, std版
			//return: ファイルの中の文字列
			inline std::wstring ReadStd();
			//----------------------------------------------------------------------------------
			//[ReadStd]
			//ファイルの中身を全文読み込む
			//return: ファイルの中の文字列
			inline void Read(sgstring& string);
			//----------------------------------------------------------------------------------
			//[ReadStd]
			//ファイルの中身を全文読み込む, std版
			//引数1: ファイルの中の文字列を代入するstring
			inline void ReadStd(std::wstring& string);
			//----------------------------------------------------------------------------------
			//[IsOpen]
			//return: ファイルがthisを用いて開いているか否か
			inline bool IsOpen();
			//----------------------------------------------------------------------------------
			//[IsBinary]
			//return: バイナリ開いているか否か
			inline bool IsBinary() { return m_isBinary; }

			//----------------------------------------------------------------------------------
			//[ChangeLocale]
			//Localeを変更する
			//引数1: 変更するlocale , 注意！thisのデストラクタが呼ばれるまで有効なポインタであること
			template <class _Elem, unsigned long _Mymax, std::codecvt_mode _Mymode>
			inline void ChangeLocale(std::codecvt_utf8<_Elem, _Mymax, _Mymode>& convert);
			//----------------------------------------------------------------------------------
			//[ChangeLocale]
			//Localeを変更する
			//引数1: 変更するlocale , 注意！thisのデストラクタが呼ばれるまで有効なポインタであること
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
		//ファイルを開く,  本関数実行時点で対応コードはUTF-8となる
		//throw: ファルが開けなかった場合
		//引数1: ファイルパス(拡張子含む)
		//引数2: バイナリで開くか否か : default = false
		//引数3: UTF-8指定を無効にする : default = false
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
		//ファイルを閉じる
		inline void FileReadStream::Close() { m_fileStream.close(); }
		//----------------------------------------------------------------------------------
		//[Read]
		//ファイルの中身を全文読み込む (char->wide変換)
		//return: ファイルの中の文字列
		inline sgstring FileReadStream::Read() { return sgstring(std::string(std::istreambuf_iterator<char>(m_fileStream), std::istreambuf_iterator<char>())); }
		//----------------------------------------------------------------------------------
		//[ReadUnite]
		//ファイルの中身を全文読み込む (変換ではなく, char2文字を合体させてwcharとして扱う)
		//return: ファイルの中の文字列
		inline sgstring FileReadStream::ReadUnite()
		{
			//1バイト文字列
			std::string read1Byte = std::string(std::istreambuf_iterator<char>(m_fileStream), std::istreambuf_iterator<char>());
			//返り値
			sgstring read2Byte;
			//wchar変換
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
		//ファイルの中身を全文読み込む, std版
		//return: ファイルの中の文字列s
		inline std::string FileReadStream::ReadStd() { return std::string(std::istreambuf_iterator<char>(m_fileStream), std::istreambuf_iterator<char>()); }
		//----------------------------------------------------------------------------------
		//[ReadVector]
		//ファイルの中身を全文読み込む, std::vector版
		//return: ファイルの中の文字列
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
		//ファイルの中身を全文読み込む, std::vector版
		//引数1: ファイルの中の文字列を代入するvector
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
		//ファイルの中身を全文読み込む
		//引数1: ファイルの中の文字列を代入するstring
		inline void FileReadStream::Read(sgstring & string) { string.assign(sgstring(std::string(std::istreambuf_iterator<char>(m_fileStream), std::istreambuf_iterator<char>()))); }
		//----------------------------------------------------------------------------------
		//[ReadUnite]
		//ファイルの中身を全文読み込む (変換ではなく, char2文字を合体させてwcharとして扱う)
		//引数1: ファイルの中の文字列を代入するstring
		inline void FileReadStream::ReadUnite(sgstring & string)
		{
			//1バイト文字列
			std::string read1Byte = std::string(std::istreambuf_iterator<char>(m_fileStream), std::istreambuf_iterator<char>());
			//clear
			string.clear();
			//wchar変換
			for (auto it = read1Byte.begin(); it != read1Byte.end();)
			{
				string.push_back(static_cast<wchar>(*it << 8 | *(it + 1)));
				++it;
				if (it != read1Byte.end()) ++it;
			}
		}
		//----------------------------------------------------------------------------------
		//[Read]
		//ファイルの中身を全文読み込む, std版
		//引数1: ファイルの中の文字列を代入するstring
		inline void FileReadStream::ReadStd(std::string & string) { string.assign(std::istreambuf_iterator<char>(m_fileStream), std::istreambuf_iterator<char>()); }
		//----------------------------------------------------------------------------------
		//[IsOpen]
		//return: ファイルがthisを用いて開いているか否か
		inline bool FileReadStream::IsOpen() { return m_fileStream.is_open(); }

		//----------------------------------------------------------------------------------
		//[ChangeLocale]
		//Localeを変更する
		//引数1: 変更するlocale , 注意！thisのデストラクタが呼ばれるまで有効なポインタであること
		template<class _Elem, unsigned long _Mymax, std::codecvt_mode _Mymode>
		inline void FileReadStream::ChangeLocale(std::codecvt_utf8<_Elem, _Mymax, _Mymode>& convert) { m_fileStream->imbue(std::locale(std::locale::empty(), &convert)); }
		//----------------------------------------------------------------------------------
		//[ChangeLocale]
		//Localeを変更する
		//引数1: 変更するlocale , 注意！thisのデストラクタが呼ばれるまで有効なポインタであること
		template<class _Elem, unsigned long _Mymax, std::codecvt_mode _Mymode>
		inline void FileReadStream::ChangeLocale(std::codecvt_utf16<_Elem, _Mymax, _Mymode>& convert) { m_fileStream->imbue(std::locale(std::locale::empty(), &convert)); }

		//() operator
		inline std::ifstream & FileReadStream::operator()() { return m_fileStream; }
		//-> operator
		inline std::ifstream & FileReadStream::operator->() { return m_fileStream; }



		//----------------------------------------------------------------------------------
		//[Open]
		//ファイルを開く,  本関数実行時点で対応コードはUTF-8となる
		//throw: ファルが開けなかった場合
		//引数1: ファイルパス(拡張子含む)
		//引数2: バイナリで開くか否か : default = false
		//引数3: UTF-8指定を無効にする : default = false
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
		//ファイルを閉じる
		inline void WFileReadStream::Close() { m_fileStream.close(); }
		//----------------------------------------------------------------------------------
		//[Read]
		//ファイルの中身を全文読み込む
		//return: ファイルの中の文字列
		inline sgstring WFileReadStream::Read() { return sgstring(std::wstring(std::istreambuf_iterator<wchar>(m_fileStream), std::istreambuf_iterator<wchar>())); }
		//----------------------------------------------------------------------------------
		//[ReadStd]
		//ファイルの中身を全文読み込む, std版
		//return: ファイルの中の文字列
		inline std::wstring WFileReadStream::ReadStd() { return std::wstring(std::istreambuf_iterator<wchar>(m_fileStream), std::istreambuf_iterator<wchar>()); }
		//----------------------------------------------------------------------------------
		//[ReadStd]
		//ファイルの中身を全文読み込む
		//return: ファイルの中の文字列
		inline void WFileReadStream::Read(sgstring & string) { string.assign(sgstring(std::wstring(std::istreambuf_iterator<wchar>(m_fileStream), std::istreambuf_iterator<wchar>()))); }
		//----------------------------------------------------------------------------------
		//[ReadStd]
		//ファイルの中身を全文読み込む, std版
		//引数1: ファイルの中の文字列を代入するstring
		inline void WFileReadStream::ReadStd(std::wstring & string) { string.assign(std::istreambuf_iterator<wchar>(m_fileStream), std::istreambuf_iterator<wchar>()); }
		//----------------------------------------------------------------------------------
		//[IsOpen]
		//return: ファイルがthisを用いて開いているか否か
		inline bool WFileReadStream::IsOpen() { return m_fileStream.is_open(); }

		//----------------------------------------------------------------------------------
		//[ChangeLocale]
		//Localeを変更する
		//引数1: 変更するlocale , 注意！thisのデストラクタが呼ばれるまで有効なポインタであること
		template<class _Elem, unsigned long _Mymax, std::codecvt_mode _Mymode>
		inline void WFileReadStream::ChangeLocale(std::codecvt_utf8<_Elem, _Mymax, _Mymode>& convert)
		{
			m_fileStream->imbue(std::locale(std::locale::empty(), &convert));
		}
		//----------------------------------------------------------------------------------
		//[ChangeLocale]
		//Localeを変更する
		//引数1: 変更するlocale , 注意！thisのデストラクタが呼ばれるまで有効なポインタであること
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