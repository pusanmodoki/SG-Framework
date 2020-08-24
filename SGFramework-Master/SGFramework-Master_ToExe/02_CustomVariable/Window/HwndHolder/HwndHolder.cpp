/*----------------------------------------------------------------------------------
hWnd���Ǘ�����HwndHolder class
------------------------------------------------------------------------------------*/
#include "HwndHolder.hpp"

//Framework namespace
namespace SGFramework
{
	//Window�Ǘ�
	namespace Window
	{
		//�N���E�B���h�E�̃n���h��
		PointerProperty<HWND> HwndHolder::main(m_main);

		//hWnds[0]
		HWND HwndHolder::m_main = nullptr;
		//hWnd���X�g
		std::vector<HWND> HwndHolder::m_hwnds;
		//mutex
		std::mutex HwndHolder::m_mutex;
	}
}