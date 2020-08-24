/*----------------------------------------------------------------------------------
入力を管理するInput class
	->Update〇〇を記述するInputUpdateParts.cpp
------------------------------------------------------------------------------------*/
#include "Input.hpp"
#include "../05_Graphics/Graphics.hpp"

//Framework namespace
namespace SGFramework
{
	//----------------------------------------------------------------------------------
	//[UpdateAny]
	//Anyの更新を行う
	void Input::UpdateAny()
	{
		//初期化
		m_isAny = false;
		m_isAnyDown = false;
		m_isAnyUp = false;

		//変更処理
		if (m_isNextChangeAnyMode & m_cNextChangeAnyInfoBit)
		{
			//AnyMode変更
			m_isAnyMode = (m_isNextChangeAnyMode & m_cNextChangeAnyValueBit) ? true : false;
			//フラグ初期化
			m_isNextChangeAnyMode = 0;
			//falseならもう終了
			if (IS_FALSE(m_isAnyMode)) return;
		}

		//KeyStatesのAny確認
		for (uint i = 0; i < m_cKeyStateSize; ++i)
		{
			if (m_keyStates[m_cNowState][i] & Keybord::cGetKeyCheckBit)
				m_isAny = true;
			if (m_keyStates[m_cNowState][i] & Keybord::cGetKeyDownCheckBit) 
				m_isAnyDown = true;
			if (m_keyStates[m_cNowState][i] & Keybord::cGetKeyUpCheckBit) 
				m_isAnyUp = true;
		}

		//ここからJoystick
		if (IS_FALSE(m_isUseJoystick)) return;

		//JoystickのAny確認
		for (uint i = 0; i < m_joystickCount; ++i)
		{
			if (m_joystickStates[m_cNowState][i].dwPacketNumber == m_joystickStates[m_cOldState][i].dwPacketNumber)
				continue;

			if ((m_joystickStates[m_cNowState][i].Gamepad.wButtons & m_cJoystickAllButtonMask) != 0)
				m_isAny = true;

			if ((m_joystickStates[m_cNowState][i].Gamepad.wButtons ^ m_joystickStates[m_cOldState][i].Gamepad.wButtons)
				& m_joystickStates[m_cNowState][i].Gamepad.wButtons)
				m_isAnyDown = true;

			if ((m_joystickStates[m_cNowState][i].Gamepad.wButtons ^ m_joystickStates[m_cOldState][i].Gamepad.wButtons)
				& m_joystickStates[m_cOldState][i].Gamepad.wButtons)
				m_isAnyUp = true;
		}
	}

	//----------------------------------------------------------------------------------
	//[UpdateKeybordStateAndSendMessage]
	//KeybordStateの更新とUpdateメッセージの送信を行う
	void Input::UpdateKeybordStateAndSendMessage()
	{
		//メッセージ送信
		SendNotifyMessage(SGFramework::Window::HwndHolder::main, WM_SGFRAMEWORK_UPDATE_INPUT, 0, 0);
		//Rapid用Map更新
		for (auto& e : m_keybordStateRapids)
		{
			e.second.value = IS_TRUE(e.second.isEnd.exchange(false)) ?
				Time::unScaledWorldDeltaTime : e.second.value + Time::unScaledWorldDeltaTime;
		}
	}

	//----------------------------------------------------------------------------------
	//[UpdateMouse]
	//Mouseの更新を行う (WindowProcedure内)
	void Input::UpdateMouse()
	{
		//取得用
		static POINT s_point = {};
		
		//wheel取得
		m_mouseWheels[m_cNowState] = m_mouseWheels[m_cMessageState];
		m_mouseWheels[m_cMessageState] = 0;

		//old取得
		m_mouseAcceleration = m_mousePosition;		
		//座標取得
		GetCursorPos(&s_point);
		ScreenToClient(SGFramework::Window::HwndHolder::main, &s_point);

		//補正
		if (s_point.x < 0)
			s_point.x = 0;
		else if (s_point.x > Graphics::screen.width)
			s_point.x = Graphics::screen.width;

		if (s_point.y < 0)
			s_point.y = 0;
		else if (s_point.y > Graphics::screen.height)
			s_point.y = Graphics::screen.height;

		//代入
		m_mousePosition.x = static_cast<float>(s_point.x);
		m_mousePosition.y = static_cast<float>(s_point.y);

		//-1.0f ~ 1.0fの値へ変換
		m_mousePosition /= (Graphics::screen.windowSize().ToFloat() * 0.5f);
		m_mousePosition -= Const::Vector2::one;
		m_mousePosition.y *= -1;

		//加速度更新
		m_mouseAcceleration = m_mousePosition - m_mouseAcceleration;
	}

	//----------------------------------------------------------------------------------
	//[UpdateJoystick]
	//Joystickの更新を行う
	void Input::UpdateJoystick()
	{
		static HRESULT s_connectionResult = 0;			//接続成功リザルト
		static uint s_nowJoystick = 0;							//i
		static std::array<bool, Joystick::cJoystickMax> s_sets;

		//Oldコピー
		memcpy_s(m_joystickStates[m_cOldState].data(), m_cJoystickStateSize, m_joystickStates[m_cNowState].data(), m_cJoystickStateSize);
		//ZeroMemory
		ZeroMemory(s_sets.data(), Joystick::cJoystickMax);

		//取得ループ
		s_nowJoystick = 0;
		for (auto& e : m_joystickStates[m_cNowState])
		{
			//取得
			s_connectionResult = XInputGetState(s_nowJoystick, &e);
			
			//接続されてるか試したけどなかった or　接続されているはずなのになかった
			if ((m_joystickCount == 0 && s_connectionResult != ERROR_SUCCESS) ||
				(s_nowJoystick < m_joystickCount && s_connectionResult != ERROR_SUCCESS))
			{
				//接続数更新してループ終了
				ReConnectJoyStick();
				break;
			}
			//接続されていないはずなのにされている
			else if (s_nowJoystick >= m_joystickCount && s_connectionResult == ERROR_SUCCESS)
			{
				//接続数更新
				ReConnectJoyStick();
			}

			//value / 32767 の結果がずれるので補正
			if (e.Gamepad.sThumbLX == -32768)
				++e.Gamepad.sThumbLX;
			if (e.Gamepad.sThumbLY == -32768)
				++e.Gamepad.sThumbLY;

			if (e.Gamepad.sThumbRX == -32768)
				++e.Gamepad.sThumbRX;
			if (e.Gamepad.sThumbRY == -32768)
				++e.Gamepad.sThumbRY;

			//i++
			++s_nowJoystick;
		}

		//Rapid更新
		for (auto& e1 : m_joystickRapids)
			for(auto& e2 : e1)
		{
			e2.second.value = IS_TRUE(e2.second.isEnd.exchange(false)) ?
				Time::unScaledWorldDeltaTime : e2.second.value + Time::unScaledWorldDeltaTime;
		}

		//Vibration更新ループ (High, Low)		
		for (s_nowJoystick = 0; s_nowJoystick < 2; ++s_nowJoystick)
		{
			//キュー更新ループ
			for (auto& e : m_joystickVibrationQueues[s_nowJoystick])
			{
				//使用数到達でbreak
				if (e.useStick >= m_joystickCount) break;

				//セット可能ならセットする
				if (e.isLive & (IS_FALSE(m_joystickSetVibrations[s_nowJoystick][e.useStick].isLive) | e.isForced))
					m_joystickSetVibrations[s_nowJoystick][e.useStick] = e;
				//セット失敗
				e.isLive = false;
			}

			//SetVibratoins更新ループ
			for (auto& e : m_joystickSetVibrations[s_nowJoystick])
			{
				//設定無しでcontinue
				if (IS_FALSE(e.isLive)) continue;
				//使用数到達でbreak
				if (e.useStick >= m_joystickCount) break;

				//フェードアウト
				if (e.type & VibrationQueue::cFadeOut)
				{
					//待機
					if (e.type & VibrationQueue::cWaitState)
					{
						e.elapased += Time::unScaledWorldDeltaTime;
						if (e.elapased >= e.wait)
							e.type ^= VibrationQueue::cWaitState;
						
						//最初です
						if (e.type & VibrationQueue::StartState)
						{
							e.type ^= VibrationQueue::StartState;

							//設定
							if (s_nowJoystick == m_cHighState)
								m_joystickVibrations[e.useStick].wRightMotorSpeed = static_cast<ushort>(e.target * m_cJoystickVibrationMax);
							else if (s_nowJoystick == m_cLowState)
								m_joystickVibrations[e.useStick].wLeftMotorSpeed = static_cast<ushort>(e.target * m_cJoystickVibrationMax);

							s_sets[e.useStick] = true;
						}
					}
					//待機終了
					else
					{
						e.target -= e.speed * Time::unScaledWorldDeltaTime;
						if (e.target < 0.0f)
						{
							e.target = 0.0f;
							e.isLive = false;
						}

						//設定
						if (s_nowJoystick == m_cHighState)
							m_joystickVibrations[e.useStick].wRightMotorSpeed = static_cast<ushort>(e.target * m_cJoystickVibrationMax);
						else if (s_nowJoystick == m_cLowState)
							m_joystickVibrations[e.useStick].wLeftMotorSpeed = static_cast<ushort>(e.target * m_cJoystickVibrationMax);

						s_sets[e.useStick] = true;
					}
				}
				//即時設定
				else if (e.type & VibrationQueue::cSet)
				{
					//設定
					if (s_nowJoystick == m_cHighState)
						m_joystickVibrations[e.useStick].wRightMotorSpeed = static_cast<ushort>(e.target * m_cJoystickVibrationMax);
					else if (s_nowJoystick == m_cLowState)
						m_joystickVibrations[e.useStick].wLeftMotorSpeed = static_cast<ushort>(e.target * m_cJoystickVibrationMax);

					s_sets[e.useStick] = true;
					e.isLive = false;
				}
			}

			//実際に設定するループ
			for (int i = 0; i < 4; i++)
			{
				if (IS_TRUE(s_sets[i])) XInputSetState(i, &m_joystickVibrations[i]);
			}
		}
	}

	//----------------------------------------------------------------------------------
	//[UpdateWindow]
	//Windowの更新を行う (WindowProcedure内)
	void Input::UpdateWindow()
	{
		//取得用
		static RECT rect = {};
		
		//old取得
		m_windowAcceleration = m_windowPosition;

		//Rect取得
		GetWindowRect(SGFramework::Window::HwndHolder::main, &rect);
		//補正
		m_windowPosition.x = static_cast<float>(rect.left + (rect.right - rect.left) / 2);
		m_windowPosition.y = static_cast<float>(rect.top + (rect.bottom - rect.top) / 2);
		m_windowPosition /= Graphics::screen.m_resolution.ToFloat();
		//加速度設定
		m_windowAcceleration = m_windowPosition - m_windowAcceleration;
		
		//Rect取得
		GetClientRect(SGFramework::Window::HwndHolder::main, &rect);
		//ウィンドウサイズ設定
		Graphics::screen.ChangeWindowSize(rect.right - rect.left, rect.bottom - rect.top);
		m_windowSize = Graphics::screen.m_windowSize.ToFloat() / Graphics::screen.m_resolution.ToFloat();
	}

	//----------------------------------------------------------------------------------
	//[ReConnectJoyStick]
	// 接続数を更新する
	void Input::ReConnectJoyStick()
	{
		//XInput初期化
		XINPUT_STATE connectionCheck[4];	//接続用バッファ作成
		m_joystickCount = 0;						//接続カウント初期化
		
		//接続確認ループ
		for (uint i = 0; i < Joystick::cJoystickMax; i++)
		{
			HRESULT connectionResult;				//接続成功リザルト
			//状態取得
			connectionResult = XInputGetState(i, &connectionCheck[i]);
			//取得成功(接続している)場合は接続カウント++
			if (connectionResult == ERROR_SUCCESS)	++m_joystickCount;
		}
		
		//Callbackある場合呼び出す
		if (m_joystickCallback)
			m_joystickCallback(m_joystickCount);
	
		//flag設定
		if (m_joystickCount == 0)
			m_isUseJoystick = false;
		else
			m_isUseJoystick = true;
	}
}