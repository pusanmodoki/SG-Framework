#include "Application.hpp"

//Framework namespace
namespace SGFramework
{
	void Application::ImportSetting()
	{
		using namespace ApplicationSetting;

		try
		{
			m_applicationSetting.ReadSetting();
		}
		catch (...)
		{
			throw;
		}
		
		SetProcessDPIAware();
		Vector2Int resolution(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
		Vector2 screenSize = Vector2(m_applicationSetting.getPack(TiredInfo::cScreenValueX)[0].valueFloat, m_applicationSetting.getPack(TiredInfo::cScreenValueY)[0].valueFloat);
		Vector2Int screenSizeInt;
		Vector2Int aspectRadio = Vector2Int(m_applicationSetting.getPack(TiredInfo::cAspectRadioX)[0].valueInt, m_applicationSetting.getPack(TiredInfo::cAspectRadioY)[0].valueInt);
		bool isFullScreen = static_cast<bool>(m_applicationSetting.getPack(TiredInfo::cIsFullScreen)[0].valueInt & 1);

		if (m_applicationSetting.getPack(TiredInfo::cIsFullScreen)[0].valueInt)
			screenSize = resolution.ToFloat();
		else if (MathAF::Absolute(screenSize.x) < 1.0f + MathAF::cEpsilon && MathAF::Absolute(screenSize.y) < 1.0f + MathAF::cEpsilon)
		{
			screenSize *= resolution.ToFloat();

			if (MathAF::AspectRatio(Vector2Int(screenSize)) != aspectRadio)
				screenSize = MathAF::AspectRatioSettingY(screenSizeInt.y, aspectRadio);
		}

		screenSizeInt = screenSize;
		
		Vector2Int center = screenSizeInt / 2;
		Graphics::screen.setInfo(resolution, screenSizeInt, Const::Vector2Int::zero, center, Const::Vector2Int::zero, isFullScreen);		
		
		m_productName = m_applicationSetting.getPack(TiredInfo::cProductName)[0].valueString;
		m_version = m_applicationSetting.getPack(TiredInfo::cVersion)[0].valueString;
		m_quitMessage = m_applicationSetting.getPack(TiredInfo::cQuitMessage)[0].valueString;
		m_quitHeader = m_applicationSetting.getPack(TiredInfo::cQuitHeader)[0].valueString;

		m_isEnableAltF4 = static_cast<bool>(m_applicationSetting.getPack(TiredInfo::cIsEnableAltF4)[0].valueInt);
	}
}