/*----------------------------------------------------------------------------------
ランダムな実数を生成する Random Class
	→static変数を記述するRandom.cpp
------------------------------------------------------------------------------------*/	
#include "Random.hpp"

//Framework namespace
namespace SGFramework
{
	//乱数
	std::unordered_map<std::thread::id, std::mt19937> Random::m_randoms;
	//float一様分布	
	std::unordered_map<std::thread::id, std::uniform_real_distribution<float>> Random::m_distributionFloats;
	//int一様分布	
	std::unordered_map<std::thread::id, std::uniform_int_distribution<int>> Random::m_distributionInts;
	//floatBind
	std::unordered_map<std::thread::id, std::_Binder<std::_Unforced, std::uniform_real_distribution<float>&, std::reference_wrapper<std::mt19937>>> Random::m_valueFloats;
	//intBind
	std::unordered_map<std::thread::id, std::_Binder<std::_Unforced, std::uniform_int_distribution<int>&, std::reference_wrapper<std::mt19937>>> Random::m_valueInts;
	//lock
	std::mutex Random::m_lock;
}