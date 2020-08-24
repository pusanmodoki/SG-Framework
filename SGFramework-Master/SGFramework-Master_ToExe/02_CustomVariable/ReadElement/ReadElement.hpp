/*----------------------------------------------------------------------------------
�e�N���X��Read�����v�f�̊i�[��Ƃ��Ďg�p����ReadElement class
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_READ_ELEMENT_HPP_
#define SGFRAMEWORK_HEADER_READ_ELEMENT_HPP_
#include "../String/String.hpp"
#include "../InvalidArgument/InvalidArgument.hpp"
#include "../ConstAndUsing/ConstAndUsing.hpp"
#include "../Window/HwndHolder/HwndHolder.hpp"

//Framework namespace
namespace SGFramework
{
	//�e�N���X��Read�����v�f�̊i�[��Ƃ��Ďg�p����ReadElement class
	class ReadElement final
	{
	public:
		//Read��������
		enum class ReadType;

		ReadType readType;		//�ǂݍ��݃^�C�v
		sgstring valueString;		//value string
		float valueFloat = 0.0f;	//value float
		int valueInt = 0;				//value int

		//Read��������
		//Int, Float�̏ꍇ�S�Ă�value�ɒl���i�[�����
		//String�̏ꍇvalueString�ɂ̂ݒl���i�[�����
		enum class ReadType
		{
			Int = 0X1,		//valueInt
			Float = 0X2,	//valueFloat
			String = 0X4	//valueString
		};

		//ReadElement�𕡐��i�[�ł���Pack class
		class Pack final
		{
		public:
			//default
			Pack() = default;
			//default
			Pack(const Pack&) = default;
			
			//[] operator
			inline const ReadElement& operator [](uint index) const 
			{ 
				if (index >= elements.size())
				{
					MessageBox(Window::HwndHolder::main, L"ReadPack error", L"error", MB_OK);
					throw InvalidArgument(L"Error!! ReadPack->operator []", L"ReadElement not found");
				}
				return elements[index]; 
			}

			//elements.begin() (get function property)
			SGF_FUNCTION_PROPERTY auto begin() { return elements.begin(); }
			//elements.begin() (get function property)
			SGF_FUNCTION_PROPERTY auto end() { return elements.end(); }
			//elements.rbegin() (get function property)
			SGF_FUNCTION_PROPERTY auto rbegin() { return elements.rbegin(); }
			//elements.rend() (get function property)
			SGF_FUNCTION_PROPERTY auto rend() { return elements.rend(); }

			//elements.cbegin() (get function property)
			SGF_FUNCTION_PROPERTY const auto cbegin() const { return elements.cbegin(); }
			//elements.cbegin() (get function property)
			SGF_FUNCTION_PROPERTY const auto cend() const { return elements.cend(); }
			//elements.crbegin() (get function property)
			SGF_FUNCTION_PROPERTY const auto crbegin() const { return elements.crbegin(); }
			//elements.crend() (get function property)
			SGF_FUNCTION_PROPERTY const auto crend() const { return elements.crend(); }

			sgstring header;									//Pack�̃w�b�_�[
			std::vector<ReadElement> elements;	//Elements
		};

		class Groop final
		{
		public:
			//default
			Groop() = default;
			//----------------------------------------------------------------------------------
			//[�R���X�g���N�^]
			//�w�b�_�[��ݒ�
			//����1: �w�b�_�[
			Groop(const sgstring& header) : header(header) {}
			
			//[] operator
			inline const Pack& operator [](uint index) const { return packs[index]; }

			//elements.begin() (get function property)
			SGF_FUNCTION_PROPERTY auto begin() { return packs.begin(); }
			//elements.begin() (get function property)
			SGF_FUNCTION_PROPERTY auto end() { return packs.end(); }
			//elements.rbegin() (get function property)
			SGF_FUNCTION_PROPERTY auto rbegin() { return packs.rbegin(); }
			//elements.rend() (get function property)
			SGF_FUNCTION_PROPERTY auto rend() { return packs.rend(); }

			//elements.cbegin() (get function property)
			SGF_FUNCTION_PROPERTY const auto cbegin() const { return packs.cbegin(); }
			//elements.cbegin() (get function property)
			SGF_FUNCTION_PROPERTY const auto cend() const { return packs.cend(); }
			//elements.crbegin() (get function property)
			SGF_FUNCTION_PROPERTY const auto crbegin() const { return packs.crbegin(); }
			//elements.crend() (get function property)
			SGF_FUNCTION_PROPERTY const auto crend() const { return packs.crend(); }

			sgstring header;					//Groop�̃w�b�_�[
			std::vector<Pack> packs;	//Packs
		};
	};
}

#endif // !SGFRAMEWORK_HEADER_READ_ELEMENT_HPP_