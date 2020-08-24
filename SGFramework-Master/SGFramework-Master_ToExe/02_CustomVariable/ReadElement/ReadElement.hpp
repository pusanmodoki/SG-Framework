/*----------------------------------------------------------------------------------
各クラスでReadした要素の格納先として使用するReadElement class
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
	//各クラスでReadした要素の格納先として使用するReadElement class
	class ReadElement final
	{
	public:
		//Readした方式
		enum class ReadType;

		ReadType readType;		//読み込みタイプ
		sgstring valueString;		//value string
		float valueFloat = 0.0f;	//value float
		int valueInt = 0;				//value int

		//Readした方式
		//Int, Floatの場合全てのvalueに値が格納される
		//Stringの場合valueStringにのみ値が格納される
		enum class ReadType
		{
			Int = 0X1,		//valueInt
			Float = 0X2,	//valueFloat
			String = 0X4	//valueString
		};

		//ReadElementを複数格納できるPack class
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

			sgstring header;									//Packのヘッダー
			std::vector<ReadElement> elements;	//Elements
		};

		class Groop final
		{
		public:
			//default
			Groop() = default;
			//----------------------------------------------------------------------------------
			//[コンストラクタ]
			//ヘッダーを設定
			//引数1: ヘッダー
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

			sgstring header;					//Groopのヘッダー
			std::vector<Pack> packs;	//Packs
		};
	};
}

#endif // !SGFRAMEWORK_HEADER_READ_ELEMENT_HPP_