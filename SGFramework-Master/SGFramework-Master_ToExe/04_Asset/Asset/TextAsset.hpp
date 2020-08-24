/*----------------------------------------------------------------------------------
Text�`���̃t�@�C����������TextAsset class
�Ή��g���q (Array���[�h): sec, secl, csv 
�Ή��g���q (NotArray���[�h): sebt, sebtl, txt, cpp, hpp, html, htm, json, xml, yaml, fnt
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
	//Asset�֘A���i�[����Asset namespace
	namespace Asset
	{
		//Text�`���̃t�@�C����������TextAsset class
		//�Ή��g���q (Array���[�h): sec, secl, csv 
		//�Ή��g���q (NotArray���[�h): sebt, sebtl, txt, cpp, hpp, html, htm, json, xml, yaml, fnt
		class TextAsset final : public BaseClass::BaseAsset
		{
		public:
			friend class AssetManager;
			template <typename TAsset>
			friend SharedPointer<TAsset> Asset::InstantiateAsset(const sgstring& filePath, bool isRegisterSharedAsset);

			inline ~TextAsset() { CloseBaseAsset(getType(), true); }

			//----------------------------------------------------------------------------------
			//[Open]
			//�t�@�C�����J��
			//throw: �Ή����Ă��Ȃ��g���q�������ꍇ
			inline void Open();

			//----------------------------------------------------------------------------------
			//[Write]
			//�t�@�C���ɏ�������, Open���s��ɉ\
			//���̊֐���NotArray���[�h�ŊJ�����ꍇ�̂ݗL�� (�Ή��g���q: sebt, sebtl, txt, cpp, hpp, html, htm, json, xml, yaml, fnt)
			//throw: �J�����t�@�C����Array���[�h�������ꍇ(sec, secl, csv)
			//����1: �������ޓ��e
			//����2: �ǉ��������݃��[�h�ŊJ����, default = false
			inline void Write(const sgstring& typeTextNotArray, bool isAppend = false);
			//----------------------------------------------------------------------------------
			//[Write]
			//�t�@�C���ɏ�������, Open���s��ɉ\
			//���̊֐���Array���[�h�ŊJ�����ꍇ�̂ݗL�� (�Ή��g���q: sec, secl, csv)
			//throw: �J�����t�@�C����NotArray���[�h�������ꍇ(sebt, sebtl, txt, cpp, hpp, html, htm, json, xml, yaml, fnt)
			//����1: �������ޓ��e
			//����2: �ǉ��������݃��[�h�ŊJ����, default = false
			inline void Write(const File::FileText& typeTextArray, bool isAppend = false);
			//----------------------------------------------------------------------------------
			//[Reload]
			//�A�Z�b�g�̃����[�h���s��, Open���s��ɉ\
			inline void Reload() override;
			//----------------------------------------------------------------------------------
			//[Close]
			//�t�@�C�������, Open���s��ɉ\
			inline void Close() override;

			//----------------------------------------------------------------------------------
			//[ReadArray]
			//return: �A�Z�b�g�̒��g, NotArray���[�h�ŊJ�����ꍇ�Ԃ�l�͋�ƂȂ�
			inline const File::FileText& ReadArray();
			//----------------------------------------------------------------------------------
			//[ReadNotArray]
			//return: �A�Z�b�g�̒��g, Array���[�h�ŊJ�����ꍇ�Ԃ�l�͋�ƂȂ�
			inline const sgstring& ReadNotArray();

			//get asset type (read function property)
			SGF_FUNCTION_PROPERTY Asset::AssetType getType() override { return Asset::AssetType::Text; }

			GetOnlyProperty<File::FileText> textArray;	//�A�Z�b�g�̒��g (Array���[�h)
			GetOnlyProperty<sgstring> textNotArray;	//�A�Z�b�g�̒��g (NotArray���[�h)
		private:
			//----------------------------------------------------------------------------------
			//[�R���X�g���N�^]
			//0���������s��
			inline TextAsset() : textArray(m_textArray), textNotArray(m_textNotArray), m_fileType(0) {}

			static constexpr int m_cFlagSec = 0x1;		//���ʃt���O sec
			static constexpr int m_cFlagSecl = 0x2;		//���ʃt���O secl
			static constexpr int m_cFlagSebt = 0x4;		//���ʃt���O sebtl
			static constexpr int m_cFlagSebtl = 0x8;		//���ʃt���O sebt
			static constexpr int m_cFlagCsv = 0x10;		//���ʃt���O csv
			static constexpr int m_cFlagOthers = 0x20;	//���ʃt���O ���̑�

			//�g���q������ sec,secl
			static constexpr wchar m_cIdentificationSecOrSecl[2][6] = { L".sec", L".secl" };
			//�g���q������ sebt, sebtl
			static constexpr wchar m_cIdentificationSebtOrSebtl[2][7] = { L".sebt", L".sebtl" };
			//�g���q������ csv
			static constexpr wchar m_cIdentificationCsv[5] = { L".csv" };
			//�g���q������ ���̑�
			static constexpr wchar m_cIdentificationOthers[9][6] = { L".txt", L".cpp", L".hpp", L".html", L".htm", L".json", L".xml", L".yaml", L".fnt" };

			std::mutex  m_mutex;		//mutex
			File::FileText m_textArray;	//�e�L�X�g���e (Array���[�h)
			sgstring m_textNotArray;	//�e�L�X�g���e (NotArray���[�h)
			byte m_fileType;					//�t�@�C�����ʃt���O
		};


		//----------------------------------------------------------------------------------
		//[Open]
		//�t�@�C�����J��
		//throw: �Ή����Ă��Ȃ��g���q�������ꍇ
		inline void TextAsset::Open()
		{
			//open?
			if (IS_TRUE(getIsFileOpened())) return;
			
			setIsOpenFailed(false);

			//mutex guard
			std::lock_guard<std::mutex> guard(m_mutex);

			//�Ȃ���������񂩂�Ƃ肠�����J���Ƃ�
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
			//�g���q�G���[
			else
			{
				setIsOpenFailed(true);

				throw InvalidArgument(L"Error!! TextAsset->Open",
					L"Extension NotFound: " + name());
			}
		}

		//----------------------------------------------------------------------------------
		//[Write]
		//�t�@�C���ɏ�������, Open���s��ɉ\
		//���̊֐���NotArray���[�h�ŊJ�����ꍇ�̂ݗL�� (�Ή��g���q: sebt, sebtl, txt, cpp, hpp, html, htm, json, xml, yaml, fnt)
		//throw: �J�����t�@�C����Array���[�h�������ꍇ(sec, secl, csv)
		//����1: �������ޓ��e
		//����2: �ǉ��������݃��[�h�ŊJ����, default = false
		inline void TextAsset::Write(const sgstring & typeTextNotArray, bool isAppend)
		{
			//�J���ĂȂ�
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
			//�g���q�G���[
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
		//�t�@�C���ɏ�������, Open���s��ɉ\
		//���̊֐���Array���[�h�ŊJ�����ꍇ�̂ݗL�� (�Ή��g���q: sec, secl, csv)
		//throw: �J�����t�@�C����NotArray���[�h�������ꍇ(sebt, sebtl, txt, cpp, hpp, html, htm, json, xml, yaml, fnt)
		//����1: �������ޓ��e
		//����2: �ǉ��������݃��[�h�ŊJ����, default = false
		inline void TextAsset::Write(const File::FileText & typeTextArray, bool isAppend)
		{
			//�J���ĂȂ�
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
			//�g���q�G���[
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
		//�A�Z�b�g�̃����[�h���s��, Open���s��ɉ\
		inline void TextAsset::Reload()
		{
			//�J���ĂȂ�
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
		//�t�@�C�������, Open���s��ɉ\
		inline void TextAsset::Close()
		{
			if (IS_FALSE(getIsFileOpened())) return;
		
			//BaseAsset
			CloseBaseAsset(getType(), false);
		}

		//----------------------------------------------------------------------------------
		//[ReadArray]
		//return: �A�Z�b�g�̒��g, NotArray���[�h�ŊJ�����ꍇ�Ԃ�l�͋�ƂȂ�
		inline const File::FileText & TextAsset::ReadArray() { return m_textArray; }
		//----------------------------------------------------------------------------------
		//[ReadNotArray]
		//return: �A�Z�b�g�̒��g, Array���[�h�ŊJ�����ꍇ�Ԃ�l�͋�ƂȂ�
		inline const sgstring & TextAsset::ReadNotArray() { return m_textNotArray; }
	}
}
#endif // !SGFRAMEWORK_HEADER_TEXT_ASSET_HPP_