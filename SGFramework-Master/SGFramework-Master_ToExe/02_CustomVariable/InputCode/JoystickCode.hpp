/*----------------------------------------------------------------------------------
�W���C�p�b�h���͂�S������Input Class���֐��ł̈����Ɏg���A
�W���C�p�b�h�擾�penum��Z�߂�JoystickCode.h
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_INPUT_JOYSTICKCODE_HPP_
#define SGFRAMEWORK_HEADER_INPUT_JOYSTICKCODE_HPP_
#include "../ConstAndUsing/ConstAndUsing.hpp"

//Framework namespace
namespace SGFramework
{
	//----------------------------------------------------------------------------------
	//�W���C�p�b�h����Ŕ���{�^���̎w����s��JoystickCode
	struct JoystickCode
	{
		enum Enum : ushort
		{
			A = 0x1000,				//A�{�^��
			B = 0x2000,				//B�{�^��
			X = 0x4000,				//X�{�^��
			Y = 0x8000,				//Y�{�^��
			LB = 0x100,				//LB�{�^��
			RB = 0x200,				//RB�{�^��
			StickLeft = 0x40,		//���X�e�B�b�N��������
			StickRight = 0x80,		//�E�X�e�B�b�N��������
			Up = 0x1,					//�\����{�^��
			Down = 0x2,				//�\�����{�^��
			Left = 0x4,				//�\�����{�^��
			Right = 0x8,				//�\���E�{�^��
			//Xbox360��p------------------------------------------------------
			Start = 0x10,				//�X�^�[�g�{�^��(Menu�{�^��)
			Back = 0x20,				//�o�b�N�{�^��(View�{�^��)
			//XboxOne��p------------------------------------------------------
			Menu = 0x10,			//���j���[�{�^��(Start�{�^��)
			View = 0x20,				//�r���[�{�^��(Back�{�^��)
		};
	};
}
#endif // !SGFRAMEWORK_HEADER_INPUT_JOYSTICKCODE_HPP_