//*****************************************************************************
//!	@file	game.cpp
//!	@brief	
//!	@note	ゲーム処理
//!	@author	
//*****************************************************************************

//-----------------------------------------------------------------------------
//	Include header files.
//-----------------------------------------------------------------------------
#include <windows.h>
#include <stdio.h>
#include <Crtdbg.h>
#include <DirectXMath.h>
#include "DX11util.h"
#include "CModel.h"
#include "game.h"
#include "CCamera.h"
#include "CDirectInput.h"
#include "DX11Settransform.h"
#include "dx11mathutil.h"
#include "explosion.h"
#include <random>

//-----------------------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------------------
CModel				g_airplane;			// 飛行機
XMFLOAT4X4			g_matairplane;		// ワールド変換行列（飛行機）

CModel				g_skydome;			// スカイドーム
XMFLOAT4X4			g_matskydome;		// ワールド変換行列（スカイドーム）

float g_speed = 10.0f;
float g_rotSpeed = 40.0f;

bool g_explosionflag = false;			// 爆発フラグ

//==============================================================================
//!	@fn		GameInit
//!	@brief	ゲーム初期処理
//!	@param	インスタンス値
//!	@param	ウインドウハンドル値
//!	@param	ウインドウサイズ幅（補正済みの値）
//!	@param	ウインドウサイズ高さ（補正済みの値）
//!	@param　	フルスクリーンフラグ　true ; フルスクリーン　false : ウインドウ
//!	@retval	true 成功　false 失敗
//==============================================================================
bool GameInit(HINSTANCE hinst, HWND hwnd, int width, int height, bool fullscreen)
{
	bool		sts;

	// DX11初期処理
	sts = DX11Init(hwnd,width,height,fullscreen);
	if (!sts) {
		MessageBox(hwnd, L"DX11 init error", L"error", MB_OK);
		return false;
	}

	// DIRECTINPUT初期化
	CDirectInput::GetInstance().Init(hinst, hwnd, width, height);

	// 飛行機を初期化
	g_airplane.Init("assets/f1.x.dat","shader/vs.fx","shader/ps.fx");

	// スカイドーム読み込み
	g_skydome.Init("assets/skydome.x.dat","shader/vs.fx","shader/psskydome.fx");

	DX11MatrixIdentity(g_matskydome);
	DX11MatrixIdentity(g_matairplane);

	// カメラ変換行列初期化
	// プロジェクション変換行列初期化
	XMFLOAT3 eye = {0,0,-10};				// 視点
	XMFLOAT3 lookat = { 0,0,0 };			// 注視点
	XMFLOAT3 up = {0,1,0};					// 上向きベクトル

	CCamera::GetInstance()->Init(1.0f, 10000.0f, XM_PIDIV2, SCREEN_X, SCREEN_Y, eye, lookat, up, hwnd);

	// 平行光源初期化
	DX11LightInit(DirectX::XMFLOAT4(1,1,-1,0));		// 平行光源の方向をセット

	// 爆発初期化
	ExplosionInit(g_airplane.GetDatFileData());

	return	true;
}

//==============================================================================
//!	@fn		GameInput
//!	@brief	ゲーム入力検知処理
//!	@param　	
//!	@retval	なし
//==============================================================================
void GameInput(){
	CDirectInput::GetInstance().GetKeyBuffer();			// キー入力取得
}

//==============================================================================
//!	@fn		GameUpdate
//!	@brief	ゲーム更新処理
//!	@param　	
//!	@retval	なし
//==============================================================================
void GameUpdate(){

	static XMFLOAT3		angle = XMFLOAT3();
	static XMFLOAT3		trans;
	static XMFLOAT3		zpos = { 0,0,0 };

	static bool keyinput = true;

	angle.x = 0.0f;
	angle.y = 0.0f;
	angle.z = 0.0f;


	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_S)) {
		angle.x += 30.0f* 0.016f;
		keyinput = true;
	}
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_W)) {
		angle.x -= 30.0f* 0.016f;
		keyinput = true;
	}
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_D)) {
		angle.y += 30.0f* 0.016f;
		keyinput = true;
	}

	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_A)) {
		angle.y -= 30.0f* 0.016f;
		keyinput = true;
	}
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_F1)) {
		angle.z += 30.0f* 0.016f;
		keyinput = true;
	}

	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_F2)) {
		angle.z -= 30.0f* 0.016f;
		keyinput = true;
	}

	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_SPACE)) {
		g_explosionflag = !g_explosionflag;
		if (g_explosionflag) {
			TriangleTransforms(g_matairplane);
		}
	}


	
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_Q))
	{
		g_speed += 1.0f * 0.016f;
	}

	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_E))
	{
		g_speed -= 1.0f * 0.016f;
		if (g_speed < 0.0f)
			g_speed = 0.0f;
	}


	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_R))
	{
		g_rotSpeed += 1.0f * 0.016f;
	}

	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_F))
	{
		g_rotSpeed -= 1.0f * 0.016f;
		if (g_rotSpeed < 0.0f)
			g_rotSpeed = 0.0f;
	}


	XMFLOAT3 direction;
	direction.x = g_matairplane._31;
	direction.y = g_matairplane._32;
	direction.z = g_matairplane._33;

	/*XMVECTOR vec = DirectX::XMLoadFloat3(&direction);
	DirectX::XMVector3Normalize(vec);
	DirectX::XMStoreFloat3(&direction, vec);
*/
	direction.x *= g_speed * 0.016f;
	direction.y *= g_speed * 0.016f;
	direction.z *= g_speed * 0.016f;

	trans.x += direction.x;
	trans.y += direction.y;
	trans.z += direction.z;



	int idx = 0;

	if (g_explosionflag) {
		ExplosionUpdate();
	}
	else {
		if (keyinput) {
			XMFLOAT4 axisX, axisY, axisZ;
			XMFLOAT4 rotX, rotY, rotZ;
			XMFLOAT4 now;

			axisX.x = g_matairplane._11;
			axisX.y = g_matairplane._12;
			axisX.z = g_matairplane._13;

			axisY.x = g_matairplane._21;
			axisY.y = g_matairplane._22;
			axisY.z = g_matairplane._23;

			axisZ.x = g_matairplane._31;
			axisZ.y = g_matairplane._32;
			axisZ.z = g_matairplane._33;
			
			DX11GetQtfromMatrix(g_matairplane, now);
		
			DX11QtRotationAxis(rotX, axisX, angle.x);
			DX11QtRotationAxis(rotY, axisY, angle.y);
			DX11QtRotationAxis(rotZ, axisZ, angle.z);

			XMFLOAT4 quaternion1, quaternion2, quaternion3;

			DX11QtMul(quaternion1, now, rotX);
			DX11QtMul(quaternion2, rotY, rotZ);
			DX11QtMul(quaternion3, quaternion1, quaternion2);
			DX11QtNormalize(quaternion3, quaternion3);
			
			DX11MtxFromQt(g_matairplane, quaternion3);
			//DX11MakeWorldMatrix(g_matairplane, angle, trans);
	
		}
		g_matairplane._41 = 0.0f;
		g_matairplane._42 = 0.0f;
		g_matairplane._43 = 0.0f;

		XMMATRIX matrixRot = XMLoadFloat4x4(&g_matairplane);
		XMMATRIX matrixTrans = XMMatrixTranslation(trans.x, trans.y, trans.z);

		XMStoreFloat4x4(&g_matairplane, matrixRot * matrixTrans);
	}


	DirectX::XMFLOAT4 rotation;
	DirectX::XMStoreFloat4(&rotation, DirectX::XMQuaternionRotationMatrix(DirectX::XMLoadFloat4x4(&g_matairplane)));
	CCamera::GetInstance()->Update(XMFLOAT3(g_matairplane._41, g_matairplane._42, g_matairplane._43), rotation);

	keyinput = false;
}

//==============================================================================
//!	@fn		GameRender
//!	@brief	ゲーム更新処理
//!	@param　	
//!	@retval	なし
//==============================================================================
void GameRender(){
	// ターゲットバッファクリア	
	//float ClearColor[4] = { 0.0f, 0.0f, 1.0f, 1.0f }; //red,green,blue,alpha
	//
	//DirectX::XMFLOAT4X4 mat;

	//// レンダリング前処理
	//DX11BeforeRender(ClearColor);

	//// ビュー変換行列セット
	//mat = CCamera::GetInstance()->GetCameraMatrix();
	//DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::VIEW, mat);

	//// プロジェクション変換行列セット
	//mat = CCamera::GetInstance()->GetProjectionMatrix();
	//DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::PROJECTION, mat);

	//// ワールド変換行列
	//DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::WORLD, g_matskydome);
	//g_skydome.Draw();

	//if (g_explosionflag) {
	//	ExplosionDraw();
	//}
	//else {
	//	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::WORLD, g_matairplane);
	//	// 飛行機描画
	//	g_airplane.Draw();
	//}

	//// レンダリング後処理
	//DX11AfterRender();
}

//==============================================================================
//!	@fn		GameMain
//!	@brief	ゲームループ処理
//!	@param　	
//!	@retval	なし
//==============================================================================
void GameMain()
{
	GameInput();					// 入力
	GameUpdate();					// 更新
	GameRender();					// 描画
}

//==============================================================================
//!	@fn		GameExit
//!	@brief	ゲーム終了処理
//!	@param　	
//!	@retval	なし
//==============================================================================
void GameExit()
{
	ExplosionExit();
	DX11Uninit();
}

//******************************************************************************
//	End of file.
//******************************************************************************
