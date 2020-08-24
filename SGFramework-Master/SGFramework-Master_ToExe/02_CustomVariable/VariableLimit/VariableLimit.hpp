/*----------------------------------------------------------------------------------
基本変数型の最小,最大値を宣言するVariableLimitLimit.h
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_VARIABLE_LIMIT_HPP_
#define SGFRAMEWORK_HEADER_VARIABLE_LIMIT_HPP_
#include <limits>
#include "../ConstAndUsing/ConstAndUsing.hpp"

#undef min
#undef max

//Framework namespace
namespace SGFramework
{
	//Limit
	namespace VariableLimit
	{
		//------------------------------------------------------------------------------------
		//signed変数
		constexpr char cCharMin = std::numeric_limits<char>::min();							//char Min
		constexpr char cCharMax = std::numeric_limits<char>::max();						//char Max 
		constexpr short cShortMin = std::numeric_limits<short>::min();						//short Min 
		constexpr short cShortMax = std::numeric_limits<short>::max();						//short Max
		constexpr int cIntMin = std::numeric_limits<int>::min();									//int Min 
		constexpr int cIntMax = std::numeric_limits<int>::max();								//int Max
		constexpr long cLongMin = std::numeric_limits<long>::min();							//long Min 
		constexpr long cLongMax = std::numeric_limits<long>::max();						//long Max
		constexpr long long cLongLongMin = std::numeric_limits<long long>::min();		//long long Min
		constexpr long long cLongLongMax = std::numeric_limits<long long>::max();	//long long Max
		constexpr int64 cInt64Min = std::numeric_limits<int64>::min();						//int64 Min
		constexpr int64 cInt64Max = std::numeric_limits<int64>::max();						//int64 Max

		//------------------------------------------------------------------------------------
		//unsigned変数

		//unsigned char Min
		constexpr unsigned char cUnsignedCharMin = std::numeric_limits<unsigned char>::min();		
		//unsigned char Max
		constexpr unsigned char cUnsignedCharMax = std::numeric_limits<unsigned char>::max();		
		//unsigned short	 Min
		constexpr unsigned short cUnsignedShortMin = std::numeric_limits<unsigned short>::min();
		//unsigned short	 Max
		constexpr unsigned short cUnsignedShortMax = std::numeric_limits<unsigned short>::max();
		//unsigned int Min
		constexpr unsigned int cUnsignedIntMin = std::numeric_limits<unsigned int>::min();
		//unsigned int Max
		constexpr unsigned int cUnsignedIntMax = std::numeric_limits<unsigned int>::max();
		//unsigned long Min
		constexpr unsigned long cUnsignedLongMin = std::numeric_limits<unsigned long>::min();
		//unsigned long Max
		constexpr unsigned long cUnsignedLongMax = std::numeric_limits<unsigned long>::max();
		//unsigned long Min
		constexpr unsigned long long cUnsignedLongLongMin = std::numeric_limits<unsigned long long>::min();
		//unsigned long Max
		constexpr unsigned long long cUnsignedLongLongMax = std::numeric_limits<unsigned long long>::max();
		constexpr byte cByteMin = std::numeric_limits<byte>::min();							//byte Min
		constexpr byte cByteMax = std::numeric_limits<byte>::max();							//byte Max
		constexpr uchar cUcharMin = std::numeric_limits<uchar>::min();						//uchar Min
		constexpr uchar cUcharMax = std::numeric_limits<uchar>::max();						//uchar Max
		constexpr ushort cUshortMin = std::numeric_limits<ushort>::min();					//ushort	 Min
		constexpr ushort cUshortMax = std::numeric_limits<ushort>::max();					//ushort	 Miax
		constexpr uint cUintMin = std::numeric_limits<uint>::min();								//uint Min
		constexpr uint cUintMax = std::numeric_limits<uint>::max();								//uint Miax
		constexpr ulong cUlongMin = std::numeric_limits<ulong>::min();						//uint Min
		constexpr ulong cUlongMax = std::numeric_limits<ulong>::max();						//uint Miax
		constexpr ulonglong cUlongLongMin = std::numeric_limits<ulonglong>::min();		//uint Min
		constexpr ulonglong cUlongLongMax = std::numeric_limits<ulonglong>::max();	//uint Miax
		constexpr uint64 cUint64Min = std::numeric_limits<uint64>::min();						//uint Min
		constexpr uint64 cUint64Max = std::numeric_limits<uint64>::max();					//uint Miax
		
		//------------------------------------------------------------------------------------
		//浮動小数点変数
		constexpr float cFloatMin = std::numeric_limits<float>::min();							//float Min
		constexpr float cFloatMax = std::numeric_limits<float>::max();							//float Max
		constexpr double cDoubleMin = std::numeric_limits<double>::min();					//double Min
		constexpr double cDoubleMax = std::numeric_limits<double>::max();					//double Max
	}
}
#endif // !SGFRAMEWORK_HEADER_VARIABLE_LIMIT_HPP_
