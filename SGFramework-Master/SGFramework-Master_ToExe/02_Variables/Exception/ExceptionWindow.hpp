/*----------------------------------------------------------------------------------
<ExceptionWindow.hpp> 例外ウィンドウを表示させるExceptionWindow class
制作者: Shota Uemura
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_EXCEPTION_WINDOW_HPP_
#define SGFRAMEWORK_HEADER_EXCEPTION_WINDOW_HPP_
#include <string>
#include <mutex>
#include <unordered_map>

//Windows Version
#if defined(SGF_PLATFORM_WINDOWS)
#include <Windows.h>
//else
#else
#error "Unsupported platform."
#endif//endif

//Framework namespace
namespace SGFramework
{
	//Exception namespace
	namespace Exception
	{
		//例外ウィンドウを表示させるExceptionWindow class
		class ExceptionWindow
		{
		public:
			//friend
			friend int WINAPI::WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow);

			//----------------------------------------------------------------------------------
			//[ShowWindow]
			//Windowを表示させる
			//argument 1: Window header
			//argument 2: Window text
			static void ShowWindow(const std::string& header, const std::string& text);

		private:
			//----------------------------------------------------------------------------------
			//[Shutdown]
			//終了処理を行う
			static void Shutdown();

			//Windows Version
#if defined(SGF_PLATFORM_WINDOWS)
			//----------------------------------------------------------------------------------
			//[StartupWin64]
			//初期化処理を行う->Windows x64
			//argument 1: Window handle
			//argument 2: Instance handle
			static void StartupWin64(HWND hWnd, HINSTANCE hInstance);
			//----------------------------------------------------------------------------------
			//[WindowProcedure]
			//Exception windowのWindowProcedure
			static LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

			//Windowクラス名
			static constexpr wchar_t m_cWindowClassName[] = L"SGFramework-ExceptionWindowClass";
			//MaxPath / 2 + strlen(source: )
			static constexpr long m_cMaxOneLineWordCount = MAX_PATH / 2 + 7;
			static constexpr long m_cBaseWindowHeightFontMulti = 11;
			static constexpr long m_cBacktraceLineHeight = 13;

			//ウィンドウテキスト保存用
			static std::unordered_map<HWND, std::wstring> m_windowTexts;
			//ロック用
			static std::mutex m_mutex;
			//HINSTANCE保存
			static HINSTANCE m_hinstance;
			//HWND保存
			static HWND m_parentWindowHandle;
			//ウィンドウ生成カウンタ
			static int m_childrenWindowCounter;
#else
#error "Unsupported platform."
#endif//Windows Version
		};
	}
}

#endif // !SGFRAMEWORK_HEADER_EXCEPTION_WINDOW_HPP_