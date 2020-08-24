/*----------------------------------------------------------------------------------
描画コマンドを保存するDrawCommand
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_DRAW_COMMAND_HPP_
#define SGFRAMEWORK_HEADER_DRAW_COMMAND_HPP_
#include <functional>
#include "../../02_CustomVariable/CustomVariable.hpp"

// Framework namespace
namespace SGFramework
{
	//描画コマンドを保存するDrawCommand
	struct DrawCommand
	{
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//引数1: Draw関数オブジェクト, 別スレッドで描画を実行するのでスレッドセーフな動作が必要
		//引数1: 描画優先度
		inline DrawCommand(const std::function<void()>& drawFunctionCommand, int priority)
			: drawFunctionCommand(drawFunctionCommand), priority(priority) {}

		//< operator
		inline bool operator < (const DrawCommand& right) { return (priority < right.priority); }
		//> operator
		inline bool operator > (const DrawCommand& right) { return (priority > right.priority); }

		std::function<void()> drawFunctionCommand;	//Draw関数
		int priority;														//優先度
	};
}
#endif // !SGFRAMEWORK_HEADER_DRAW_COMMAND_HPP_
