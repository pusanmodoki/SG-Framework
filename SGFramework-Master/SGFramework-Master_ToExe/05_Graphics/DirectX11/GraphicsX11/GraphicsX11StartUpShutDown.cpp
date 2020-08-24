#include "GraphicsX11.hpp"
#include "../../../07_Thread/Thread.hpp"
#include "assimp/matrix4x4.h"
#undef near

// Framework namespace
namespace SGFramework
{
	//フレームワークの管理をするAdministrator namespace
	namespace Administrator
	{
		void GraphicsX11::StartUp(const ReadElement::Pack& isVsyncEnabled, const ReadElement::Pack& isNoRestrictionsFPS, 
			const ReadElement::Pack& depth, const ReadElement::Pack& near, const ReadElement::Pack& clearColor, bool isRemote)
		{
			HRESULT result = S_OK;
			IDXGIFactory* factory = nullptr;
			
			//vsync set
			m_isEnabledVsync = static_cast<bool>(isVsyncEnabled[0].valueInt);

			//color set
			for (int i = 0; i < 4; ++i)
				m_clearColor[i] = clearColor[i].valueFloat;

			//create factory
			result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
			if (FAILED(result))
			{
				MessageBox(Window::HwndHolder::main, L"D3D11 Create DXGIFactory error", L"error!!", MB_OK);
				throw InvalidArgument(L"Error!! GraphicsX11->StartUp", L"D3D11 Create DXGIFactory error", result);
			}

			//Adapter Setting
			{
				IDXGIAdapter* adapter = nullptr;

				//グラフィック インタフェース アダプターを作成
				result = factory->EnumAdapters(0, &adapter);
				if (FAILED(result))
					throw InvalidArgument(L"Error!! GraphicsX11->StartUp", L"D3D11 EnumAdapters error", result);

				{
					//AdapterDesc
					DXGI_ADAPTER_DESC adapterDesc;

					//ビデオカードの詳細を取得
					result = adapter->GetDesc(&adapterDesc);
					if (FAILED(result))
						throw InvalidArgument(L"Error!! GraphicsX11->StartUp", L"D3D11 Adapter GetDesc error", result);

					//ビデオカードメモリを取得（MB単位）
					m_videoCardMemoryWithGB = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024 / 1024);

					//ビデオカード名を取得
					m_videoCardName = adapterDesc.Description;
				}

				//アダプタを解放
				adapter->Release();
			}


			//Create Device
			{
				D3D_FEATURE_LEVEL featureLevels[] = {
					D3D_FEATURE_LEVEL_11_1,
					D3D_FEATURE_LEVEL_11_0,
				};

				// デバイス&コンテキストを生成
				result = D3D11CreateDevice(
					nullptr,
					D3D_DRIVER_TYPE_HARDWARE, // ハードウェア ドライバー を使用
					nullptr,
					0,
					featureLevels,
					2,
					D3D11_SDK_VERSION,
					&m_device,
					&m_featureLevel,
					&m_deviceContext
				);

				if (FAILED(result))
					throw InvalidArgument(L"Error!! GraphicsX11->StartUp", L"D3D11 Create Device error", result);
			}


			//SwapChain
			{
				DXGI_SWAP_CHAIN_DESC swapChainDesc;
				ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

				//バッファサイズ
				swapChainDesc.BufferDesc.Width = screen.width;
				swapChainDesc.BufferDesc.Height = screen.height;

				//リフレッシュレート
				swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
				if (IS_FALSE(isRemote))
				{
					if (isNoRestrictionsFPS[0].valueInt == 0)
					{
						swapChainDesc.BufferDesc.RefreshRate.Numerator =
							static_cast<uint>(Administrator::GetRefreshRate(Window::HwndHolder::main));
					}
					else
					{
						swapChainDesc.BufferDesc.RefreshRate.Numerator = VariableLimit::cUintMax;
					}
				}
				else
					swapChainDesc.BufferDesc.RefreshRate.Numerator = 30;

				
				//スキャンライン, デフォルト
				swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
				//スケーリング, 有効
				swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_STRETCHED;
				//バッファフォーマット
				swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				//MSAA
				swapChainDesc.SampleDesc.Count = 1;
				swapChainDesc.SampleDesc.Quality = 0;
				//Usage
				swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
				//ダブルバッファ
				swapChainDesc.BufferCount = 2;
				//フルスクリーン
				if (screen.m_isFullScreen) swapChainDesc.Windowed = false;
				else swapChainDesc.Windowed = true;
				//アウトプット
				swapChainDesc.OutputWindow = Window::HwndHolder::main;
				//スワップエフェクト, 規定
				swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
				//動作オプション, フルスクリーンとウィンドウモードの切り替え可能
				swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

				//スワップチェイン作成
				result = factory->CreateSwapChain(m_device, &swapChainDesc, &m_swapChain);
				if (FAILED(result))
					throw InvalidArgument(L"Error!! GraphicsX11->StartUp", L"D3D11 SwapChain GetBuffer error", result);
			}

			////DeferredContext
			//{
			//	result = m_device->CreateDeferredContext(0, &m_deferredContext);
			//	if (FAILED(result))
			//		throw InvalidArgument(L"Error!! GraphicsX11->StartUp", L"CreateDeferredContext error", result);
			//}

			//Get RenderTargetView
			{
				ID3D11Texture2D* backBuffer = nullptr;
				result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
				if (FAILED(result))
					throw InvalidArgument(L"Error!! GraphicsX11->StartUp", L"D3D11 SwapChain GetBuffer error", result);

				result = m_device->CreateRenderTargetView(backBuffer, NULL, &m_renderTargetView);
				if (FAILED(result))
					throw InvalidArgument(L"Error!! GraphicsX11->StartUp", L"D3D11 Create RenderTargetView error", result);

				//release
				backBuffer->Release();
			}

			//Depth Buffer
			{
				//深度バッファ
				D3D11_TEXTURE2D_DESC depthBufferDesc;
				ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

				//バッファサイズ
				depthBufferDesc.Width = screen.width;
				depthBufferDesc.Height = screen.height;
				//ミップマップレベル最大数
				depthBufferDesc.MipLevels = 1;
				//?
				depthBufferDesc.ArraySize = 1;
				//テクスチャフォーマット
				depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
				//MSAA
				depthBufferDesc.SampleDesc.Count = 1;
				depthBufferDesc.SampleDesc.Quality = 0;
				//Usage
				depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
				//バインド
				depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
				//CPUアクセスはしない
				depthBufferDesc.CPUAccessFlags = 0;
				//?
				depthBufferDesc.MiscFlags = 0;

				//深度バッファ作成
				result = m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
				if (FAILED(result))
					throw InvalidArgument(L"Error!! GraphicsX11->StartUp", L"D3D11 Create Texture2D error", result);
			}

			//Depth Stencil View
			{
				D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
				D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
				ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
				ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

				//Depth有効
				depthStencilDesc.DepthEnable = true;
				//全て変更可能
				depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
				//?
				depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
				//Stencil有効
				depthStencilDesc.StencilEnable = true;
				//? 全部行けるということ？
				depthStencilDesc.StencilReadMask = 0xFF;
				//? 全部行けるということ？
				depthStencilDesc.StencilWriteMask = 0xFF;
				//?
				depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
				depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
				depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
				depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
				//?
				depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
				depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
				depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
				depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

				//DepthStencilState作成
				result = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilEnableState);
				if (FAILED(result))
					throw InvalidArgument(L"Error!! GraphicsX11->StartUp", L"D3D11 Create DepthStencilState error", result);

				//デバイスコンテキストにセット, なぜ1？？0でもいいらしいが
				m_deviceContext->OMSetDepthStencilState(m_depthStencilEnableState, 1);


				//フォーマット
				depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
				//アクセス方法
				depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
				//最初に使用するミップマップ？
				depthStencilViewDesc.Texture2D.MipSlice = 0;

				//DepthStencilView作成
				result = m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
				if (FAILED(result))
					throw InvalidArgument(L"Error!! GraphicsX11->StartUp", L"D3D11 Create DepthStencilView error", result);
			
				//コンテキストにセット
				m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
			}

			//RasterizerState
			{
				D3D11_RASTERIZER_DESC rasterDesc;

				//三角形塗りつぶし
				rasterDesc.FillMode = D3D11_FILL_SOLID;
				//後ろは描画しない
				rasterDesc.CullMode = D3D11_CULL_BACK;
				//時計回りが表
				rasterDesc.FrontCounterClockwise = false;
				//?
				rasterDesc.DepthBias = 0;
				//?
				rasterDesc.DepthBiasClamp = 0.0f;
				//?
				rasterDesc.SlopeScaledDepthBias = 0.0f;
				//クリッピングON?
				rasterDesc.DepthClipEnable = true;
				//シザーカリング無効
				rasterDesc.ScissorEnable = false;
				//ラスターステート設定
				rasterDesc.AntialiasedLineEnable = false;
				//MSAA有効
				rasterDesc.MultisampleEnable = true;

				//RasterizerState作成
				result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterState);
				if (FAILED(result))
					throw InvalidArgument(L"Error!! GraphicsX11->StartUp", L"D3D11 Create RasterizerState error", result);
				//ラスタステートをセット
				m_deviceContext->RSSetState(m_rasterState);
			}

			//View Port
			{
				//左位置
				m_viewPort.TopLeftX = 0.0f;
				//上位置
				m_viewPort.TopLeftY = 0.0f;
				//幅
				m_viewPort.Width = static_cast<float>(screen.width);
				//高さ
				m_viewPort.Height = static_cast<float>(screen.height);
				//最小奥行き
				m_viewPort.MinDepth = 0.0f;
				//最大奥行き
				m_viewPort.MaxDepth = 1.0f;

				//ビューポート作成
				m_deviceContext->RSSetViewports(1, &m_viewPort);
			}

			//Depth Disabled Stencil State (2D)
			{
				D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
				ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

				//Depth無効
				depthDisabledStencilDesc.DepthEnable = false;
				//?
				depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
				//全て変更可能
				depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
				//Stencil有効
				depthDisabledStencilDesc.StencilEnable = true;
				//? 全部行けるということ？
				depthDisabledStencilDesc.StencilReadMask = 0xFF;
				//? 全部行けるということ？
				depthDisabledStencilDesc.StencilWriteMask = 0xFF;
				//?
				depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
				depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
				depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
				depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
				//?
				depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
				depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
				depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
				depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

				//DepthStencilStateの作成
				result = m_device->CreateDepthStencilState(&depthDisabledStencilDesc, &m_depthStencilDisabledState);
				if (FAILED(result))
					throw InvalidArgument(L"Error!! GraphicsX11->StartUp", L"D3D11 Create DepthStencilState(2D) error", result);
			}

			{
				D3D11_BLEND_DESC blendStateDescription;
				ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));

				//アルファブレンド可
				blendStateDescription.RenderTarget[0].BlendEnable = true;

				//?
				//blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
				blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
				blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
				//blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
				blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
				blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
				blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
				blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
				blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

				//BlendState作成
				result = m_device->CreateBlendState(&blendStateDescription, &m_alphaBlendingEnableState);
				if (FAILED(result))
					throw InvalidArgument(L"Error!! GraphicsX11->StartUp", L"D3D11 Create BlendState(Enable) error", result);

				//アルファブレンド不可
				blendStateDescription.RenderTarget[0].BlendEnable = false;

				//BlendState作成
				result = m_device->CreateBlendState(&blendStateDescription, &m_alphaBlendingDisableState);
				if (FAILED(result))
					throw InvalidArgument(L"Error!! GraphicsX11->StartUp", L"D3D11 Create BlendState(Enable) error", result);
			}


			//Constant Buffer
			{
				D3D11_BUFFER_DESC desc = {};
				uint size = 0;

				//Scene
				desc.Usage = D3D11_USAGE_DEFAULT;							// バッファ使用方法
				desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;		// コンスタントバッファ
				desc.CPUAccessFlags = 0;												// CPUアクセス不要
				//バッファの大きさ, サイズは16の倍数でないといけない
				size = static_cast<uint>(sizeof(m_setConstantBufferScene));
				size += (size % 16 == 0) ? 0 : 16 - size % 16;
				desc.ByteWidth = size;

				result = device->CreateBuffer(&desc, nullptr, &m_constantBufferScene);
				if (FAILED(result)) 
					throw InvalidArgument(L"Error!! GraphicsX11->StartUp", L"D3D11 CreateBuffer error", result);

				//Light
				ZeroMemory(&desc, sizeof(desc));
				desc.Usage = D3D11_USAGE_DEFAULT;							// バッファ使用方法
				desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;		// コンスタントバッファ
				desc.CPUAccessFlags = 0;												// CPUアクセス不要
				//バッファの大きさ, サイズは16の倍数でないといけない
				size = static_cast<uint>(sizeof(m_setConstantBufferLight));
				size += (size % 16 == 0) ? 0 : 16 - size % 16;
				desc.ByteWidth = size;

				result = device->CreateBuffer(&desc, nullptr, &m_constantBufferLight);
				if (FAILED(result))
					throw InvalidArgument(L"Error!! GraphicsX11->StartUp", L"D3D11 CreateBuffer error", result);

				//Object
				ZeroMemory(&desc, sizeof(desc));
				desc.Usage = D3D11_USAGE_DEFAULT;							// バッファ使用方法
				desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;		// コンスタントバッファ
				desc.CPUAccessFlags = 0;												// CPUアクセス不要
				//バッファの大きさ, サイズは16の倍数でないといけない
				size = static_cast<uint>(sizeof(m_setConstantBufferObject));
				size += (size % 16 == 0) ? 0 : 16 - size % 16;
				desc.ByteWidth = size;

				result = device->CreateBuffer(&desc, nullptr, &m_constantBufferObject);
				if (FAILED(result))
					throw InvalidArgument(L"Error!! GraphicsX11->StartUp", L"D3D11 CreateBuffer error", result);

				//Material
				ZeroMemory(&desc, sizeof(desc));
				desc.Usage = D3D11_USAGE_DEFAULT;							// バッファ使用方法
				desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;		// コンスタントバッファ
				desc.CPUAccessFlags = 0;												// CPUアクセス不要
				//バッファの大きさ, サイズは16の倍数でないといけない
				size = static_cast<uint>(sizeof(m_setConstantBufferMaterial2D));
				size += (size % 16 == 0) ? 0 : 16 - size % 16;
				desc.ByteWidth = size;

				result = device->CreateBuffer(&desc, nullptr, &m_constantBufferMaterial2D);
				if (FAILED(result))
					throw InvalidArgument(L"Error!! GraphicsX11->StartUp", L"D3D11 CreateBuffer error", result);

				//Material
				ZeroMemory(&desc, sizeof(desc));
				desc.Usage = D3D11_USAGE_DEFAULT;							// バッファ使用方法
				desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;		// コンスタントバッファ
				desc.CPUAccessFlags = 0;												// CPUアクセス不要
				//バッファの大きさ, サイズは16の倍数でないといけない
				size = static_cast<uint>(sizeof(m_setConstantBufferMaterial3D));
				size += (size % 16 == 0) ? 0 : 16 - size % 16;
				desc.ByteWidth = size;

				result = device->CreateBuffer(&desc, nullptr, &m_constantBufferMaterial3D);
				if (FAILED(result))
					throw InvalidArgument(L"Error!! GraphicsX11->StartUp", L"D3D11 CreateBuffer error", result);
		

				//Model
				ZeroMemory(&desc, sizeof(desc));
				desc.Usage = D3D11_USAGE_DEFAULT;							// バッファ使用方法
				desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;		// コンスタントバッファ
				desc.CPUAccessFlags = 0;												// CPUアクセス不要
				//バッファの大きさ, サイズは16の倍数でないといけない
				size = static_cast<uint>(sizeof(m_setConstantBufferModel));
				size += (size % 16 == 0) ? 0 : 16 - size % 16;
				desc.ByteWidth = size;

				result = device->CreateBuffer(&desc, nullptr, &m_constantBufferModel);
				if (FAILED(result))
					throw InvalidArgument(L"Error!! GraphicsX11->StartUp", L"D3D11 CreateBuffer error", result);

				aiMatrix4x4 identity = aiMatrix4x4();
				for (auto& e : m_setConstantBufferModel.bones)
					memcpy_s(&e, sizeof(Vector4x4), &identity, sizeof(Vector4x4));
			}

			//Sampler State
			{
				D3D11_SAMPLER_DESC samplerDesc = {};

				samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
				samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
				samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
				samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
				result = device->CreateSamplerState(&samplerDesc, &m_defaultSamplerState);
				if (FAILED(result))
					throw InvalidArgument(L"Error!! GraphicsX11->StartUp", L"D3D11 CreateSamplerState error", result);
			}
		}

		void GraphicsX11::ShutDown()
		{
			//開放してまわる
			if (m_swapChain != nullptr) 
				m_swapChain->SetFullscreenState(false, nullptr);
			
			COM_RELEASE(m_alphaBlendingEnableState);
			COM_RELEASE(m_alphaBlendingDisableState);
			COM_RELEASE(m_rasterState);
			COM_RELEASE(m_depthStencilView);
			COM_RELEASE(m_depthStencilDisabledState);
			COM_RELEASE(m_depthStencilEnableState);
			COM_RELEASE(m_depthStencilBuffer);
			COM_RELEASE(m_defaultSamplerState);
			COM_RELEASE(m_constantBufferScene);
			COM_RELEASE(m_constantBufferLight);
			COM_RELEASE(m_constantBufferObject);
			COM_RELEASE(m_constantBufferMaterial2D);
			COM_RELEASE(m_constantBufferMaterial3D);
			COM_RELEASE(m_renderTargetView);
			COM_RELEASE(m_swapChain);
			COM_RELEASE(m_deviceContext);
			COM_RELEASE(m_device);
		}
	}
}