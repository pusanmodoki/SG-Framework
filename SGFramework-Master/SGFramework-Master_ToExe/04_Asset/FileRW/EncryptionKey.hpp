/*----------------------------------------------------------------------------------
�Í����L�[������EncryptionKey class
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_ENCRYPTION_KEY_HPP_
#define SGFRAMEWORK_HEADER_ENCRYPTION_KEY_HPP_
#include "../02_CustomVariable/CustomVariable.hpp"

// Framework namespace
namespace SGFramework
{
	//File Edit
	namespace File
	{
		//�Í����L�[������EncryptionKey class
		class EncryptionKey final
		{
		public:
			//delete
			inline EncryptionKey() = delete;
			//----------------------------------------------------------------------------------
			//[�R���X�g���N�^]
			//�L�[��������
			//����1: value
			inline EncryptionKey(const wchar value = 0) : value(m_value) { m_value = value; }
			//----------------------------------------------------------------------------------
			//[�R���X�g���N�^]
			//�L�[�𐶐�����
			//����1: �V�[�h
			inline EncryptionKey(const sgstring& seed) : value(m_value) { *this = CreateKey(seed); }
			//----------------------------------------------------------------------------------
			//[�R���X�g���N�^]
			//�L�[�𐶐�����
			//����1: �V�[�h
			inline EncryptionKey(const int seed) : value(m_value) { *this = CreateKey(seed); }

			//----------------------------------------------------------------------------------
			//[CreateKey]
			//�L�[�𐶐�����
			//return: ���������L�[
			//����1: �V�[�h
			inline static EncryptionKey CreateKey(const sgstring& seed);
			//----------------------------------------------------------------------------------
			//[CreateKey]
			//�L�[�𐶐�����
			//return: ���������L�[
			//����1: �V�[�h
			inline static EncryptionKey CreateKey(const int seed);

			//----------------------------------------------------------------------------------
			//[InitDefaultKey]
			//�e�֐��œK�p�����f�t�H���g�C���X�^���X��ݒ肷��
			//return: �ݒ肵���L�[
			//����1: �ݒ肷��L�[
			inline static EncryptionKey InitDefaultKey(EncryptionKey key) { return Key(&key); }
			//----------------------------------------------------------------------------------
			//[DefaultKey]
			//�e�֐��œK�p�����f�t�H���g�C���X�^���X���擾����
			//return: �ݒ肵���L�[
			inline static EncryptionKey DefaultKey() { return Key(nullptr); }

			//----------------------------------------------------------------------------------
			//[Lock]
			//��������Í�������
			//return: �Í���������
			//����1: �Í������镶����
			//����2: �g�p����L�[, default = DefaultKey
			inline static sgstring Lock(const sgstring& encryptionString, const EncryptionKey lockKey = DefaultKey());
			//----------------------------------------------------------------------------------
			//[Unlock]
			//��������Í�����������
			//return: �Í�������������
			//����1: �Í����������镶����
			//����2: �g�p����L�[, default = DefaultKey
			inline static sgstring Unlock(const sgstring& encryptionString, const EncryptionKey unlockKey = DefaultKey());
			//----------------------------------------------------------------------------------
			//[Lock]
			//��������Í�������
			//����1: �Í���������������镶����
			//����2: �Í������镶����
			//����3: �g�p����L�[, default = DefaultKey
			inline static void Lock(sgstring& string, const sgstring& encryptionString, const EncryptionKey lockKey = DefaultKey());
			//----------------------------------------------------------------------------------
			//[Unlock]
			//��������Í�����������
			//����1: �Í�������������������镶����
			//����2: �Í����������镶����
			//����3: �g�p����L�[, default = DefaultKey
			inline static void Unlock(sgstring& string, const sgstring& encryptionString, const EncryptionKey unlockKey = DefaultKey());

			//----------------------------------------------------------------------------------
			//[LockBinary]
			//��������Í�������, �o�C�i���� (byte)
			//return: �Í���������
			//����1: �Í������镶����
			//����2: �g�p����L�[, default = DefaultKey
			inline static std::vector<byte> LockBinary(const std::vector<byte>& encryptionString, const EncryptionKey lockKey = DefaultKey());
			//----------------------------------------------------------------------------------
			//[Unlock]
			//��������Í�����������, �o�C�i���� (byte)
			//return: �Í�������������
			//����1: �Í����������镶����
			//����2: �g�p����L�[, default = DefaultKey
			inline static std::vector<byte> UnlockBinary(const std::vector<byte>& encryptionString, const EncryptionKey unlockKey = DefaultKey());
			//----------------------------------------------------------------------------------
			//[Lock]
			//��������Í�������, �o�C�i���� (byte)
			//����1: �Í���������������镶����
			//����2: �Í������镶����
			//����3: �g�p����L�[, default = DefaultKey
			inline static void LockBinary(std::vector<byte>& string, const std::vector<byte>& encryptionString, const EncryptionKey lockKey = DefaultKey());
			//----------------------------------------------------------------------------------
			//[Unlock]
			//��������Í�����������, �o�C�i���� (byte)
			//����1: �Í�������������������镶����
			//����2: �Í����������镶����
			//����3: �g�p����L�[, default = DefaultKey
			inline static void UnlockBinary(std::vector<byte>& string, const std::vector<byte>& encryptionString, const EncryptionKey unlockKey = DefaultKey());

			//wchar operator
			inline operator wchar () { return m_value; }
			//byte operator
			inline operator byte () { return static_cast<byte>(m_value); }
			//() operator
			inline wchar operator () () { return m_value; }

			//�L�[
			GetOnlyProperty<wchar> value;

		private:
			//----------------------------------------------------------------------------------
			//[Key]
			//�X�^�e�B�b�N�C���X�^���X���Q�b�g, �Z�b�g����
			//return: �X�^�e�B�b�N�C���X�^���X
			//����1: Key�|�C���^, �l���� = Set, nullptr = Get
			inline static EncryptionKey Key(EncryptionKey* set);

			// = operator
			inline EncryptionKey operator = (const EncryptionKey& key) { this->m_value = key.value; return *this; }

			//KeyValue
			wchar m_value;		
		};




		//----------------------------------------------------------------------------------
		//[CreateKey]
		//�L�[�𐶐�����
		//return: ���������L�[
		//����1: �V�[�h
		inline EncryptionKey EncryptionKey::CreateKey(const sgstring& seed)
		{
			wchar value = 0;
			for (auto& e : seed)
				value ^= e;
			return EncryptionKey(value);
		}
		//----------------------------------------------------------------------------------
		//[CreateKey]
		//�L�[�𐶐�����
		//return: ���������L�[
		//����1: �V�[�h
		inline EncryptionKey EncryptionKey::CreateKey(const int seed)
		{
			wchar value = 0;
			for (int i = 0, _seed = seed; i < 4; i++, _seed >> 8)
				value ^= static_cast<wchar>(seed & 0xFFFF);
			return EncryptionKey(value);
		}


		//----------------------------------------------------------------------------------
		//[Lock]
		//��������Í�������
		//return: �Í���������
		//����1: �Í������镶����
		//����2: �g�p����L�[, default = DefaultKey
		inline sgstring EncryptionKey::Lock(const sgstring& encryptionString, const EncryptionKey lockKey)
		{
			sgstring result;
			result.reserve(encryptionString.size());
			for (auto& e : encryptionString)
				result.push_back(e ^ lockKey.m_value);
			return result;
		}
		//----------------------------------------------------------------------------------
		//[Unlock]
		//��������Í�����������
		//return: �Í�������������
		//����1: �Í����������镶����
		//����2: �g�p����L�[, default = DefaultKey
		inline sgstring EncryptionKey::Unlock(const sgstring& encryptionString, const EncryptionKey unlockKey)
		{
			sgstring result;
			result.reserve(encryptionString.size());
			for (auto& e : encryptionString)
				result.push_back(e ^ unlockKey.m_value);
			return result;
		}
		//----------------------------------------------------------------------------------
		//[Lock]
		//��������Í�������
		//����1: �Í���������������镶����
		//����2: �Í������镶����
		//����3: �g�p����L�[, default = DefaultKey
		inline void EncryptionKey::Lock(sgstring & string, const sgstring & encryptionString, const EncryptionKey lockKey)
		{
			string.clear();
			string.reserve(encryptionString.size());
			for (auto& e : encryptionString)
				string.push_back(e ^ lockKey.m_value);
		}
		//----------------------------------------------------------------------------------
		//[Unlock]
		//��������Í�����������
		//����1: �Í�������������������镶����
		//����2: �Í����������镶����
		//����3: �g�p����L�[, default = DefaultKey
		inline void EncryptionKey::Unlock(sgstring & string, const sgstring & encryptionString, const EncryptionKey unlockKey)
		{
			string.clear();
			string.reserve(encryptionString.size());
			for (auto& e : encryptionString)
				string.push_back(e ^ unlockKey.m_value);
		}


		//----------------------------------------------------------------------------------
		//[LockBinary]
		//��������Í�������, �o�C�i���� (byte)
		//return: �Í���������
		//����1: �Í������镶����
		//����2: �g�p����L�[, default = DefaultKey
		inline std::vector<byte> EncryptionKey::LockBinary(const std::vector<byte> & encryptionString, const EncryptionKey lockKey)
		{
			byte value = static_cast<byte>(lockKey.m_value);
			std::vector<byte> string(encryptionString.size());

			uint size = static_cast<uint>(string.size());
			for (uint i = 0; i < size; i++)
				string[i] = encryptionString[i] ^ value;

			return std::move(string);
		}
		//----------------------------------------------------------------------------------
		//[Unlock]
		//��������Í�����������, �o�C�i���� (byte)
		//return: �Í�������������
		//����1: �Í����������镶����
		//����2: �g�p����L�[, default = DefaultKey
		inline std::vector<byte> EncryptionKey::UnlockBinary(const std::vector<byte> & encryptionString, const EncryptionKey unlockKey)
		{
			byte value = static_cast<byte>(unlockKey.m_value);
			std::vector<byte> string(encryptionString.size());

			uint size = static_cast<uint>(string.size());
			for (uint i = 0; i < size; i++)
				string[i] = encryptionString[i] ^ value;

			return std::move(string);
		}
		//----------------------------------------------------------------------------------
		//[Lock]
		//��������Í�������, �o�C�i���� (byte)
		//����1: �Í���������������镶����
		//����2: �Í������镶����
		//����3: �g�p����L�[, default = DefaultKey
		inline void EncryptionKey::LockBinary(std::vector<byte>& string, const std::vector<byte>& encryptionString, const EncryptionKey lockKey)
		{
			byte value = static_cast<byte>(lockKey.m_value);
			string.resize(encryptionString.size());

			uint size = static_cast<uint>(string.size());
			for (uint i = 0; i < size; i++)
				string[i] = encryptionString[i] ^ value;
		}
		//----------------------------------------------------------------------------------
		//[Unlock]
		//��������Í�����������, �o�C�i���� (byte)
		//����1: �Í�������������������镶����
		//����2: �Í����������镶����
		//����3: �g�p����L�[, default = DefaultKey
		inline void EncryptionKey::UnlockBinary(std::vector<byte>& string, const std::vector<byte>& encryptionString, const EncryptionKey unlockKey)
		{
			byte value = static_cast<byte>(unlockKey.m_value);
			string.resize(encryptionString.size());

			uint size = static_cast<uint>(string.size());
			for (uint i = 0; i < size; i++)
				string[i] = encryptionString[i] ^ value;
		}



		//----------------------------------------------------------------------------------
		//[Key]
		//�X�^�e�B�b�N�C���X�^���X���Q�b�g, �Z�b�g����
		//return: �X�^�e�B�b�N�C���X�^���X
		//����1: Key�|�C���^, �l���� = Set, nullptr = Get
		inline EncryptionKey EncryptionKey::Key(EncryptionKey* set)
		{
			static EncryptionKey key(0);
			if (set != nullptr) key = *set;
			return key;
		}
	}
}

#endif // !SGFRAMEWORK_HEADER_ENCRYPTION_KEY_HPP_