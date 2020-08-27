/*----------------------------------------------------------------------------------
各クラスでReadした要素の格納先として使用するReadElement class
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_READ_ELEMENT_HPP_
#define SGFRAMEWORK_HEADER_READ_ELEMENT_HPP_
#include "../Stringf/Stringf.hpp"
#include "../Exception/Exception.hpp"
#include "../MacroAndUsing/MacroAndUsing.hpp"

//Framework namespace
namespace SGFramework
{
	//ファイル読み込み等で使用する変数の型が不定のIndefiniteContainer class
	class IndefiniteContainer
	{
	public:
		//コンテナの中身
		//Int, Floatの場合全てのvalueに値が格納される
		//Stringの場合valueStringにのみ値が格納される
		enum class Type
		{
			Int = 0x1,		//valueInt
			Float = 0x2,	//valueFloat
			String = 0x4	//valueString
		};

		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//コンテナをstring_w型で初期化する
		//argument 1: init value
		inline IndefiniteContainer(const string_w& value) { Init(value); }
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//コンテナをfloat型で初期化する
		//argument 1: init value
		inline IndefiniteContainer(float value) { Init(value); }
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//コンテナをint型で初期化する
		//argument 1: init value
		inline IndefiniteContainer(int value) { Init(value); }

		//delete
		IndefiniteContainer() = delete;
		//default
		IndefiniteContainer(const IndefiniteContainer&) = default;
		IndefiniteContainer(IndefiniteContainer&&) = default;
		IndefiniteContainer& operator =(const IndefiniteContainer&) = default;
		IndefiniteContainer& operator =(IndefiniteContainer&&) = default;

		//----------------------------------------------------------------------------------
		//[Init]
		//コンテナをstring_w型で初期化する
		//argument 1: init value
		void Init(const string_w& value) noexcept { m_type = Type::String; m_valueString = value; m_valueFloat = 0.0f; m_valueInt = 0; }
		//----------------------------------------------------------------------------------
		//[Init]<possibly throw(debug only)>
		//コンテナをfloat型で初期化する
		//argument 1: init value
		void Init(float value) { TRY_CATCH_ON_DEBUG(m_type = Type::Float; m_valueString = stringf::to(value); m_valueFloat = value; m_valueInt = static_cast<int>(value), throw); }
		//----------------------------------------------------------------------------------
		//[Init]<possibly throw(debug only)>
		//コンテナをint型で初期化する
		//argument 1: init value
		void Init(int value) { TRY_CATCH_ON_DEBUG(m_type = Type::Int; m_valueString = stringf::to(value); m_valueFloat = static_cast<float>(value); m_valueInt = value;, throw); }
		
		//<property> saved type
		SGF_PROPERTY Type _type() const { return m_type; }
		//<property> value (string_w)
		SGF_PROPERTY const string_w& _valueString() const { return m_valueString; }
		//<property> value (float)
		SGF_PROPERTY float _valueFloat() const { return m_valueFloat; }
		//<property> value (int)
		SGF_PROPERTY int _valueInt() const { return m_valueInt; }


	private:
		Type m_type;						//読み込みタイプ
		string_w m_valueString;		//value string
		float m_valueFloat;				//value float
		int m_valueInt;					//value int
	};

	//IndefiniteContainerをヘッダー付きリストとして実装できるIndefiniteContainersLine class
	class IndefiniteContainersLine
	{
	public:
		//using list = std::vector<IndefiniteContainer>
		using ListType = std::vector<IndefiniteContainer>;

		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//header無しの状態でインスタンス化する
		inline IndefiniteContainersLine() : m_header(), m_containers() {}
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//argument 1: initialize heder
		inline IndefiniteContainersLine(const string_w& header) : m_header(header), m_containers() {}

		//----------------------------------------------------------------------------------
		//[EmplaceBack]
		//リストの最後尾にコンテナをstring_w格納型で追加する
		//argument 1: init value
		inline void EmplaceBack(const string_w& value) { m_containers.emplace_back(value); }
		//----------------------------------------------------------------------------------
		//[EmplaceBack]
		//リストの最後尾にコンテナをfloat格納型で追加する
		//argument 1: init value
		inline void EmplaceBack(float value) { m_containers.emplace_back(value); }
		//----------------------------------------------------------------------------------
		//[EmplaceBack]
		//リストの最後尾にコンテナをint格納型で追加する
		//argument 1: init value
		inline void EmplaceBack(int value) { m_containers.emplace_back(value); }

		//----------------------------------------------------------------------------------
		//[RemoveAt]<possibly throw(debug only)>
		//リストからindexの要素を削除する
		//argument 1: index
		inline void RemoveAt(int index) { TRY_CATCH_ON_DEBUG(m_containers.erase(m_containers.begin() + index), throw Exception::InvalidArgumentException("IndefiniteContainersLine", "RemoveAt", "invalid index")); }

		//<property> header
		SGF_PROPERTY const string_w& _header() const { return m_header; }
		//<property> header(set) argument 1: set value
		SGF_PROPERTY const string_w& _header(const string_w& set) { m_header = set; return m_header; }
		//<property> containers list
		SGF_PROPERTY const ListType& _containers() const { return m_containers; }
		//<property> containers.size()
		SGF_PROPERTY std::size_t _containersSize() const { return m_containers.size(); }

		//[] operator<possibly throw(debug only)> argument 1: index
		inline IndefiniteContainer& operator [](uint index)
		{
#if defined (SGF_DEBUG)
			if (index >= static_cast<uint>(m_containers.size()))
				throw Exception::InvalidArgumentException("IndefiniteContainersLine", "operator []", "invalid index");
#endif	//SGF_DEBUG
			return m_containers[index];
		}
		//[] operator<possibly throw(debug only)> argument 1: index
		inline const IndefiniteContainer& operator [](uint index) const
		{
#if defined (SGF_DEBUG)
			if (index >= static_cast<uint>(m_containers.size()))
				throw Exception::InvalidArgumentException("IndefiniteContainersLine", "operator []", "invalid index");
#endif	//SGF_DEBUG
			return m_containers[index];
		}

	private:
		string_w m_header;		//ヘッダー
		ListType m_containers;	//コンテナリスト
	};

	//IndefiniteContainersLineをヘッダー付きリスト(IndefiniteContainer 2D List)として実装できるIndefiniteContainersTable class
	class IndefiniteContainersTable
	{
		//using list = std::vector<IndefiniteContainersLine>
		using ListType = std::vector<IndefiniteContainersLine>;

		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//header無しの状態でインスタンス化する
		inline IndefiniteContainersTable() : m_header(), m_containers() {}
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//argument 1: initialize heder
		inline IndefiniteContainersTable(const string_w& header) : m_header(header), m_containers() {}

		//----------------------------------------------------------------------------------
		//[EmplaceBack]
		//リストの最後尾にコンテナリストを追加する
		//argument 1: init value
		inline void EmplaceBack(const string_w& header) { m_containers.emplace_back(header); }

		//----------------------------------------------------------------------------------
		//[RemoveAt]<possibly throw(debug only)>
		//リストからindexの要素を削除する
		//argument 1: index
		inline void RemoveAt(int index) { TRY_CATCH_ON_DEBUG(m_containers.erase(m_containers.begin() + index), throw Exception::InvalidArgumentException("IndefiniteContainersLine", "RemoveAt", "invalid index")); }

		//<property> header
		SGF_PROPERTY const string_w& _header() const { return m_header; }
		//<property> header(set) argument 1: set value
		SGF_PROPERTY const string_w& _header(const string_w& set) { m_header = set; return m_header; }
		
		//<property> containers list (ref)
		SGF_PROPERTY ListType& _containers() { return m_containers; }
		//<property> containers list (const ref)
		SGF_PROPERTY const ListType& _containers() const { return m_containers; }

		//<property> containers.size()
		SGF_PROPERTY std::size_t _containersSize() const { return m_containers.size(); }

		//[] operator<possibly throw(debug only)> argument 1: index
		inline IndefiniteContainersLine& operator [](uint index)
		{
#if defined (SGF_DEBUG)
			if (index >= static_cast<uint>(m_containers.size()))
				throw Exception::InvalidArgumentException("IndefiniteContainersTable", "operator []", "invalid index");
#endif	//SGF_DEBUG
			return m_containers[index];
		}
		//[] operator<possibly throw(debug only)> argument 1: index
		inline const IndefiniteContainersLine& operator [](uint index) const
		{
#if defined (SGF_DEBUG)
			if (index >= static_cast<uint>(m_containers.size()))
				throw Exception::InvalidArgumentException("IndefiniteContainersTable", "operator []", "invalid index");
#endif	//SGF_DEBUG
			return m_containers[index];
		}

	private:
		string_w m_header;		//ヘッダー
		ListType m_containers;	//コンテナリスト
	};
}

#endif // !SGFRAMEWORK_HEADER_READ_ELEMENT_HPP_