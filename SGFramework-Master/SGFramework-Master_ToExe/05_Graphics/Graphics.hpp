#ifndef SGFRAMEWORK_HEADER_GRAPHICS_HPP_
#define SGFRAMEWORK_HEADER_GRAPHICS_HPP_
#include "Animation/Animator.hpp"
#include "TransformUsing.hpp"
#include "DirectX11/GraphicsX11/GraphicsX11.hpp"
#include "DirectX11/ShaderAsset/ShaderAssetX11.hpp"
#include "DirectX11/ShaderAsset/VertexShaderX11.hpp"
#include "DirectX11/ShaderAsset/PixelShaderAssetX11.hpp"
#include "DirectX11/Light/LightX11.hpp"
#include "DirectX11/Material/MaterialX11.hpp"
#include "DirectX11/Transform/TransformX11.hpp"
#include "DirectX11/Camera/CameraX11.hpp"
#include "DirectX11/TextureAsset/TextureAssetX11.hpp"
#include "DirectX11/Polygon/PolygonX11.hpp"
#include "DirectX11/ModelAsset/ModelAssetX11.hpp"

#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "winmm.lib")

// Framework namespace
namespace SGFramework
{
	//Used Graphics DirectX11
	using Graphics = Administrator::GraphicsX11;
	using Material = Administrator::GraphicsDetail::MaterialX11;
	using Camera = Administrator::GraphicsDetail::CameraX11;
	using VSInputElement = Administrator::GraphicsDetail::VertexShaderAssetX11::InputElement;
	using PolygonSquare = Administrator::GraphicsDetail::PolygonSquareX11;

	//Used Graphics DirectX11
	namespace Asset
	{
		using VertexShaderAsset = Administrator::GraphicsDetail::VertexShaderAssetX11;
		using PixelShaderAsset = Administrator::GraphicsDetail::PixelShaderAssetX11;
		using TextureAsset = Administrator::GraphicsDetail::TextureAssetX11;
		using ModelAsset = Administrator::GraphicsDetail::ModelAssetX11;
	}
	//Used Graphics DirectX11
	namespace Light
	{
		using Directional = Administrator::GraphicsDetail::DirectionalLightX11;
		using DirectionalElement = Administrator::GraphicsDetail::DirectionalLightX11::Element;
		using Point = Administrator::GraphicsDetail::PointLightX11;
		using PointElement = Administrator::GraphicsDetail::PointLightX11::Element;
		using HemiSphere = Administrator::GraphicsDetail::HemiSphereLightX11;
		using HemiSphereElement = Administrator::GraphicsDetail::HemiSphereLightX11::Element;
	}	
}
#endif // !SGFRAMEWORK_HEADER_GRAPHICS_HPP_