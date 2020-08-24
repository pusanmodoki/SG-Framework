/*----------------------------------------------------------------------------------
�e�L�X�g�`���œǂݏ�������TextFileRW namespace
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
			//�e�L�X�g�`���Ńt�@�C����ǂݍ���
			//return: �ǂݍ��񂾓��e
			//����1: �t�@�C���p�X (�g���q�܂�)
			//����2: �o�C�i���ǂݍ���, default = false
			inline sgstring Read(const sgstring& path, const bool isBinary = false);
			//----------------------------------------------------------------------------------
			//[Read]
			//�e�L�X�g�`���Ńt�@�C����ǂݍ���
			//����1: �t�@�C���p�X (�g���q�܂�)
			//����2: �ǂݍ��񂾓��e�̑����
			//����3: �o�C�i���ǂݍ���, default = false
			inline void Read(const sgstring& path, sgstring& write, const bool isBinary = false);
			//----------------------------------------------------------------------------------
			//[Write]
			//�e�L�X�g�`���Ńt�@�C������������
			//����1: �t�@�C���p�X (�g���q�܂�)
			//����2: �������ޓ��e 
			//����3: �o�C�i����������, default = false
			//����4: �ǉ���������, default = false
			inline void Write(const sgstring& path, const sgstring& write, bool isBinary = false, bool isAdd = false);
			//----------------------------------------------------------------------------------
			//[ReadBinaryRead]
			//�o�C�i���`���Ńt�@�C����ǂݍ���
			//return: �ǂݍ��񂾓��e (byte)
			//����1: �t�@�C���p�X (�g���q�܂�)
			inline std::vector<byte> ReadBinary(const sgstring& path);
			//----------------------------------------------------------------------------------
			//[ReadBinaryRead]
			//�o�C�i���`���Ńt�@�C����ǂݍ���
			//����1: �t�@�C���p�X (�g���q�܂�)
			//����2: �ǂݍ��񂾓��e��������vector (byte)
			inline void ReadBinary(const sgstring& path, std::vector<byte>& write);
			//----------------------------------------------------------------------------------
			//[WriteBinary]
			//�o�C�i���`���Ńt�@�C������������
			//����1: �t�@�C���p�X (�g���q�܂�)
			//����2: �������ޓ��e (byte)
			inline void WriteBinary(const sgstring& path, const std::vector<byte>& write, bool isAdd = false);





			//----------------------------------------------------------------------------------
			//[Read]
			//�e�L�X�g�`���Ńt�@�C����ǂݍ���
			//return: �ǂݍ��񂾓��e
			//����1: �t�@�C���p�X (�g���q�܂�)
			//����2: �o�C�i���ǂݍ���, default = false	
			inline sgstring Read(const sgstring& path, const bool isBinary)
			{
				WFileReadStream frs;
				
				try { frs.Open(path, isBinary); }
				catch (...) { throw; }
				
				return frs.Read();
			}
			//----------------------------------------------------------------------------------
			//[Read]
			//�e�L�X�g�`���Ńt�@�C����ǂݍ���
			//����1: �t�@�C���p�X (�g���q�܂�)
			//����2: �ǂݍ��񂾓��e�̑����
			//����3: �o�C�i���ǂݍ���, default = false
			inline void Read(const sgstring& path, sgstring& write, const bool isBinary)
			{
				WFileReadStream frs;

				try { frs.Open(path, isBinary); }
				catch (...) { throw; }

				frs.Read(write);
			}
			//----------------------------------------------------------------------------------
			//[Write]
			//�e�L�X�g�`���Ńt�@�C������������
			//����1: �t�@�C���p�X (�g���q�܂�)
			//����2: �������ޓ��e 
			//����3: �o�C�i����������, default = false
			//����4: �ǉ���������, default = false
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
			//�o�C�i���`���Ńt�@�C����ǂݍ���
			//return: �ǂݍ��񂾓��e (byte)
			//����1: �t�@�C���p�X (�g���q�܂�)
			std::vector<byte> ReadBinary(const sgstring & path)
			{
				FileReadStream frs;

				try { frs.Open(path, true); }
				catch (...) { throw; }

				return frs.ReadVector();
			}
			//----------------------------------------------------------------------------------
			//[ReadBinaryRead]
			//�o�C�i���`���Ńt�@�C����ǂݍ���
			//����1: �t�@�C���p�X (�g���q�܂�)
			//����2: �ǂݍ��񂾓��e��������vector (byte)
			void ReadBinary(const sgstring & path, std::vector<byte>& write)
			{
				FileReadStream frs;

				try { frs.Open(path, true); }
				catch (...) { throw; }

				frs.ReadVector(write);
			}
			//----------------------------------------------------------------------------------
			//[WriteBinary]
			//�o�C�i���`���Ńt�@�C������������
			//����1: �t�@�C���p�X (�g���q�܂�)
			//����2: �������ޓ��e (byte)
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
