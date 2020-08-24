/*----------------------------------------------------------------------------------
DirectX11 PixelSHaderを扱うPixelShaderAssetX11 class
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_PIXEL_SHADER_ASSET_X11_HPP_
#define SGFRAMEWORK_HEADER_PIXEL_SHADER_ASSET_X11_HPP_
#include <d3d11.h>
#include "../GraphicsX11/GraphicsX11.hpp"
#include "ShaderAssetX11.hpp"

// Framework namespace
namespace SGFramework
{
	//フレームワークの管理をするAdministrator namespace
	namespace Administrator
	{
		class AssetManager;
		//GraphicsDetail namespace
		namespace GraphicsDetail
		{
			//DirectX11 PixelSHaderを扱うPixelShaderAssetX11 class
			class PixelShaderAssetX11 final : public SGFramework::BaseClass::BaseShaderAssetX11
			{
			public:
				friend class AssetManager;
				template <typename TAsset>
				friend SharedPointer<TAsset> Asset::InstantiateAsset(const sgstring& filePath, bool isRegisterSharedAsset);

				//----------------------------------------------------------------------------------
				//[デストラクタ]
				//シェーダーリソースの開放を行う
				inline ~PixelShaderAssetX11() override;

				//----------------------------------------------------------------------------------
				//[InstantiateDefaultShader3D]
				//return: 生成したデフォルトシェーダー
				static inline SharedPointer<PixelShaderAssetX11> InstantiateDefaultShader3D()
				{
					return SharedPointer<PixelShaderAssetX11>();
				}
				//----------------------------------------------------------------------------------
				//[InstantiateDefaultShader2D]
				//return: 生成したデフォルトシェーダー
				static inline SharedPointer<PixelShaderAssetX11> InstantiateDefaultShader2D()
				{
					return SharedPointer<PixelShaderAssetX11>();
				}

				//----------------------------------------------------------------------------------
				//[Open]
				//シェーダーをロードする
				//引数1: シェーダーモデル, default = "ps_5_0"
				//引数2: エントリーポイント, default = "main"
				inline void Open(const std::string& shaderModel = "ps_5_0", const std::string& entryPoint = "main");
				//----------------------------------------------------------------------------------
				//[Reload]
				//アセットのリロードを行う
				inline void Reload() override;
				//----------------------------------------------------------------------------------
				//[Close]
				//ファイルを閉じる
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
				//[コンストラクタ]
				//初期化を行う
				inline PixelShaderAssetX11();

				//----------------------------------------------------------------------------------
				//[ReleaseShader]
				//シェーダーを開放する
				void ReleaseShader() override;

				//シェーダー
				ID3D11PixelShader* m_pixelShader = nullptr;
				//Reload用
				std::string m_shaderModel;
				//Reload用
				std::string m_entryPoint;
			};

			//----------------------------------------------------------------------------------
			//[デストラクタ]
			//シェーダーリソースの開放を行う
			inline PixelShaderAssetX11::~PixelShaderAssetX11()
			{
				ReleaseShader();
				CloseBaseAsset(getType(), true);
			}

			//----------------------------------------------------------------------------------
			//[Open]
			//シェーダーをロードする
			//引数1: シェーダーモデル, default = "ps_5_0"
			//引数2: エントリーポイント, default = "main"
			inline void PixelShaderAssetX11::Open(const std::string& shaderModel, const std::string& entryPoint)
			{
				if (IS_TRUE(getIsFileOpened())) return;

				setIsOpenFailed(false);

				//初期化
				m_pixelShader = nullptr;
				m_shaderModel = shaderModel;
				m_entryPoint = entryPoint;


				try
				{
					//シェーダーコンパイル
					Compile(name(), m_shaderModel, m_entryPoint);

					//作成
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
			//アセットのリロードを行う
			inline void PixelShaderAssetX11::Reload()
			{
				if (IS_FALSE(getIsFileOpened())) return;
				
				//まず開放
				ReleaseShader();

				try
				{
					//シェーダーコンパイル
					Compile(name, m_shaderModel, m_entryPoint);

					//作成
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

				//リロード
				ReloadBaseAsset();
			}

			//----------------------------------------------------------------------------------
			//[Close]
			//ファイルを閉じる
			inline void PixelShaderAssetX11::Close()
			{
				//シェーダー開放
				ReleaseShader();
				//BaseAsset
				CloseBaseAsset(getType(), false);
			}

			//----------------------------------------------------------------------------------
			//[コンストラクタ]
			//初期化を行う
			inline PixelShaderAssetX11::PixelShaderAssetX11() : BaseShaderAssetX11() {}

			//----------------------------------------------------------------------------------
			//[コンストラクタ]
			//シェーダーをロードする
			//引数1: シェーダーのファイルパス
			//引数2: シェーダーモデル, default = "ps_5_0"
			//引数3: エントリーポイント, default = "main"
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