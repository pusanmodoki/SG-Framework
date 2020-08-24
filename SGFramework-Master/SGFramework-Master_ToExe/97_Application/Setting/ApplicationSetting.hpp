#ifndef SGFRAMEWORK_HEADER_APPLICATION_SETTING_IMPORT_HPP_
#define SGFRAMEWORK_HEADER_APPLICATION_SETTING_IMPORT_HPP_
#include <array>
#include <map>
#include "../../01_LibraryImport/LibraryImport.hpp"
#include "ApplicationSettingTiedInfo.hpp"

//Framework namespace
namespace SGFramework
{
	namespace ApplicationSetting
	{
		constexpr wchar cApplicationSettingPath[] = L"Assets/Settings/SGFApplicationSetting";
		constexpr wchar StartGroopIdentifier[] = L"[StartGroop]";
		constexpr wchar cEndGroopIdentifier[] = L"[EndGroop]";

		class ApplicationSettingValues final
		{
		public:
			friend class Application;

			const ReadElement::Pack& SearchPack(const sgstring& header) const;

			const ReadElement::Groop& SearchGroop(const sgstring& header) const;

			SGF_NOTINLINE_FUNCTION_PROPERTY ReadElement::Pack& getPack(uint tiedNumber);

			SGF_NOTINLINE_FUNCTION_PROPERTY ReadElement::Groop& getGroop(uint tiedNumber);

		private:
			void ReadSetting();
			std::unordered_map<uint, ReadElement::Pack> m_elements;
			std::unordered_map<uint, ReadElement::Groop> m_elementGroops;
		};
	}
}
#endif // !SGFRAMEWORK_HEADER_APPLICATION_SETTING_IMPORT_HPP_
