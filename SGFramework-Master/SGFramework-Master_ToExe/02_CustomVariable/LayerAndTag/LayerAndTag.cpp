/*----------------------------------------------------------------------------------
識別子となるLayer, LayerMask structure
識別子となるTag class
------------------------------------------------------------------------------------*/
#include "Layer.hpp"
#include "Tag.hpp"


// Framework namespace
namespace SGFramework
{
	//Layer名マップ
	std::unordered_map<std::wstring, Layer> Layer::m_layers;
	//Layer名マップ
	std::unordered_map<uint, Layer> Layer::m_layersKeyID;
	//LayerHitTable
	std::unordered_map<uint, uint> Layer::m_layerHitTable;

	//タグ名マップ
	std::unordered_map<std::wstring, uint> Tag::m_tags;
}