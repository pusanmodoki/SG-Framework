#include "Screen.hpp"
#include "../DirectX11/GraphicsX11/GraphicsX11.hpp"

// Framework namespace
namespace SGFramework
{
	namespace BaseClass
	{
		GetOnlyProperty<Vector4x4> BaseScreen<Administrator::GraphicsX11>::clipToScreenMatrix(m_clipToScreenMatrix);
		GetOnlyProperty<Vector2Int> BaseScreen<Administrator::GraphicsX11>::resolution(m_resolution);
		GetOnlyProperty<Vector2Int> BaseScreen<Administrator::GraphicsX11>::screenSize(m_screenSize);
		GetOnlyProperty<Vector2> BaseScreen<Administrator::GraphicsX11>::screenSizeFloat(m_screenSizeFloat);
		GetOnlyProperty<Vector2Int> BaseScreen<Administrator::GraphicsX11>::windowSize(m_windowSize);
		GetOnlyProperty<Vector2Int> BaseScreen<Administrator::GraphicsX11>::screenCenter(m_screenCenter);
		GetOnlyProperty<Vector2Int> BaseScreen<Administrator::GraphicsX11>::dpi(m_dpi);
		GetOnlyProperty<int> BaseScreen<Administrator::GraphicsX11>::width(m_screenSize.x);
		GetOnlyProperty<int> BaseScreen<Administrator::GraphicsX11>::height(m_screenSize.y);
		GetOnlyProperty<bool> BaseScreen<Administrator::GraphicsX11>::isFullScreen(m_isFullScreen);

		Vector4x4 BaseScreen<Administrator::GraphicsX11>::m_clipToScreenMatrix(Const::Vector4x4::zero);
		Vector2Int BaseScreen<Administrator::GraphicsX11>::m_resolution(0, 0);
		Vector2Int BaseScreen<Administrator::GraphicsX11>::m_screenSize(0, 0);
		Vector2 BaseScreen<Administrator::GraphicsX11>::m_screenSizeFloat(0.0f, 0.0f);
		Vector2Int BaseScreen<Administrator::GraphicsX11>::m_windowSize(0, 0);
		Vector2Int BaseScreen<Administrator::GraphicsX11>::m_screenCenter(0, 0);
		Vector2Int BaseScreen<Administrator::GraphicsX11>::m_dpi(0, 0);
		bool BaseScreen<Administrator::GraphicsX11>::m_isFullScreen = false;

		//template <typename UseGraphics>
		//void BaseScreen<UseGraphics>::setResolution(const int width, const int height, const bool isFullScreen, const int refreshRate)
		//{
		//	UseGraphics::setResolution(width, height, isFullScreen, refreshRate);
		//}
		//template <typename UseGraphics>
		//void BaseScreen<UseGraphics>::setFullScreen(const bool isFullScreen)
		//{
		//	UseGraphics::setFullScreen(isFullScreen);
		//}
	}
}