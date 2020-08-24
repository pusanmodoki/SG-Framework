/*----------------------------------------------------------------------------------
���͂�S�ĒS������Input Class���֐��ł̈����Ɏg���A
enum��typedef��Z�߂�InputCode.h
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_INPUTCODE_INPUTCODE_HPP_
#define SGFRAMEWORK_HEADER_INPUTCODE_INPUTCODE_HPP_
#include "JoyStickCode.hpp"	//�W���C�p�b�h�p�R�[�h
#include "KeyBordCode.hpp"	//�L�[�{�[�h�p�R�[�h
#include "MouseCode.hpp"		//�}�E�X�p�R�[�h

//Framework namespace
namespace SGFramework
{
	//friend�p
	class Input;

	//----------------------------------------------------------------------------------
	//AxisPack->AxisUser�Ɏg�p����AxisCode
	struct AxisCode
	{
	public:
		//friend
		friend Input;

		enum Enum : short
		{
			MouseAccelerationX = 0x101,		//�}�E�X�����xX
			MouseAccelerationY = 0x102,		//�}�E�X�����xY
			JoystickLTrigger = 0x201,			//�W���C�X�e�B�b�N�g���K�[L
			JoystickRTrigger = 0x202,			//�W���C�X�e�B�b�N�g���K�[R
			JoystickLStickX = 0x204,				//�W���C�X�e�B�b�NL�X�e�B�b�NX
			JoystickLStickY = 0x208,				//�W���C�X�e�B�b�NL�X�e�B�b�NY
			JoystickRStickX = 0x210,			//�W���C�X�e�B�b�NR�X�e�B�b�NX
			JoystickRStickY = 0x220,			//�W���C�X�e�B�b�NR�X�e�B�b�NY
			WindowAccelerationX = 0x401,	//�E�B���h�E�ړ�X
			WindowAccelerationY = 0x402,	//�E�B���h�E�ړ�Y
		};

	private:
		static constexpr short m_cUpperBitMouse = 0x0100;					//�}�E�X
		static constexpr short m_cUpperBitJoystick = 0x0200;					//�W���C�X�e�B�b�N
		static constexpr short m_cUpperBitWindow = 0x0400;					//�E�B���h�E
		static constexpr short m_cLowerBitMouseAccelerationX = 0x01;	//�}�E�X�����xX
		static constexpr short m_cLowerBitMouseAccelerationY = 0x02;	//�}�E�X�����xY
		static constexpr short m_cLowerBitJoystickLTrigger = 0x01;			//�W���C�X�e�B�b�N�g���K�[L
		static constexpr short m_cLowerBitJoystickRTrigger = 0x02;			//�W���C�X�e�B�b�N�g���K�[R
		static constexpr short m_cLowerBitJoystickLStickX = 0x04;			//�W���C�X�e�B�b�NL�X�e�B�b�NX
		static constexpr short m_cLowerBitJoystickLStickY = 0x08;			//�W���C�X�e�B�b�NL�X�e�B�b�NY
		static constexpr short m_cLowerBitJoystickRStickX = 0x10;			//�W���C�X�e�B�b�NR�X�e�B�b�NX
		static constexpr short m_cLowerBitJoystickRStickY = 0x20;			//�W���C�X�e�B�b�NR�X�e�B�b�NY
		static constexpr short m_cLowerBitWindowAccelerationX = 0x01;	//�E�B���h�E�ړ�X
		static constexpr short m_cLowerBitWindowAccelerationY = 0x32;	//�E�B���h�E�ړ�Y
	};
}
#endif // !SGFRAMEWORK_HEADER_INPUTCODE_INPUTCODE_HPP_