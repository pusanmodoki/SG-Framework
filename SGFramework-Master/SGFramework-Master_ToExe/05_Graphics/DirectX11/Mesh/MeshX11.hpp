#ifndef SGFRAMEWORK_HEADER_MESH_X11_HPP_
#define SGFRAMEWORK_HEADER_MESH_X11_HPP_
#include "../../../02_CustomVariable/CustomVariable.hpp"
#include "../TextureAsset/TextureAssetX11.hpp"
#include "../Material/MaterialX11.hpp"
#include "../GraphicsX11/GraphicsX11.hpp"
#include "../GraphicsX11/GraphicsX11ConstantBuffer.hpp"

// Framework namespace
namespace SGFramework
{
	//フレームワークの管理をするAdministrator namespace
	namespace Administrator
	{
		//GraphicsDetail namespace
		namespace GraphicsDetail
		{
			class ModelAssetX11;

			template<class VertexType, class MaterialPointerType>
			class MeshX11
			{
			public:
				friend class ModelAssetX11;
				
				inline void CreateBuffers()
				{
					if (m_vertexBuffer || m_indexBuffer) return;

					D3D11_BUFFER_DESC bufferDesc = {};
					D3D11_SUBRESOURCE_DATA resource = {};
					HRESULT result = S_OK;

					bufferDesc.Usage = D3D11_USAGE_DEFAULT;
					bufferDesc.ByteWidth = static_cast<uint>(sizeof(VertexType) * m_vertexes.size());
					bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
					bufferDesc.CPUAccessFlags = 0;

					resource.pSysMem = m_vertexes.data();

					result = Administrator::GraphicsX11::device->CreateBuffer(&bufferDesc, &resource, &m_vertexBuffer);
					if (FAILED(result))
						throw InvalidArgument(L"Error!! Mesh->CreateBuffers", L"CreateBuffer(vertex) failed", result);

					ZeroMemory(&bufferDesc, sizeof(bufferDesc));
					ZeroMemory(&resource, sizeof(resource));

					bufferDesc.Usage = D3D11_USAGE_DEFAULT;
					bufferDesc.ByteWidth = static_cast<uint>(sizeof(uint) * m_indexes.size());
					bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
					bufferDesc.CPUAccessFlags = 0;

					resource.pSysMem = m_indexes.data();

					result = Administrator::GraphicsX11::device->CreateBuffer(&bufferDesc, &resource, &m_indexBuffer);
					if (FAILED(result))
					{
						COM_RELEASE(m_vertexBuffer);
						throw InvalidArgument(L"Error!! Mesh->CreateBuffers", L"CreateBuffer(index) failed", result);
					}
				}
				inline void ReleaseBuffers()
				{
					COM_RELEASE(m_vertexBuffer);
					COM_RELEASE(m_indexBuffer);
				}

				inline void Draw()
				{
					uint stride = static_cast<uint>(sizeof(VertexType));
					uint offset = 0;

					m_material->setMaterial();
		
					Administrator::GraphicsX11::context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
					Administrator::GraphicsX11::context->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
					Administrator::GraphicsX11::context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

					Administrator::GraphicsX11::context->DrawIndexed(static_cast<uint>(m_indexes.size()), 0, 0);
				}


			private:
				std::vector<VertexType> m_vertexes;			//頂点データ
				std::vector<uint> m_indexes;						//インデックス
				MaterialPointerType m_material;					//マテリアル

				ID3D11Buffer* m_vertexBuffer = nullptr;		//Vertex buffer
				ID3D11Buffer* m_indexBuffer = nullptr;		//Index buffer
			};
		}
	}
}
#endif //!SGFRAMEWORK_HEADER_MESH_X11_HPP_