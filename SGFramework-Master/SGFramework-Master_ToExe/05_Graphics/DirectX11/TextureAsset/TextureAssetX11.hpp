/*----------------------------------------------------------------------------------
DirectX11 Texture������BaseTextureAssetX11 class
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

	//�t���[�����[�N�̊Ǘ�������Administrator namespace
	namespace Administrator
	{
		class AssetManager;

		//GraphicsDetail namespace
		namespace GraphicsDetail
		{
			class ModelAssetX11;
			class MaterialX11;

			//DirectX11 Texture������TextureAssetX11 class
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
				//[�f�X�g���N�^]
				//���\�[�X�̊J�����s��
				inline ~TextureAssetX11() override { COM_RELEASE(m_shaderResourceView); CloseBaseAsset(getType(), true); }

				//----------------------------------------------------------------------------------
				//[Open]
				//�V�F�[�_�[�����[�h����
				//throw: ���[�h�Ɏ��s�����ꍇ
				//����1: samplerState, default = Graphics::defaultSamplerState
				inline virtual void Open(ID3D11SamplerState* samplerState = Administrator::GraphicsX11::m_defaultSamplerState);
				//----------------------------------------------------------------------------------
				//[Reload]
				//�A�Z�b�g�̃����[�h���s��
				//throw: ���[�h�Ɏ��s�����ꍇ
				inline void Reload() override;
				//----------------------------------------------------------------------------------
				//[Close]
				//�t�@�C�������
				inline void Close() override;

				//----------------------------------------------------------------------------------
				//[ChangeSamplerState]
				//SamplerState��ύX����, null�|�C���^�̏ꍇ�ύX���s��Ȃ�
				//Graphics::defaultSamplerState�ȊO�͎����J�����s��Ȃ��̂Œ���
				//����1: samplerState, default = Graphics::defaultSamplerState
				void ChangeSamplerState(ID3D11SamplerState* samplerState
					= Administrator::GraphicsX11::m_defaultSamplerState);

				//get asset type (read function property)
				SGF_FUNCTION_PROPERTY Asset::AssetType getType() override { return Asset::AssetType::Texture; }

				//set texture graphics (draw function only property)
				//argument 1 : slot, defualt = 0
				SGF_FUNCTION_PROPERTY void setTexture(uint slot = 0)
				{
					//�V�F�[�_�[���\�[�X�r���[null�łȂ���΃e�N�X�`���Z�b�g
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
			//�V�F�[�_�[�����[�h����
			//throw: ���[�h�Ɏ��s�����ꍇ
			//����1: �e�N�X�`���̃t�@�C���p�X
			//����2: samplerState, default = Graphics::defaultSamplerState
			inline void TextureAssetX11::Open(ID3D11SamplerState * samplerState)
			{
				if (IS_TRUE(getIsFileOpened())) return;
			
				setIsOpenFailed(false);
				
				ID3D11Resource* textureResource = nullptr;
				HRESULT result = S_OK;

				//�g���q�擾
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

					//���s�Ȃ�throw
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

					//���s�Ȃ�throw
					if (FAILED(result))
					{
						setIsOpenFailed(true);
						throw InvalidArgument(L"Error!! TextureAssetX11->Open",
							L"LoadFromTGAFile failed", result);
					}

					result = DirectX::CreateShaderResourceView(SGFramework::Administrator::GraphicsX11::m_device,
						image->GetImages(), image->GetImageCount(), metaData, &m_shaderResourceView);

					//���s�Ȃ�throw
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

					//���s�Ȃ�throw
					if (FAILED(result))
					{
						setIsOpenFailed(true);
						throw InvalidArgument(L"Error!! TextureAssetX11->Open",
							L"CreateWICTextureFromFile failed", result);
					}
				}

				//�ݒ�
				m_samplerState = samplerState;

				//BaseAsset
				OpenBaseAsset();
			}

			//----------------------------------------------------------------------------------
			//[Close]
			//�t�@�C�������
			inline void TextureAssetX11::Close()
			{
				if (IS_FALSE(getIsFileOpened())) return;
				
				//���\�[�X�J��
				COM_RELEASE(m_shaderResourceView);

				//BaseAsset
				CloseBaseAsset(getType(), false);
			}
			//----------------------------------------------------------------------------------
			//[Reload]
			//throw: ���[�h�Ɏ��s�����ꍇ
			//�A�Z�b�g�̃����[�h���s��
			inline void TextureAssetX11::Reload()
			{
				if (IS_FALSE(getIsFileOpened())) return;
				
				//�J��
				Close();

				//Open
				try
				{
					Open(m_samplerState);
				}
				//���s�Ȃ�throw
				catch (...)
				{
					throw;
				}
			}
			//----------------------------------------------------------------------------------
			//[ChangeSamplerState]
			//SamplerState��ύX����, Graphics::defaultSamplerState�ȊO�͎����J�����s��Ȃ�
			//����1: samplerState, default = Graphics::defaultSamplerState
			inline void TextureAssetX11::ChangeSamplerState(ID3D11SamplerState * samplerState)
			{
				//null�łȂ���Α��
				m_samplerState = (samplerState != nullptr) ? samplerState : m_samplerState;
			}
		}
	}
}
#endif // !SGFRAMEWORK_HEADER_TEXTURE_ASSET_X11_HPP_