#pragma once
#include	<d3d11.h>

class CDirectXGraphics{
private:
	ID3D11Device*				m_lpDevice;				// DIRECT3DDEVICE11デバイス
	ID3D11DeviceContext*		m_lpImmediateContext;	// DIRECT3DDEVICE11デバイスコンテキスト
	IDXGISwapChain*				m_lpSwapChain;			// スワップチェイン

	ID3D11RenderTargetView*		m_lpRenderTargetView;	// レンダーターゲットビュー
	ID3D11Texture2D*			m_depthStencilBuffer;	// Ｚバッファ、ステンシルバッファ
	ID3D11DepthStencilState*	m_depthStencilEnableState;	// Ｚバッファ、ステンシルステート
	ID3D11DepthStencilView*		m_depthStencilView;		// Ｚバッファ、ステンシルビュー
	ID3D11RasterizerState*		m_rasterState;			// ラスターステータス

	D3D_DRIVER_TYPE				m_DriverType;			// ドライバタイプ
	D3D_FEATURE_LEVEL			m_FeatureLevel;			// 機能レベル

	ID3D11BlendState*			m_alphaEnableBlendingState;
	ID3D11BlendState*			m_alphaDisableBlendingState;

	int							m_Width;				// バックバッファＸサイズ
	int							m_Height;				// バックバッファＹサイズ
public:
	CDirectXGraphics(){								// コンストラクタ
		m_lpDevice=NULL;
		m_lpImmediateContext=NULL;
		m_Height=0;
		m_Width = 0;
	}

	// 初期処理
	bool Init(HWND hWnd, unsigned int Width, unsigned int Height, bool fullscreen = false);
	
	// 終了処理
	void	Exit();														

	// デバイスGET
	ID3D11Device*	GetDXDevice() const {							
		return m_lpDevice;
	}

	// イミディエイトデバイスGET
	ID3D11DeviceContext* GetImmediateContext() const {
		return m_lpImmediateContext;
	}

	// SWAPチェインGET
	IDXGISwapChain* GetSwapChain()const {
		return m_lpSwapChain;
	}

	// レンダリングターゲットGET
	ID3D11RenderTargetView* GetRenderTargetView() const{
		return m_lpRenderTargetView;
	}

	// depthstencil view
	ID3D11DepthStencilView* GetDepthStencilView() const{
		return m_depthStencilView;
	}

	void TurnOnAlphaBlending();
	void TurnOffAlphaBlending();
};
