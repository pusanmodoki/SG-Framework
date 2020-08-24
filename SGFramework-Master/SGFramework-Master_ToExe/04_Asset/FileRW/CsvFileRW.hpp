/*----------------------------------------------------------------------------------
CSV�`���œǂݏ�������CsvFileRW namespace
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
			//csv�g���q
			constexpr wchar cCsvExtension[] = L".csv";

			//----------------------------------------------------------------------------------
			//[Read]
			//CSV�`���Ńt�@�C����ǂݍ���
			//return: �ǂݍ��񂾓��e
			//����1: �t�@�C���p�X (�g���q�܂�)
			//����2: �o�C�i���ǂݍ���, default = false
			//����3: �g���q�`�F�b�N, default = true
			inline FileText Read(const sgstring& path, bool isBinary = false, bool isCheckExtension = true)
			{
				WFileReadStream frs;//�t�@�C���X�g���[��

				//�g���q�`�F�b�N
				if (isCheckExtension && !sgstring::functions::IsEndWith(path, cCsvExtension))
					throw InvalidArgument(L"Error!! CsvFileRW->Read",
						L"Extension NotFound: " + path + L" Read extension csv");

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
				FileText result(lineCount);		//�Ԃ�l

				//�S�sgetline, vector�o�b�t�@��1�s�̗v�f�𕪊����
				while (std::getline(read, getline))
					result.emplace_back(sgstring::functions::Split(getline, ',', true));
				
				
				//return
				return result;
			}

			//----------------------------------------------------------------------------------
			//[Read]
			//�e�L�X�g�`���Ńt�@�C����ǂݍ���
			//����1: �t�@�C���p�X (�g���q�܂�)
			//����2: �ǂݍ��񂾓��e�̑����
			//����3: �o�C�i���ǂݍ���, default = false
			//����4: �g���q�`�F�b�N, default = true
			inline void Read(const sgstring& path, FileText& write, bool isBinary = false, bool isCheckExtension = true)
			{
				FileReadStream frs;//�t�@�C���X�g���[��

				//�g���q�`�F�b�N
				if (isCheckExtension && !sgstring::functions::IsEndWith(path, cCsvExtension))
					throw InvalidArgument(L"Error!! CsvFileRW->Read",
						L"Extension NotFound: " + path + L" Read extension csv");

				//�t�@�C�����J���Ȃ������ꍇ��throw�Ԃ�
				try { frs.Open(path, true); }
				catch (...) { throw; }

				//�Í��������㕶����
				sgstring readUnlcok = frs.ReadUnite();
				//����
				readUnlcok = EncryptionKey::Unlock(readUnlcok, EncryptionKey::DefaultKey());
				frs.Close();		//�t�@�C�������

				std::wstringstream read(readUnlcok);		//stringstream
				sgstring getline;										//getline�p�o�b�t�@
				uint lineCount = 0;									//�s���J�E���g

				//�S�sgetline, �s���v�Z
				while (std::getline(read, getline))
					lineCount++;
				
				read.clear();							//clear
				read.seekg(0, std::ios::beg);	//seek
				write.clear();							//clear
				write.reserve(lineCount);		//get of lineCount

				//�S�sgetline
				while (std::getline(read, getline))
					//vector�o�b�t�@��1�s�̗v�f�𕪊����
					write.emplace_back(sgstring::functions::Split(getline, ',', true));
			}

			//----------------------------------------------------------------------------------
			//[Write]
			//�e�L�X�g�`���Ńt�@�C������������
			//����1: �t�@�C���p�X (�g���q�܂�)
			//����2: �������ޓ��e 
			//����3: �o�C�i����������, default = false
			//����4: �ǉ���������, default = false
			//����5: �g���q�`�F�b�N, default = true
			inline void Write(const sgstring& path, const FileText& write, bool isBinary = false, bool isAdd = false, bool isCheckExtension = true)
			{
				WFileWriteStream fws;

				//�g���q�`�F�b�N
				if (isCheckExtension && !sgstring::functions::IsEndWith(path, cCsvExtension))
					throw InvalidArgument(L"Error!! CsvFileRW->Write",
						L"Extension NotFound: " + path + L" Read extension csv");

				//�t�@�C�����J���Ȃ������ꍇ��throw�Ԃ�
				try { fws.Open(path, false, isAdd); }
				catch (...) { throw; }

				//�S�v�f���[�v
				for (const auto& it : write)
				{
					//�������ݗpstring��1�v�f�̏������ޓ��e��A�����đ��
					sgstring string = sgstring::functions::Join(it, ',', true);
					fws.Write(string);		//�t�@�C���ɏ�������
				}
				fws.Close();			//�t�@�C�������
			}
		}
	}
}
#endif // !SGFRAMEWORK_HEADER_CSV_FILE_RW_HPP_
