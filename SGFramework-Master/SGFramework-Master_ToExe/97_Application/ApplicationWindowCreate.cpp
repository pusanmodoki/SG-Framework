#include "Application.hpp"

//Framework namespace
namespace SGFramework
{
	WNDCLASSEX Application::WindowCreate(HINSTANCE hInstance, int nCmdShow)
	{
		std::wstring name = (m_productName + Time::CurrentTimeToString());
		//ウィンドウクラス作成
		WNDCLASSEX wCex =
		{
			sizeof(WNDCLASSEX),
			CS_CLASSDC,
			WindowProcedure,
			0,
			0,
			hInstance,
			NULL,
			LoadCursor(NULL,IDC_ARROW),
			(HBRUSH)GetStockObject(BLACK_BRUSH),
			NULL,
			name.c_str(),
			NULL
		};

		//ウィンドウ作成
		RegisterClassEx(&wCex);
		HWND hWnd = CreateWindowEx(
			0,
			name.c_str(),
			m_productName.data(),
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			1000,
			500,
			NULL,
			NULL,
			hInstance,
			NULL);

		//代入
		Window::HwndHolder::AddHwnd(hWnd);
		Window::HinstanceHolder::AddHinstance(hInstance);

		//Windowサイズ, Position再設定
		{
			RECT windowRect, clientRect;
			POINT setSize = {};
			DwmGetWindowAttribute(Window::HwndHolder::main, DWMWA_EXTENDED_FRAME_BOUNDS, &windowRect, sizeof(windowRect));
			GetClientRect(hWnd, &clientRect);
			setSize.x = Graphics::screen.width + ((windowRect.right - windowRect.left) - (clientRect.right - clientRect.left));
			setSize.y = Graphics::screen.height + ((windowRect.bottom - windowRect.top) - (clientRect.bottom - clientRect.top));
			SetWindowPos(hWnd, NULL, static_cast<int>(Graphics::screen.resolution->x * 0.5f - static_cast<float>(setSize.x) * 0.5f),
				static_cast<int>(Graphics::screen.resolution->y * 0.5f - static_cast<float>(setSize.y) * 0.5f),
				setSize.x, setSize.y, (SWP_NOZORDER | SWP_NOOWNERZORDER));
		}

		//Screen情報設定
		{
			RECT windowRect;
			GetWindowRect(hWnd, &windowRect);

			SetProcessDPIAware();
			HDC hdc = GetWindowDC(hWnd);
			Graphics::screen.setInfo(Graphics::screen.resolution, Graphics::screen.screenSize, Vector2Int(windowRect.right, windowRect.bottom),
				Graphics::screen.screenSize() / 2, Vector2Int(GetDeviceCaps(hdc, LOGPIXELSX), GetDeviceCaps(hdc, LOGPIXELSY)), Graphics::screen.isFullScreen);
			ReleaseDC(hWnd, hdc);
		}

		ShowWindow(hWnd, nCmdShow);
		InvalidateRect(hWnd, NULL, false);
		UpdateWindow(hWnd);

		return wCex;
	}
}