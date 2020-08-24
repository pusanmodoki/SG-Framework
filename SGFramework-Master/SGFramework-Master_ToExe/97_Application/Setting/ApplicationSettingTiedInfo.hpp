#ifndef SGFRAMEWORK_HEADER_APPLICATION_SETTING_IMPORT_TIED_INFO_HPP_
#define SGFRAMEWORK_HEADER_APPLICATION_SETTING_IMPORT_TIED_INFO_HPP_
#include "../../02_CustomVariable/ConstAndUsing/ConstAndUsing.hpp"
#include "../../02_CustomVariable/String/String.hpp"

namespace SGFramework
{
	namespace ApplicationSetting
	{
		namespace TiredInfo
		{
			constexpr uint cTiredSize = 39;
			constexpr uint cTiredGroopSize = 4;
			constexpr uint cTiredError = VariableLimit::cUintMax;

			constexpr uint cProductName = 0;
			constexpr uint cVersion = 1;
			constexpr uint cQuitMessage = 2;
			constexpr uint cQuitHeader = 3;
			constexpr uint cScreenValueX = 4;
			constexpr uint cScreenValueY = 5;
			constexpr uint cAspectRadioX = 6;
			constexpr uint cAspectRadioY = 7;
			constexpr uint cIsFullScreen = 8;
			constexpr uint cIsVsync = 9;
			constexpr uint cDepth = 10;
			constexpr uint cNear = 11;
			constexpr uint cNumFunctionLines = 12;
			constexpr uint cFixedInterval = 13;
			constexpr uint cMaterVolume = 14;
			constexpr uint cChannels = 15;
			constexpr uint cSampleRate = 16;
			constexpr uint cIsEnableCloseButton = 17;
			constexpr uint cIsEnableAltF4 = 18;
			constexpr uint cRapidInterval = 19;
			constexpr uint cMouseDedZone = 20;
			constexpr uint cJoystickDedZone = 21;
			constexpr uint cWindowDedZone = 22;
			constexpr uint cIsMouseInsideFrame = 23;
			constexpr uint cNumDeleteCapacity = 24;
			constexpr uint cNumDeleteOfOneFlame = 25;
			constexpr uint cGravity = 26;
			constexpr uint cHitPairVectorReserves = 27;
			constexpr uint cIsUsingGUI = 28;
			constexpr uint cGUIVertexShaderDebugPath = 29;
			constexpr uint cGUIPixelShaderDebugPath = 30;
			constexpr uint cNumMaxUsingThreads = 31;
			constexpr uint cGUIVertexShaderReleasePath = 32;
			constexpr uint cGUIPixelShaderReleasePath = 33;
			constexpr uint cIsNoRestrictionsFPS = 34;
			constexpr uint cFontSizePerDpi = 35;
			constexpr uint cClearColor = 36;
			constexpr uint cAudioUpdateInterval = 37;
			constexpr uint cAudioSmoothReserve = 38;
			

			constexpr uint cGroopSubmixAudios = 0;
			constexpr uint cGroopLayers = 1;
			constexpr uint cGroopTags = 2; 
			constexpr uint cGroopTimeLayers = 3;

			constexpr wchar cTiredNames[cTiredSize][37] =
			{
				L"productName",
				L"version",
				L"quitMessage",
				L"quitHeader",
				L"screenValueX",
				L"screenValueY",
				L"aspectRadioX",
				L"aspectRadioY",
				L"isFullScreen",
				L"isVsync",
				L"depth",
				L"near",
				L"numThreadFunctionLines",
				L"fixedInterval",
				L"masterVolume",
				L"masterChannels",
				L"sampleRate",
				L"isEnableCloseButton",
				L"isEnableAltF4",
				L"rapidInterval",
				L"mouseDedZone",
				L"joystickDedZone",
				L"windowDedZone",
				L"isMouseInsideFrame",
				L"numDeleteCapacity",
				L"numDeleteOfOneFlame",
				L"gravity",
				L"hitPairVectorReserves",
				L"isUsingGUI",
				L"guiVertexShaderPath[Debug]",
				L"guiPixelShaderPath[Debug]",
				L"numMaxUsingThreads",
				L"guiVertexShaderPath[Release]",
				L"guiPixelShaderPath[Release]",
				L"isNoRestrictionsFPS",
				L"fontScalePerDpi",
				L"clearColor",
				L"audioUpdateInterval",
				L"audioSmoothReserve"
			};
			
			constexpr wchar cTiredGroopNames[cTiredGroopSize][14] =
			{
				L"submixLayers",
				L"Layers",
				L"Tags",
				L"timeLayers"
			};

			inline uint SearchTired(const sgstring& string, uint* result)
			{
				for (uint i = 0; i < cTiredSize; i++)
				{
					if (string == cTiredNames[i])
					{
						if (result != nullptr) *result = i;
						return i;
					}
				}
				if (result != nullptr) *result = cTiredError;
				return cTiredError;
			}


			inline uint SearchTiredGroop(const sgstring& string, uint* result)
			{
				for (uint i = 0; i < cTiredGroopSize; i++)
				{
					if (string == cTiredGroopNames[i])
					{
						if (result != nullptr) *result = i;
						return i;
					}
				}
				if (result != nullptr) *result = cTiredError;
				return cTiredError;
			}
		}
	}
}
#endif //!_APPLICATION_SETTING_IMPORT_TIED_INFO_HPP_