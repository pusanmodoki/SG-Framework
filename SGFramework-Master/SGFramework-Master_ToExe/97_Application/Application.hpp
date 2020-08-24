#ifndef SGFRAMEWORK_HEADER_APPLICATION_HPP_
#define SGFRAMEWORK_HEADER_APPLICATION_HPP_
#include <memory>
#include <Windows.h>
#include "../01_LibraryImport/LibraryImport.hpp"
#include "Setting/ApplicationSetting.hpp"

//Framework namespace
namespace SGFramework
{
	//フレームワークの管理をするAdministrator namespace
	namespace Administrator { class ApplicationFunctionCaller; }

	class Application final
	{
	public:
		friend class Time;
		friend class Administrator::ApplicationFunctionCaller;
		friend class Thread;
		friend class GUI;
		friend int WINAPI ::WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow);
		
		DELETE_ALL_DEFAULT_CLASS_FUNCTIONS(Application);

		inline static void Quit(const bool isQuitMessage = false, const sgstring& quitHeader = L"確認", const sgstring& quitMassage = L"終了しますか？");

		SGF_FUNCTION_PROPERTY static void setWindowName(const sgstring& name);
		SGF_FUNCTION_PROPERTY static void setWindowName(const sgstring& name, const bool isJoinProduct);
		
		inline static void InvalidMessage(const InvalidArgument& invalid);
		inline static void InvalidMessage(const sgstring& errorHeader, const sgstring& errorText, const long errorCode = 0);

		static GetOnlyProperty<ApplicationSetting::ApplicationSettingValues> applicationSetting;
		static GetOnlyProperty<sgstring> windowName;
		static GetOnlyProperty<sgstring> version;
		static GetOnlyProperty<sgstring> productName;

	private:
		static constexpr WPARAM m_cNotMessage = 0X1234;
	
		inline static void InvalidMessageDirect(const InvalidArgument& invalid);

		static void StartUp(HINSTANCE hInstance, int nCmdShow);
		static void ShutDown();
		
		static void ImportSetting();
		static WNDCLASSEX WindowCreate(HINSTANCE hInstance, int nCmdShow);
		
		static int ApplicationMain();
		static LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);		
	
		static ApplicationSetting::ApplicationSettingValues m_applicationSetting;
		static std::thread m_mainThread;
		static sgstring m_setWindowName;
		static std::vector<InvalidArgument> m_invalids;
		static sgstring m_windowName;
		static sgstring m_version;
		static sgstring m_productName;
		static sgstring m_quitHeader;
		static sgstring m_quitMessage;
		static WNDCLASSEX m_wCex;
		static MSG m_message;
		static std::atomic_bool m_lock;
		static std::atomic_bool m_isEndUpdate;
		static std::atomic_bool m_isUpdateReady;
		static bool m_isRemote;
		static bool m_isEnableAltF4;
	};

	
	inline void Application::Quit(const bool isQuitMessage, const sgstring& quitHeader, const sgstring& quitMassage)
	{
		if (isQuitMessage)
		{
			m_quitHeader = quitHeader;
			m_quitMessage = quitMassage;
			Window::HwndHolder::main.getValue();
			PostMessage(Window::HwndHolder::main.getValue(), WM_CLOSE, 0, 0);
		}
		else 	PostMessage(Window::HwndHolder::main, WM_CLOSE, m_cNotMessage, 0);
		
	}


	SGF_FUNCTION_PROPERTY void Application::setWindowName(const sgstring& name)
	{
		AtomicOperation::LockGuard guard(m_lock);
		m_setWindowName = name;
		PostMessage(Window::HwndHolder::main, WM_SGFRAMEWORK_SET_WINDOW_NAME, 0, 0);
	}

	SGF_FUNCTION_PROPERTY void Application::setWindowName(const sgstring& name, const bool isJoinProduct)
	{
		AtomicOperation::LockGuard guard(m_lock);
		if (IS_FALSE(isJoinProduct))
			m_setWindowName = name;
		else
			m_setWindowName = m_productName + name;

		PostMessage(Window::HwndHolder::main, WM_SGFRAMEWORK_SET_WINDOW_NAME, 0, 0);
	}
	inline void Application::InvalidMessage(const InvalidArgument & invalid)
	{
		AtomicOperation::LockGuard guard(m_lock);
		m_invalids.emplace_back(invalid);

		PostMessage(Window::HwndHolder::main, WM_SGFRAMEWORK_INVALID_MESSAGE, 0, 0);
	}
	inline void Application::InvalidMessage(const sgstring & errorHeader, const sgstring & errorText, const long errorCode)
	{
		AtomicOperation::LockGuard guard(m_lock);
		m_invalids.emplace_back(errorHeader, errorText, errorCode);

		PostMessage(Window::HwndHolder::main, WM_SGFRAMEWORK_INVALID_MESSAGE, 0, 0);
	}


	inline void Application::InvalidMessageDirect(const InvalidArgument & invalid)
	{
		if(invalid.Code())
			MessageBox(Window::HwndHolder::main, (invalid.Text() + L"\nCode->" + sgstring::to_sgstring(invalid.Code())).c_str(), invalid.Header().c_str(), MB_OK);
		else
			MessageBox(Window::HwndHolder::main, (invalid.Text() + sgstring::to_sgstring(invalid.Code())).c_str(), invalid.Header().c_str(), MB_OK);
	}
}
#endif // !SGFRAMEWORK_HEADER_APPLICATION_HPP_