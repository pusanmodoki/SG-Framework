#include <Windows.h>
#include "../SGFramework-Master_ToExe/02_Variables/Variables.hpp"
#include "../SGFramework-Master_ToExe/02_Variables/Exception/ExceptionWindow.hpp"

void A01()
{
	using namespace SGFramework;
	Color color;

	try { color /= 0; }
	catch (SGFramework::Exception::Detail::BaseException& e) { e.ShowMessageBox(); }
}
void A02() { A01(); }
void A03() { A02(); }
void A04() { A03(); }
void A05() { A04(); }
void A06() { A05(); }
void A07() { A06(); }
void A08() { A07(); }
void A09() { A08(); }
void A10(){ A09(); }

void F(int a)
{
	if (a == 10)
	{
	}
	else
	{
		F(a + 1);
	}
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	//ウインドウクラス作成
	WNDCLASSEX wndClassEx = {};
	wndClassEx.cbSize = sizeof(wndClassEx);
	wndClassEx.style = 0;
	wndClassEx.lpfnWndProc = DefWindowProc;
	wndClassEx.cbClsExtra = 0;
	wndClassEx.cbWndExtra = 0;
	wndClassEx.hInstance = hInstance;
	wndClassEx.hIcon = reinterpret_cast<HICON>(LoadImage(NULL, IDI_ERROR, IMAGE_ICON, 0, 0, LR_SHARED));
	wndClassEx.hCursor = reinterpret_cast<HCURSOR>(LoadImage(NULL, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED));
	wndClassEx.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	wndClassEx.lpszMenuName = nullptr;
	wndClassEx.lpszClassName = L"TEST";
	wndClassEx.hIconSm = reinterpret_cast<HICON>(LoadImage(NULL, IDI_ERROR, IMAGE_ICON, 0, 0, LR_SHARED));

	RegisterClassExW(&wndClassEx);

	//ウインドウ作成
	HWND hwnd = CreateWindowExW(
		WS_EX_LEFT,
		L"TEST",
		L"TEST",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		1000,
		1000,
		nullptr,
		nullptr,
		hInstance,
		nullptr
	);
	//ウインドウ表示
	::ShowWindow(hwnd, SW_SHOW);
	::UpdateWindow(hwnd);

	using namespace SGFramework;
	Exception::ExceptionWindow::StartupWin64(NULL, hInstance);
	Exception::Detail::BaseException::FirstStartupWin64();
	A08();
	//auto ex = Exception::InvalidArgumentException("stringf", "to_radix", "radix is invalid value.");
	//ex.ShowMessageBox();
//	Exception::ExceptionWindow::ShowWindow("FOO!!", "YEEEE!!");

	Exception::Detail::BaseException::Shutdown();

	return 0;
}