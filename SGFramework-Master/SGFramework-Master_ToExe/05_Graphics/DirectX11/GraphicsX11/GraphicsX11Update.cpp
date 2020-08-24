#include "GraphicsX11.hpp"
#include "../GraphicsObject/GraphicsObjectX11.hpp"
#include "../Camera/CameraX11.hpp"
#include "../../../12_GUI/GUI.hpp"
#include "../../../08_Input/Input.hpp"

// Framework namespace
namespace SGFramework
{
	//フレームワークの管理をするAdministrator namespace
	namespace Administrator
	{
		void GraphicsX11::InitCommandList()
		{
			m_commandList.try_emplace(std::this_thread::get_id(), std::list<DrawCommand>());
			m_commandQueue.try_emplace(std::this_thread::get_id(), std::list<DrawCommand>());
		}
		void GraphicsX11::InitCommandList(const std::vector<std::thread>& subIDs)
		{
			for (auto& e : subIDs)
			{
				m_commandList.try_emplace(e.get_id(), std::list<DrawCommand>());
				m_commandQueue.try_emplace(e.get_id(), std::list<DrawCommand>());			
			}
		}
		void GraphicsX11::SortList()
		{
			m_commandQueue.at(std::this_thread::get_id()).sort();
		}
		void GraphicsX11::SyncGraphics()
		{
			//InstanceId初期化
			for(auto& e : m_nowShaderInstanceIDs)
				e = -1;
			m_nowTextureInstanceID = -1;

			//コマンドを移動
			for (auto& e : m_commandQueue)
				m_commandList[e.first].splice(m_commandList[e.first].end(), e.second);

			//Enable情報コピー
			m_isEnabledAlphaBlending[m_cGraphicsThreadState] 
				= m_isEnabledAlphaBlending[m_UpdateThreadState];
			m_isEnabledDepthBuffer[m_cGraphicsThreadState]
				= m_isEnabledDepthBuffer[m_UpdateThreadState];

			//ライトの値をコピー
			memcpy_s(&m_setConstantBufferLight.directional, sizeof(m_setConstantBufferLight.directional), &GraphicsDetail::DirectionalLightX11::m_pack, sizeof(GraphicsDetail::DirectionalLightX11::m_pack));
			memcpy_s(&m_setConstantBufferLight.point, sizeof(m_setConstantBufferLight.point), &GraphicsDetail::PointLightX11::m_pack, sizeof(GraphicsDetail::PointLightX11::m_pack));
			memcpy_s(&m_setConstantBufferLight.hemiSphere, sizeof(m_setConstantBufferLight.hemiSphere), &GraphicsDetail::HemiSphereLightX11::m_pack, sizeof(GraphicsDetail::HemiSphereLightX11::m_pack));
			
			m_isSetConstantBufferLight = GraphicsDetail::DirectionalLightX11::m_isChangeds[GraphicsDetail::DirectionalLightX11::m_cIsChangedDirection]
				| GraphicsDetail::PointLightX11::m_isChangeds[GraphicsDetail::PointLightX11::m_cIsChangedPoint] | GraphicsDetail::HemiSphereLightX11::m_isChangeds[GraphicsDetail::HemiSphereLightX11::m_cIsChangedHemiSphere];
			
			GraphicsDetail::DirectionalLightX11::m_isChangeds[GraphicsDetail::DirectionalLightX11::m_cIsChangedDirection] =
				GraphicsDetail::PointLightX11::m_isChangeds[GraphicsDetail::PointLightX11::m_cIsChangedPoint] = 
				GraphicsDetail::HemiSphereLightX11::m_isChangeds[GraphicsDetail::HemiSphereLightX11::m_cIsChangedHemiSphere] = false;

			//GraphicsObject Sync
			for (auto& e : m_addOrEraseGraphicsObjects)
			{
				if (IS_TRUE(e.e2))
					m_graphicsObjects.emplace(e.e1);
				else
					m_graphicsObjects.erase(e.e1);
			}
			m_addOrEraseGraphicsObjects.clear();
		
			for (auto& e : m_graphicsObjects)
				e->Sync();

			//Camera Init Setting
			GraphicsDetail::CameraX11::InitCameraSetting();

			//GUI Sync
			GUI::Sync();
		}
		void GraphicsX11::UpdateGraphics()
		{
			if (IS_TRUE(m_isFirstFrame))
			{
				m_isFirstFrame = false;
				return;
			}

			//GUI Update
			GUI::Update();

			for (auto& e : m_commandList)
			{
				m_makeCommand.merge(e.second);
				e.second.clear();
			}

			// ターゲットバッファクリア
			m_deviceContext->ClearRenderTargetView(m_renderTargetView, m_clearColor);
			// Zバッファクリア
			m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
			//ライト更新
			if (IS_TRUE(m_isSetConstantBufferLight))
			{
				m_deviceContext->UpdateSubresource(m_constantBufferLight, 0, nullptr, &m_setConstantBufferLight, 0, 0);
				m_deviceContext->VSSetConstantBuffers(m_cConstantBufferLightSlot, 1, &m_constantBufferLight);
				m_deviceContext->PSSetConstantBuffers(m_cConstantBufferLightSlot, 1, &m_constantBufferLight);
			}


			{
				//Lock GabageCollector
				AtomicOperation::LockGuard guard(GabageCollector::m_lock);

				//DrawCommand
				for (auto& e : m_makeCommand)
					e.drawFunctionCommand();
			}

			m_makeCommand.clear();

			GUI::DrawGUI();
			m_swapChain->Present(static_cast<uint>(m_isEnabledVsync), 0);

			//PostMessage(Window::HwndHolder::main, WM_DRAW_GRAPHICS, 0, 0);
		}
	}
}