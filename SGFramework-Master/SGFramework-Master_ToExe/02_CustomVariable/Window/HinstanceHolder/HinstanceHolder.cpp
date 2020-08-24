/*----------------------------------------------------------------------------------
hInstance���Ǘ�����HinstanceHolder class
------------------------------------------------------------------------------------*/
#include "HinstanceHolder.hpp"

//Framework namespace
namespace SGFramework
{
	//Window�Ǘ�
	namespace Window
	{
		//�N���E�B���h�E�̃n���h��
		PointerProperty<HINSTANCE> HinstanceHolder::main(m_main);

		//hInstances[0]
		HINSTANCE HinstanceHolder::m_main = nullptr;
		//hInstances���X�g
		std::vector<HINSTANCE> HinstanceHolder::m_hInstances;
		//mutex
		std::mutex HinstanceHolder::m_mutex;
	}
}