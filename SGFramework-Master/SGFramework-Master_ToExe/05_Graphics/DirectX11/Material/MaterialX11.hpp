/*----------------------------------------------------------------------------------
DirectX11 Materialを扱うMaterialX11 class
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_MATERIAL_X11_HPP_
#define SGFRAMEWORK_HEADER_MATERIAL_X11_HPP_
#include "../../../02_CustomVariable/CustomVariable.hpp"
#include "../GraphicsX11/GraphicsX11.hpp"
#include "../GraphicsObject/GraphicsObjectX11.hpp"
#include "../ShaderAsset/VertexShaderX11.hpp"
#include "../ShaderAsset/PixelShaderAssetX11.hpp"
#include "../TextureAsset/TextureAssetX11.hpp"

// Framework namespace
namespace SGFramework
{
	//フレームワークの管理をするAdministrator namespace
	namespace Administrator
	{
		//friend
		class GraphicsX11;

		//GraphicsDetail namespace
		namespace GraphicsDetail
		{
			class ModelAssetX11;

			class MaterialX11
			{
			public:
				friend ModelAssetX11;

				MaterialX11() {}

				//マルチテクスチャ対応数
				static constexpr uint cNumSupportedTextures = 2;
				//ノーマルマップ対応数
				static constexpr uint cNumSupportedNormalMaps = 1;
				//ハイトマップ対応数
				static constexpr uint cNumSupportedHeightMaps = 1;

				SGF_FUNCTION_PROPERTY void setMaterial();

				Color ambient = Const::Color::white;		//Ambient
				Color diffuse = Const::Color::white;		//Diffuse
				Color emission = Const::Color::white;		//Emission
				Color specular = Const::Color::white;		//Specular
				Vector3 bump = Const::Vector3::zero;	//Bump
				float manualAlpha = 1.0f;						//	manual alpha
				float power = 1.0f;									//Power

				//name
				sgstring name;
				//Vertex Shader Asset
				SharedPointer<Administrator::GraphicsDetail::VertexShaderAssetX11> vertexShader;
				//Pixel Shader Asset
				SharedPointer<Administrator::GraphicsDetail::PixelShaderAssetX11> pixelShader;
				//Texture Assets
				SharedPointer<TextureAssetX11> textures[cNumSupportedTextures];
				//Texture Asset (Noraml Map)
				SharedPointer<TextureAssetX11> normalMap[cNumSupportedNormalMaps];
				//Texture Asset (Height Map)
				SharedPointer<TextureAssetX11> heightMap[cNumSupportedHeightMaps];
			};

			void MaterialX11::setMaterial()
			{
				memcpy_s(&Administrator::GraphicsX11::m_setConstantBufferMaterial3D,
					sizeof(Administrator::GraphicsX11::m_setConstantBufferMaterial3D),
					this, sizeof(Administrator::GraphicsDetail::X11::ConstantBufferMaterial3D));

				Administrator::GraphicsX11::context->UpdateSubresource(Administrator::GraphicsX11::m_constantBufferMaterial3D,
					0, nullptr, &Administrator::GraphicsX11::m_setConstantBufferMaterial3D, 0, 0);

				Administrator::GraphicsX11::context->VSSetConstantBuffers(Administrator::GraphicsX11::m_cConstantBufferMaterial3DSlot,
					1, &Administrator::GraphicsX11::m_constantBufferMaterial3D);

				Administrator::GraphicsX11::context->PSSetConstantBuffers(Administrator::GraphicsX11::m_cConstantBufferMaterial3DSlot,
					1, &Administrator::GraphicsX11::m_constantBufferMaterial3D);

				for (uint i = 0, size = MaterialX11::cNumSupportedTextures; i < size; ++i)
					if (IS_TRUE(textures[i].getIsValid())) textures[i]->setTexture(i);

				if (IS_TRUE(vertexShader.getIsValid())) vertexShader->setShader();
				if (IS_TRUE(pixelShader.getIsValid())) pixelShader->setShader();
			}
		}
	}
}
#endif //!SGFRAMEWORK_HEADER_MATERIAL_X11_HPP_