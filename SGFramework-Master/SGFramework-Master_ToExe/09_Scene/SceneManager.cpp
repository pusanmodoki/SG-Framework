#include "SceneManager.hpp"


//Framework namespace
namespace SGFramework
{
	namespace Scene
	{
		std::stack<UniquePointer<BaseScene>, std::vector<UniquePointer<BaseScene>>> SceneManager::m_scenes;
		std::unordered_map<std::wstring, Detail::Pointer::BaseCountPointer*> SceneManager::m_unloadSharedPointers;
		std::mutex SceneManager::m_mutex;
		std::atomic<ushort> SceneManager::m_unloadCounter = 0;
		bool SceneManager::m_isEndUpdate = false;
		bool SceneManager::m_isInstantiate = false;
	}
}