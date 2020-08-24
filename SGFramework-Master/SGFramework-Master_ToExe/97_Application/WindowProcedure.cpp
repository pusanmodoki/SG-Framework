#include "Application.hpp"

//Framework namespace
namespace SGFramework
{
	LRESULT CALLBACK Application::WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		Input::ReceiveInputMessageWithWindowProcedure(message, wParam, lParam);

		//メッセージスイッチ
		switch (message)
		{
		//Input
		case WM_SGFRAMEWORK_UPDATE_INPUT:
		{
			Input::UpdateWindowProcedure();
			return 0;
		}
		
		//GUI
		case WM_SGFRAMEWORK_UPDATE_GUI:
		{
			GUI::UpdateWindowProcedure();
			return 0;
		}

		//文字列
		case WM_CHAR:
		{
			GUI::m_inputCharacters.emplace_back(static_cast<uint>(wParam));
			break;
		}

		//ウィンドウ変更
		case WM_EXITSIZEMOVE:
		case WM_ACTIVATE:
		{
			//クリップ状態
			if (IS_TRUE(Input::m_isMouseInsideFrame))
			{
				RECT rect;
				DwmGetWindowAttribute(hWnd, DWMWA_EXTENDED_FRAME_BOUNDS, &rect, sizeof(rect));
				ClipCursor(&rect);
			}
			break;
		}

		//コマンド
		case WM_SYSCOMMAND:
		{
			//AltF4無効ならDefWindowProcでWM_CLOSEを送信しない
			if (wParam == SC_CLOSE && IS_FALSE(m_isEnableAltF4))
				return 0;
			break;
		}

		//デストロイメッセージ
		case WM_DESTROY:
		{
			Input::ShutdownWindowProcedure();
			GUI::ShutdownWindowProcedure();
			AtomicOperation::Init(m_isEndUpdate, true);
			PostQuitMessage(0);			//ウィンドウ削除
			break;
		}

		//閉じる
		case WM_CLOSE:
		{
			//聞く
			if (wParam != m_cNotMessage && MessageBox(hWnd, m_quitMessage, m_quitHeader, MB_YESNO | MB_ICONQUESTION) == IDYES)
				DestroyWindow(hWnd);	//アプリ終了
			else if (wParam == m_cNotMessage)
				DestroyWindow(hWnd);	//アプリ終了
			else
				message -= WM_CLOSE;
			break;
		}

		//デバイス変更
		case WM_DEVICECHANGE:
		{
			//DBT_DEVNODES_CHANGED
			if (wParam == 0x0007)
				Input::ReConnectJoyStick();
			break;
		}

		//default
		default:
		{
			//FrameworkMessage 処理を減らしたいからswitchを分ける
			if (message >= WM_SGFRAMEWORK_BEGIN)
			{
				switch (message)
				{
				//例外メッセージ->終了
				case WM_SGFRAMEWORK_INVALID_MESSAGE:
				{
					AtomicOperation::LockGuard guard(m_lock);
					for (auto& e : m_invalids)
						InvalidMessageDirect(e);
					DestroyWindow(hWnd);	//アプリ終了
					return 0;
				}

				//WindowName設定
				case WM_SGFRAMEWORK_SET_WINDOW_NAME:
				{
					AtomicOperation::LockGuard guard(m_lock);
					SetWindowText(Window::HwndHolder::main, m_setWindowName.c_str());
					return 0;
				}
				default:
					break;
				}
			}
			break;
		}
		}

		//メッセージ送信
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}