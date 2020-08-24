/*----------------------------------------------------------------------------------
Binary Text形式のファイルを扱えるBinaryAsset class
対応拡張子 : sebt, sebtl, txt, cpp, hpp, html, htm, json, xml, yaml, fnt
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_BINARY_ASSET_HPP_
#define SGFRAMEWORK_HEADER_BINARY_ASSET_HPP_
#include "../../02_CustomVariable/CustomVariable.hpp"
#include "../FileRW/FileRW.hpp"
#include "BaseAsset.hpp"

// Framework namespace
namespace SGFramework
{
	//Asset関連を格納したAsset namespace
	namespace Asset
	{
		//Binary Text形式のファイルを扱えるBinaryAsset class
		//対応拡張子 : sebt, sebtl, txt, cpp, hpp, html, htm, json, xml, yaml, fnt
		class BinaryAsset final : public BaseClass::BaseAsset
		{
		public:
			friend class AssetManager;
			template <typename TAsset>
			friend SharedPointer<TAsset> Asset::InstantiateAsset(const sgstring& filePath, bool isRegisterSharedAsset);

			inline ~BinaryAsset() { CloseBaseAsset(getType(), true); }

			//----------------------------------------------------------------------------------
			//[Open]
			//ファイルを開く
			//throw: 対応していない拡張子だった場合
			inline void Open();

			//----------------------------------------------------------------------------------
			//[Write]
			//ファイルに書き込む, Open実行後に可能
			//引数1: 書き込む内容
			//引数2: 追加書き込みモードで開くか, default = false
			inline void Write(const std::vector<byte>& writeBytes, bool isAppend = false);
			//----------------------------------------------------------------------------------
			//[Reload]
			//アセットのリロードを行う, Open実行後に可能
			inline virtual void Reload();
			//----------------------------------------------------------------------------------
			//[Close]
			//ファイルを閉じる, Open実行後に可能
			inline virtual void Close();

			//----------------------------------------------------------------------------------
			//[Read]
			//return: アセットの中身
			inline const std::vector<byte>& Read();

			//get asset type (read function property)
			SGF_FUNCTION_PROPERTY Asset::AssetType getType() override { return Asset::AssetType::Binary; }

			GetOnlyProperty<std::vector<byte>> bytes;			//アセットの中身

		private:
			//----------------------------------------------------------------------------------
			//[コンストラクタ]
			//0初期化を行う
			inline BinaryAsset() : bytes(m_bytes), m_fileType(0) {}

			static constexpr int m_cFlagSebt = 0x1;		//識別フラグ sebtl
			static constexpr int m_cFlagSebtl = 0x2;		//識別フラグ sebt
			static constexpr int m_cFlagOthers = 0x4;	//識別フラグ その他

			//拡張子文字列 sebt, sebtl
			static constexpr wchar m_cIdentificationSebtOrSebtl[2][7] = { L".sebt", L".sebtl" };
			//拡張子文字列 その他
			static constexpr wchar m_cIdentificationOthers[9][6] = { L".txt", L".cpp", L".hpp", L".html", L".htm", L".json", L".xml", L".yaml", L".fnt" };

			std::vector<byte> m_bytes;		//テキスト内容
			std::mutex m_mutex;				//mutex
			byte m_fileType;							//ファイル識別フラグ
		};


		//----------------------------------------------------------------------------------
		//[Open]
		//ファイルを開く
		//throw: 対応していない拡張子だった場合
		//引数1: ファイルパス (拡張子含む)
		inline void BinaryAsset::Open()
		{
			//open?
			if (IS_TRUE(getIsFileOpened())) return;

			setIsOpenFailed(false);

			//mutex guard
			std::lock_guard<std::mutex> guard(m_mutex);

			//ないかもしらんからとりあえず開いとく
			File::FileWriteStream fws;
			fws.Open(name(), false, true);
			fws.Close();

			bool isOpened = false;
			//sebt
			if (name().my_functions.IsEndWith(m_cIdentificationSebtOrSebtl[0]))
			{
				isOpened = true;
				File::SebtFileRW::ReadBinaryNotLocked(name(), m_bytes, false);
				m_fileType = m_cFlagSebt;
			}
			//sebtl
			else if (name().my_functions.IsEndWith(m_cIdentificationSebtOrSebtl[1]))
			{
				isOpened = true;
				File::SebtFileRW::ReadBinaryLocked(name(), m_bytes, false);
				m_fileType = m_cFlagSebtl;
			}
			//others
			else
			{
				for (int i = 0; i < 9; i++)
					if (name().my_functions.IsEndWith(m_cIdentificationOthers[i]))
					{
						isOpened = true;
						File::TextFileRW::ReadBinary(name(), m_bytes);
						m_fileType = m_cFlagOthers;
					}
			}

			//BaseAsset
			if (isOpened) OpenBaseAsset();
			//拡張子エラー
			else
			{
				//filed
				setIsOpenFailed(true);

				throw InvalidArgument(L"Error!! TextAsset->Open",
					L"Extension NotFound: " + name());
			}
		}

		//----------------------------------------------------------------------------------
		//[Write]
		//ファイルに書き込む, Open実行後に可能
		//引数1: 書き込む内容
		//引数2: 追加書き込みモードで開くか, default = false
		inline void BinaryAsset::Write(const std::vector<byte>& writeBytes, bool isAppend)
		{
			//開いてない
			if (IS_FALSE(getIsFileOpened())) return;

			//mutex guard
			std::lock_guard<std::mutex> guard(m_mutex);

			//sebt
			if ((m_fileType & m_cFlagSebt) != 0)
				File::SebtFileRW::WriteBinaryNotLocked(name, writeBytes, isAppend, false);
			//sebtl
			else if ((m_fileType & m_cFlagSebtl) != 0)
				File::SebtFileRW::WriteBinaryLocked(name, writeBytes, isAppend, false);
			//others
			else if ((m_fileType & m_cFlagOthers) != 0)
				File::TextFileRW::WriteBinary(name, writeBytes);
			//拡張子エラー
			else
				throw InvalidArgument(L"Error!! BinaryAsset->Write",
					L"Extension NotSupported: " + name.getValue());

			//text
			m_bytes.assign(writeBytes.cbegin(), writeBytes.cend());
			//BaseAsset
			ReloadBaseAsset();
		}

		//----------------------------------------------------------------------------------
		//[Reload]
		//アセットのリロードを行う, Open実行後に可能
		inline void BinaryAsset::Reload()
		{
			//開いてない
			if (IS_FALSE(getIsFileOpened())) return;

			//mutex guard
			std::lock_guard<std::mutex> guard(m_mutex);

			try
			{
				//sebt
				if ((m_fileType & m_cFlagSebt) != 0)
					File::SebtFileRW::ReadBinaryNotLocked(name, m_bytes, false);
				//sebtl
				else if ((m_fileType & m_cFlagSebtl) != 0)
					File::SebtFileRW::ReadBinaryLocked(name, m_bytes, false);
				//others
				else
					File::TextFileRW::ReadBinary(name, m_bytes);
			}
			catch (...)
			{
				setIsOpenFailed(true);
				throw;
			}

			//BaseAsset
			ReloadBaseAsset();
		}
		//----------------------------------------------------------------------------------
		//[Close]
		//ファイルを閉じる, Open実行後に可能
		inline void BinaryAsset::Close()
		{
			if (IS_FALSE(getIsFileOpened())) return;
		
			//BaseAsset
			CloseBaseAsset(getType(), false);
		}

		//----------------------------------------------------------------------------------
		//[ReadNotArray]
		//return: アセットの中身
		inline const std::vector<byte>& BinaryAsset::Read() { return m_bytes; }
	}
}
#endif // !SGFRAMEWORK_HEADER_BINARY_ASSET_HPP_