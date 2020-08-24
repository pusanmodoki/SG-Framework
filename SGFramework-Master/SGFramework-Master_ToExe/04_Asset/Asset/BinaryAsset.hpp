/*----------------------------------------------------------------------------------
Binary Text�`���̃t�@�C����������BinaryAsset class
�Ή��g���q : sebt, sebtl, txt, cpp, hpp, html, htm, json, xml, yaml, fnt
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_BINARY_ASSET_HPP_
#define SGFRAMEWORK_HEADER_BINARY_ASSET_HPP_
#include "../../02_CustomVariable/CustomVariable.hpp"
#include "../FileRW/FileRW.hpp"
#include "BaseAsset.hpp"

// Framework namespace
namespace SGFramework
{
	//Asset�֘A���i�[����Asset namespace
	namespace Asset
	{
		//Binary Text�`���̃t�@�C����������BinaryAsset class
		//�Ή��g���q : sebt, sebtl, txt, cpp, hpp, html, htm, json, xml, yaml, fnt
		class BinaryAsset final : public BaseClass::BaseAsset
		{
		public:
			friend class AssetManager;
			template <typename TAsset>
			friend SharedPointer<TAsset> Asset::InstantiateAsset(const sgstring& filePath, bool isRegisterSharedAsset);

			inline ~BinaryAsset() { CloseBaseAsset(getType(), true); }

			//----------------------------------------------------------------------------------
			//[Open]
			//�t�@�C�����J��
			//throw: �Ή����Ă��Ȃ��g���q�������ꍇ
			inline void Open();

			//----------------------------------------------------------------------------------
			//[Write]
			//�t�@�C���ɏ�������, Open���s��ɉ\
			//����1: �������ޓ��e
			//����2: �ǉ��������݃��[�h�ŊJ����, default = false
			inline void Write(const std::vector<byte>& writeBytes, bool isAppend = false);
			//----------------------------------------------------------------------------------
			//[Reload]
			//�A�Z�b�g�̃����[�h���s��, Open���s��ɉ\
			inline virtual void Reload();
			//----------------------------------------------------------------------------------
			//[Close]
			//�t�@�C�������, Open���s��ɉ\
			inline virtual void Close();

			//----------------------------------------------------------------------------------
			//[Read]
			//return: �A�Z�b�g�̒��g
			inline const std::vector<byte>& Read();

			//get asset type (read function property)
			SGF_FUNCTION_PROPERTY Asset::AssetType getType() override { return Asset::AssetType::Binary; }

			GetOnlyProperty<std::vector<byte>> bytes;			//�A�Z�b�g�̒��g

		private:
			//----------------------------------------------------------------------------------
			//[�R���X�g���N�^]
			//0���������s��
			inline BinaryAsset() : bytes(m_bytes), m_fileType(0) {}

			static constexpr int m_cFlagSebt = 0x1;		//���ʃt���O sebtl
			static constexpr int m_cFlagSebtl = 0x2;		//���ʃt���O sebt
			static constexpr int m_cFlagOthers = 0x4;	//���ʃt���O ���̑�

			//�g���q������ sebt, sebtl
			static constexpr wchar m_cIdentificationSebtOrSebtl[2][7] = { L".sebt", L".sebtl" };
			//�g���q������ ���̑�
			static constexpr wchar m_cIdentificationOthers[9][6] = { L".txt", L".cpp", L".hpp", L".html", L".htm", L".json", L".xml", L".yaml", L".fnt" };

			std::vector<byte> m_bytes;		//�e�L�X�g���e
			std::mutex m_mutex;				//mutex
			byte m_fileType;							//�t�@�C�����ʃt���O
		};


		//----------------------------------------------------------------------------------
		//[Open]
		//�t�@�C�����J��
		//throw: �Ή����Ă��Ȃ��g���q�������ꍇ
		//����1: �t�@�C���p�X (�g���q�܂�)
		inline void BinaryAsset::Open()
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
			//�g���q�G���[
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
		//�t�@�C���ɏ�������, Open���s��ɉ\
		//����1: �������ޓ��e
		//����2: �ǉ��������݃��[�h�ŊJ����, default = false
		inline void BinaryAsset::Write(const std::vector<byte>& writeBytes, bool isAppend)
		{
			//�J���ĂȂ�
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
			//�g���q�G���[
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
		//�A�Z�b�g�̃����[�h���s��, Open���s��ɉ\
		inline void BinaryAsset::Reload()
		{
			//�J���ĂȂ�
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
		//�t�@�C�������, Open���s��ɉ\
		inline void BinaryAsset::Close()
		{
			if (IS_FALSE(getIsFileOpened())) return;
		
			//BaseAsset
			CloseBaseAsset(getType(), false);
		}

		//----------------------------------------------------------------------------------
		//[ReadNotArray]
		//return: �A�Z�b�g�̒��g
		inline const std::vector<byte>& BinaryAsset::Read() { return m_bytes; }
	}
}
#endif // !SGFRAMEWORK_HEADER_BINARY_ASSET_HPP_