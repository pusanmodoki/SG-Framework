/*----------------------------------------------------------------------------------
DirectX11 Lightを纏めるLightX11.cpp
------------------------------------------------------------------------------------*/
#include "LightX11.hpp"

// Framework namespace
namespace SGFramework
{
	//フレームワークの管理をするAdministrator namespace
	namespace Administrator
	{
		//GraphicsDetail namespace
		namespace GraphicsDetail
		{
			//value pack
			DirectionalLightX11::Pack DirectionalLightX11::m_pack = {};
			//value pack
			PointLightX11::Pack PointLightX11::m_pack = {};
			//value pack
			HemiSphereLightX11::Pack HemiSphereLightX11::m_pack = {};
		}
	}
	//Base Classes
	namespace BaseClass
	{
		//is changed
		bool BaseLightX11::m_isChangeds[3] = {};
	}
}