/*----------------------------------------------------------------------------------
<ExceptionWindow.cpp> 例外ウィンドウを表示させるExceptionWindow class
制作者: Shota Uemura
------------------------------------------------------------------------------------*/
//Windows version
#ifdef SGF_PLATFORM_WINDOWS
#include <Windows.h>
#include <dwmapi.h>
#include "../../01_MacroAndLibrarys/MacroAndLibrarys.hpp"
#include "ExceptionWindow.hpp"
#include "../Stringf/Stringf.hpp"

//ウィンドウテキスト保存用
std::unordered_map<HWND, std::wstring> SGFramework::Exception::ExceptionWindow::m_windowTexts;
//ロック用
std::mutex SGFramework::Exception::ExceptionWindow::m_mutex;
//HINSTANCE保存
HINSTANCE SGFramework::Exception::ExceptionWindow::m_hinstance = nullptr;
//HWND保存
HWND SGFramework::Exception::ExceptionWindow::m_parentWindowHandle = nullptr;
//ウィンドウ生成カウンタ
int SGFramework::Exception::ExceptionWindow::m_childrenWindowCounter = 0;

//----------------------------------------------------------------------------------
//[ShowWindow]
//Windowを表示させる
//argument 1: Window header
//argument 2: Window text
void SGFramework::Exception::ExceptionWindow::ShowWindow(const std::string & header, const std::string & text)
{
	std::wstring useText = stringf::convert(text);

	//ウインドウ作成
	HWND childWindowHandle = CreateWindowExW(
		WS_EX_LEFT,
		m_cWindowClassName,
		stringf::convert(header).c_str(),
		(m_parentWindowHandle != nullptr ? WS_CHILD | WS_CAPTION | WS_BORDER | WS_SYSMENU : WS_POPUPWINDOW | WS_CAPTION),
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		1000,
		1000,
		m_parentWindowHandle,
		m_parentWindowHandle == nullptr ? nullptr : CREATE_WINDOW_HMENU_VALUE_CHILDLEN_EXCEPTION(++m_childrenWindowCounter),
		m_hinstance,
		reinterpret_cast<void*> (&useText)
	);

	//ウインドウテキストを保存
	{
		std::lock_guard<std::mutex> guard(m_mutex);
		m_windowTexts.try_emplace(childWindowHandle, useText);
	}

	//ウインドウ表示
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


//----------------------------------------------------------------------------------
//[Shutdown]
//終了処理を行う
void SGFramework::Exception::ExceptionWindow::Shutdown()
{
	//クラス登録解除
	UnregisterClass(m_cWindowClassName, m_hinstance);
}

//----------------------------------------------------------------------------------
//[StartupWin64]
//初期化処理を行う->Windows x64
//argument 1: Window handle
//argument 2: Instance handle
void SGFramework::Exception::ExceptionWindow::StartupWin64(HWND hWnd, HINSTANCE hInstance)
{
	//ウインドウクラス作成
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

	RegisterClassExW(&wndClassEx);
	m_hinstance = hInstance;
	m_parentWindowHandle = hWnd;
}

//----------------------------------------------------------------------------------
//[WindowProcedure]
//Exception windowのWindowProcedure
LRESULT SGFramework::Exception::ExceptionWindow::WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//クライアントサイズ保存用
	static RECT clientSize = {};
	//ボタンサイズ保存用
	static POINT buttonSize = {};
	//フォントサイズ保存用
	static POINT fontSize = {};

	//メッセージスイッチ
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
		fontSize = { static_cast<long>(static_cast<float>(textMatrics.tmAveCharWidth) * 0.9f), textMatrics.tmHeight };
		//ボタンサイズを決定する
		buttonSize = { fontSize.x * 14,  static_cast<long>(static_cast<float>(fontSize.y) * 1.5f) };

		//backtrace行数を調べる
		int backtraceNumLines = 0;
		{
			bool isBackTrace = false;

			//文字列の取得
			std::wstringstream stream = std::wstringstream (*reinterpret_cast<std::wstring*>((reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams)));
			std::wstring getLine;

			//backtraceの行数を計算
			while (std::getline(stream, getLine))
			{
				if ((isBackTrace ^ true) && stringf::is_contains(getLine, L"backtrace:"))
					isBackTrace = true;
				if (isBackTrace)
					backtraceNumLines++;
			}
		}

		//Windowサイズを求める
		int width = fontSize.x * m_cMaxOneLineWordCount, height;
		{
			RECT windowRect, clientRect;
			POINT setSize = {};
			DwmGetWindowAttribute(hWnd, DWMWA_EXTENDED_FRAME_BOUNDS, &windowRect, sizeof(windowRect));
			GetClientRect(hWnd, &clientRect);
			height = fontSize.y * m_cBaseWindowHeightFontMulti + ((windowRect.bottom - windowRect.top) - (clientRect.bottom - clientRect.top))
				+ backtraceNumLines * m_cBacktraceLineHeight;
		}
		MoveWindow(hWnd, desktopRect.right / 2 - width / 2, desktopRect.bottom / 2 - height / 2, width, height, true);
		
		//クライアントサイズを求める(height用)
		GetClientRect(hWnd, &clientSize);

		//ボタン作成
		HWND button = CreateWindow(L"BUTTON", L"OK",
			WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
			width - static_cast<long>(static_cast<float>(buttonSize.x) * 1.35f),
			clientSize.bottom - static_cast<long>(static_cast<float>(buttonSize.y) * 1.5f),
			buttonSize.x, buttonSize.y, hWnd, nullptr, m_hinstance, nullptr);
		
		//フォントを取得
		HFONT font = reinterpret_cast<HFONT>(CreateFont(fontSize.y, fontSize.x,
			0, 0, FW_THIN, false , false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, PROOF_QUALITY, VARIABLE_PITCH | FF_DONTCARE, L"Yu Gothic UI"));
		//フォント変更のメッセージを送信
		SendMessage(button, WM_SETFONT, reinterpret_cast<WPARAM>(font), MAKELPARAM(TRUE, 0));
		SendMessage(hWnd, WM_SETFONT, reinterpret_cast<WPARAM>(font), MAKELPARAM(TRUE, 0));
		//フォント削除
		DeleteObject(font);

		return 0;
	}

		//デストロイメッセージ
	case WM_DESTROY:
	{
		//ウインドウテキスト削除
		{
			std::lock_guard<std::mutex> guard(m_mutex);
			m_windowTexts.erase(hWnd);
		}
		PostQuitMessage(0);			//ウィンドウ削除
		return 0;
	}
		//閉じる
	case WM_CLOSE:
	case WM_COMMAND:
	{
		//メッセージポスト
		PostMessage(hWnd, WM_SGFRAMEWORK_CLOSE_EXCEPTION_WINDOW, 0, 0);
		return 0;
	}
		//Destroyするメッセージ
	case WM_SGFRAMEWORK_CLOSE_EXCEPTION_WINDOW:
	{
		DestroyWindow(hWnd);
		return 0;
	}
	
	//描画
	case WM_PAINT:
	{
		TEXTMETRIC textMetric;
		PAINTSTRUCT paintStruct;
		HDC hdc = BeginPaint(hWnd, &paintStruct);

		//灰色矩形の描画
		auto blush = CreateSolidBrush(RGB(0xF0, 0xF0, 0xF0));
		SelectObject(hdc, GetStockObject(NULL_PEN));
		SelectObject(hdc, blush);
		Rectangle(hdc, 0, clientSize.bottom - buttonSize.y * 2, clientSize.right, clientSize.bottom);
		DeleteObject(blush);
		
		//文字列の取得
		std::wstringstream stream;
		std::wstring getLine;
		{
			std::lock_guard<std::mutex> guard(m_mutex);
			stream = std::wstringstream(m_windowTexts.at(hWnd));
		}

		GetTextMetrics(hdc, &textMetric);
		int textHeight = fontSize.y, textHeightAdd = fontSize.y;
		bool isChangeFont = false;

		{
			//フォントを取得
			HFONT font = reinterpret_cast<HFONT>(CreateFont(fontSize.y, fontSize.x,
				0, 0, FW_THIN, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
				CLIP_DEFAULT_PRECIS, PROOF_QUALITY, VARIABLE_PITCH | FF_DONTCARE, L"Yu Gothic UI"));
			//フォント変更
			SelectObject(hdc, font);
			//フォント削除
			DeleteObject(font);
		}

		while (std::getline(stream, getLine))
		{
			if ((isChangeFont ^ true) && stringf::is_contains(getLine, L"backtrace:"))
			{
				isChangeFont = true;
				textHeight -= textHeightAdd / 2;
				textHeightAdd = static_cast<long>(static_cast<float>(fontSize.y) * 0.8f);
				//フォントを取得
				HFONT font = reinterpret_cast<HFONT>(CreateFont(textHeightAdd,
					static_cast<long>(static_cast<float>(fontSize.x) * 0.8f),
					0, 0, FW_THIN, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
					CLIP_DEFAULT_PRECIS, PROOF_QUALITY, VARIABLE_PITCH | FF_DONTCARE, L"Yu Gothic UI"));
				//フォント変更
				SelectObject(hdc, font);
				//フォント削除
				DeleteObject(font);
			}

			TabbedTextOut(hdc, 0, textHeight, getLine.c_str(), static_cast<int>(getLine.length()), 0, nullptr, 0);
			textHeight += textHeightAdd;
		}

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