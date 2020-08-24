/*----------------------------------------------------------------------------------
DirectX11 PixelSHader������PixelShaderAssetX11 class
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_PIXEL_SHADER_ASSET_X11_HPP_
#define SGFRAMEWORK_HEADER_PIXEL_SHADER_ASSET_X11_HPP_
#include <d3d11.h>
#include "../GraphicsX11/GraphicsX11.hpp"
#include "ShaderAssetX11.hpp"

// Framework namespace
namespace SGFramework
{
	//�t���[�����[�N�̊Ǘ�������Administrator namespace
	namespace Administrator
	{
		class AssetManager;
		//GraphicsDetail namespace
		namespace GraphicsDetail
		{
			//DirectX11 PixelSHader������PixelShaderAssetX11 class
			class PixelShaderAssetX11 final : public SGFramework::BaseClass::BaseShaderAssetX11
			{
			public:
				friend class AssetManager;
				template <typename TAsset>
				friend SharedPointer<TAsset> Asset::InstantiateAsset(const sgstring& filePath, bool isRegisterSharedAsset);

				//----------------------------------------------------------------------------------
				//[�f�X�g���N�^]
				//�V�F�[�_�[���\�[�X�̊J�����s��
				inline ~PixelShaderAssetX11() override;

				//----------------------------------------------------------------------------------
				//[InstantiateDefaultShader3D]
				//return: ���������f�t�H���g�V�F�[�_�[
				static inline SharedPointer<PixelShaderAssetX11> InstantiateDefaultShader3D()
				{
					return SharedPointer<PixelShaderAssetX11>();
				}
				//----------------------------------------------------------------------------------
				//[InstantiateDefaultShader2D]
				//return: ���������f�t�H���g�V�F�[�_�[
				static inline SharedPointer<PixelShaderAssetX11> InstantiateDefaultShader2D()
				{
					return SharedPointer<PixelShaderAssetX11>();
				}

				//----------------------------------------------------------------------------------
				//[Open]
				//�V�F�[�_�[�����[�h����
				//����1: �V�F�[�_�[���f��, default = "ps_5_0"
				//����2: �G���g���[�|�C���g, default = "main"
				inline void Open(const std::string& shaderModel = "ps_5_0", const std::string& entryPoint = "main");
				//----------------------------------------------------------------------------------
				//[Reload]
				//�A�Z�b�g�̃����[�h���s��
				inline void Reload() override;
				//----------------------------------------------------------------------------------
				//[Close]
				//�t�@�C�������
				inline void Close() override;

				//get asset type (read function property)
				SGF_FUNCTION_PROPERTY Asset::AssetType getType() override { return Asset::AssetType::PixelShader; }

				//set shader graphics (draw function only property)
				SGF_FUNCTION_PROPERTY void setShader() override
				{
					using UseGraphics = SGFramework::Administrator::GraphicsX11;
					if (UseGraphics::m_nowShaderInstanceIDs[UseGraphics::m_cTypePixelShader] != instanceID())
					{
						UseGraphics::context->PSSetShader(m_pixelShader, nullptr, 0);
						UseGraphics::m_nowShaderInstanceIDs[UseGraphics::m_cTypePixelShader] = instanceID();
					}
				}

			private:
				//----------------------------------------------------------------------------------
				//[�R���X�g���N�^]
				//���������s��
				inline PixelShaderAssetX11();

				//----------------------------------------------------------------------------------
				//[ReleaseShader]
				//�V�F�[�_�[���J������
				void ReleaseShader() override;

				//�V�F�[�_�[
				ID3D11PixelShader* m_pixelShader = nullptr;
				//Reload�p
				std::string m_shaderModel;
				//Reload�p
				std::string m_entryPoint;
			};

			//----------------------------------------------------------------------------------
			//[�f�X�g���N�^]
			//�V�F�[�_�[���\�[�X�̊J�����s��
			inline PixelShaderAssetX11::~PixelShaderAssetX11()
			{
				ReleaseShader();
				CloseBaseAsset(getType(), true);
			}

			//----------------------------------------------------------------------------------
			//[Open]
			//�V�F�[�_�[�����[�h����
			//����1: �V�F�[�_�[���f��, default = "ps_5_0"
			//����2: �G���g���[�|�C���g, default = "main"
			inline void PixelShaderAssetX11::Open(const std::string& shaderModel, const std::string& entryPoint)
			{
				if (IS_TRUE(getIsFileOpened())) return;

				setIsOpenFailed(false);

				//������
				m_pixelShader = nullptr;
				m_shaderModel = shaderModel;
				m_entryPoint = entryPoint;


				try
				{
					//�V�F�[�_�[�R���p�C��
					Compile(name(), m_shaderModel, m_entryPoint);

					//�쐬
					auto result = SGFramework::Administrator::GraphicsX11::device->CreatePixelShader(
						m_blob ? m_blob->GetBufferPointer() : m_byteData.data(),
						m_blob ? m_blob->GetBufferSize() : m_byteData.size(),
						nullptr, &m_pixelShader);

					if (FAILED(result))
					{
						setIsOpenFailed(true);
						COM_RELEASE(m_blob);
						throw InvalidArgument(L"Error!! VertexShaderAssetX11->Open", L"CreateVertexShader	failed", result);
					}
				}
				catch (...)
				{
					throw;
				}

				//BaseAsset
				OpenBaseAsset();
			}

			//----------------------------------------------------------------------------------
			//[Reload]
			//�A�Z�b�g�̃����[�h���s��
			inline void PixelShaderAssetX11::Reload()
			{
				if (IS_FALSE(getIsFileOpened())) return;
				
				//�܂��J��
				ReleaseShader();

				try
				{
					//�V�F�[�_�[�R���p�C��
					Compile(name, m_shaderModel, m_entryPoint);

					//�쐬
					auto result = SGFramework::Administrator::GraphicsX11::device->CreatePixelShader(
						m_blob ? m_blob->GetBufferPointer() : m_byteData.data(),
						m_blob ? m_blob->GetBufferSize() : m_byteData.size(),
						nullptr, &m_pixelShader);

					if (FAILED(result))
					{
						setIsOpenFailed(true);
						COM_RELEASE(m_blob);
						throw InvalidArgument(L"Error!! VertexShaderAssetX11->Open", L"CreateVertexShader	failed", result);
					}
				}
				catch (...)
				{
					throw;
				}

				//�����[�h
				ReloadBaseAsset();
			}

			//----------------------------------------------------------------------------------
			//[Close]
			//�t�@�C�������
			inline void PixelShaderAssetX11::Close()
			{
				//�V�F�[�_�[�J��
				ReleaseShader();
				//BaseAsset
				CloseBaseAsset(getType(), false);
			}

			//----------------------------------------------------------------------------------
			//[�R���X�g���N�^]
			//���������s��
			inline PixelShaderAssetX11::PixelShaderAssetX11() : BaseShaderAssetX11() {}

			//----------------------------------------------------------------------------------
			//[�R���X�g���N�^]
			//�V�F�[�_�[�����[�h����
			//����1: �V�F�[�_�[�̃t�@�C���p�X
			//����2: �V�F�[�_�[���f��, default = "ps_5_0"
			//����3: �G���g���[�|�C���g, default = "main"
			inline void PixelShaderAssetX11::ReleaseShader()
			{
				COM_RELEASE(m_pixelShader);
				COM_RELEASE(m_blob);

				m_byteData.clear();
			}
		}
	}
}
#endif // !SGFRAMEWORK_HEADER_PIXEL_SHADER_ASSET_X11_HPP_