/*----------------------------------------------------------------------------------
���ʎq�ƂȂ�Tag class
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_TAG_HPP_
#define SGFRAMEWORK_HEADER_TAG_HPP_
#include "../ConstAndUsing/ConstAndUsing.hpp"
#include "../String/SGString.hpp"
#include "../ReadElement/ReadElement.hpp"

// Framework namespace
namespace SGFramework
{
	//friend
	class Application;
	
	//���ʎq�ƂȂ�Tag class
	class Tag
	{
	public:
		//friend
		friend class Application;

		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�^�O������������, ID = cMissingTag
		inline Tag() : m_id(cMissingTag) {}
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�^�O������������
		//throw: Name�������̏ꍇ
		//����1: �^�O��
		inline Tag(const sgstring& tagName) : m_tagName(L""), m_id(cMissingTag)
		{
			auto find = m_tags.find(tagName);

			if (find != m_tags.end())
			{
				m_tagName = tagName;
				m_id = find->second;
			}
			else
				throw InvalidArgument(L"Error! Tag->TagManagement", L"Missing Tag Name : " + tagName);
		}
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�^�O������������
		//throw: ID�������̏ꍇ
		//����1: �^�OID
		inline Tag(uint tagID) : m_tagName(L""), m_id(cMissingTag)
		{
			if (tagID < static_cast<uint>(m_tags.size()))
			{
				m_id = tagID;
				for (auto& e : m_tags)
					if (e.second == m_id)
					{
						m_tagName = e.first;
						break;
					}
			}
			else
				throw InvalidArgument(L"Error! Tag->TagManagement", L"Missing Tag ID : " + tagID);
		}

		//default
		Tag(const Tag& tag) = default;
		//default
		Tag(Tag&& tag) = default;
		//default
		Tag& operator = (const Tag& tag) = default;
		//default
		Tag& operator = (Tag&& tag) = default;

		//get tag name property (read only)
		inline const sgstring& getName() const { return m_tagName; }
		//get tag id property (read only)
		inline uint getID() const { return m_id; }


		//----------------------------------------------------------------------------------
		//[SearchName]
		//return: tagID��tag���������ꍇ: tag->getName(), tag���Ȃ������ꍇ: L""
		inline static sgstring SearchName(uint tagID)
		{
			if (tagID < static_cast<uint>(m_tags.size()))
			{
				for (auto& e : m_tags)
					if (e.second == tagID)
						return e.first;
			}
			else 
				return L"";
		}
		//----------------------------------------------------------------------------------
		//[SearchID]
		//return: tagName��tag���������ꍇ: tag->getID(), tag���Ȃ������ꍇ: cMissingTag
		inline static uint SearchID(const sgstring& tagName) 
		{ 
			auto find = m_tags.find(tagName);

			if (find != m_tags.end()) return find->second;
			else return cMissingTag;
		}
		//----------------------------------------------------------------------------------
		//[DefaultTag]
		//return: default tag
		//get default tag property
		inline static Tag DefaultTag() { return Tag(sgstring(cDefaultTagName), cDefaultTagID); }

		// == operator
		inline bool operator == (const Tag& left) const { return (m_id == left.m_id); }
		// != operator
		inline bool operator != (const Tag& left) const { return (m_id != left.m_id); }
		// == operator
		inline bool operator == (const sgstring& left) const { return (m_tagName == left); }
		// != operator
		inline bool operator != (const sgstring& left) const { return (m_tagName != left); }

		//Default Tag
		static constexpr wchar cDefaultTagName[] = L"Default";
		//Default Tag ID
		static constexpr uint cDefaultTagID = 0;
		//SearchID���s���^�O���Ȃ������ꍇ�ɕԋp�����
		static constexpr uint cMissingTag = VariableLimit::cUintMax;
	
	private:
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�^�O������������
		//����1: �^�O��
		//����2: �^�OID
		inline Tag(const sgstring& tagName, uint tagID) : m_tagName(tagName), m_id(tagID) {}

		//----------------------------------------------------------------------------------
		//[ReadTags]
		//SettingFile����Tag��ǂݍ���
		//����1: TagGroop
		inline static void ReadTags(const ReadElement::Groop& tags);

		//�^�O���}�b�v
		static std::unordered_map<std::wstring, uint> m_tags;
		sgstring m_tagName;	//�^�O��
		uint m_id;					//�^�OID
	};
	//----------------------------------------------------------------------------------
	//[ReadTags]
	//SettingFile����Tag��ǂݍ���
	//����1: TagGroop
	inline void Tag::ReadTags(const ReadElement::Groop & tags)
	{
		//Tag ID
		uint tagID = 1;

		//Default�o�^
		m_tags.try_emplace(cDefaultTagName, cDefaultTagID);
		
		//�o�^���[�v
		for (auto& e : tags.packs)
			m_tags.try_emplace(e.header, tagID++);
	}
}
#endif //!SGFRAMEWORK_HEADER_TAG_HPP_