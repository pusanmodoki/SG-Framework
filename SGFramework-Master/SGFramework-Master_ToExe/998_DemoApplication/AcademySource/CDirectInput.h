#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include	<dinput.h>
#include "../SGFrameworkToExeProject/SGFramwork.hpp"

class CDirectInput{
private:
	LPDIRECTINPUT8			m_dinput;		// DirectInput8オブジェクト
	LPDIRECTINPUTDEVICE8	m_dikeyboard;	// キーボードデバイス
	LPDIRECTINPUTDEVICE8	m_dimouse;		// マウスデバイス
	char					m_keybuffer[256];		// キーボードバッファ
	char					m_oldkeybuffer[256];	// 前回の入力キーボードバッファ
	DIMOUSESTATE2			m_diMouseState;		// マウスの状態
	float						m_MousePosX;		// マウスのＸ座標
	float						m_MousePosY;		// マウスのＹ座標
	int						m_width;			// マウスのＸ座標最大
	int						m_height;			// マウスのＹ座標最大
	CDirectInput() :m_dinput(nullptr), m_dikeyboard(nullptr), m_dimouse(nullptr), m_MousePosX(0), m_MousePosY(0){
	}
public:

	CDirectInput(const CDirectInput&) = delete;
	CDirectInput& operator=(const CDirectInput&) = delete;
	CDirectInput(CDirectInput&&) = delete;
	CDirectInput& operator=(CDirectInput&&) = delete;

	static CDirectInput& GetInstance(){
		static CDirectInput Instance;
		return Instance;
	}

	~CDirectInput(){
		Exit();
	}

	//----------------------------------
	// DirectInput 初期処理
	//
	//		P1 : インスタンス値
	//		P2 : ウインドウハンドル値
	//
	//	戻り値
	//		true : 初期化成功
	//		false : 初期化失敗
	//----------------------------------
	bool Init(HINSTANCE hInst,HWND hwnd,int width,int height){
		HRESULT	hr;
		hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&m_dinput, NULL);
		if(FAILED(hr)) {
			return false;
		}

		// キーボードデバイス生成
		m_dinput->CreateDevice(GUID_SysKeyboard, &m_dikeyboard, NULL);
		if(FAILED(hr)) {
			return false;
		}

		// データフォーマットの設定
		hr = m_dikeyboard->SetDataFormat(&c_dfDIKeyboard);
		if(FAILED(hr)) {
			return false;
		}
		
		// 協調レベルの設定
		hr = m_dikeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		if(FAILED(hr)) {
			return false;
		}

		// マウスデバイス生成
		m_dinput->CreateDevice(GUID_SysMouse, &m_dimouse, NULL);
		if(FAILED(hr)) {
			return false;
		}

		// データフォーマットの設定
		hr = m_dimouse->SetDataFormat(&c_dfDIMouse2);
		if(FAILED(hr)) {
			return false;
		}
		
		// 協調レベルの設定
		hr = m_dimouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		if(FAILED(hr)) {
			return false;
		}

		// デバイスの設定
		DIPROPDWORD diprop;
		diprop.diph.dwSize = sizeof(diprop);
		diprop.diph.dwHeaderSize = sizeof(diprop.diph);
		diprop.diph.dwObj = 0;
		diprop.diph.dwHow = DIPH_DEVICE;
		diprop.dwData = DIPROPAXISMODE_REL;							// 相対値モード
		m_dimouse->SetProperty(DIPROP_AXISMODE, &diprop.diph);		// 軸モードの設定


		DIPROPRANGE diprg;
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);
		diprg.diph.dwObj = DIJOFS_X;
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = 0;
		diprg.lMax = width - 1;

		m_dimouse->SetProperty(DIPROP_RANGE, &diprg.diph);		// Ｘ方向の範囲を指定
		diprg.diph.dwObj = DIJOFS_Y;
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = 0;
		diprg.lMax = height - 1;
		m_dimouse->SetProperty(DIPROP_RANGE, &diprg.diph);	// Ｙ方向の範囲を指定

		m_height = height;
		m_width  = width;

		return true;
	}

	//----------------------------------
	// マウス状態取得処理
	//----------------------------------
	void GetMouseState(){
		HRESULT	hr;
		// デバイスの認識
		hr = m_dimouse->Acquire();

		hr = m_dimouse->GetDeviceState(sizeof(m_diMouseState),&m_diMouseState);
		if(hr == DIERR_INPUTLOST){
			// デバイスの認識
			hr = m_dimouse->Acquire();
		}

		POINT get;
		GetCursorPos(&get);
		ScreenToClient(Window::HwndHolder::main, &get);
		

		RECT rect;
		GetClientRect(Window::HwndHolder::main, &rect);
		
		//正規化代入
		m_MousePosX = (float)get.x;// -(float)(rect.right - rect.left);
		m_MousePosY = (float)get.y;// -(float)(rect.right - rect.left);
		
		if (m_MousePosX < 0) m_MousePosX = 0;
		else if (m_MousePosX > (float)Graphics::screen.width) m_MousePosX = (float)Graphics::screen.width;
		if (m_MousePosY < 0) m_MousePosY = 0;
		else if (m_MousePosY > (float)Graphics::screen.height) m_MousePosY = (float)Graphics::screen.height;


		m_MousePosX /= ((float)Graphics::screen.width) * 0.5f;
		m_MousePosY /= ((float)Graphics::screen.height) * 0.5f;

		m_MousePosX -= 1.0f;
		m_MousePosY -= 1.0f;
	}

	//----------------------------------
	// マウスＸ座標取得処理
	//----------------------------------
	float GetMousePosX() const{
		return m_MousePosX;
	}

	//----------------------------------
	// マウスＹ座標取得処理
	//----------------------------------
	float GetMousePosY() const{
		return m_MousePosY;
	}

	//----------------------------------
	// マウス左ボタンチェック
	//----------------------------------
	bool GetMouseLButtonCheck() const{
		if(m_diMouseState.rgbButtons[0] & 0x80){
			return true;
		}else{
			return false;	
		}
	}

	//----------------------------------
	// マウス右ボタンチェック
	//----------------------------------
	bool GetMouseRButtonCheck() const{
		if(m_diMouseState.rgbButtons[1] & 0x80){
			return true;
		}else{
			return false;	
		}
	}

	//----------------------------------
	// マウス中央ボタンチェック
	//----------------------------------
	bool GetMouseCButtonCheck() const{
		if(m_diMouseState.rgbButtons[2] & 0x80){
			return true;
		}else{
			return false;	
		}
	}

	//----------------------------------
	// キーボードバッファ取得処理
	//----------------------------------
	void GetKeyBuffer(){
		HRESULT	hr;
		// デバイスの認識
		hr = m_dikeyboard->Acquire();
		// 前回の状態を保存
		memcpy(&m_oldkeybuffer,m_keybuffer,sizeof(m_keybuffer));
		hr = m_dikeyboard->GetDeviceState(sizeof(m_keybuffer),(LPVOID)&m_keybuffer);
		if(hr == DIERR_INPUTLOST){
			// デバイスの認識
			hr = m_dikeyboard->Acquire();
		}
	}

	//----------------------------------
	// キーが押されているかどうかをチェックする
	//		p1 :　チェックしたいキー番号
	//	戻り値
	//		true : 指定されたキーが押されている
	//----------------------------------
	bool CheckKeyBuffer(int keyno){
		if(m_keybuffer[keyno] & 0x80){
			return true;
		}
		else{
			return false;
		}
	}

	//----------------------------------
	// キーが押されているかどうかをチェックする
	//		p1 :　チェックしたいキー番号(トリガー)
	//	戻り値
	//		true : 指定されたキーが押されている
	//----------------------------------
	bool CheckKeyBufferTrigger(int keyno){
		if(((m_keybuffer[keyno]^m_oldkeybuffer[keyno]) & m_keybuffer[keyno]) & 0x80){
			return true;
		}
		else{
			return false;
		}
	}

	//----------------------------------
	// DirectInput 終了処理
	//----------------------------------
	void Exit(){
		if(m_dikeyboard!=nullptr){
			m_dikeyboard->Release();
		}
		if(m_dimouse!=nullptr){
			m_dimouse->Release();
		}
		if(m_dinput!=nullptr){
			m_dinput->Release();
		}
	}	
};
