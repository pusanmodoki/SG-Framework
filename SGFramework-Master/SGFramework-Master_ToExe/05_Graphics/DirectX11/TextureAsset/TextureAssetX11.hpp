/*----------------------------------------------------------------------------------
DirectX11 Textureを扱うBaseTextureAssetX11 class
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_TEXTURE_ASSET_X11_HPP_
#define SGFRAMEWORK_HEADER_TEXTURE_ASSET_X11_HPP_
#include <d3d11.h>
#include "../../../02_CustomVariable/CustomVariable.hpp"
#include "../../../04_Asset/Asset/BaseAsset.hpp"
#include "../GraphicsX11/GraphicsX11.hpp"

//Framework namespace
namespace SGFramework
{
	namespace Administrator
	{
		namespace GraphicsDetail
		{
			template<class VertexType, class MaterialPointerType>
			class MeshX11;
		}
	}

	//フレームワークの管理をするAdministrator namespace
	namespace Administrator
	{
		class AssetManager;

		//GraphicsDetail namespace
		namespace GraphicsDetail
		{
			class ModelAssetX11;
			class MaterialX11;

			//DirectX11 Textureを扱うTextureAssetX11 class
			class TextureAssetX11 final : public BaseClass::BaseAsset
			{
			public:
				//friend
				friend class AssetManager;
				friend class ModelAssetX11;
				friend class MaterialX11;
				template <typename TAsset>
				friend SharedPointer<TAsset> Asset::InstantiateAsset(const sgstring& filePath, bool isRegisterSharedAsset);

				//----------------------------------------------------------------------------------
				//[デストラクタ]
				//リソースの開放を行う
				inline ~TextureAssetX11() override { COM_RELEASE(m_shaderResourceView); CloseBaseAsset(getType(), true); }

				//----------------------------------------------------------------------------------
				//[Open]
				//シェーダーをロードする
				//throw: ロードに失敗した場合
				//引数1: samplerState, default = Graphics::defaultSamplerState
				inline virtual void Open(ID3D11SamplerState* samplerState = Administrator::GraphicsX11::m_defaultSamplerState);
				//----------------------------------------------------------------------------------
				//[Reload]
				//アセットのリロードを行う
				//throw: ロードに失敗した場合
				inline void Reload() override;
				//----------------------------------------------------------------------------------
				//[Close]
				//ファイルを閉じる
				inline void Close() override;

				//----------------------------------------------------------------------------------
				//[ChangeSamplerState]
				//SamplerStateを変更する, nullポインタの場合変更を行わない
				//Graphics::defaultSamplerState以外は自動開放を行わないので注意
				//引数1: samplerState, default = Graphics::defaultSamplerState
				void ChangeSamplerState(ID3D11SamplerState* samplerState
					= Administrator::GraphicsX11::m_defaultSamplerState);

				//get asset type (read function property)
				SGF_FUNCTION_PROPERTY Asset::AssetType getType() override { return Asset::AssetType::Texture; }

				//set texture graphics (draw function only property)
				//argument 1 : slot, defualt = 0
				SGF_FUNCTION_PROPERTY void setTexture(uint slot = 0)
				{
					//シェーダーリソースビューnullでなければテクスチャセット
					if (m_shaderResourceView != nullptr && Administrator::GraphicsX11::m_nowTextureInstanceID != instanceID())
					{
						Administrator::GraphicsX11::m_deviceContext->PSSetShaderResources(slot, 1, &m_shaderResourceView);
						Administrator::GraphicsX11::m_deviceContext->PSSetSamplers(slot, 1, &m_samplerState);
						Administrator::GraphicsX11::m_nowTextureInstanceID = instanceID();
					}
				}

				//Default Slots Texture 0
				static constexpr uint cSlotTexture0 = 0;
				//Default Slots Texture 1
				static constexpr uint cSlotTexture1 = 1;
				//Default Slots Texture Noraml Map
				static constexpr uint cSlotTextureNormalMap = 2;
				//Default Slots Texture Height Map
				static constexpr uint cSlotTextureHeightMap = 3;

				//ShaderResourceView (read function property)
				ID3D11ShaderResourceView* getShaderResourceView() const { return m_shaderResourceView; };
				//SamplerState (read function property)
				ID3D11SamplerState* getSamplerState() const { return m_samplerState; };

			protected:
				//default
				inline TextureAssetX11() : BaseAsset(),
					m_shaderResourceView(nullptr), m_samplerState(nullptr) {}

				//ShaderResourceView
				ID3D11ShaderResourceView* m_shaderResourceView;
				//SamplerState
				ID3D11SamplerState* m_samplerState;
			};

			//----------------------------------------------------------------------------------
			//[Open]
			//シェーダーをロードする
			//throw: ロードに失敗した場合
			//引数1: テクスチャのファイルパス
			//引数2: samplerState, default = Graphics::defaultSamplerState
			inline void TextureAssetX11::Open(ID3D11SamplerState * samplerState)
			{
				if (IS_TRUE(getIsFileOpened())) return;
			
				setIsOpenFailed(false);
				
				ID3D11Resource* textureResource = nullptr;
				HRESULT result = S_OK;

				//拡張子取得
				sgstring extension = name().my_functions.FindExtension();
				if (extension == L"")
				{
					setIsOpenFailed(true);
					throw InvalidArgument(L"Error!! TextureAssetX11->Open",
						L"Invalid extension, extension : " + extension);
				}

				//dds
				if (extension == L".dds")
				{
					result = DirectX::CreateDDSTextureFromFile(
						SGFramework::Administrator::GraphicsX11::m_device,
						name().c_str(), &textureResource, &m_shaderResourceView);

					//失敗ならthrow
					if (FAILED(result))
					{
						setIsOpenFailed(true);
						throw InvalidArgument(L"Error!! TextureAssetX11->Open",
							L"CreateDDSTextureFromFile failed", result);
					}
				}
				//tga
				else if (extension == L".tga")
				{
					DirectX::TexMetadata metaData;
					UniquePointer<DirectX::ScratchImage> image(
						Pointer::MakeUnique::Auto<DirectX::ScratchImage>());

					DirectX::GetMetadataFromTGAFile(name().c_str(), metaData);
					result = DirectX::LoadFromTGAFile(name().c_str(), &metaData, *image);

					//失敗ならthrow
					if (FAILED(result))
					{
						setIsOpenFailed(true);
						throw InvalidArgument(L"Error!! TextureAssetX11->Open",
							L"LoadFromTGAFile failed", result);
					}

					result = DirectX::CreateShaderResourceView(SGFramework::Administrator::GraphicsX11::m_device,
						image->GetImages(), image->GetImageCount(), metaData, &m_shaderResourceView);

					//失敗ならthrow
					if (FAILED(result))
					{
						setIsOpenFailed(true);
						throw InvalidArgument(L"Error!! TextureAssetX11->Open",
							L"CreateShaderResourceView failed", result);
					}
				}
				//other
				else
				{
					result = DirectX::CreateWICTextureFromFile(SGFramework::Administrator::GraphicsX11::m_device,
						name().c_str(), &textureResource, &m_shaderResourceView);

					//失敗ならthrow
					if (FAILED(result))
					{
						setIsOpenFailed(true);
						throw InvalidArgument(L"Error!! TextureAssetX11->Open",
							L"CreateWICTextureFromFile failed", result);
					}
				}

				//設定
				m_samplerState = samplerState;

				//BaseAsset
				OpenBaseAsset();
			}

			//----------------------------------------------------------------------------------
			//[Close]
			//ファイルを閉じる
			inline void TextureAssetX11::Close()
			{
				if (IS_FALSE(getIsFileOpened())) return;
				
				//リソース開放
				COM_RELEASE(m_shaderResourceView);

				//BaseAsset
				CloseBaseAsset(getType(), false);
			}
			//----------------------------------------------------------------------------------
			//[Reload]
			//throw: ロードに失敗した場合
			//アセットのリロードを行う
			inline void TextureAssetX11::Reload()
			{
				if (IS_FALSE(getIsFileOpened())) return;
				
				//開放
				Close();

				//Open
				try
				{
					Open(m_samplerState);
				}
				//失敗ならthrow
				catch (...)
				{
					throw;
				}
			}
			//----------------------------------------------------------------------------------
			//[ChangeSamplerState]
			//SamplerStateを変更する, Graphics::defaultSamplerState以外は自動開放を行わない
			//引数1: samplerState, default = Graphics::defaultSamplerState
			inline void TextureAssetX11::ChangeSamplerState(ID3D11SamplerState * samplerState)
			{
				//nullでなければ代入
				m_samplerState = (samplerState != nullptr) ? samplerState : m_samplerState;
			}
		}
	}
}
#endif // !SGFRAMEWORK_HEADER_TEXTURE_ASSET_X11_HPP_