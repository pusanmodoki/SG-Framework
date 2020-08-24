#include "Application.hpp"

using namespace SGFramework;

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{	
	if (IsDebug::isValue())
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	setlocale(LC_ALL, "");
	std::locale::global(std::locale(""));
	GUI::RegisterDebugStream(L"WinMain", std::this_thread::get_id());

	try
	{
		Application::StartUp(hInstance, nCmdShow);
	}
	catch (...)
	{
		Application::ShutDown();
		return 0;
	}

	Thread::setAffinityWinMain();
	Random::InitMainThread();

	while (true)
	{
		if (GetMessage(&Application::m_message, nullptr, 0, 0) > 0)
		{
			TranslateMessage(&Application::m_message);				// 文字メッセージへのコンバート
			DispatchMessage(&Application::m_message);				// メッセージをWindowProcedureへ送る
		}
		else
			break;
	}

	//PeekMessage
	PeekMessage(&Application::m_message, nullptr, 0, 0, PM_REMOVE);

	Application::ShutDown();

	//return
	return static_cast<int>(Application::m_message.wParam);
}
