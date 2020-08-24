/*----------------------------------------------------------------------------------
入力を管理するInput class
------------------------------------------------------------------------------------*/
#include "Input.hpp"
#include "../97_Application/Application.hpp"

//Framework namespace
namespace SGFramework
{
	//連射間隔秒数, 全Input共通
	GetOnlyProperty<float> Input::rapidInterval = Input::m_rapidInterval;

	//ReConnectJoyStick時に呼ぶコールバック
	std::function<void(uint joystickCount)> Input::m_joystickCallback;
	//GetRapid用マップ
	std::unordered_map<int, Input::Rapid> Input::m_keybordStateRapids;
	//GetRapid用vector
	std::vector<int> Input::m_addKeybordStateRapids;
	//GetRapid用マップ
	std::array<std::unordered_map<ushort, Input::Rapid>, Input::Joystick::cJoystickMax>  Input::m_joystickRapids;
	//GetRapid用vector
	std::vector <Tuple::Two<ushort, uint>> Input::m_addJoystickRapids;
	//GetAxisUser用マップ
	std::unordered_map<sgstring, Input::AxisPack> Input::m_axisPacks;
	//GetKeyDown or Up用vector
	std::vector<uint> Input::m_getKeyDownOrUpVkeys;
	//GetKeyState用配列
	std::array<std::array<byte, Input::m_cKeyStateSize>, 2> Input::m_keyStates;
	//XIpnput->Input用配列
	std::array<std::array<XINPUT_STATE, Input::Joystick::cJoystickMax>, 2> Input::m_joystickStates;
	//XInput->Vibration用配列
	std::array<XINPUT_VIBRATION, Input::Joystick::cJoystickMax>  Input::m_joystickVibrations;
	//XInput->Vibration->更新用配列
	std::array<std::array<Input::VibrationQueue, Input::Joystick::cJoystickMax>, 2> Input::m_joystickSetVibrations;
	//XInput->Vibration->更新用キュー配列
	std::array<std::array<Input::VibrationQueue, Input::Joystick::cJoystickMax>, 2> Input::m_joystickVibrationQueues;
	//マウスホイール
	std::array<short, 2> Input::m_mouseWheels = { 0, 0 };
	//WindowProcedure用条件変数
	ConditionVariable::Auto Input::m_inputCompletedCondition;
	Vector2 Input::m_mousePosition = Const::Vector2::zero;				//マウスの位置
	Vector2 Input::m_mouseAcceleration = Const::Vector2::zero;		//マウス加速度
	Vector2 Input::m_windowPosition = Const::Vector2::zero;			//ウィンドウのポジション
	Vector2 Input::m_windowSize = Const::Vector2::zero;					//ウィンドウのサイズ
	Vector2 Input::m_windowAcceleration = Const::Vector2::zero;		//ウィンドウ加速度
	float Input::m_rapidInterval = 0.0f;												//連射間隔
	float Input::m_mouseDedZone = 0.0f;											//マウスのデッドゾーン
	float Input::m_joystickDedZone = 0.0f;										//ジョイスティックのデッドゾーン
	float Input::m_windowDedZone = 0.0f;										//ウィンドウのデッドゾーン
	
	//次フレームAnyMode更新フラグ
	std::atomic<byte> Input::m_isNextChangeAnyMode = 0;									
	//Any有効か否か
	std::atomic<bool> Input::m_isAnyMode = false;
	//rapid atomic
	std::atomic<bool> Input::m_rapidAtomic = false;
	//joystick atomic
	std::atomic<bool> Input::m_joystickAtomic = false;
	//axis atomic
	std::atomic<bool> Input::m_axisAtomic=false;
	byte Input::m_joystickCount = 0;												//ジョイスティック接続数	
	bool Input::m_isAny = false;														//GetAny
	bool Input::m_isAnyDown = false;												//GetAnyDown
	bool Input::m_isAnyUp = false;													//GetAnyUp
	bool Input::m_isUseJoystick = false;											//ジョイスティック接続数 > 0
	bool Input::m_isMouseInsideFrame = false;									//マウスをウィンドウ内に留めるか

	//次フレーム更新ビット
	GetOnlyProperty<std::atomic<bool>> Input::Any::isAnyMode = Input::m_isAnyMode;
	//ウィンドウの大きさを0.0f ~ 1.0fとした場合の現在のマウス座標
	GetOnlyProperty<Vector2> Input::Mouse::position = Input::m_mousePosition;
	//dedZoneの影響を受けない前フレームからのポジションの移動量
	GetOnlyProperty<Vector2> Input::Mouse::rawAcceleration = Input::m_mouseAcceleration;
	//GetAccelerationに使用されるデッドゾーン
	GetOnlyProperty<float> Input::Mouse::dedZone = Input::m_mouseDedZone;
	//現在のwheel遷移状態 (奥 -> +1~, 手前 -> -1~)
	GetOnlyProperty<short> Input::Mouse::wheel = Input::m_mouseWheels[Input::m_cNowState];
	//trueの場合マウスカーソルがウィンドウの外へでなくなる
	GetOnlyProperty<bool> Input::Mouse::isMouseInsideFrame = Input::m_isMouseInsideFrame;;
	//デッドゾーン
	GetOnlyProperty<float> Input::Joystick::dedZone = Input::m_joystickDedZone;
	//現在の接続数
	GetOnlyProperty<byte> Input::Joystick::numUseJoysticks = Input::m_joystickCount;
	//現在の接続数 > 0
	GetOnlyProperty<bool> Input::Joystick::isUseJoystick = Input::m_isUseJoystick;
	//画面解像度を 0.0f ~ 1.0fとした場合のウィンドウの中心座標
	GetOnlyProperty<Vector2> Input::Window::position = Input::m_windowPosition;
	//画面解像度を 0.0f ~ 1.0fとした場合のウィンドウのサイズ
	GetOnlyProperty<Vector2> Input::Window::size = Input::m_windowSize;
	//画面解像度を 0.0f ~ 1.0fとした場合のdedZoneの影響を受けない前フレームからのウィンドウ移動量
	GetOnlyProperty<Vector2> Input::Window::rawAcceleration = Input::m_windowAcceleration;
	//デッドゾーン
	GetOnlyProperty<float> Input::Window::dedZone = Input::m_windowDedZone;

	//----------------------------------------------------------------------------------
	//[StartUp]
	//起動時初期化を行う
	//引数1: 連射間隔
	//引数2: マウスのデッドゾーン
	//引数3: ジョイスティックのデッドゾーン
	//引数4: ウィンドウのデッドゾーン
	//引数5: マウスをウィンドウ内に留めるか
	void Input::StartUp(const ReadElement::Pack& rapidInterval, const ReadElement::Pack& mouseDedZone,
		const ReadElement::Pack& joystickDedZone, const ReadElement::Pack& windowDedZone, const ReadElement::Pack& isMouseInsideFrame)
	{
		//ReadElementから取得
		try
		{
			m_rapidInterval = rapidInterval[0].valueFloat;
			m_mouseDedZone = mouseDedZone[0].valueFloat;
			m_joystickDedZone = joystickDedZone[0].valueFloat;
			m_windowDedZone = windowDedZone[0].valueFloat;
			m_isMouseInsideFrame = static_cast<bool>(isMouseInsideFrame[0].valueInt);
		}
		catch (...)
		{
			throw;
		}	

		//ZeroMemory
		ZeroMemory(m_keyStates[m_cNowState].data(), m_cKeyStateSize);
		ZeroMemory(m_keyStates[m_cOldState].data(), m_cKeyStateSize);
		ZeroMemory(m_joystickStates[m_cNowState].data(), m_cJoystickStateSize);
		ZeroMemory(m_joystickStates[m_cOldState].data(), m_cJoystickStateSize);

		//マウスをウィンドウに留める
		if (IS_TRUE(m_isMouseInsideFrame))
		{
			RECT rect = {};			
			DwmGetWindowAttribute(SGFramework::Window::HwndHolder::main, DWMWA_EXTENDED_FRAME_BOUNDS, &rect, sizeof(rect));
			ClipCursor(&rect);
		}

		//XInput初期化
		XINPUT_STATE connectionCheck[4];	//接続用バッファ作成
		m_joystickCount = 0;						//接続カウント初期化
		//接続確認ループ
		for (uint i = 0; i < 4; i++)
		{
			HRESULT connectionResult;				//接続成功リザルト
			//状態取得
			connectionResult = XInputGetState(i, &connectionCheck[i]);
			//取得成功(接続している)場合は接続カウント++
			if (connectionResult == ERROR_SUCCESS)	++m_joystickCount;
		}

		//フラグセット
		if (m_joystickCount > 0)
			m_isUseJoystick = true;
		else
			m_isUseJoystick = false;
	}

	//----------------------------------------------------------------------------------
	//[Update]
	//更新を行う
	void Input::Update()
	{
		//Rapid追加更新
		for (auto& e : m_addKeybordStateRapids)
			m_keybordStateRapids.try_emplace(e, 0.0f, false);
		for (auto& e : m_addJoystickRapids)
			m_joystickRapids[e.e2].try_emplace(e.e1, 0.0f, false);

		//KeyState更新 & メッセージ送信
		UpdateKeybordStateAndSendMessage();
		//Joystick更新
		UpdateJoystick();
		//WindowProcedureの更新終了を待機
		m_inputCompletedCondition.Wait();
		//AnyModeならAny更新
		if (IS_TRUE(m_isAnyMode) || (m_isNextChangeAnyMode & m_cNextChangeAnyInfoBit))
			UpdateAny();
	}

	//----------------------------------------------------------------------------------
	//[Shutdown]
	//終了処理を行う
	void Input::Shutdown()
	{
		//マウスを留める処理を行っている場合は戻す
		if (IS_TRUE(m_isMouseInsideFrame))
		{
			RECT rect = {};
			GetClientRect(GetDesktopWindow(), &rect);
			ClipCursor(&rect);
		}
	}

	//----------------------------------------------------------------------------------
	//[UpdateWindowProcedure]
	//更新を行う WindowProcedure ver
	void Input::UpdateWindowProcedure()
	{
		//現在のキーボード状態コピー
		memcpy_s(m_keyStates[m_cNowState].data(), m_cKeyStateSize, m_keyStates[m_cMessageState].data(), m_cKeyStateSize);
		
		//GetKeyDown & GetKeyUp処理, ビット除去
		for (auto& e : m_getKeyDownOrUpVkeys)
		{
			if (e & m_cKeyStatesEmplaceUp)
				m_keyStates[m_cMessageState][e ^ m_cKeyStatesEmplaceUp] = 0;
			else
				m_keyStates[m_cMessageState][e ^ m_cKeyStatesEmplaceDown] ^= Keybord::cGetKeyDownCheckBit;
		}
		m_getKeyDownOrUpVkeys.clear();
		
		//マウス更新
		UpdateMouse();
		//ウィンドウ更新
		UpdateWindow();
		//終了イベント
		m_inputCompletedCondition.NotifyOne();
	}

	//----------------------------------------------------------------------------------
	//[ShutdownWindowProcedure]
	//終了処理を行う WindowProcedure ver
	void Input::ShutdownWindowProcedure()
	{
		//終了イベント, メインスレッド停止防止
		m_inputCompletedCondition.NotifyOne();
	}
}