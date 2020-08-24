/*----------------------------------------------------------------------------------
�L�[�{�[�h���͂�S������KeyBordInput Class���֐��ł̈����Ɏg���A
�L�[�{�[�h�擾�penum��Z�߂�KeyBordCode.h
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_INPUTCODE_KEYCODE_HPP_
#define SGFRAMEWORK_HEADER_INPUTCODE_KEYCODE_HPP_

#if defined(SGF_PLATFORM_WINDOWS)
#include <Windows.h>
#endif //SGF_PLATFORM_WINDOWS

#include "../MacroAndUsing/MacroAndUsing.hpp"

//Framework namespace
namespace SGFramework
{
	//----------------------------------------------------------------------------------
	//�L�[�{�[�h����Ŕ���L�[�̎w��Ɏg��KeyCode	  
	struct KeyCode
	{
		enum Enum : int
		{
			//Windows key list
#if defined(SGF_PLATFORM_WINDOWS)
			//�V�X�e���L�[ ---------------------------------------
			Shiht = VK_SHIFT,						//Shift
			RightShiht = VK_RSHIFT,				//�EShift
			LeftShift = VK_LSHIFT,				//��Shift
			Alt = VK_MENU,							//Alt
			LeftAlt = VK_LMENU,					//��Alt
			RightAlt = VK_RMENU,					//�EAlt
			Control = VK_CONTROL,				//Ctrl
			RightControl = VK_RCONTROL,		//�ECtrl
			LeftControl = VK_LCONTROL,		//��ctrl
			LeftWin = VK_LWIN,					//��Windows
			RightWin = VK_RWIN,					//�EWindows
			Up = VK_UP,								//��
			Left = VK_LEFT,							//��
			Right = VK_RIGHT,						//��
			Down = VK_DOWN,						//��
			PageUp = VK_PRIOR,					//PageUp
			PageDown = VK_NEXT,				//PageDown
			End = VK_END,							//End
			Home = VK_HOME,						//Home
			Escape = VK_ESCAPE,					//�G�X�P�[�v�L�[(ESC)
			BackSpace = VK_BACK,				//�o�b�N�X�y�[�X
			Tab = VK_TAB,							//�^�u
			Enter = VK_RETURN,					//�G���^�[
			Space = VK_SPACE,						//�X�y�[�X				
			Delete = VK_DELETE,					//Delete
			Pause = VK_PAUSE,						//Pause
			CapsLock = VK_CAPITAL,				//CapsLock
			Sleep = VK_SLEEP,						//Sleep
			SnapShot = VK_SNAPSHOT,			//SnapShot
			Kana = VK_KANA,						//����
			Kanji = VK_KANJI,						//����
			Convert = VK_CONVERT,				//�ϊ�
			NoConvert = VK_NONCONVERT,	//���ϊ�
			Insert = VK_INSERT,					//Insert
			Help = VK_HELP,							//Help
			Plus = VK_OEM_PLUS,					//+
			Minus = VK_OEM_MINUS,				//-
			Comma = VK_OEM_COMMA,		//,
			Period = VK_OEM_PERIOD,			//.
			Oem1 = VK_OEM_1,					//�č��L�[�{�[�h�̏ꍇ,:�L�[
			Oem2 = VK_OEM_2,					//�č��L�[�{�[�h�̏ꍇ/?�L�[
			Oem3 = VK_OEM_3,					//�č��L�[�{�[�h�̏ꍇ'~�L�[
			Oem4 = VK_OEM_4,					//�č��L�[�{�[�h�̏ꍇ[{�L�[
			Oem5 = VK_OEM_5,					//�č��L�[�{�[�h�̏ꍇ/|�L�[
			Oem6 = VK_OEM_6,					//�č��L�[�{�[�h�̏ꍇ]}�L�[
			Oem7 = VK_OEM_7,					//�č��L�[�{�[�h�̏ꍇ���p���L�[
			Oem8 = VK_OEM_8,					//oem

			//�����L�[ ---------------------------------------------
			Num1 = '1',
			Num2 = '2',
			Num3 = '3',
			Num4 = '4',
			Num5 = '5',
			Num6 = '6',
			Num7 = '7',
			Num8 = '8',
			Num9 = '9',
			Num0 = '0',

			//�A���t�@�x�b�g�L�[-------------------------------------------
			A = 'A',
			B = 'B',
			C = 'C',
			D = 'D',
			E = 'E',
			F = 'F',
			G = 'G',
			H = 'H',
			I = 'I',
			J = 'J',
			K = 'K',
			L = 'L',
			M = 'M',
			N = 'N',
			O = 'O',
			P = 'P',
			Q = 'Q',
			R = 'R',
			S = 'S',
			T = 'T',
			U = 'U',
			V = 'V',
			W = 'W',
			X = 'X',
			Y = 'Y',
			Z = 'Z',

			//�t�@���N�V�����L�[-------------------------------------------
			F1 = VK_F1,
			F2 = VK_F2,
			F3 = VK_F3,
			F4 = VK_F4,
			F5 = VK_F5,
			F6 = VK_F6,
			F7 = VK_F7,
			F8 = VK_F8,
			F9 = VK_F9,
			F10 = VK_F10,
			F11 = VK_F11,
			F12 = VK_F12,
#endif //SGF_PLATFORM_WINDOWS
		};
	};
}
#endif // !SGFRAMEWORK_HEADER_INPUTCODE_KEYCODE_HPP_
