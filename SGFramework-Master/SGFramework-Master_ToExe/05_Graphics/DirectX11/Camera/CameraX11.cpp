/*----------------------------------------------------------------------------------
DirectX11 Cameraを扱うCameraX11 class
------------------------------------------------------------------------------------*/
#include "CameraX11.hpp"

// Framework namespace
namespace SGFramework
{
	//フレームワークの管理をするAdministrator namespace
	namespace Administrator
	{
		//GraphicsDetail namespace
		namespace GraphicsDetail
		{
			CameraX11* CameraX11::m_setCamera = nullptr;	//最後に設定したカメラ
			LayerMask CameraX11::m_setLayerMask = 0;			//最後に設定したレイヤーマスク
		}
	}
}