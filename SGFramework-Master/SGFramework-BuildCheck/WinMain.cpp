#include <Windows.h>
#include "../SGFramework-Master_ToExe/02_Variables/Variables.hpp"
#include "../SGFramework-Master_ToExe/02_Variables/Exception/ExceptionWindow.hpp"

#pragma comment (lib, "dbghelp.lib")

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	using namespace SGFramework;
	Exception::Detail::BaseException::FirstStartupWin64();
	Exception::ExceptionWindow::StartupWin64(NULL, hInstance);

	//auto ex = Exception::InvalidArgumentException("stringf", "to_radix", "radix is invalid value.");
	//ex.ShowMessageBox();

	Exception::ExceptionWindow::ShowWindow("FOO!!", "YEEEE!!");

	Exception::Detail::BaseException::Shutdown();

	return 0;
}