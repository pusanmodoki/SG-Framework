#include "ApplicationSetting.hpp"

namespace SGFramework
{
	namespace ApplicationSetting
	{
		const ReadElement::Pack & ApplicationSettingValues::SearchPack(const sgstring & header) const
		{
			for (auto& e : m_elements)
				if (e.second.header == header) return e.second;
			throw InvalidArgument(L"Error! ApplicationSettingValues->SearchPack", L"content not found : " + header);
		}

		const ReadElement::Groop & ApplicationSettingValues::SearchGroop(const sgstring & header) const
		{
			for (auto& e : m_elementGroops)
				if (e.second.header == header) return e.second;
			throw InvalidArgument(L"Error! ApplicationSettingValues->SearchGroop", L"content not found : " + header);
		}

		ReadElement::Pack & ApplicationSettingValues::getPack(uint tiedNumber)
		{
			auto it = m_elements.find(tiedNumber);
			if (it != m_elements.end()) return it->second;
			throw InvalidArgument(L"Error! ApplicationSettingValues->getPack", sgstring(L"content not found : ") + TiredInfo::cTiredNames[tiedNumber]);
		}

		ReadElement::Groop & ApplicationSettingValues::getGroop(uint tiedNumber)
		{
			auto it = m_elementGroops.find(tiedNumber);
			if (it != m_elementGroops.end()) return it->second;
			throw InvalidArgument(L"Error! ApplicationSettingValues->GetGroop", sgstring(L"content not found : ") + TiredInfo::cTiredGroopNames[tiedNumber]);
		}

		void ApplicationSettingValues::ReadSetting()
		{
			sgstring settingPathEdit = cApplicationSettingPath;
			File::FileText read;

			if (IsDebug::isValue())
			{
				try
				{
					File::SecFileRW::ReadNotLocked(settingPathEdit + L".sec", read, false);
				}
				catch (...)
				{
					MessageBox(Window::HwndHolder::main, L"ApplicationSetting.sec Not Found", L"error", MB_OK);
					throw;
				}
			}
			else
			{
				try
				{
					File::SecFileRW::ReadLocked(settingPathEdit + L".secl", read, false);
				}
				catch (...)
				{
					MessageBox(Window::HwndHolder::main, sgstring(sgstring(cApplicationSettingPath) + sgstring(L".sec Not Found")).c_str(), L"error", MB_OK);
					throw;
				}
			}

			uint searchResult = 0;
			uint notTiredCount = TiredInfo::cTiredSize;
			uint notTiredGroopCount = TiredInfo::cTiredGroopSize;
			uint groopKey = 0;
			bool isGroopMode = false;
			for (auto& e : read)
			{
				uint size = static_cast<uint>(e.size());
				if (size == 1)
				{
					if (e[0].my_functions.IsStartWith(StartGroopIdentifier))
					{
						isGroopMode = true;
						e[0].erase(e[0].begin(), e[0].begin() + (sizeof(StartGroopIdentifier) - 2) / 2);
						if (TiredInfo::SearchTiredGroop(e[0], &searchResult) == TiredInfo::cTiredError)
						{
							m_elementGroops.try_emplace(notTiredGroopCount, ReadElement::Groop(e[0]));
							groopKey = notTiredGroopCount++;
						}
						else
						{
							m_elementGroops.try_emplace(searchResult, ReadElement::Groop(e[0]));
							groopKey = searchResult;
						}
					}
					else if (e[0].my_functions.IsStartWith(cEndGroopIdentifier))
						isGroopMode = false;
					continue;
				}
				else if (size == 0) continue;

				ReadElement::Pack push;
				push.header = e[0];

				for (auto it = e.begin() + 1; it != e.end(); it++)
				{
					push.elements.push_back(ReadElement());
					auto& pushElement = push.elements.back();
					if (it->my_functions.IsFirstOfNumbers())
					{
						try
						{
							if (it->find_first_of(L'.') != sgstring::npos)
							{
								pushElement.valueFloat = std::stof(*it);
								pushElement.valueInt = static_cast<int>(pushElement.valueFloat);
								pushElement.valueString = std::move(*it);
								pushElement.readType = ReadElement::ReadType::Float;
							}
							else
							{
								pushElement.valueInt = std::stoi(*it);
								pushElement.valueFloat = static_cast<float>(pushElement.valueInt);
								pushElement.valueString = std::move(*it);
								pushElement.readType = ReadElement::ReadType::Int;
							}
						}
						catch (...)
						{
							pushElement.valueString = *it;
							pushElement.readType = ReadElement::ReadType::String;
						}
					}
					else
					{
						pushElement.valueString = *it;
						pushElement.readType = ReadElement::ReadType::String;
					}
				}
				if (IS_FALSE(isGroopMode))
				{
					if (TiredInfo::SearchTired(push.header, &searchResult) != TiredInfo::cTiredError)
						m_elements.try_emplace(searchResult, std::move(push));
					else
						m_elements.try_emplace(notTiredCount++, std::move(push));
				}
				else
				{
					m_elementGroops[groopKey].packs.push_back(push);
				}
			}
		}
	}
}