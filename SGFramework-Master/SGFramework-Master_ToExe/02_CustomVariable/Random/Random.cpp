/*----------------------------------------------------------------------------------
�����_���Ȏ����𐶐����� Random Class
	��static�ϐ����L�q����Random.cpp
------------------------------------------------------------------------------------*/	
#include "Random.hpp"

//Framework namespace
namespace SGFramework
{
	//����
	std::unordered_map<std::thread::id, std::mt19937> Random::m_randoms;
	//float��l���z	
	std::unordered_map<std::thread::id, std::uniform_real_distribution<float>> Random::m_distributionFloats;
	//int��l���z	
	std::unordered_map<std::thread::id, std::uniform_int_distribution<int>> Random::m_distributionInts;
	//floatBind
	std::unordered_map<std::thread::id, std::_Binder<std::_Unforced, std::uniform_real_distribution<float>&, std::reference_wrapper<std::mt19937>>> Random::m_valueFloats;
	//intBind
	std::unordered_map<std::thread::id, std::_Binder<std::_Unforced, std::uniform_int_distribution<int>&, std::reference_wrapper<std::mt19937>>> Random::m_valueInts;
	//lock
	std::mutex Random::m_lock;
}