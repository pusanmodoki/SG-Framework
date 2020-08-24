/*----------------------------------------------------------------------------------
Asset系クラスのベースとなるBaseAsset class
------------------------------------------------------------------------------------*/
#include "BaseAsset.hpp"
#include "../AssetManager.hpp"

//Framework namespace
namespace SGFramework
{
	//Base Classes
	namespace BaseClass
	{
		//----------------------------------------------------------------------------------
		//[CloseBaseAsset]
		//BaseAsset側で開いているファイルハンドルを閉じる
		void BaseAsset::CloseBaseAsset(Asset::AssetType type, bool isDestructor)
		{
			//開いてなかったら終了
			if (IS_FALSE(getIsFileOpened())) return;

			if (isDestructor)
				Administrator::AssetManager::UnregisterSharedAsset(m_name, type, instanceID());

			//閉じる
			CloseHandle(m_fileHandle);
			//初期化
			m_fileHandle = nullptr;

			AtomicOperation::Init(m_isOpened, false);
			AtomicOperation::Init(m_isOpeneFailed, false);
		}
	}
}