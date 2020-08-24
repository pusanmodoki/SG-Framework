/*----------------------------------------------------------------------------------
テキスト形式で読み書きするTextFileRW namespace
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_TEXT_FILE_RW_HPP_
#define SGFRAMEWORK_HEADER_TEXT_FILE_RW_HPP_
#include "../../02_CustomVariable/CustomVariable.hpp"
#include "../FileStream/FileStream.hpp"

// Framework namespace
namespace SGFramework
{
	//File Edit
	namespace File
	{
		//Text File
		namespace TextFileRW
		{
			//----------------------------------------------------------------------------------
			//[Read]
			//テキスト形式でファイルを読み込む
			//return: 読み込んだ内容
			//引数1: ファイルパス (拡張子含む)
			//引数2: バイナリ読み込み, default = false
			inline sgstring Read(const sgstring& path, const bool isBinary = false);
			//----------------------------------------------------------------------------------
			//[Read]
			//テキスト形式でファイルを読み込む
			//引数1: ファイルパス (拡張子含む)
			//引数2: 読み込んだ内容の代入先
			//引数3: バイナリ読み込み, default = false
			inline void Read(const sgstring& path, sgstring& write, const bool isBinary = false);
			//----------------------------------------------------------------------------------
			//[Write]
			//テキスト形式でファイルを書き込む
			//引数1: ファイルパス (拡張子含む)
			//引数2: 書き込む内容 
			//引数3: バイナリ書き込み, default = false
			//引数4: 追加書き込み, default = false
			inline void Write(const sgstring& path, const sgstring& write, bool isBinary = false, bool isAdd = false);
			//----------------------------------------------------------------------------------
			//[ReadBinaryRead]
			//バイナリ形式でファイルを読み込む
			//return: 読み込んだ内容 (byte)
			//引数1: ファイルパス (拡張子含む)
			inline std::vector<byte> ReadBinary(const sgstring& path);
			//----------------------------------------------------------------------------------
			//[ReadBinaryRead]
			//バイナリ形式でファイルを読み込む
			//引数1: ファイルパス (拡張子含む)
			//引数2: 読み込んだ内容を代入するvector (byte)
			inline void ReadBinary(const sgstring& path, std::vector<byte>& write);
			//----------------------------------------------------------------------------------
			//[WriteBinary]
			//バイナリ形式でファイルを書き込む
			//引数1: ファイルパス (拡張子含む)
			//引数2: 書き込む内容 (byte)
			inline void WriteBinary(const sgstring& path, const std::vector<byte>& write, bool isAdd = false);





			//----------------------------------------------------------------------------------
			//[Read]
			//テキスト形式でファイルを読み込む
			//return: 読み込んだ内容
			//引数1: ファイルパス (拡張子含む)
			//引数2: バイナリ読み込み, default = false	
			inline sgstring Read(const sgstring& path, const bool isBinary)
			{
				WFileReadStream frs;
				
				try { frs.Open(path, isBinary); }
				catch (...) { throw; }
				
				return frs.Read();
			}
			//----------------------------------------------------------------------------------
			//[Read]
			//テキスト形式でファイルを読み込む
			//引数1: ファイルパス (拡張子含む)
			//引数2: 読み込んだ内容の代入先
			//引数3: バイナリ読み込み, default = false
			inline void Read(const sgstring& path, sgstring& write, const bool isBinary)
			{
				WFileReadStream frs;

				try { frs.Open(path, isBinary); }
				catch (...) { throw; }

				frs.Read(write);
			}
			//----------------------------------------------------------------------------------
			//[Write]
			//テキスト形式でファイルを書き込む
			//引数1: ファイルパス (拡張子含む)
			//引数2: 書き込む内容 
			//引数3: バイナリ書き込み, default = false
			//引数4: 追加書き込み, default = false
			inline void Write(const sgstring& path, const sgstring& write, bool isBinary, bool isAdd)
			{
				WFileWriteStream fws;
				try { fws.Open(path, isBinary, isAdd); }
				catch (...) { throw; }

				fws << write;

				fws.Close();
			}


			//----------------------------------------------------------------------------------
			//[ReadBinaryRead]
			//バイナリ形式でファイルを読み込む
			//return: 読み込んだ内容 (byte)
			//引数1: ファイルパス (拡張子含む)
			std::vector<byte> ReadBinary(const sgstring & path)
			{
				FileReadStream frs;

				try { frs.Open(path, true); }
				catch (...) { throw; }

				return frs.ReadVector();
			}
			//----------------------------------------------------------------------------------
			//[ReadBinaryRead]
			//バイナリ形式でファイルを読み込む
			//引数1: ファイルパス (拡張子含む)
			//引数2: 読み込んだ内容を代入するvector (byte)
			void ReadBinary(const sgstring & path, std::vector<byte>& write)
			{
				FileReadStream frs;

				try { frs.Open(path, true); }
				catch (...) { throw; }

				frs.ReadVector(write);
			}
			//----------------------------------------------------------------------------------
			//[WriteBinary]
			//バイナリ形式でファイルを書き込む
			//引数1: ファイルパス (拡張子含む)
			//引数2: 書き込む内容 (byte)
			void WriteBinary(const sgstring & path, const std::vector<byte>& write, bool isAdd)
			{
				FileWriteStream fws;
				try { fws.Open(path, true, isAdd); }
				catch (...) { throw; }

				fws.Write(write);

				fws.Close();
			}
		}
	}
}

#endif // !SGFRAMEWORK_HEADER_TEXT_FILE_RW_HPP_
