/*----------------------------------------------------------------------------------
�`��R�}���h��ۑ�����DrawCommand
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_DRAW_COMMAND_HPP_
#define SGFRAMEWORK_HEADER_DRAW_COMMAND_HPP_
#include <functional>
#include "../../02_CustomVariable/CustomVariable.hpp"

// Framework namespace
namespace SGFramework
{
	//�`��R�}���h��ۑ�����DrawCommand
	struct DrawCommand
	{
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//����1: Draw�֐��I�u�W�F�N�g, �ʃX���b�h�ŕ`������s����̂ŃX���b�h�Z�[�t�ȓ��삪�K�v
		//����1: �`��D��x
		inline DrawCommand(const std::function<void()>& drawFunctionCommand, int priority)
			: drawFunctionCommand(drawFunctionCommand), priority(priority) {}

		//< operator
		inline bool operator < (const DrawCommand& right) { return (priority < right.priority); }
		//> operator
		inline bool operator > (const DrawCommand& right) { return (priority > right.priority); }

		std::function<void()> drawFunctionCommand;	//Draw�֐�
		int priority;														//�D��x
	};
}
#endif // !SGFRAMEWORK_HEADER_DRAW_COMMAND_HPP_
