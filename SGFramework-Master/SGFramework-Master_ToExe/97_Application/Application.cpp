#include "Application.hpp"

namespace SGFramework
{
	GetOnlyProperty<ApplicationSetting::ApplicationSettingValues> Application::applicationSetting(m_applicationSetting);
	GetOnlyProperty<sgstring> Application::windowName(m_windowName);
	GetOnlyProperty<sgstring> Application::version(m_version);
	GetOnlyProperty<sgstring> Application::productName(m_productName);

	ApplicationSetting::ApplicationSettingValues Application::m_applicationSetting;
	std::thread Application::m_mainThread;
	sgstring Application::m_setWindowName;
	std::vector<InvalidArgument> Application::m_invalids;
	sgstring Application::m_windowName;
	sgstring Application::m_version;
	sgstring Application::m_productName;
	sgstring Application::m_quitHeader;
	sgstring Application::m_quitMessage;
	WNDCLASSEX Application::m_wCex = {};
	MSG Application::m_message = {};
	std::atomic_bool Application::m_lock = false;
	std::atomic_bool Application::m_isEndUpdate = false;
	std::atomic_bool Application::m_isUpdateReady = true;
	bool Application::m_isRemote = false;
	bool Application::m_isEnableAltF4 = false;
}