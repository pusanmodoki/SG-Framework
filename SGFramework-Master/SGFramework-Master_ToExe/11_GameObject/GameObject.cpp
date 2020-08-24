#include "GameObject.hpp"


//Framework namespace
namespace SGFramework
{
	std::unordered_map<uint, GameObject*> GameObject::m_findObjects;
	std::vector<SharedPointer<GameObject>> GameObject::m_destroyObjects;
	std::atomic_bool GameObject::m_staticLock = false;
}