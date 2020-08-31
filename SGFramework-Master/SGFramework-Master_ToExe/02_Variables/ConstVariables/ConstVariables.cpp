/*----------------------------------------------------------------------------------
<ConstVariables.cpp> contents->02_Variable内のconst static変数
制作者: Shota Uemura
------------------------------------------------------------------------------------*/
#include "../Color/Color.hpp"

//Framework namespace
namespace SGFramework 
{
	//Color members
	const Color Color::black(0.0f, 0.0f, 0.0f, 1.0f);			//black (0.0f, 0.0f, 0.0f, 1.0f)
	const Color Color::blue(0.0f, 0.0f, 1.0f, 1.0f);			//blue (0.0f, 0.0f, 1.0f, 1.0f)
	const Color Color::clear(0.0f, 0.0f, 0.0f, 0.0f);			//clear (0.0f, 0.0f, 0.0f, 0.0f)
	const Color Color::cyan(0.0f, 1.0f, 1.0f, 1.0f);			//cyan (0.0f, 1.0f, 1.0f, 1.0f)
	const Color Color::gray(0.5f, 0.5f, 0.5f, 1.0f);			//gray (0.5f, 0.5f, 0.5f, 1.0f)
	const Color Color::green(0.0f, 1.0f, 0.0f, 1.0f);			//green (0.0f, 1.0f, 0.0f, 1.0f)
	const Color Color::magenta(1.0f, 0.0f, 1.0f, 1.0f);	//magenta (1.0f, 0.0f, 1.0f, 1.0f)
	const Color Color::red(1.0f, 0.0f, 0.0f, 1.0f);			//red (1.0f, 0.0f, 0.0f, 1.0f)
	const Color Color::white(1.0f, 1.0f, 1.0f, 1.0f);			//white (1.0f, 1.0f, 1.0f, 1.0f)
	const Color Color::yellow(1.0f, 1.0f, 0.0f, 1.0f);		//yellow (1.0f, 1.0f, 0.0f, 1.0f)
}