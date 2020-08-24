#include "Application.hpp"

//Framework namespace
namespace SGFramework
{
	LRESULT CALLBACK Application::WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		Input::ReceiveInputMessageWithWindowProcedure(message, wParam, lParam);

		//���b�Z�[�W�X�C�b�`
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

		//������
		case WM_CHAR:
		{
			GUI::m_inputCharacters.emplace_back(static_cast<uint>(wParam));
			break;
		}

		//�E�B���h�E�ύX
		case WM_EXITSIZEMOVE:
		case WM_ACTIVATE:
		{
			//�N���b�v���
			if (IS_TRUE(Input::m_isMouseInsideFrame))
			{
				RECT rect;
				DwmGetWindowAttribute(hWnd, DWMWA_EXTENDED_FRAME_BOUNDS, &rect, sizeof(rect));
				ClipCursor(&rect);
			}
			break;
		}

		//�R�}���h
		case WM_SYSCOMMAND:
		{
			//AltF4�����Ȃ�DefWindowProc��WM_CLOSE�𑗐M���Ȃ�
			if (wParam == SC_CLOSE && IS_FALSE(m_isEnableAltF4))
				return 0;
			break;
		}

		//�f�X�g���C���b�Z�[�W
		case WM_DESTROY:
		{
			Input::ShutdownWindowProcedure();
			GUI::ShutdownWindowProcedure();
			AtomicOperation::Init(m_isEndUpdate, true);
			PostQuitMessage(0);			//�E�B���h�E�폜
			break;
		}

		//����
		case WM_CLOSE:
		{
			//����
			if (wParam != m_cNotMessage && MessageBox(hWnd, m_quitMessage, m_quitHeader, MB_YESNO | MB_ICONQUESTION) == IDYES)
				DestroyWindow(hWnd);	//�A�v���I��
			else if (wParam == m_cNotMessage)
				DestroyWindow(hWnd);	//�A�v���I��
			else
				message -= WM_CLOSE;
			break;
		}

		//�f�o�C�X�ύX
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
			//FrameworkMessage ���������炵��������switch�𕪂���
			if (message >= WM_SGFRAMEWORK_BEGIN)
			{
				switch (message)
				{
				//��O���b�Z�[�W->�I��
				case WM_SGFRAMEWORK_INVALID_MESSAGE:
				{
					AtomicOperation::LockGuard guard(m_lock);
					for (auto& e : m_invalids)
						InvalidMessageDirect(e);
					DestroyWindow(hWnd);	//�A�v���I��
					return 0;
				}

				//WindowName�ݒ�
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

		//���b�Z�[�W���M
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}