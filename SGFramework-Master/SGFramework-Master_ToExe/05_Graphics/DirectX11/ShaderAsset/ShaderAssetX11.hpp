/*----------------------------------------------------------------------------------
DirectX11 ShaderAssetのベースとなるBaseShaderAssetX11 class
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
		//DirectX11 ShaderAssetのベースとなるBaseShaderAssetX11 class
		class BaseShaderAssetX11 : public BaseClass::BaseAsset
		{
		public:
			//default
			inline virtual ~BaseShaderAssetX11() override { }

			//----------------------------------------------------------------------------------
			//[Reload]
			//アセットのリロードを行う
			virtual void Reload() = 0;
			//----------------------------------------------------------------------------------
			//[Close]
			//ファイルを閉じる
			virtual void Close() = 0;
			
			//set shader graphics (draw function only property)
			SGF_FUNCTION_PROPERTY virtual void setShader() = 0;

		protected:
			//default
			inline BaseShaderAssetX11() : BaseAsset() {}
			//----------------------------------------------------------------------------------
			//[Compile]
			//シェーダーをコンパイルする
			//引数1: ファイルネーム
			//引数2: blob
			//引数3: シェーダーモデル
			//引数4: エントリーポイント
			inline void Compile(const sgstring& filePath, const std::string& shaderModel, const std::string& entryPoint = "main");

			//----------------------------------------------------------------------------------
			//[ReleaseShader]
			//シェーダーを開放する
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
		//シェーダーをコンパイルする
		//引数1: ファイルネーム
		//引数2: blob
		//引数3: シェーダーモデル
		//引数4: エントリーポイント
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
				//エラー用
				ID3DBlob* error = nullptr;

				//フラグ
				uint flag = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef DEBUG
				flag |= D3DCOMPILE_DEBUG;
#endif // DEBUG

				//コンパイル
				auto result = D3DCompileFromFile(filePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
					entryPoint.c_str(), shaderModel.c_str(), flag, 0, &m_blob, &error);

				//失敗
				if (FAILED(result))
				{
					InvalidArgument errorResult = InvalidArgument(L"Error!! ShaderAssetX11->Compile", (error->GetBufferPointer()) ? reinterpret_cast<wchar*>(error->GetBufferPointer()) : 0);
					error->Release();
					throw errorResult;
				}

				//開放
				COM_RELEASE(error);
			}
			else
				throw InvalidArgument(L"Error!! ShaderAssetX11->Compile", 
					L"File format Only .cso or .hlsl or .fx are supported");
		}
	}
}

#endif // !SGFRAMEWORK_HEADER_SHADER_ASSET_X11_HPP_