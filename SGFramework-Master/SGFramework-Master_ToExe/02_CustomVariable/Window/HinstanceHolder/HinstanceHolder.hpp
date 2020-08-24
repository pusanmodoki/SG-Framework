/*----------------------------------------------------------------------------------
hInstance���Ǘ�����HinstanceHolder class
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
	//Window�Ǘ�
	namespace Window
	{
		//hInstance���Ǘ�����HinstanceHolder class
		class HinstanceHolder final
		{
		public:
			//----------------------------------------------------------------------------------
			//[AddHinstance]
			//�E�B���h�E�n���h����o�^
			//return: i, error = VariableLimit::cUintMax
			//����1: �E�B���h�E�n���h��
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
			//return: �o�^���X�g.size > i�̏ꍇ�o�^���X�g[i]��ԋp, �Ȃ��Ȃ�null
			//����1: i
			inline static HINSTANCE Hinstances(const uint i)
			{
				std::lock_guard<std::mutex> lock(m_mutex);
				if (i < (uint)m_hInstances.size()) return m_hInstances[i];
				else return nullptr;
			}

			//�N���E�B���h�E�̃n���h��
			static PointerProperty<HINSTANCE> main;

		private:
			//hInstances[0]
			static HINSTANCE m_main;
			//hInstances���X�g
			static std::vector<HINSTANCE> m_hInstances;
			//mutex
			static std::mutex m_mutex;
		};
	}
}
#endif // !SGFRAMEWORK_HEADER_HINSTANCE_HOLDER_HPP_
