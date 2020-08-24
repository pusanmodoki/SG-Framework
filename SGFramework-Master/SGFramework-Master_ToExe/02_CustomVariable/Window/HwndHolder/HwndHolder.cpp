/*----------------------------------------------------------------------------------
hWndを管理するHwndHolder class
------------------------------------------------------------------------------------*/
#include "HwndHolder.hpp"

//Framework namespace
namespace SGFramework
{
	//Window管理
	namespace Window
	{
		//起動ウィンドウのハンドル
		PointerProperty<HWND> HwndHolder::main(m_main);

		//hWnds[0]
		HWND HwndHolder::m_main = nullptr;
		//hWndリスト
		std::vector<HWND> HwndHolder::m_hwnds;
		//mutex
		std::mutex HwndHolder::m_mutex;
	}
}