/*----------------------------------------------------------------------------------
デバッグモードか否かを保存するIsDebug class
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_DEBUG_MODE_HPP_
#define SGFRAMEWORK_HEADER_DEBUG_MODE_HPP_
#include <atomic>
#include "../Singleton/Singleton.hpp"
#include "../Property/Property.hpp"
#include "../Atomic/Atomic.hpp"

//Framework namespace
namespace SGFramework
{
	//Scene
	namespace Scene { class SceneManager; }

	//デバッグモードか否かを保存するIsDebug class
	class IsDebug
	{
	public:
		friend class Scene::SceneManager;

		//delete
		IsDebug() = delete;
		//delete
		IsDebug(const IsDebug &) = delete;
		//delete
		IsDebug& operator =(const IsDebug &) = delete;
		//delete
		IsDebug(IsDebug &&) = delete;
		//delete
		IsDebug& operator=(IsDebug &&) = delete;

		//value
		static GetOnlyProperty<std::atomic<bool>> isValue;

	private:
		//debug value<static>(set function property)
		SGF_FUNCTION_PROPERTY static void setValue(bool isValue)
		{
			AtomicOperation::Init(m_isValue, isValue);
		}
		
		static std::atomic<bool> m_isValue;				//value
	};
}

#endif // !SGFRAMEWORK_HEADER_DEBUG_MODE_HPP_