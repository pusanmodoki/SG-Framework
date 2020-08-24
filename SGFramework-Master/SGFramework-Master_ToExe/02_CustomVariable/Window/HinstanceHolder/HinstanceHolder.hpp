/*----------------------------------------------------------------------------------
hInstanceを管理するHinstanceHolder class
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_HINSTANCE_HOLDER_HPP_
#define SGFRAMEWORK_HEADER_HINSTANCE_HOLDER_HPP_
#include <Windows.h>
#include <vector>
#include <mutex>
#include "../../ConstAndUsing/ConstAndUsing.hpp"
#include "../../Property/Property.hpp"
#include "../../VariableLimit/VariableLimit.hpp"

//Framework namespace
namespace SGFramework
{
	//Window管理
	namespace Window
	{
		//hInstanceを管理するHinstanceHolder class
		class HinstanceHolder final
		{
		public:
			//----------------------------------------------------------------------------------
			//[AddHinstance]
			//ウィンドウハンドルを登録
			//return: i, error = VariableLimit::cUintMax
			//引数1: ウィンドウハンドル
			inline static uint AddHinstance(const HINSTANCE hInstance)
			{
				if (hInstance == nullptr) return VariableLimit::cUintMax;
				if (m_hInstances.size() == 0) m_main = hInstance;

				m_mutex.lock();
				m_hInstances.emplace_back(hInstance);
				m_mutex.unlock();

				return static_cast<uint>(m_hInstances.size() - 1);
			}
			//----------------------------------------------------------------------------------
			//[Hinstances]
			//return: 登録リスト.size > iの場合登録リスト[i]を返却, ないならnull
			//引数1: i
			inline static HINSTANCE Hinstances(const uint i)
			{
				std::lock_guard<std::mutex> lock(m_mutex);
				if (i < (uint)m_hInstances.size()) return m_hInstances[i];
				else return nullptr;
			}

			//起動ウィンドウのハンドル
			static PointerProperty<HINSTANCE> main;

		private:
			//hInstances[0]
			static HINSTANCE m_main;
			//hInstancesリスト
			static std::vector<HINSTANCE> m_hInstances;
			//mutex
			static std::mutex m_mutex;
		};
	}
}
#endif // !SGFRAMEWORK_HEADER_HINSTANCE_HOLDER_HPP_
