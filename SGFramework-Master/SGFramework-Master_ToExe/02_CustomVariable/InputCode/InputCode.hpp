/*----------------------------------------------------------------------------------
入力を全て担当するInput Class内関数での引数に使う、
enumやtypedefを纏めたInputCode.h
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_INPUTCODE_INPUTCODE_HPP_
#define SGFRAMEWORK_HEADER_INPUTCODE_INPUTCODE_HPP_
#include "JoyStickCode.hpp"	//ジョイパッド用コード
#include "KeyBordCode.hpp"	//キーボード用コード
#include "MouseCode.hpp"		//マウス用コード

//Framework namespace
namespace SGFramework
{
	//friend用
	class Input;

	//----------------------------------------------------------------------------------
	//AxisPack->AxisUserに使用するAxisCode
	struct AxisCode
	{
	public:
		//friend
		friend Input;

		enum Enum : short
		{
			MouseAccelerationX = 0x101,		//マウス加速度X
			MouseAccelerationY = 0x102,		//マウス加速度Y
			JoystickLTrigger = 0x201,			//ジョイスティックトリガーL
			JoystickRTrigger = 0x202,			//ジョイスティックトリガーR
			JoystickLStickX = 0x204,				//ジョイスティックLスティックX
			JoystickLStickY = 0x208,				//ジョイスティックLスティックY
			JoystickRStickX = 0x210,			//ジョイスティックRスティックX
			JoystickRStickY = 0x220,			//ジョイスティックRスティックY
			WindowAccelerationX = 0x401,	//ウィンドウ移動X
			WindowAccelerationY = 0x402,	//ウィンドウ移動Y
		};

	private:
		static constexpr ValueShort m_cUpperBitMouse = 0x0100;					//マウス
		static constexpr ValueShort m_cUpperBitJoystick = 0x0200;					//ジョイスティック
		static constexpr ValueShort m_cUpperBitWindow = 0x0400;					//ウィンドウ
		static constexpr ValueShort m_cLowerBitMouseAccelerationX = 0x01;	//マウス加速度X
		static constexpr ValueShort m_cLowerBitMouseAccelerationY = 0x02;	//マウス加速度Y
		static constexpr ValueShort m_cLowerBitJoystickLTrigger = 0x01;			//ジョイスティックトリガーL
		static constexpr ValueShort m_cLowerBitJoystickRTrigger = 0x02;			//ジョイスティックトリガーR
		static constexpr ValueShort m_cLowerBitJoystickLStickX = 0x04;			//ジョイスティックLスティックX
		static constexpr ValueShort m_cLowerBitJoystickLStickY = 0x08;			//ジョイスティックLスティックY
		static constexpr ValueShort m_cLowerBitJoystickRStickX = 0x10;			//ジョイスティックRスティックX
		static constexpr ValueShort m_cLowerBitJoystickRStickY = 0x20;			//ジョイスティックRスティックY
		static constexpr ValueShort m_cLowerBitWindowAccelerationX = 0x01;	//ウィンドウ移動X
		static constexpr ValueShort m_cLowerBitWindowAccelerationY = 0x32;	//ウィンドウ移動Y
	};
}
#endif // !SGFRAMEWORK_HEADER_INPUTCODE_INPUTCODE_HPP_