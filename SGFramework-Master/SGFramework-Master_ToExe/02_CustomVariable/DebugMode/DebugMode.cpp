/*----------------------------------------------------------------------------------
�f�o�b�O���[�h���ۂ���ۑ�����IsDebug class
------------------------------------------------------------------------------------*/
#include "DebugMode.hpp"

//Framework namespace
namespace SGFramework
{
	GetOnlyProperty<std::atomic<bool>> IsDebug::isValue = m_isValue;
	std::atomic<bool> IsDebug::m_isValue = true;	//value
}