/*----------------------------------------------------------------------------------
Text形式のファイルを扱えるTextAsset class
対応拡張子 (Arrayモード): sec, secl, csv 
対応拡張子 (NotArrayモード): sebt, sebtl, txt, cpp, hpp, html, htm, json, xml, yaml, fnt
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_TEXT_ASSET_HPP_
#define SGFRAMEWORK_HEADER_TEXT_ASSET_HPP_
#include "../../02_CustomVariable/CustomVariable.hpp"
#include "../FileRW/FileRW.hpp"
#include "BinaryAsset.hpp"
#include "BaseAsset.hpp"

// Framework namespace
namespace SGFramework
{
	//Asset関連を格納したAsset namespace
	namespace Asset
	{
		//Text形式のファイルを扱えるTextAsset class
		//対応拡張子 (Arrayモード): sec, secl, csv 
		//対応拡張子 (NotArrayモード): sebt, sebtl, txt, cpp, hpp, html, htm, json, xml, yaml, fnt
		class TextAsset final : public BaseClass::BaseAsset
		{
		public:
			friend class AssetManager;
			template <typename TAsset>
			friend SharedPointer<TAsset> Asset::InstantiateAsset(const sgstring& filePath, bool isRegisterSharedAsset);

			inline ~TextAsset() { CloseBaseAsset(getType(), true); }

			//----------------------------------------------------------------------------------
			//[Open]
			//ファイルを開く
			//throw: 対応していない拡張子だった場合
			inline void Open();

			//----------------------------------------------------------------------------------
			//[Write]
			//ファイルに書き込む, Open実行後に可能
			//この関数はNotArrayモードで開いた場合のみ有効 (対応拡張子: sebt, sebtl, txt, cpp, hpp, html, htm, json, xml, yaml, fnt)
			//throw: 開いたファイルがArrayモードだった場合(sec, secl, csv)
			//引数1: 書き込む内容
			//引数2: 追加書き込みモードで開くか, default = false
			inline void Write(const sgstring& typeTextNotArray, bool isAppend = false);
			//----------------------------------------------------------------------------------
			//[Write]
			//ファイルに書き込む, Open実行後に可能
			//この関数はArrayモードで開いた場合のみ有効 (対応拡張子: sec, secl, csv)
			//throw: 開いたファイルがNotArrayモードだった場合(sebt, sebtl, txt, cpp, hpp, html, htm, json, xml, yaml, fnt)
			//引数1: 書き込む内容
			//引数2: 追加書き込みモードで開くか, default = false
			inline void Write(const File::FileText& typeTextArray, bool isAppend = false);
			//----------------------------------------------------------------------------------
			//[Reload]
			//アセットのリロードを行う, Open実行後に可能
			inline void Reload() override;
			//----------------------------------------------------------------------------------
			//[Close]
			//ファイルを閉じる, Open実行後に可能
			inline void Close() override;

			//----------------------------------------------------------------------------------
			//[ReadArray]
			//return: アセットの中身, NotArrayモードで開いた場合返り値は空となる
			inline const File::FileText& ReadArray();
			//----------------------------------------------------------------------------------
			//[ReadNotArray]
			//return: アセットの中身, Arrayモードで開いた場合返り値は空となる
			inline const sgstring& ReadNotArray();

			//get asset type (read function property)
			SGF_FUNCTION_PROPERTY Asset::AssetType getType() override { return Asset::AssetType::Text; }

			GetOnlyProperty<File::FileText> textArray;	//アセットの中身 (Arrayモード)
			GetOnlyProperty<sgstring> textNotArray;	//アセットの中身 (NotArrayモード)
		private:
			//----------------------------------------------------------------------------------
			//[コンストラクタ]
			//0初期化を行う
			inline TextAsset() : textArray(m_textArray), textNotArray(m_textNotArray), m_fileType(0) {}

			static constexpr int m_cFlagSec = 0x1;		//識別フラグ sec
			static constexpr int m_cFlagSecl = 0x2;		//識別フラグ secl
			static constexpr int m_cFlagSebt = 0x4;		//識別フラグ sebtl
			static constexpr int m_cFlagSebtl = 0x8;		//識別フラグ sebt
			static constexpr int m_cFlagCsv = 0x10;		//識別フラグ csv
			static constexpr int m_cFlagOthers = 0x20;	//識別フラグ その他

			//拡張子文字列 sec,secl
			static constexpr wchar m_cIdentificationSecOrSecl[2][6] = { L".sec", L".secl" };
			//拡張子文字列 sebt, sebtl
			static constexpr wchar m_cIdentificationSebtOrSebtl[2][7] = { L".sebt", L".sebtl" };
			//拡張子文字列 csv
			static constexpr wchar m_cIdentificationCsv[5] = { L".csv" };
			//拡張子文字列 その他
			static constexpr wchar m_cIdentificationOthers[9][6] = { L".txt", L".cpp", L".hpp", L".html", L".htm", L".json", L".xml", L".yaml", L".fnt" };

			std::mutex  m_mutex;		//mutex
			File::FileText m_textArray;	//テキスト内容 (Arrayモード)
			sgstring m_textNotArray;	//テキスト内容 (NotArrayモード)
			byte m_fileType;					//ファイル識別フラグ
		};


		//----------------------------------------------------------------------------------
		//[Open]
		//ファイルを開く
		//throw: 対応していない拡張子だった場合
		inline void TextAsset::Open()
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
			//sec
			if (name().my_functions.IsEndWith(m_cIdentificationSecOrSecl[0]))
			{
				isOpened = true;
				File::SecFileRW::ReadNotLocked(name(), m_textArray, false);
				m_fileType = m_cFlagSec;
			}
			//secl
			else if (name().my_functions.IsEndWith(m_cIdentificationSecOrSecl[1]))
			{
				isOpened = true;
				File::SecFileRW::ReadLocked(name(), m_textArray, false);
				m_fileType = m_cFlagSecl;
			}
			//sebt
			else if (name().my_functions.IsEndWith(m_cIdentificationSebtOrSebtl[0]))
			{
				isOpened = true;
				File::SebtFileRW::ReadNotLocked(name(), m_textNotArray, false);
				m_fileType = m_cFlagSebt;
			}
			//sebtl
			else if (name().my_functions.IsEndWith(m_cIdentificationSebtOrSebtl[1]))
			{
				isOpened = true;
				File::SebtFileRW::ReadLocked(name(), m_textNotArray, false);
				m_fileType = m_cFlagSebtl;
			}
			//csv
			else if (name().my_functions.IsEndWith(m_cIdentificationCsv))
			{
				isOpened = true;
				File::CsvFileRW::Read(name(), m_textArray, false);
				m_fileType = m_cFlagCsv;
			}
			//others
			else
			{
				for (int i = 0; i < 9; i++)
					if (name().my_functions.IsEndWith(m_cIdentificationOthers[i]))
					{
						isOpened = true;
						File::TextFileRW::Read(name(), m_textNotArray);
						m_fileType = m_cFlagOthers;
					}
			}

			//BaseAsset
			if (isOpened) OpenBaseAsset();
			//拡張子エラー
			else
			{
				setIsOpenFailed(true);

				throw InvalidArgument(L"Error!! TextAsset->Open",
					L"Extension NotFound: " + name());
			}
		}

		//----------------------------------------------------------------------------------
		//[Write]
		//ファイルに書き込む, Open実行後に可能
		//この関数はNotArrayモードで開いた場合のみ有効 (対応拡張子: sebt, sebtl, txt, cpp, hpp, html, htm, json, xml, yaml, fnt)
		//throw: 開いたファイルがArrayモードだった場合(sec, secl, csv)
		//引数1: 書き込む内容
		//引数2: 追加書き込みモードで開くか, default = false
		inline void TextAsset::Write(const sgstring & typeTextNotArray, bool isAppend)
		{
			//開いてない
			if (IS_FALSE(getIsFileOpened())) return;

			//mutex guard
			std::lock_guard<std::mutex> guard(m_mutex);

			//sebt
			if ((m_fileType & m_cFlagSebt) != 0)
				File::SebtFileRW::WriteNotLocked(name, typeTextNotArray, isAppend, false);
			//sebtl
			else if ((m_fileType & m_cFlagSebtl) != 0)
				File::SebtFileRW::WriteLocked(name, typeTextNotArray, isAppend, false);
			//others
			else if ((m_fileType & m_cFlagOthers) != 0)
				File::TextFileRW::Write(name, typeTextNotArray);
			//拡張子エラー
			else
				throw InvalidArgument(L"Error!! TextAsset->Write",
					L"Extension NotSupported: " + name.getValue());

			//text
			m_textNotArray.assign(typeTextNotArray);
			//BaseAsset
			ReloadBaseAsset();
		}
		//----------------------------------------------------------------------------------
		//[Write]
		//ファイルに書き込む, Open実行後に可能
		//この関数はArrayモードで開いた場合のみ有効 (対応拡張子: sec, secl, csv)
		//throw: 開いたファイルがNotArrayモードだった場合(sebt, sebtl, txt, cpp, hpp, html, htm, json, xml, yaml, fnt)
		//引数1: 書き込む内容
		//引数2: 追加書き込みモードで開くか, default = false
		inline void TextAsset::Write(const File::FileText & typeTextArray, bool isAppend)
		{
			//開いてない
			if (IS_FALSE(getIsFileOpened())) return;

			//mutex guard
			std::lock_guard<std::mutex> guard(m_mutex);

			//sec
			if ((m_fileType & m_cFlagSec) != 0)
				File::SecFileRW::WriteNotLocked(name, typeTextArray, isAppend, false);
			//secl
			else if ((m_fileType & m_cFlagSecl) != 0)
				File::SecFileRW::WriteLocked(name, typeTextArray, isAppend, false);
			//csv
			else if ((m_fileType & m_cFlagCsv) != 0)
				File::CsvFileRW::Write(name, typeTextArray, isAppend, false);
			//拡張子エラー
			else
				throw InvalidArgument(L"Error!! TextAsset->Write",
					L"Extension NotSupported: " + name.getValue());

			//text
			m_textArray = typeTextArray;
			//BaseAsset
			ReloadBaseAsset();
		}

		//----------------------------------------------------------------------------------
		//[Reload]
		//アセットのリロードを行う, Open実行後に可能
		inline void TextAsset::Reload()
		{
			//開いてない
			if (IS_FALSE(getIsFileOpened())) return;

			try
			{
				//sec
				if ((m_fileType & m_cFlagSec) != 0)
					File::SecFileRW::ReadNotLocked(name, m_textArray, false);
				//secl
				else if ((m_fileType & m_cFlagSecl) != 0)
					File::SecFileRW::ReadLocked(name, m_textArray, false);
				//sebt
				else if ((m_fileType & m_cFlagSebt) != 0)
					File::SebtFileRW::ReadNotLocked(name, m_textNotArray, false);
				//sebtl
				else if ((m_fileType & m_cFlagSebtl) != 0)
					File::SebtFileRW::ReadLocked(name, m_textNotArray, false);
				//csv
				else if ((m_fileType & m_cFlagCsv) != 0)
					File::CsvFileRW::Read(name, m_textArray);
				//others
				else
					File::TextFileRW::Read(name, m_textNotArray);
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
		inline void TextAsset::Close()
		{
			if (IS_FALSE(getIsFileOpened())) return;
		
			//BaseAsset
			CloseBaseAsset(getType(), false);
		}

		//----------------------------------------------------------------------------------
		//[ReadArray]
		//return: アセットの中身, NotArrayモードで開いた場合返り値は空となる
		inline const File::FileText & TextAsset::ReadArray() { return m_textArray; }
		//----------------------------------------------------------------------------------
		//[ReadNotArray]
		//return: アセットの中身, Arrayモードで開いた場合返り値は空となる
		inline const sgstring & TextAsset::ReadNotArray() { return m_textNotArray; }
	}
}
#endif // !SGFRAMEWORK_HEADER_TEXT_ASSET_HPP_