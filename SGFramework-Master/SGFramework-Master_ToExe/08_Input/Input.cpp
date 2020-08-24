/*----------------------------------------------------------------------------------
���͂��Ǘ�����Input class
------------------------------------------------------------------------------------*/
#include "Input.hpp"
#include "../97_Application/Application.hpp"

//Framework namespace
namespace SGFramework
{
	//�A�ˊԊu�b��, �SInput����
	GetOnlyProperty<float> Input::rapidInterval = Input::m_rapidInterval;

	//ReConnectJoyStick���ɌĂԃR�[���o�b�N
	std::function<void(uint joystickCount)> Input::m_joystickCallback;
	//GetRapid�p�}�b�v
	std::unordered_map<int, Input::Rapid> Input::m_keybordStateRapids;
	//GetRapid�pvector
	std::vector<int> Input::m_addKeybordStateRapids;
	//GetRapid�p�}�b�v
	std::array<std::unordered_map<ushort, Input::Rapid>, Input::Joystick::cJoystickMax>  Input::m_joystickRapids;
	//GetRapid�pvector
	std::vector <Tuple::Two<ushort, uint>> Input::m_addJoystickRapids;
	//GetAxisUser�p�}�b�v
	std::unordered_map<sgstring, Input::AxisPack> Input::m_axisPacks;
	//GetKeyDown or Up�pvector
	std::vector<uint> Input::m_getKeyDownOrUpVkeys;
	//GetKeyState�p�z��
	std::array<std::array<byte, Input::m_cKeyStateSize>, 2> Input::m_keyStates;
	//XIpnput->Input�p�z��
	std::array<std::array<XINPUT_STATE, Input::Joystick::cJoystickMax>, 2> Input::m_joystickStates;
	//XInput->Vibration�p�z��
	std::array<XINPUT_VIBRATION, Input::Joystick::cJoystickMax>  Input::m_joystickVibrations;
	//XInput->Vibration->�X�V�p�z��
	std::array<std::array<Input::VibrationQueue, Input::Joystick::cJoystickMax>, 2> Input::m_joystickSetVibrations;
	//XInput->Vibration->�X�V�p�L���[�z��
	std::array<std::array<Input::VibrationQueue, Input::Joystick::cJoystickMax>, 2> Input::m_joystickVibrationQueues;
	//�}�E�X�z�C�[��
	std::array<short, 2> Input::m_mouseWheels = { 0, 0 };
	//WindowProcedure�p�����ϐ�
	ConditionVariable::Auto Input::m_inputCompletedCondition;
	Vector2 Input::m_mousePosition = Const::Vector2::zero;				//�}�E�X�̈ʒu
	Vector2 Input::m_mouseAcceleration = Const::Vector2::zero;		//�}�E�X�����x
	Vector2 Input::m_windowPosition = Const::Vector2::zero;			//�E�B���h�E�̃|�W�V����
	Vector2 Input::m_windowSize = Const::Vector2::zero;					//�E�B���h�E�̃T�C�Y
	Vector2 Input::m_windowAcceleration = Const::Vector2::zero;		//�E�B���h�E�����x
	float Input::m_rapidInterval = 0.0f;												//�A�ˊԊu
	float Input::m_mouseDedZone = 0.0f;											//�}�E�X�̃f�b�h�]�[��
	float Input::m_joystickDedZone = 0.0f;										//�W���C�X�e�B�b�N�̃f�b�h�]�[��
	float Input::m_windowDedZone = 0.0f;										//�E�B���h�E�̃f�b�h�]�[��
	
	//���t���[��AnyMode�X�V�t���O
	std::atomic<byte> Input::m_isNextChangeAnyMode = 0;									
	//Any�L�����ۂ�
	std::atomic<bool> Input::m_isAnyMode = false;
	//rapid atomic
	std::atomic<bool> Input::m_rapidAtomic = false;
	//joystick atomic
	std::atomic<bool> Input::m_joystickAtomic = false;
	//axis atomic
	std::atomic<bool> Input::m_axisAtomic=false;
	byte Input::m_joystickCount = 0;												//�W���C�X�e�B�b�N�ڑ���	
	bool Input::m_isAny = false;														//GetAny
	bool Input::m_isAnyDown = false;												//GetAnyDown
	bool Input::m_isAnyUp = false;													//GetAnyUp
	bool Input::m_isUseJoystick = false;											//�W���C�X�e�B�b�N�ڑ��� > 0
	bool Input::m_isMouseInsideFrame = false;									//�}�E�X���E�B���h�E���ɗ��߂邩

	//���t���[���X�V�r�b�g
	GetOnlyProperty<std::atomic<bool>> Input::Any::isAnyMode = Input::m_isAnyMode;
	//�E�B���h�E�̑傫����0.0f ~ 1.0f�Ƃ����ꍇ�̌��݂̃}�E�X���W
	GetOnlyProperty<Vector2> Input::Mouse::position = Input::m_mousePosition;
	//dedZone�̉e�����󂯂Ȃ��O�t���[������̃|�W�V�����̈ړ���
	GetOnlyProperty<Vector2> Input::Mouse::rawAcceleration = Input::m_mouseAcceleration;
	//GetAcceleration�Ɏg�p�����f�b�h�]�[��
	GetOnlyProperty<float> Input::Mouse::dedZone = Input::m_mouseDedZone;
	//���݂�wheel�J�ڏ�� (�� -> +1~, ��O -> -1~)
	GetOnlyProperty<short> Input::Mouse::wheel = Input::m_mouseWheels[Input::m_cNowState];
	//true�̏ꍇ�}�E�X�J�[�\�����E�B���h�E�̊O�ւłȂ��Ȃ�
	GetOnlyProperty<bool> Input::Mouse::isMouseInsideFrame = Input::m_isMouseInsideFrame;;
	//�f�b�h�]�[��
	GetOnlyProperty<float> Input::Joystick::dedZone = Input::m_joystickDedZone;
	//���݂̐ڑ���
	GetOnlyProperty<byte> Input::Joystick::numUseJoysticks = Input::m_joystickCount;
	//���݂̐ڑ��� > 0
	GetOnlyProperty<bool> Input::Joystick::isUseJoystick = Input::m_isUseJoystick;
	//��ʉ𑜓x�� 0.0f ~ 1.0f�Ƃ����ꍇ�̃E�B���h�E�̒��S���W
	GetOnlyProperty<Vector2> Input::Window::position = Input::m_windowPosition;
	//��ʉ𑜓x�� 0.0f ~ 1.0f�Ƃ����ꍇ�̃E�B���h�E�̃T�C�Y
	GetOnlyProperty<Vector2> Input::Window::size = Input::m_windowSize;
	//��ʉ𑜓x�� 0.0f ~ 1.0f�Ƃ����ꍇ��dedZone�̉e�����󂯂Ȃ��O�t���[������̃E�B���h�E�ړ���
	GetOnlyProperty<Vector2> Input::Window::rawAcceleration = Input::m_windowAcceleration;
	//�f�b�h�]�[��
	GetOnlyProperty<float> Input::Window::dedZone = Input::m_windowDedZone;

	//----------------------------------------------------------------------------------
	//[StartUp]
	//�N�������������s��
	//����1: �A�ˊԊu
	//����2: �}�E�X�̃f�b�h�]�[��
	//����3: �W���C�X�e�B�b�N�̃f�b�h�]�[��
	//����4: �E�B���h�E�̃f�b�h�]�[��
	//����5: �}�E�X���E�B���h�E���ɗ��߂邩
	void Input::StartUp(const ReadElement::Pack& rapidInterval, const ReadElement::Pack& mouseDedZone,
		const ReadElement::Pack& joystickDedZone, const ReadElement::Pack& windowDedZone, const ReadElement::Pack& isMouseInsideFrame)
	{
		//ReadElement����擾
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

		//�}�E�X���E�B���h�E�ɗ��߂�
		if (IS_TRUE(m_isMouseInsideFrame))
		{
			RECT rect = {};			
			DwmGetWindowAttribute(SGFramework::Window::HwndHolder::main, DWMWA_EXTENDED_FRAME_BOUNDS, &rect, sizeof(rect));
			ClipCursor(&rect);
		}

		//XInput������
		XINPUT_STATE connectionCheck[4];	//�ڑ��p�o�b�t�@�쐬
		m_joystickCount = 0;						//�ڑ��J�E���g������
		//�ڑ��m�F���[�v
		for (uint i = 0; i < 4; i++)
		{
			HRESULT connectionResult;				//�ڑ��������U���g
			//��Ԏ擾
			connectionResult = XInputGetState(i, &connectionCheck[i]);
			//�擾����(�ڑ����Ă���)�ꍇ�͐ڑ��J�E���g++
			if (connectionResult == ERROR_SUCCESS)	++m_joystickCount;
		}

		//�t���O�Z�b�g
		if (m_joystickCount > 0)
			m_isUseJoystick = true;
		else
			m_isUseJoystick = false;
	}

	//----------------------------------------------------------------------------------
	//[Update]
	//�X�V���s��
	void Input::Update()
	{
		//Rapid�ǉ��X�V
		for (auto& e : m_addKeybordStateRapids)
			m_keybordStateRapids.try_emplace(e, 0.0f, false);
		for (auto& e : m_addJoystickRapids)
			m_joystickRapids[e.e2].try_emplace(e.e1, 0.0f, false);

		//KeyState�X�V & ���b�Z�[�W���M
		UpdateKeybordStateAndSendMessage();
		//Joystick�X�V
		UpdateJoystick();
		//WindowProcedure�̍X�V�I����ҋ@
		m_inputCompletedCondition.Wait();
		//AnyMode�Ȃ�Any�X�V
		if (IS_TRUE(m_isAnyMode) || (m_isNextChangeAnyMode & m_cNextChangeAnyInfoBit))
			UpdateAny();
	}

	//----------------------------------------------------------------------------------
	//[Shutdown]
	//�I���������s��
	void Input::Shutdown()
	{
		//�}�E�X�𗯂߂鏈�����s���Ă���ꍇ�͖߂�
		if (IS_TRUE(m_isMouseInsideFrame))
		{
			RECT rect = {};
			GetClientRect(GetDesktopWindow(), &rect);
			ClipCursor(&rect);
		}
	}

	//----------------------------------------------------------------------------------
	//[UpdateWindowProcedure]
	//�X�V���s�� WindowProcedure ver
	void Input::UpdateWindowProcedure()
	{
		//���݂̃L�[�{�[�h��ԃR�s�[
		memcpy_s(m_keyStates[m_cNowState].data(), m_cKeyStateSize, m_keyStates[m_cMessageState].data(), m_cKeyStateSize);
		
		//GetKeyDown & GetKeyUp����, �r�b�g����
		for (auto& e : m_getKeyDownOrUpVkeys)
		{
			if (e & m_cKeyStatesEmplaceUp)
				m_keyStates[m_cMessageState][e ^ m_cKeyStatesEmplaceUp] = 0;
			else
				m_keyStates[m_cMessageState][e ^ m_cKeyStatesEmplaceDown] ^= Keybord::cGetKeyDownCheckBit;
		}
		m_getKeyDownOrUpVkeys.clear();
		
		//�}�E�X�X�V
		UpdateMouse();
		//�E�B���h�E�X�V
		UpdateWindow();
		//�I���C�x���g
		m_inputCompletedCondition.NotifyOne();
	}

	//----------------------------------------------------------------------------------
	//[ShutdownWindowProcedure]
	//�I���������s�� WindowProcedure ver
	void Input::ShutdownWindowProcedure()
	{
		//�I���C�x���g, ���C���X���b�h��~�h�~
		m_inputCompletedCondition.NotifyOne();
	}
}