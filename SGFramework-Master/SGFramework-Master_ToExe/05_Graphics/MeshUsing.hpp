#ifndef SGFRAMEWORK_HEADER_MESH_USING_HPP_
#define SGFRAMEWORK_HEADER_MESH_USING_HPP_
#include "DirectX11/Mesh/MeshX11.hpp"
#include "DirectX11/Mesh/MeshRendererX11.hpp"

// Framework namespace
namespace SGFramework
{
	//Used Graphics DirectX11
	template<class VertexType, class MaterialPointerType>
	using Mesh = Administrator::GraphicsDetail::MeshX11<VertexType, MaterialPointerType>;
	template<class MeshVertexType, class MeshMaterialPointerType>
	using MeshRenderer = Administrator::GraphicsDetail::MeshRendererX11<MeshVertexType, MeshMaterialPointerType>;
}
#endif // !SGFRAMEWORK_HEADER_MESH_USING_HPP_