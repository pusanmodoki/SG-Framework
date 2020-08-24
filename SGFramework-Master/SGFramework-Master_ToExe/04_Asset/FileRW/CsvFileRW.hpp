/*----------------------------------------------------------------------------------
CSV形式で読み書きするCsvFileRW namespace
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_CSV_FILE_RW_HPP_
#define SGFRAMEWORK_HEADER_CSV_FILE_RW_HPP_
#include <fstream>
#include <sstream>
#include <iterator>
#include "../../02_CustomVariable/CustomVariable.hpp"
#include "EncryptionKey.hpp"
#include "../FileStream/FileStream.hpp"
#include "FileText.hpp"

// Framework namespace
namespace SGFramework
{
	//File Edit
	namespace File
	{
		//CSV File
		namespace CsvFileRW
		{
			//csv拡張子
			constexpr wchar cCsvExtension[] = L".csv";

			//----------------------------------------------------------------------------------
			//[Read]
			//CSV形式でファイルを読み込む
			//return: 読み込んだ内容
			//引数1: ファイルパス (拡張子含む)
			//引数2: バイナリ読み込み, default = false
			//引数3: 拡張子チェック, default = true
			inline FileText Read(const sgstring& path, bool isBinary = false, bool isCheckExtension = true)
			{
				WFileReadStream frs;//ファイルストリーム

				//拡張子チェック
				if (isCheckExtension && !sgstring::functions::IsEndWith(path, cCsvExtension))
					throw InvalidArgument(L"Error!! CsvFileRW->Read",
						L"Extension NotFound: " + path + L" Read extension csv");

				//ファイルを開けなかった場合はthrow返す
				try { frs.Open(path); }
				catch (...) { throw; }

				sgstring getline;		//getline用バッファ
				uint lineCount = 0;	//行数
				//file中身
				std::wstring readBuf = frs.Read();
				//stringstream
				std::wstringstream read(readBuf);

				frs.Close();		//ファイルを閉じる

				//全行getline, 行数計算
				while (std::getline(read, getline))
					lineCount++;

				read.clear();							//clear
				read.seekg(0, std::ios::beg);	//seek
				FileText result(lineCount);		//返り値

				//全行getline, vectorバッファに1行の要素を分割代入
				while (std::getline(read, getline))
					result.emplace_back(sgstring::functions::Split(getline, ',', true));
				
				
				//return
				return result;
			}

			//----------------------------------------------------------------------------------
			//[Read]
			//テキスト形式でファイルを読み込む
			//引数1: ファイルパス (拡張子含む)
			//引数2: 読み込んだ内容の代入先
			//引数3: バイナリ読み込み, default = false
			//引数4: 拡張子チェック, default = true
			inline void Read(const sgstring& path, FileText& write, bool isBinary = false, bool isCheckExtension = true)
			{
				FileReadStream frs;//ファイルストリーム

				//拡張子チェック
				if (isCheckExtension && !sgstring::functions::IsEndWith(path, cCsvExtension))
					throw InvalidArgument(L"Error!! CsvFileRW->Read",
						L"Extension NotFound: " + path + L" Read extension csv");

				//ファイルを開けなかった場合はthrow返す
				try { frs.Open(path, true); }
				catch (...) { throw; }

				//暗号化解除後文字列
				sgstring readUnlcok = frs.ReadUnite();
				//解除
				readUnlcok = EncryptionKey::Unlock(readUnlcok, EncryptionKey::DefaultKey());
				frs.Close();		//ファイルを閉じる

				std::wstringstream read(readUnlcok);		//stringstream
				sgstring getline;										//getline用バッファ
				uint lineCount = 0;									//行数カウント

				//全行getline, 行数計算
				while (std::getline(read, getline))
					lineCount++;
				
				read.clear();							//clear
				read.seekg(0, std::ios::beg);	//seek
				write.clear();							//clear
				write.reserve(lineCount);		//get of lineCount

				//全行getline
				while (std::getline(read, getline))
					//vectorバッファに1行の要素を分割代入
					write.emplace_back(sgstring::functions::Split(getline, ',', true));
			}

			//----------------------------------------------------------------------------------
			//[Write]
			//テキスト形式でファイルを書き込む
			//引数1: ファイルパス (拡張子含む)
			//引数2: 書き込む内容 
			//引数3: バイナリ書き込み, default = false
			//引数4: 追加書き込み, default = false
			//引数5: 拡張子チェック, default = true
			inline void Write(const sgstring& path, const FileText& write, bool isBinary = false, bool isAdd = false, bool isCheckExtension = true)
			{
				WFileWriteStream fws;

				//拡張子チェック
				if (isCheckExtension && !sgstring::functions::IsEndWith(path, cCsvExtension))
					throw InvalidArgument(L"Error!! CsvFileRW->Write",
						L"Extension NotFound: " + path + L" Read extension csv");

				//ファイルを開けなかった場合はthrow返す
				try { fws.Open(path, false, isAdd); }
				catch (...) { throw; }

				//全要素ループ
				for (const auto& it : write)
				{
					//書き込み用stringに1要素の書き込む内容を連結して代入
					sgstring string = sgstring::functions::Join(it, ',', true);
					fws.Write(string);		//ファイルに書き込み
				}
				fws.Close();			//ファイルを閉じる
			}
		}
	}
}
#endif // !SGFRAMEWORK_HEADER_CSV_FILE_RW_HPP_
