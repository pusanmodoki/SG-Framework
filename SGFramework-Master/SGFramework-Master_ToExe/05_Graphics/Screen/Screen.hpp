#ifndef SGFRAMEWORK_HEADER_SCREEN_HPP_
#define SGFRAMEWORK_HEADER_SCREEN_HPP_
#include "../../02_CustomVariable/CustomVariable.hpp"
#include "../../03_MathAF/MathAF.hpp"

// Framework namespace
namespace SGFramework
{
	class Application;
	class Input;
	namespace Administrator 
	{
		class GraphicsX11; 
		class GraphicsGL;
	}
	namespace BaseClass
	{
		template <typename UseGraphics>
		class BaseScreen
		{
		public:
			friend class Application;
			friend class Input;
			friend class Administrator::GraphicsX11;
			friend class Administrator::GraphicsGL;

			//SGF_NOTINLINE_FUNCTION_PROPERTY static void setResolution(const int width, const int height, const bool isFullScreen, const int refreshRate = 0);
			//SGF_NOTINLINE_FUNCTION_PROPERTY static void setFullScreen(const bool isFullScreen);

			static GetOnlyProperty<Vector4x4> clipToScreenMatrix;
			static GetOnlyProperty<Vector2Int> resolution;
			static GetOnlyProperty<Vector2Int> screenSize;
			static GetOnlyProperty<Vector2> screenSizeFloat;
			static GetOnlyProperty<Vector2Int> windowSize;
			static GetOnlyProperty<Vector2Int> screenCenter;
			static GetOnlyProperty<Vector2Int> dpi;
			static GetOnlyProperty<int> width;
			static GetOnlyProperty<int> height;
			static GetOnlyProperty<bool> isFullScreen;

		private:
			inline static void setInfo(const Vector2Int& resolution, const Vector2Int& screenSize, const Vector2Int& windowSize,
				const Vector2Int& screenCenter, const Vector2Int& dpi, const bool isFullScreen);
			inline static void ChangeWindowSize(const int x, const int y);

			static Vector4x4 m_clipToScreenMatrix;
			static Vector2Int m_resolution;
			static Vector2Int m_screenSize;
			static Vector2 m_screenSizeFloat;
			static Vector2Int m_windowSize;
			static Vector2Int m_screenCenter;
			static Vector2Int m_dpi;
			static bool m_isFullScreen;
		};

		template <typename UseGraphics>
		inline void BaseScreen<UseGraphics>::setInfo(const Vector2Int& resolution, const Vector2Int& screenSize, const Vector2Int& windowSize,
			const Vector2Int& screenCenter, const Vector2Int& dpi, const bool isFullScreen)
		{
			m_resolution = resolution;
			m_screenSize = screenSize;
			m_screenSizeFloat = screenSize.ToFloat();
			m_windowSize = windowSize;
			m_screenCenter = screenCenter;
			m_dpi = dpi;
			m_isFullScreen = isFullScreen;

			m_clipToScreenMatrix._11 = m_screenSizeFloat.x * 0.5f;
			m_clipToScreenMatrix._22 = -m_screenSizeFloat.y * 0.5f;
			m_clipToScreenMatrix._33 = 1.0f;
			m_clipToScreenMatrix._41 = m_screenSizeFloat.x * 0.5f;
			m_clipToScreenMatrix._42 = m_screenSizeFloat.y * 0.5f;
			m_clipToScreenMatrix._44 = 1.0f;
		}
		template <typename UseGraphics>
		inline void BaseScreen<UseGraphics>::ChangeWindowSize(const int x, const int y)
		{
			m_windowSize.x = x;
			m_windowSize.y = y;
		}
	}
}

#endif // !SGFRAMEWORK_HEADER_SCREEN_HPP_
