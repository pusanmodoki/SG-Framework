/*----------------------------------------------------------------------------------
���͂��Ǘ�����Input class
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_INPUT_HPP_
#define SGFRAMEWORK_HEADER_INPUT_HPP_
#include <array>
#include <exception>
#include <stdexcept>
#include <Windows.h>
#include <Xinput.h>
#include <dwmapi.h>
#include "../02_CustomVariable/CustomVariable.hpp"
#include "../03_MathAF/MathAF.hpp"
#include "../12_GUI/GUI.hpp"
#pragma comment(lib,"xinput.lib")			
#pragma comment(lib,"dwmapi.lib")			

//Framework namespace
namespace SGFramework
{
	//friend�p
	class Application;
	class Thread;
	
	//���͂��Ǘ�����Input class
	//�e���͊֐��̓����o�[�ɂ���\���̂̒���static�֐��Ƃ��Ċi�[����Ă���
	class Input
	{
	public:
		//friend
		friend class Application;
		friend class Thread;
		friend class GUI;
		//prototype
		struct AxisPack;

		DELETE_ALL_DEFAULT_CLASS_FUNCTIONS(Input);

		//�A�ˊԊu�b��, �SInput����
		static GetOnlyProperty<float> rapidInterval;
		
		//Any input function
		struct Any
		{
			//friend
			friend class Input;

			//----------------------------------------------------------------------------------
			//[GetAny]
			//return: �S�ẴL�[, �{�^���̂����ǂꂩ��������Ă�����true
			//				isAnyMode��true�łȂ��ꍇ�͕K��false��Ԃ�
			inline static bool GetAny();
			//----------------------------------------------------------------------------------
			//[GetAnyDown]
			//return: �S�ẴL�[, �{�^���̂����ǂꂩ�������ꂽ�u�ԂȂ�true
			//				isAnyMode��true�łȂ��ꍇ�͕K��false��Ԃ�
			inline static bool GetAnyDown();
			//----------------------------------------------------------------------------------
			//[GetAnyUp]
			//return: �S�ẴL�[, �{�^���̂����ǂꂩ�����ꂽ�u�ԂȂ�true
			//				isAnyMode��true�łȂ��ꍇ�͕K��false��Ԃ�
			inline static bool GetAnyUp();
			//----------------------------------------------------------------------------------
			//[SetAnyMode]
			//Any�n�֐��̗L���� or ��������ݒ肷��, default�ł�false
			//�S�Ă̓��͂��m�F����K�v������̂ŕK�v�Ȏ��ȊO��false�ɐݒ肷�邱�Ƃ𐄏�
			//����1: true = �L����, false = ������
			inline static void SetAnyMode(bool mode);

			//Any�n�֐����L�����ۂ�
			static GetOnlyProperty<std::atomic<bool>> isAnyMode;
		};
		//Keybord input function
		struct Keybord
		{
			//----------------------------------------------------------------------------------
			//[GetKey]
			//return: �ΏۃL�[��������Ă�����0���傫���l��Ԃ�, �����łȂ��ꍇ��0
			//����1:  KeyCode::�Z�Z
			inline static byte GetKey(KeyCode::Enum keyCode);
			//----------------------------------------------------------------------------------
			//[GetKey]
			//return: �ΏۃL�[��������Ă�����0���傫���l��Ԃ�, �����łȂ��ꍇ��0
			//����1:  VK_�Z�Z
			inline static byte GetKey(int vKey);
			//----------------------------------------------------------------------------------
			//[GetKeyDown]
			//return: �ΏۃL�[�������ꂽ�u�ԂȂ�0���傫���l��Ԃ�, �����łȂ��ꍇ��0
			//����1:  KeyCode::�Z�Z
			inline static byte GetKeyDown(KeyCode::Enum keyCode);
			//----------------------------------------------------------------------------------
			//[GetKeyDown]
			//return: �ΏۃL�[�������ꂽ�u�ԂȂ�0���傫���l��Ԃ�, �����łȂ��ꍇ��0
			//����1:  VK_�Z�Z
			inline static byte GetKeyDown(int vKey);
			//----------------------------------------------------------------------------------
			//[GetKeyUp]
			//return: �ΏۃL�[�����ꂽ�u�ԂȂ�0���傫���l��Ԃ�, �����łȂ��ꍇ��0
			//����1:  KeyCode::�Z�Z
			inline static byte GetKeyUp(KeyCode::Enum keyCode);
			//----------------------------------------------------------------------------------
			//[GetKeyUp]
			//return: �ΏۃL�[�����ꂽ�u�ԂȂ�0���傫���l��Ԃ�, �����łȂ��ꍇ��0
			//����1:  VK_�Z�Z
			inline static byte GetKeyUp(int vKey);
			//----------------------------------------------------------------------------------
			//[GetRapid]
			//return: GetKey == true�̂��O��̐���������A�ˊԊu�b�����o�߂������̂�true��Ԃ�
			//				true�Ɣ��f���ꂽ�ꍇ�A�ˊԊu�b���͎����ŏ����������
			//����1:  KeyCode::�Z�Z
			inline static bool GetRapid(KeyCode::Enum keyCode);
			//----------------------------------------------------------------------------------
			//[GetRapid]
			//return: GetKey == true�̂��O��̐���������A�ˊԊu�b�����o�߂������̂�true��Ԃ�
			//				true�Ɣ��f���ꂽ�ꍇ�A�ˊԊu�b���͎����ŏ����������
			//����1:  VK_�Z�Z
			inline static bool GetRapid(int vKey);

			//GetKey��������Ԃ����ꍇ�ɓ����Ă���l
			static constexpr byte cGetKeyCheckBit = 0x80;
			//GetKeyDown��������Ԃ����ꍇ�ɓ����Ă���l
			static constexpr byte cGetKeyDownCheckBit = 0x40;
			//GetKeyUp��������Ԃ����ꍇ�ɓ����Ă���l
			static constexpr byte cGetKeyUpCheckBit = 0x20;
		};

		//Mouse input function
		struct Mouse
		{
			//----------------------------------------------------------------------------------
			//[GetButton]
			//return: �Ώۃ{�^����������Ă�����0���傫���l��Ԃ�, �����łȂ��ꍇ��0
			//����1:  MouseCode::�Z�Z
			inline static byte GetButton(MouseCode::Enum mouseCode);
			//----------------------------------------------------------------------------------
			//[GetButton]
			//return: �ΏۃL�[��������Ă�����0���傫���l��Ԃ�, �����łȂ��ꍇ��0
			//����1:  VK_�Z�Z
			inline static byte GetButton(int vKey);
			//----------------------------------------------------------------------------------
			//[GetButtonDown]
			//return: �ΏۃL�[�������ꂽ�u�ԂȂ�0���傫���l��Ԃ�, �����łȂ��ꍇ��0
			//����1:  MouseCode::�Z�Z
			inline static byte GetButtonDown(MouseCode::Enum mouseCode);
			//----------------------------------------------------------------------------------
			//[GetButtonDown]
			//return: �ΏۃL�[�������ꂽ�u�ԂȂ�0���傫���l��Ԃ�, �����łȂ��ꍇ��0
			//����1:  VK_�Z�Z
			inline static byte GetButtonDown(int vKey);
			//----------------------------------------------------------------------------------
			//[GetButtonUp]
			//return: �ΏۃL�[�����ꂽ�u�ԂȂ�0���傫���l��Ԃ�, �����łȂ��ꍇ��0
			//����1:  MouseCode::�Z�Z
			inline static byte GetButtonUp(MouseCode::Enum mouseCode);
			//----------------------------------------------------------------------------------
			//[GetButtonUp]
			//return: �ΏۃL�[�����ꂽ�u�ԂȂ�0���傫���l��Ԃ�, �����łȂ��ꍇ��0
			//����1:  VK_�Z�Z
			inline static byte GetButtonUp(int vKey);
			//----------------------------------------------------------------------------------
			//[GetRapid]
			//return: GetButton == true�̂��O��̐���������A�ˊԊu�b�����o�߂������̂�true��Ԃ�
			//				true�Ɣ��f���ꂽ�ꍇ�A�ˊԊu�b���͎����ŏ����������
			//����1:  MouseCode::�Z�Z
			inline static bool GetRapid(MouseCode::Enum mouseCode);
			//----------------------------------------------------------------------------------
			//[GetRapid]
			//return: GetButton == true�̂��O��̐���������A�ˊԊu�b�����o�߂������̂�true��Ԃ�
			//				true�Ɣ��f���ꂽ�ꍇ�A�ˊԊu�b���͎����ŏ����������
			//����1:  VK_�Z�Z
			inline static bool GetRapid(int vKey);

			//----------------------------------------------------------------------------------
			//[GetPosition]
			//return: �E�B���h�E�̑傫�����c��-1.0f ~ 1.0f�Ƃ����ꍇ�̌��݂̃}�E�X���W
			inline static Vector2 GetPosition();
			//----------------------------------------------------------------------------------
			//[GetPositionToWindow]
			//return: �E�B���h�E�̑傫�����c��0.0f ~ 1.0f�Ƃ����ꍇ�̌��݂̃}�E�X���W
			inline static Vector2 GetPositionToWindow();
			//----------------------------------------------------------------------------------
			//[GetAcceleration]
			//return: �O�t���[����̃|�W�V�����ړ���, �e�v�f�̐�Βl��dedZone�ȉ��̏ꍇ�K��0.0f�ƂȂ�
			inline static Vector2 GetAcceleration();
			//----------------------------------------------------------------------------------
			//[GetAcceleration]
			//return: dedZone�̉e�����󂯂Ȃ��O�t���[����̃|�W�V�����ړ���
			inline static Vector2 GetRawAcceleration();
			//----------------------------------------------------------------------------------
			//[GetWheelDelta]
			//return: ���݂�wheel�J�ڏ�� (�� -> +1~, ��O -> -1~)
			inline static short GetWheelDelta();

			//cGetButton��������Ԃ����ꍇ�ɓ����Ă���l
			static constexpr byte cGetButtonCheckBit = 0x80;
			//cGetButtonDown��������Ԃ����ꍇ�ɓ����Ă���l
			static constexpr byte cGetButtonDownCheckBit = 0x40;
			//cGetButtonUp��������Ԃ����ꍇ�ɓ����Ă���l
			static constexpr byte cGetButtonUpCheckBit = 0x20;

			//�E�B���h�E�̑傫����0.0f ~ 1.0f�Ƃ����ꍇ�̌��݂̃}�E�X���W
			static GetOnlyProperty<Vector2> position;
			//dedZone�̉e�����󂯂Ȃ��O�t���[������̃|�W�V�����̈ړ���
			static GetOnlyProperty<Vector2> rawAcceleration;
			//GetAcceleration�Ɏg�p�����f�b�h�]�[��
			static GetOnlyProperty<float> dedZone;
			//���݂�wheel�J�ڏ�� (�� -> +1~, ��O -> -1~)
			static GetOnlyProperty<short> wheel;
			//true�̏ꍇ�}�E�X�J�[�\�����E�B���h�E�̊O�ւłȂ��Ȃ�
			static GetOnlyProperty<bool> isMouseInsideFrame;
		};

		//Joystick input function
		struct Joystick
		{
			//friend
			friend class Input;

			//----------------------------------------------------------------------------------
			//[GetButton]
			//return: �Ώۃ{�^����������Ă�����0���傫���l��Ԃ�, �����łȂ��ꍇ��0
			//����1: JoystickCode::�Z�Z
			//����2: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), ��O�����͍s��Ȃ�, default = 0
			inline static ushort GetButton(JoystickCode::Enum joystickCode, uint useStick = 0);
			//----------------------------------------------------------------------------------
			//[GetButtonDown]
			//return: �ΏۃL�[�������ꂽ�u�ԂȂ�0���傫���l��Ԃ�, �����łȂ��ꍇ��0
			//����1: JoystickCode::�Z�Z
			//����2: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), ��O�����͍s��Ȃ�, default = 0
			inline static ushort GetButtonDown(JoystickCode::Enum joystickCode, uint useStick = 0);
			//----------------------------------------------------------------------------------
			//[GetButtonUp]
			//return: �ΏۃL�[�����ꂽ�u�ԂȂ�0���傫���l��Ԃ�, �����łȂ��ꍇ��0
			//����1: JoystickCode::�Z�Z
			//����2: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), ��O�����͍s��Ȃ�, default = 0
			inline static ushort GetButtonUp(JoystickCode::Enum joystickCode, uint useStick = 0);
			//----------------------------------------------------------------------------------
			//[GetRapid]
			//return: GetButton == true�̂��O��̐���������A�ˊԊu�b�����o�߂������̂�true��Ԃ�
			//				true�Ɣ��f���ꂽ�ꍇ�A�ˊԊu�b���͎����ŏ����������
			//����1: JoystickCode::�Z�Z
			//����2: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), ��O�����͍s��Ȃ�, default = 0
			inline static bool GetRapid(JoystickCode::Enum joystickCode, uint useStick = 0);

			//----------------------------------------------------------------------------------
			//[GetLTrigger]
			//return: 0.0f ~ 1.0f�̒l�ƂȂ�L�g���K�[�̌��ݒl, ��Βl��dedZone�ȉ��̏ꍇ�K��0.0f��ԋp����
			//����1: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), ��O�����͍s��Ȃ�, default = 0
			inline static float GetLTrigger(uint useStick = 0);
			//----------------------------------------------------------------------------------
			//[GetRawLTrigger]
			//return: 0.0f ~ 1.0f�̒l�ƂȂ�dedZone���l�����Ȃ�L�g���K�[�̌��ݒl
			//����1: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), ��O�����͍s��Ȃ�, default = 0
			inline static float GetRawLTrigger(uint useStick = 0);
			//----------------------------------------------------------------------------------
			//[GetLTrigger]
			//return: 0.0f ~ 1.0f�̒l�����L�g���K�[�̑O�t���[����̈ړ���,
			//				��r�Ɏg����l��dedZone�̉e�����󂯂�
			//����1: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), ��O�����͍s��Ȃ�, default = 0
			inline static float GetLTriggerAcceleration(uint useStick = 0);
			//----------------------------------------------------------------------------------
			//[GetRawLTriggerAcceleration]
			//return: 0.0f ~ 1.0f�̒l�ƂȂ�dedZone���l�����Ȃ�L�g���K�[�̑O�t���[����ړ���
			//����1: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), ��O�����͍s��Ȃ�, default = 0
			inline static float GetRawLTriggerAcceleration(uint useStick = 0);

			//----------------------------------------------------------------------------------
			//[GetRTrigger]
			//return: 0.0f ~ 1.0f�̒l�ƂȂ�R�g���K�[�̌��ݒl, ��Βl��dedZone�ȉ��̏ꍇ�K��0.0f��ԋp����
			//����1: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), ��O�����͍s��Ȃ�, default = 0
			inline static float GetRTrigger(uint useStick = 0);
			//----------------------------------------------------------------------------------
			//[GetRawRTrigger]
			//return: 0.0f ~ 1.0f�̒l�ƂȂ�dedZone���l�����Ȃ�R�g���K�[�̌��ݒl
			//����1: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), ��O�����͍s��Ȃ�, default = 0
			inline static float GetRawRTrigger(uint useStick = 0);
			//----------------------------------------------------------------------------------
			//[GetRTriggerAcceleration]
			//return: 0.0f ~ 1.0f�̒l�����R�g���K�[�̑O�t���[����̈ړ���,
			//				��r�Ɏg����l��dedZone�̉e�����󂯂�
			//����1: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), ��O�����͍s��Ȃ�, default = 0
			inline static float GetRTriggerAcceleration(uint useStick = 0);
			//----------------------------------------------------------------------------------
			//[GetRawRTriggerAcceleration]
			//return: 0.0f ~ 1.0f�̒l�ƂȂ�dedZone���l�����Ȃ�R�g���K�[�̑O�t���[����ړ���
			//����1: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), ��O�����͍s��Ȃ�, default = 0
			inline static float GetRawRTriggerAcceleration(uint useStick = 0);

			//----------------------------------------------------------------------------------
			//[GetLStick]
			//return: -1.0f ~ 1.0f�̒l�ƂȂ�L�X�e�B�b�N�̌��ݒl, �e�v�f�̐�Βl��dedZone�ȉ��̏ꍇ�K��0.0f�ƂȂ�
			//����1: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), ��O�����͍s��Ȃ�, default = 0
			inline static Vector2 GetLStick(uint useStick = 0);
			//----------------------------------------------------------------------------------
			//[GetRawLStick]
			//return: -1.0f ~ 1.0f�̒l�ƂȂ�dedZone���l�����Ȃ�L�X�e�B�b�N�̌��ݒl
			//����1: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), ��O�����͍s��Ȃ�, default = 0
			inline static Vector2 GetRawLStick(uint useStick = 0);
			//----------------------------------------------------------------------------------
			//[GetLStickAcceleration]
			//return: -1.0f ~ 1.0f�̒l�����L�X�e�B�b�N�̑O�t���[����̈ړ���,
			//				��r�Ɏg����l��dedZone�̉e�����󂯂�
			//����1: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), ��O�����͍s��Ȃ�, default = 0
			inline static Vector2 GetLStickAcceleration(uint useStick = 0);
			//----------------------------------------------------------------------------------
			//[GetRawLStickAcceleration]
			//return: -1.0f ~ 1.0f�̒l�ƂȂ�dedZone���l�����Ȃ�L�X�e�B�b�N�̑O�t���[����ړ���
			//����1: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), ��O�����͍s��Ȃ�, default = 0
			inline static Vector2 GetRawLStickAcceleration(uint useStick = 0);

			//----------------------------------------------------------------------------------
			//[GetRStick]
			//return: -1.0f ~ 1.0f�̒l�ƂȂ�R�X�e�B�b�N�̌��ݒl, �e�v�f�̐�Βl��dedZone�ȉ��̏ꍇ�K��0.0f�ƂȂ�
			//����1: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), ��O�����͍s��Ȃ�, default = 0
			inline static Vector2 GetRStick(uint useStick = 0);
			//----------------------------------------------------------------------------------
			//[GetRawRStick]
			//return: -1.0f ~ 1.0f�̒l�ƂȂ�dedZone���l�����Ȃ�R�X�e�B�b�N�̌��ݒl
			//����1: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), ��O�����͍s��Ȃ�, default = 0
			inline static Vector2 GetRawRStick(uint useStick = 0);
			//----------------------------------------------------------------------------------
			//[GetRStickAcceleration]
			//return: -1.0f ~ 1.0f�̒l�����R�X�e�B�b�N�̑O�t���[����̈ړ���,
			//				��r�Ɏg����l��dedZone�̉e�����󂯂�
			//����1: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), ��O�����͍s��Ȃ�, default = 0
			inline static Vector2 GetRStickAcceleration(uint useStick = 0);
			//----------------------------------------------------------------------------------
			//[GetRawRStickAcceleration]
			//return: -1.0f ~ 1.0f�̒l�ƂȂ�dedZone���l�����Ȃ�R�X�e�B�b�N�̑O�t���[����ړ���
			//����1: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), ��O�����͍s��Ȃ�, default = 0
			inline static Vector2 GetRawRStickAcceleration(uint useStick = 0);

			//----------------------------------------------------------------------------------
			//[GetHighVibration]
			//return: 0.0f ~ 1.0f�̒l�ƂȂ錻�݂̍����g�U���l
			//����1: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), ��O�����͍s��Ȃ�, default = 0
			inline static float GetHighVibration(uint useStick = 0);
			//----------------------------------------------------------------------------------
			//[GetLowVibration]
			//return: 0.0f ~ 1.0f�̒l�ƂȂ錻�݂̒���g�U���l
			//����1: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), ��O�����͍s��Ȃ�, default = 0
			inline static float GetLowVibration(uint useStick = 0);

			//----------------------------------------------------------------------------------
			//[SetHighVibration]
			//�����g�U���l��ݒ肷��, ���s��1�t���[����ɍs���邪�AFadeOut���̏ꍇ�͓o�^�Ɏ��s����
			//�e�R���g���[���[��High, Low�e��������ݒ肷�鎖�͂ł��Ȃ�
			//return: �ݒ肪���������true, ���s(���łɐݒ肳��Ă���)�̏ꍇfalse
			//����1: �ݒ肷��U���l, 0.0f ~ 1.0f��Clamp�����
			//����2: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), �ڑ�����Ă��Ȃ��ꍇ���s, default = 0
			//����3: true�̏ꍇ, ���ɐݒ�ς݂̏ꍇ�������I�ɐݒ肷��, default = false
			inline static bool SetHighVibration(float setVibration, uint useStick = 0, bool isForced = false);
			//----------------------------------------------------------------------------------
			//[SetLowVibration]
			//����g�U���l��ݒ肷��, ���s��1�t���[����ɍs���邪�AFadeOut���̏ꍇ�͓o�^�Ɏ��s����
			//�e�R���g���[���[��High, Low�e��������ݒ肷�鎖�͂ł��Ȃ�
			//return: �ݒ肪���������true, ���s(���łɐݒ肳��Ă���)�̏ꍇfalse
			//����1: �ݒ肷��U���l, 0.0f ~ 1.0f��Clamp�����
			//����2: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), �ڑ�����Ă��Ȃ��ꍇ���s, default = 0
			//����3: true�̏ꍇ, ���ɐݒ�ς݂̏ꍇ�������I�ɐݒ肷��, default = false
			inline static bool SetLowVibration(float setVibration, uint useStick = 0, bool isForced = false);

			//----------------------------------------------------------------------------------
			//[SetHighVibrationFadeOut]
			//�����g�U���l��ݒ�l����0.0f�փt�F�[�h�A�E�g����悤�ɐݒ肷��
			//���s��1�t���[����ɍs���邪�AFadeOut���̏ꍇ�͓o�^�Ɏ��s����
			//���s������ setVibration�ݒ� -> waitReachSeconds�o�� -> reachZeroSeconds������0.0f�܂Ō���
			//�e�R���g���[���[��High, Low�e��������ݒ肷�鎖�͂ł��Ȃ�
			//return: �ݒ肪���������true, ���s(���łɐݒ肳��Ă��� or FadeOut��)�̏ꍇfalse
			//����1: �ݒ肷��U���l, 0.0f ~ 1.0f��Clamp�����
			//����2: �������n�߂�܂ł̑ҋ@����
			//����3: 0.0f�ɂȂ�܂ł����鏊�v����
			//����4: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), �ڑ�����Ă��Ȃ��ꍇ���s, default = 0
			//����5: true�̏ꍇ, ���ɐݒ�ς݂̏ꍇ�������I�ɐݒ肷��, default = false
			inline static bool SetHighVibrationFadeOut(float setVibration, float waitReachSeconds, float reachZeroSeconds, uint useStick = 0, bool isForced = false);
			//----------------------------------------------------------------------------------
			//[SetLowVibrationFadeOut]
			//����g�U���l��ݒ�l����0.0f�փt�F�[�h�A�E�g����悤�ɐݒ肷��, ���s��1�t���[����ɍs����
			//���s��1�t���[����ɍs���邪�AFadeOut���̏ꍇ�͓o�^�Ɏ��s����
			//���s������ setVibration�ݒ� -> waitReachSeconds�o�� -> reachZeroSeconds������0.0f�܂Ō���
			//�e�R���g���[���[��High, Low�e��������ݒ肷�鎖�͂ł��Ȃ�
			//return: �ݒ肪���������true, ���s(���łɐݒ肳��Ă��� or FadeOut��)�̏ꍇfalse
			//����1: �ݒ肷��U���l, 0.0f ~ 1.0f��Clamp�����
			//����2: �������n�߂�܂ł̑ҋ@����
			//����3: 0.0f�ɂȂ�܂ł����鏊�v����
			//����4: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), �ڑ�����Ă��Ȃ��ꍇ���s, default = 0
			//����5: true�̏ꍇ, ���ɐݒ�ς݂̏ꍇ�������I�ɐݒ肷��, default = false
			inline static bool SetLowVibrationFadeOut(float setVibration, float waitReachSeconds, float reachZeroSeconds, uint useStick = 0, bool isForced = false);

			//----------------------------------------------------------------------------------
			//[ResetVibration]
			//���݂̐U���ݒ�l��High Low�Ƃ���0.0f�܂Ńt�F�[�h�A�E�g������
			//���̊֐��͊��ɐݒ肳��Ă��邩�ۂ��Ɋւ�炸�����I�Ɏ��s�����
			//����1: 0.0f�ɂȂ�܂ł����鏊�v����
			//����2: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), ��O�����͍s��Ȃ�, default = 0
			inline static void ResetVibration(float reachZeroSpeedPerSeconds, uint useStick = 0);
			//----------------------------------------------------------------------------------
			//[ResetAllVibration]
			//�S�ẴW���C�X�e�B�b�N�Ō��݂̐U���ݒ�l��High Low�Ƃ���0.0f�܂Ńt�F�[�h�A�E�g������
			//���̊֐��͊��ɐݒ肳��Ă��邩�ۂ��Ɋւ�炸�����I�Ɏ��s�����
			//����1: 0.0f�ɂȂ�܂ł����鏊�v����
			inline static void ResetAllVibration(float reachZeroSpeedPerSeconds);

			//----------------------------------------------------------------------------------
			//[SetAutoReConnectCallback]
			//�ڑ����̕ύX���s��ꂽ���̃R�[���o�b�N�֐���ݒ肷��
			//����1: �R�[���o�b�N�֐�, ����1: �V�����W���C�X�e�B�b�N�̐ڑ���(uint)
			inline static void SetAutoReConnectCallback(const std::function<void(uint joystickCount)>& callback);

			//�ő�ڑ���
			static constexpr uint cJoystickMax = 4;
			//�f�b�h�]�[��
			static GetOnlyProperty<float> dedZone;
			//���݂̐ڑ���
			static GetOnlyProperty<byte> numUseJoysticks;
			//���݂̐ڑ��� > 0
			static GetOnlyProperty<bool> isUseJoystick;
		};
		//Window input function
		struct Window
		{
			//----------------------------------------------------------------------------------
			//[GetPosition]
			//return: ��ʉ𑜓x�� 0.0f ~ 1.0f�Ƃ����ꍇ�̃E�B���h�E�̒��S���W
			inline static Vector2 GetPosition();
			//----------------------------------------------------------------------------------
			//[GetSize]
			//return: ��ʉ𑜓x�� 0.0f ~ 1.0f�Ƃ����ꍇ�̃E�B���h�E�̃T�C�Y
			inline static Vector2 GetSize();
			//----------------------------------------------------------------------------------
			//[GetAcceleration]
			//return: ��ʉ𑜓x�� 0.0f ~ 1.0f�Ƃ����ꍇ�̑O�t���[������̃E�B���h�E�ړ���
			//				�e�v�f�̐�Βl��dedZone�ȉ��̏ꍇ�K��0.0f�ƂȂ�
			inline static Vector2 GetAcceleration();
			//----------------------------------------------------------------------------------
			//[GetAcceleration]
			//return: ��ʉ𑜓x�� 0.0f ~ 1.0f�Ƃ����ꍇ��dedZone�̉e�����󂯂Ȃ��O�t���[������̃E�B���h�E�ړ���
			inline static Vector2 GetRawAcceleration();

			//��ʉ𑜓x�� 0.0f ~ 1.0f�Ƃ����ꍇ�̃E�B���h�E�̒��S���W
			static GetOnlyProperty<Vector2> position;
			//��ʉ𑜓x�� 0.0f ~ 1.0f�Ƃ����ꍇ�̃E�B���h�E�̃T�C�Y
			static GetOnlyProperty<Vector2> size;
			//��ʉ𑜓x�� 0.0f ~ 1.0f�Ƃ����ꍇ��dedZone�̉e�����󂯂Ȃ��O�t���[������̃E�B���h�E�ړ���
			static GetOnlyProperty<Vector2> rawAcceleration;
			//�f�b�h�]�[��
			static GetOnlyProperty<float> dedZone;
		};

		//Axis input function
		struct Axis
		{
		public:
			//----------------------------------------------------------------------------------
			//[GetAxisHorizontal]
			//���zHorizontalAxis���̎擾���s��, �擾����l��[Keybord->A (-), D (+)], [Joystick->LStick.x]
			//return: �e�擾�l�̒��ōł���Βl���傫���l
			//����1: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), ��O�����͍s��Ȃ�, default = 0
			inline static float GetAxisHorizontal(uint useJoystick = 0);
			//----------------------------------------------------------------------------------
			//[GetRawAxisHorizontal]
			//���zHorizontalAxis���̎擾��dedZone�̉e�����󂯂��ɍs��
			//�擾����l��[Keybord->A (-), D (+)], [Joystick->RawLStick.x]
			//return: �e�擾�l�̒��ōł���Βl���傫���l
			//����1: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), ��O�����͍s��Ȃ�, default = 0
			inline static float GetRawAxisHorizontal(uint useJoystick = 0);
			//----------------------------------------------------------------------------------
			//[GetAxisVertical]
			//���zVerticalAxis���̎擾���s��, �擾����l��[Keybord->W (-), S (+)], [Joystick->LStick.y]
			//return: �e�擾�l�̒��ōł���Βl���傫���l
			//����1: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), ��O�����͍s��Ȃ�, default = 0
			inline static float GetAxisVertical(uint useJoystick = 0);
			//----------------------------------------------------------------------------------
			//[GetRawAxisVertical]
			//���zVerticalAxis���̎擾��dedZone�̉e�����󂯂��ɍs��
			//�擾����l��[Keybord->W (-), S (+)], [Joystick->RawLStick.y]
			//return: �e�擾�l�̒��ōł���Βl���傫���l
			//����1: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), ��O�����͍s��Ȃ�, default = 0
			inline static float GetRawAxisVertical(uint useJoystick = 0);
			//----------------------------------------------------------------------------------
			//[GetAxisJump]
			//���zJumpAxis���̎擾���s��, �擾����l��[Keybord->Space (+)], [Joystick->A (+)]
			//return: �e�擾�l�̒��ōł���Βl���傫���l
			//����1: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), ��O�����͍s��Ȃ�, default = 0
			inline static float GetAxisJump(uint useJoystick = 0);
			//----------------------------------------------------------------------------------
			//[GetAxisAction]
			//���zActionAxis���̎擾���s��, �擾����l��[Mouse->Left (+)], [Joystick->A (+)]
			//return: �e�擾�l�̒��ōł���Βl���傫���l
			//����1: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), ��O�����͍s��Ȃ�, default = 0
			inline static float GetAxisAction(uint useJoystick = 0);
			//----------------------------------------------------------------------------------
			//[GetAxisUser]
			//���O��RegisterAxisUser�œo�^���ꂽ���zAxis���̎擾���s��
			//�擾����l�͓o�^����AxisPack�ɏ�����
			//return: �e�擾�l�̒��ōł���Βl���傫���l
			//throw: name�̃G�������g���o�^����Ă��Ȃ��ꍇ
			//����1: �o�^���Ɏg�p����name (map->key)
			//����2: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), ��O�����͍s��Ȃ�, default = 0
			inline static float GetAxisUser(const sgstring& name, uint useJoystick = 0);
			//----------------------------------------------------------------------------------
			//[GetRawAxisUser]
			//���O��RegisterAxisUser�œo�^���ꂽ���zAxis���̎擾��dedZone�̉e�����󂯂��ɍs��
			//�擾����l�͓o�^����AxisPack�ɏ�����
			//AxisPack->axisCodes���ݒ肳��Ă��Ȃ��ꍇ, ���ʂ�GetAxisUser�Ɠ����ɂȂ�
			//return: �e�擾�l�̒��ōł���Βl���傫���l
			//throw: name�̃G�������g���o�^����Ă��Ȃ��ꍇ
			//����1: �o�^���Ɏg�p����name (map->key)
			//����2: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), ��O�����͍s��Ȃ�, default = 0
			inline static float GetRawAxisUser(const sgstring& name, uint useJoystick = 0);
			//----------------------------------------------------------------------------------
			//[RegisterAxisUser]
			//���zAxis���̓o�^���s��, name (map->key)���܂��o�^����Ă��Ȃ����ɂ̂ݓo�^���s��
			//return: �o�^������������true, ���s�����ꍇ��false
			//����1: �g�p���Ɉ����Ƃ��Ďg����name (map->key)
			//����2: �ݒ肷��Axis��
			inline static bool RegisterAxisUser(const sgstring& name, const AxisPack& axisPack);

		private:
			//----------------------------------------------------------------------------------
			//[BaseGetAxisFloat]
			//GetAxis��Stick����ver�ŗp����Base�֐�
			//return: GetAxis�̌���
			//����1: axis1 (+)
			//����2: axis1 (-)
			//����3: axis2
			inline static float BaseGetAxisFloat(byte axis1Puls, byte axis1Minus, float axis2);
			//----------------------------------------------------------------------------------
			//[BaseGetAxisByte]
			//GetAxis��Stick�Ȃ�ver�ŗp����Base�֐�
			//return: GetAxis�̌���
			//����1: axis1 (+)
			//����2: axis2 (-)
			inline static float BaseGetAxisByte(ushort axis1Puls, byte axis2Puls);
			//----------------------------------------------------------------------------------
			//[GetAxisPackKeyStatesPuls]
			//GetAxisUser��keyAndMouseCodeWithPlus��Axis���擾�Ɏg�p����
			//return: �S�Ď擾�������ʍł���Βl�̑傫���l (�Ȃ��ꍇ0.0f)
			//����1: keyAndMouseCodeWithPlus
			inline static float GetAxisPackKeyStatesPuls(const std::vector<int>& keyCodes);
			//----------------------------------------------------------------------------------
			//[GetAxisPackKeyStatesMinus]
			//GetAxisUser��keyAndMouseCodeWithMinus��Axis���擾�Ɏg�p����
			//return: �S�Ď擾�������ʍł���Βl�̑傫���l (�Ȃ��ꍇ0.0f)
			//����1: keyAndMouseCodeWithMinus
			inline static float GetAxisPackKeyStatesMinus(const std::vector<int>& keyCodes);
			//----------------------------------------------------------------------------------
			//[GetAxisPackJoystickButtomsPuls]
			//GetAxisUser��joystickCodeWithPlus��Axis���擾�Ɏg�p����
			//return: �S�Ď擾�������ʍł���Βl�̑傫���l (�Ȃ��ꍇ0.0f)
			//����1: joystickCodeWithPlus
			inline static float GetAxisPackJoystickButtomsPuls(const std::vector<JoystickCode::Enum>& joystickCodes, uint useStick);
			//----------------------------------------------------------------------------------
			//[GetAxisPackJoystickButtomsMinus]
			//GetAxisUser��GetAxisPackJoystickButtomsMinus��Axis���擾�Ɏg�p����
			//return: �S�Ď擾�������ʍł���Βl�̑傫���l (�Ȃ��ꍇ0.0f)
			//����1: joystickCodeWithMinus
			inline static float GetAxisPackJoystickButtomsMinus(const std::vector<JoystickCode::Enum>& joystickCodes, uint useStick);
			//----------------------------------------------------------------------------------
			//[GetAxisPackAxisCodes]
			//GetAxisUser��axisCodes��Axis���擾�Ɏg�p����
			//return: �S�Ď擾�������ʍł���Βl�̑傫���l (�Ȃ��ꍇ0.0f)
			//����1: axisCodes
			//����2: �g�p����stick
			inline static float GetAxisPackAxisCodes(const std::vector<AxisCode::Enum>& axisCodes, uint useStick);
			//----------------------------------------------------------------------------------
			//[GetRawAxisPackAxisCodes]
			//GetAxisUser��axisCodes��Axis���擾�Ɏg�p���� (Raw ver)
			//return: �S�Ď擾�������ʍł���Βl�̑傫���l (�Ȃ��ꍇ0.0f)
			//����1: axisCodes
			//����2: �g�p����stick
			inline static float GetRawAxisPackAxisCodes(const std::vector<AxisCode::Enum>& axisCodes, uint useStick);
		};

		//RegisterAxisUser�Ɏg�p����AxisPack struct
		struct AxisPack
		{
			//default
			inline AxisPack() = default;

			//----------------------------------------------------------------------------------
			//[SetKeyAndMouseCodeWithPuls]
			//keyAndMouseCodeWithPlus�ɕ����v�f���܂Ƃ߂ēo�^����
			//����1: �o�^����p�����[�^�[�p�b�N (KeyCode::�Z�Z, MosueCode::�Z�Z)
			template < typename... Args >
			inline void SetKeyAndMouseCodeWithPuls(const Args&... args);
			//----------------------------------------------------------------------------------
			//[SetKeyAndMouseCodeWithMinus]
			//keyAndMouseCodeWithMinus�����v�f���܂Ƃ߂ēo�^����
			//����1: �o�^����p�����[�^�[�p�b�N (KeyCode::�Z�Z, MosueCode::�Z�Z)
			template < typename... Args >
			inline void SetKeyAndMouseCodeWithMinus(const Args&... args);
			//----------------------------------------------------------------------------------
			//[SetJoystickCodeWithPuls]
			//joystickCodeWithPlus�����v�f���܂Ƃ߂ēo�^����
			//����1: �o�^����p�����[�^�[�p�b�N (JoystickCode::�Z�Z)
			template < typename... Args >
			inline void SetJoystickCodeWithPuls(const Args&... args);
			//----------------------------------------------------------------------------------
			//[SetJoystickCodeWithMinus]
			//joystickCodeWithMinus�����v�f���܂Ƃ߂ēo�^����
			//����1: �o�^����p�����[�^�[�p�b�N (JoystickCode::�Z�Z)
			template < typename... Args >
			inline void SetJoystickCodeWithMinus(const Args&... args);
			//----------------------------------------------------------------------------------
			//[SetAxisCodes]
			//axisCodes�����v�f���܂Ƃ߂ēo�^����, �o�^����̂͊e�擾�֐��ƂȂ�
			//����1: �o�^����p�����[�^�[�p�b�N (AxisCode::�Z�Z)
			template < typename... Args >
			inline void SetAxisCodes(const Args&... args);

			std::vector<int> keyAndMouseCodeWithPlus;					//KeyCode::�Z�Z, MosueCode::�Z�Z (+)
			std::vector<int> keyAndMouseCodeWithMinus;				//KeyCode::�Z�Z, MosueCode::�Z�Z (-)
			std::vector<JoystickCode::Enum> joystickCodeWithPlus;	//JoystickCode::�Z�Z (+)
			std::vector<JoystickCode::Enum> joystickCodeWithMinus;		//JoystickCode::�Z�Z (-)
			std::vector<AxisCode::Enum> axisCodes;						//AxisCode::�Z�Z (�e�擾�֐�)
		};


	private:
		//SetVibration�p�L���[
		struct VibrationQueue
		{
			//default
			inline VibrationQueue() = default;
			//----------------------------------------------------------------------------------
			//[SetValue]
			//�l������������
			//����1: target
			//����2: speed
			//����3: wait
			//����4: useStick
			//����5: cSet or cFadeOut or cWaitState
			//����6: isForced
			inline void SetValue(float target, float speed, float wait, byte useStick, byte type, bool isForced)
			{
				this->target = target;
				this->speed = speed;
				this->wait = wait;
				this->elapased = 0.0f;
				this->useStick = useStick;
				this->type = type;
				this->isLive = true;
				this->isForced = isForced;
			}

			//Set state bit
			static constexpr byte cSet = 0x01;
			//Fade out state bit
			static constexpr byte cFadeOut = 0x02;
			//Wait state bit
			static constexpr byte cWaitState = 0x10;
			//Start state bit
			static constexpr byte StartState = 0x20;
		
			float target = 0.0f;		//�ݒ�l	
			float speed = 0.0f;		//�������x per seconds
			float wait = 0.0f;		//�ҋ@�b��
			float elapased = 0.0f;	//�ҋ@�b���v��
			byte useStick = 0;		//�g�p�W���C�X�e�B�b�N (�o�C�g�]���Ă邵����ƕ֗�)
			byte type = 0;			//�X�e�[�g�ۑ��t���O
			bool isLive = false;		//�L�����ۂ�
			bool isForced = false;	//�������ۂ�
		};
		//�A�˗p
		struct Rapid
		{
			//----------------------------------------------------------------------------------
			//[�R���X�g���N�^]
			//�l������������
			//����1: �b��
			//����2: �t���O
			inline Rapid(float value, bool isEnd) : value(value), isEnd(isEnd) {}
			float value = 0.0f;			//�b��
			std::atomic_bool isEnd;	//�t���O
		};

		//----------------------------------------------------------------------------------
		//[StartUp]
		//�N�������������s��
		//����1: �A�ˊԊu
		//����2: �}�E�X�̃f�b�h�]�[��
		//����3: �W���C�X�e�B�b�N�̃f�b�h�]�[��
		//����4: �E�B���h�E�̃f�b�h�]�[��
		//����5: �}�E�X���E�B���h�E���ɗ��߂邩
		static void StartUp(const ReadElement::Pack& rapidInterval, const ReadElement::Pack& mouseDedZone, 
			const ReadElement::Pack& joystickDedZone, const ReadElement::Pack& windowDedZone, const ReadElement::Pack& isMouseInsideFrame);
		//----------------------------------------------------------------------------------
		//[Update]
		//�X�V���s��
		static void Update();
		//----------------------------------------------------------------------------------
		//[Shutdown]
		//�I���������s��
		static void Shutdown();

		//----------------------------------------------------------------------------------
		//[UpdateWindowProcedure]
		//�X�V���s�� WindowProcedure ver
		static void UpdateWindowProcedure();
		//----------------------------------------------------------------------------------
		//[ReceiveInputMessageWithWindowProcedure]
		//WindowProcedure�œ���Message����M����
		//����1: Message
		//����2: WPARAM
		//����3: LPARAM
		inline static void ReceiveInputMessageWithWindowProcedure(uint message, WPARAM wParam, LPARAM lParam);
		//----------------------------------------------------------------------------------
		//[ShutdownWindowProcedure]
		//�I���������s�� WindowProcedure ver
		static void ShutdownWindowProcedure();

		//----------------------------------------------------------------------------------
		//[UpdateAny]
		//Any�̍X�V���s��
		static void UpdateAny();
		//----------------------------------------------------------------------------------
		//[UpdateKeybordStateAndSendMessage]
		//KeybordState�̍X�V��Update���b�Z�[�W�̑��M���s��
		static void UpdateKeybordStateAndSendMessage();
		//----------------------------------------------------------------------------------
		//[UpdateMouse]
		//Mouse�̍X�V���s�� (WindowProcedure��)
		static void UpdateMouse();
		//----------------------------------------------------------------------------------
		//[UpdateJoystick]
		//Joystick�̍X�V���s��
		static void UpdateJoystick();
		//----------------------------------------------------------------------------------
		//[UpdateWindow]
		//Window�̍X�V���s�� (WindowProcedure��)
		static void UpdateWindow();
		//----------------------------------------------------------------------------------
		//[ReConnectJoyStick]
		// �ڑ������X�V����
		static void ReConnectJoyStick();

		//----------------------------------------------------------------------------------
		//[DecodingKeyboradMessage]
		//�L�[�{�[�hMessage����ǂ���GetKey���Z�b�g����
		//return: �����Ȃ�true
		//����1: Message
		//����2: WPARAM
		inline static bool DecodingKeyboradMessage(uint message, WPARAM wParam);
		//----------------------------------------------------------------------------------
		//[DecodingMouseMessage]
		//�}�E�XMessage����ǂ���GetKey���Z�b�g����
		//����1: Message
		//����2: WPARAM
		//����3: LPARAM
		inline static void DecodingMouseMessage(uint message, WPARAM wParam, LPARAM lParam);
		//----------------------------------------------------------------------------------
		//[DecodingMouseLRButtonMessage]
		//DecodingMouseMessage����LR�{�^���̉�ǂɎg�p
		//����1: Message
		//����2: WPARAM
		inline static void DecodingMouseLRButtonMessage(uint message, WPARAM wParam);
		//----------------------------------------------------------------------------------
		//[DecodingMouseOtherMessage]
		//DecodingMouseMessage����LR�{�^���ȊO�̉�ǂɎg�p
		//����1: Message
		//����2: WPARAM
		inline static void DecodingMouseOtherMessage(uint message, WPARAM wParam);
		//----------------------------------------------------------------------------------
		//[SetKeyDown]
		//GetKeyDown��ݒ肷��
		//����1: vKey
		inline static void SetKeyDown(uint64 vKey);
		//----------------------------------------------------------------------------------
		//[SetKeyUp]
		//GetKeyUp��ݒ肷��
		//����1: vKey
		inline static void SetKeyUp(uint64 vKey);
		//----------------------------------------------------------------------------------
		//[SetKeyDown]
		//GetKeyDown��ݒ肷��
		//����1: vKey
		inline static void SetKeyDown(int vKey);
		//----------------------------------------------------------------------------------
		//[SetKeyUp]
		//GetKeyUp��ݒ肷��
		//����1: vKey
		inline static void SetKeyUp(int vKey);

		static constexpr float m_cJoystickTriggerMax = 255.0f;															//�g���K�[�ő�l
		static constexpr float m_cJoystickStickMax = 32767.0f;															//�X�e�B�b�N�ő�l
		static constexpr float m_cJoystickVibrationMax = 65535.0f;													//�U���ő�l
		static constexpr uint m_cNowState = 0;																				//�z��[����]
		static constexpr uint m_cOldState = 1;																				//�z��[1�t���[���O]
		static constexpr uint m_cMessageState = 1;																		//�z��[RawInput]
		static constexpr uint m_cHighState = 0;																				//�z��[�����g]
		static constexpr uint m_cLowState = 1;																				//�z��[����g]
		static constexpr uint m_cKeyStatesVkeyMask = VariableLimit::cUshortMax;							//�S�ẴL�[�̃}�X�N		
		static constexpr uint m_cKeyStatesEmplaceDown = 0x10000000;											//Donw�r�b�g
		static constexpr uint m_cKeyStatesEmplaceUp = 0x20000000;												//Up�r�b�g	
		static constexpr uint m_cKeyStateSize = 256;																		//�z��T�C�Y
		static constexpr uint m_cJoystickStateSize = sizeof(XINPUT_STATE) * Joystick::cJoystickMax;	//�z��T�C�Y		
		static constexpr ushort m_cJoystickAllButtonMask = 0xF3FF;													//�S�Ẵ{�^���̃}�X�N		
		static constexpr byte m_cNextChangeAnyInfoBit = 0x10;														//���t���[���X�V�r�b�g		
		static constexpr byte m_cNextChangeAnyValueBit = 0x01;													//�X�V����l�r�b�g

		//ReConnectJoyStick���ɌĂԃR�[���o�b�N
		static std::function<void(uint joystickCount)> m_joystickCallback;
		//GetRapid�p�}�b�v
		static std::unordered_map<int, Rapid> m_keybordStateRapids;
		//GetRapid�pvector
		static std::vector<int> m_addKeybordStateRapids;
		//GetRapid�p�}�b�v
		static std::array<std::unordered_map<ushort, Rapid>, Joystick::cJoystickMax> m_joystickRapids;
		//GetRapid�pvector
		static std::vector <Tuple::Two<ushort, uint>> m_addJoystickRapids;
		//GetAxisUser�p�}�b�v
		static std::unordered_map<sgstring, AxisPack> m_axisPacks;
		//GetKeyDown or Up�pvector
		static std::vector<uint> m_getKeyDownOrUpVkeys;
		//GetKeyState�p�z��
		static std::array<std::array<byte, m_cKeyStateSize>, 2>  m_keyStates;
		//XIpnput->Input�p�z��
		static std::array<std::array<XINPUT_STATE, Joystick::cJoystickMax>, 2> m_joystickStates;
		//XInput->Vibration�p�z��
		static std::array<XINPUT_VIBRATION, Joystick::cJoystickMax>  m_joystickVibrations;
		//XInput->Vibration->�X�V�p�z��
		static std::array<std::array<VibrationQueue, Joystick::cJoystickMax>, 2> m_joystickSetVibrations;
		//XInput->Vibration->�X�V�p�L���[�z��
		static std::array<std::array<VibrationQueue, Joystick::cJoystickMax>, 2> m_joystickVibrationQueues;
		//�}�E�X�z�C�[��
		static std::array<short, 2> m_mouseWheels;		
		//WindowProcedure�p�����ϐ�
		static ConditionVariable::Auto m_inputCompletedCondition;
		static Vector2 m_mousePosition;							//�}�E�X�̈ʒu
		static Vector2 m_mouseAcceleration;					//�}�E�X�����x
		static Vector2 m_windowPosition;						//�E�B���h�E�̃|�W�V����
		static Vector2 m_windowSize;								//�E�B���h�E�̃T�C�Y
		static Vector2 m_windowAcceleration;					//�E�B���h�E�����x
		static float m_rapidInterval;								//�A�ˊԊu
		static float m_mouseDedZone;							//�}�E�X�̃f�b�h�]�[��
		static float m_joystickDedZone;							//�W���C�X�e�B�b�N�̃f�b�h�]�[��
		static float m_windowDedZone;							//�E�B���h�E�̃f�b�h�]�[��

		//���t���[��AnyMode�X�V�t���O
		static std::atomic<byte> m_isNextChangeAnyMode;
		//Any�L�����ۂ�
		static std::atomic<bool> m_isAnyMode;
		//rapid atomic
		static std::atomic<bool> m_rapidAtomic;
		//joystick atomic
		static std::atomic<bool> m_joystickAtomic;
		//axis atomic
		static std::atomic<bool> m_axisAtomic;
		static byte m_joystickCount;								//�W���C�X�e�B�b�N�ڑ���	
		static bool m_isAny;											//GetAny
		static bool m_isAnyDown;									//GetAnyDown
		static bool m_isAnyUp;										//GetAnyUp
		static bool m_isUseJoystick;								//�W���C�X�e�B�b�N�ڑ��� > 0
		static bool m_isMouseInsideFrame;						//�}�E�X���E�B���h�E���ɗ��߂邩
	};

	//----------------------------------------------------------------------------------
	//[ReceiveInputMessageWithWindowProcedure]
	//WindowProcedure�œ���Message����M����
	//����1: Message
	//����2: WPARAM
	//����3: LPARAM
	void Input::ReceiveInputMessageWithWindowProcedure(uint message, WPARAM wParam, LPARAM lParam)
	{
		//�r�b�g���Z�ŃL�[�{�[�hMessage������, ���
		if ((message & WM_KEYDOWN) && (((message & 0xFF) == 0x0) || (message & 0x05)))
			DecodingKeyboradMessage(message, wParam);
		//�r�b�g���Z�Ń}�E�XMessage������, ���
		else if ((message & WM_MOUSEMOVE) && (((message & 0xFF) == 0x0)
			|| ((message & 0xC) == 0xC) || (static_cast<byte>(message) <= 11 && static_cast<byte>(message) % 3 != 0)))
			DecodingMouseMessage(message, wParam, lParam);
	}
	//----------------------------------------------------------------------------------
	//[DecodingKeyboradMessage]
	//�L�[�{�[�hMessage����ǂ���GetKey���Z�b�g����
	//����1: Message
	//����2: WPARAM
	inline bool Input::DecodingKeyboradMessage(uint message, WPARAM wParam)
	{
		//KeyDown
		if (message == WM_KEYDOWN || message == WM_SYSKEYDOWN)
		{
			SetKeyDown(wParam);
			GUI::m_inputKeyDowns[wParam] = true;
			return true;
		}
		//KeyUp
		else if (message == WM_KEYUP || message == WM_SYSKEYUP)
		{
			SetKeyUp(wParam);
			GUI::m_inputKeyDowns[wParam] = false;
			return true;
		}
		else
			return false;
	}
	//----------------------------------------------------------------------------------
	//[DecodingMouseMessage]
	//�}�E�XMessage����ǂ���GetKey���Z�b�g����
	//����1: Message
	//����2: WPARAM
	//����3: LPARAM
	inline void Input::DecodingMouseMessage(uint message, WPARAM wParam, LPARAM lParam)
	{
		if (message <= WM_RBUTTONUP)
			DecodingMouseLRButtonMessage(message, wParam);
		else
			DecodingMouseOtherMessage(message, wParam);
	}
	//----------------------------------------------------------------------------------
	//[DecodingMouseLRButtonMessage]
	//DecodingMouseMessage����LR�{�^���̉�ǂɎg�p
	//����1: Message
	//����2: WPARAM
	inline void Input::DecodingMouseLRButtonMessage(uint message, WPARAM wParam)
	{
		//LButton
		if (message <= WM_LBUTTONUP)
		{
			//WM_LBUTTONDOWN
			if (message & 0x1)
			{
				SetKeyDown(MouseCode::Left);
				GUI::m_inputMouseDowns[0] = true;
			}
			//WM_LBUTTONUP
			else if (message & 0x2)
			{
				SetKeyUp(MouseCode::Left);
				GUI::m_inputMouseDowns[0] = false;
			}
		}
		//RButton
		else
		{
			//WM_RBUTTONUP
			if (message & 0x1)
			{ 
				SetKeyUp(MouseCode::Right);
				GUI::m_inputMouseDowns[1] = false;
			}
			//WM_RBUTTONDOWN
			else
			{
				SetKeyDown(MouseCode::Right);
				GUI::m_inputMouseDowns[1] = true;
			}
		}
	}
	//----------------------------------------------------------------------------------
	//[DecodingMouseOtherMessage]
	//DecodingMouseMessage����LR�{�^���ȊO�̉�ǂɎg�p
	//����1: Message
	//����2: WPARAM
	inline void Input::DecodingMouseOtherMessage(uint message, WPARAM wParam)
	{
		//MButton
		if (message <= WM_MBUTTONUP)
		{
			//WM_MBUTTONDOWN
			if (message & 0x1)
			{
				SetKeyDown(MouseCode::Center);
				GUI::m_inputMouseDowns[2] = true;
			}
			//WM_MBUTTONUP
			else
			{
				SetKeyUp(MouseCode::Center);
				GUI::m_inputMouseDowns[2] = false;
			}
		}
		//XButton or Wheel
		else
		{
			//WM_XBUTTONDOWN
			if (message & 0x1)
			{
				SetKeyDown(MouseCode::Button5 - ((wParam >> 16) & 1));
				GUI::m_inputMouseDowns[4 - ((wParam >> 16) & 1)] = true;
			}
			//WM_XBUTTONUP
			else if(message & 0x4)
			{
				SetKeyUp(MouseCode::Button5 - ((wParam >> 16) & 1));
				GUI::m_inputMouseDowns[4 - ((wParam >> 16) & 1)] = false;
			}
			//WM_MOUSEWHEEL
			else
			{
				m_mouseWheels[m_cMessageState] += static_cast<short>((wParam >> 16) & 0xFFFF) / WHEEL_DELTA;
				GUI::m_inputMouseWheel = static_cast<float>(m_mouseWheels[m_cMessageState]);
			}
		}
	}
	//----------------------------------------------------------------------------------
	//[SetKeyDown]
	//GetKeyDown��ݒ肷��
	//����1: vKey
	inline void Input::SetKeyDown(uint64 vKey)
	{
		if (m_keyStates[m_cMessageState][vKey] == 0)
		{
			m_getKeyDownOrUpVkeys.emplace_back(static_cast<uint>(vKey | m_cKeyStatesEmplaceDown));
			m_keyStates[m_cMessageState][vKey] = Keybord::cGetKeyDownCheckBit;
		}

		m_keyStates[m_cMessageState][vKey] |= Keybord::cGetKeyCheckBit;
	}
	//----------------------------------------------------------------------------------
	//[SetKeyUp]
	//GetKeyUp��ݒ肷��
	//����1: vKey
	inline void Input::SetKeyUp(uint64 vKey)
	{
		m_keyStates[m_cMessageState][vKey] = Keybord::cGetKeyUpCheckBit;

		m_getKeyDownOrUpVkeys.emplace_back(static_cast<uint>(vKey | m_cKeyStatesEmplaceUp));
	}
	//----------------------------------------------------------------------------------
	//[SetKeyDown]
	//GetKeyDown��ݒ肷��
	//����1: vKey
	inline void Input::SetKeyDown(int vKey)
	{
		if (m_keyStates[m_cMessageState][vKey] == 0)
		{
			m_getKeyDownOrUpVkeys.emplace_back(vKey | m_cKeyStatesEmplaceDown);
			m_keyStates[m_cMessageState][vKey] = Keybord::cGetKeyDownCheckBit;
		}

		m_keyStates[m_cMessageState][vKey] |= Keybord::cGetKeyCheckBit;
	}
	//----------------------------------------------------------------------------------
	//[SetKeyUp]
	//GetKeyUp��ݒ肷��
	//����1: vKey
	inline void Input::SetKeyUp(int vKey)
	{
		m_keyStates[m_cMessageState][vKey] = Keybord::cGetKeyUpCheckBit;

		m_getKeyDownOrUpVkeys.emplace_back(vKey | m_cKeyStatesEmplaceUp);
	}



	//----------------------------------------------------------------------------------
	//Any
	//----------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------
	//[GetAny]
	//return: �S�ẴL�[, �{�^���̂����ǂꂩ��������Ă�����true
	//				isAnyMode��true�łȂ��ꍇ�͕K��false��Ԃ�
	inline bool Input::Any::GetAny() { return m_isAny; }
	//----------------------------------------------------------------------------------
	//[GetAnyDown]
	//return: �S�ẴL�[, �{�^���̂����ǂꂩ�������ꂽ�u�ԂȂ�true
	//				isAnyMode��true�łȂ��ꍇ�͕K��false��Ԃ�
	inline bool Input::Any::GetAnyDown() { return m_isAnyDown; }
	//----------------------------------------------------------------------------------
	//[GetAnyUp]
	//return: �S�ẴL�[, �{�^���̂����ǂꂩ�����ꂽ�u�ԂȂ�true
	//				isAnyMode��true�łȂ��ꍇ�͕K��false��Ԃ�
	inline bool Input::Any::GetAnyUp() { return m_isAnyUp; };
	//----------------------------------------------------------------------------------
	//[SetAnyMode]
	//Any�n�֐��̗L���� or ��������ݒ肷��, default�ł�false
	//�S�Ă̓��͂��m�F����K�v������̂ŕK�v�Ȏ��ȊO��false�ɐݒ肷�邱�Ƃ𐄏�
	//����1: true = �L����, false = ������
	inline void Input::Any::SetAnyMode(bool mode)
	{
		AtomicOperation::Init(m_isNextChangeAnyMode, static_cast<byte>(m_cNextChangeAnyInfoBit | (IS_TRUE(mode) ? 1 : 0)));
	}



	//----------------------------------------------------------------------------------
	//Keybord
	//----------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------
	//[GetKey]
	//return: �ΏۃL�[��������Ă�����0���傫���l��Ԃ�, �����łȂ��ꍇ��0
	//����1:  KeyCode::�Z�Z
	inline byte Input::Keybord::GetKey(KeyCode::Enum keyCode)
	{
		return m_keyStates[m_cNowState][keyCode] & Keybord::cGetKeyCheckBit;
	}
	//----------------------------------------------------------------------------------
	//[GetKey]
	//return: �ΏۃL�[��������Ă�����0���傫���l��Ԃ�, �����łȂ��ꍇ��0
	//����1:  VK_�Z�Z
	inline byte Input::Keybord::GetKey(int vKey)
	{
		return m_keyStates[m_cNowState][vKey] & Keybord::cGetKeyCheckBit;
	}
	//----------------------------------------------------------------------------------
	//[GetKeyDown]
	//return: �ΏۃL�[�������ꂽ�u�ԂȂ�0���傫���l��Ԃ�, �����łȂ��ꍇ��0
	//����1:  KeyCode::�Z�Z
	inline byte Input::Keybord::GetKeyDown(KeyCode::Enum keyCode)
	{
		return m_keyStates[m_cNowState][keyCode] & cGetKeyDownCheckBit;
	}
	//----------------------------------------------------------------------------------
	//[GetKeyDown]
	//return: �ΏۃL�[�������ꂽ�u�ԂȂ�0���傫���l��Ԃ�, �����łȂ��ꍇ��0
	//����1:  VK_�Z�Z
	inline byte Input::Keybord::GetKeyDown(int vKey)
	{
		return m_keyStates[m_cNowState][vKey] & cGetKeyDownCheckBit;
	}
	//----------------------------------------------------------------------------------
	//[GetKeyUp]
	//return: �ΏۃL�[�����ꂽ�u�ԂȂ�0���傫���l��Ԃ�, �����łȂ��ꍇ��0
	//����1:  KeyCode::�Z�Z
	inline byte Input::Keybord::GetKeyUp(KeyCode::Enum keyCode)
	{
		return m_keyStates[m_cNowState][keyCode] & cGetKeyUpCheckBit;
	}
	//----------------------------------------------------------------------------------
	//[GetKeyUp]
	//return: �ΏۃL�[�����ꂽ�u�ԂȂ�0���傫���l��Ԃ�, �����łȂ��ꍇ��0
	//����1:  VK_�Z�Z
	inline byte Input::Keybord::GetKeyUp(int vKey)
	{
		return m_keyStates[m_cNowState][vKey] & cGetKeyUpCheckBit;
	}
	//----------------------------------------------------------------------------------
	//[GetRapid]
	//return: GetKey == true�̂��O��̐���������A�ˊԊu�b�����o�߂������̂�true��Ԃ�
	//				true�Ɣ��f���ꂽ�ꍇ�A�ˊԊu�b���͎����ŏ����������
	//����1:  KeyCode::�Z�Z
	inline bool Input::Keybord::GetRapid(KeyCode::Enum keyCode)
	{
		//find
		auto it = m_keybordStateRapids.find(keyCode);

		//���ɗv�f������
		if (it != m_keybordStateRapids.end())
		{
			//�擾�ltrue && interval�o�� && �I���t���Ofalse->true�ɏ�����������
			if ((m_keyStates[m_cNowState][keyCode] & Keybord::cGetKeyCheckBit)
				&& it->second.value >= m_rapidInterval && IS_FALSE(it->second.isEnd.exchange(true)))
				return true;
			else
				return false;
		}
		//�Ȃ��ꍇ�͓o�^
		else
		{
			AtomicOperation::LockAtomic(m_rapidAtomic);
			m_addKeybordStateRapids.emplace_back(keyCode);
			AtomicOperation::UnlockAtomic(m_rapidAtomic);
			return true;
		}
	}
	//----------------------------------------------------------------------------------
	//[GetRapid]
	//return: GetKey == true�̂��O��̐���������A�ˊԊu�b�����o�߂������̂�true��Ԃ�
	//				true�Ɣ��f���ꂽ�ꍇ�A�ˊԊu�b���͎����ŏ����������
	//����1:  VK_�Z�Z
	inline bool Input::Keybord::GetRapid(int vKey)
	{
		//find
		auto it = m_keybordStateRapids.find(vKey);

		//���ɗv�f������
		if (it != m_keybordStateRapids.end())
		{
			//�擾�ltrue && interval�o�� && �I���t���Ofalse->true�ɏ�����������
			if ((m_keyStates[m_cNowState][vKey] & Keybord::cGetKeyCheckBit)
				&& it->second.value >= m_rapidInterval && IS_FALSE(it->second.isEnd.exchange(true)))
				return true;
			else
				return false;
		}
		//�Ȃ��ꍇ�͓o�^
		else
		{
			AtomicOperation::LockAtomic(m_rapidAtomic);
			m_addKeybordStateRapids.emplace_back(vKey);
			AtomicOperation::UnlockAtomic(m_rapidAtomic);
			return true;
		}
	}



	//----------------------------------------------------------------------------------
	//Mouse
	//----------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------
	//[GetButton]
	//return: �Ώۃ{�^����������Ă�����0���傫���l��Ԃ�, �����łȂ��ꍇ��0
	//����1:  MouseCode::�Z�Z
	inline byte Input::Mouse::GetButton(MouseCode::Enum mouseCode)
	{
		return m_keyStates[m_cNowState][mouseCode] & cGetButtonCheckBit;
	}
	//----------------------------------------------------------------------------------
	//[GetButton]
	//return: �ΏۃL�[��������Ă�����0���傫���l��Ԃ�, �����łȂ��ꍇ��0
	//����1:  VK_�Z�Z
	inline byte Input::Mouse::GetButton(int vKey)
	{
		return m_keyStates[m_cNowState][vKey] & cGetButtonCheckBit;
	}
	//----------------------------------------------------------------------------------
	//[GetButtonDown]
	//return: �ΏۃL�[�������ꂽ�u�ԂȂ�0���傫���l��Ԃ�, �����łȂ��ꍇ��0
	//����1:  MouseCode::�Z�Z
	inline byte Input::Mouse::GetButtonDown(MouseCode::Enum mouseCode)
	{
		return m_keyStates[m_cNowState][mouseCode] & cGetButtonDownCheckBit;
	}
	//----------------------------------------------------------------------------------
	//[GetButtonDown]
	//return: �ΏۃL�[�������ꂽ�u�ԂȂ�0���傫���l��Ԃ�, �����łȂ��ꍇ��0
	//����1:  VK_�Z�Z
	inline byte Input::Mouse::GetButtonDown(int vKey)
	{
		return m_keyStates[m_cNowState][vKey] & cGetButtonDownCheckBit;
	}
	//----------------------------------------------------------------------------------
	//[GetButtonUp]
	//return: �ΏۃL�[�����ꂽ�u�ԂȂ�0���傫���l��Ԃ�, �����łȂ��ꍇ��0
	//����1:  MouseCode::�Z�Z
	inline byte Input::Mouse::GetButtonUp(MouseCode::Enum mouseCode)
	{
		return m_keyStates[m_cNowState][mouseCode] & cGetButtonUpCheckBit;
	}
	//----------------------------------------------------------------------------------
	//[GetButtonUp]
	//return: �ΏۃL�[�����ꂽ�u�ԂȂ�0���傫���l��Ԃ�, �����łȂ��ꍇ��0
	//����1:  VK_�Z�Z
	inline byte Input::Mouse::GetButtonUp(int vKey)
	{
		return m_keyStates[m_cNowState][vKey] & cGetButtonUpCheckBit;
	}
	//----------------------------------------------------------------------------------
	//[GetRapid]
	//return: GetButton == true�̂��O��̐���������A�ˊԊu�b�����o�߂������̂�true��Ԃ�
	//				true�Ɣ��f���ꂽ�ꍇ�A�ˊԊu�b���͎����ŏ����������
	//����1:  MouseCode::�Z�Z
	inline bool Input::Mouse::GetRapid(MouseCode::Enum mouseCode)
	{
		//find
		auto it = m_keybordStateRapids.find(mouseCode);
		
		//���ɗv�f������
		if (it != m_keybordStateRapids.end())
		{
			//�擾�ltrue && interval�o�� && �I���t���Ofalse->true�ɏ�����������
			if ((m_keyStates[m_cNowState][mouseCode] & cGetButtonCheckBit)
				&& it->second.value >= m_rapidInterval && IS_FALSE(it->second.isEnd.exchange(true)))
				return true;
			else
				return false;
		}
		//�Ȃ��ꍇ�͓o�^
		else
		{
			AtomicOperation::LockAtomic(m_rapidAtomic);
			m_addKeybordStateRapids.emplace_back(mouseCode);
			AtomicOperation::UnlockAtomic(m_rapidAtomic);
			return true;
		}
	}
	//----------------------------------------------------------------------------------
	//[GetRapid]
	//return: GetButton == true�̂��O��̐���������A�ˊԊu�b�����o�߂������̂�true��Ԃ�
	//				true�Ɣ��f���ꂽ�ꍇ�A�ˊԊu�b���͎����ŏ����������
	//����1:  VK_�Z�Z
	inline bool Input::Mouse::GetRapid(int vKey)
	{
		//find
		auto it = m_keybordStateRapids.find(vKey);

		//���ɗv�f������
		if (it != m_keybordStateRapids.end())
		{
			//�擾�ltrue && interval�o�� && �I���t���Ofalse->true�ɏ�����������
			if ((m_keyStates[m_cNowState][vKey] & cGetButtonCheckBit)
				&& it->second.value >= m_rapidInterval && IS_FALSE(it->second.isEnd.exchange(true)))
				return true;
			else
				return false;
		}
		//�Ȃ��ꍇ�͓o�^
		else
		{
			AtomicOperation::LockAtomic(m_rapidAtomic);
			m_addKeybordStateRapids.emplace_back(vKey);
			AtomicOperation::UnlockAtomic(m_rapidAtomic);
			return true;
		}
	}
	//----------------------------------------------------------------------------------
	//[GetPosition]
	//return: �E�B���h�E�̑傫�����c��-1.0f ~ 1.0f�Ƃ����ꍇ�̌��݂̃}�E�X���W
	inline Vector2 Input::Mouse::GetPosition()
	{
		return m_mousePosition;
	}
	//----------------------------------------------------------------------------------
	//[GetPositionToWindow]
	//return: �E�B���h�E�̑傫�����c��0.0f ~ 1.0f�Ƃ����ꍇ�̌��݂̃}�E�X���W
	inline Vector2 Input::Mouse::GetPositionToWindow()
	{
		return m_mousePosition / 2 + Const::Vector2::one;
	}
	//----------------------------------------------------------------------------------
	//[GetAcceleration]
	//return: �O�t���[����̃|�W�V�����ړ���, �e�v�f�̐�Βl��dedZone�ȉ��̏ꍇ�K��0.0f�ƂȂ�
	inline Vector2 Input::Mouse::GetAcceleration()
	{
		Vector2 result;
		result.x = (MathAF::Absolute(m_mouseAcceleration.x) > m_mouseDedZone) ?
			m_mouseAcceleration.x : 0.0f;
		result.y = (MathAF::Absolute(m_mouseAcceleration.y) > m_mouseDedZone) ?
			m_mouseAcceleration.y : 0.0f;
		return result;
	}
	//----------------------------------------------------------------------------------
	//[GetAcceleration]
	//return: dedZone�̉e�����󂯂Ȃ��O�t���[����̃|�W�V�����ړ���
	inline Vector2 Input::Mouse::GetRawAcceleration()
	{
		return m_mouseAcceleration;
	}
	//----------------------------------------------------------------------------------
	//[GetWheelDelta]
	//return: ���݂�wheel�J�ڏ�� (�� -> +1~, ��O -> -1~)
	inline short Input::Mouse::GetWheelDelta()
	{
		return m_mouseWheels[m_cNowState];
	}



	//----------------------------------------------------------------------------------
	//Joystick
	//----------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------
	//[GetButton]
	//return: �Ώۃ{�^����������Ă�����0���傫���l��Ԃ�, �����łȂ��ꍇ��0
	//����1: JoystickCode::�Z�Z
	//����2: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), ��O�����͍s��Ȃ�, default = 0
	inline ushort Input::Joystick::GetButton(JoystickCode::Enum joystickCode, uint useStick)
	{
		return (m_joystickStates[m_cNowState][useStick].Gamepad.wButtons & joystickCode);
	}
	//----------------------------------------------------------------------------------
	//[GetButtonDown]
	//return: �ΏۃL�[�������ꂽ�u�ԂȂ�0���傫���l��Ԃ�, �����łȂ��ꍇ��0
	//����1: JoystickCode::�Z�Z
	//����2: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), ��O�����͍s��Ȃ�, default = 0
	inline ushort Input::Joystick::GetButtonDown(JoystickCode::Enum joystickCode, uint useStick)
	{
		return (((m_joystickStates[m_cNowState][useStick].Gamepad.wButtons ^ m_joystickStates[m_cOldState][useStick].Gamepad.wButtons)
			&  m_joystickStates[m_cNowState][useStick].Gamepad.wButtons) & joystickCode);
	}
	//----------------------------------------------------------------------------------
	//[GetButtonUp]
	//return: �ΏۃL�[�����ꂽ�u�ԂȂ�0���傫���l��Ԃ�, �����łȂ��ꍇ��0
	//����1: JoystickCode::�Z�Z
	//����2: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), ��O�����͍s��Ȃ�, default = 0
	inline ushort Input::Joystick::GetButtonUp(JoystickCode::Enum joystickCode, uint useStick)
	{
		return (((m_joystickStates[m_cNowState][useStick].Gamepad.wButtons ^ m_joystickStates[m_cOldState][useStick].Gamepad.wButtons)
			&  m_joystickStates[m_cOldState][useStick].Gamepad.wButtons) & joystickCode);
	}
	//----------------------------------------------------------------------------------
	//[GetRapid]
	//return: GetButton == true�̂��O��̐���������A�ˊԊu�b�����o�߂������̂�true��Ԃ�
	//				true�Ɣ��f���ꂽ�ꍇ�A�ˊԊu�b���͎����ŏ����������
	//����1: JoystickCode::�Z�Z
	//����2: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), ��O�����͍s��Ȃ�, default = 0
	inline bool Input::Joystick::GetRapid(JoystickCode::Enum joystickCode, uint useStick)
	{
		//find
		auto it = m_joystickRapids[useStick].find(joystickCode);

		//���ɗv�f������
		if (it != m_joystickRapids[useStick].end())
		{
			//�擾�ltrue && interval�o�� && �I���t���Ofalse->true�ɏ�����������
			if ((m_joystickStates[m_cNowState][useStick].Gamepad.wButtons  & joystickCode)
				&& it->second.value >= m_rapidInterval && IS_FALSE(it->second.isEnd.exchange(true)))
				return true;
			else
				return false;
		}
		//�Ȃ��ꍇ�o�^����
		else
		{
			AtomicOperation::LockAtomic(m_rapidAtomic);
			m_addJoystickRapids.emplace_back(joystickCode, useStick);
			AtomicOperation::UnlockAtomic(m_rapidAtomic);
			return true;
		}
	}
	//----------------------------------------------------------------------------------
	//[GetLTrigger]
	//return: 0.0f ~ 1.0f�̒l�ƂȂ�L�g���K�[�̌��ݒl, ��Βl��dedZone�ȉ��̏ꍇ�K��0.0f��ԋp����
	//����1: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), ��O�����͍s��Ȃ�, default = 0
	inline float Input::Joystick::GetLTrigger(uint useStick)
	{
		float now = static_cast<float>(m_joystickStates[m_cNowState][useStick].Gamepad.bLeftTrigger) / m_cJoystickTriggerMax;
		return (now > m_joystickDedZone) ? now : 0.0f;
	}
	//----------------------------------------------------------------------------------
	//[GetRawLTrigger]
	//return: 0.0f ~ 1.0f�̒l�ƂȂ�dedZone���l�����Ȃ�L�g���K�[�̌��ݒl
	//����1: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), ��O�����͍s��Ȃ�, default = 0
	inline float Input::Joystick::GetRawLTrigger(uint useStick)
	{
		return static_cast<float>(m_joystickStates[m_cNowState][useStick].Gamepad.bLeftTrigger) / m_cJoystickTriggerMax;
	}
	inline float Input::Joystick::GetLTriggerAcceleration(uint useStick)
	{
		float now = static_cast<float>(m_joystickStates[m_cNowState][useStick].Gamepad.bLeftTrigger) / m_cJoystickTriggerMax;
		float old = static_cast<float>(m_joystickStates[m_cOldState][useStick].Gamepad.bLeftTrigger) / m_cJoystickTriggerMax;

		if (now > m_joystickDedZone)
			return (old > m_joystickDedZone) ? now - old : now - m_joystickDedZone;
		else
			return (old > m_joystickDedZone) ? m_joystickDedZone - old : 0.0f;
	}
	//----------------------------------------------------------------------------------
	//[GetRawLTriggerAcceleration]
	//return: 0.0f ~ 1.0f�̒l�ƂȂ�dedZone���l�����Ȃ�L�g���K�[�̑O�t���[����ړ���
	//����1: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), ��O�����͍s��Ȃ�, default = 0
	inline float Input::Joystick::GetRawLTriggerAcceleration(uint useStick)
	{
		return static_cast<float>(m_joystickStates[m_cNowState][useStick].Gamepad.bLeftTrigger) / m_cJoystickTriggerMax
			- static_cast<float>(m_joystickStates[m_cOldState][useStick].Gamepad.bLeftTrigger) / m_cJoystickTriggerMax;
	}
	//----------------------------------------------------------------------------------
	//[GetRTrigger]
	//return: 0.0f ~ 1.0f�̒l�ƂȂ�R�g���K�[�̌��ݒl, ��Βl��dedZone�ȉ��̏ꍇ�K��0.0f��ԋp����
	//����1: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), ��O�����͍s��Ȃ�, default = 0
	inline float Input::Joystick::GetRTrigger(uint useStick)
	{
		float now = static_cast<float>(m_joystickStates[m_cNowState][useStick].Gamepad.bRightTrigger) / m_cJoystickTriggerMax;
		return (now > m_joystickDedZone) ? now : 0.0f;
	}
	//----------------------------------------------------------------------------------
	//[GetRawRTrigger]
	//return: 0.0f ~ 1.0f�̒l�ƂȂ�dedZone���l�����Ȃ�R�g���K�[�̌��ݒl
	//����1: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), ��O�����͍s��Ȃ�, default = 0
	inline float Input::Joystick::GetRawRTrigger(uint useStick)
	{
		return static_cast<float>(m_joystickStates[m_cNowState][useStick].Gamepad.bRightTrigger) / m_cJoystickTriggerMax;
	}
	//----------------------------------------------------------------------------------
	//[GetRTriggerAcceleration]
	//return: 0.0f ~ 1.0f�̒l�����R�g���K�[�̑O�t���[����̈ړ���,
	//				��r�Ɏg����l��dedZone�̉e�����󂯂�
	//����1: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), ��O�����͍s��Ȃ�, default = 0
	inline float Input::Joystick::GetRTriggerAcceleration(uint useStick)
	{
		float now = static_cast<float>(m_joystickStates[m_cNowState][useStick].Gamepad.bRightTrigger) / m_cJoystickTriggerMax;
		float old = static_cast<float>(m_joystickStates[m_cOldState][useStick].Gamepad.bRightTrigger) / m_cJoystickTriggerMax;

		if (now > m_joystickDedZone) return (old > m_joystickDedZone) ? now - old : now - m_joystickDedZone;
		else return (old > m_joystickDedZone) ? m_joystickDedZone - old : 0.0f;
	}
	//----------------------------------------------------------------------------------
	//[GetRawRTriggerAcceleration]
	//return: 0.0f ~ 1.0f�̒l�ƂȂ�dedZone���l�����Ȃ�R�g���K�[�̑O�t���[����ړ���
	//����1: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), ��O�����͍s��Ȃ�, default = 0
	inline float Input::Joystick::GetRawRTriggerAcceleration(uint useStick)
	{
		return static_cast<float>(m_joystickStates[m_cNowState][useStick].Gamepad.bRightTrigger) / m_cJoystickTriggerMax
			- static_cast<float>(m_joystickStates[m_cOldState][useStick].Gamepad.bRightTrigger) / m_cJoystickTriggerMax;
	}
	//----------------------------------------------------------------------------------
	//[GetLStick]
	//return: -1.0f ~ 1.0f�̒l�ƂȂ�L�X�e�B�b�N�̌��ݒl, �e�v�f�̐�Βl��dedZone�ȉ��̏ꍇ�K��0.0f�ƂȂ�
	//����1: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), ��O�����͍s��Ȃ�, default = 0
	inline Vector2 Input::Joystick::GetLStick(uint useStick)
	{
		Vector2 result(Vector2::CreateWithInt(m_joystickStates[m_cNowState][useStick].Gamepad.sThumbLX,
			m_joystickStates[m_cNowState][useStick].Gamepad.sThumbLY) / m_cJoystickStickMax);

		if (MathAF::Absolute(result.x) <= m_joystickDedZone) result.x = 0.0f;
		if (MathAF::Absolute(result.y) <= m_joystickDedZone) result.y = 0.0f;

		return result;
	}
	//----------------------------------------------------------------------------------
	//[GetRawLStick]
	//return: -1.0f ~ 1.0f�̒l�ƂȂ�dedZone���l�����Ȃ�L�X�e�B�b�N�̌��ݒl
	//����1: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), ��O�����͍s��Ȃ�, default = 0
	inline Vector2 Input::Joystick::GetRawLStick(uint useStick)
	{
		return Vector2::CreateWithInt(m_joystickStates[m_cNowState][useStick].Gamepad.sThumbLX,
			m_joystickStates[m_cNowState][useStick].Gamepad.sThumbLY) / m_cJoystickStickMax;
	}
	//----------------------------------------------------------------------------------
	//[GetLStickAcceleration]
	//return: -1.0f ~ 1.0f�̒l�����L�X�e�B�b�N�̑O�t���[����̈ړ���,
	//				��r�Ɏg����l��dedZone�̉e�����󂯂�
	//����1: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), ��O�����͍s��Ȃ�, default = 0
	inline Vector2 Input::Joystick::GetLStickAcceleration(uint useStick)
	{
		Vector2 nowResult(Vector2::CreateWithInt(m_joystickStates[m_cNowState][useStick].Gamepad.sThumbLX,
			m_joystickStates[m_cNowState][useStick].Gamepad.sThumbLY) / m_cJoystickStickMax);
		Vector2 oldResult(Vector2::CreateWithInt(m_joystickStates[m_cOldState][useStick].Gamepad.sThumbLX,
			m_joystickStates[m_cOldState][useStick].Gamepad.sThumbLY) / m_cJoystickStickMax);


		if (MathAF::Absolute(nowResult.x) > m_joystickDedZone)
		{
			if (MathAF::Absolute(oldResult.x) > m_joystickDedZone)
				nowResult.x = nowResult.x - oldResult.x;
			else if (nowResult.x > 0.0f)
				nowResult.x = nowResult.x - m_joystickDedZone;
			else
				nowResult.x = nowResult.x + m_joystickDedZone;
		}
		else
		{
			if (oldResult.x > m_joystickDedZone)
				nowResult.x = m_joystickDedZone - oldResult.x;
			else if (oldResult.x < m_joystickDedZone)
				nowResult.x = -m_joystickDedZone - oldResult.x;
			else
				nowResult.x = 0.0f;
		}

		return nowResult;
	}
	//----------------------------------------------------------------------------------
	//[GetRawLStickAcceleration]
	//return: -1.0f ~ 1.0f�̒l�ƂȂ�dedZone���l�����Ȃ�L�X�e�B�b�N�̑O�t���[����ړ���
	//����1: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), ��O�����͍s��Ȃ�, default = 0
	inline Vector2 Input::Joystick::GetRawLStickAcceleration(uint useStick)
	{
		return Vector2::CreateWithInt(m_joystickStates[m_cNowState][useStick].Gamepad.sThumbLX - m_joystickStates[m_cOldState][useStick].Gamepad.sThumbLX,
			m_joystickStates[m_cNowState][useStick].Gamepad.sThumbLY - m_joystickStates[m_cOldState][useStick].Gamepad.sThumbLY) / m_cJoystickStickMax;
	}
	//----------------------------------------------------------------------------------
	//[GetRStick]
	//return: -1.0f ~ 1.0f�̒l�ƂȂ�R�X�e�B�b�N�̌��ݒl, �e�v�f�̐�Βl��dedZone�ȉ��̏ꍇ�K��0.0f�ƂȂ�
	//����1: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), ��O�����͍s��Ȃ�, default = 0
	inline Vector2 Input::Joystick::GetRStick(uint useStick)
	{
		Vector2 result(Vector2::CreateWithInt(m_joystickStates[m_cNowState][useStick].Gamepad.sThumbRX,
			m_joystickStates[m_cNowState][useStick].Gamepad.sThumbRY) / m_cJoystickStickMax);

		if (MathAF::Absolute(result.x) <= m_joystickDedZone) result.x = 0.0f;
		if (MathAF::Absolute(result.y) <= m_joystickDedZone) result.y = 0.0f;

		return result;
	}
	//----------------------------------------------------------------------------------
	//[GetRawRStick]
	//return: -1.0f ~ 1.0f�̒l�ƂȂ�dedZone���l�����Ȃ�R�X�e�B�b�N�̌��ݒl
	//����1: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), ��O�����͍s��Ȃ�, default = 0
	inline Vector2 Input::Joystick::GetRawRStick(uint useStick)
	{
		return Vector2::CreateWithInt(m_joystickStates[m_cNowState][useStick].Gamepad.sThumbRX,
			m_joystickStates[m_cNowState][useStick].Gamepad.sThumbRY) / m_cJoystickStickMax;
	}
	//----------------------------------------------------------------------------------
	//[GetRStickAcceleration]
	//return: -1.0f ~ 1.0f�̒l�����R�X�e�B�b�N�̑O�t���[����̈ړ���,
	//				��r�Ɏg����l��dedZone�̉e�����󂯂�
	//����1: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), ��O�����͍s��Ȃ�, default = 0
	inline Vector2 Input::Joystick::GetRStickAcceleration(uint useStick)
	{
		Vector2 nowResult(Vector2::CreateWithInt(m_joystickStates[m_cNowState][useStick].Gamepad.sThumbRX,
			m_joystickStates[m_cNowState][useStick].Gamepad.sThumbRY) / m_cJoystickStickMax);
		Vector2 oldResult(Vector2::CreateWithInt(m_joystickStates[m_cOldState][useStick].Gamepad.sThumbRX,
			m_joystickStates[m_cOldState][useStick].Gamepad.sThumbRY) / m_cJoystickStickMax);

		if (MathAF::Absolute(nowResult.x) > m_joystickDedZone)
		{
			if (MathAF::Absolute(oldResult.x) > m_joystickDedZone)
				nowResult.x = nowResult.x - oldResult.x;
			else if (nowResult.x > 0.0f)
				nowResult.x = nowResult.x - m_joystickDedZone;
			else
				nowResult.x = nowResult.x + m_joystickDedZone;
		}
		else
		{
			if (oldResult.x > m_joystickDedZone)
				nowResult.x = m_joystickDedZone - oldResult.x;
			else if (oldResult.x < m_joystickDedZone)
				nowResult.x = -m_joystickDedZone - oldResult.x;
			else
				nowResult.x = 0.0f;
		}

		return nowResult;
	}
	//----------------------------------------------------------------------------------
	//[GetRawRStickAcceleration]
	//return: -1.0f ~ 1.0f�̒l�ƂȂ�dedZone���l�����Ȃ�R�X�e�B�b�N�̑O�t���[����ړ���
	//����1: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), ��O�����͍s��Ȃ�, default = 0
	inline Vector2 Input::Joystick::GetRawRStickAcceleration(uint useStick)
	{
		return Vector2::CreateWithInt(m_joystickStates[m_cNowState][useStick].Gamepad.sThumbRX - m_joystickStates[m_cOldState][useStick].Gamepad.sThumbRX,
			m_joystickStates[m_cNowState][useStick].Gamepad.sThumbRY - m_joystickStates[m_cOldState][useStick].Gamepad.sThumbRY) / m_cJoystickStickMax;
	}
	//----------------------------------------------------------------------------------
	//[GetHighVibration]
	//return: 0.0f ~ 1.0f�̒l�ƂȂ錻�݂̍����g�U���l
	//����1: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), ��O�����͍s��Ȃ�, default = 0
	inline float Input::Joystick::GetHighVibration(uint useStick)
	{
		return static_cast<float>(m_joystickVibrations[useStick].wRightMotorSpeed) / m_cJoystickVibrationMax;;
	}
	//----------------------------------------------------------------------------------
	//[GetLowVibration]
	//return: 0.0f ~ 1.0f�̒l�ƂȂ錻�݂̒���g�U���l
	//����1: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), ��O�����͍s��Ȃ�, default = 0
	inline float Input::Joystick::GetLowVibration(uint useStick)
	{
		return static_cast<float>(m_joystickVibrations[useStick].wLeftMotorSpeed) / m_cJoystickVibrationMax;;
	}
	//----------------------------------------------------------------------------------
	//[SetHighVibration]
	//�����g�U���l��ݒ肷��, ���s��1�t���[����ɍs���邪�AFadeOut���̏ꍇ�͓o�^�Ɏ��s����
	//�e�R���g���[���[��High, Low�e��������ݒ肷�鎖�͂ł��Ȃ�
	//return: �ݒ肪���������true, ���s(���łɐݒ肳��Ă��� or FadeOut��)�̏ꍇfalse
	//����1: �ݒ肷��U���l, 0.0f ~ 1.0f��Clamp�����
	//����2: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), �ڑ�����Ă��Ȃ��ꍇ���s, default = 0
	//����3: true�̏ꍇ, ���ɐݒ�ς݂̏ꍇ�������I�ɐݒ肷��, default = false
	inline bool Input::Joystick::SetHighVibration(float setVibration, uint useStick, bool isForced)
	{
		if (useStick >= m_joystickCount) return false;

		setVibration = MathAF::Clamp01(setVibration);
		AtomicOperation::LockAtomic(m_joystickAtomic);

		//���� or �o�^�\
		if (IS_TRUE(isForced) | IS_FALSE(m_joystickSetVibrations[m_cHighState][useStick].isLive))
		{
			m_joystickVibrationQueues[m_cHighState][useStick].SetValue(setVibration, 0.0f, 0.0f, useStick, VibrationQueue::cSet, isForced);
			AtomicOperation::UnlockAtomic(m_joystickAtomic);
			return true;
		}

		AtomicOperation::UnlockAtomic(m_joystickAtomic);
		return false;
	}
	//----------------------------------------------------------------------------------
	//[SetLowVibration]
	//����g�U���l��ݒ肷��, ���s��1�t���[����ɍs���邪�AFadeOut���̏ꍇ�͓o�^�Ɏ��s����
	//�e�R���g���[���[��High, Low�e��������ݒ肷�鎖�͂ł��Ȃ�
	//return: �ݒ肪���������true, ���s(���łɐݒ肳��Ă��� or FadeOut��)�̏ꍇfalse
	//����1: �ݒ肷��U���l, 0.0f ~ 1.0f��Clamp�����
	//����2: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), �ڑ�����Ă��Ȃ��ꍇ���s, default = 0
	//����3: true�̏ꍇ, ���ɐݒ�ς݂̏ꍇ�������I�ɐݒ肷��, default = false
	inline bool Input::Joystick::SetLowVibration(float setVibration, uint useStick, bool isForced)
	{
		if (useStick >= m_joystickCount) return false;

		setVibration = MathAF::Clamp01(setVibration);
		AtomicOperation::LockAtomic(m_joystickAtomic);

		//���� or �o�^�\
		if (IS_TRUE(isForced) | IS_FALSE(m_joystickSetVibrations[m_cLowState][useStick].isLive))
		{
			m_joystickVibrationQueues[m_cLowState][useStick].SetValue(setVibration, 0.0f, 0.0f, useStick, VibrationQueue::cSet, isForced);
			AtomicOperation::UnlockAtomic(m_joystickAtomic);
			return true;
		}
	
		AtomicOperation::UnlockAtomic(m_joystickAtomic);
		return false;
	}
	//----------------------------------------------------------------------------------
	//[SetHighVibrationFadeOut]
	//�����g�U���l��ݒ�l����0.0f�փt�F�[�h�A�E�g����悤�ɐݒ肷��
	//���s��1�t���[����ɍs���邪�AFadeOut���̏ꍇ�͓o�^�Ɏ��s����
	//���s������ setVibration�ݒ� -> waitReachSeconds�o�� -> reachZeroSeconds������0.0f�܂Ō���
	//�e�R���g���[���[��High, Low�e��������ݒ肷�鎖�͂ł��Ȃ�
	//return: �ݒ肪���������true, ���s(���łɐݒ肳��Ă��� or FadeOut��)�̏ꍇfalse
	//����1: �ݒ肷��U���l, 0.0f ~ 1.0f��Clamp�����
	//����2: �������n�߂�܂ł̑ҋ@����
	//����3: 0.0f�ɂȂ�܂ł����鏊�v����
	//����4: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), �ڑ�����Ă��Ȃ��ꍇ���s, default = 0
	//����5: true�̏ꍇ, ���ɐݒ�ς݂̏ꍇ�������I�ɐݒ肷��, default = false
	inline bool Input::Joystick::SetHighVibrationFadeOut(float setVibration, float waitReachSeconds, float reachZeroSeconds, uint useStick, bool isForced)
	{
		if (useStick >= m_joystickCount) return false;

		setVibration = MathAF::Clamp01(setVibration);
		AtomicOperation::LockAtomic(m_joystickAtomic);

		//���� or �o�^�\
		if (IS_TRUE(isForced) | IS_FALSE(m_joystickSetVibrations[m_cHighState][useStick].isLive))
		{
			m_joystickVibrationQueues[m_cHighState][useStick].SetValue(setVibration, setVibration / reachZeroSeconds, waitReachSeconds,
				useStick, VibrationQueue::cFadeOut | VibrationQueue::cWaitState | VibrationQueue::StartState, isForced);
			AtomicOperation::UnlockAtomic(m_joystickAtomic);
		}

		AtomicOperation::UnlockAtomic(m_joystickAtomic);
	}
	//----------------------------------------------------------------------------------
	//[SetLowVibrationFadeOut]
	//����g�U���l��ݒ�l����0.0f�փt�F�[�h�A�E�g����悤�ɐݒ肷��			
	//���s��1�t���[����ɍs���邪�AFadeOut���̏ꍇ�͓o�^�Ɏ��s����
	//���s������ setVibration�ݒ� -> waitReachSeconds�o�� -> reachZeroSeconds������0.0f�܂Ō���
	//�e�R���g���[���[��High, Low�e��������ݒ肷�鎖�͂ł��Ȃ�
	//return: �ݒ肪���������true, ���s(���łɐݒ肳��Ă��� or FadeOut��)�̏ꍇfalse
	//����1: �ݒ肷��U���l, 0.0f ~ 1.0f��Clamp�����
	//����2: �������n�߂�܂ł̑ҋ@����
	//����3: 0.0f�ɂȂ�܂ł����鏊�v����
	//����4: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), �ڑ�����Ă��Ȃ��ꍇ���s, default = 0
	//����5: true�̏ꍇ, ���ɐݒ�ς݂̏ꍇ�������I�ɐݒ肷��, default = false
	inline bool Input::Joystick::SetLowVibrationFadeOut(float setVibration, float waitReachSeconds, float reachZeroSeconds, uint useStick, bool isForced)
	{
		if (useStick >= m_joystickCount) return false;

		setVibration = MathAF::Clamp01(setVibration);
		AtomicOperation::LockAtomic(m_joystickAtomic);

		//���� or �o�^�\
		if (IS_TRUE(isForced) | IS_FALSE(m_joystickSetVibrations[m_cLowState][useStick].isLive))
		{
			m_joystickVibrationQueues[m_cLowState][useStick].SetValue(setVibration, setVibration / reachZeroSeconds, waitReachSeconds,
				useStick, VibrationQueue::cFadeOut | VibrationQueue::cWaitState | VibrationQueue::StartState, isForced);
			AtomicOperation::UnlockAtomic(m_joystickAtomic);
		}

		AtomicOperation::UnlockAtomic(m_joystickAtomic);
	}
	//----------------------------------------------------------------------------------
	//[ResetVibration]
	//���݂̐U���ݒ�l��High Low�Ƃ���0.0f�܂Ńt�F�[�h�A�E�g������
	//���̊֐��͊��ɐݒ肳��Ă��邩�ۂ��Ɋւ�炸�����I�Ɏ��s�����
	//����1: 0.0f�ɂȂ�܂ł����鏊�v����
	//����2: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), ��O�����͍s��Ȃ�, default = 0
	inline void Input::Joystick::ResetVibration(float reachZeroSpeedPerSeconds, uint useStick)
	{
		float target1 = static_cast<float>(m_joystickVibrations[useStick].wRightMotorSpeed) / m_cJoystickVibrationMax,
			target2 = static_cast<float>(m_joystickVibrations[useStick].wLeftMotorSpeed) / m_cJoystickVibrationMax;
		
		AtomicOperation::LockAtomic(m_joystickAtomic);
		if (target1 > 0.0f) m_joystickVibrationQueues[m_cHighState][useStick].SetValue(target1, reachZeroSpeedPerSeconds, 0.0f, useStick, VibrationQueue::cFadeOut, true);
		if (target2 > 0.0f) m_joystickVibrationQueues[m_cLowState][useStick].SetValue(target2, reachZeroSpeedPerSeconds, 0.0f, useStick, VibrationQueue::cFadeOut, true);
		AtomicOperation::UnlockAtomic(m_joystickAtomic);
	}
	//----------------------------------------------------------------------------------
	//[ResetAllVibration]
	//�S�ẴW���C�X�e�B�b�N�Ō��݂̐U���ݒ�l��High Low�Ƃ���0.0f�܂Ńt�F�[�h�A�E�g������
	//���̊֐��͊��ɐݒ肳��Ă��邩�ۂ��Ɋւ�炸�����I�Ɏ��s�����
	//����1: 0.0f�ɂȂ�܂ł����鏊�v����
	inline void Input::Joystick::ResetAllVibration(float reachZeroSpeedPerSeconds)
	{
		float target1, target2;

		AtomicOperation::LockAtomic(m_joystickAtomic);
		for (uint i = 0; i < m_joystickCount; i++)
		{
			target1 = static_cast<float>(m_joystickVibrations[i].wRightMotorSpeed) / m_cJoystickVibrationMax;
			target2 = static_cast<float>(m_joystickVibrations[i].wLeftMotorSpeed) / m_cJoystickVibrationMax;
			
			if (target1 > 0.0f) m_joystickVibrationQueues[m_cHighState][i].SetValue(target1, reachZeroSpeedPerSeconds, 0.0f, i, VibrationQueue::cFadeOut, true);
			if (target2 > 0.0f) m_joystickVibrationQueues[m_cLowState][i].SetValue(target2, reachZeroSpeedPerSeconds, 0.0f, i, VibrationQueue::cFadeOut, true);
		}
		AtomicOperation::UnlockAtomic(m_joystickAtomic);
	}
	//----------------------------------------------------------------------------------
	//[SetAutoReConnectCallback]
	//�ڑ����̕ύX���s��ꂽ���̃R�[���o�b�N�֐���ݒ肷��
	//����1: �R�[���o�b�N�֐�, ����1: �V�����W���C�X�e�B�b�N�̐ڑ���(uint)
	inline void Input::Joystick::SetAutoReConnectCallback(const std::function<void(uint joystickCount)>& callback)
	{
		AtomicOperation::LockAtomic(m_joystickAtomic);
		m_joystickCallback = callback;
		AtomicOperation::UnlockAtomic(m_joystickAtomic);
	}



	//----------------------------------------------------------------------------------
	//Window
	//----------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------
	//[GetPosition]
	//return: ��ʉ𑜓x�� 0.0f ~ 1.0f�Ƃ����ꍇ�̃E�B���h�E�̒��S���W
	inline Vector2 Input::Window::GetPosition()
	{
		return m_windowPosition;
	}
	//----------------------------------------------------------------------------------
	//[GetSize]
	//return: ��ʉ𑜓x�� 0.0f ~ 1.0f�Ƃ����ꍇ�̃E�B���h�E�̃T�C�Y
	inline Vector2 Input::Window::GetSize()
	{
		return m_windowSize;
	}
	//----------------------------------------------------------------------------------
	//[GetAcceleration]
	//return: ��ʉ𑜓x�� 0.0f ~ 1.0f�Ƃ����ꍇ�̑O�t���[������̃E�B���h�E�ړ���
	//				�e�v�f�̐�Βl��dedZone�ȉ��̏ꍇ�K��0.0f�ƂȂ�
	inline Vector2 Input::Window::GetAcceleration()
	{
		Vector2 result;
	
		result.x = (MathAF::Absolute(m_windowAcceleration.x) > m_windowDedZone) ?
			m_windowAcceleration.x : 0.0f;
		result.y = (MathAF::Absolute(m_windowAcceleration.y) > m_windowDedZone) ?
			m_windowAcceleration.y : 0.0f;
		return result;
	}
	//----------------------------------------------------------------------------------
	//[GetAcceleration]
	//return: ��ʉ𑜓x�� 0.0f ~ 1.0f�Ƃ����ꍇ��dedZone�̉e�����󂯂Ȃ��O�t���[������̃E�B���h�E�ړ���
	inline Vector2 Input::Window::GetRawAcceleration()
	{
		return m_windowAcceleration;
	}



	//----------------------------------------------------------------------------------
	//Axis
	//----------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------
	//[GetAxisHorizontal]
	//���zHorizontalAxis���̎擾���s��, �擾����l��[Keybord->A (-), D (+)], [Joystick->LStick.x]
	//return: �e�擾�l�̒��ōł���Βl���傫���l
	//����1: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), ��O�����͍s��Ȃ�, default = 0
	inline float Input::Axis::GetAxisHorizontal(uint useJoystick)
	{
		return BaseGetAxisFloat(Keybord::GetKey('A'), Keybord::GetKey('D'), Joystick::GetLStick(useJoystick).x);
	}
	//----------------------------------------------------------------------------------
	//[GetRawAxisHorizontal]
	//���zHorizontalAxis���̎擾��dedZone�̉e�����󂯂��ɍs��
	//�擾����l��[Keybord->A (-), D (+)], [Joystick->RawLStick.x]
	//return: �e�擾�l�̒��ōł���Βl���傫���l
	//����1: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), ��O�����͍s��Ȃ�, default = 0
	inline float Input::Axis::GetRawAxisHorizontal(uint useJoystick)
	{
		return BaseGetAxisFloat(Keybord::GetKey('A'), Keybord::GetKey('D'), Joystick::GetRawLStick(useJoystick).x);
	}
	//----------------------------------------------------------------------------------
	//[GetAxisVertical]
	//���zVerticalAxis���̎擾���s��, �擾����l��[Keybord->W (-), S (+)], [Joystick->LStick.y]
	//return: �e�擾�l�̒��ōł���Βl���傫���l
	//����1: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), ��O�����͍s��Ȃ�, default = 0
	inline float Input::Axis::GetAxisVertical(uint useJoystick)
	{
		return BaseGetAxisFloat(Keybord::GetKey('W'), Keybord::GetKey('S'), Joystick::GetLStick(useJoystick).y);
	}
	//----------------------------------------------------------------------------------
	//[GetRawAxisVertical]
	//���zVerticalAxis���̎擾��dedZone�̉e�����󂯂��ɍs��
	//�擾����l��[Keybord->W (-), S (+)], [Joystick->RawLStick.y]
	//return: �e�擾�l�̒��ōł���Βl���傫���l
	//����1: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), ��O�����͍s��Ȃ�, default = 0
	inline float Input::Axis::GetRawAxisVertical(uint useJoystick)
	{
		return BaseGetAxisFloat(Keybord::GetKey('W'), Keybord::GetKey('S'), Joystick::GetRawLStick(useJoystick).y);
	}
	//----------------------------------------------------------------------------------
	//[GetAxisJump]
	//���zJumpAxis���̎擾���s��, �擾����l��[Keybord->Space (+)], [Joystick->A (+)]
	//return: �e�擾�l�̒��ōł���Βl���傫���l
	//����1: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), ��O�����͍s��Ȃ�, default = 0
	inline float Input::Axis::GetAxisJump(uint useJoystick)
	{
		return BaseGetAxisByte(Joystick::GetButton(JoystickCode::A), Keybord::GetKey(VK_SPACE));
	}
	//----------------------------------------------------------------------------------
	//[GetAxisAction]
	//���zActionAxis���̎擾���s��, �擾����l��[Mouse->Left (+)], [Joystick->A (+)]
	//return: �e�擾�l�̒��ōł���Βl���傫���l
	//����1: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), ��O�����͍s��Ȃ�, default = 0
	inline float Input::Axis::GetAxisAction(uint useJoystick)
	{
		return BaseGetAxisByte(Joystick::GetButton(JoystickCode::B), Mouse::GetButton(VK_LBUTTON));
	}
	//----------------------------------------------------------------------------------
	//[GetAxisUser]
	//���O��RegisterAxisUser�œo�^���ꂽ���zAxis���̎擾���s��
	//�擾����l�͓o�^����AxisPack�ɏ�����
	//return: �e�擾�l�̒��ōł���Βl���傫���l
	//throw: name�̃G�������g���o�^����Ă��Ȃ��ꍇ
	//����1: �o�^���Ɏg�p����name (map->key)
	//����2: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), ��O�����͍s��Ȃ�, default = 0
	inline float Input::Axis::GetAxisUser(const sgstring& name, uint useJoystick)
	{
		AtomicOperation::LockGuard guard(m_axisAtomic);
		
		//find
		auto it = m_axisPacks.find(name);
		
		//�v�f������
		if (it != m_axisPacks.end())
		{
			float results[5] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

			//�e�v�f�Ɍ��ʂ��i�[����
			results[0] = GetAxisPackKeyStatesPuls(it->second.keyAndMouseCodeWithPlus);
			results[1] = GetAxisPackKeyStatesMinus(it->second.keyAndMouseCodeWithMinus);
			results[2] = GetAxisPackJoystickButtomsPuls(it->second.joystickCodeWithPlus, useJoystick);
			results[3] = GetAxisPackJoystickButtomsMinus(it->second.joystickCodeWithMinus, useJoystick);
			results[4] = GetAxisPackAxisCodes(it->second.axisCodes, useJoystick);

			if (MathAF::Absolute(results[0]) == MathAF::Absolute(results[1]))
				results[0] = results[1] = 0.0f;
			if (MathAF::Absolute(results[2]) == MathAF::Absolute(results[3]))
				results[2] = results[3] = 0.0f;

			//�ő�l�����߂�
			for (int i = 1; i < 5; i++)
				results[0] = MathAF::MaxWithAbsolute(results[0], results[i]);

			//�ԋp
			return results[0];
		}
		//�Ȃ��ꍇthrow
		else
			throw InvalidArgument(L"Error!! Input->Axis->GetAxisUser", L"Name element not found");
	}
	//----------------------------------------------------------------------------------
	//[GetRawAxisUser]
	//���O��RegisterAxisUser�œo�^���ꂽ���zAxis���̎擾��dedZone�̉e�����󂯂��ɍs��
	//�擾����l�͓o�^����AxisPack�ɏ�����
	//AxisPack->axisCodes���ݒ肳��Ă��Ȃ��ꍇ, ���ʂ�GetAxisUser�Ɠ����ɂȂ�
	//return: �e�擾�l�̒��ōł���Βl���傫���l
	//throw: name�̃G�������g���o�^����Ă��Ȃ��ꍇ
	//����1: �o�^���Ɏg�p����name (map->key)
	//����2: �擾�Ɏg�p����W���C�X�e�B�b�N�̔ԍ� (0 ~ 4), ��O�����͍s��Ȃ�, default = 0
	inline float Input::Axis::GetRawAxisUser(const sgstring&  name, uint useJoystick)
	{
		AtomicOperation::LockGuard guard(m_axisAtomic);
		
		//find
		auto it = m_axisPacks.find(name);
	
		//�v�f������
		if (it != m_axisPacks.end())
		{
			float results[5] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };

			//�e�v�f�Ɍ��ʂ��i�[����
			results[0] = GetAxisPackKeyStatesPuls(it->second.keyAndMouseCodeWithPlus);
			results[1] = GetAxisPackKeyStatesMinus(it->second.keyAndMouseCodeWithMinus);
			results[2] = GetAxisPackJoystickButtomsPuls(it->second.joystickCodeWithPlus, useJoystick);
			results[3] = GetAxisPackJoystickButtomsMinus(it->second.joystickCodeWithMinus, useJoystick);
			results[4] = GetRawAxisPackAxisCodes(it->second.axisCodes, useJoystick);

			if (MathAF::Absolute(results[0]) == MathAF::Absolute(results[1]))
				results[0] = results[1] = 0.0f;
			if (MathAF::Absolute(results[2]) == MathAF::Absolute(results[3]))
				results[2] = results[3] = 0.0f;

			//�ő�l�����߂�
			for (int i = 1; i < 5; i++)
				results[0] = MathAF::MaxWithAbsolute(results[0], results[i]);

			//�ԋp
			return results[0];
		}
		//�Ȃ��ꍇthrow
		else
			throw InvalidArgument(L"Error!! Input->Axis->GetRawAxisUser", L"Name element not found");
	}
	//----------------------------------------------------------------------------------
	//[RegisterAxisUser]
	//���zAxis���̓o�^���s��, name (map->key)���܂��o�^����Ă��Ȃ����ɂ̂ݓo�^���s��
	//return: �o�^������������true, ���s�����ꍇ��false
	//����1: �g�p���Ɉ����Ƃ��Ďg����name (map->key)
	//����2: �ݒ肷��Axis��
	inline bool Input::Axis::RegisterAxisUser(const sgstring& name, const AxisPack& axisPack)
	{
		AtomicOperation::LockGuard guard(m_axisAtomic);
		
		//find
		auto it = m_axisPacks.find(name);

		//�Ȃ��ꍇ�o�^����
		if (it == m_axisPacks.end())
		{
			m_axisPacks.try_emplace(name, axisPack);
			return true;
		}
		else
			return false;
	}

	//----------------------------------------------------------------------------------
	//[BaseGetAxisFloat]
	//GetAxis��Stick����ver�ŗp����Base�֐�
	//return: GetAxis�̌���
	//����1: axis1 (+)
	//����2: axis1 (-)
	//����3: axis2
	inline float Input::Axis::BaseGetAxisFloat(byte axis1Puls, byte axis1Minus, float axis2)
	{
		return MathAF::MaxWithAbsolute(axis2, 
			(axis1Puls) ?
				(axis1Minus) ?
					0.0f : 1.0f
			: (axis1Minus) ?
				-1.0f : 0.0f);
	}
	//----------------------------------------------------------------------------------
	//[BaseGetAxisByte]
	//GetAxis��Stick�Ȃ�ver�ŗp����Base�֐�
	//return: GetAxis�̌���
	//����1: axis1 (+)
	//����2: axis2 (-)
	inline float Input::Axis::BaseGetAxisByte(ushort axis1Puls, byte axis2Puls)
	{
		if (axis1Puls || axis2Puls) return 1.0f;
		else return 0.0f;
	}
	//----------------------------------------------------------------------------------
	//[GetAxisPackKeyStatesPuls]
	//GetAxisUser��keyAndMouseCodeWithPlus��Axis���擾�Ɏg�p����
	//return: �S�Ď擾�������ʍł���Βl�̑傫���l (�Ȃ��ꍇ0.0f)
	//����1: keyAndMouseCodeWithPlus
	inline float Input::Axis::GetAxisPackKeyStatesPuls(const std::vector<int>& keyCodes)
	{
		for (const auto& e : keyCodes)
		{
			if (Keybord::GetKey(e))
				return 1.0f;
		}
		return 0.0f;
	}
	//----------------------------------------------------------------------------------
	//[GetAxisPackKeyStatesMinus]
	//GetAxisUser��keyAndMouseCodeWithMinus��Axis���擾�Ɏg�p����
	//return: �S�Ď擾�������ʍł���Βl�̑傫���l (�Ȃ��ꍇ0.0f)
	//����1: keyAndMouseCodeWithMinus
	inline float Input::Axis::GetAxisPackKeyStatesMinus(const std::vector<int>& keyCodes)
	{
		for (const auto& e : keyCodes)
		{
			if (Keybord::GetKey(e))
				return -1.0f;
		}
		return 0.0f;
	}
	//----------------------------------------------------------------------------------
	//[GetAxisPackJoystickButtomsPuls]
	//GetAxisUser��joystickCodeWithPlus��Axis���擾�Ɏg�p����
	//return: �S�Ď擾�������ʍł���Βl�̑傫���l (�Ȃ��ꍇ0.0f)
	//����1: joystickCodeWithPlus
	inline float Input::Axis::GetAxisPackJoystickButtomsPuls(const std::vector<JoystickCode::Enum>& joystickCodes, uint useStick)
	{
		for (const auto& e : joystickCodes)
		{
			if (Joystick::GetButton(e, useStick))
				return 1.0f;
		}
		return 0.0f;
	}
	//----------------------------------------------------------------------------------
	//[GetAxisPackJoystickButtomsMinus]
	//GetAxisUser��GetAxisPackJoystickButtomsMinus��Axis���擾�Ɏg�p����
	//return: �S�Ď擾�������ʍł���Βl�̑傫���l (�Ȃ��ꍇ0.0f)
	//����1: joystickCodeWithMinus
	inline float Input::Axis::GetAxisPackJoystickButtomsMinus(const std::vector<JoystickCode::Enum>& joystickCodes, uint useStick)
	{
		for (const auto& e : joystickCodes)
		{
			if (Joystick::GetButton(e, useStick))
				return -1.0f;
		}
		return 0.0f;
	}
	//----------------------------------------------------------------------------------
	//[GetAxisPackAxisCodes]
	//GetAxisUser��axisCodes��Axis���擾�Ɏg�p����
	//return: �S�Ď擾�������ʍł���Βl�̑傫���l (�Ȃ��ꍇ0.0f)
	//����1: axisCodes
	//����2: �g�p����stick
	inline float Input::Axis::GetAxisPackAxisCodes(const std::vector<AxisCode::Enum>& axisCodes, uint useStick)
	{
		//0 = return, 1 = buf 
		float result[2] = { 0.0f,0.0f };

		for (const auto& e : axisCodes)
		{
			//Joystick
			if (e & AxisCode::m_cUpperBitJoystick)
			{
				//�r�b�g���Z�Ŕ��� & �擾
				if (e & AxisCode::m_cLowerBitJoystickLStickX)
					result[1] = Joystick::GetLStick(useStick).x;
				else if (e & AxisCode::m_cLowerBitJoystickLStickY)
					result[1] = Joystick::GetLStick(useStick).y;
				else if (e & AxisCode::m_cLowerBitJoystickRStickX)
					result[1] = Joystick::GetRStick(useStick).x;
				else if (e & AxisCode::m_cLowerBitJoystickRStickY)
					result[1] = Joystick::GetRStick(useStick).y;
				else if (e & AxisCode::m_cLowerBitJoystickLTrigger)
					result[1] = Joystick::GetLTrigger(useStick);
				else if (e & AxisCode::m_cLowerBitJoystickRTrigger)
					result[1] = Joystick::GetRTrigger(useStick);
				else
					result[1] = 0.0f;
			}
			//Mouse
			else if (e & AxisCode::m_cUpperBitMouse)
			{
				//�r�b�g���Z�Ŕ��� & �擾
				if (e & AxisCode::m_cLowerBitMouseAccelerationX)
					result[1] = Mouse::GetAcceleration().x;
				else if (e & AxisCode::m_cLowerBitMouseAccelerationY)
					result[1] = Mouse::GetAcceleration().y;
				else
					result[1] = 0.0f;
			}
			//Window
			else if (e & AxisCode::m_cUpperBitWindow)
			{
				//�r�b�g���Z�Ŕ��� & �擾
				if (e & AxisCode::m_cLowerBitWindowAccelerationX)
					result[1] = Window::GetAcceleration().x;
				else if (e & AxisCode::m_cLowerBitWindowAccelerationY)
					result[1] = Window::GetAcceleration().y;
				else
					result[1] = 0.0f;
			}

			//Max
			result[0] = MathAF::MaxWithAbsolute(result[0], result[1]);
		}

		//return
		return result[0];
	}
	//----------------------------------------------------------------------------------
	//[GetRawAxisPackAxisCodes]
	//GetAxisUser��axisCodes��Axis���擾�Ɏg�p���� (Raw ver)
	//return: �S�Ď擾�������ʍł���Βl�̑傫���l (�Ȃ��ꍇ0.0f)
	//����1: axisCodes
	//����2: �g�p����stick
	inline float Input::Axis::GetRawAxisPackAxisCodes(const std::vector<AxisCode::Enum>& axisCodes, uint useStick)
	{
		//0 = return, 1 = buf 
		float result[2] = { 0.0f,0.0f };
		
		for (const auto& e : axisCodes)
		{
			//Joystick
			if (e & AxisCode::m_cUpperBitJoystick)
			{
				//�r�b�g���Z�Ŕ��� & �擾
				if (e & AxisCode::m_cLowerBitJoystickLStickX)
					result[1] = Joystick::GetRawLStick(useStick).x;
				else if (e & AxisCode::m_cLowerBitJoystickLStickY)
					result[1] = Joystick::GetRawLStick(useStick).y;
				else if (e & AxisCode::m_cLowerBitJoystickRStickX)
					result[1] = Joystick::GetRawRStick(useStick).x;
				else if (e & AxisCode::m_cLowerBitJoystickRStickY)
					result[1] = Joystick::GetRawRStick(useStick).y;
				else if (e & AxisCode::m_cLowerBitJoystickLTrigger)
					result[1] = Joystick::GetRawLTrigger(useStick);
				else if (e & AxisCode::m_cLowerBitJoystickRTrigger)
					result[1] = Joystick::GetRawRTrigger(useStick);
				else
					result[1] = 0.0f;
			}
			//Mouse
			else if (e & AxisCode::m_cUpperBitMouse)
			{
				//�r�b�g���Z�Ŕ��� & �擾
				if (e & AxisCode::m_cLowerBitMouseAccelerationX)
					result[1] = Mouse::GetRawAcceleration().x;
				else if (e & AxisCode::m_cLowerBitMouseAccelerationY)
					result[1] = Mouse::GetRawAcceleration().y;
				else
					result[1] = 0.0f;
			}
			//Window
			else if (e & AxisCode::m_cUpperBitWindow)
			{
				//�r�b�g���Z�Ŕ��� & �擾
				if (e & AxisCode::m_cLowerBitWindowAccelerationX)
					result[1] = Window::GetRawAcceleration().x;
				else if (e & AxisCode::m_cLowerBitWindowAccelerationY)
					result[1] = Window::GetRawAcceleration().y;
				else
					result[1] = 0.0f;
			}

			//Max
			result[0] = MathAF::MaxWithAbsolute(result[0], result[1]);
		}
	
		//return
		return result[0];
	}



	//----------------------------------------------------------------------------------
	//AxisPack
	//----------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------
	//[SetKeyAndMouseCodeWithPuls]
	//keyAndMouseCodeWithPlus�ɕ����v�f���܂Ƃ߂ēo�^����
	//����1: �o�^����p�����[�^�[�p�b�N (KeyCode::�Z�Z, MosueCode::�Z�Z)
	template < typename... Args >
	inline void Input::AxisPack::SetKeyAndMouseCodeWithPuls(const Args&... args)
	{
		for (const auto& e : { args... })
			keyAndMouseCodeWithPlus.emplace_back(e);
	}
	//----------------------------------------------------------------------------------
	//[SetKeyAndMouseCodeWithMinus]
	//keyAndMouseCodeWithMinus�����v�f���܂Ƃ߂ēo�^����
	//����1: �o�^����p�����[�^�[�p�b�N (KeyCode::�Z�Z, MosueCode::�Z�Z)
	template < typename... Args >
	inline void Input::AxisPack::SetKeyAndMouseCodeWithMinus(const Args&... args)
	{
		for (const auto& e : { args... })
			keyAndMouseCodeWithMinus.emplace_back(e);
	}
	//----------------------------------------------------------------------------------
	//[SetJoystickCodeWithPuls]
	//joystickCodeWithPlus�����v�f���܂Ƃ߂ēo�^����
	//����1: �o�^����p�����[�^�[�p�b�N (JoystickCode::�Z�Z)
	template < typename... Args >
	inline void Input::AxisPack::SetJoystickCodeWithPuls(const Args&... args)
	{
		for (const auto& e : { args... })
			joystickCodeWithPlus.emplace_back(e);
	}
	//----------------------------------------------------------------------------------
	//[SetJoystickCodeWithMinus]
	//joystickCodeWithMinus�����v�f���܂Ƃ߂ēo�^����
	//����1: �o�^����p�����[�^�[�p�b�N (JoystickCode::�Z�Z)
	template < typename... Args >
	inline void Input::AxisPack::SetJoystickCodeWithMinus(const Args&... args)
	{
		for (const auto& e : { args... })
			joystickCodeWithMinus.emplace_back(e);
	}
	//----------------------------------------------------------------------------------
	//[SetAxisCodes]
	//axisCodes�����v�f���܂Ƃ߂ēo�^����, �o�^����̂͊e�擾�֐��ƂȂ�
	//����1: �o�^����p�����[�^�[�p�b�N (AxisCode::�Z�Z)
	template < typename... Args >
	inline void Input::AxisPack::SetAxisCodes(const Args&... args)
	{
		for (const auto& e : { args... })
			axisCodes.emplace_back(e);
	}
}
#endif // !SGFRAMEWORK_HEADER_INPUT_HPP_