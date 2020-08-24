/*----------------------------------------------------------------------------------
�����_���Ȏ����𐶐����� Random Class
------------------------------------------------------------------------------------*/	
#ifndef SGFRAMEWORK_HEADER_RANDOM_HPP_
#define SGFRAMEWORK_HEADER_RANDOM_HPP_
#include <random>
#include <functional>
#include <map>
#include <thread>
#include <unordered_map>
#include <mutex>
#include <Windows.h>

//Framework namespace
namespace SGFramework
{
	//friend�p
	class Thread;
	class Application;

	//�����_���Ȏ����𐶐����� Random Class
	class Random final
	{
	public:
		//friend
		friend class Thread;
		friend class Application;
		friend int WINAPI ::WinMain(_In_ HINSTANCE hInstance, 
			_In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow);

		//----------------------------------------------------------------------------------
		//[Value]
		//return: 0.0f ~ 1.0f�̃����_����float�l
		inline static float Value() { return m_valueFloats.at(std::this_thread::get_id())(); }
		//----------------------------------------------------------------------------------
		//[ValueToInt]
		//return: 0 ~ 100�̃����_����int�l
		inline static int ValueToInt() { return m_valueInts.at(std::this_thread::get_id())(); };
		//----------------------------------------------------------------------------------
		//[Range]
		//return: min(�܂�) ~ max(�܂�)�̃����_����float�l 
		//����1: �ŏ��l(�܂�)
		//����2: �ő�l(�܂�)
		inline static float Range(float min, float max)
		{
			std::uniform_real_distribution<float> distribution(min, max);
			return distribution(m_randoms.at(std::this_thread::get_id()));
		}
		//----------------------------------------------------------------------------------
		//[RangeToInt]
		//return: min(�܂�) ~ max(�܂�)�̃����_����int�l 
		//����1: �ŏ��l(�܂�)
		//����2: �ő�l(�܂�)
		inline static int RangeToInt(int min, int max)
		{
			std::uniform_int_distribution<int> distribution(min, max);
			return distribution(m_randoms.at(std::this_thread::get_id()));
		}

	private:
		//----------------------------------------------------------------------------------
		//[InitMainThread]
		//���C���X���b�h�p, �����_�����X�g��ǉ�
		inline static void InitMainThread()
		{
			std::lock_guard<std::mutex> guard(m_lock);

			auto id = std::this_thread::get_id();
			m_randoms.try_emplace(id, std::random_device{}());
			m_distributionFloats.try_emplace(id, 0.0f, 1.0f);
			m_distributionInts.try_emplace(id, 0, 100);
			m_valueFloats.try_emplace(id, std::bind(m_distributionFloats.at(id), std::ref(m_randoms.at(id))));
			m_valueInts.try_emplace(id, std::bind(m_distributionInts.at(id), std::ref(m_randoms.at(id))));
		}
		//----------------------------------------------------------------------------------
		//[InitMainThread]
		//�蓮�p, �����_�����X�g��ǉ�
		//����1: �X���b�h
		inline static void InitThread(const std::thread& thread)
		{
			std::lock_guard<std::mutex> guard(m_lock);

			auto id = thread.get_id();
			m_randoms.try_emplace(id, std::random_device{}());
			m_distributionFloats.try_emplace(id, 0.0f, 1.0f);
			m_distributionInts.try_emplace(id, 0, 100);
			m_valueFloats.try_emplace(id, std::bind(m_distributionFloats.at(id), std::ref(m_randoms.at(id))));
			m_valueInts.try_emplace(id, std::bind(m_distributionInts.at(id), std::ref(m_randoms.at(id))));
		}
		//----------------------------------------------------------------------------------
		//[InitThreads]
		//�T�u�p, �����_�����X�g��ǉ�
		//����1: �X���b�h
		inline static void InitThreads(const std::vector<std::thread>& threads)
		{
			std::lock_guard<std::mutex> guard(m_lock);

			for (const auto& e : threads)
			{
				auto id = e.get_id();
				m_randoms.try_emplace(id, std::random_device{}());
				m_distributionFloats.try_emplace(id, 0.0f, 1.0f);
				m_distributionInts.try_emplace(id, 0, 100);
				m_valueFloats.try_emplace(id, std::bind(m_distributionFloats.at(id), std::ref(m_randoms.at(id))));
				m_valueInts.try_emplace(id, std::bind(m_distributionInts.at(id), std::ref(m_randoms.at(id))));
			}
		}

		//����
		static std::unordered_map<std::thread::id, std::mt19937> m_randoms;
		//float��l���z	
		static std::unordered_map<std::thread::id, std::uniform_real_distribution<float>> m_distributionFloats;
		//int��l���z	
		static std::unordered_map<std::thread::id, std::uniform_int_distribution<int>> m_distributionInts;
		//floatBind
		static std::unordered_map<std::thread::id, std::_Binder<std::_Unforced, std::uniform_real_distribution<float>&, std::reference_wrapper<std::mt19937>>> m_valueFloats;
		//intBind
		static std::unordered_map<std::thread::id, std::_Binder<std::_Unforced, std::uniform_int_distribution<int>&, std::reference_wrapper<std::mt19937>>> m_valueInts;
		//lock
		static std::mutex m_lock;
	};
}
#endif // !SGFRAMEWORK_HEADER_RANDOM_HPP_
