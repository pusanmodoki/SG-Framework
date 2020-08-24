/*----------------------------------------------------------------------------------
File出力を行うFileWriteStream, WFileWriteStream class
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
		//File出力を行うFileWriteStream
		//対応文字はchar文字, 規定文字コードはUTF-8
		class FileWriteStream final
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

		//File出力を行うWFileWriteStream
		//対応文字はwchar文字, 規定文字コードはUTF-8
		class WFileWriteStream final
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
		//ファイルを開く,  本関数実行時点で対応コードはUTF-8となる
		//throw: ファルが開けなかった場合
		//引数1: ファイルパス(拡張子含む)
		//引数2: バイナリで開くか否か : default = false
		//引数3: 追加書き込み仕様で開くか否か : default = false
		//引数4: UTF-8指定を無効にする : default = false
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
		//ファイルを閉じる
		inline void FileWriteStream::Close() { m_fileStream.close(); }
		//----------------------------------------------------------------------------------
		//[Write]
		//ファイルにwriteを書き込む
		//引数1: ファイルに書き込むstring
		inline void FileWriteStream::Write(const sgstring & write)
		{
			//変換
			int inLength = static_cast<int>(write.length());
			int outLength = WideCharToMultiByte(CP_ACP, 0, write.c_str(), inLength, 0, 0, nullptr, nullptr);
			std::string result(outLength, L'\0');
			if (outLength) WideCharToMultiByte(CP_UTF8, 0, write.c_str(), inLength, &result[0], outLength, nullptr, nullptr);
			//書き込み
			if (IS_FALSE(m_isBinary)) m_fileStream << result;
			else m_fileStream.write(result.c_str(), outLength);
		}
		//----------------------------------------------------------------------------------
		//[WriteSplit]
		//ファイルにwriteを書き込む (変換ではなく, wchar1文字を分割させてcharとして扱う)
		//引数1: ファイルに書き込むstring
		inline void FileWriteStream::WriteSplit(const sgstring & write)
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
		inline void FileWriteStream::Write(const std::string & write) { IS_FALSE(m_isBinary) ? m_fileStream << write : m_fileStream.write(write.c_str(), write.size()); }
		//----------------------------------------------------------------------------------
		//[Write]
		//ファイルにwriteを書き込む , std版
		//引数1: ファイルに書き込むstring
		inline void FileWriteStream::Write(const std::vector<byte>& write) { m_fileStream.write(reinterpret_cast<const char*>(write.data()), write.size()); }
		//----------------------------------------------------------------------------------
		//[IsOpen]
		//return: ファイルがthisを用いて開いているか否か
		inline bool FileWriteStream::IsOpen() { return m_fileStream.is_open(); }

		//----------------------------------------------------------------------------------
		//[ChangeLocale]
		//Localeを変更する
		//引数1: 変更するlocale , 注意！thisのデストラクタが呼ばれるまで有効なポインタであること
		template<class _Elem, unsigned long _Mymax, std::codecvt_mode _Mymode>
		inline void FileWriteStream::ChangeLocale(std::codecvt_utf8<_Elem, _Mymax, _Mymode>& convert) { m_fileStream->imbue(std::locale(std::locale::empty(), &convert)); }
		//----------------------------------------------------------------------------------
		//[ChangeLocale]
		//Localeを変更する
		//引数1: 変更するlocale , 注意！thisのデストラクタが呼ばれるまで有効なポインタであること
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
		//ファイルを開く,  本関数実行時点で対応コードはUTF-8となる
		//throw: ファルが開けなかった場合
		//引数1: ファイルパス(拡張子含む)
		//引数2: バイナリで開くか否か : default = false
			//引数4: UTF-8指定を無効にする : default = false
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
		//ファイルを閉じる
		inline void WFileWriteStream::Close() { m_fileStream.close(); }
		//----------------------------------------------------------------------------------
		//[Write]
		//ファイルにwriteを書き込む
		//引数1: ファイルに書き込むstring
		inline void WFileWriteStream::Write(const sgstring & write) { IS_FALSE(m_isBinary) ? m_fileStream << write : m_fileStream.write(write.c_str(), write.size()); }
		//----------------------------------------------------------------------------------
		//[Write]
		//ファイルにwriteを書き込む
		//引数1: ファイルに書き込むstring
		inline void WFileWriteStream::Write(const std::wstring & write) { IS_FALSE(m_isBinary) ? m_fileStream << write : m_fileStream.write(write.c_str(), write.size()); }
		//----------------------------------------------------------------------------------
		//[IsOpen]
		//return: ファイルがthisを用いて開いているか否か
		inline bool WFileWriteStream::IsOpen() { return m_fileStream.is_open(); }

		//----------------------------------------------------------------------------------
		//[ChangeLocale]
		//Localeを変更する
		//引数1: 変更するlocale , 注意！thisのデストラクタが呼ばれるまで有効なポインタであること
		template<class _Elem, unsigned long _Mymax, std::codecvt_mode _Mymode>
		inline void WFileWriteStream::ChangeLocale(std::codecvt_utf8<_Elem, _Mymax, _Mymode>& convert)
		{
			m_fileStream->imbue(std::locale(std::locale::empty(), &convert));
		}
		//----------------------------------------------------------------------------------
		//[ChangeLocale]
		//Localeを変更する
		//引数1: 変更するlocale , 注意！thisのデストラクタが呼ばれるまで有効なポインタであること
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
