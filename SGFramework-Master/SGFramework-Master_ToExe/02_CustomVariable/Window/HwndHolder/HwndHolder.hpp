/*----------------------------------------------------------------------------------
hWnd���Ǘ�����HwndHolder class
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_HWND_HOLDER_HPP_
#define SGFRAMEWORK_HEADER_HWND_HOLDER_HPP_
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
		//hWnd���Ǘ�����HwndHolder class
		class HwndHolder final
		{
		public:
			//----------------------------------------------------------------------------------
			//[AddHwnd]
			//�E�B���h�E�n���h����o�^
			//return: i, error = VariableLimit::cUintMax
			//����1: �E�B���h�E�n���h��
			inline static uint AddHwnd(const HWND hWnd) 
			{ 
				if (hWnd == nullptr) return VariableLimit::cUintMax; 
				if (m_hwnds.size() == 0) m_main = hWnd;
			
				m_mutex.lock();
				m_hwnds.emplace_back(hWnd);
				m_mutex.unlock();

				return static_cast<uint>(m_hwnds.size() - 1);
			}
			//----------------------------------------------------------------------------------
			//[Hwnds]
			//return: �o�^���X�g.size > i�̏ꍇ�o�^���X�g[i]��ԋp, �Ȃ��Ȃ�null
			//����1: i
			inline static HWND Hwnds(const uint i) 
			{ 
				std::lock_guard<std::mutex> lock(m_mutex);
				if (i < (uint)m_hwnds.size()) return m_hwnds[i]; 
				else return nullptr; 
			}

			//�N���E�B���h�E�̃n���h��
			static PointerProperty<HWND> main;

		private:
			//hWnds[0]
			static HWND m_main;
			//hWnd���X�g
			static std::vector<HWND> m_hwnds;
			//mutex
			static std::mutex m_mutex;
		};
	}
}
#endif // !SGFRAMEWORK_HEADER_HWND_HOLDER_HPP_
