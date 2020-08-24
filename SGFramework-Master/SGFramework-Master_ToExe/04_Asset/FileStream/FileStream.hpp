/*----------------------------------------------------------------------------------
File入出力を行うFileStream, WFileStream class
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
		//File入出力を行うFileStream
		//対応文字はchar文字, 規定文字コードはUTF-8
		class FileStream final
		{
		public:
			//----------------------------------------------------------------------------------
			//[Open]
			//ファイルを開く,  本関数実行時点で対応コードはUTF-8となる
			//throw: ファルが開けなかった場合
			//引数1: ファイルパス(拡張子含む)
			//引数2: バイナリで開くか否か : default = false
			//引数3: 追加書き込み仕様で開くか否か : default = false
			//引数4: UTF-8指定を無効にする : default = false
			inline void Open(const sgstring& path, bool isBinary = false, 
				bool isAppend = false, bool isInvalidUtf8 = false);
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
			//[Read]
			//ファイルの中身を全文読み込む (char->wide変換)
			//引数1: ファイルの中の文字列を代入するstring
			inline void Read(sgstring& string);
			//----------------------------------------------------------------------------------
			//[ReadUnite]
			//ファイルの中身を全文読み込む (変換ではなく, char2文字を合体させてwcharとして扱う)
			//引数1: ファイルの中の文字列を代入するstring
			inline sgstring ReadUnite(sgstring& string);
			//----------------------------------------------------------------------------------
			//[Read]
			//ファイルの中身を全文読み込む, std版
			//引数1: ファイルの中の文字列を代入するstring
			inline void ReadStd(std::string& string);
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
			//[Write]
			//ファイルにwriteを書き込む (wide->char変換)
			//引数1: ファイルに書き込むstring
			inline void Write(const sgstring& write);
			//----------------------------------------------------------------------------------
			//[WriteSplit]
			//ファイルにwriteを書き込む (変換ではなく, wchar1文字を分割させてcharとして扱う)
			//引数1: ファイルに書き込むstring
			inline void WriteSplit(const sgstring& write);
			//----------------------------------------------------------------------------------
			//[Write]
			//ファイルにwriteを書き込む , std版
			//引数1: ファイルに書き込むstring
			inline void Write(const std::string& write);
			//----------------------------------------------------------------------------------
			//[Write]
			//ファイルにwriteを書き込む , std版
			//引数1: ファイルに書き込むstring
			inline void Write(const std::vector<byte>& write);
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

		//File入出力を行うWFileStream
		//対応文字はwchar文字, 規定文字コードはUTF-8
		class WFileStream final
		{
		public:
			//----------------------------------------------------------------------------------
			//[Open]
			//ファイルを開く,  本関数実行時点で対応コードはUTF-8となる
			//throw: ファルが開けなかった場合
			//引数1: ファイルパス(拡張子含む)
			//引数2: バイナリで開くか否か : default = false
			//引数3: 追加書き込み仕様で開くか否か : default = false
			//引数4: UTF-8指定を無効にする : default = false
			inline void Open(const sgstring& path, bool isBinary = false,
				bool isAppend = false, bool isInvalidUtf8 = false);
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
			inline std::string ReadStd();
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
			//[Write]
			//ファイルにwriteを書き込む
			//引数1: ファイルに書き込むstring
			inline void Write(const sgstring& write);
			//----------------------------------------------------------------------------------
			//[Write]
			//ファイルにwriteを書き込む
			//引数1: ファイルに書き込むstring
			inline void Write(const std::wstring& write);
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
		//ファイルを開く,  本関数実行時点で対応コードはUTF-8となる
		//throw: ファルが開けなかった場合
		//引数1: ファイルパス(拡張子含む)
		//引数2: バイナリで開くか否か : default = false
		//引数3: 追加書き込み仕様で開くか否か : default = false
		//引数4: UTF-8指定を無効にする : default = false
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
		//ファイルを閉じる
		inline void FileStream::Close() { m_fileStream.close(); }
		//----------------------------------------------------------------------------------
		//[Read]
		//ファイルの中身を全文読み込む (char->wide変換)
		//return: ファイルの中の文字列
		inline sgstring FileStream::Read() { return sgstring(std::string(std::istreambuf_iterator<char>(m_fileStream), std::istreambuf_iterator<char>())); }
		//----------------------------------------------------------------------------------
		//[ReadUnite]
		//ファイルの中身を全文読み込む (変換ではなく, char2文字を合体させてwcharとして扱う)
		//return: ファイルの中の文字列
		inline sgstring FileStream::ReadUnite()
		{
			//1バイト文字列
			std::string read1Byte = std::string(std::istreambuf_iterator<char>(m_fileStream), std::istreambuf_iterator<char>());
			//返り値
			sgstring read2Byte;
			//wchar変換
			for (auto it = read1Byte.begin(); it != read1Byte.end();)
			{
				read2Byte.push_back(static_cast<wchar>(*(it + 1) << 8) | *it);
				++it;
				if (it != read1Byte.end()) ++it;
			}
		}
		//----------------------------------------------------------------------------------
		//[ReadStd]
		//ファイルの中身を全文読み込む, std版
		//return: ファイルの中の文字列s
		inline std::string FileStream::ReadStd() { return std::string(std::istreambuf_iterator<char>(m_fileStream), std::istreambuf_iterator<char>()); }
		//----------------------------------------------------------------------------------
		//[Read]
		//ファイルの中身を全文読み込む
		//引数1: ファイルの中の文字列を代入するstring
		inline void FileStream::Read(sgstring & string) { string.assign(sgstring(std::string(std::istreambuf_iterator<char>(m_fileStream), std::istreambuf_iterator<char>()))); }
		//----------------------------------------------------------------------------------
		//[ReadUnite]
		//ファイルの中身を全文読み込む (変換ではなく, char2文字を合体させてwcharとして扱う)
		//引数1: ファイルの中の文字列を代入するstring
		inline sgstring FileStream::ReadUnite(sgstring & string)
		{
			//1バイト文字列
			std::string read1Byte = std::string(std::istreambuf_iterator<char>(m_fileStream), std::istreambuf_iterator<char>());
			//clear
			string.clear();
			//wchar変換
			for (auto it = read1Byte.begin(); it != read1Byte.end();)
			{
				string.push_back(static_cast<wchar>(*(it + 1) << 8) | *it);
				++it;
				if (it != read1Byte.end()) ++it;
			}
		}
		//----------------------------------------------------------------------------------
		//[Read]
		//ファイルの中身を全文読み込む, std版
		//引数1: ファイルの中の文字列を代入するstring
		inline void FileStream::ReadStd(std::string & string) { string.assign(std::istreambuf_iterator<char>(m_fileStream), std::istreambuf_iterator<char>()); }
		//----------------------------------------------------------------------------------
		//[ReadVector]
		//ファイルの中身を全文読み込む, std::vector版
		//return: ファイルの中の文字列
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
		//ファイルの中身を全文読み込む, std::vector版
		//引数1: ファイルの中の文字列を代入するvector
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
		//ファイルにwriteを書き込む
		//引数1: ファイルに書き込むstring
		inline void FileStream::Write(const sgstring & write)
		{
			//変換
			int inLength = static_cast<int>(write.length());
			int outLength = WideCharToMultiByte(CP_ACP, 0, write.c_str(), inLength, 0, 0, nullptr, nullptr);
			std::string result(outLength, L'\0');
			if (outLength) WideCharToMultiByte(CP_UTF8, 0, write.c_str(), inLength, &result[0], outLength, nullptr, nullptr);
			
			//書き込み
			if (IS_FALSE(m_isBinary)) m_fileStream << result;
			else m_fileStream.write(result.c_str(), result.size());
		}
		//----------------------------------------------------------------------------------
		//[WriteSplit]
		//ファイルにwriteを書き込む (変換ではなく, wchar1文字を分割させてcharとして扱う)
		//引数1: ファイルに書き込むstring
		inline void FileStream::WriteSplit(const sgstring & write)
		{
			std::string join;
			//string変換
			for (auto& e : write)
			{
				join.push_back((char)(e >> 8));
				join.push_back((char)(e & 0xff));
			}
			//書き込み
			if (IS_FALSE(m_isBinary)) m_fileStream << join;
			else m_fileStream.write(join.c_str(), join.size());
		}
		//----------------------------------------------------------------------------------
		//[Write]
		//ファイルにwriteを書き込む , std版
		//引数1: ファイルに書き込むstring
		inline void FileStream::Write(const std::string & write) { IS_FALSE(m_isBinary) ? m_fileStream << write : m_fileStream.write(write.c_str(), write.size()); }
		//----------------------------------------------------------------------------------
		//[Write]
		//ファイルにwriteを書き込む , std版
		//引数1: ファイルに書き込むstring
		inline void FileStream::Write(const std::vector<byte>& write) { m_fileStream.write(reinterpret_cast<const char*>(write.data()), write.size()); }
		//----------------------------------------------------------------------------------
		//[IsOpen]
		//return: ファイルがthisを用いて開いているか否か
		inline bool FileStream::IsOpen() { return m_fileStream.is_open(); }

		//----------------------------------------------------------------------------------
		//[ChangeLocale]
		//Localeを変更する
		//引数1: 変更するlocale , 注意！thisのデストラクタが呼ばれるまで有効なポインタであること
		template<class _Elem, unsigned long _Mymax, std::codecvt_mode _Mymode>
		inline void FileStream::ChangeLocale(std::codecvt_utf8<_Elem, _Mymax, _Mymode>& convert) { m_fileStream->imbue(std::locale(std::locale::empty(), &convert)); }
		//----------------------------------------------------------------------------------
		//[ChangeLocale]
		//Localeを変更する
		//引数1: 変更するlocale , 注意！thisのデストラクタが呼ばれるまで有効なポインタであること
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
		//ファイルを開く,  本関数実行時点で対応コードはUTF-8となる
		//throw: ファルが開けなかった場合
		//引数1: ファイルパス(拡張子含む)
		//引数2: バイナリで開くか否か : default = false
		//引数3: 追加書き込み仕様で開くか否か : default = false
		//引数4: UTF-8指定を無効にする : default = false
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
		//ファイルを閉じる
		inline void WFileStream::Close() { m_fileStream.close(); }
		//----------------------------------------------------------------------------------
		//[Read]
		//ファイルの中身を全文読み込む
		//return: ファイルの中の文字列
		inline sgstring WFileStream::Read() { return sgstring(std::wstring(std::istreambuf_iterator<wchar>(m_fileStream), std::istreambuf_iterator<wchar>())); }
		//----------------------------------------------------------------------------------
		//[ReadStd]
		//ファイルの中身を全文読み込む, std版
		//return: ファイルの中の文字列
		inline std::string WFileStream::ReadStd() { return std::string(std::istreambuf_iterator<wchar>(m_fileStream), std::istreambuf_iterator<wchar>()); }
		//----------------------------------------------------------------------------------
		//[ReadStd]
		//ファイルの中身を全文読み込む
		//return: ファイルの中の文字列
		inline void WFileStream::Read(sgstring & string) { string.assign(sgstring(std::wstring(std::istreambuf_iterator<wchar>(m_fileStream), std::istreambuf_iterator<wchar>()))); }
		//----------------------------------------------------------------------------------
		//[ReadStd]
		//ファイルの中身を全文読み込む, std版
		//引数1: ファイルの中の文字列を代入するstring
		inline void WFileStream::ReadStd(std::wstring & string) { string.assign(std::istreambuf_iterator<wchar>(m_fileStream), std::istreambuf_iterator<wchar>()); }
		//----------------------------------------------------------------------------------
		//[Write]
		//ファイルにwriteを書き込む
		//引数1: ファイルに書き込むstring
		inline void WFileStream::Write(const sgstring & write) { IS_FALSE(m_isBinary) ? m_fileStream << write : m_fileStream.write(write.c_str(), write.size()); }
		//----------------------------------------------------------------------------------
		//[Write]
		//ファイルにwriteを書き込む
		//引数1: ファイルに書き込むstring
		inline void WFileStream::Write(const std::wstring & write) { IS_FALSE(m_isBinary) ? m_fileStream << write : m_fileStream.write(write.c_str(), write.size()); }
		//----------------------------------------------------------------------------------
		//[IsOpen]
		//return: ファイルがthisを用いて開いているか否か
		inline bool WFileStream::IsOpen() { return m_fileStream.is_open(); }

		//----------------------------------------------------------------------------------
		//[ChangeLocale]
		//Localeを変更する
		//引数1: 変更するlocale , 注意！thisのデストラクタが呼ばれるまで有効なポインタであること
		template<class _Elem, unsigned long _Mymax, std::codecvt_mode _Mymode>
		inline void WFileStream::ChangeLocale(std::codecvt_utf8<_Elem, _Mymax, _Mymode>& convert)
		{
			m_fileStream->imbue(std::locale(std::locale::empty(), &convert));
		}
		//----------------------------------------------------------------------------------
		//[ChangeLocale]
		//Localeを変更する
		//引数1: 変更するlocale , 注意！thisのデストラクタが呼ばれるまで有効なポインタであること
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