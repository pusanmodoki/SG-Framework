#include "GraphicsX11.hpp"


// Framework namespace
namespace SGFramework
{
	//フレームワークの管理をするAdministrator namespace
	namespace Administrator
	{
		//Public

		BaseClass::BaseScreen<GraphicsX11> GraphicsX11::screen;

		PointerProperty<ID3D11Device*> GraphicsX11::device(m_device);
		PointerProperty<ID3D11DeviceContext*> GraphicsX11::context(m_deviceContext);
		PointerProperty<IDXGISwapChain*> GraphicsX11::swapChain(m_swapChain);
		PointerProperty<ID3D11RenderTargetView*> GraphicsX11::renderTargetView(m_renderTargetView);
		PointerProperty<ID3D11DepthStencilView*> GraphicsX11::depthStencilView(m_depthStencilView);
		PointerProperty<ID3D11SamplerState*> GraphicsX11::defaultSamplerState(m_defaultSamplerState);
		GetOnlyProperty<sgstring> GraphicsX11::videoCardName(m_videoCardName);
		GetOnlyProperty<Color> GraphicsX11::clearColor(m_clearColor);

		//Private

		D3D11_VIEWPORT GraphicsX11::m_viewPort = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

		ID3D11Device* GraphicsX11::m_device = nullptr;
		ID3D11DeviceContext* GraphicsX11::m_deviceContext = nullptr;
		IDXGISwapChain* GraphicsX11::m_swapChain = nullptr;
		ID3D11RenderTargetView*	GraphicsX11::m_renderTargetView = nullptr;
		ID3D11Texture2D* GraphicsX11::m_depthStencilBuffer = nullptr;
		ID3D11DepthStencilState* GraphicsX11::m_depthStencilEnableState = nullptr;
		ID3D11DepthStencilView* GraphicsX11::m_depthStencilView = nullptr;
		ID3D11RasterizerState* GraphicsX11::m_rasterState = nullptr;
	
		ID3D11DepthStencilState* GraphicsX11::m_depthStencilDisabledState = nullptr;
		ID3D11BlendState* GraphicsX11::m_alphaBlendingEnableState = nullptr;
		ID3D11BlendState* GraphicsX11::m_alphaBlendingDisableState = nullptr;
		ID3D11SamplerState* GraphicsX11::m_defaultSamplerState = nullptr;

		GraphicsX11::DrawCommandList GraphicsX11::m_commandList;
		GraphicsX11::DrawCommandList GraphicsX11::m_commandQueue;
		std::list<DrawCommand> GraphicsX11::m_makeCommand;

		D3D_DRIVER_TYPE GraphicsX11::m_driverType = D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_NULL;
		D3D_FEATURE_LEVEL GraphicsX11::m_featureLevel = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_9_1;

		ID3D11Buffer* GraphicsX11::m_constantBufferScene = nullptr;
		ID3D11Buffer* GraphicsX11::m_constantBufferLight = nullptr;
		ID3D11Buffer* GraphicsX11::m_constantBufferObject = nullptr;
		ID3D11Buffer* GraphicsX11::m_constantBufferMaterial2D = nullptr;
		ID3D11Buffer* GraphicsX11::m_constantBufferMaterial3D = nullptr;
		ID3D11Buffer* GraphicsX11::m_constantBufferModel = nullptr;

		GraphicsDetail::X11::ConstantBufferScene GraphicsX11::m_setConstantBufferScene;
		GraphicsDetail::X11::ConstantBufferLight GraphicsX11::m_setConstantBufferLight;
		GraphicsDetail::X11::ConstantBufferObject GraphicsX11::m_setConstantBufferObject;
		GraphicsDetail::X11::ConstantBufferMaterial2D GraphicsX11::m_setConstantBufferMaterial2D;
		GraphicsDetail::X11::ConstantBufferMaterial3D GraphicsX11::m_setConstantBufferMaterial3D;
		GraphicsDetail::X11::ConstantBufferModel GraphicsX11::m_setConstantBufferModel;

		std::unordered_set<GraphicsDetail::GraphicsObjectX11*> GraphicsX11::m_graphicsObjects;
		std::vector<Tuple::Two<GraphicsDetail::GraphicsObjectX11*, bool>> GraphicsX11::m_addOrEraseGraphicsObjects;
		std::atomic_bool GraphicsX11::m_graphicsObjectsLock = false;

		sgstring GraphicsX11::m_videoCardName = L"Not Found";
		Color GraphicsX11::m_clearColor = Const::Color::black;
		uint GraphicsX11::m_nowShaderInstanceIDs[m_cNumTypeShader] = {-1, -1};
		uint GraphicsX11::m_nowTextureInstanceID = -1;
		int GraphicsX11::m_videoCardMemoryWithGB = 0;
		bool GraphicsX11::m_isEnabledDepthBuffer[2] = { true, true };
		bool GraphicsX11::m_isEnabledAlphaBlending[2] = { true, true };
		bool GraphicsX11::m_isEnabledVsync = false;
		bool GraphicsX11::m_isSetConstantBufferLight = false;
		bool GraphicsX11::m_isFirstFrame = true;
	}
}