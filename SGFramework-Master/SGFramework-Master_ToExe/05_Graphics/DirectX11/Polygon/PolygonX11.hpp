#ifndef SGFRAMEWORK_HEADER_POLYGON_X11_HPP_
#define SGFRAMEWORK_HEADER_POLYGON_X11_HPP_
#include "../../../02_CustomVariable/CustomVariable.hpp"
#include "../Transform/TransformX11.hpp"
#include "../../Vertex/Vertex.hpp"
#include "../GraphicsObject/GraphicsObjectX11.hpp"
#include "../GraphicsX11/GraphicsX11.hpp"

//Framework namespace
namespace SGFramework
{
	//BaseClasses
	namespace BaseClass
	{
		//Polygon classのBaseとなるBasePolygonX11 class
		class BasePolygonX11 : Administrator::GraphicsDetail::GraphicsObjectX11
		{
		public:
			//COM_RELEASE destructor
			inline ~BasePolygonX11() { COM_RELEASE(m_vertexBuffer); }

			//vertex array write access
			AccessorPropertyNotify<std::array<Vertex::Polygon, 4>> vertexesWrite;
			//vertex array read access
			GetOnlyProperty<std::array<Vertex::Polygon, 4>> vertexesRead;
			
		protected:
			//reference set constructor
			BasePolygonX11() : vertexesWrite(m_vertexes, m_isChangedVertex), 
				vertexesRead(m_vertexes) {}

			//----------------------------------------------------------------------------------
			//[Sync]
			//情報を同期する, コールバック関数
			inline void Sync() override;
			//----------------------------------------------------------------------------------
			//[CreateVertex]
			//頂点バッファを作成する
			inline void CreateVertex();
	
			//Vertex buffer
			ID3D11Buffer* m_vertexBuffer = nullptr;
			//Vertex array
			std::array<Vertex::Polygon, 4> m_vertexes = {};
			//Vertex changed notify
			bool m_isChangedVertex = false;
		};

		//----------------------------------------------------------------------------------
		//[Sync]
		//情報を同期する, コールバック関数
		inline void BasePolygonX11::Sync()
		{
			//Changed? -> Update Buffer
			if (IS_TRUE(m_isChangedVertex))
			{
				D3D11_MAPPED_SUBRESOURCE data;

				HRESULT result = SGFramework::Administrator::GraphicsX11::m_deviceContext->Map(
					m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);

				if (SUCCEEDED(result))
				{
					memcpy_s(data.pData, data.RowPitch, m_vertexes.data(), sizeof(Vertex::Polygon) * 4);
					SGFramework::Administrator::GraphicsX11::m_deviceContext->Unmap(m_vertexBuffer, 0);
				}
				m_isChangedVertex = false;
			}
		}

		//----------------------------------------------------------------------------------
		//[CreateVertex]
		//頂点バッファを作成する
		inline void BasePolygonX11::CreateVertex()
		{
			//Init vertex position
			m_vertexes[0].position = Const::Vector3::leftTop;
			m_vertexes[1].position = Const::Vector3::rightTop;
			m_vertexes[2].position = Const::Vector3::leftDown;
			m_vertexes[3].position = Const::Vector3::rightDown;
			////Init vertex uv & normal map uv
			//m_vertexes[0].uv = m_vertexes[0].uvNormalMap = Const::UV::leftTop;
			//m_vertexes[1].uv = m_vertexes[1].uvNormalMap = Const::UV::rightTop;
			//m_vertexes[2].uv = m_vertexes[2].uvNormalMap = Const::UV::leftDown;
			//m_vertexes[3].uv = m_vertexes[3].uvNormalMap = Const::UV::rightDown;
			//Init vertex color
			for (auto& e : m_vertexes) e.color = Const::Color::white;

			//Create Buffer
			D3D11_BUFFER_DESC bufferDesc = {};
			bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			bufferDesc.ByteWidth = sizeof(Vertex::Polygon) * 4;
			bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			D3D11_SUBRESOURCE_DATA data = {};
			data.pSysMem = m_vertexes.data();

			auto result = SGFramework::Administrator::GraphicsX11::m_device->CreateBuffer(&bufferDesc,
				&data, &m_vertexBuffer);
			if (FAILED(result))
				throw InvalidArgument(L"Error!! PolygonSquareX11->Constructor", L"CreateBuffer failed", result);
		}
	}

	//フレームワークの管理をするAdministrator namespace
	namespace Administrator
	{
		//GraphicsDetail namespace
		namespace GraphicsDetail
		{
			class PolygonSquareX11 : GraphicsObjectX11
			{
			public:
				inline ~PolygonSquareX11();
				
				inline static SharedPointer<PolygonSquareX11> Instantiate(const Vector3& position = Const::Vector3::zero,
					const Vector3& scale = Const::Vector3::one, const Quaternion& rotation = Const::Quaternion::identity,
					const sgstring& name = L"PolygonSquare");				
				inline static SharedPointer<PolygonSquareX11> Instantiate(SharedPointer<TransformX11> parent, bool isInstantiateInWorldSpace,
					const Vector3& position = Const::Vector3::zero, const Vector3& scale = Const::Vector3::one, 
					const Quaternion& rotation = Const::Quaternion::identity, const sgstring& name = L"PolygonSquare");

				inline void SetPositionForAllVertexes(const Vector3& pos1 = Const::Vector3::leftTop, const Vector3& pos2 = Const::Vector3::rightTop, 
					const Vector3& pos3 = Const::Vector3::leftDown, const Vector3& pos4 = Const::Vector3::rightDown);
				inline void SetUVForAllVertexes(const UV& uv1 = Const::UV::leftTop, const UV& uv2 = Const::UV::rightTop, 
					const UV& uv3 = Const::UV::leftDown, const UV& uv4 = Const::UV::rightDown);
				inline void SetColorForAllVertexes(const Color& color = Const::Color::white);

				inline void SetPosition(uint index, const Vector3& pos);
				inline void SetUV(uint index, const UV& uv);
				inline void SetColor(uint index, const Color& color);

				inline const Vertex::Polygon& GetVertex(uint index);

				inline void Draw2D();
				inline void Draw3D();
				inline void Draw2DTryangle();

				WeakPointer<TransformX11> transform;
			private:
				inline PolygonSquareX11(const Vector3& position, const Vector3& scale, 
					const Quaternion& rotation, const sgstring& name);
				inline PolygonSquareX11(SharedPointer<TransformX11>& parent, bool isInstantiateInWorldSpace,
					const Vector3& position, const Vector3& scale, const Quaternion& rotation, const sgstring& name);
				inline void Sync() override;
				inline void Init();

				SharedPointer<TransformX11> m_transform;
				ID3D11Buffer* m_vertexBuffer;
				std::array<Vertex::Polygon, 4> m_vertexes;
				bool m_isChanged;
			};


			inline PolygonSquareX11::~PolygonSquareX11()
			{
				COM_RELEASE(m_vertexBuffer);
			}

			inline SharedPointer<PolygonSquareX11> PolygonSquareX11::Instantiate(const Vector3 & position,
				const Vector3 & scale, const Quaternion & rotation, const sgstring & name)
			{
				return std::move(SharedPointer<PolygonSquareX11>(new PolygonSquareX11(position, scale, rotation, name)));
			}
			inline SharedPointer<PolygonSquareX11> PolygonSquareX11::Instantiate(SharedPointer<TransformX11> parent, bool isInstantiateInWorldSpace,
				const Vector3 & position, const Vector3 & scale, const Quaternion & rotation, const sgstring & name)
			{
				return std::move(SharedPointer<PolygonSquareX11>(new PolygonSquareX11(parent, isInstantiateInWorldSpace,  position, scale, rotation, name)));
			}

			inline void PolygonSquareX11::SetPositionForAllVertexes(const Vector3 & pos1, const Vector3 & pos2, const Vector3 & pos3, const Vector3 & pos4)
			{
				m_vertexes[0].position = pos1;
				m_vertexes[1].position = pos2;
				m_vertexes[2].position = pos3;
				m_vertexes[3].position = pos4;
			}

			inline void PolygonSquareX11::SetUVForAllVertexes(const UV & uv1, const UV & uv2, const UV & uv3, const UV & uv4)
			{
				m_vertexes[0].uv = uv1;
				m_vertexes[1].uv = uv2;
				m_vertexes[2].uv = uv3;
				m_vertexes[3].uv = uv4;
			}

			inline void PolygonSquareX11::SetColorForAllVertexes(const Color & color)
			{
				for (auto& e : m_vertexes) e.color = color;
				m_isChanged = true;
			}

			inline void PolygonSquareX11::SetPosition(uint index, const Vector3 & pos)
			{
				m_vertexes[index].position = pos;
				m_isChanged = true;
			}

			inline void PolygonSquareX11::SetUV(uint index, const UV & uv)
			{
				m_vertexes[index].uv = uv;
				m_isChanged = true;
			}

			inline void PolygonSquareX11::SetColor(uint index, const Color & color)
			{
				m_vertexes[index].color = color;
				m_isChanged = true;
			}

			inline const Vertex::Polygon & PolygonSquareX11::GetVertex(uint index)
			{
				return m_vertexes[index];
			}

			inline void PolygonSquareX11::Draw2D()
			{
				using GraphicsX11 = SGFramework::Administrator::GraphicsX11;
				static Vector3 old;

				m_transform->m_drawMatrix._42 *= -1.0f;

				m_transform->setDrawMatrix();

				uint stride = static_cast<uint>(sizeof(Vertex::Polygon));
				uint offset = 0;

				GraphicsX11::context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
				GraphicsX11::context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

				GraphicsX11::context->Draw(4, 0);	
			}

			inline void PolygonSquareX11::Draw2DTryangle()
			{
				using GraphicsX11 = SGFramework::Administrator::GraphicsX11;
				static Vector3 old;

				m_transform->m_drawMatrix._42 *= -1.0f;

				m_transform->setDrawMatrix();

				uint stride = static_cast<uint>(sizeof(Vertex::Polygon));
				uint offset = 0;

				GraphicsX11::context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
				GraphicsX11::context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

				GraphicsX11::context->Draw(3, 0);
			}
			inline void PolygonSquareX11::Draw3D()
			{
				using GraphicsX11 = SGFramework::Administrator::GraphicsX11;

				m_transform->setDrawMatrix();
				
				uint stride = static_cast<uint>(sizeof(Vertex::Polygon));
				uint offset = 0;

				GraphicsX11::context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
				GraphicsX11::context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

				GraphicsX11::context->Draw(4, 0);
			}
			inline PolygonSquareX11::PolygonSquareX11(const Vector3 & position,
				const Vector3 & scale, const Quaternion & rotation, const sgstring & name)
			{
				m_transform = TransformX11::Instantiate(position, scale, rotation, name);
				transform = m_transform;
				try
				{
					Init();
				}
				catch (...)
				{
					throw;
				}
			}
				
			inline PolygonSquareX11::PolygonSquareX11(SharedPointer<TransformX11>& parent, bool isInstantiateInWorldSpace,
				const Vector3 & position, const Vector3 & scale, const Quaternion & rotation, const sgstring & name)
			{
				m_transform = TransformX11::Instantiate(parent, isInstantiateInWorldSpace, position, scale, rotation, name);
				transform = m_transform;
				try
				{
					Init();
				}
				catch (...)
				{
					throw;
				}
			}
			inline void PolygonSquareX11::Sync()
			{
				if (IS_TRUE(m_isChanged))
				{
					D3D11_MAPPED_SUBRESOURCE data;

					auto result = SGFramework::Administrator::GraphicsX11::m_deviceContext->Map(m_vertexBuffer,
						0, D3D11_MAP_WRITE_DISCARD, 0, &data);
					
					if (SUCCEEDED(result))
					{
						memcpy_s(data.pData, data.RowPitch, m_vertexes.data(), sizeof(Vertex::Polygon) * 4);
						SGFramework::Administrator::GraphicsX11::m_deviceContext->Unmap(m_vertexBuffer, 0);
					}
					m_isChanged = false;
				}
			}
			inline void PolygonSquareX11::Init()
			{
				m_vertexes[0].position = Const::Vector3::rightTop;
				m_vertexes[1].position = Const::Vector3::leftTop;
				m_vertexes[2].position = Const::Vector3::rightDown;
				m_vertexes[3].position = Const::Vector3::leftDown;
			
				m_vertexes[0].uv = Const::UV::rightTop;
				m_vertexes[1].uv = Const::UV::leftTop;
				m_vertexes[2].uv = Const::UV::rightDown;
				m_vertexes[3].uv = Const::UV::leftDown;
				for (auto& e : m_vertexes) e.color = Const::Color::white;


				D3D11_BUFFER_DESC bufferDesc = {};
				bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
				bufferDesc.ByteWidth = sizeof(Vertex::Polygon) * 4;
				bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
				bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

				D3D11_SUBRESOURCE_DATA data = {};
				data.pSysMem = m_vertexes.data();

				auto result = SGFramework::Administrator::GraphicsX11::m_device->CreateBuffer(&bufferDesc,
					&data, &m_vertexBuffer);
				if (FAILED(result))
					throw InvalidArgument(L"Error!! PolygonSquareX11->Constructor", L"CreateBuffer failed", result);
			}
		}
	}
}
#endif // !SGFRAMEWORK_HEADER_POLYGON_X11_HPP_