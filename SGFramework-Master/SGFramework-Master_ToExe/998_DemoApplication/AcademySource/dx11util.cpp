#include	<d3d11.h>
#include	<DirectXMath.h>
#include	"CDirectxGraphics.h"
#include	"Shader.h"
#include	"CCamera.h"
#include	"memory.h"
#include	"CLight.h"
#include	"DX11Settransform.h"

#include "../SGFrameworkToExeProject/SGFramwork.hpp"
//CDirectXGraphics	g_DXGrobj;			// �c�h�q�d�b�s�w�@�f�q�`�o�g�h�b�r�@�I�u�W�F�N�g		
										// �萔�o�b�t�@��`
ID3D11Buffer*       g_pConstantBuffer1 = nullptr;	// �R���X�^���g�o�b�t�@1

CLight				g_directionallight;

void DX11LightInit(DirectX::XMFLOAT4 lightpos) {
	g_directionallight.Init(CCamera::GetInstance()->GetEye(), lightpos);
}

void DX11LightUninit() {
	g_directionallight.Uninit();
}

void DX11LightUpdate(DirectX::XMFLOAT4 lpos) {
	g_directionallight.SetEyePos(CCamera::GetInstance()->GetEye());
	g_directionallight.SetLightPos(lpos);
	g_directionallight.Update();
}
\
ID3D11Device* GetDX11Device() {
	return SGFramework::Graphics::device();
}

ID3D11DeviceContext*	GetDX11DeviceContext() {
	return SGFramework::Graphics::context();
}

bool DX11Init(HWND hwnd, int width, int height, bool fullscreen) {
	bool sts;
	//\

	//g_DXGrobj.TurnOnAlphaBlending();

	sts = DX11SetTransform::GetInstance()->Init();
	if (!sts) {
		MessageBox(NULL, L"SetTransform error", L"Error", MB_OK);
		return false;
	}

	return true;
}

void DX11Uninit() {

	DX11SetTransform::GetInstance()->Uninit();

	// �������
	//g_DXGrobj.Exit();

}

void DX11BeforeRender(float ClearColor[]) {
	// �^�[�Q�b�g�o�b�t�@�N���A
	//g_DXGrobj.GetImmediateContext()->ClearRenderTargetView(g_DXGrobj.GetRenderTargetView(), ClearColor);
	// Z�o�b�t�@�N���A
	//g_DXGrobj.GetImmediateContext()->ClearDepthStencilView(g_DXGrobj.GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void DX11AfterRender() {
	// ��������
//	g_DXGrobj.GetSwapChain()->Present(0, 0);
}


