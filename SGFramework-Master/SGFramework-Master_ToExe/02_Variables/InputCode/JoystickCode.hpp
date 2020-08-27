/*----------------------------------------------------------------------------------
ジョイパッド入力を担当するInput Class内関数での引数に使う、
ジョイパッド取得用enumを纏めたJoystickCode.h
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_INPUT_JOYSTICKCODE_HPP_
#define SGFRAMEWORK_HEADER_INPUT_JOYSTICKCODE_HPP_
#include "../MacroAndUsing/MacroAndUsing.hpp"

//Framework namespace
namespace SGFramework
{
	//----------------------------------------------------------------------------------
	//ジョイパッド判定で判定ボタンの指定を行うJoystickCode
	struct JoystickCode
	{
		enum Enum : ushort
		{
			//Windows(XInput) key list
#if defined(SGF_PLATFORM_WINDOWS)
			A = 0x1000,				//Aボタン
			B = 0x2000,				//Bボタン
			X = 0x4000,				//Xボタン
			Y = 0x8000,				//Yボタン
			LB = 0x100,				//LBボタン
			RB = 0x200,				//RBボタン
			StickLeft = 0x40,		//左スティック押し込み
			StickRight = 0x80,		//右スティック押し込み
			Up = 0x1,					//十字上ボタン
			Down = 0x2,				//十字下ボタン
			Left = 0x4,				//十字左ボタン
			Right = 0x8,				//十字右ボタン
			//Xbox360専用------------------------------------------------------
			Start = 0x10,				//スタートボタン(Menuボタン)
			Back = 0x20,				//バックボタン(Viewボタン)
			//XboxOne専用------------------------------------------------------
			Menu = 0x10,			//メニューボタン(Startボタン)
			View = 0x20,				//ビューボタン(Backボタン)
#endif
		};
	};
}
#endif // !SGFRAMEWORK_HEADER_INPUT_JOYSTICKCODE_HPP_