/*----------------------------------------------------------------------------------
キーボード入力を担当するKeyBordInput Class内関数での引数に使う、
キーボード取得用enumを纏めたKeyBordCode.h
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_INPUTCODE_KEYCODE_HPP_
#define SGFRAMEWORK_HEADER_INPUTCODE_KEYCODE_HPP_
#include <Windows.h>
#include "../ConstAndUsing/ConstAndUsing.hpp"

//Framework namespace
namespace SGFramework
{
	//----------------------------------------------------------------------------------
	//キーボード判定で判定キーの指定に使うKeyCode	  
	struct KeyCode
	{
		enum Enum : int
		{
			//システムキー ---------------------------------------
			Shiht = VK_SHIFT,						//Shift
			RightShiht = VK_RSHIFT,				//右Shift
			LeftShift = VK_LSHIFT,				//左Shift
			Alt = VK_MENU,							//Alt
			LeftAlt = VK_LMENU,					//左Alt
			RightAlt = VK_RMENU,					//右Alt
			Control = VK_CONTROL,				//Ctrl
			RightControl = VK_RCONTROL,		//右Ctrl
			LeftControl = VK_LCONTROL,		//左ctrl
			LeftWin = VK_LWIN,					//左Windows
			RightWin = VK_RWIN,					//右Windows
			Up = VK_UP,								//↑
			Left = VK_LEFT,							//←
			Right = VK_RIGHT,						//→
			Down = VK_DOWN,						//↓
			PageUp = VK_PRIOR,					//PageUp
			PageDown = VK_NEXT,				//PageDown
			End = VK_END,							//End
			Home = VK_HOME,						//Home
			Escape = VK_ESCAPE,					//エスケープキー(ESC)
			BackSpace = VK_BACK,				//バックスペース
			Tab = VK_TAB,							//タブ
			Enter = VK_RETURN,					//エンター
			Space = VK_SPACE,						//スペース				
			Delete = VK_DELETE,					//Delete
			Pause = VK_PAUSE,						//Pause
			CapsLock = VK_CAPITAL,				//CapsLock
			Sleep = VK_SLEEP,						//Sleep
			SnapShot = VK_SNAPSHOT,			//SnapShot
			Kana = VK_KANA,						//かな
			Kanji = VK_KANJI,						//漢字
			Convert = VK_CONVERT,				//変換
			NoConvert = VK_NONCONVERT,	//無変換
			Insert = VK_INSERT,					//Insert
			Help = VK_HELP,							//Help
			Plus = VK_OEM_PLUS,					//+
			Minus = VK_OEM_MINUS,				//-
			Comma = VK_OEM_COMMA,		//,
			Period = VK_OEM_PERIOD,			//.
			Oem1 = VK_OEM_1,					//米国キーボードの場合,:キー
			Oem2 = VK_OEM_2,					//米国キーボードの場合/?キー
			Oem3 = VK_OEM_3,					//米国キーボードの場合'~キー
			Oem4 = VK_OEM_4,					//米国キーボードの場合[{キー
			Oem5 = VK_OEM_5,					//米国キーボードの場合/|キー
			Oem6 = VK_OEM_6,					//米国キーボードの場合]}キー
			Oem7 = VK_OEM_7,					//米国キーボードの場合引用符キー
			Oem8 = VK_OEM_8,					//oem

			//数字キー ---------------------------------------------
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

			//アルファベットキー-------------------------------------------
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

			//ファンクションキー-------------------------------------------
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
		};
	};
}
#endif // !SGFRAMEWORK_HEADER_INPUTCODE_KEYCODE_HPP_
