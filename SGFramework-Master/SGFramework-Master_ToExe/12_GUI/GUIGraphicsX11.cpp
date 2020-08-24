/*----------------------------------------------------------------------------------
GUIを管理するGUI class
------------------------------------------------------------------------------------*/
#include "GUI.hpp"
#include "../04_Asset/InstantiateAsset.hpp"

// Framework namespace
namespace SGFramework
{
	//構造体, グローバル変数を宣言するDetailGUIGraphics
	namespace DetailGUIGraphics
	{
		//現在のGraphics設定を保存するためのStates
		struct GUIBukupDirectX11States
		{
			UINT numScissorRects = 0, numViewports = 0;
			D3D11_RECT scissorRects[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE] = {};
			D3D11_VIEWPORT viewports[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE] = {};
			ID3D11RasterizerState* rasterizerState = nullptr;
			ID3D11BlendState* blendState = nullptr;
			D3D11_PRIMITIVE_TOPOLOGY primitiveTopology = {};
			DXGI_FORMAT indexBufferFormat = {};
			ID3D11InputLayout* inputLayout = nullptr;
			FLOAT blendFactor[4] = {};
			UINT sampleMask = 0;
			UINT stencilRef = 0;
			ID3D11Buffer* indexBuffer = nullptr, *vertexBuffer = nullptr, *vertexConstantBuffer = nullptr;
			UINT indexBufferOffset = 0, vertexBufferStride = 0, vertexBufferOffset = 0;
			ID3D11DepthStencilState* depthStencilState = nullptr;
			ID3D11ShaderResourceView* shaderResourceView = nullptr;
			ID3D11SamplerState* samplerState = nullptr;
			ID3D11PixelShader* pixelShader = nullptr;
			ID3D11VertexShader* vertexShader = nullptr;
			ID3D11GeometryShader* geometryShader = nullptr;
			ID3D11HullShader* hullShader = nullptr;
			ID3D11DomainShader* domainShader = nullptr;
			ID3D11ComputeShader* computeShader = nullptr;
			UINT numPixelShaderInstances = 0, numVertexShaderInstances = 0, numGeometryShaderInstances = 0;
			UINT numHullShaderInstances = 0, numDomainShaderInstances = 0, numComputeShaderInstances = 0;
			// 256 is max according to PSSetShader documentation
			ID3D11ClassInstance *pixelShaderInstance[256] = {}, *vertexShaderInstance[256] = {}, *getometryShaderInstance[256] = {};
			ID3D11ClassInstance *hullShaderInstance[256] = {}, *domainShaderInstance[256] = {}, *computeShaderInstance[256] = {};
		};

		//Vertex buffer
		static ID3D11Buffer* g_vertexBuffer = nullptr;
		//Index buffer
		static ID3D11Buffer* g_indexBuffer = nullptr;
		//Font sampler
		static ID3D11SamplerState* g_fontSampler = nullptr;
		//Font texture view
		static ID3D11ShaderResourceView* g_fontTextureView = nullptr;
		//Rasterizer state
		static ID3D11RasterizerState* g_rasterizerState = nullptr;
		//Blend state
		static ID3D11BlendState* g_blendState = nullptr;
		//Depth stencil state
		static ID3D11DepthStencilState* g_depthStencilState = nullptr;
		//Vertex & Index buffer size
		static int g_numVertexes = 5000, g_numIndexes = 10000;
	}

	//----------------------------------------------------------------------------------
	//[StratUpGraphicsX11]
	//DirectX11を使用する初期化を行う
	//引数1: vertex shader path
	//引数1: pixel shader path
	void GUI::StratUpGraphicsX11(const sgstring& vertexShaderName,
		const sgstring& pixelShaderName)
	{
		using namespace DetailGUIGraphics;

		//シェーダーを開く
		m_vertexShader = Asset::InstantiateAsset<Asset::VertexShaderAsset>(vertexShaderName, false);
		m_pixelShader = Asset::InstantiateAsset<Asset::PixelShaderAsset>(pixelShaderName, false);
		
		m_vertexShader->Open(std::move(Asset::VertexShaderAsset::getImGuiVertexInputElements()));
		m_pixelShader->Open();

		//以下、リネームやthrowなど軽微な変更はあるものの基本的にはimgui-master->sumpleのコピー
		
		// Setup back-end capabilities flags
		ImGuiIO& io = ImGui::GetIO();
		io.BackendRendererName = "imgui_impl_dx11";
		// We can honor the ImDrawCmd::VtxOffset field, allowing for large meshes.
		io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;  

		// Create the blending setup
		{
			D3D11_BLEND_DESC desc;
			ZeroMemory(&desc, sizeof(desc));
			desc.AlphaToCoverageEnable = false;
			desc.RenderTarget[0].BlendEnable = true;
			desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
			desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
			desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
			desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
			HRESULT result = Graphics::device->CreateBlendState(&desc, &g_blendState);
			if (FAILED(result))
				throw InvalidArgument(L"Error!! GUI->StartUp", L"CreateBlendState failed", result);
		}

		// Create the rasterizer state
		{
			D3D11_RASTERIZER_DESC desc;
			ZeroMemory(&desc, sizeof(desc));
			desc.FillMode = D3D11_FILL_SOLID;
			desc.CullMode = D3D11_CULL_NONE;
			desc.ScissorEnable = true;
			desc.DepthClipEnable = true;
			HRESULT result = Graphics::device->CreateRasterizerState(&desc, &g_rasterizerState);
			if (FAILED(result))
				throw InvalidArgument(L"Error!! GUI->StartUp", L"CreateRasterizerState failed", result);
		}

		// Create depth-stencil State
		{
			D3D11_DEPTH_STENCIL_DESC desc;
			ZeroMemory(&desc, sizeof(desc));
			desc.DepthEnable = false;
			desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			desc.DepthFunc = D3D11_COMPARISON_ALWAYS;
			desc.StencilEnable = false;
			desc.FrontFace.StencilFailOp = desc.FrontFace.StencilDepthFailOp = desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
			desc.BackFace = desc.FrontFace;
			HRESULT result = Graphics::device->CreateDepthStencilState(&desc, &g_depthStencilState);
			if (FAILED(result))
				throw InvalidArgument(L"Error!! GUI->StartUp", L"CreateDepthStencilState failed", result);
		}

		// Build texture atlas
		unsigned char* pixels;
		int width, height;
		io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

		// Upload texture to graphics system
		{
			HRESULT result = 0;
			D3D11_TEXTURE2D_DESC desc = {};

			desc.Width = width;
			desc.Height = height;
			desc.MipLevels = 1;
			desc.ArraySize = 1;
			desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			desc.SampleDesc.Count = 1;
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			desc.CPUAccessFlags = 0;

			ID3D11Texture2D *pTexture = nullptr;
			D3D11_SUBRESOURCE_DATA subResource;

			subResource.pSysMem = pixels;
			subResource.SysMemPitch = desc.Width * 4;
			subResource.SysMemSlicePitch = 0;
			result = Graphics::device->CreateTexture2D(&desc, &subResource, &pTexture);
			if (FAILED(result))
				throw InvalidArgument(L"Error!! GUI->StartUp", L"CreateTexture2D failed", result);

			// Create texture view
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};

			srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = desc.MipLevels;
			srvDesc.Texture2D.MostDetailedMip = 0;
			result = Graphics::device->CreateShaderResourceView(pTexture, &srvDesc, &g_fontTextureView);
			if (FAILED(result))
				throw InvalidArgument(L"Error!! GUI->StartUp", L"CreateShaderResourceView failed", result);

			pTexture->Release();
		}

		// Store our identifier
		io.Fonts->TexID = (ImTextureID)g_fontTextureView;

		// Create texture sampler
		{
			D3D11_SAMPLER_DESC desc = {};

			desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			desc.MipLODBias = 0.f;
			desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
			desc.MinLOD = 0.f;
			desc.MaxLOD = 0.f;
			HRESULT result = Graphics::device->CreateSamplerState(&desc, &g_fontSampler);
			if (FAILED(result))
				throw InvalidArgument(L"Error!! GUI->StartUp", L"CreateTexture2D failed", result);
		}


		// Create vertex/index buffers
		{
			HRESULT result = 0;
			D3D11_BUFFER_DESC desc = {};

			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.ByteWidth = g_numVertexes * sizeof(ImDrawVert);
			desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			desc.MiscFlags = 0;
			result = Graphics::device->CreateBuffer(&desc, NULL, &g_vertexBuffer);
			if (FAILED(result))
				throw InvalidArgument(L"Error!! GUI->StartUp", L"CreateBuffer failed", result);

			ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

			memset(&desc, 0, sizeof(D3D11_BUFFER_DESC));
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.ByteWidth = g_numIndexes * sizeof(ImDrawIdx);
			desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			result = Graphics::device->CreateBuffer(&desc, NULL, &g_indexBuffer);
			if (FAILED(result))
				throw InvalidArgument(L"Error!! GUI->StartUp", L"CreateBuffer failed", result);
		}
	}

	//----------------------------------------------------------------------------------
	//[ShutDownGraphicsX11]
	//DirectX11を使用する終了処理を行う
	void GUI::ShutDownGraphicsX11()
	{
		using namespace DetailGUIGraphics;

		//Releaseを行う
		COM_RELEASE(g_vertexBuffer);
		COM_RELEASE(g_indexBuffer);
		COM_RELEASE(g_fontSampler);
		COM_RELEASE(g_fontTextureView);
		COM_RELEASE(g_rasterizerState);
		COM_RELEASE(g_blendState);
		COM_RELEASE(g_depthStencilState);
	}


	//----------------------------------------------------------------------------------
	//[DrawGraphicsX11]
	//ImGui::Render後の描画処理を行う
	void GUI::DrawGraphicsX11()
	{ 
		using namespace DetailGUIGraphics;

		if (IS_FALSE(m_isUsing)) return;
		
		//以下、リネームやthrowなど軽微な変更はあるものの基本的にはimgui-master->sumpleのコピー

		ImDrawData* drawData = ImGui::GetDrawData();
		// Avoid rendering when minimized
		if (drawData->DisplaySize.x <= 0.0f || drawData->DisplaySize.y <= 0.0f)
			return;

		// Create and grow vertex/index buffers if needed
		if (g_numVertexes < drawData->TotalVtxCount)
		{
			COM_RELEASE(g_vertexBuffer);			
			D3D11_BUFFER_DESC desc = {};
			g_numVertexes = drawData->TotalVtxCount + 5000;

			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.ByteWidth = g_numVertexes * sizeof(ImDrawVert);
			desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			desc.MiscFlags = 0;
			if (Graphics::device->CreateBuffer(&desc, NULL, &g_vertexBuffer) < 0)
				return;
		}
		if (g_numIndexes < drawData->TotalIdxCount)
		{
			COM_RELEASE(g_indexBuffer);
			D3D11_BUFFER_DESC desc = {};
			g_numIndexes = drawData->TotalIdxCount + 10000;
	
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.ByteWidth = g_numIndexes * sizeof(ImDrawIdx);
			desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			if (Graphics::device->CreateBuffer(&desc, NULL, &g_indexBuffer) < 0)
				return;
		}

		// Upload vertex/index data into a single contiguous GPU buffer
		D3D11_MAPPED_SUBRESOURCE vertexResource, indexResource;
		ImDrawVert* vertexData = nullptr;
		ImDrawIdx* indexData = nullptr;
		HRESULT result = 0;

		result = Graphics::context->Map(g_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &vertexResource);
		if (FAILED(result))
			throw InvalidArgument(L"Error!! GUI->Update", L"GraphicsX11::Map failed", result);
		vertexData = reinterpret_cast<ImDrawVert*>(vertexResource.pData);
		
		result = Graphics::context->Map(g_indexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &indexResource);
		if (FAILED(result))
			throw InvalidArgument(L"Error!! GUI->Update", L"GraphicsX11::Map failed", result);
		indexData = reinterpret_cast<ImDrawIdx*>(indexResource.pData);

		for (int i = 0; i < drawData->CmdListsCount; i++)
		{
			const ImDrawList* cmd_list = drawData->CmdLists[i];

			memcpy_s(vertexData, drawData->CmdLists[i]->VtxBuffer.Size * sizeof(ImDrawVert),
				drawData->CmdLists[i]->VtxBuffer.Data, drawData->CmdLists[i]->VtxBuffer.Size * sizeof(ImDrawVert));
		
			memcpy_s(indexData, drawData->CmdLists[i]->IdxBuffer.Size * sizeof(ImDrawIdx),
				drawData->CmdLists[i]->IdxBuffer.Data, drawData->CmdLists[i]->IdxBuffer.Size * sizeof(ImDrawIdx));

			vertexData += cmd_list->VtxBuffer.Size;
			indexData += cmd_list->IdxBuffer.Size;
		}
		Graphics::context->Unmap(g_vertexBuffer, 0);
		Graphics::context->Unmap(g_indexBuffer, 0);

		// Setup orthographic projection matrix into our constant buffer
		// Our visible imgui space lies from draw_data->DisplayPos (top left) to draw_data->DisplayPos+data_data->DisplaySize (bottom right). DisplayPos is (0,0) for single viewport apps.
		{
			float L = drawData->DisplayPos.x;
			float R = drawData->DisplayPos.x + drawData->DisplaySize.x;
			float T = drawData->DisplayPos.y;
			float B = drawData->DisplayPos.y + drawData->DisplaySize.y;
			float mvp[4][4] =
			{
				{ 2.0f / (R - L),   0.0f,           0.0f,       0.0f },
				{ 0.0f,         2.0f / (T - B),     0.0f,       0.0f },
				{ 0.0f,         0.0f,           0.5f,       0.0f },
				{ (R + L) / (L - R),  (T + B) / (B - T),    0.5f,       1.0f },
			};
			//コンスタントバッファ更新
			memcpy_s(&Graphics::m_setConstantBufferScene.projectionMatrix, sizeof(Vector4x4), mvp, sizeof(Vector4x4));
			Graphics::context->UpdateSubresource(Graphics::m_constantBufferScene, 0, nullptr, &Graphics::m_setConstantBufferScene, 0, 0);
		}

		// Backup DX state that will be modified to restore it afterwards (unfortunately this is very ugly looking and verbose. Close your eyes!)
		GUIBukupDirectX11States old;
		old.numScissorRects = old.numViewports = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
		Graphics::context->RSGetScissorRects(&old.numScissorRects, old.scissorRects);
		Graphics::context->RSGetViewports(&old.numViewports, old.viewports);
		Graphics::context->RSGetState(&old.rasterizerState);
		Graphics::context->OMGetBlendState(&old.blendState, old.blendFactor, &old.sampleMask);
		Graphics::context->OMGetDepthStencilState(&old.depthStencilState, &old.stencilRef);
		Graphics::context->PSGetShaderResources(0, 1, &old.shaderResourceView);
		Graphics::context->PSGetSamplers(0, 1, &old.samplerState);
		old.numPixelShaderInstances = old.numVertexShaderInstances = old.numGeometryShaderInstances = 256;
		Graphics::context->PSGetShader(&old.pixelShader, old.pixelShaderInstance, &old.numPixelShaderInstances);
		Graphics::context->VSGetShader(&old.vertexShader, old.vertexShaderInstance, &old.numVertexShaderInstances);
		Graphics::context->VSGetConstantBuffers(0, 1, &old.vertexConstantBuffer);
		Graphics::context->GSGetShader(&old.geometryShader, old.getometryShaderInstance, &old.numGeometryShaderInstances);
		Graphics::context->HSGetShader(&old.hullShader, old.hullShaderInstance, &old.numHullShaderInstances);
		Graphics::context->DSGetShader(&old.domainShader, old.domainShaderInstance, &old.numDomainShaderInstances);
		Graphics::context->CSGetShader(&old.computeShader, old.computeShaderInstance, &old.numComputeShaderInstances);

		Graphics::context->IAGetPrimitiveTopology(&old.primitiveTopology);
		Graphics::context->IAGetIndexBuffer(&old.indexBuffer, &old.indexBufferFormat, &old.indexBufferOffset);
		Graphics::context->IAGetVertexBuffers(0, 1, &old.vertexBuffer, &old.vertexBufferStride, &old.vertexBufferOffset);
		Graphics::context->IAGetInputLayout(&old.inputLayout);

		// Setup desired DX state
		SetRendererStatesX11(drawData);

		// Render command lists
		// (Because we merged all buffers into a single one, we maintain our own offset into them)
		int globalIndexOffset = 0;
		int globalVertexOffset = 0;
		ImVec2 clipOff = drawData->DisplayPos;

		for (int i = 0; i < drawData->CmdListsCount; i++)
		{
			const ImDrawList* commands = drawData->CmdLists[i];

			for (int k = 0; k < commands->CmdBuffer.Size; k++)
			{
				const ImDrawCmd* pcmd = &commands->CmdBuffer[k];

				if (pcmd->UserCallback != NULL)
				{
					// User callback, registered via ImDrawList::AddCallback()
					// (ImDrawCallback_ResetRenderState is a special callback value used by the user to request the renderer to reset render state.)
					if (pcmd->UserCallback == ImDrawCallback_ResetRenderState)
						SetRendererStatesX11(drawData);
					else
						pcmd->UserCallback(commands, pcmd);
				}
				else
				{
					// Apply scissor/clipping rectangle
					D3D11_RECT rect = { (LONG)(pcmd->ClipRect.x - clipOff.x),
						(LONG)(pcmd->ClipRect.y - clipOff.y),
						(LONG)(pcmd->ClipRect.z - clipOff.x),
						(LONG)(pcmd->ClipRect.w - clipOff.y) };
					Graphics::context->RSSetScissorRects(1, &rect);

					// Bind texture, Draw
					Graphics::context->PSSetShaderResources(0, 1, &g_fontTextureView);
					Graphics::context->DrawIndexed(pcmd->ElemCount, pcmd->IdxOffset + globalIndexOffset, pcmd->VtxOffset + globalVertexOffset);
				}
			}
			globalIndexOffset += commands->IdxBuffer.Size;
			globalVertexOffset += commands->VtxBuffer.Size;
		}

		// Restore modified DX state
		Graphics::context->RSSetScissorRects(old.numScissorRects, old.scissorRects);
		Graphics::context->RSSetViewports(old.numViewports, old.viewports);
		Graphics::context->RSSetState(old.rasterizerState); if (old.rasterizerState) old.rasterizerState->Release();
		Graphics::context->OMSetBlendState(old.blendState, old.blendFactor, old.sampleMask); if (old.blendState) old.blendState->Release();
		Graphics::context->OMSetDepthStencilState(old.depthStencilState, old.stencilRef); if (old.depthStencilState) old.depthStencilState->Release();
		Graphics::context->PSSetShaderResources(0, 1, &old.shaderResourceView); if (old.shaderResourceView) old.shaderResourceView->Release();
		Graphics::context->PSSetSamplers(0, 1, &old.samplerState); if (old.samplerState) old.samplerState->Release();
		Graphics::context->PSSetShader(old.pixelShader, old.pixelShaderInstance, old.numPixelShaderInstances); if (old.pixelShader) old.pixelShader->Release();
		for (UINT i = 0; i < old.numPixelShaderInstances; i++) if (old.pixelShaderInstance[i]) old.pixelShaderInstance[i]->Release();
		Graphics::context->VSSetShader(old.vertexShader, old.vertexShaderInstance, old.numVertexShaderInstances); if (old.vertexShader) old.vertexShader->Release();
		Graphics::context->VSSetConstantBuffers(0, 1, &old.vertexConstantBuffer); if (old.vertexConstantBuffer) old.vertexConstantBuffer->Release();
		Graphics::context->GSSetShader(old.geometryShader, old.getometryShaderInstance, old.numGeometryShaderInstances); if (old.geometryShader) old.geometryShader->Release();
		for (UINT i = 0; i < old.numVertexShaderInstances; i++) if (old.vertexShaderInstance[i]) old.vertexShaderInstance[i]->Release();
		Graphics::context->IASetPrimitiveTopology(old.primitiveTopology);
		Graphics::context->IASetIndexBuffer(old.indexBuffer, old.indexBufferFormat, old.indexBufferOffset); if (old.indexBuffer) old.indexBuffer->Release();
		Graphics::context->IASetVertexBuffers(0, 1, &old.vertexBuffer, &old.vertexBufferStride, &old.vertexBufferOffset); if (old.vertexBuffer) old.vertexBuffer->Release();
		Graphics::context->IASetInputLayout(old.inputLayout); if (old.inputLayout) old.inputLayout->Release();
		Graphics::context->HSSetShader(old.hullShader, old.hullShaderInstance, old.numHullShaderInstances); if (old.hullShader) old.hullShader->Release();
		Graphics::context->DSSetShader(old.domainShader, old.domainShaderInstance, old.numDomainShaderInstances); if (old.domainShader) old.domainShader->Release();
		Graphics::context->CSSetShader(old.computeShader, old.computeShaderInstance, old.numComputeShaderInstances); if (old.computeShader) old.computeShader->Release();
	}


	//----------------------------------------------------------------------------------
	//[SetRendererStatesX11]
	//DrawGraphicsX11->Set Renderer States
	void GUI::SetRendererStatesX11(ImDrawData* draw_data)
	{	
		using namespace DetailGUIGraphics;

		// Setup viewport
		D3D11_VIEWPORT viewPort;
		memset(&viewPort, 0, sizeof(D3D11_VIEWPORT));
		viewPort.Width = draw_data->DisplaySize.x;
		viewPort.Height = draw_data->DisplaySize.y;
		viewPort.MinDepth = 0.0f;
		viewPort.MaxDepth = 1.0f;
		viewPort.TopLeftX = viewPort.TopLeftY = 0;
		Graphics::context->RSSetViewports(1, &viewPort);

		// Setup shader and vertex buffers
		unsigned int stride = sizeof(ImDrawVert);
		unsigned int offset = 0;
		Graphics::context->IASetVertexBuffers(0, 1, &g_vertexBuffer, &stride, &offset);
		Graphics::context->IASetIndexBuffer(g_indexBuffer, sizeof(ImDrawIdx) == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT, 0);
		Graphics::context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		Graphics::context->VSSetConstantBuffers(Graphics::m_cConstantBufferSceneSlot, 1, &Graphics::m_constantBufferScene);
		Graphics::context->PSSetSamplers(0, 1, &g_fontSampler);
		Graphics::context->GSSetShader(nullptr, nullptr, 0);
		Graphics::context->HSSetShader(nullptr, nullptr, 0);
		Graphics::context->DSSetShader(nullptr, nullptr, 0);
		Graphics::context->CSSetShader(nullptr, nullptr, 0);

		m_vertexShader->setShader();
		m_pixelShader->setShader();

		// Setup blend state
		const float blend_factor[4] = { 0.f, 0.f, 0.f, 0.f };
		Graphics::context->OMSetBlendState(g_blendState, blend_factor, 0xffffffff);
		Graphics::context->OMSetDepthStencilState(g_depthStencilState, 0);
		Graphics::context->RSSetState(g_rasterizerState);
	}
}