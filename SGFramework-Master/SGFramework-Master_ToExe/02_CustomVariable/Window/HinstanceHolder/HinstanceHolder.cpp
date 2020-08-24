/*----------------------------------------------------------------------------------
hInstanceを管理するHinstanceHolder class
------------------------------------------------------------------------------------*/
#include "HinstanceHolder.hpp"

//Framework namespace
namespace SGFramework
{
	//Window管理
	namespace Window
	{
		//起動ウィンドウのハンドル
		PointerProperty<HINSTANCE> HinstanceHolder::main(m_main);

		//hInstances[0]
		HINSTANCE HinstanceHolder::m_main = nullptr;
		//hInstancesリスト
		std::vector<HINSTANCE> HinstanceHolder::m_hInstances;
		//mutex
		std::mutex HinstanceHolder::m_mutex;
	}
}