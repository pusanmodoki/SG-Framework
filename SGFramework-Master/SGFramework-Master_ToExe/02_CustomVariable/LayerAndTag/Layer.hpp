/*----------------------------------------------------------------------------------
識別子となるLayer, LayerMask structure
------------------------------------------------------------------------------------*/
#ifndef SGFREAMWORK_HEADER_LAYER_HPP_
#define SGFREAMWORK_HEADER_LAYER_HPP_
#include "../ConstAndUsing/ConstAndUsing.hpp"
#include "../String/SGString.hpp"
#include "../ReadElement/ReadElement.hpp"
#include "../../03_MathAF/MathAF.hpp"

// Framework namespace
namespace SGFramework
{
	//friend
	class Application;

	//prototype
	struct Layer;
	struct LayerMask;

	//識別子となるLayer structure
	struct Layer
	{
	public:
		//friend
		friend class Application;
		friend struct LayerMask;

		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//タグを初期化する, Layer = cMissingLayer
		inline Layer() : m_layer(cMissingLayer) {}
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//タグを初期化する
		//throw: Nameが無効の場合
		//引数1: タグ名
		inline Layer(const sgstring& layerName) : m_layer(cMissingLayer)
		{
			auto find = m_layers.find(layerName);

			//有効
			if (find != m_layers.end())
				*this = find->second;
			//無効
			else
			{
				m_layer = cMissingLayer;
				throw InvalidArgument(L"Error! Layer->Constructor", L"Missing Layer Name : " + layerName);
			}
		}
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//タグを初期化する
		//throw: IDが無効の場合
		//引数1: LayerID
		inline Layer(uint layerID) : m_layer(cMissingLayer)
		{
			auto find = m_layersKeyID.find(layerID);

			//有効
			if (find != m_layersKeyID.end())
				*this = find->second;
			//無効
			else
			{
				m_layer = cMissingLayer;
				throw InvalidArgument(L"Error! Layer->Constructor", L"Missing Layer : " + sgstring::to_sgstring(m_layer));
			}
		}

		//default
		Layer(const Layer& layer) = default;
		//default
		Layer(Layer&& layer) = default;
		//default
		Layer& operator = (const Layer& layer) = default;
		//default
		Layer& operator = (Layer&& layer) = default;

		//----------------------------------------------------------------------------------
		//[DefaultLayer]
		//return: default Layer
		inline static Layer DefaultLayer() { return Layer(cDefaultLayerID); }
		//----------------------------------------------------------------------------------
		//[SearchLayer]
		//return: LayerNameのLayerがあった場合: Layer, Layerがなかった場合: cMissingLayer
		//throw: Layerが存在しなかった場合
		//引数1: LayerName 
		inline static Layer SearchLayer(const sgstring& layerName) 
		{
			auto find = m_layers.find(layerName);

			if (find != m_layers.end())
				return find->second;
			else
				throw InvalidArgument(L"Error! Layer->SearchLayer", L"Missing Layer Name : " + layerName);
		}
		//----------------------------------------------------------------------------------
		//[SearchLayer]
		//return: LayerNameのLayerがあった場合: Layer, Layerがなかった場合: cMissingLayer
		//throw: Layerが存在しなかった場合
		//引数1: LayerID
		inline static Layer SearchLayer(uint layerID)
		{
			auto find = m_layersKeyID.find(layerID);

			if (find != m_layersKeyID.end())
				return find->second;
			else
				throw InvalidArgument(L"Error! Layer->SearchLayer", L"Missing LayerID : " + layerID);
		}

		//layer name (get function property)
		SGF_FUNCTION_PROPERTY const sgstring& getLayaerName() const
		{
			return m_name;
		}
		//layer id (layer value = 1 << result, get function property)
		SGF_FUNCTION_PROPERTY uint getLayaerID() const
		{
			if (m_layer == cMissingLayer || m_layer == 0) return cMissingLayer;

			uint result = 1;
			for (uint layer = m_layer; (layer & 1) == 0; layer >> 1, ++result) {}

			return result;
		}
		//layer value (get function property)
		SGF_FUNCTION_PROPERTY uint getLayerValue() const { return m_layer; }

		//all layer names<static>(get function property)
		static SGF_FUNCTION_PROPERTY std::vector<sgstring> getAllLayaerNames()
		{
			//result
			std::vector<sgstring> result;
			for (auto& e : m_layers) result.emplace_back(e.first);
			return std::move(result);
		}
		//all layer ids<static>(get function property)
		static SGF_FUNCTION_PROPERTY std::vector<uint> getAllLayaerIDs()
		{
			//result
			std::vector<uint> result;
			for (auto& e : m_layers) result.emplace_back(e.second);
			return std::move(result);
		}
		//layer->collision mask<static>(get function property)
		static SGF_FUNCTION_PROPERTY uint getLayerCollisionMask(uint layerValue)
		{
			auto find = m_layerHitTable.find(layerValue);
			if (find != m_layerHitTable.end()) return find->second;
			else return 0;
		}

		//uint operator
		inline operator uint() const { return m_layer; }
		//() operator
		inline uint operator ()() const { return m_layer; }

		// == operator
		inline bool operator == (Layer left) const { return (m_layer == left.m_layer) ? true : false; }
		// == operator
		inline bool operator == (uint left) const { return (m_layer == left) ? true : false; }
		// != operator
		inline bool operator != (Layer left) const { return (m_layer != left.m_layer) ? true : false; }
		// != operator
		inline bool operator != (uint left) const { return (m_layer != left) ? true : false; }

		// & operator
		inline uint operator & (Layer left) const { return this->m_layer & left.m_layer; }
		// & operator
		inline uint  operator & (uint left) const { return this->m_layer & left; }
		// | operator
		inline uint operator | (Layer left) const { return this->m_layer | left.m_layer; }
		// | operator
		inline uint  operator | (uint left) const { return this->m_layer | left; }
		// ^ operator
		inline uint operator ^ (Layer left) const { return this->m_layer ^ left.m_layer; }
		// ^ operator
		inline uint  operator ^ (uint left) const { return this->m_layer ^ left; }

		// &= operator
		inline uint operator &= (Layer left) { return this->m_layer &= left.m_layer; }
		// &= operator
		inline uint  operator &= (uint left) { return this->m_layer &= left; }
		// |= operator
		inline uint operator |= (Layer left) { return this->m_layer |= left.m_layer; }
		// |= operator
		inline uint  operator |= (uint left) { return this->m_layer |= left; }
		// ^= operator
		inline uint operator ^= (Layer left) { return this->m_layer ^= left.m_layer; }
		// ^= operator
		inline uint  operator ^= (uint left) { return this->m_layer ^= left; }

		// == operator
		inline bool operator == (LayerMask left) const;
		// != operator
		inline bool operator != (LayerMask left) const;
		// & operator
		inline uint operator & (LayerMask left) const;
		// | operator
		inline uint operator | (LayerMask left) const;
		// ^ operator
		inline uint operator ^ (LayerMask left) const;
		// &= operator
		inline uint operator &= (LayerMask left);
		// |= operator
		inline uint operator |= (LayerMask left);
		// ^= operator
		inline uint operator ^= (LayerMask left);
		
		//Default Layer
		static constexpr wchar cDefaultLayerName[] = L"Default";
		//Default Layer ID
		static constexpr uint cDefaultLayerID = 0;
		//Default Layer Value
		static constexpr uint cDefaultLayerValue = 0x00000001;
		//SearchLayer実行時タグがなかった場合に返却される
		static constexpr uint cMissingLayer = VariableLimit::cUintMax;

	private:	
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//レイヤーを初期化する
		//引数1: Layer
		inline Layer(const Layer* layer) : m_layer(layer->getLayerValue()), m_name(layer->getLayaerName()) {}
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//レイヤーを初期化する
		//引数1: Layer id
		//引数2: Layer name
		inline Layer(uint value, const sgstring& name) : m_layer(1 << value), m_name(name) {}

		//----------------------------------------------------------------------------------
		//[ReadLayers]
		//SettingFileからLayerを読み込む
		//引数1: LayerGroop
		inline static void ReadLayers(const ReadElement::Groop& layers);

		//----------------------------------------------------------------------------------
		//[LayerManagement]
		//return: タグを割り当てる or タグを検索 or タグを登録
		//引数1: Layer名*
		//引数2: LayerID*
		//引数3: 実行モード
		//inline static uint LayerManagement(const sgstring* layerName, uint* layerID, uint mode);

		//Layer名マップ
		static std::unordered_map<std::wstring, Layer> m_layers;
		//Layer名マップ
		static std::unordered_map<uint, Layer> m_layersKeyID;
		//LayerHitTable
		static std::unordered_map<uint, uint> m_layerHitTable;

		sgstring m_name;	//name
		uint m_layer;			//layer
	};

	//LayerMask structure
	struct LayerMask
	{
	public:
		friend struct Layer;

		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//レイヤーマスクを初期化する
		inline LayerMask() : value(0) {}
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//レイヤーマスクを初期化する
		//引数1: Layer Value
		inline LayerMask(uint layerValue) : value(layerValue) {}
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//レイヤーマスクを初期化する
		//引数1: Layer Value
		inline LayerMask(int layerValue) : value(static_cast<uint>(layerValue)) {}
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//レイヤーマスクを初期化する
		//引数1: Layer
		inline LayerMask(Layer layer) : value(layer.m_layer) {}
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//レイヤーマスクを初期化する
		//引数pack: Layer IDs
		template<class ...Args>
		inline LayerMask(const Args&... layerIDs) : value(0)
		{
			uint size = static_cast<uint>(Layer::m_layers.size());

			for (uint& e : { layerIDs... })
			{
				if (e < size)
					value |= e;
				else
				{
					value = Layer::cMissingLayer;
					throw InvalidArgument(L"Error! Layer->LayerManagement", L"Missing ID : " + sgstring(e));
				}
			}
		}

		//----------------------------------------------------------------------------------
		//[InitializeUsingNames]
		//レイヤーマスクをLayerNameを使用して初期化する
		//引数pack: Layer Names
		template<class ...Args>
		inline void InitializeUsingNames(const Args&... layerNames)
		{
			for (sgstring& e : { layerNames... })
			{
				if (Layer::m_layers.find(e) != Layer::m_layers.end())
					value |= 1 << Layer::m_layers.at(e);
				else
				{
					value = Layer::cMissingLayer;
					throw InvalidArgument(L"Error! Layer->LayerManagement", L"Missing Name : " + e);
				}
			}
		}
		//----------------------------------------------------------------------------------
		//[InitializeUsingIDs]
		//レイヤーマスクをLayerNameを使用して初期化する
		//引数pack: Layer IDs
		template<class ...Args>
		inline void InitializeUsingIDs(const Args&... layerIDs)
		{
			uint size = static_cast<uint>(Layer::m_layers.size());

			for (uint& e : { layerIDs... })
			{
				if (e < size)
					value |= 1 << e;
				else
				{
					value = Layer::cMissingLayer;
					throw InvalidArgument(L"Error! Layer->LayerManagement", L"Missing ID : " + sgstring(e));
				}
			}
		}

		//all layer mask<static>(get function property)
		SGF_FUNCTION_PROPERTY static LayerMask getAllLayersMask()
		{
			static LayerMask result(VariableLimit::cUintMax);

			return result;
		}

		//----------------------------------------------------------------------------------
		//[CreateUsingNames]
		//レイヤーマスクをLayerNameを使用して初期化する]
		//return: 作成したLayerMask
		//引数pack: Layer Names
		template<class ...Args>
		static inline LayerMask CreateUsingNames(const Args&... layerNames)
		{
			LayerMask result;

			for (sgstring& e : { layerNames... })
			{
				if (Layer::m_layers.find(e) != Layer::m_layers.end())
					result.value |= 1 << Layer::m_layers.at(e);
				else
				{
					result.value = Layer::cMissingLayer;
					throw InvalidArgument(L"Error! Layer->LayerManagement", L"Missing Name : " + e);
				}
			}
		
			return result;
		}
		//----------------------------------------------------------------------------------
		//[CreateUsingIDs]
		//レイヤーマスクをLayerNameを使用して初期化する
		//return: 作成したLayerMask
		//引数pack: Layer IDs
		template<class ...Args>
		static inline LayerMask CreateUsingIDs(const Args&... layerIDs)
		{
			LayerMask result;	
			uint size = static_cast<uint>(Layer::m_layers.size());

			for (uint& e : { layerIDs... })
			{
				if (e < size)
					result.value |= 1 << e;
				else
				{
					result.value = Layer::cMissingLayer;
					throw InvalidArgument(L"Error! Layer->LayerManagement", L"Missing ID : " + sgstring(e));
				}
			}

			return result;
		}

		//default
		LayerMask(const LayerMask& layer) = default;
		//default
		LayerMask(LayerMask&& layer) = default;
		//default
		LayerMask& operator = (const LayerMask& layer) = default;
		//default
		LayerMask& operator = (LayerMask&& layer) = default;

		//uint operator
		inline operator uint() const { return value; }
		//() operator
		inline uint operator ()() const { return value; }

		// == operator
		inline bool operator == (LayerMask left) const { return (value == left.value) ? true : false; }
		// == operator
		inline bool operator == (Layer left) const { return (value == left.m_layer) ? true : false; }
		// == operator
		inline bool operator == (uint left) const { return (value == left) ? true : false; }
		// != operator
		inline bool operator != (LayerMask left) const { return (value != left.value) ? true : false; }
		// != operator
		inline bool operator != (Layer left) const { return (value != left.m_layer) ? true : false; }
		// != operator
		inline bool operator != (uint left) const { return (value != left) ? true : false; }

		// & operator
		inline uint operator & (LayerMask left) const { return this->value & left.value; }
		// & operator
		inline uint operator & (Layer left) const { return this->value & left.m_layer; }
		// & operator
		inline uint  operator & (uint left) const { return this->value& left; }
		// | operator
		inline uint operator | (LayerMask left) const { return this->value | left.value; }
		// | operator
		inline uint operator | (Layer left) const { return this->value | left.m_layer; }
		// | operator
		inline uint  operator | (uint left) const { return this->value | left; }
		// ^ operator
		inline uint operator ^ (LayerMask left) const { return this->value ^ left.value; }
		// ^ operator
		inline uint operator ^ (Layer left) const { return this->value^ left.m_layer; }
		// ^ operator
		inline uint  operator ^ (uint left) const { return this->value^ left; }

		// &= operator
		inline uint operator &= (LayerMask left) { return this->value &= left.value; }
		// &= operator
		inline uint operator &= (Layer left) { return this->value &= left.m_layer; }
		// &= operator
		inline uint  operator &= (uint left) { return this->value &= left; }
		// |= operator
		inline uint operator |= (LayerMask left) { return this->value |= left.value; }
		// |= operator
		inline uint operator |= (Layer left) { return this->value |= left.m_layer; }
		// |= operator
		inline uint  operator |= (uint left) { return this->value |= left; }
		// ^= operator
		inline uint operator ^= (LayerMask left) { return this->value ^= left.value; }
		// ^= operator
		inline uint operator ^= (Layer left) { return this->value ^= left.m_layer; }
		// ^= operator
		inline uint  operator ^= (uint left) { return this->value ^= left; }

		//value
		uint value;
	};


	// == operator
	inline bool Layer::operator == (LayerMask left) const { return (m_layer == left.value) ? true : false; }
	// != operator
	inline bool Layer::operator != (LayerMask left) const { return (m_layer != left.value) ? true : false; }
	// & operator
	inline uint Layer::operator & (LayerMask left) const { return this->m_layer & left.value; }
	// | operator
	inline uint Layer::operator | (LayerMask left) const { return this->m_layer | left.value; }
	// ^ operator
	inline uint Layer::operator ^ (LayerMask left) const { return this->m_layer ^ left.value; }
	// &= operator
	inline uint Layer::operator &= (LayerMask left) { return this->m_layer &= left.value; }
	// |= operator
	inline uint Layer::operator |= (LayerMask left) { return this->m_layer |= left.value; }
	// ^= operator
	inline uint Layer::operator ^= (LayerMask left) { return this->m_layer ^= left.value; }

	//----------------------------------------------------------------------------------
	//[ReadLayers]
	//SettingFileからLayerを読み込む
	//引数1: LayerGroop
	inline void Layer::ReadLayers(const ReadElement::Groop & layers)
	{
		//Layer ID
		uint layerID = 1;

		//Default登録
		m_layers.try_emplace(cDefaultLayerName, Layer(cDefaultLayerID, cDefaultLayerName));
		m_layersKeyID.try_emplace(cDefaultLayerID, Layer(cDefaultLayerID, cDefaultLayerName));

		//登録ループ
		for (auto& e : layers.packs)
		{	
			//Layer登録
			m_layers.try_emplace(e.header, Layer(layerID, e.header));
			m_layersKeyID.try_emplace(layerID, Layer(layerID, e.header));
			layerID++;
		}

		//Table作成ループ
		
		//DefaultLayer
		{
			uint hitLayerMask = 1;
			for (uint i = 0, size = static_cast<uint>(layers.packs.size()); i < size; ++i)
			{
				if (layers.packs[i].elements.size() >= 1)
					hitLayerMask |= static_cast<uint>(layers.packs[i].elements[0].valueInt) << (i + 1);
			}
			m_layerHitTable.try_emplace(1 << cDefaultLayerID, hitLayerMask);
		}

		//OtherLayer
		for (uint i = 0, size = static_cast<uint>(layers.packs.size()); i < size; ++i)
		{
			uint hitLayerMask = 0;
			//横ループ
			for (uint k = 0, size1 = static_cast<uint>(layers.packs[i].elements.size()); k < size1; ++k)
				hitLayerMask |= static_cast<uint>(layers.packs[i].elements[k].valueInt) << k;
			//縦ループ
			for (uint k = i + 1; k < size; ++k)
			{
				if (layers.packs[k].elements.size() > i + 1)
					hitLayerMask |= static_cast<uint>(layers.packs[k].elements[i + 1].valueInt) << (i + 1);
			}
		
			m_layerHitTable.try_emplace(1 << (i + 1), hitLayerMask);
		}
	}
}
#endif // !SGFREAMWORK_HEADER_LAYER_HPP