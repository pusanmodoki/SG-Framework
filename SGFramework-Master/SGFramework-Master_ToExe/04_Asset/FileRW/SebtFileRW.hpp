/*----------------------------------------------------------------------------------
独自のSgframework  Encrypted Binary Text Locked file を扱うSebtFileRW namespace
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_SEBT_FILE_RW_HPP_
#define SGFRAMEWORK_HEADER_SEBT_FILE_RW_HPP_
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

// Framework namespace
namespace SGFramework
{
	//File Edit
	namespace File
	{
		//Sgframework  Encrypted Binary Text Locked file
		namespace SebtFileRW
		{
			//secl拡張子
			constexpr wchar cSebtLockedExtension[] = L".sebtl";
			//sec拡張子
			constexpr wchar cSebtNotLockedExtension[] = L".sebt";

			//----------------------------------------------------------------------------------
			//[Read]
			//sec or seclファイルを読み込む
			//throw: 拡張子が異なる場合 
			//return: 読み込んだ内容
			//引数1: ファイルパス (拡張子含む)
			inline sgstring Read(const sgstring& path);
			//----------------------------------------------------------------------------------
			//[ReadLocked]
			//sebtlファイルを読み込む
			//throw: 拡張子が異なる場合 
			//return: 読み込んだ内容
			//引数1: ファイルパス (拡張子含む)
			//引数2: 拡張子チェックを行うか否か, default = true
			inline sgstring ReadLocked(const sgstring& path, bool isCheckExtension = true);
			//----------------------------------------------------------------------------------
			//[ReadNotLocked]
			//sebtファイルを読み込む
			//throw: 拡張子が異なる場合 
			//return: 読み込んだ内容
			//引数1: ファイルパス (拡張子含む)
			//引数2: 拡張子チェックを行うか否か, default = true
			inline sgstring ReadNotLocked(const sgstring& path, bool isCheckExtension = true);

			//----------------------------------------------------------------------------------
			//[Read]
			//sebt or sebtlファイルを読み込む
			//throw: 拡張子が異なる場合 
			//引数1: ファイルパス (拡張子含む)
			//引数2: 読み込んだ内容の代入先
			inline void Read(const sgstring& path, sgstring& write);
			//----------------------------------------------------------------------------------
			//[ReadLocked]
			//sebtlファイルを読み込む
			//throw: 拡張子が異なる場合 
			//引数1: ファイルパス (拡張子含む)
			//引数2: 読み込んだ内容の代入先
			//引数3: 拡張子チェックを行うか否か, default = true
			inline void ReadLocked(const sgstring& path, sgstring& write, bool isCheckExtension = true);
			//----------------------------------------------------------------------------------
			//[ReadNotLocked]
			//sebtファイルを読み込む
			//throw: 拡張子が異なる場合 
			//引数1: ファイルパス (拡張子含む)
			//引数2: 読み込んだ内容の代入先
			//引数3: 拡張子チェックを行うか否か, default = true
			inline void ReadNotLocked(const sgstring& path, sgstring& write, bool isCheckExtension = true);


			//----------------------------------------------------------------------------------
			//[Read]
			//sebt or sebtlファイルを読み込む
			//throw: 拡張子が異なる場合 
			//return: 読み込んだ内容
			//引数1: ファイルパス (拡張子含む)
			inline std::vector<byte> ReadBinary(const sgstring& path);
			//----------------------------------------------------------------------------------
			//[ReadLocked]
			//sebtlファイルを読み込む
			//throw: 拡張子が異なる場合 
			//return: 読み込んだ内容
			//引数1: ファイルパス (拡張子含む)
			//引数2: 拡張子チェックを行うか否か, default = true
			inline std::vector<byte> ReadBinaryLocked(const sgstring& path, bool isCheckExtension = true);
			//----------------------------------------------------------------------------------
			//[ReadNotLocked]
			//sebtファイルを読み込む
			//throw: 拡張子が異なる場合 
			//return: 読み込んだ内容
			//引数1: ファイルパス (拡張子含む)
			//引数2: 拡張子チェックを行うか否か, default = true
			inline std::vector<byte> ReadBinaryNotLocked(const sgstring& path, bool isCheckExtension = true);

			//----------------------------------------------------------------------------------
			//[Read]
			//sebt or sebtlファイルを読み込む
			//throw: 拡張子が異なる場合 
			//引数1: ファイルパス (拡張子含む)
			//引数2: 読み込んだ内容の代入先
			inline void ReadBinary(const sgstring& path, std::vector<byte>& write);
			//----------------------------------------------------------------------------------
			//[ReadLocked]
			//sebtlファイルを読み込む
			//throw: 拡張子が異なる場合 
			//引数1: ファイルパス (拡張子含む)
			//引数2: 読み込んだ内容の代入先
			//引数3: 拡張子チェックを行うか否か, default = true
			inline void ReadBinaryLocked(const sgstring& path, std::vector<byte>& write, bool isCheckExtension = true);
			//----------------------------------------------------------------------------------
			//[ReadNotLocked]
			//sebtファイルを読み込む
			//throw: 拡張子が異なる場合 
			//引数1: ファイルパス (拡張子含む)
			//引数2: 読み込んだ内容の代入先
			//引数3: 拡張子チェックを行うか否か, default = true
			inline void ReadBinaryNotLocked(const sgstring& path, std::vector<byte>& write, bool isCheckExtension = true);

			//----------------------------------------------------------------------------------
			//[Write]
			//sebt or sebtlファイルを書き込む
			//throw: 拡張子が異なる場合 
			//引数1: ファイルパス (拡張子含む)
			//引数2: 書き込む内容
			//引数3: 追加書き込みモードならtrue, default = false
			inline void Write(const sgstring& path, const sgstring& write, bool isAdd = false);
			//----------------------------------------------------------------------------------
			//[WriteLocked]
			//sebtlファイルを書き込む
			//throw: 拡張子が異なる場合 
			//引数1: ファイルパス (拡張子含む)
			//引数2: 書き込む内容
			//引数3: 追加書き込みモードならtrue, default = false
			//引数4: 拡張子チェックを行うか否か, default = true
			inline void WriteLocked(const sgstring& path, const sgstring& write, bool isAdd = false, bool isCheckExtension = true);
			//----------------------------------------------------------------------------------
			//[WriteNotLocked]
			//sebtファイルを書き込む
			//throw: 拡張子が異なる場合 
			//引数1: ファイルパス (拡張子含む)
			//引数2: 書き込む内容
			//引数3: 追加書き込みモードならtrue, default = false
			//引数4: 拡張子チェックを行うか否か, default = true
			inline void WriteNotLocked(const sgstring& path, const sgstring& write, bool isAdd = false, bool isCheckExtension = true);

			//----------------------------------------------------------------------------------
			//[WriteBinary]
			//sebt or sebtlファイルを書き込む
			//throw: 拡張子が異なる場合 
			//引数1: ファイルパス (拡張子含む)
			//引数2: 書き込む内容
			//引数3: 追加書き込みモードならtrue, default = false
			inline void WriteBinary(const sgstring& path, const std::vector<byte>& write, bool isAdd = false);
			//----------------------------------------------------------------------------------
			//[WriteBinaryLocked]
			//sebtlファイルを書き込む
			//throw: 拡張子が異なる場合 
			//引数1: ファイルパス (拡張子含む)
			//引数2: 書き込む内容
			//引数3: 追加書き込みモードならtrue, default = false
			//引数4: 拡張子チェックを行うか否か, default = true
			inline void WriteBinaryLocked(const sgstring& path, const std::vector<byte>& write, bool isAdd = false, bool isCheckExtension = true);
			//----------------------------------------------------------------------------------
			//[WriteBinaryNotLocked]
			//sebtファイルを書き込む
			//throw: 拡張子が異なる場合 
			//引数1: ファイルパス (拡張子含む)
			//引数2: 書き込む内容
			//引数3: 追加書き込みモードならtrue, default = false
			//引数4: 拡張子チェックを行うか否か, default = true
			inline void WriteBinaryNotLocked(const sgstring& path, const std::vector<byte>& write, bool isAdd = false, bool isCheckExtension = true);
		

			//----------------------------------------------------------------------------------
			//[Read]
			//sebt or sebtlファイルを読み込む
			//throw: 拡張子が異なる場合 
			//return: 読み込んだ内容
			//引数1: ファイルパス (拡張子含む)
			inline sgstring Read(const sgstring& path)
			{
				if (sgstring::functions::IsEndWith(path, cSebtLockedExtension))
					return ReadLocked(path, false);
				else if (sgstring::functions::IsEndWith(path, cSebtNotLockedExtension))
					return ReadNotLocked(path, false);
				else
					throw InvalidArgument(L"Error!! SebtFileRW->Read", L"Extension NotFound: " + path + L" Read extension sebt or sebtl");
			}
			//----------------------------------------------------------------------------------
			//[ReadLocked]
			//sebtlファイルを読み込む
			//throw: 拡張子が異なる場合 
			//return: 読み込んだ内容
			//引数1: ファイルパス (拡張子含む)
			//引数2: 拡張子チェックを行うか否か, default = true
			inline sgstring ReadLocked(const sgstring& path, bool isCheckExtension)
			{
				FileReadStream frs;//ファイルストリーム

				//拡張子チェック
				if (isCheckExtension && !sgstring::functions::IsEndWith(path, cSebtLockedExtension))
					throw InvalidArgument(L"Error!! SebtFileRW->ReadLocked",
						L"Extension NotFound: " + path + L" Read extension sebtl");

				//ファイルを開けなかった場合はthrow返す
				try { frs.Open(path, true); }
				catch (...) { throw; }

				//暗号化解除後文字列
				sgstring readUnlcok = frs.ReadUnite();
				frs.Close();		//ファイルを閉じる
				//解除
				return EncryptionKey::Unlock(readUnlcok, EncryptionKey::DefaultKey());
			}
			//----------------------------------------------------------------------------------
			//[ReadNotLocked]
			//sebtファイルを読み込む
			//throw: 拡張子が異なる場合 
			//return: 読み込んだ内容
			//引数1: ファイルパス (拡張子含む)
			//引数2: 拡張子チェックを行うか否か, default = true
			inline sgstring ReadNotLocked(const sgstring& path, bool isCheckExtension)
			{
				WFileReadStream frs;//ファイルストリーム

				//拡張子チェック
				if (isCheckExtension && !sgstring::functions::IsEndWith(path, cSebtNotLockedExtension))
					throw InvalidArgument(L"Error!! SebtFileRW->ReadNotLocked",
						L"Extension NotFound: " + path + L" Read extension sebt");

				//ファイルを開けなかった場合はthrow返す
				try { frs.Open(path); }
				catch (...) { throw; }

				return frs.Read();
			}

			//----------------------------------------------------------------------------------
			//[Read]
			//sec or seclファイルを読み込む
			//throw: 拡張子が異なる場合 
			//引数1: ファイルパス (拡張子含む)
			//引数2: 読み込んだ内容の代入先
			inline void Read(const sgstring& path, sgstring& write)
			{
				if (sgstring::functions::IsEndWith(path, cSebtLockedExtension))
					ReadLocked(path, write, false);
				else if (sgstring::functions::IsEndWith(path, cSebtNotLockedExtension))
					ReadNotLocked(path, write, false);
				else
					throw InvalidArgument(L"Error!! SebtFileRW->Read", L"Extension NotFound: " + path + L" Read extension sebt or sebtl");
			}
			//----------------------------------------------------------------------------------
			//[ReadLocked]
			//sebtlファイルを読み込む
			//throw: 拡張子が異なる場合 
			//引数1: ファイルパス (拡張子含む)
			//引数2: 読み込んだ内容の代入先
			//引数3: 拡張子チェックを行うか否か, default = true
			inline void ReadLocked(const sgstring& path, sgstring& write, bool isCheckExtension)
			{
				FileReadStream frs;//ファイルストリーム

				//拡張子チェック
				if (isCheckExtension && !sgstring::functions::IsEndWith(path, cSebtLockedExtension))
					throw InvalidArgument(L"Error!! SebtFileRW->ReadLocked",
						L"Extension NotFound: " + path + L" Read extension sebtl");

				//ファイルを開けなかった場合はthrow返す
				try { frs.Open(path, true); }
				catch (...) { throw; }

				//暗号化解除後文字列
				sgstring readUnlcok;
				frs.ReadUnite(readUnlcok);
				frs.Close();		//ファイルを閉じる
				//解除
				EncryptionKey::Unlock(write, readUnlcok, EncryptionKey::DefaultKey());
			}
			//----------------------------------------------------------------------------------
			//[ReadNotLocked]
			//sebtファイルを読み込む
			//throw: 拡張子が異なる場合 
			//引数1: ファイルパス (拡張子含む)
			//引数2: 読み込んだ内容の代入先
			//引数3: 拡張子チェックを行うか否か, default = true
			inline void ReadNotLocked(const sgstring& path, sgstring& write, bool isCheckExtension)
			{
				WFileReadStream frs;//ファイルストリーム

				//拡張子チェック
				if (isCheckExtension && !sgstring::functions::IsEndWith(path, cSebtNotLockedExtension))
					throw InvalidArgument(L"Error!! SebtFileRW->ReadNotLocked",
						L"Extension NotFound: " + path + L" Read extension sebt");

				//ファイルを開けなかった場合はthrow返す
				try { frs.Open(path); }
				catch (...) { throw; }

				frs.Read(write);
			}

			//----------------------------------------------------------------------------------
			//[Read]
			//sebt or sebtlファイルを読み込む
			//throw: 拡張子が異なる場合 
			//return: 読み込んだ内容
			//引数1: ファイルパス (拡張子含む)
			std::vector<byte> ReadBinary(const sgstring & path)
			{				
				if (sgstring::functions::IsEndWith(path, cSebtLockedExtension))
					return ReadBinaryLocked(path, false);
				else if (sgstring::functions::IsEndWith(path, cSebtNotLockedExtension))
					return ReadBinaryNotLocked(path, false);
				else
					throw InvalidArgument(L"Error!! SebtFileRW->ReadBinary", L"Extension NotFound: " + path + L" Read extension sebt or sebtl");
			}
			//----------------------------------------------------------------------------------
			//[ReadLocked]
			//sebtlファイルを読み込む
			//throw: 拡張子が異なる場合 
			//return: 読み込んだ内容
			//引数1: ファイルパス (拡張子含む)
			//引数2: 拡張子チェックを行うか否か, default = true
			std::vector<byte> ReadBinaryLocked(const sgstring & path, bool isCheckExtension)
			{
				FileReadStream frs;//ファイルストリーム

				//拡張子チェック
				if (isCheckExtension && !sgstring::functions::IsEndWith(path, cSebtLockedExtension))
					throw InvalidArgument(L"Error!! SebtFileRW->ReadBinaryLocked",
						L"Extension NotFound: " + path + L" Read extension sebtl");

				//ファイルを開けなかった場合はthrow返す
				try { frs.Open(path, true); }
				catch (...) { throw; }

				//暗号化解除後文字列
				std::vector<byte> readUnlcok = frs.ReadVector();
				frs.Close();		//ファイルを閉じる
				//解除
				return EncryptionKey::UnlockBinary(readUnlcok, EncryptionKey::DefaultKey());
			}
			//----------------------------------------------------------------------------------
			//[ReadNotLocked]
			//sebtファイルを読み込む
			//throw: 拡張子が異なる場合 
			//return: 読み込んだ内容
			//引数1: ファイルパス (拡張子含む)
			//引数2: 拡張子チェックを行うか否か, default = true
			std::vector<byte> ReadBinaryNotLocked(const sgstring & path, bool isCheckExtension)
			{
				FileReadStream frs;//ファイルストリーム

				//拡張子チェック
				if (isCheckExtension && !sgstring::functions::IsEndWith(path, cSebtNotLockedExtension))
					throw InvalidArgument(L"Error!! SebtFileRW->ReadBinaryNotLocked",
						L"Extension NotFound: " + path + L" Read extension sebt");

				//ファイルを開けなかった場合はthrow返す
				try { frs.Open(path, true); }
				catch (...) { throw; }

				return std::move(frs.ReadVector());
			}

			//----------------------------------------------------------------------------------
			//[Read]
			//sebt or sebtlファイルを読み込む
			//throw: 拡張子が異なる場合 
			//引数1: ファイルパス (拡張子含む)
			//引数2: 読み込んだ内容の代入先
			void ReadBinary(const sgstring & path, std::vector<byte> & write)
			{
				if (sgstring::functions::IsEndWith(path, cSebtLockedExtension))
					ReadBinaryLocked(path, write, false);
				else if (sgstring::functions::IsEndWith(path, cSebtNotLockedExtension))
					ReadBinaryNotLocked(path, write, false);
				else
					throw InvalidArgument(L"Error!! SebtFileRW->ReadBinary", L"Extension NotFound: " + path + L" Read extension sebt or sebtl");
			}
			//----------------------------------------------------------------------------------
			//[ReadLocked]
			//sebtlファイルを読み込む
			//throw: 拡張子が異なる場合 
			//引数1: ファイルパス (拡張子含む)
			//引数2: 読み込んだ内容の代入先
			//引数3: 拡張子チェックを行うか否か, default = true
			void ReadBinaryLocked(const sgstring & path, std::vector<byte> & write, bool isCheckExtension)
			{
				FileReadStream frs;//ファイルストリーム

				//拡張子チェック
				if (isCheckExtension && !sgstring::functions::IsEndWith(path, cSebtLockedExtension))
					throw InvalidArgument(L"Error!! SebtFileRW->ReadBinaryLocked",
						L"Extension NotFound: " + path + L" Read extension sebtl");

				//ファイルを開けなかった場合はthrow返す
				try { frs.Open(path, true); }
				catch (...) { throw; }

				//暗号化解除後文字列
				std::vector<byte> readUnlcok = frs.ReadVector();
				frs.Close();		//ファイルを閉じる
				//解除
				EncryptionKey::UnlockBinary(write, readUnlcok, EncryptionKey::DefaultKey());
			}
			//----------------------------------------------------------------------------------
			//[ReadNotLocked]
			//sebtファイルを読み込む
			//throw: 拡張子が異なる場合 
			//引数1: ファイルパス (拡張子含む)
			//引数2: 読み込んだ内容の代入先
			//引数3: 拡張子チェックを行うか否か, default = true
			void ReadBinaryNotLocked(const sgstring & path, std::vector<byte> & write, bool isCheckExtension)
			{
				FileReadStream frs;//ファイルストリーム

				//拡張子チェック
				if (isCheckExtension && !sgstring::functions::IsEndWith(path, cSebtNotLockedExtension))
					throw InvalidArgument(L"Error!! SebtFileRW->ReadNotLocked",
						L"Extension NotFound: " + path + L" Read extension sebt");

				//ファイルを開けなかった場合はthrow返す
				try { frs.Open(path, true); }
				catch (...) { throw; }

				frs.ReadVector(write);
			}

			//----------------------------------------------------------------------------------
			//[Write]
			//sebt or sebtlファイルを書き込む
			//throw: 拡張子が異なる場合 
			//引数1: ファイルパス (拡張子含む)
			//引数2: 書き込む内容
			//引数3: 追加書き込みモードならtrue, default = false
			inline void Write(const sgstring& path, const sgstring& write, bool isAdd)
			{
				if (sgstring::functions::IsEndWith(path, cSebtLockedExtension))
					WriteLocked(path, write, isAdd, false);
				else if (sgstring::functions::IsEndWith(path, cSebtNotLockedExtension))
					WriteNotLocked(path, write, isAdd, false);
				else
					throw InvalidArgument(L"Error!! SebtFileRW->Write", L"Extension NotFound: " + path + L" Write extension sebt or sebtl");
			}
			//----------------------------------------------------------------------------------
			//[WriteLocked]
			//sebtlファイルを書き込む
			//throw: 拡張子が異なる場合 
			//引数1: ファイルパス (拡張子含む)
			//引数2: 書き込む内容
			//引数3: 追加書き込みモードならtrue, default = false
			//引数4: 拡張子チェックを行うか否か, default = true
			inline void WriteLocked(const sgstring& path, const sgstring& write, bool isAdd, bool isCheckExtension)
			{
				FileWriteStream fws;

				//拡張子チェック
				if (isCheckExtension && !sgstring::functions::IsEndWith(path, cSebtLockedExtension))
					throw InvalidArgument(L"Error!! SecFileRW->WriteLocked",
						L"Extension NotFound: " + path + L" Read extension secl");

				//ファイルを開けなかった場合はthrow返す
				try { fws.Open(path, true, isAdd); }
				catch (...) { throw; }
				
				fws.WriteSplit(EncryptionKey::Lock(write));
				fws.Close();
			}
			//----------------------------------------------------------------------------------
			//[WriteNotLocked]
			//sebtファイルを書き込む
			//throw: 拡張子が異なる場合 
			//引数1: ファイルパス (拡張子含む)
			//引数2: 書き込む内容
			//引数3: 追加書き込みモードならtrue, default = false
			//引数4: 拡張子チェックを行うか否か, default = true
			inline void WriteNotLocked(const sgstring& path, const sgstring& write, bool isAdd, bool isCheckExtension)
			{
				WFileWriteStream fws;

				//拡張子チェック
				if (isCheckExtension && !sgstring::functions::IsEndWith(path, cSebtNotLockedExtension))
					throw InvalidArgument(L"Error!! SecFileRW->WriteNotLocked",
						L"Extension NotFound: " + path + L" Read extension sec");

				//ファイルを開けなかった場合はthrow返す
				try { fws.Open(path, false, isAdd); }
				catch (...) { throw; }

				fws.Write(write);
				fws.Close();			//ファイルを閉じる
			}

			//----------------------------------------------------------------------------------
			//[WriteBinary]
			//sebt or sebtlファイルを書き込む
			//throw: 拡張子が異なる場合 
			//引数1: ファイルパス (拡張子含む)
			//引数2: 書き込む内容
			//引数3: 追加書き込みモードならtrue, default = false
			void WriteBinary(const sgstring & path, const std::vector<byte> & write, bool isAdd)
			{
				if (sgstring::functions::IsEndWith(path, cSebtLockedExtension))
					WriteBinaryLocked(path, write, isAdd, false);
				else if (sgstring::functions::IsEndWith(path, cSebtNotLockedExtension))
					WriteBinaryNotLocked(path, write, isAdd, false);
				else
					throw InvalidArgument(L"Error!! SebtFileRW->WriteBinary", L"Extension NotFound: " + path + L" Write extension sebt or sebtl");
			}
			//----------------------------------------------------------------------------------
			//[WriteBinaryLocked]
			//sebtlファイルを書き込む
			//throw: 拡張子が異なる場合 
			//引数1: ファイルパス (拡張子含む)
			//引数2: 書き込む内容
			//引数3: 追加書き込みモードならtrue, default = false
			//引数4: 拡張子チェックを行うか否か, default = true
			void WriteBinaryLocked(const sgstring & path, const std::vector<byte> & write, bool isAdd, bool isCheckExtension)
			{
				FileWriteStream fws;

				//拡張子チェック
				if (isCheckExtension && !sgstring::functions::IsEndWith(path, cSebtLockedExtension))
					throw InvalidArgument(L"Error!! SecFileRW->WriteBinaryLocked",
						L"Extension NotFound: " + path + L" Read extension secl");

				//ファイルを開けなかった場合はthrow返す
				try { fws.Open(path, true, isAdd); }
				catch (...) { throw; }

				fws.Write(EncryptionKey::LockBinary(write));
				fws.Close();
			}
			//----------------------------------------------------------------------------------
			//[WriteBinaryNotLocked]
			//sebtファイルを書き込む
			//throw: 拡張子が異なる場合 
			//引数1: ファイルパス (拡張子含む)
			//引数2: 書き込む内容
			//引数3: 追加書き込みモードならtrue, default = false
			//引数4: 拡張子チェックを行うか否か, default = true
			void WriteBinaryNotLocked(const sgstring & path, const std::vector<byte> & write, bool isAdd, bool isCheckExtension)
			{
				FileWriteStream fws;

				//拡張子チェック
				if (isCheckExtension && !sgstring::functions::IsEndWith(path, cSebtLockedExtension))
					throw InvalidArgument(L"Error!! SecFileRW->WriteBinaryLocked",
						L"Extension NotFound: " + path + L" Read extension secl");

				//ファイルを開けなかった場合はthrow返す
				try { fws.Open(path, true, isAdd); }
				catch (...) { throw; }

				fws.Write(write);
				fws.Close();
			}
		};
	}
}


#endif // !SGFRAMEWORK_HEADER_SEBT_FILE_RW_HPP_