/*----------------------------------------------------------------------------------
DirectX11 VertexSHader������VertexShaderAssetX11 class
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_VERTEX_SHADER_ASSET_X11_HPP_
#define SGFRAMEWORK_HEADER_VERTEX_SHADER_ASSET_X11_HPP_
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
			//DirectX11 VertexSHader������VertexShaderAssetX11 class
			class VertexShaderAssetX11 final : public SGFramework::BaseClass::BaseShaderAssetX11
			{
			public:
				friend class AssetManager;
				template <typename TAsset>
				friend SharedPointer<TAsset> Asset::InstantiateAsset(const sgstring& filePath, bool isRegisterSharedAsset);

				//D3D11_INPUT_ELEMENT_DESC�ɃR���X�g���N�^��ǉ�����InputElement struct
				struct InputElement : public D3D11_INPUT_ELEMENT_DESC
				{
					//----------------------------------------------------------------------------------
					//[�R���X�g���N�^]
					//���������s��
					//����1: �Z�}���e�B�b�N��
					//����2: �Z�}���e�B�b�N�C���f�b�N�X
					//����3: �Z�}���e�B�b�N�C���f�b�N�X
					//����4: �C���v�b�g�X���b�g, default = 0
					//����5: �I�t�Z�b�g, default = D3D11_APPEND_ALIGNED_ELEMENT
					//����6: �C���v�b�g�X���b�g�N���X, default = D3D11_INPUT_PER_VERTEX_DATA
					//����7: �C���X�^���X�f�[�^�Z�p���[�g, default = 0
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
				//[�f�X�g���N�^]
				//�V�F�[�_�[���\�[�X�̊J�����s��
				inline ~VertexShaderAssetX11() override;

				//----------------------------------------------------------------------------------
				//[InstantiateDefaultShader3D]
				//return: ���������f�t�H���g�V�F�[�_�[
				static inline SharedPointer<VertexShaderAssetX11> InstantiateDefaultShader3D()
				{
					return SharedPointer<VertexShaderAssetX11>();
				}
				//----------------------------------------------------------------------------------
				//[InstantiateDefaultShader2D]
				//return: ���������f�t�H���g�V�F�[�_�[
				static inline SharedPointer<VertexShaderAssetX11> InstantiateDefaultShader2D()
				{
					return SharedPointer<VertexShaderAssetX11>();
				}

				//----------------------------------------------------------------------------------
				//[Open]
				//�V�F�[�_�[�����[�h����
				//����1: INPUT_ELEMENT
				//����2: �V�F�[�_�[���f��, default = "vs_5_0"
				//����3: �G���g���[�|�C���g, default = "main"
				inline void Open(const std::vector<InputElement>& inputElements,
					const std::string& shaderModel = "vs_5_0", const std::string& entryPoint = "main");
				//----------------------------------------------------------------------------------
				//[Reload]
				//�A�Z�b�g�̃����[�h���s��
				inline void Reload() override;
				//----------------------------------------------------------------------------------
				//[Close]
				//�t�@�C�������
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
				//Open�Ŏg�p����InputElement��vector���쐬����
				//return: �쐬����vector
				//����Pack: VSInputElement or VertexShader::InputElement�R���X�g���N�^
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
				//[�R���X�g���N�^]
				//���������s��
				inline VertexShaderAssetX11() : BaseShaderAssetX11() {}

				//----------------------------------------------------------------------------------
				//[ReleaseShader]
				//�V�F�[�_�[���J������
				void ReleaseShader() override;

				ID3D11VertexShader* m_vertexShader = nullptr;				//�V�F�[�_�[
				ID3D11InputLayout*  m_vertexLayout = nullptr;				//�V�F�[�_�[���C�A�E�g
				//Reload�p
				std::vector<VertexShaderAssetX11::InputElement> m_inputElements;
				//Reload�p
				std::string m_shaderModel;
				//Reload�p
				std::string m_entryPoint;
			};

			//----------------------------------------------------------------------------------
			//[�R���X�g���N�^]
			//���������s��
			//����1: �Z�}���e�B�b�N��
			//����2: �Z�}���e�B�b�N�C���f�b�N�X
			//����3: �Z�}���e�B�b�N�C���f�b�N�X
			//����4: �C���v�b�g�X���b�g, default = 0
			//����5: �I�t�Z�b�g, default = D3D11_APPEND_ALIGNED_ELEMENT
			//����6: �C���v�b�g�X���b�g�N���X, default = D3D11_INPUT_PER_VERTEX_DATA
			//����7: �C���X�^���X�f�[�^�Z�p���[�g, default = 0
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
			//[�f�X�g���N�^]
			//�V�F�[�_�[���\�[�X�̊J�����s��
			inline VertexShaderAssetX11::~VertexShaderAssetX11()
			{
				ReleaseShader();
				CloseBaseAsset(getType(), true);
			}

			//----------------------------------------------------------------------------------
			//[Open]
			//�V�F�[�_�[�����[�h����
			//����1: INPUT_ELEMENT
			//����2: �V�F�[�_�[���f��, default = "vs_5_0"
			//����3: �G���g���[�|�C���g, default = "main"
			inline void VertexShaderAssetX11::Open(const std::vector<VertexShaderAssetX11::InputElement>& inputElements,
				const std::string& shaderModel, const std::string& entryPoint)
			{
				if (IS_TRUE(getIsFileOpened())) return;

				setIsOpenFailed(false);

				//������
				m_vertexShader = nullptr;
				m_vertexLayout = nullptr;
				m_inputElements = std::move(inputElements);
				m_shaderModel = shaderModel;
				m_entryPoint = entryPoint;


				try
				{
					//�V�F�[�_�[�R���p�C��
					Compile(name(), m_shaderModel, m_entryPoint);

					//�쐬
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

					//�쐬
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
			//�A�Z�b�g�̃����[�h���s��
			inline void VertexShaderAssetX11::Reload()
			{
				if (IS_FALSE(getIsFileOpened())) return;
			
				//�܂��J��
				ReleaseShader();

				try
				{
					//�V�F�[�_�[�R���p�C��
					Compile(name, m_shaderModel, m_entryPoint);

					//�쐬
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

					//�쐬
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


				//�����[�h
				ReloadBaseAsset();
			}

			//----------------------------------------------------------------------------------
			//[Close]
			//�t�@�C�������
			inline void VertexShaderAssetX11::Close()
			{
				if (IS_FALSE(getIsFileOpened())) return;
				
				//�V�F�[�_�[�J��
				ReleaseShader();
				//BaseAsset
				CloseBaseAsset(getType(), false);
			}

			//----------------------------------------------------------------------------------
			//[CreateInputElements]
			//Open�Ŏg�p����InputElement��vector���쐬����
			//return: �쐬����vector
			//����Pack: VSInputElement or VertexShader::InputElement�R���X�g���N�^
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
			//�V�F�[�_�[���J������
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