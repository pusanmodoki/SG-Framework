/*----------------------------------------------------------------------------------
DirectX11 VertexSHaderを扱うVertexShaderAssetX11 class
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_VERTEX_SHADER_ASSET_X11_HPP_
#define SGFRAMEWORK_HEADER_VERTEX_SHADER_ASSET_X11_HPP_
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
			//DirectX11 VertexSHaderを扱うVertexShaderAssetX11 class
			class VertexShaderAssetX11 final : public SGFramework::BaseClass::BaseShaderAssetX11
			{
			public:
				friend class AssetManager;
				template <typename TAsset>
				friend SharedPointer<TAsset> Asset::InstantiateAsset(const sgstring& filePath, bool isRegisterSharedAsset);

				//D3D11_INPUT_ELEMENT_DESCにコンストラクタを追加したInputElement struct
				struct InputElement : public D3D11_INPUT_ELEMENT_DESC
				{
					//----------------------------------------------------------------------------------
					//[コンストラクタ]
					//初期化を行う
					//引数1: セマンティック名
					//引数2: セマンティックインデックス
					//引数3: セマンティックインデックス
					//引数4: インプットスロット, default = 0
					//引数5: オフセット, default = D3D11_APPEND_ALIGNED_ELEMENT
					//引数6: インプットスロットクラス, default = D3D11_INPUT_PER_VERTEX_DATA
					//引数7: インスタンスデータセパレート, default = 0
					inline InputElement(const std::string& semanticName, uint semanticIndex, DXGI_FORMAT format, uint inputSlot = 0,
						uint alignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT,
						D3D11_INPUT_CLASSIFICATION inputSlotClass = D3D11_INPUT_PER_VERTEX_DATA,
						uint instanceDataStepRate = 0);

					InputElement(const InputElement&) = default;
					InputElement(InputElement&&) = default;
					InputElement& operator =(InputElement&&) = default;
					InputElement& operator =(const InputElement&) = default;
				};

				//----------------------------------------------------------------------------------
				//[デストラクタ]
				//シェーダーリソースの開放を行う
				inline ~VertexShaderAssetX11() override;

				//----------------------------------------------------------------------------------
				//[InstantiateDefaultShader3D]
				//return: 生成したデフォルトシェーダー
				static inline SharedPointer<VertexShaderAssetX11> InstantiateDefaultShader3D()
				{
					return SharedPointer<VertexShaderAssetX11>();
				}
				//----------------------------------------------------------------------------------
				//[InstantiateDefaultShader2D]
				//return: 生成したデフォルトシェーダー
				static inline SharedPointer<VertexShaderAssetX11> InstantiateDefaultShader2D()
				{
					return SharedPointer<VertexShaderAssetX11>();
				}

				//----------------------------------------------------------------------------------
				//[Open]
				//シェーダーをロードする
				//引数1: INPUT_ELEMENT
				//引数2: シェーダーモデル, default = "vs_5_0"
				//引数3: エントリーポイント, default = "main"
				inline void Open(const std::vector<InputElement>& inputElements,
					const std::string& shaderModel = "vs_5_0", const std::string& entryPoint = "main");
				//----------------------------------------------------------------------------------
				//[Reload]
				//アセットのリロードを行う
				inline void Reload() override;
				//----------------------------------------------------------------------------------
				//[Close]
				//ファイルを閉じる
				inline void Close() override;

				//get asset type (read function property)
				SGF_FUNCTION_PROPERTY Asset::AssetType getType() override { return Asset::AssetType::VertexShader; }

				//set shader graphics (draw function only property)
				SGF_FUNCTION_PROPERTY void setShader() override
				{
					using UseGraphics = SGFramework::Administrator::GraphicsX11;
					if (UseGraphics::m_nowShaderInstanceIDs[UseGraphics::m_cTypeVertexShader] != instanceID())
					{
						SGFramework::Administrator::GraphicsX11::context->IASetInputLayout(m_vertexLayout);
						SGFramework::Administrator::GraphicsX11::context->VSSetShader(m_vertexShader, nullptr, 0);
						UseGraphics::m_nowShaderInstanceIDs[UseGraphics::m_cTypeVertexShader] = instanceID();
					}
				}

				//----------------------------------------------------------------------------------
				//[CreateInputElements]
				//Openで使用するInputElementのvectorを作成する
				//return: 作成したvector
				//引数Pack: VSInputElement or VertexShader::InputElementコンストラクタ
				template<class ...Args>
				inline static std::vector<InputElement> CreateInputElements(const Args&... args);

				//vertex->imgui input elements(static) (read only property)
				SGF_FUNCTION_PROPERTY static std::vector<InputElement> getImGuiVertexInputElements();
				//vertex->model input elements(static) (read only property)
				SGF_FUNCTION_PROPERTY static std::vector<InputElement> getModelVertexInputElements();
				//vertex->polygon input elements(static) (read only property)
				SGF_FUNCTION_PROPERTY static std::vector<InputElement> getPolygonVertexInputElements();

			private:
				//----------------------------------------------------------------------------------
				//[コンストラクタ]
				//初期化を行う
				inline VertexShaderAssetX11() : BaseShaderAssetX11() {}

				//----------------------------------------------------------------------------------
				//[ReleaseShader]
				//シェーダーを開放する
				void ReleaseShader() override;

				ID3D11VertexShader* m_vertexShader = nullptr;				//シェーダー
				ID3D11InputLayout*  m_vertexLayout = nullptr;				//シェーダーレイアウト
				//Reload用
				std::vector<VertexShaderAssetX11::InputElement> m_inputElements;
				//Reload用
				std::string m_shaderModel;
				//Reload用
				std::string m_entryPoint;
			};

			//----------------------------------------------------------------------------------
			//[コンストラクタ]
			//初期化を行う
			//引数1: セマンティック名
			//引数2: セマンティックインデックス
			//引数3: セマンティックインデックス
			//引数4: インプットスロット, default = 0
			//引数5: オフセット, default = D3D11_APPEND_ALIGNED_ELEMENT
			//引数6: インプットスロットクラス, default = D3D11_INPUT_PER_VERTEX_DATA
			//引数7: インスタンスデータセパレート, default = 0
			inline VertexShaderAssetX11::InputElement::InputElement(const std::string& semanticName, uint semanticIndex, DXGI_FORMAT format, uint inputSlot,
				uint alignedByteOffset, D3D11_INPUT_CLASSIFICATION inputSlotClass, uint instanceDataStepRate)
			{
				SemanticName = semanticName.c_str();
				SemanticIndex = semanticIndex;
				Format = format;
				InputSlot = inputSlot;
				AlignedByteOffset = alignedByteOffset;
				InputSlotClass = inputSlotClass;
				InstanceDataStepRate = instanceDataStepRate;
			}

			//----------------------------------------------------------------------------------
			//[デストラクタ]
			//シェーダーリソースの開放を行う
			inline VertexShaderAssetX11::~VertexShaderAssetX11()
			{
				ReleaseShader();
				CloseBaseAsset(getType(), true);
			}

			//----------------------------------------------------------------------------------
			//[Open]
			//シェーダーをロードする
			//引数1: INPUT_ELEMENT
			//引数2: シェーダーモデル, default = "vs_5_0"
			//引数3: エントリーポイント, default = "main"
			inline void VertexShaderAssetX11::Open(const std::vector<VertexShaderAssetX11::InputElement>& inputElements,
				const std::string& shaderModel, const std::string& entryPoint)
			{
				if (IS_TRUE(getIsFileOpened())) return;

				setIsOpenFailed(false);

				//初期化
				m_vertexShader = nullptr;
				m_vertexLayout = nullptr;
				m_inputElements = std::move(inputElements);
				m_shaderModel = shaderModel;
				m_entryPoint = entryPoint;


				try
				{
					//シェーダーコンパイル
					Compile(name(), m_shaderModel, m_entryPoint);

					//作成
					auto result = SGFramework::Administrator::GraphicsX11::device->CreateVertexShader(
						m_blob ? m_blob->GetBufferPointer() : m_byteData.data(),
						m_blob ? m_blob->GetBufferSize() : m_byteData.size(),
						nullptr, &m_vertexShader);

					if (FAILED(result))
					{
						setIsOpenFailed(true);
						COM_RELEASE(m_blob);
						throw InvalidArgument(L"Error!! VertexShaderAssetX11->Open", L"CreateVertexShader	failed", result);
					}

					//作成
					result = SGFramework::Administrator::GraphicsX11::device->CreateInputLayout(
						m_inputElements.data(), static_cast<uint>(m_inputElements.size()),
						m_blob ? m_blob->GetBufferPointer() : m_byteData.data(),
						static_cast<uint>(m_blob ? m_blob->GetBufferSize() : m_byteData.size()),
						&m_vertexLayout);

					if (FAILED(result))
					{
						setIsOpenFailed(true);
						COM_RELEASE(m_blob);
						throw InvalidArgument(L"Error!! VertexShaderAssetX11->Open", L"CreateInputLayout failed", result);
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
			inline void VertexShaderAssetX11::Reload()
			{
				if (IS_FALSE(getIsFileOpened())) return;
			
				//まず開放
				ReleaseShader();

				try
				{
					//シェーダーコンパイル
					Compile(name, m_shaderModel, m_entryPoint);

					//作成
					auto result = SGFramework::Administrator::GraphicsX11::device->CreateVertexShader(
						m_blob ? m_blob->GetBufferPointer() : m_byteData.data(),
						m_blob ? m_blob->GetBufferSize() : m_byteData.size(),
						nullptr, &m_vertexShader);

					if (FAILED(result))
					{
						setIsOpenFailed(true);
						COM_RELEASE(m_blob);
						throw InvalidArgument(L"Error!! VertexShaderAssetX11->Open", L"CreateVertexShader	failed", result);
					}

					//作成
					result = SGFramework::Administrator::GraphicsX11::device->CreateInputLayout(
						m_inputElements.data(), static_cast<uint>(m_inputElements.size()),
						m_blob ? m_blob->GetBufferPointer() : m_byteData.data(), 
						static_cast<uint>(m_blob ? m_blob->GetBufferSize() : m_byteData.size()), 
						&m_vertexLayout);

					if (FAILED(result))
					{
						setIsOpenFailed(true);
						COM_RELEASE(m_blob);
						throw InvalidArgument(L"Error!! VertexShaderAssetX11->Open", L"CreateInputLayout failed", result);
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
			inline void VertexShaderAssetX11::Close()
			{
				if (IS_FALSE(getIsFileOpened())) return;
				
				//シェーダー開放
				ReleaseShader();
				//BaseAsset
				CloseBaseAsset(getType(), false);
			}

			//----------------------------------------------------------------------------------
			//[CreateInputElements]
			//Openで使用するInputElementのvectorを作成する
			//return: 作成したvector
			//引数Pack: VSInputElement or VertexShader::InputElementコンストラクタ
			template<class ...Args>
			inline static std::vector<VertexShaderAssetX11::InputElement> VertexShaderAssetX11::CreateInputElements(const Args&... args)
			{
				std::vector<InputElement> result;
				for (const auto& e : { args... })
					result.emplace_back(e);
				return std::move(result);
			}

			//vertex->imgui input elements(static) (read only property)
			SGF_FUNCTION_PROPERTY std::vector<VertexShaderAssetX11::InputElement> VertexShaderAssetX11::getImGuiVertexInputElements()
			{
				static std::vector<InputElement> result;
				static const char* names[] = { "POSITION", "TEXCOORD", "COLOR"};
				static std::atomic_bool lock = false;

				AtomicOperation::LockGuard guard(lock);
				if (IS_TRUE(result.empty()))
				{
					result.emplace_back(InputElement("", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, (size_t)(&((ImDrawVert*)0)->pos)));
					result.emplace_back(InputElement("", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, (size_t)(&((ImDrawVert*)0)->uv)));
					result.emplace_back(InputElement("", 0, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, 0, (size_t)(&((ImDrawVert*)0)->col)));
					for (uint i = 0; i < static_cast<uint>(result.size()); ++i)
						result[i].SemanticName = names[i];
				}

				return result;
			}

			//vertex->model input elements(static) (read only property)
			SGF_FUNCTION_PROPERTY std::vector<VertexShaderAssetX11::InputElement> VertexShaderAssetX11::getModelVertexInputElements()
			{
				static std::vector<InputElement> result;
				static const char* names[] = { "POSITION", "NORMAL", "TEXCOORD", "TEXCOORD",
					"BLENDWEIGHT", "BLENDWEIGHT", "BLENDINDICES","BLENDINDICES" };
				static constexpr int cPosition = 0, cNormal = 1, cUV0 = 2, cUV1 = 3, cWeight0 = 4, 
					cWeight1 = 5, cBoneIndex0 = 6, cBoneIndex1 = 7;
				static std::atomic_bool lock = false;

				AtomicOperation::LockGuard guard(lock);
				if (IS_TRUE(result.empty()))
				{
					result.emplace_back(InputElement(names[cPosition], 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT));
					result.emplace_back(InputElement(names[cNormal], 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT));
					result.emplace_back(InputElement(names[cUV0], 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT));
					result.emplace_back(InputElement(names[cUV1], 1, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT));
					result.emplace_back(InputElement(names[cWeight0], 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT));
					result.emplace_back(InputElement(names[cWeight1], 1, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT));
					result.emplace_back(InputElement(names[cBoneIndex0], 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_SINT));
					result.emplace_back(InputElement(names[cBoneIndex1], 1, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_SINT));
		
					for (uint i = 0; i < static_cast<uint>(result.size()); ++i)
						result[i].SemanticName = names[i];
				}

				return result;
			}

			//vertex->polygon input elements(static) (read only property)
			SGF_FUNCTION_PROPERTY std::vector<VertexShaderAssetX11::InputElement> VertexShaderAssetX11::getPolygonVertexInputElements()
			{
				static std::vector<InputElement> result;
				static const char* names[] = { "POSITION", "COLOR", "TEXCOORD" };
				static std::atomic_bool lock = false;

				AtomicOperation::LockGuard guard(lock);
				if (IS_TRUE(result.empty()))
				{
					result.emplace_back(InputElement("", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT));
					result.emplace_back(InputElement("", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT));
					result.emplace_back(InputElement("", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT));
					for (uint i = 0; i < static_cast<uint>(result.size()); ++i)
						result[i].SemanticName = names[i];
				}

				return result;
			}

			//----------------------------------------------------------------------------------
			//[ReleaseShader]
			//シェーダーを開放する
			inline void VertexShaderAssetX11::ReleaseShader()
			{
				COM_RELEASE(m_vertexShader);
				COM_RELEASE(m_vertexLayout);
				COM_RELEASE(m_blob);

				m_byteData.clear();
			}
		}
	}
}
#endif // !SGFRAMEWORK_HEADER_VERTEX_SHADER_ASSET_X11_HPP_