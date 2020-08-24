/*----------------------------------------------------------------------------------
ランダムな実数を生成する Random Class
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
	//friend用
	class Thread;
	class Application;

	//ランダムな実数を生成する Random Class
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
		//return: 0.0f ~ 1.0fのランダムなfloat値
		inline static float Value() { return m_valueFloats.at(std::this_thread::get_id())(); }
		//----------------------------------------------------------------------------------
		//[ValueToInt]
		//return: 0 ~ 100のランダムなint値
		inline static int ValueToInt() { return m_valueInts.at(std::this_thread::get_id())(); };
		//----------------------------------------------------------------------------------
		//[Range]
		//return: min(含む) ~ max(含む)のランダムなfloat値 
		//引数1: 最小値(含む)
		//引数2: 最大値(含む)
		inline static float Range(float min, float max)
		{
			std::uniform_real_distribution<float> distribution(min, max);
			return distribution(m_randoms.at(std::this_thread::get_id()));
		}
		//----------------------------------------------------------------------------------
		//[RangeToInt]
		//return: min(含む) ~ max(含む)のランダムなint値 
		//引数1: 最小値(含む)
		//引数2: 最大値(含む)
		inline static int RangeToInt(int min, int max)
		{
			std::uniform_int_distribution<int> distribution(min, max);
			return distribution(m_randoms.at(std::this_thread::get_id()));
		}

	private:
		//----------------------------------------------------------------------------------
		//[InitMainThread]
		//メインスレッド用, ランダムリストを追加
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
		//手動用, ランダムリストを追加
		//引数1: スレッド
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
		//サブ用, ランダムリストを追加
		//引数1: スレッド
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

		//乱数
		static std::unordered_map<std::thread::id, std::mt19937> m_randoms;
		//float一様分布	
		static std::unordered_map<std::thread::id, std::uniform_real_distribution<float>> m_distributionFloats;
		//int一様分布	
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
