/*----------------------------------------------------------------------------------
DirectX11 Camera������CameraX11 class
------------------------------------------------------------------------------------*/
#include "CameraX11.hpp"

// Framework namespace
namespace SGFramework
{
	//�t���[�����[�N�̊Ǘ�������Administrator namespace
	namespace Administrator
	{
		//GraphicsDetail namespace
		namespace GraphicsDetail
		{
			CameraX11* CameraX11::m_setCamera = nullptr;	//�Ō�ɐݒ肵���J����
			LayerMask CameraX11::m_setLayerMask = 0;			//�Ō�ɐݒ肵�����C���[�}�X�N
		}
	}
}