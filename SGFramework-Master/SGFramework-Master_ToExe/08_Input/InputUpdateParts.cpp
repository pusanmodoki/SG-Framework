/*----------------------------------------------------------------------------------
���͂��Ǘ�����Input class
	->Update�Z�Z���L�q����InputUpdateParts.cpp
------------------------------------------------------------------------------------*/
#include "Input.hpp"
#include "../05_Graphics/Graphics.hpp"

//Framework namespace
namespace SGFramework
{
	//----------------------------------------------------------------------------------
	//[UpdateAny]
	//Any�̍X�V���s��
	void Input::UpdateAny()
	{
		//������
		m_isAny = false;
		m_isAnyDown = false;
		m_isAnyUp = false;

		//�ύX����
		if (m_isNextChangeAnyMode & m_cNextChangeAnyInfoBit)
		{
			//AnyMode�ύX
			m_isAnyMode = (m_isNextChangeAnyMode & m_cNextChangeAnyValueBit) ? true : false;
			//�t���O������
			m_isNextChangeAnyMode = 0;
			//false�Ȃ�����I��
			if (IS_FALSE(m_isAnyMode)) return;
		}

		//KeyStates��Any�m�F
		for (uint i = 0; i < m_cKeyStateSize; ++i)
		{
			if (m_keyStates[m_cNowState][i] & Keybord::cGetKeyCheckBit)
				m_isAny = true;
			if (m_keyStates[m_cNowState][i] & Keybord::cGetKeyDownCheckBit) 
				m_isAnyDown = true;
			if (m_keyStates[m_cNowState][i] & Keybord::cGetKeyUpCheckBit) 
				m_isAnyUp = true;
		}

		//��������Joystick
		if (IS_FALSE(m_isUseJoystick)) return;

		//Joystick��Any�m�F
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
	//KeybordState�̍X�V��Update���b�Z�[�W�̑��M���s��
	void Input::UpdateKeybordStateAndSendMessage()
	{
		//���b�Z�[�W���M
		SendNotifyMessage(SGFramework::Window::HwndHolder::main, WM_SGFRAMEWORK_UPDATE_INPUT, 0, 0);
		//Rapid�pMap�X�V
		for (auto& e : m_keybordStateRapids)
		{
			e.second.value = IS_TRUE(e.second.isEnd.exchange(false)) ?
				Time::unScaledWorldDeltaTime : e.second.value + Time::unScaledWorldDeltaTime;
		}
	}

	//----------------------------------------------------------------------------------
	//[UpdateMouse]
	//Mouse�̍X�V���s�� (WindowProcedure��)
	void Input::UpdateMouse()
	{
		//�擾�p
		static POINT s_point = {};
		
		//wheel�擾
		m_mouseWheels[m_cNowState] = m_mouseWheels[m_cMessageState];
		m_mouseWheels[m_cMessageState] = 0;

		//old�擾
		m_mouseAcceleration = m_mousePosition;		
		//���W�擾
		GetCursorPos(&s_point);
		ScreenToClient(SGFramework::Window::HwndHolder::main, &s_point);

		//�␳
		if (s_point.x < 0)
			s_point.x = 0;
		else if (s_point.x > Graphics::screen.width)
			s_point.x = Graphics::screen.width;

		if (s_point.y < 0)
			s_point.y = 0;
		else if (s_point.y > Graphics::screen.height)
			s_point.y = Graphics::screen.height;

		//���
		m_mousePosition.x = static_cast<float>(s_point.x);
		m_mousePosition.y = static_cast<float>(s_point.y);

		//-1.0f ~ 1.0f�̒l�֕ϊ�
		m_mousePosition /= (Graphics::screen.windowSize().ToFloat() * 0.5f);
		m_mousePosition -= Const::Vector2::one;
		m_mousePosition.y *= -1;

		//�����x�X�V
		m_mouseAcceleration = m_mousePosition - m_mouseAcceleration;
	}

	//----------------------------------------------------------------------------------
	//[UpdateJoystick]
	//Joystick�̍X�V���s��
	void Input::UpdateJoystick()
	{
		static HRESULT s_connectionResult = 0;			//�ڑ��������U���g
		static uint s_nowJoystick = 0;							//i
		static std::array<bool, Joystick::cJoystickMax> s_sets;

		//Old�R�s�[
		memcpy_s(m_joystickStates[m_cOldState].data(), m_cJoystickStateSize, m_joystickStates[m_cNowState].data(), m_cJoystickStateSize);
		//ZeroMemory
		ZeroMemory(s_sets.data(), Joystick::cJoystickMax);

		//�擾���[�v
		s_nowJoystick = 0;
		for (auto& e : m_joystickStates[m_cNowState])
		{
			//�擾
			s_connectionResult = XInputGetState(s_nowJoystick, &e);
			
			//�ڑ�����Ă邩���������ǂȂ����� or�@�ڑ�����Ă���͂��Ȃ̂ɂȂ�����
			if ((m_joystickCount == 0 && s_connectionResult != ERROR_SUCCESS) ||
				(s_nowJoystick < m_joystickCount && s_connectionResult != ERROR_SUCCESS))
			{
				//�ڑ����X�V���ă��[�v�I��
				ReConnectJoyStick();
				break;
			}
			//�ڑ�����Ă��Ȃ��͂��Ȃ̂ɂ���Ă���
			else if (s_nowJoystick >= m_joystickCount && s_connectionResult == ERROR_SUCCESS)
			{
				//�ڑ����X�V
				ReConnectJoyStick();
			}

			//value / 32767 �̌��ʂ������̂ŕ␳
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

		//Rapid�X�V
		for (auto& e1 : m_joystickRapids)
			for(auto& e2 : e1)
		{
			e2.second.value = IS_TRUE(e2.second.isEnd.exchange(false)) ?
				Time::unScaledWorldDeltaTime : e2.second.value + Time::unScaledWorldDeltaTime;
		}

		//Vibration�X�V���[�v (High, Low)		
		for (s_nowJoystick = 0; s_nowJoystick < 2; ++s_nowJoystick)
		{
			//�L���[�X�V���[�v
			for (auto& e : m_joystickVibrationQueues[s_nowJoystick])
			{
				//�g�p�����B��break
				if (e.useStick >= m_joystickCount) break;

				//�Z�b�g�\�Ȃ�Z�b�g����
				if (e.isLive & (IS_FALSE(m_joystickSetVibrations[s_nowJoystick][e.useStick].isLive) | e.isForced))
					m_joystickSetVibrations[s_nowJoystick][e.useStick] = e;
				//�Z�b�g���s
				e.isLive = false;
			}

			//SetVibratoins�X�V���[�v
			for (auto& e : m_joystickSetVibrations[s_nowJoystick])
			{
				//�ݒ薳����continue
				if (IS_FALSE(e.isLive)) continue;
				//�g�p�����B��break
				if (e.useStick >= m_joystickCount) break;

				//�t�F�[�h�A�E�g
				if (e.type & VibrationQueue::cFadeOut)
				{
					//�ҋ@
					if (e.type & VibrationQueue::cWaitState)
					{
						e.elapased += Time::unScaledWorldDeltaTime;
						if (e.elapased >= e.wait)
							e.type ^= VibrationQueue::cWaitState;
						
						//�ŏ��ł�
						if (e.type & VibrationQueue::StartState)
						{
							e.type ^= VibrationQueue::StartState;

							//�ݒ�
							if (s_nowJoystick == m_cHighState)
								m_joystickVibrations[e.useStick].wRightMotorSpeed = static_cast<ushort>(e.target * m_cJoystickVibrationMax);
							else if (s_nowJoystick == m_cLowState)
								m_joystickVibrations[e.useStick].wLeftMotorSpeed = static_cast<ushort>(e.target * m_cJoystickVibrationMax);

							s_sets[e.useStick] = true;
						}
					}
					//�ҋ@�I��
					else
					{
						e.target -= e.speed * Time::unScaledWorldDeltaTime;
						if (e.target < 0.0f)
						{
							e.target = 0.0f;
							e.isLive = false;
						}

						//�ݒ�
						if (s_nowJoystick == m_cHighState)
							m_joystickVibrations[e.useStick].wRightMotorSpeed = static_cast<ushort>(e.target * m_cJoystickVibrationMax);
						else if (s_nowJoystick == m_cLowState)
							m_joystickVibrations[e.useStick].wLeftMotorSpeed = static_cast<ushort>(e.target * m_cJoystickVibrationMax);

						s_sets[e.useStick] = true;
					}
				}
				//�����ݒ�
				else if (e.type & VibrationQueue::cSet)
				{
					//�ݒ�
					if (s_nowJoystick == m_cHighState)
						m_joystickVibrations[e.useStick].wRightMotorSpeed = static_cast<ushort>(e.target * m_cJoystickVibrationMax);
					else if (s_nowJoystick == m_cLowState)
						m_joystickVibrations[e.useStick].wLeftMotorSpeed = static_cast<ushort>(e.target * m_cJoystickVibrationMax);

					s_sets[e.useStick] = true;
					e.isLive = false;
				}
			}

			//���ۂɐݒ肷�郋�[�v
			for (int i = 0; i < 4; i++)
			{
				if (IS_TRUE(s_sets[i])) XInputSetState(i, &m_joystickVibrations[i]);
			}
		}
	}

	//----------------------------------------------------------------------------------
	//[UpdateWindow]
	//Window�̍X�V���s�� (WindowProcedure��)
	void Input::UpdateWindow()
	{
		//�擾�p
		static RECT rect = {};
		
		//old�擾
		m_windowAcceleration = m_windowPosition;

		//Rect�擾
		GetWindowRect(SGFramework::Window::HwndHolder::main, &rect);
		//�␳
		m_windowPosition.x = static_cast<float>(rect.left + (rect.right - rect.left) / 2);
		m_windowPosition.y = static_cast<float>(rect.top + (rect.bottom - rect.top) / 2);
		m_windowPosition /= Graphics::screen.m_resolution.ToFloat();
		//�����x�ݒ�
		m_windowAcceleration = m_windowPosition - m_windowAcceleration;
		
		//Rect�擾
		GetClientRect(SGFramework::Window::HwndHolder::main, &rect);
		//�E�B���h�E�T�C�Y�ݒ�
		Graphics::screen.ChangeWindowSize(rect.right - rect.left, rect.bottom - rect.top);
		m_windowSize = Graphics::screen.m_windowSize.ToFloat() / Graphics::screen.m_resolution.ToFloat();
	}

	//----------------------------------------------------------------------------------
	//[ReConnectJoyStick]
	// �ڑ������X�V����
	void Input::ReConnectJoyStick()
	{
		//XInput������
		XINPUT_STATE connectionCheck[4];	//�ڑ��p�o�b�t�@�쐬
		m_joystickCount = 0;						//�ڑ��J�E���g������
		
		//�ڑ��m�F���[�v
		for (uint i = 0; i < Joystick::cJoystickMax; i++)
		{
			HRESULT connectionResult;				//�ڑ��������U���g
			//��Ԏ擾
			connectionResult = XInputGetState(i, &connectionCheck[i]);
			//�擾����(�ڑ����Ă���)�ꍇ�͐ڑ��J�E���g++
			if (connectionResult == ERROR_SUCCESS)	++m_joystickCount;
		}
		
		//Callback����ꍇ�Ăяo��
		if (m_joystickCallback)
			m_joystickCallback(m_joystickCount);
	
		//flag�ݒ�
		if (m_joystickCount == 0)
			m_isUseJoystick = false;
		else
			m_isUseJoystick = true;
	}
}