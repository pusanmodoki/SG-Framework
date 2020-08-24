/*----------------------------------------------------------------------------------
���ʎq�ƂȂ�Layer, LayerMask structure
���ʎq�ƂȂ�Tag class
------------------------------------------------------------------------------------*/
#include "Layer.hpp"
#include "Tag.hpp"


// Framework namespace
namespace SGFramework
{
	//Layer���}�b�v
	std::unordered_map<std::wstring, Layer> Layer::m_layers;
	//Layer���}�b�v
	std::unordered_map<uint, Layer> Layer::m_layersKeyID;
	//LayerHitTable
	std::unordered_map<uint, uint> Layer::m_layerHitTable;

	//�^�O���}�b�v
	std::unordered_map<std::wstring, uint> Tag::m_tags;
}