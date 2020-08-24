/*----------------------------------------------------------------------------------
Base Light Class
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_BASE_LIGHT_X11_HPP_
#define SGFRAMEWORK_HEADER_BASE_LIGHT_X11_HPP_
#include "../../../02_CustomVariable/CustomVariable.hpp"

// Framework namespace
namespace SGFramework
{
	//Base Classes
	namespace BaseClass
	{
		//Base Light Class
		class BaseLightX11
		{
		protected:
			//is changed access
			static constexpr uint m_cIsChangedDirection = 0;
			static constexpr uint m_cIsChangedPoint = 1;
			static constexpr uint m_cIsChangedHemiSphere = 2;
			//is changed
			static bool m_isChangeds[3];
		};
	}
}
#endif // !SGFRAMEWORK_HEADER_BASE_LIGHT_X11_HPP_