#ifndef SGFRAMEWORK_HEADER_GRAPHICS_X11_HPP_
#define SGFRAMEWORK_HEADER_GRAPHICS_X11_HPP_
#include <d3d11.h>
#include <unordered_map>
#include <functional>
#include "../../../02_CustomVariable/CustomVariable.hpp"
#include "../../DrawCommand/DrawCommand.hpp"
#include "../../Screen/Screen.hpp"
#include "GraphicsX11ConstantBuffer.hpp"
#include "DirectXTex/DirectXTex.h"
#include "DDSTextureLoader/DDSTextureLoader.h"
#include "WICTextureLoader/WICTextureLoader.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "DirectXTex.lib")

// Framework namespace
namespace SGFramework
{
	class Application;
	class Thread;
	class GUI;
	class Animator;
	namespace BaseClass
	{
		class BasePolygonX11;
	}
	//フレームワークの管理をするAdministrator namespace
	namespace Administrator
	{
		//GraphicsDetail namespace
		namespace GraphicsDetail
		{
			class TransformX11;
			class TextureAssetX11;
			class PolygonSquareX11;
			class GraphicsObjectX11;
			class MaterialX11;
			class VertexShaderAssetX11;
			class PixelShaderAssetX11;
			class CameraX11;
		}

		class GraphicsX11 final
		{
		public:
			friend class Application;
			friend class Thread;
			friend class GUI;
			friend class Animator;
			friend class GraphicsDetail::TransformX11;
			friend class GraphicsDetail::TextureAssetX11;
			friend class GraphicsDetail::PolygonSquareX11;
			friend class GraphicsDetail::GraphicsObjectX11;
			friend class GraphicsDetail::MaterialX11;
			friend class GraphicsDetail::VertexShaderAssetX11;
			friend class GraphicsDetail::PixelShaderAssetX11;
			friend class GraphicsDetail::CameraX11;
			friend class BaseClass::BasePolygonX11;

			DELETE_ALL_DEFAULT_CLASS_FUNCTIONS(GraphicsX11);

			//set alpha blending graphics <static>(sync function only property)
			//argument 1: enabled = true, disabled = false
			static SGF_FUNCTION_PROPERTY void setAlphaBlending(bool value);
			//set depth buffer graphics <static>(sync function only property)
			//argument 1: enabled = true, disabled = false
			static SGF_FUNCTION_PROPERTY void setDepthBuffer(bool value);
			
			//send draw function to graphics thread <static>(draw function only property)
			//argument 1: function (member function -> ramda)
			//argument 2: priority
			static inline void SendDrawFunction(const std::function<void()> drawFunctionCommand, int priority);
		

			static BaseClass::BaseScreen<GraphicsX11> screen;

			static PointerProperty<ID3D11Device*> device;
			static PointerProperty<ID3D11DeviceContext*> context;
			static PointerProperty<IDXGISwapChain*> swapChain;
			static PointerProperty<ID3D11RenderTargetView*> renderTargetView;
			static PointerProperty<ID3D11DepthStencilView*> depthStencilView;
			static PointerProperty<ID3D11SamplerState*> defaultSamplerState;
			static GetOnlyProperty<sgstring> videoCardName;
			static GetOnlyProperty<Color> clearColor;

		private:
			struct ScreenChangeSet
			{
				uint width = 0U;
				uint height = 0U;
				std::atomic_bool lock = false;
				bool isChange = false;
			};

			//GraphicsUpdateで使用
			static constexpr uint m_cGraphicsThreadState = 0;
			//GraphicsUpdate以外のスレッドで使用
			static constexpr uint m_UpdateThreadState = 1;
			static constexpr uint m_cConstantBufferSceneSlot = 0;
			static constexpr uint m_cConstantBufferLightSlot = 1;
			static constexpr uint m_cConstantBufferObjectSlot = 2;
			static constexpr uint m_cConstantBufferMaterial3DSlot = 3;
			static constexpr uint m_cConstantBufferMaterial2DSlot = 4;
			static constexpr uint m_cConstantBufferModelSlot = 5;

			using DrawCommandList = std::unordered_map<std::thread::id, std::list<DrawCommand>>;

			static void StartUp(const ReadElement::Pack& isVsyncEnabled, const ReadElement::Pack& isNoRestrictionsFPS, 
				const ReadElement::Pack& depth, const ReadElement::Pack& _near, const ReadElement::Pack& clearColor, bool isRemote);
			static void ShutDown();

			static void InitCommandList();
			static void InitCommandList(const std::vector<std::thread>& subIDs);
			static void SortList();
			static void SyncGraphics();
			static void UpdateGraphics();

			static constexpr uint m_cNumTypeShader = 2;
			static constexpr uint m_cTypeVertexShader = 0;
			static constexpr uint m_cTypePixelShader = 1;

			static D3D11_VIEWPORT m_viewPort;

			static ID3D11Device* m_device;
			static ID3D11DeviceContext* m_deviceContext;
			static IDXGISwapChain* m_swapChain;
			static ID3D11RenderTargetView*	m_renderTargetView;
			static ID3D11Texture2D* m_depthStencilBuffer;
			static ID3D11DepthStencilState* m_depthStencilEnableState;
			static ID3D11DepthStencilView* m_depthStencilView;
			static ID3D11RasterizerState* m_rasterState;

			static ID3D11DepthStencilState* m_depthStencilDisabledState;
			static ID3D11BlendState* m_alphaBlendingEnableState;
			static ID3D11BlendState* m_alphaBlendingDisableState;
			static ID3D11SamplerState* m_defaultSamplerState;

			static DrawCommandList m_commandList;
			static DrawCommandList m_commandQueue;
			static std::list<DrawCommand> m_makeCommand;
			
			static D3D_DRIVER_TYPE m_driverType;
			static D3D_FEATURE_LEVEL m_featureLevel;

			static ID3D11Buffer* m_constantBufferScene;
			static ID3D11Buffer* m_constantBufferLight;
			static ID3D11Buffer* m_constantBufferObject;
			static ID3D11Buffer* m_constantBufferMaterial2D;
			static ID3D11Buffer* m_constantBufferMaterial3D;
			static ID3D11Buffer* m_constantBufferModel;

			static std::unordered_set<GraphicsDetail::GraphicsObjectX11*> m_graphicsObjects;
			static std::vector<Tuple::Two<GraphicsDetail::GraphicsObjectX11*, bool>> m_addOrEraseGraphicsObjects;
			static std::atomic_bool m_graphicsObjectsLock;

			static GraphicsDetail::X11::ConstantBufferScene m_setConstantBufferScene;
			static GraphicsDetail::X11::ConstantBufferLight m_setConstantBufferLight;
			static GraphicsDetail::X11::ConstantBufferObject m_setConstantBufferObject;
			static GraphicsDetail::X11::ConstantBufferMaterial2D m_setConstantBufferMaterial2D;
			static GraphicsDetail::X11::ConstantBufferMaterial3D m_setConstantBufferMaterial3D;
			static GraphicsDetail::X11::ConstantBufferModel m_setConstantBufferModel;

			static sgstring m_videoCardName;
			static Color m_clearColor;
			static uint m_nowShaderInstanceIDs[m_cNumTypeShader];
			static uint m_nowTextureInstanceID;
			static int m_videoCardMemoryWithGB;
			static bool m_isEnabledDepthBuffer[2];
			static bool m_isEnabledAlphaBlending[2];
			static bool m_isEnabledVsync;
			static bool m_isSetConstantBufferLight;
			static bool m_isFirstFrame;
		};

		inline void GraphicsX11::SendDrawFunction(const std::function<void()> drawFunctionCommand, int priority)
		{
			m_commandQueue.at(std::this_thread::get_id()).emplace_back(drawFunctionCommand, priority);
		}
		
		SGF_FUNCTION_PROPERTY void GraphicsX11::setAlphaBlending(bool value)
		{
			static float blendFactor[4] = { 0.0f,0.0f,0.0f,0.0f };

			if ((value ^ m_isEnabledAlphaBlending[m_cGraphicsThreadState]) & value)
			{
				m_deviceContext->OMSetBlendState(m_alphaBlendingEnableState, blendFactor, 0xffffffff);
			}
			else if ((value ^ m_isEnabledAlphaBlending[m_cGraphicsThreadState])
				& m_isEnabledAlphaBlending[m_cGraphicsThreadState])
			{
				m_deviceContext->OMSetBlendState(m_alphaBlendingDisableState, blendFactor, 0xffffffff);
			}

			m_isEnabledAlphaBlending[m_cGraphicsThreadState] = value;
		}

		SGF_FUNCTION_PROPERTY void GraphicsX11::setDepthBuffer(bool value)
		{	
			if ((value ^ m_isEnabledDepthBuffer[m_cGraphicsThreadState]) & value)
			{
				m_deviceContext->OMSetDepthStencilState(m_depthStencilEnableState, 1);
			}
			else if ((value ^ m_isEnabledAlphaBlending[m_cGraphicsThreadState])
				& m_isEnabledAlphaBlending[m_cGraphicsThreadState])
			{
				m_deviceContext->OMSetDepthStencilState(m_depthStencilDisabledState, 1);
			}

			m_isEnabledDepthBuffer[m_cGraphicsThreadState] = value;
		}
	}
}
#endif // !SGFRAMEWORK_HEADER_GRAPHICS_X11_HPP_