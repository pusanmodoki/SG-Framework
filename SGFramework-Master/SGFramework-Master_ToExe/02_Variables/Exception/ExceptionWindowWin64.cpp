#include "ExceptionWindow.hpp"
#ifdef SGF_PLATFORM_WINDOWS
#include <Windows.h>
#include "../MacroAndUsing/MacroAndUsing.hpp"

HINSTANCE SGFramework::Exception::ExceptionWindow::m_hinstance = nullptr;
HWND SGFramework::Exception::ExceptionWindow::m_parentWindowHandle = nullptr;
int SGFramework::Exception::ExceptionWindow::m_childrenWindowCounter = 0;

void SGFramework::Exception::ExceptionWindow::ShowWindow(const std::string & header, const std::string & text)
{
	HWND childWindowHandle = CreateWindowEx(
		WS_EX_LEFT,
		m_cWindowClassName,
		L"SGFramework error!!",
		(m_parentWindowHandle != nullptr ? WS_CHILD | WS_CAPTION | WS_BORDER | WS_SYSMENU : WS_POPUPWINDOW | WS_CAPTION),
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		1000,
		1000,
		m_parentWindowHandle,
		m_parentWindowHandle == nullptr ? nullptr : CREATE_WINDOW_HMENU_VALUE_CHILDLEN_EXCEPTION(++m_childrenWindowCounter),
		m_hinstance,
		nullptr
	);

	::ShowWindow(childWindowHandle, SW_SHOW);
	::UpdateWindow(childWindowHandle);

	MSG message = {};
	//メッセージループ
	while (true)
	{
		if (GetMessage(&message, childWindowHandle, 0, 0) > 0)
		{
			TranslateMessage(&message);				// 文字メッセージへのコンバート
			DispatchMessage(&message);				// メッセージをWindowProcedureへ送る

			//閉じますメッセージなら出る
			if (message.message == WM_SGFRAMEWORK_CLOSE_EXCEPTION_WINDOW)
				break;
		}
		else break;
	}
}


void SGFramework::Exception::ExceptionWindow::Shutdown()
{
	UnregisterClass(m_cWindowClassName, m_hinstance);
}

void SGFramework::Exception::ExceptionWindow::StartupWin64(HWND hWnd, HINSTANCE hInstance)
{
	WNDCLASSEX wndClassEx = {};
	wndClassEx.cbSize = sizeof(wndClassEx);
	wndClassEx.style = 0;
	wndClassEx.lpfnWndProc = WindowProcedure;
	wndClassEx.cbClsExtra = 0;
	wndClassEx.cbWndExtra = 0;
	wndClassEx.hInstance = hInstance;
	wndClassEx.hIcon = reinterpret_cast<HICON>(LoadImage(NULL, IDI_ERROR, IMAGE_ICON, 0, 0, LR_SHARED));
	wndClassEx.hCursor = reinterpret_cast<HCURSOR>(LoadImage(NULL, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED));
	wndClassEx.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	wndClassEx.lpszMenuName = nullptr;
	wndClassEx.lpszClassName = m_cWindowClassName;
	wndClassEx.hIconSm = reinterpret_cast<HICON>(LoadImage(NULL, IDI_ERROR, IMAGE_ICON, 0, 0, LR_SHARED));

	RegisterClassEx(&wndClassEx);
	m_hinstance = hInstance;
	m_parentWindowHandle = hWnd;
}

LRESULT SGFramework::Exception::ExceptionWindow::WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HFONT font = nullptr;
	//クライアントサイズ保存用
	static RECT clientSize = {};
	//ボタンサイズ保存用
	static POINT buttonSize = {};

	switch (message)
	{
	case WM_CREATE:
	{
		//フラッシュさせる
		FLASHWINFO flashInfo;
		flashInfo.cbSize = sizeof(FLASHWINFO);
		flashInfo.dwTimeout = 50;
		flashInfo.uCount = 8;
		flashInfo.hwnd = hWnd;
		flashInfo.dwFlags = FLASHW_CAPTION;
		FlashWindowEx(&flashInfo);
		//警告音を出す
		MessageBeep(MB_ICONERROR);

		//デスクトップサイズ取得
		RECT desktopRect;
		GetWindowRect(GetDesktopWindow(), &desktopRect);

		//システムフォントの情報取得
		HDC hdc;
		TEXTMETRIC textMatrics = {};
		hdc = GetDC(hWnd);
		GetTextMetrics(hdc, &textMatrics);
		ReleaseDC(hWnd, hdc);

		//fontサイズを決定する
		POINT fontSize = { static_cast<long>(static_cast<float>(textMatrics.tmAveCharWidth) * 0.9f), textMatrics.tmHeight };
		//ボタンサイズを決定する
		buttonSize = { fontSize.x * 14,  static_cast<long>(static_cast<float>(fontSize.y) * 1.5f) };

		//Windowサイズを求める
		int width = fontSize.x * m_cMaxOneLineWordCount;
		int height = fontSize.y * m_cMaxLines + buttonSize.y * 2;
		MoveWindow(hWnd, desktopRect.right / 2 - width / 2, desktopRect.bottom / 2 - height / 2, width, height, true);
		
		//クライアントサイズを求める(height用)
		GetClientRect(hWnd, &clientSize);

		MessageBox(hWnd, L"FF", L"FF", MB_OK);

		//ボタン作成
		HWND button = CreateWindow(L"BUTTON", L"OK",
			WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
			width - static_cast<long>(static_cast<float>(buttonSize.x) * 1.35f),
			clientSize.bottom - static_cast<long>(static_cast<float>(buttonSize.y) * 1.5f),
			buttonSize.x, buttonSize.y, hWnd, nullptr, m_hinstance, nullptr);
		
		//フォントを取得
		font = (HFONT)CreateFont(fontSize.y, fontSize.x,
			0, 0, FW_THIN, false , false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, PROOF_QUALITY, VARIABLE_PITCH | FF_DONTCARE, L"Yu Gothic UI");
		//フォント変更のメッセージを送信
		SendMessage(button, WM_SETFONT, (WPARAM)font, MAKELPARAM(TRUE, 0));
		//フォント削除
		DeleteObject(font);

		return 0;
	}

		//デストロイメッセージ
	case WM_DESTROY:
	{
		DeleteObject(font);				//フォント削除
		PostQuitMessage(0);			//ウィンドウ削除
		return 0;
	}
		//閉じる
	case WM_CLOSE:
	case WM_COMMAND:
	{
		PostMessage(hWnd, WM_SGFRAMEWORK_CLOSE_EXCEPTION_WINDOW, 0, 0);
		return 0;
	}
		//Destroyするメッセージ
	case WM_SGFRAMEWORK_CLOSE_EXCEPTION_WINDOW:
	{
		DestroyWindow(hWnd);
		return 0;
	}

	case WM_PAINT:
	{
		PAINTSTRUCT paintStruct;
		HDC hdc = BeginPaint(hWnd, &paintStruct);

		//灰色矩形の描画
		auto blush = CreateSolidBrush(RGB(0xF0, 0xF0, 0xF0));
		SelectObject(hdc, GetStockObject(NULL_PEN));
		SelectObject(hdc, blush);
		Rectangle(hdc, 0, clientSize.bottom - buttonSize.y * 2, clientSize.right, clientSize.bottom);
		DeleteObject(blush);
		
		//文字列の描画



		EndPaint(hWnd, &paintStruct);
		return 0;
	}
	default:
			break;
	}
	

	//メッセージ送信
	return DefWindowProc(hWnd, message, wParam, lParam);
}


#endif // SGF_PLATFORM_WINDOWS