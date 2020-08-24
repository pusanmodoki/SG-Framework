/*----------------------------------------------------------------------------------
�}�E�X���͂�S������MouseInput Class���֐��ł̈����Ɏg���A
�}�E�X�擾�penum��Z�߂�MouseCode.h
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_INPUTCODE_MOUSECODE_HPP_
#define SGFRAMEWORK_HEADER_INPUTCODE_MOUSECODE_HPP_

#if defined(SGF_PLATFORM_WINDOWS)
#include <Windows.h>
#endif //SGF_PLATFORM_WINDOWS

#include "../MacroAndUsing/MacroAndUsing.hpp"

//Framework namespace
namespace SGFramework
{
	//----------------------------------------------------------------------------------
	//�}�E�X����Ń{�^���̎w����s��MouseCode
	struct MouseCode
	{
		enum Enum : int
		{
			//Windows button list
#if defined(SGF_PLATFORM_WINDOWS)
			Left = VK_LBUTTON,			//���{�^��
			Right = VK_RBUTTON,		//�E�{�^��
			Center = VK_MBUTTON,		//�����{�^��
			Button4 = VK_XBUTTON1,	//�{�^��4
			Button5 = VK_XBUTTON2,	//�{�^��5
#endif//SGF_PLATFORM_WINDOWS
		};
	};
}
#endif // !SGFRAMEWORK_HEADER_INPUTCODE_MOUSECODE_HPP_
