/*----------------------------------------------------------------------------------
マウス入力を担当するMouseInput Class内関数での引数に使う、
マウス取得用enumを纏めたMouseCode.h
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_INPUTCODE_MOUSECODE_HPP_
#define SGFRAMEWORK_HEADER_INPUTCODE_MOUSECODE_HPP_
#include <Windows.h>
#include "../ConstAndUsing/ConstAndUsing.hpp"

//Framework namespace
namespace SGFramework
{
	//----------------------------------------------------------------------------------
	//マウス判定でボタンの指定を行うMouseCode
	struct MouseCode
	{
		enum Enum : int
		{
			Left = VK_LBUTTON,			//左ボタン
			Right = VK_RBUTTON,		//右ボタン
			Center = VK_MBUTTON,		//中央ボタン
			Button4 = VK_XBUTTON1,	//ボタン4
			Button5 = VK_XBUTTON2,	//ボタン5
		};
	};
}
#endif // !SGFRAMEWORK_HEADER_INPUTCODE_MOUSECODE_HPP_
