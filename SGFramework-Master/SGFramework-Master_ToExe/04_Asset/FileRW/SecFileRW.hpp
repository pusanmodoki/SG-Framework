/*----------------------------------------------------------------------------------
独自のSgframework  Encrypted Csv Locked file を扱うSecFileRW namespace
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_SEC_FILE_RW_HPP_
#define SGFRAMEWORK_HEADER_SEC_FILE_RW_HPP_
#include <fstream>
#include <sstream>
#include <iterator>
#include <codecvt>
#include <locale>
#include <fstream>
#include <algorithm>
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
		//Sgframework  Encrypted Csv Locked file
		namespace SecFileRW
		{
			//secl拡張子
			constexpr wchar cSecLockedExtension[] = L".secl";
			//sec拡張子
			constexpr wchar cSecNotLockedExtension[] = L".sec";

			//----------------------------------------------------------------------------------
			//[Read]
			//sec or seclファイルを読み込む
			//throw: 拡張子が異なる場合 
			//return: 読み込んだ内容
			//引数1: ファイルパス (拡張子含む)
			inline FileText Read(const sgstring& path);
			//----------------------------------------------------------------------------------
			//[ReadLocked]
			//seclファイルを読み込む
			//throw: 拡張子が異なる場合 
			//return: 読み込んだ内容
			//引数1: ファイルパス (拡張子含む)
			//引数2: 拡張子チェックを行うか否か, default = true
			inline FileText ReadLocked(const sgstring& path, bool isCheckExtension = true);
			//----------------------------------------------------------------------------------
			//[ReadNotLocked]
			//secファイルを読み込む
			//throw: 拡張子が異なる場合 
			//return: 読み込んだ内容
			//引数1: ファイルパス (拡張子含む)
			//引数2: 拡張子チェックを行うか否か, default = true
			inline FileText ReadNotLocked(const sgstring& path, bool isCheckExtension = true);

			//----------------------------------------------------------------------------------
			//[Read]
			//sec or seclファイルを読み込む
			//throw: 拡張子が異なる場合 
			//引数1: ファイルパス (拡張子含む)
			//引数2: 読み込んだ内容の代入先
			inline void Read(const sgstring& path, FileText& write);
			//----------------------------------------------------------------------------------
			//[ReadLocked]
			//seclファイルを読み込む
			//throw: 拡張子が異なる場合 
			//引数1: ファイルパス (拡張子含む)
			//引数2: 読み込んだ内容の代入先
			//引数3: 拡張子チェックを行うか否か, default = true
			inline void ReadLocked(const sgstring& path, FileText& write, bool isCheckExtension = true);
			//----------------------------------------------------------------------------------
			//[ReadNotLocked]
			//secファイルを読み込む
			//throw: 拡張子が異なる場合 
			//引数1: ファイルパス (拡張子含む)
			//引数2: 読み込んだ内容の代入先
			//引数3: 拡張子チェックを行うか否か, default = true
			inline void ReadNotLocked(const sgstring& path, FileText& write, bool isCheckExtension = true);

			//----------------------------------------------------------------------------------
			//[Write]
			//sec or seclファイルを書き込む
			//throw: 拡張子が異なる場合 
			//引数1: ファイルパス (拡張子含む)
			//引数2: 書き込む内容
			//引数3: 追加書き込みモードならtrue, default = false
			inline void Write(const sgstring& path, const FileText& write, bool isAdd = false);
			//----------------------------------------------------------------------------------
			//[WriteLocked]
			//seclファイルを書き込む
			//throw: 拡張子が異なる場合 
			//引数1: ファイルパス (拡張子含む)
			//引数2: 書き込む内容
			//引数3: 追加書き込みモードならtrue, default = false
			//引数4: 拡張子チェックを行うか否か, default = true
			inline void WriteLocked(const sgstring& path, const FileText& write, bool isAdd = false, bool isCheckExtension = true);
			//----------------------------------------------------------------------------------
			//[WriteNotLocked]
			//secファイルを書き込む
			//throw: 拡張子が異なる場合 
			//引数1: ファイルパス (拡張子含む)
			//引数2: 書き込む内容
			//引数3: 追加書き込みモードならtrue, default = false
			//引数4: 拡張子チェックを行うか否か, default = true
			inline void WriteNotLocked(const sgstring& path, const FileText& write, bool isAdd = false, bool isCheckExtension = true);



			//----------------------------------------------------------------------------------
			//[Read]
			//sec or seclファイルを読み込む
			//throw: 拡張子が異なる場合 
			//return: 読み込んだ内容
			//引数1: ファイルパス (拡張子含む)
			inline FileText Read(const sgstring& path)
			{
				if (sgstring::functions::IsEndWith(path, cSecLockedExtension))
					return ReadLocked(path, false);
				else if (sgstring::functions::IsEndWith(path, cSecNotLockedExtension))
					return ReadNotLocked(path, false);
				else
					throw InvalidArgument(L"Error!! SecFileRW->Read", L"Extension NotFound: " + path + L" Read extension sec or secl");
			}
			//----------------------------------------------------------------------------------
			//[ReadLocked]
			//seclファイルを読み込む
			//throw: 拡張子が異なる場合 
			//return: 読み込んだ内容
			//引数1: ファイルパス (拡張子含む)
			//引数2: 拡張子チェックを行うか否か, default = true
			inline FileText ReadLocked(const sgstring& path, bool isCheckExtension)
			{
				FileReadStream frs;//ファイルストリーム

				//拡張子チェック
				if (isCheckExtension && !sgstring::functions::IsEndWith(path, cSecNotLockedExtension))
					throw InvalidArgument(L"Error!! SecFileRW->ReadNotLocked",
						L"Extension NotFound: " + path + L" Read extension sec");

				//ファイルを開けなかった場合はthrow返す
				try { frs.Open(path, true); }
				catch (...) { throw; }

				//暗号化解除後文字列
				sgstring readUnlcok;
				//解除
				EncryptionKey::Unlock(readUnlcok, frs.ReadUnite(), EncryptionKey::DefaultKey());
				frs.Close();		//ファイルを閉じる

				sgstring getline;		//getline用バッファ
				uint lineCount = 0;	//行数
				//stringstream
				std::wstringstream read(readUnlcok);

				//全行getline, 行数計算
				while (std::getline(read, getline))
					lineCount++;

				read.clear();							//clear
				read.seekg(0, std::ios::beg);	//seek
				FileText result(lineCount);		//return

				//全行getline, vectorバッファに1行の要素を分割代入
				while (std::getline(read, getline))
					result.emplace_back(sgstring::functions::Split(getline, ',', true));
			}
			//----------------------------------------------------------------------------------
			//[ReadNotLocked]
			//secファイルを読み込む
			//throw: 拡張子が異なる場合 
			//return: 読み込んだ内容
			//引数1: ファイルパス (拡張子含む)
			//引数2: 拡張子チェックを行うか否か, default = true
			inline FileText ReadNotLocked(const sgstring& path, bool isCheckExtension)
			{
				WFileReadStream frs;//ファイルストリーム

				//拡張子チェック
				if(isCheckExtension && !sgstring::functions::IsEndWith(path, cSecNotLockedExtension))
					throw InvalidArgument(L"Error!! SecFileRW->ReadNotLocked", 
						L"Extension NotFound: " + path + L" Read extension sec");

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
				FileText result(lineCount);		//return

				//全行getline, vectorバッファに1行の要素を分割代入
				while (std::getline(read, getline))
					result.emplace_back(sgstring::functions::Split(getline, ',', true));
			}

			//----------------------------------------------------------------------------------
			//[Read]
			//sec or seclファイルを読み込む
			//throw: 拡張子が異なる場合 
			//引数1: ファイルパス (拡張子含む)
			//引数2: 読み込んだ内容の代入先
			inline void Read(const sgstring& path, FileText& write)
			{
				if (sgstring::functions::IsEndWith(path, cSecLockedExtension))
					ReadLocked(path, write, false);
				else if (sgstring::functions::IsEndWith(path, cSecNotLockedExtension))
					ReadNotLocked(path, write, false);
				else
					throw InvalidArgument(L"Error!! SecFileRW->Read", L"Extension NotFound: " + path + L" Read extension sec or secl");
			}
			//----------------------------------------------------------------------------------
			//[ReadLocked]
			//seclファイルを読み込む
			//throw: 拡張子が異なる場合 
			//引数1: ファイルパス (拡張子含む)
			//引数2: 読み込んだ内容の代入先
			//引数3: 拡張子チェックを行うか否か, default = true
			inline void ReadLocked(const sgstring& path, FileText& write, bool isCheckExtension)
			{
				FileReadStream frs;//ファイルストリーム

				//拡張子チェック
				if (isCheckExtension && !sgstring::functions::IsEndWith(path, cSecNotLockedExtension))
					throw InvalidArgument(L"Error!! SecFileRW->ReadNotLocked",
						L"Extension NotFound: " + path + L" Read extension sec");

				//ファイルを開けなかった場合はthrow返す
				try { frs.Open(path, true); }
				catch (...) { throw; }

				//暗号化解除後文字列
				sgstring readUnlcok;
				//解除
				EncryptionKey::Unlock(readUnlcok, frs.ReadUnite(), EncryptionKey::DefaultKey());
				frs.Close();		//ファイルを閉じる

				sgstring getline;		//getline用バッファ
				uint lineCount = 0;	//行数
				//stringstream
				std::wstringstream read(readUnlcok);
		
				//全行getline, 行数計算
				while (std::getline(read, getline))
					lineCount++;

				read.clear();							//clear
				read.seekg(0, std::ios::beg);	//seek
				write.clear();							//clear
				write.reserve(lineCount);		//get of lineCount

				//全行getline, vectorバッファに1行の要素を分割代入
				while (std::getline(read, getline))
					write.emplace_back(sgstring::functions::Split(getline, ',', true));
			}
			//----------------------------------------------------------------------------------
			//[ReadNotLocked]
			//secファイルを読み込む
			//throw: 拡張子が異なる場合 
			//引数1: ファイルパス (拡張子含む)
			//引数2: 読み込んだ内容の代入先
			//引数3: 拡張子チェックを行うか否か, default = true
			inline void ReadNotLocked(const sgstring& path, FileText& write, bool isCheckExtension)
			{
				WFileReadStream frs;//ファイルストリーム

				//拡張子チェック
				if (isCheckExtension && !sgstring::functions::IsEndWith(path, cSecLockedExtension))
					throw InvalidArgument(L"Error!! SecFileRW->ReadLocked",
						L"Extension NotFound: " + path + L" Read extension secl");

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
				write.clear();							//clear
				write.reserve(lineCount);		//get of lineCount

				//全行getline, vectorバッファに1行の要素を分割代入
				while (std::getline(read, getline))
				{
					auto string = sgstring::functions::Split(getline, ',', true);
					if(IS_FALSE(string.empty())) write.emplace_back(std::move(string));
				}
			}

			//----------------------------------------------------------------------------------
			//[Write]
			//sec or seclファイルを書き込む
			//throw: 拡張子が異なる場合 
			//引数1: ファイルパス (拡張子含む)
			//引数2: 書き込む内容
			//引数3: 追加書き込みモードならtrue, default = false
			inline void Write(const sgstring& path, const FileText& write, bool isAdd)
			{
				if (sgstring::functions::IsEndWith(path, cSecLockedExtension))
					WriteLocked(path, write, isAdd, false);
				else if (sgstring::functions::IsEndWith(path, cSecNotLockedExtension))
					WriteNotLocked(path, write, isAdd, false);
				else
					throw InvalidArgument(L"Error!! SecFileRW->Write", L"Extension NotFound: " + path + L" Write extension sec or secl");
			}
			//----------------------------------------------------------------------------------
			//[WriteLocked]
			//seclファイルを書き込む
			//throw: 拡張子が異なる場合 
			//引数1: ファイルパス (拡張子含む)
			//引数2: 書き込む内容
			//引数3: 追加書き込みモードならtrue, default = false
			//引数4: 拡張子チェックを行うか否か, default = true
			inline void WriteLocked(const sgstring& path, const FileText& write, bool isAdd, bool isCheckExtension)
			{
				FileWriteStream fws;

				//拡張子チェック
				if (isCheckExtension && !sgstring::functions::IsEndWith(path, cSecLockedExtension))
					throw InvalidArgument(L"Error!! SecFileRW->WriteLocked",
						L"Extension NotFound: " + path + L" Read extension secl");

				//ファイルを開けなかった場合はthrow返す
				try { fws.Open(path, true, isAdd); }
				catch (...) { throw; }

				std::string join;
				//全要素ループ
				for (auto& it : write)
				{
					//書き込み用stringに1要素の書き込む内容を連結して代入
					fws.WriteSplit(EncryptionKey::Lock(sgstring::functions::Join(it, ',', true), EncryptionKey::DefaultKey()));
				}
				fws.Close();			//ファイルを閉じる
			}
			//----------------------------------------------------------------------------------
			//[WriteNotLocked]
			//secファイルを書き込む
			//throw: 拡張子が異なる場合 
			//引数1: ファイルパス (拡張子含む)
			//引数2: 書き込む内容
			//引数3: 追加書き込みモードならtrue, default = false
			//引数4: 拡張子チェックを行うか否か, default = true
			inline void WriteNotLocked(const sgstring& path, const FileText& write, bool isAdd, bool isCheckExtension)
			{
				WFileWriteStream fws;

				//拡張子チェック
				if (isCheckExtension && !sgstring::functions::IsEndWith(path, cSecNotLockedExtension))
					throw InvalidArgument(L"Error!! SecFileRW->WriteNotLocked",
						L"Extension NotFound: " + path + L" Read extension sec");

				//ファイルを開けなかった場合はthrow返す
				try { fws.Open(path, false, isAdd); }
				catch (...) { throw; }
				
				//全要素ループ
				for (const auto& it : write)
				{
					//書き込み用stringに1要素の書き込む内容を連結して代入
					fws.Write(sgstring::functions::Join(it, ',', true));		//ファイルに書き込み
				}
				fws.Close();			//ファイルを閉じる
			}
		};


		}
	}


#endif // !SGFRAMEWORK_HEADER_SEC_FILE_RW_HPP_