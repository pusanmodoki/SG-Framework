/*----------------------------------------------------------------------------------
SYSTEMTIMEを拡張したSystemTime class,
FILETIMEを拡張したFileTime class,
Asset系クラスのベースとなるBaseAsset class
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_BASE_ASSET_HPP_
#define SGFRAMEWORK_HEADER_BASE_ASSET_HPP_
#include <Windows.h>
#include <iostream>
#include <mutex>
#include "../../02_CustomVariable/CustomVariable.hpp"

//Framework namespace
namespace SGFramework
{
	//SYSTEMTIMEを拡張したSystemTime class
	class SystemTime : public SYSTEMTIME 
	{ 
	public: 
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//0初期化
		inline SystemTime();
		//----------------------------------------------------------------------------------
		//[ToString]
		//年/月/日 時:分:秒 の形式で文字出力する
		//return: 文字列
		//引数1: 年月日の出力有無, default = true
		//引数2: 時分秒の出力有無, default = true
		inline sgstring ToString(bool isDate = true, bool isTime = true) const;
		//----------------------------------------------------------------------------------
		//[ToString]
		//時:分:秒 月/日/年 の形式で文字出力する
		//return: 文字列
		//引数1: 時分秒の出力有無, default = true
		//引数2: 月日年の出力有無, default = true
		inline sgstring ToStringUsStyle(bool isTime = true, bool isDate = true) const;
	};


	//FILETIMEを拡張したFileTime class 
	class FileTime : public FILETIME
	{
	public:
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//0初期化
		inline FileTime() { this->dwLowDateTime = 0; this->dwHighDateTime = 0; }
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化
		//引数1: dwLowDateTime
		//引数2: dwHighDateTime
		inline FileTime(ulong _dwLowDateTime, ulong _dwHighDateTime) { this->dwLowDateTime = _dwLowDateTime; this->dwHighDateTime = _dwHighDateTime; }
		//----------------------------------------------------------------------------------
		//[ToSystemTime]
		//SystemTime形式に変換
		//return: SystemTime(失敗した場合は全ての値が0)
		inline SystemTime ToSystemTime() const;
		//----------------------------------------------------------------------------------
		//[ToString]
		//年/月/日 時:分:秒 の形式で文字出力する
		//return: 文字列
		//引数1: 年月日の出力有無, default = true
		//引数2: 時分秒の出力有無, default = true
		inline sgstring ToString(bool isDate = true, bool isTime = true) const;
		//----------------------------------------------------------------------------------
		//[ToString]
		//時:分:秒 月/日/年 の形式で文字出力する
		//return: 文字列
		//引数1: 時分秒の出力有無, default = true
		//引数2: 月日年の出力有無, default = true
		inline sgstring ToStringUsStyle(bool isTime = true, bool isDate = true) const;
		//== operator
		inline bool operator == (const FileTime& fileTime) const;
		//!= operator
		inline bool operator != (const FileTime& fileTime) const;
	};

	namespace Asset
	{
		template <typename TAsset>
		inline SharedPointer<TAsset> InstantiateAsset(const sgstring& filePath, bool isRegisterSharedAsset);

		//Asset Types
		enum class AssetType
		{
			Null,
			Binary,
			Text,
			VertexShader,
			PixelShader,
			Texture,
			Model,
			AudioMusic,
			AudioEffect,
			AudioEffect3D
		};

		//----------------------------------------------------------------------------------
		//[StringForAssetType]
		//return: string(asset type)
		//引数1: Asset Type
		inline sgstring StringForAssetType(AssetType type)
		{
			switch (type)
			{
			case SGFramework::Asset::AssetType::Binary:
				return L"Binary";
			case SGFramework::Asset::AssetType::Text:
				return L"Text";
			case SGFramework::Asset::AssetType::VertexShader:
				return L"VertexShader";
			case SGFramework::Asset::AssetType::PixelShader:
				return L"PixelShader";
			case SGFramework::Asset::AssetType::Texture:
				return L"Texture";
			case SGFramework::Asset::AssetType::Model:
				return L"Model";
			case SGFramework::Asset::AssetType::AudioMusic:
				return L"AudioMusic";
			case SGFramework::Asset::AssetType::AudioEffect:
				return L"AudioEffect";
			case SGFramework::Asset::AssetType::AudioEffect3D:
				return L"AudioEffect3D";
			default:
				return L"Unknown";
			}
		}
		//----------------------------------------------------------------------------------
		//[StdStringForAssetType]
		//return: string(asset type)
		//引数1: Asset Type
		inline std::string StdStringForAssetType(AssetType type)
		{
			switch (type)
			{
			case SGFramework::Asset::AssetType::Binary:
				return "Binary";
			case SGFramework::Asset::AssetType::Text:
				return "Text";
			case SGFramework::Asset::AssetType::VertexShader:
				return "VertexShader";
			case SGFramework::Asset::AssetType::PixelShader:
				return "PixelShader";
			case SGFramework::Asset::AssetType::Texture:
				return "Texture";
			case SGFramework::Asset::AssetType::Model:
				return "Model";
			case SGFramework::Asset::AssetType::AudioMusic:
				return "AudioMusic";
			case SGFramework::Asset::AssetType::AudioEffect:
				return "AudioEffect";
			case SGFramework::Asset::AssetType::AudioEffect3D:
				return "AudioEffect3D";
			default:
				return "Unknown";
			}
		}
	}
	namespace Administrator
	{
		class AssetManager;
	}

	//Base Classes
	namespace BaseClass
	{
		//Asset系クラスのベースとなるBaseAsset class
		//引数が異なることが予想されるためOpen関数は記述していない
		//Object classを継承, Object->name = File Path
		class BaseAsset : public Object
		{
		public:
			friend class SGFramework::Administrator::AssetManager;
			template <typename TAsset>
			friend SharedPointer<TAsset> Asset::InstantiateAsset(const sgstring& filePath, bool isRegisterSharedAsset);

			//FileSizeを返却するFileSizeWith class
			struct FileSizeWith
			{
				friend BaseAsset;
	
				//file size(byte) (read only propery)
				SGF_FUNCTION_PROPERTY constexpr float getByte() const { return m_size; }
				//file size(kilo byte) (read only propery)
				SGF_FUNCTION_PROPERTY constexpr float getKiloByte() const { return m_size / DataVolumeConversion::cByteToKiloByteFloat; }
				//file size(mega byte) (read only propery)
				SGF_FUNCTION_PROPERTY constexpr float getMegaByte() const { return m_size / DataVolumeConversion::cByteToMegaByteFloat; }
				//file size(giga byte) (read only propery)
				SGF_FUNCTION_PROPERTY constexpr float getGigaByte() const { return m_size / DataVolumeConversion::cByteToGigaByteFloat; }

			private:
				//----------------------------------------------------------------------------------
				//[FileSizeWith]
				//初期化
				//引数1: サイズ
				inline FileSizeWith(uint64 size) : m_size(static_cast<float>(size)) {}

				float m_size;
			};


			//----------------------------------------------------------------------------------
			//[デストラクタ]
			//ファイルハンドルの開放を行う
			inline virtual ~BaseAsset() {}

			//----------------------------------------------------------------------------------
			//[Reload]
			//アセットのリロードを行う
			virtual void Reload() = 0;
			//----------------------------------------------------------------------------------
			//[Close]
			//ファイルを閉じる
			virtual void Close() = 0;


			//----------------------------------------------------------------------------------
			//[WaitLoading]
			//ロード完了するまで待機する, 既にロードが終わっていた場合即時に終了する
			//throw: open failed OR time out
			//引数1: time out seconds, default = 15.0f
			inline void WaitLoading(float timeOutSeconds = 15.0f);

			//asset type (read function property)
			SGF_FUNCTION_PROPERTY virtual Asset::AssetType getType() = 0;
			//file is open? (read function property)
			SGF_FUNCTION_PROPERTY bool getIsFileOpened() const { return m_isOpened.load(); }
			//file is open failed? (read function property)
			SGF_FUNCTION_PROPERTY bool getIsOpenFailed() const { return m_isOpeneFailed.load(); }
			//this shared asset? (read function property)
			SGF_FUNCTION_PROPERTY bool getIsSharedAsset() const { return m_isSharedAsset; }

			//更新時間
			GetOnlyProperty<FileTime> lastWriteTime = m_lastWriteTime;
			//ファイルサイズ
			GetOnlyProperty<FileSizeWith> fileSizes = m_fileSize;

		protected:
			//----------------------------------------------------------------------------------
			//[コンストラクタ]
			//0初期化
			inline BaseAsset() : Object(L"") {}
			//----------------------------------------------------------------------------------
			//[OpenBaseAsset]
			//ファイルをオープンしBaseAssetの変数を初期化する
			//引数1: ファイルパス
			inline void OpenBaseAsset();
			//----------------------------------------------------------------------------------
			//[ReloadBaseAsset]
			//ファイルをリロードしBaseAssetの変数を更新する
			inline void ReloadBaseAsset();
			//----------------------------------------------------------------------------------
			//[CloseBaseAsset]
			//BaseAsset側で開いているファイルハンドルを閉じる
			void CloseBaseAsset(Asset::AssetType type, bool isDestructor);
			//----------------------------------------------------------------------------------
			//[ExtractionForObjectName]
			//return: xx/yyy.zzz <- "yyy"
			inline sgstring ExtractionForObjectName();
			
			//open failed (set function property)
			SGF_FUNCTION_PROPERTY void setIsOpenFailed(bool set) { AtomicOperation::Init(m_isOpeneFailed, set); }

			WeakPointer<BaseAsset> m_thisPointer;

		private:

			//----------------------------------------------------------------------------------
			//[CheckForUpdates]
			//ファイル更新時間の確認を行う
			//return: 現在保存している値と更新時間が異なる場合はtrue
			inline bool CheckForUpdates() const;
			
			FileTime m_lastWriteTime = { 0, 0 };						//更新時間
			FileSizeWith m_fileSize = 0;									//ファイルサイズ
			HANDLE m_fileHandle = nullptr;								//ファイルハンドル
			std::atomic_bool m_isOpened = false;						//open?
			std::atomic_bool m_isOpeneFailed = false;				//open failed?
			bool m_isSharedAsset = false;									//shared asset?
		};
	}



	//----------------------------------------------------------------------------------
	//SystemTime
	//----------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------
	//[wostream operator]
	//引数1: stream
	//引数2: this
	inline std::wostream& operator<<(std::wostream& stream, const SystemTime & value)
	{
		const wchar* string = value.ToString();
		stream << string;
		return stream;
	}
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//0初期化
	inline SystemTime::SystemTime()
	{
		wYear = 0; wMonth = 0; wDayOfWeek = 0;
		wDay = 0; wHour = 0; wMinute = 0;
		wSecond = 0; wMilliseconds = 0;
	}
	//----------------------------------------------------------------------------------
	//[ToString]
	//年/月/日 時:分:秒 の形式で文字出力する
	//return: 文字列
	//引数1: 年月日の出力有無, default = true
	//引数2: 時分秒の出力有無, default = true
	inline sgstring SystemTime::ToString(bool isDate, bool isTime) const
	{
		sgstring result = L"";
		if (isDate)
		{
			result = sgstring::to_sgstring(this->wYear) + L"/"
				+ sgstring::to_sgstring(this->wMonth) + L"/"
				+ sgstring::to_sgstring(this->wDay);
			if (isTime) result += L" ";
		}
		if (isTime)
		{
			result += sgstring::to_sgstring(this->wHour) + L":"
				+ sgstring::to_sgstring(this->wMinute) + L":"
				+ sgstring::to_sgstring(this->wSecond);
		}
		return result;
	}
	//----------------------------------------------------------------------------------
	//[ToString]
	//時:分:秒 月/日/年 の形式で文字出力する
	//return: 文字列
	//引数1: 時分秒の出力有無, default = true
	//引数2: 月日年の出力有無, default = true
	inline sgstring SystemTime::ToStringUsStyle(bool isTime, bool isDate) const
	{
		sgstring result = L"";
		if (isTime)
		{
			result = sgstring::to_sgstring(this->wHour) + L":"
				+ sgstring::to_sgstring(this->wMinute) + L":"
				+ sgstring::to_sgstring(this->wSecond);
			if (isDate) result += L" ";
		}
		if (isDate)
		{
			result += sgstring::to_sgstring(this->wMonth) + L"/"
				+ sgstring::to_sgstring(this->wDay) + L"/"
				+ sgstring::to_sgstring(this->wYear);
		}
		return result;
	}



	//----------------------------------------------------------------------------------
	//FileTime
	//----------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------
	//[wostream operator]
	//引数1: stream
	//引数2: this
	inline std::wostream& operator<<(std::wostream& stream, const FileTime & value)
	{
		const wchar* string = value.ToString();
		stream << string;
		return stream;
	}
	//----------------------------------------------------------------------------------
	//[ToSystemTime]
	//SystemTime形式に変換
	//return: SystemTime(失敗した場合は全ての値が0)
	inline SystemTime FileTime::ToSystemTime() const
	{
		SystemTime systemTime;
		if (FileTimeToSystemTime(this, &systemTime) == 0) return SystemTime();
		return systemTime;
	}
	//----------------------------------------------------------------------------------
	//[ToString]
	//年/月/日 時:分:秒 の形式で文字出力する
	//return: 文字列
	//引数1: 年月日の出力有無, default = true
	//引数2: 時分秒の出力有無, default = true
	inline sgstring FileTime::ToString(bool isDate, bool isTime) const
	{
		SystemTime systemTime;
		
		if (FileTimeToSystemTime(this, &systemTime) == 0) return L"FileTime->ToString Failed";
		
		return systemTime.ToString(isDate, isTime);
	}
	//----------------------------------------------------------------------------------
	//[ToString]
	//時:分:秒 月/日/年 の形式で文字出力する
	//return: 文字列
	//引数1: 時分秒の出力有無, default = true
	//引数2: 月日年の出力有無, default = true
	inline sgstring FileTime::ToStringUsStyle(bool isTime, bool isDate) const
	{
		SystemTime systemTime;

		if (FileTimeToSystemTime(this, &systemTime) == 0) return L"FileTime->ToString Failed";

		return systemTime.ToStringUsStyle(isTime, isDate);
	}
	//== operator
	inline bool FileTime::operator==(const FileTime & fileTime) const
	{
		return (this->dwLowDateTime == fileTime.dwLowDateTime
			&& this->dwHighDateTime == fileTime.dwHighDateTime);
	}
	//!= operator
	inline bool FileTime::operator!=(const FileTime & fileTime) const
	{
		return (this->dwLowDateTime != fileTime.dwLowDateTime
			|| this->dwHighDateTime != fileTime.dwHighDateTime);
	}



	//----------------------------------------------------------------------------------
	//BaseAsset
	//----------------------------------------------------------------------------------

	//Base Classes
	namespace BaseClass
	{
		//----------------------------------------------------------------------------------
		//[OpenBaseAsset]
		//ファイルをオープンしBaseAssetの変数を初期化する
		inline void BaseAsset::OpenBaseAsset()
		{
			//open?
			if (IS_TRUE(getIsFileOpened())) return;

			//開く
			m_fileHandle = CreateFile(m_name.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

			ulong high, low;	//上位, 下位32ビット
			//ゲット
			low = GetFileSize(m_fileHandle, &high);
			//合成
			m_fileSize.m_size = static_cast<float>(((int64)high << 32) | low);

			//ゲット
			GetFileTime(m_fileHandle, nullptr, nullptr, &m_lastWriteTime);

			AtomicOperation::Init(m_isOpened, true);
		}

		//----------------------------------------------------------------------------------
		//[ReloadBaseAsset]
		//ファイルをリロードしBaseAssetの変数を更新する
		inline void BaseAsset::ReloadBaseAsset()
		{
			//開いてなかったら終了
			if (IS_FALSE(getIsFileOpened())) return;

			ulong high, low;	//上位, 下位32ビット
			//ゲット
			low = GetFileSize(m_fileHandle, &high);
			//合成
			m_fileSize.m_size = static_cast<float>(((int64)high << 32) | low);

			//ゲット
			GetFileTime(m_fileHandle, nullptr, nullptr, &m_lastWriteTime);
		}

		//----------------------------------------------------------------------------------
		//[WaitLoading]
		//ロード完了するまで待機する, 既にロードが終わっていた場合即時に終了する
		//throw: open failed OR time out
		//引数1: time out seconds, default = 15.0f
		inline void BaseAsset::WaitLoading(float timeOutSeconds)
		{
			TimeProcessing::TimeCounter counter;
			counter.Start();

			while (IS_FALSE(m_isOpened.load()))
			{
				bool isLoad = m_isOpened.load();

				if (m_isOpeneFailed.load() & (m_isOpened.load() ^ 1))
					throw InvalidArgument(L"Error!! BaseAsset->WaitLoading", L"open file failed");
				else if (counter() >= timeOutSeconds && (m_isOpened.load() ^ 1))
					throw InvalidArgument(L"Error!! BaseAsset->WaitLoading", L"time out");
			}
		}

		//----------------------------------------------------------------------------------
		//[ExtractionForObjectName]
		//return: xx/yyy.zzz <- "yyy"
		inline sgstring BaseAsset::ExtractionForObjectName()
		{
			if (IS_FALSE(getIsFileOpened())) return L"";

			//区切る場所を検索
			size_t slash = m_name.find_last_of(L'/');
			size_t dot = m_name.find_last_of(L'.');
	
			//nposなら先頭
			if (slash == sgstring::npos) slash = 0;
			//最後じゃなければ一つすすめる
			else if (slash != m_name.length() - 1) ++slash;

			//抽出
			return std::move(m_name.substr(slash, dot - slash));
		}
		//----------------------------------------------------------------------------------
		//[CheckForUpdates]
		//ファイル更新時間の確認を行う
		//return: 現在保存している値と更新時間が異なる場合はtrue
		inline bool BaseAsset::CheckForUpdates() const
		{
			FileTime check;
			GetFileTime(m_fileHandle, nullptr, nullptr, &check);

			return (check != m_lastWriteTime);
		}
	}
}

#endif // !SGFRAMEWORK_HEADER_BASE_ASSET_HPP_