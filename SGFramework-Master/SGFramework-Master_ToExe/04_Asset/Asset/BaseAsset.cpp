/*----------------------------------------------------------------------------------
Asset�n�N���X�̃x�[�X�ƂȂ�BaseAsset class
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
		//BaseAsset���ŊJ���Ă���t�@�C���n���h�������
		void BaseAsset::CloseBaseAsset(Asset::AssetType type, bool isDestructor)
		{
			//�J���ĂȂ�������I��
			if (IS_FALSE(getIsFileOpened())) return;

			if (isDestructor)
				Administrator::AssetManager::UnregisterSharedAsset(m_name, type, instanceID());

			//����
			CloseHandle(m_fileHandle);
			//������
			m_fileHandle = nullptr;

			AtomicOperation::Init(m_isOpened, false);
			AtomicOperation::Init(m_isOpeneFailed, false);
		}
	}
}