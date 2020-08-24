/*----------------------------------------------------------------------------------
�Ǝ���Sgframework  Encrypted Binary Text Locked file ������SebtFileRW namespace
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
			//secl�g���q
			constexpr wchar cSebtLockedExtension[] = L".sebtl";
			//sec�g���q
			constexpr wchar cSebtNotLockedExtension[] = L".sebt";

			//----------------------------------------------------------------------------------
			//[Read]
			//sec or secl�t�@�C����ǂݍ���
			//throw: �g���q���قȂ�ꍇ 
			//return: �ǂݍ��񂾓��e
			//����1: �t�@�C���p�X (�g���q�܂�)
			inline sgstring Read(const sgstring& path);
			//----------------------------------------------------------------------------------
			//[ReadLocked]
			//sebtl�t�@�C����ǂݍ���
			//throw: �g���q���قȂ�ꍇ 
			//return: �ǂݍ��񂾓��e
			//����1: �t�@�C���p�X (�g���q�܂�)
			//����2: �g���q�`�F�b�N���s�����ۂ�, default = true
			inline sgstring ReadLocked(const sgstring& path, bool isCheckExtension = true);
			//----------------------------------------------------------------------------------
			//[ReadNotLocked]
			//sebt�t�@�C����ǂݍ���
			//throw: �g���q���قȂ�ꍇ 
			//return: �ǂݍ��񂾓��e
			//����1: �t�@�C���p�X (�g���q�܂�)
			//����2: �g���q�`�F�b�N���s�����ۂ�, default = true
			inline sgstring ReadNotLocked(const sgstring& path, bool isCheckExtension = true);

			//----------------------------------------------------------------------------------
			//[Read]
			//sebt or sebtl�t�@�C����ǂݍ���
			//throw: �g���q���قȂ�ꍇ 
			//����1: �t�@�C���p�X (�g���q�܂�)
			//����2: �ǂݍ��񂾓��e�̑����
			inline void Read(const sgstring& path, sgstring& write);
			//----------------------------------------------------------------------------------
			//[ReadLocked]
			//sebtl�t�@�C����ǂݍ���
			//throw: �g���q���قȂ�ꍇ 
			//����1: �t�@�C���p�X (�g���q�܂�)
			//����2: �ǂݍ��񂾓��e�̑����
			//����3: �g���q�`�F�b�N���s�����ۂ�, default = true
			inline void ReadLocked(const sgstring& path, sgstring& write, bool isCheckExtension = true);
			//----------------------------------------------------------------------------------
			//[ReadNotLocked]
			//sebt�t�@�C����ǂݍ���
			//throw: �g���q���قȂ�ꍇ 
			//����1: �t�@�C���p�X (�g���q�܂�)
			//����2: �ǂݍ��񂾓��e�̑����
			//����3: �g���q�`�F�b�N���s�����ۂ�, default = true
			inline void ReadNotLocked(const sgstring& path, sgstring& write, bool isCheckExtension = true);


			//----------------------------------------------------------------------------------
			//[Read]
			//sebt or sebtl�t�@�C����ǂݍ���
			//throw: �g���q���قȂ�ꍇ 
			//return: �ǂݍ��񂾓��e
			//����1: �t�@�C���p�X (�g���q�܂�)
			inline std::vector<byte> ReadBinary(const sgstring& path);
			//----------------------------------------------------------------------------------
			//[ReadLocked]
			//sebtl�t�@�C����ǂݍ���
			//throw: �g���q���قȂ�ꍇ 
			//return: �ǂݍ��񂾓��e
			//����1: �t�@�C���p�X (�g���q�܂�)
			//����2: �g���q�`�F�b�N���s�����ۂ�, default = true
			inline std::vector<byte> ReadBinaryLocked(const sgstring& path, bool isCheckExtension = true);
			//----------------------------------------------------------------------------------
			//[ReadNotLocked]
			//sebt�t�@�C����ǂݍ���
			//throw: �g���q���قȂ�ꍇ 
			//return: �ǂݍ��񂾓��e
			//����1: �t�@�C���p�X (�g���q�܂�)
			//����2: �g���q�`�F�b�N���s�����ۂ�, default = true
			inline std::vector<byte> ReadBinaryNotLocked(const sgstring& path, bool isCheckExtension = true);

			//----------------------------------------------------------------------------------
			//[Read]
			//sebt or sebtl�t�@�C����ǂݍ���
			//throw: �g���q���قȂ�ꍇ 
			//����1: �t�@�C���p�X (�g���q�܂�)
			//����2: �ǂݍ��񂾓��e�̑����
			inline void ReadBinary(const sgstring& path, std::vector<byte>& write);
			//----------------------------------------------------------------------------------
			//[ReadLocked]
			//sebtl�t�@�C����ǂݍ���
			//throw: �g���q���قȂ�ꍇ 
			//����1: �t�@�C���p�X (�g���q�܂�)
			//����2: �ǂݍ��񂾓��e�̑����
			//����3: �g���q�`�F�b�N���s�����ۂ�, default = true
			inline void ReadBinaryLocked(const sgstring& path, std::vector<byte>& write, bool isCheckExtension = true);
			//----------------------------------------------------------------------------------
			//[ReadNotLocked]
			//sebt�t�@�C����ǂݍ���
			//throw: �g���q���قȂ�ꍇ 
			//����1: �t�@�C���p�X (�g���q�܂�)
			//����2: �ǂݍ��񂾓��e�̑����
			//����3: �g���q�`�F�b�N���s�����ۂ�, default = true
			inline void ReadBinaryNotLocked(const sgstring& path, std::vector<byte>& write, bool isCheckExtension = true);

			//----------------------------------------------------------------------------------
			//[Write]
			//sebt or sebtl�t�@�C������������
			//throw: �g���q���قȂ�ꍇ 
			//����1: �t�@�C���p�X (�g���q�܂�)
			//����2: �������ޓ��e
			//����3: �ǉ��������݃��[�h�Ȃ�true, default = false
			inline void Write(const sgstring& path, const sgstring& write, bool isAdd = false);
			//----------------------------------------------------------------------------------
			//[WriteLocked]
			//sebtl�t�@�C������������
			//throw: �g���q���قȂ�ꍇ 
			//����1: �t�@�C���p�X (�g���q�܂�)
			//����2: �������ޓ��e
			//����3: �ǉ��������݃��[�h�Ȃ�true, default = false
			//����4: �g���q�`�F�b�N���s�����ۂ�, default = true
			inline void WriteLocked(const sgstring& path, const sgstring& write, bool isAdd = false, bool isCheckExtension = true);
			//----------------------------------------------------------------------------------
			//[WriteNotLocked]
			//sebt�t�@�C������������
			//throw: �g���q���قȂ�ꍇ 
			//����1: �t�@�C���p�X (�g���q�܂�)
			//����2: �������ޓ��e
			//����3: �ǉ��������݃��[�h�Ȃ�true, default = false
			//����4: �g���q�`�F�b�N���s�����ۂ�, default = true
			inline void WriteNotLocked(const sgstring& path, const sgstring& write, bool isAdd = false, bool isCheckExtension = true);

			//----------------------------------------------------------------------------------
			//[WriteBinary]
			//sebt or sebtl�t�@�C������������
			//throw: �g���q���قȂ�ꍇ 
			//����1: �t�@�C���p�X (�g���q�܂�)
			//����2: �������ޓ��e
			//����3: �ǉ��������݃��[�h�Ȃ�true, default = false
			inline void WriteBinary(const sgstring& path, const std::vector<byte>& write, bool isAdd = false);
			//----------------------------------------------------------------------------------
			//[WriteBinaryLocked]
			//sebtl�t�@�C������������
			//throw: �g���q���قȂ�ꍇ 
			//����1: �t�@�C���p�X (�g���q�܂�)
			//����2: �������ޓ��e
			//����3: �ǉ��������݃��[�h�Ȃ�true, default = false
			//����4: �g���q�`�F�b�N���s�����ۂ�, default = true
			inline void WriteBinaryLocked(const sgstring& path, const std::vector<byte>& write, bool isAdd = false, bool isCheckExtension = true);
			//----------------------------------------------------------------------------------
			//[WriteBinaryNotLocked]
			//sebt�t�@�C������������
			//throw: �g���q���قȂ�ꍇ 
			//����1: �t�@�C���p�X (�g���q�܂�)
			//����2: �������ޓ��e
			//����3: �ǉ��������݃��[�h�Ȃ�true, default = false
			//����4: �g���q�`�F�b�N���s�����ۂ�, default = true
			inline void WriteBinaryNotLocked(const sgstring& path, const std::vector<byte>& write, bool isAdd = false, bool isCheckExtension = true);
		

			//----------------------------------------------------------------------------------
			//[Read]
			//sebt or sebtl�t�@�C����ǂݍ���
			//throw: �g���q���قȂ�ꍇ 
			//return: �ǂݍ��񂾓��e
			//����1: �t�@�C���p�X (�g���q�܂�)
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
			//sebtl�t�@�C����ǂݍ���
			//throw: �g���q���قȂ�ꍇ 
			//return: �ǂݍ��񂾓��e
			//����1: �t�@�C���p�X (�g���q�܂�)
			//����2: �g���q�`�F�b�N���s�����ۂ�, default = true
			inline sgstring ReadLocked(const sgstring& path, bool isCheckExtension)
			{
				FileReadStream frs;//�t�@�C���X�g���[��

				//�g���q�`�F�b�N
				if (isCheckExtension && !sgstring::functions::IsEndWith(path, cSebtLockedExtension))
					throw InvalidArgument(L"Error!! SebtFileRW->ReadLocked",
						L"Extension NotFound: " + path + L" Read extension sebtl");

				//�t�@�C�����J���Ȃ������ꍇ��throw�Ԃ�
				try { frs.Open(path, true); }
				catch (...) { throw; }

				//�Í��������㕶����
				sgstring readUnlcok = frs.ReadUnite();
				frs.Close();		//�t�@�C�������
				//����
				return EncryptionKey::Unlock(readUnlcok, EncryptionKey::DefaultKey());
			}
			//----------------------------------------------------------------------------------
			//[ReadNotLocked]
			//sebt�t�@�C����ǂݍ���
			//throw: �g���q���قȂ�ꍇ 
			//return: �ǂݍ��񂾓��e
			//����1: �t�@�C���p�X (�g���q�܂�)
			//����2: �g���q�`�F�b�N���s�����ۂ�, default = true
			inline sgstring ReadNotLocked(const sgstring& path, bool isCheckExtension)
			{
				WFileReadStream frs;//�t�@�C���X�g���[��

				//�g���q�`�F�b�N
				if (isCheckExtension && !sgstring::functions::IsEndWith(path, cSebtNotLockedExtension))
					throw InvalidArgument(L"Error!! SebtFileRW->ReadNotLocked",
						L"Extension NotFound: " + path + L" Read extension sebt");

				//�t�@�C�����J���Ȃ������ꍇ��throw�Ԃ�
				try { frs.Open(path); }
				catch (...) { throw; }

				return frs.Read();
			}

			//----------------------------------------------------------------------------------
			//[Read]
			//sec or secl�t�@�C����ǂݍ���
			//throw: �g���q���قȂ�ꍇ 
			//����1: �t�@�C���p�X (�g���q�܂�)
			//����2: �ǂݍ��񂾓��e�̑����
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
			//sebtl�t�@�C����ǂݍ���
			//throw: �g���q���قȂ�ꍇ 
			//����1: �t�@�C���p�X (�g���q�܂�)
			//����2: �ǂݍ��񂾓��e�̑����
			//����3: �g���q�`�F�b�N���s�����ۂ�, default = true
			inline void ReadLocked(const sgstring& path, sgstring& write, bool isCheckExtension)
			{
				FileReadStream frs;//�t�@�C���X�g���[��

				//�g���q�`�F�b�N
				if (isCheckExtension && !sgstring::functions::IsEndWith(path, cSebtLockedExtension))
					throw InvalidArgument(L"Error!! SebtFileRW->ReadLocked",
						L"Extension NotFound: " + path + L" Read extension sebtl");

				//�t�@�C�����J���Ȃ������ꍇ��throw�Ԃ�
				try { frs.Open(path, true); }
				catch (...) { throw; }

				//�Í��������㕶����
				sgstring readUnlcok;
				frs.ReadUnite(readUnlcok);
				frs.Close();		//�t�@�C�������
				//����
				EncryptionKey::Unlock(write, readUnlcok, EncryptionKey::DefaultKey());
			}
			//----------------------------------------------------------------------------------
			//[ReadNotLocked]
			//sebt�t�@�C����ǂݍ���
			//throw: �g���q���قȂ�ꍇ 
			//����1: �t�@�C���p�X (�g���q�܂�)
			//����2: �ǂݍ��񂾓��e�̑����
			//����3: �g���q�`�F�b�N���s�����ۂ�, default = true
			inline void ReadNotLocked(const sgstring& path, sgstring& write, bool isCheckExtension)
			{
				WFileReadStream frs;//�t�@�C���X�g���[��

				//�g���q�`�F�b�N
				if (isCheckExtension && !sgstring::functions::IsEndWith(path, cSebtNotLockedExtension))
					throw InvalidArgument(L"Error!! SebtFileRW->ReadNotLocked",
						L"Extension NotFound: " + path + L" Read extension sebt");

				//�t�@�C�����J���Ȃ������ꍇ��throw�Ԃ�
				try { frs.Open(path); }
				catch (...) { throw; }

				frs.Read(write);
			}

			//----------------------------------------------------------------------------------
			//[Read]
			//sebt or sebtl�t�@�C����ǂݍ���
			//throw: �g���q���قȂ�ꍇ 
			//return: �ǂݍ��񂾓��e
			//����1: �t�@�C���p�X (�g���q�܂�)
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
			//sebtl�t�@�C����ǂݍ���
			//throw: �g���q���قȂ�ꍇ 
			//return: �ǂݍ��񂾓��e
			//����1: �t�@�C���p�X (�g���q�܂�)
			//����2: �g���q�`�F�b�N���s�����ۂ�, default = true
			std::vector<byte> ReadBinaryLocked(const sgstring & path, bool isCheckExtension)
			{
				FileReadStream frs;//�t�@�C���X�g���[��

				//�g���q�`�F�b�N
				if (isCheckExtension && !sgstring::functions::IsEndWith(path, cSebtLockedExtension))
					throw InvalidArgument(L"Error!! SebtFileRW->ReadBinaryLocked",
						L"Extension NotFound: " + path + L" Read extension sebtl");

				//�t�@�C�����J���Ȃ������ꍇ��throw�Ԃ�
				try { frs.Open(path, true); }
				catch (...) { throw; }

				//�Í��������㕶����
				std::vector<byte> readUnlcok = frs.ReadVector();
				frs.Close();		//�t�@�C�������
				//����
				return EncryptionKey::UnlockBinary(readUnlcok, EncryptionKey::DefaultKey());
			}
			//----------------------------------------------------------------------------------
			//[ReadNotLocked]
			//sebt�t�@�C����ǂݍ���
			//throw: �g���q���قȂ�ꍇ 
			//return: �ǂݍ��񂾓��e
			//����1: �t�@�C���p�X (�g���q�܂�)
			//����2: �g���q�`�F�b�N���s�����ۂ�, default = true
			std::vector<byte> ReadBinaryNotLocked(const sgstring & path, bool isCheckExtension)
			{
				FileReadStream frs;//�t�@�C���X�g���[��

				//�g���q�`�F�b�N
				if (isCheckExtension && !sgstring::functions::IsEndWith(path, cSebtNotLockedExtension))
					throw InvalidArgument(L"Error!! SebtFileRW->ReadBinaryNotLocked",
						L"Extension NotFound: " + path + L" Read extension sebt");

				//�t�@�C�����J���Ȃ������ꍇ��throw�Ԃ�
				try { frs.Open(path, true); }
				catch (...) { throw; }

				return std::move(frs.ReadVector());
			}

			//----------------------------------------------------------------------------------
			//[Read]
			//sebt or sebtl�t�@�C����ǂݍ���
			//throw: �g���q���قȂ�ꍇ 
			//����1: �t�@�C���p�X (�g���q�܂�)
			//����2: �ǂݍ��񂾓��e�̑����
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
			//sebtl�t�@�C����ǂݍ���
			//throw: �g���q���قȂ�ꍇ 
			//����1: �t�@�C���p�X (�g���q�܂�)
			//����2: �ǂݍ��񂾓��e�̑����
			//����3: �g���q�`�F�b�N���s�����ۂ�, default = true
			void ReadBinaryLocked(const sgstring & path, std::vector<byte> & write, bool isCheckExtension)
			{
				FileReadStream frs;//�t�@�C���X�g���[��

				//�g���q�`�F�b�N
				if (isCheckExtension && !sgstring::functions::IsEndWith(path, cSebtLockedExtension))
					throw InvalidArgument(L"Error!! SebtFileRW->ReadBinaryLocked",
						L"Extension NotFound: " + path + L" Read extension sebtl");

				//�t�@�C�����J���Ȃ������ꍇ��throw�Ԃ�
				try { frs.Open(path, true); }
				catch (...) { throw; }

				//�Í��������㕶����
				std::vector<byte> readUnlcok = frs.ReadVector();
				frs.Close();		//�t�@�C�������
				//����
				EncryptionKey::UnlockBinary(write, readUnlcok, EncryptionKey::DefaultKey());
			}
			//----------------------------------------------------------------------------------
			//[ReadNotLocked]
			//sebt�t�@�C����ǂݍ���
			//throw: �g���q���قȂ�ꍇ 
			//����1: �t�@�C���p�X (�g���q�܂�)
			//����2: �ǂݍ��񂾓��e�̑����
			//����3: �g���q�`�F�b�N���s�����ۂ�, default = true
			void ReadBinaryNotLocked(const sgstring & path, std::vector<byte> & write, bool isCheckExtension)
			{
				FileReadStream frs;//�t�@�C���X�g���[��

				//�g���q�`�F�b�N
				if (isCheckExtension && !sgstring::functions::IsEndWith(path, cSebtNotLockedExtension))
					throw InvalidArgument(L"Error!! SebtFileRW->ReadNotLocked",
						L"Extension NotFound: " + path + L" Read extension sebt");

				//�t�@�C�����J���Ȃ������ꍇ��throw�Ԃ�
				try { frs.Open(path, true); }
				catch (...) { throw; }

				frs.ReadVector(write);
			}

			//----------------------------------------------------------------------------------
			//[Write]
			//sebt or sebtl�t�@�C������������
			//throw: �g���q���قȂ�ꍇ 
			//����1: �t�@�C���p�X (�g���q�܂�)
			//����2: �������ޓ��e
			//����3: �ǉ��������݃��[�h�Ȃ�true, default = false
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
			//sebtl�t�@�C������������
			//throw: �g���q���قȂ�ꍇ 
			//����1: �t�@�C���p�X (�g���q�܂�)
			//����2: �������ޓ��e
			//����3: �ǉ��������݃��[�h�Ȃ�true, default = false
			//����4: �g���q�`�F�b�N���s�����ۂ�, default = true
			inline void WriteLocked(const sgstring& path, const sgstring& write, bool isAdd, bool isCheckExtension)
			{
				FileWriteStream fws;

				//�g���q�`�F�b�N
				if (isCheckExtension && !sgstring::functions::IsEndWith(path, cSebtLockedExtension))
					throw InvalidArgument(L"Error!! SecFileRW->WriteLocked",
						L"Extension NotFound: " + path + L" Read extension secl");

				//�t�@�C�����J���Ȃ������ꍇ��throw�Ԃ�
				try { fws.Open(path, true, isAdd); }
				catch (...) { throw; }
				
				fws.WriteSplit(EncryptionKey::Lock(write));
				fws.Close();
			}
			//----------------------------------------------------------------------------------
			//[WriteNotLocked]
			//sebt�t�@�C������������
			//throw: �g���q���قȂ�ꍇ 
			//����1: �t�@�C���p�X (�g���q�܂�)
			//����2: �������ޓ��e
			//����3: �ǉ��������݃��[�h�Ȃ�true, default = false
			//����4: �g���q�`�F�b�N���s�����ۂ�, default = true
			inline void WriteNotLocked(const sgstring& path, const sgstring& write, bool isAdd, bool isCheckExtension)
			{
				WFileWriteStream fws;

				//�g���q�`�F�b�N
				if (isCheckExtension && !sgstring::functions::IsEndWith(path, cSebtNotLockedExtension))
					throw InvalidArgument(L"Error!! SecFileRW->WriteNotLocked",
						L"Extension NotFound: " + path + L" Read extension sec");

				//�t�@�C�����J���Ȃ������ꍇ��throw�Ԃ�
				try { fws.Open(path, false, isAdd); }
				catch (...) { throw; }

				fws.Write(write);
				fws.Close();			//�t�@�C�������
			}

			//----------------------------------------------------------------------------------
			//[WriteBinary]
			//sebt or sebtl�t�@�C������������
			//throw: �g���q���قȂ�ꍇ 
			//����1: �t�@�C���p�X (�g���q�܂�)
			//����2: �������ޓ��e
			//����3: �ǉ��������݃��[�h�Ȃ�true, default = false
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
			//sebtl�t�@�C������������
			//throw: �g���q���قȂ�ꍇ 
			//����1: �t�@�C���p�X (�g���q�܂�)
			//����2: �������ޓ��e
			//����3: �ǉ��������݃��[�h�Ȃ�true, default = false
			//����4: �g���q�`�F�b�N���s�����ۂ�, default = true
			void WriteBinaryLocked(const sgstring & path, const std::vector<byte> & write, bool isAdd, bool isCheckExtension)
			{
				FileWriteStream fws;

				//�g���q�`�F�b�N
				if (isCheckExtension && !sgstring::functions::IsEndWith(path, cSebtLockedExtension))
					throw InvalidArgument(L"Error!! SecFileRW->WriteBinaryLocked",
						L"Extension NotFound: " + path + L" Read extension secl");

				//�t�@�C�����J���Ȃ������ꍇ��throw�Ԃ�
				try { fws.Open(path, true, isAdd); }
				catch (...) { throw; }

				fws.Write(EncryptionKey::LockBinary(write));
				fws.Close();
			}
			//----------------------------------------------------------------------------------
			//[WriteBinaryNotLocked]
			//sebt�t�@�C������������
			//throw: �g���q���قȂ�ꍇ 
			//����1: �t�@�C���p�X (�g���q�܂�)
			//����2: �������ޓ��e
			//����3: �ǉ��������݃��[�h�Ȃ�true, default = false
			//����4: �g���q�`�F�b�N���s�����ۂ�, default = true
			void WriteBinaryNotLocked(const sgstring & path, const std::vector<byte> & write, bool isAdd, bool isCheckExtension)
			{
				FileWriteStream fws;

				//�g���q�`�F�b�N
				if (isCheckExtension && !sgstring::functions::IsEndWith(path, cSebtLockedExtension))
					throw InvalidArgument(L"Error!! SecFileRW->WriteBinaryLocked",
						L"Extension NotFound: " + path + L" Read extension secl");

				//�t�@�C�����J���Ȃ������ꍇ��throw�Ԃ�
				try { fws.Open(path, true, isAdd); }
				catch (...) { throw; }

				fws.Write(write);
				fws.Close();
			}
		};
	}
}


#endif // !SGFRAMEWORK_HEADER_SEBT_FILE_RW_HPP_