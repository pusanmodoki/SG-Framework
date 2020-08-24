#include "TransformX11.hpp"


//Framework namespace
namespace SGFramework
{
	//friend用
	class Thread;

	//フレームワークの管理をするAdministrator namespace
	namespace Administrator
	{
		//GraphicsDetail namespace
		namespace GraphicsDetail
		{
			TransformX11::RootList TransformX11::m_rootList;
			std::unordered_map<uint, void (ComponentX11::*)()> ComponentX11::m_functionTable;
			std::unordered_map<uint, void (ComponentX11::*)(const ContactInfo&)> ComponentX11::m_physicsFunctionTable;
			std::unordered_map<uint, void (ComponentX11::*)(WeakPointer<TransformX11>, WeakPointer<TransformX11>)> ComponentX11::m_pointerFunctionTable;
		
			std::thread::id TransformX11::LockGuardTransform::m_cDefauldID = std::thread::id();
			std::atomic<bool> TransformX11::LockGuardTransform::m_isNowSyncFrame = false;
		}
	}
}