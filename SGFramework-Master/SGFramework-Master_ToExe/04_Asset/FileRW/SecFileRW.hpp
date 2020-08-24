/*----------------------------------------------------------------------------------
�Ǝ���Sgframework  Encrypted Csv Locked file ������SecFileRW namespace
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
			//secl�g���q
			constexpr wchar cSecLockedExtension[] = L".secl";
			//sec�g���q
			constexpr wchar cSecNotLockedExtension[] = L".sec";

			//----------------------------------------------------------------------------------
			//[Read]
			//sec or secl�t�@�C����ǂݍ���
			//throw: �g���q���قȂ�ꍇ 
			//return: �ǂݍ��񂾓��e
			//����1: �t�@�C���p�X (�g���q�܂�)
			inline FileText Read(const sgstring& path);
			//----------------------------------------------------------------------------------
			//[ReadLocked]
			//secl�t�@�C����ǂݍ���
			//throw: �g���q���قȂ�ꍇ 
			//return: �ǂݍ��񂾓��e
			//����1: �t�@�C���p�X (�g���q�܂�)
			//����2: �g���q�`�F�b�N���s�����ۂ�, default = true
			inline FileText ReadLocked(const sgstring& path, bool isCheckExtension = true);
			//----------------------------------------------------------------------------------
			//[ReadNotLocked]
			//sec�t�@�C����ǂݍ���
			//throw: �g���q���قȂ�ꍇ 
			//return: �ǂݍ��񂾓��e
			//����1: �t�@�C���p�X (�g���q�܂�)
			//����2: �g���q�`�F�b�N���s�����ۂ�, default = true
			inline FileText ReadNotLocked(const sgstring& path, bool isCheckExtension = true);

			//----------------------------------------------------------------------------------
			//[Read]
			//sec or secl�t�@�C����ǂݍ���
			//throw: �g���q���قȂ�ꍇ 
			//����1: �t�@�C���p�X (�g���q�܂�)
			//����2: �ǂݍ��񂾓��e�̑����
			inline void Read(const sgstring& path, FileText& write);
			//----------------------------------------------------------------------------------
			//[ReadLocked]
			//secl�t�@�C����ǂݍ���
			//throw: �g���q���قȂ�ꍇ 
			//����1: �t�@�C���p�X (�g���q�܂�)
			//����2: �ǂݍ��񂾓��e�̑����
			//����3: �g���q�`�F�b�N���s�����ۂ�, default = true
			inline void ReadLocked(const sgstring& path, FileText& write, bool isCheckExtension = true);
			//----------------------------------------------------------------------------------
			//[ReadNotLocked]
			//sec�t�@�C����ǂݍ���
			//throw: �g���q���قȂ�ꍇ 
			//����1: �t�@�C���p�X (�g���q�܂�)
			//����2: �ǂݍ��񂾓��e�̑����
			//����3: �g���q�`�F�b�N���s�����ۂ�, default = true
			inline void ReadNotLocked(const sgstring& path, FileText& write, bool isCheckExtension = true);

			//----------------------------------------------------------------------------------
			//[Write]
			//sec or secl�t�@�C������������
			//throw: �g���q���قȂ�ꍇ 
			//����1: �t�@�C���p�X (�g���q�܂�)
			//����2: �������ޓ��e
			//����3: �ǉ��������݃��[�h�Ȃ�true, default = false
			inline void Write(const sgstring& path, const FileText& write, bool isAdd = false);
			//----------------------------------------------------------------------------------
			//[WriteLocked]
			//secl�t�@�C������������
			//throw: �g���q���قȂ�ꍇ 
			//����1: �t�@�C���p�X (�g���q�܂�)
			//����2: �������ޓ��e
			//����3: �ǉ��������݃��[�h�Ȃ�true, default = false
			//����4: �g���q�`�F�b�N���s�����ۂ�, default = true
			inline void WriteLocked(const sgstring& path, const FileText& write, bool isAdd = false, bool isCheckExtension = true);
			//----------------------------------------------------------------------------------
			//[WriteNotLocked]
			//sec�t�@�C������������
			//throw: �g���q���قȂ�ꍇ 
			//����1: �t�@�C���p�X (�g���q�܂�)
			//����2: �������ޓ��e
			//����3: �ǉ��������݃��[�h�Ȃ�true, default = false
			//����4: �g���q�`�F�b�N���s�����ۂ�, default = true
			inline void WriteNotLocked(const sgstring& path, const FileText& write, bool isAdd = false, bool isCheckExtension = true);



			//----------------------------------------------------------------------------------
			//[Read]
			//sec or secl�t�@�C����ǂݍ���
			//throw: �g���q���قȂ�ꍇ 
			//return: �ǂݍ��񂾓��e
			//����1: �t�@�C���p�X (�g���q�܂�)
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
			//secl�t�@�C����ǂݍ���
			//throw: �g���q���قȂ�ꍇ 
			//return: �ǂݍ��񂾓��e
			//����1: �t�@�C���p�X (�g���q�܂�)
			//����2: �g���q�`�F�b�N���s�����ۂ�, default = true
			inline FileText ReadLocked(const sgstring& path, bool isCheckExtension)
			{
				FileReadStream frs;//�t�@�C���X�g���[��

				//�g���q�`�F�b�N
				if (isCheckExtension && !sgstring::functions::IsEndWith(path, cSecNotLockedExtension))
					throw InvalidArgument(L"Error!! SecFileRW->ReadNotLocked",
						L"Extension NotFound: " + path + L" Read extension sec");

				//�t�@�C�����J���Ȃ������ꍇ��throw�Ԃ�
				try { frs.Open(path, true); }
				catch (...) { throw; }

				//�Í��������㕶����
				sgstring readUnlcok;
				//����
				EncryptionKey::Unlock(readUnlcok, frs.ReadUnite(), EncryptionKey::DefaultKey());
				frs.Close();		//�t�@�C�������

				sgstring getline;		//getline�p�o�b�t�@
				uint lineCount = 0;	//�s��
				//stringstream
				std::wstringstream read(readUnlcok);

				//�S�sgetline, �s���v�Z
				while (std::getline(read, getline))
					lineCount++;

				read.clear();							//clear
				read.seekg(0, std::ios::beg);	//seek
				FileText result(lineCount);		//return

				//�S�sgetline, vector�o�b�t�@��1�s�̗v�f�𕪊����
				while (std::getline(read, getline))
					result.emplace_back(sgstring::functions::Split(getline, ',', true));
			}
			//----------------------------------------------------------------------------------
			//[ReadNotLocked]
			//sec�t�@�C����ǂݍ���
			//throw: �g���q���قȂ�ꍇ 
			//return: �ǂݍ��񂾓��e
			//����1: �t�@�C���p�X (�g���q�܂�)
			//����2: �g���q�`�F�b�N���s�����ۂ�, default = true
			inline FileText ReadNotLocked(const sgstring& path, bool isCheckExtension)
			{
				WFileReadStream frs;//�t�@�C���X�g���[��

				//�g���q�`�F�b�N
				if(isCheckExtension && !sgstring::functions::IsEndWith(path, cSecNotLockedExtension))
					throw InvalidArgument(L"Error!! SecFileRW->ReadNotLocked", 
						L"Extension NotFound: " + path + L" Read extension sec");

				//�t�@�C�����J���Ȃ������ꍇ��throw�Ԃ�
				try { frs.Open(path); }
				catch (...) { throw; }

				sgstring getline;		//getline�p�o�b�t�@
				uint lineCount = 0;	//�s��
				//file���g
				std::wstring readBuf = frs.Read();
				//stringstream
				std::wstringstream read(readBuf);
				frs.Close();		//�t�@�C�������

				//�S�sgetline, �s���v�Z
				while (std::getline(read, getline))
					lineCount++;

				read.clear();							//clear
				read.seekg(0, std::ios::beg);	//seek
				FileText result(lineCount);		//return

				//�S�sgetline, vector�o�b�t�@��1�s�̗v�f�𕪊����
				while (std::getline(read, getline))
					result.emplace_back(sgstring::functions::Split(getline, ',', true));
			}

			//----------------------------------------------------------------------------------
			//[Read]
			//sec or secl�t�@�C����ǂݍ���
			//throw: �g���q���قȂ�ꍇ 
			//����1: �t�@�C���p�X (�g���q�܂�)
			//����2: �ǂݍ��񂾓��e�̑����
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
			//secl�t�@�C����ǂݍ���
			//throw: �g���q���قȂ�ꍇ 
			//����1: �t�@�C���p�X (�g���q�܂�)
			//����2: �ǂݍ��񂾓��e�̑����
			//����3: �g���q�`�F�b�N���s�����ۂ�, default = true
			inline void ReadLocked(const sgstring& path, FileText& write, bool isCheckExtension)
			{
				FileReadStream frs;//�t�@�C���X�g���[��

				//�g���q�`�F�b�N
				if (isCheckExtension && !sgstring::functions::IsEndWith(path, cSecNotLockedExtension))
					throw InvalidArgument(L"Error!! SecFileRW->ReadNotLocked",
						L"Extension NotFound: " + path + L" Read extension sec");

				//�t�@�C�����J���Ȃ������ꍇ��throw�Ԃ�
				try { frs.Open(path, true); }
				catch (...) { throw; }

				//�Í��������㕶����
				sgstring readUnlcok;
				//����
				EncryptionKey::Unlock(readUnlcok, frs.ReadUnite(), EncryptionKey::DefaultKey());
				frs.Close();		//�t�@�C�������

				sgstring getline;		//getline�p�o�b�t�@
				uint lineCount = 0;	//�s��
				//stringstream
				std::wstringstream read(readUnlcok);
		
				//�S�sgetline, �s���v�Z
				while (std::getline(read, getline))
					lineCount++;

				read.clear();							//clear
				read.seekg(0, std::ios::beg);	//seek
				write.clear();							//clear
				write.reserve(lineCount);		//get of lineCount

				//�S�sgetline, vector�o�b�t�@��1�s�̗v�f�𕪊����
				while (std::getline(read, getline))
					write.emplace_back(sgstring::functions::Split(getline, ',', true));
			}
			//----------------------------------------------------------------------------------
			//[ReadNotLocked]
			//sec�t�@�C����ǂݍ���
			//throw: �g���q���قȂ�ꍇ 
			//����1: �t�@�C���p�X (�g���q�܂�)
			//����2: �ǂݍ��񂾓��e�̑����
			//����3: �g���q�`�F�b�N���s�����ۂ�, default = true
			inline void ReadNotLocked(const sgstring& path, FileText& write, bool isCheckExtension)
			{
				WFileReadStream frs;//�t�@�C���X�g���[��

				//�g���q�`�F�b�N
				if (isCheckExtension && !sgstring::functions::IsEndWith(path, cSecLockedExtension))
					throw InvalidArgument(L"Error!! SecFileRW->ReadLocked",
						L"Extension NotFound: " + path + L" Read extension secl");

				//�t�@�C�����J���Ȃ������ꍇ��throw�Ԃ�
				try { frs.Open(path); }
				catch (...) { throw; }

				sgstring getline;		//getline�p�o�b�t�@
				uint lineCount = 0;	//�s��
				//file���g
				std::wstring readBuf = frs.Read();
				//stringstream
				std::wstringstream read(readBuf);
				frs.Close();		//�t�@�C�������

				//�S�sgetline, �s���v�Z
				while (std::getline(read, getline))
					lineCount++;

				read.clear();							//clear
				read.seekg(0, std::ios::beg);	//seek
				write.clear();							//clear
				write.reserve(lineCount);		//get of lineCount

				//�S�sgetline, vector�o�b�t�@��1�s�̗v�f�𕪊����
				while (std::getline(read, getline))
				{
					auto string = sgstring::functions::Split(getline, ',', true);
					if(IS_FALSE(string.empty())) write.emplace_back(std::move(string));
				}
			}

			//----------------------------------------------------------------------------------
			//[Write]
			//sec or secl�t�@�C������������
			//throw: �g���q���قȂ�ꍇ 
			//����1: �t�@�C���p�X (�g���q�܂�)
			//����2: �������ޓ��e
			//����3: �ǉ��������݃��[�h�Ȃ�true, default = false
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
			//secl�t�@�C������������
			//throw: �g���q���قȂ�ꍇ 
			//����1: �t�@�C���p�X (�g���q�܂�)
			//����2: �������ޓ��e
			//����3: �ǉ��������݃��[�h�Ȃ�true, default = false
			//����4: �g���q�`�F�b�N���s�����ۂ�, default = true
			inline void WriteLocked(const sgstring& path, const FileText& write, bool isAdd, bool isCheckExtension)
			{
				FileWriteStream fws;

				//�g���q�`�F�b�N
				if (isCheckExtension && !sgstring::functions::IsEndWith(path, cSecLockedExtension))
					throw InvalidArgument(L"Error!! SecFileRW->WriteLocked",
						L"Extension NotFound: " + path + L" Read extension secl");

				//�t�@�C�����J���Ȃ������ꍇ��throw�Ԃ�
				try { fws.Open(path, true, isAdd); }
				catch (...) { throw; }

				std::string join;
				//�S�v�f���[�v
				for (auto& it : write)
				{
					//�������ݗpstring��1�v�f�̏������ޓ��e��A�����đ��
					fws.WriteSplit(EncryptionKey::Lock(sgstring::functions::Join(it, ',', true), EncryptionKey::DefaultKey()));
				}
				fws.Close();			//�t�@�C�������
			}
			//----------------------------------------------------------------------------------
			//[WriteNotLocked]
			//sec�t�@�C������������
			//throw: �g���q���قȂ�ꍇ 
			//����1: �t�@�C���p�X (�g���q�܂�)
			//����2: �������ޓ��e
			//����3: �ǉ��������݃��[�h�Ȃ�true, default = false
			//����4: �g���q�`�F�b�N���s�����ۂ�, default = true
			inline void WriteNotLocked(const sgstring& path, const FileText& write, bool isAdd, bool isCheckExtension)
			{
				WFileWriteStream fws;

				//�g���q�`�F�b�N
				if (isCheckExtension && !sgstring::functions::IsEndWith(path, cSecNotLockedExtension))
					throw InvalidArgument(L"Error!! SecFileRW->WriteNotLocked",
						L"Extension NotFound: " + path + L" Read extension sec");

				//�t�@�C�����J���Ȃ������ꍇ��throw�Ԃ�
				try { fws.Open(path, false, isAdd); }
				catch (...) { throw; }
				
				//�S�v�f���[�v
				for (const auto& it : write)
				{
					//�������ݗpstring��1�v�f�̏������ޓ��e��A�����đ��
					fws.Write(sgstring::functions::Join(it, ',', true));		//�t�@�C���ɏ�������
				}
				fws.Close();			//�t�@�C�������
			}
		};


		}
	}


#endif // !SGFRAMEWORK_HEADER_SEC_FILE_RW_HPP_