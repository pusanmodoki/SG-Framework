#ifndef SGFRAMEWORK_HEADER_EXCEPTION_WINDOW_HPP_
#define SGFRAMEWORK_HEADER_EXCEPTION_WINDOW_HPP_
#include <string>

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
		class ExceptionWindow
		{
		public:
			//friend
			friend int WINAPI::WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow);
			static void ShowWindow(const std::string& header, const std::string& text);
		private:
			static void Shutdown();
			//Windows Version
#if defined(SGF_PLATFORM_WINDOWS)
			static void StartupWin64(HWND hWnd, HINSTANCE hInstance);
			static LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

			static constexpr wchar_t m_cWindowClassName[] = L"SGFramework-ExceptionWindowClass";
			//MaxPath / 2 + strlen(source: )
			static constexpr long m_cMaxOneLineWordCount = MAX_PATH / 2 + 7;
			static constexpr long m_cMaxLines = 15;

			static HINSTANCE m_hinstance;
			static HWND m_parentWindowHandle;
			static int m_childrenWindowCounter;
#else
#error "Unsupported platform."
#endif//Windows Version
		};
	}
}

#endif // !SGFRAMEWORK_HEADER_EXCEPTION_WINDOW_HPP_