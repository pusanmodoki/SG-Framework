/*----------------------------------------------------------------------------------
暗号化キーを扱うEncryptionKey class
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
		//暗号化キーを扱うEncryptionKey class
		class EncryptionKey final
		{
		public:
			//delete
			inline EncryptionKey() = delete;
			//----------------------------------------------------------------------------------
			//[コンストラクタ]
			//キーを代入する
			//引数1: value
			inline EncryptionKey(const wchar value = 0) : value(m_value) { m_value = value; }
			//----------------------------------------------------------------------------------
			//[コンストラクタ]
			//キーを生成する
			//引数1: シード
			inline EncryptionKey(const sgstring& seed) : value(m_value) { *this = CreateKey(seed); }
			//----------------------------------------------------------------------------------
			//[コンストラクタ]
			//キーを生成する
			//引数1: シード
			inline EncryptionKey(const int seed) : value(m_value) { *this = CreateKey(seed); }

			//----------------------------------------------------------------------------------
			//[CreateKey]
			//キーを生成する
			//return: 生成したキー
			//引数1: シード
			inline static EncryptionKey CreateKey(const sgstring& seed);
			//----------------------------------------------------------------------------------
			//[CreateKey]
			//キーを生成する
			//return: 生成したキー
			//引数1: シード
			inline static EncryptionKey CreateKey(const int seed);

			//----------------------------------------------------------------------------------
			//[InitDefaultKey]
			//各関数で適用されるデフォルトインスタンスを設定する
			//return: 設定したキー
			//引数1: 設定するキー
			inline static EncryptionKey InitDefaultKey(EncryptionKey key) { return Key(&key); }
			//----------------------------------------------------------------------------------
			//[DefaultKey]
			//各関数で適用されるデフォルトインスタンスを取得する
			//return: 設定したキー
			inline static EncryptionKey DefaultKey() { return Key(nullptr); }

			//----------------------------------------------------------------------------------
			//[Lock]
			//文字列を暗号化する
			//return: 暗号化文字列
			//引数1: 暗号化する文字列
			//引数2: 使用するキー, default = DefaultKey
			inline static sgstring Lock(const sgstring& encryptionString, const EncryptionKey lockKey = DefaultKey());
			//----------------------------------------------------------------------------------
			//[Unlock]
			//文字列を暗号化解除する
			//return: 暗号化解除文字列
			//引数1: 暗号化解除する文字列
			//引数2: 使用するキー, default = DefaultKey
			inline static sgstring Unlock(const sgstring& encryptionString, const EncryptionKey unlockKey = DefaultKey());
			//----------------------------------------------------------------------------------
			//[Lock]
			//文字列を暗号化する
			//引数1: 暗号化文字列を代入する文字列
			//引数2: 暗号化する文字列
			//引数3: 使用するキー, default = DefaultKey
			inline static void Lock(sgstring& string, const sgstring& encryptionString, const EncryptionKey lockKey = DefaultKey());
			//----------------------------------------------------------------------------------
			//[Unlock]
			//文字列を暗号化解除する
			//引数1: 暗号化解除文字列を代入する文字列
			//引数2: 暗号化解除する文字列
			//引数3: 使用するキー, default = DefaultKey
			inline static void Unlock(sgstring& string, const sgstring& encryptionString, const EncryptionKey unlockKey = DefaultKey());

			//----------------------------------------------------------------------------------
			//[LockBinary]
			//文字列を暗号化する, バイナリ版 (byte)
			//return: 暗号化文字列
			//引数1: 暗号化する文字列
			//引数2: 使用するキー, default = DefaultKey
			inline static std::vector<byte> LockBinary(const std::vector<byte>& encryptionString, const EncryptionKey lockKey = DefaultKey());
			//----------------------------------------------------------------------------------
			//[Unlock]
			//文字列を暗号化解除する, バイナリ版 (byte)
			//return: 暗号化解除文字列
			//引数1: 暗号化解除する文字列
			//引数2: 使用するキー, default = DefaultKey
			inline static std::vector<byte> UnlockBinary(const std::vector<byte>& encryptionString, const EncryptionKey unlockKey = DefaultKey());
			//----------------------------------------------------------------------------------
			//[Lock]
			//文字列を暗号化する, バイナリ版 (byte)
			//引数1: 暗号化文字列を代入する文字列
			//引数2: 暗号化する文字列
			//引数3: 使用するキー, default = DefaultKey
			inline static void LockBinary(std::vector<byte>& string, const std::vector<byte>& encryptionString, const EncryptionKey lockKey = DefaultKey());
			//----------------------------------------------------------------------------------
			//[Unlock]
			//文字列を暗号化解除する, バイナリ版 (byte)
			//引数1: 暗号化解除文字列を代入する文字列
			//引数2: 暗号化解除する文字列
			//引数3: 使用するキー, default = DefaultKey
			inline static void UnlockBinary(std::vector<byte>& string, const std::vector<byte>& encryptionString, const EncryptionKey unlockKey = DefaultKey());

			//wchar operator
			inline operator wchar () { return m_value; }
			//byte operator
			inline operator byte () { return static_cast<byte>(m_value); }
			//() operator
			inline wchar operator () () { return m_value; }

			//キー
			GetOnlyProperty<wchar> value;

		private:
			//----------------------------------------------------------------------------------
			//[Key]
			//スタティックインスタンスをゲット, セットする
			//return: スタティックインスタンス
			//引数1: Keyポインタ, 値あり = Set, nullptr = Get
			inline static EncryptionKey Key(EncryptionKey* set);

			// = operator
			inline EncryptionKey operator = (const EncryptionKey& key) { this->m_value = key.value; return *this; }

			//KeyValue
			wchar m_value;		
		};




		//----------------------------------------------------------------------------------
		//[CreateKey]
		//キーを生成する
		//return: 生成したキー
		//引数1: シード
		inline EncryptionKey EncryptionKey::CreateKey(const sgstring& seed)
		{
			wchar value = 0;
			for (auto& e : seed)
				value ^= e;
			return EncryptionKey(value);
		}
		//----------------------------------------------------------------------------------
		//[CreateKey]
		//キーを生成する
		//return: 生成したキー
		//引数1: シード
		inline EncryptionKey EncryptionKey::CreateKey(const int seed)
		{
			wchar value = 0;
			for (int i = 0, _seed = seed; i < 4; i++, _seed >> 8)
				value ^= static_cast<wchar>(seed & 0xFFFF);
			return EncryptionKey(value);
		}


		//----------------------------------------------------------------------------------
		//[Lock]
		//文字列を暗号化する
		//return: 暗号化文字列
		//引数1: 暗号化する文字列
		//引数2: 使用するキー, default = DefaultKey
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
		//文字列を暗号化解除する
		//return: 暗号化解除文字列
		//引数1: 暗号化解除する文字列
		//引数2: 使用するキー, default = DefaultKey
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
		//文字列を暗号化する
		//引数1: 暗号化文字列を代入する文字列
		//引数2: 暗号化する文字列
		//引数3: 使用するキー, default = DefaultKey
		inline void EncryptionKey::Lock(sgstring & string, const sgstring & encryptionString, const EncryptionKey lockKey)
		{
			string.clear();
			string.reserve(encryptionString.size());
			for (auto& e : encryptionString)
				string.push_back(e ^ lockKey.m_value);
		}
		//----------------------------------------------------------------------------------
		//[Unlock]
		//文字列を暗号化解除する
		//引数1: 暗号化解除文字列を代入する文字列
		//引数2: 暗号化解除する文字列
		//引数3: 使用するキー, default = DefaultKey
		inline void EncryptionKey::Unlock(sgstring & string, const sgstring & encryptionString, const EncryptionKey unlockKey)
		{
			string.clear();
			string.reserve(encryptionString.size());
			for (auto& e : encryptionString)
				string.push_back(e ^ unlockKey.m_value);
		}


		//----------------------------------------------------------------------------------
		//[LockBinary]
		//文字列を暗号化する, バイナリ版 (byte)
		//return: 暗号化文字列
		//引数1: 暗号化する文字列
		//引数2: 使用するキー, default = DefaultKey
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
		//文字列を暗号化解除する, バイナリ版 (byte)
		//return: 暗号化解除文字列
		//引数1: 暗号化解除する文字列
		//引数2: 使用するキー, default = DefaultKey
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
		//文字列を暗号化する, バイナリ版 (byte)
		//引数1: 暗号化文字列を代入する文字列
		//引数2: 暗号化する文字列
		//引数3: 使用するキー, default = DefaultKey
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
		//文字列を暗号化解除する, バイナリ版 (byte)
		//引数1: 暗号化解除文字列を代入する文字列
		//引数2: 暗号化解除する文字列
		//引数3: 使用するキー, default = DefaultKey
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
		//スタティックインスタンスをゲット, セットする
		//return: スタティックインスタンス
		//引数1: Keyポインタ, 値あり = Set, nullptr = Get
		inline EncryptionKey EncryptionKey::Key(EncryptionKey* set)
		{
			static EncryptionKey key(0);
			if (set != nullptr) key = *set;
			return key;
		}
	}
}

#endif // !SGFRAMEWORK_HEADER_ENCRYPTION_KEY_HPP_