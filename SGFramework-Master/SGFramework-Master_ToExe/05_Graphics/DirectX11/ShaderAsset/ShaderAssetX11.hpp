/*----------------------------------------------------------------------------------
DirectX11 ShaderAsset�̃x�[�X�ƂȂ�BaseShaderAssetX11 class
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_SHADER_ASSET_X11_HPP_
#define SGFRAMEWORK_HEADER_SHADER_ASSET_X11_HPP_
#include <type_traits>
#include <d3dcompiler.h>
#include "../../../04_Asset/FileStream/FileStream.hpp"
#include "../../../04_Asset/Asset/BaseAsset.hpp"

// Framework namespace
namespace SGFramework
{
	//Base Classes
	namespace BaseClass
	{
		//DirectX11 ShaderAsset�̃x�[�X�ƂȂ�BaseShaderAssetX11 class
		class BaseShaderAssetX11 : public BaseClass::BaseAsset
		{
		public:
			//default
			inline virtual ~BaseShaderAssetX11() override { }

			//----------------------------------------------------------------------------------
			//[Reload]
			//�A�Z�b�g�̃����[�h���s��
			virtual void Reload() = 0;
			//----------------------------------------------------------------------------------
			//[Close]
			//�t�@�C�������
			virtual void Close() = 0;
			
			//set shader graphics (draw function only property)
			SGF_FUNCTION_PROPERTY virtual void setShader() = 0;

		protected:
			//default
			inline BaseShaderAssetX11() : BaseAsset() {}
			//----------------------------------------------------------------------------------
			//[Compile]
			//�V�F�[�_�[���R���p�C������
			//����1: �t�@�C���l�[��
			//����2: blob
			//����3: �V�F�[�_�[���f��
			//����4: �G���g���[�|�C���g
			inline void Compile(const sgstring& filePath, const std::string& shaderModel, const std::string& entryPoint = "main");

			//----------------------------------------------------------------------------------
			//[ReleaseShader]
			//�V�F�[�_�[���J������
			virtual void ReleaseShader() = 0;

			static constexpr uint m_cReadBlob = 0x1;
			static constexpr uint m_cReadVector = 0x2;

			//blob
			ID3DBlob* m_blob = nullptr;
			//byteData
			std::vector<byte> m_byteData;
		};

		//----------------------------------------------------------------------------------
		//[Compile]
		//�V�F�[�_�[���R���p�C������
		//����1: �t�@�C���l�[��
		//����2: blob
		//����3: �V�F�[�_�[���f��
		//����4: �G���g���[�|�C���g
		inline void BaseShaderAssetX11::Compile(const sgstring& filePath, const std::string& shaderModel, const std::string& entryPoint)
		{
			sgstring find = filePath.my_functions.FindExtension();

			//read cso
			if (find == L".cso")
			{
				File::FileReadStream read;
				
				try { read.Open(filePath, true, true); }
				catch (...) { throw; }

				std::string readData(read.ReadStd());
				
				for (auto e : readData)
					m_byteData.emplace_back(static_cast<byte>(e));
			}
			//hlsl or fx
			else if (find == L".hlsl" || find == L"fx")
			{
				//�G���[�p
				ID3DBlob* error = nullptr;

				//�t���O
				uint flag = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef DEBUG
				flag |= D3DCOMPILE_DEBUG;
#endif // DEBUG

				//�R���p�C��
				auto result = D3DCompileFromFile(filePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
					entryPoint.c_str(), shaderModel.c_str(), flag, 0, &m_blob, &error);

				//���s
				if (FAILED(result))
				{
					InvalidArgument errorResult = InvalidArgument(L"Error!! ShaderAssetX11->Compile", (error->GetBufferPointer()) ? reinterpret_cast<wchar*>(error->GetBufferPointer()) : 0);
					error->Release();
					throw errorResult;
				}

				//�J��
				COM_RELEASE(error);
			}
			else
				throw InvalidArgument(L"Error!! ShaderAssetX11->Compile", 
					L"File format Only .cso or .hlsl or .fx are supported");
		}
	}
}

#endif // !SGFRAMEWORK_HEADER_SHADER_ASSET_X11_HPP_