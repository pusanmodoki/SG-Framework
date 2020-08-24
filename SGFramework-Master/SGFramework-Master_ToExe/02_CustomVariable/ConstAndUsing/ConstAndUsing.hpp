/*----------------------------------------------------------------------------------
using �� const�萔���L�q����ConstAndUsing.h
----------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_CONST_AND_USING_HPP_
#define SGFRAMEWORK_HEADER_CONST_AND_USING_HPP_
#include <amp.h>
#include <cmath>
#include <limits>
#include <mutex>
#include <array>
#include <DirectXMath.h>
#include <Windows.h>
#include "../Atomic/Atomic.hpp"

#undef min
#undef max

//inline property
#define SGF_FUNCTION_PROPERTY inline
//inline property
#define SGF_NOTINLINE_FUNCTION_PROPERTY

#define IS_TRUE(val) ((val) ^ false)
#define IS_FALSE(val) ((val) ^ true)
#define MAKE_COMMAND(function) ([this]() { function; })
#define MAKE_FOR_COMMAND(function) ([&]() { function; })
#define DELETE_POINTER(pointer) if (pointer != nullptr) { delete pointer; pointer = nullptr; }
#define COM_RELEASE(pointer) if (pointer != nullptr) { pointer->Release(); pointer = nullptr; }
#define PHYSX_RELEASE(pointer) if (pointer != nullptr) { pointer->release(); pointer = nullptr; }

#define DELETE_ALL_DEFAULT_CLASS_FUNCTIONS(className) \
		className() = delete; \
		className(const className&) = delete; \
		className(className&&) = delete; \
		className& operator = (const className&) = delete; \
		className& operator = (className&&) = delete;

#define DEFAULT_COPY_MOVE_FUNCTIONS(className) \
		className(const className&) = default; \
		className(className&&) = default; \
		className& operator = (const className&) = default; \
		className& operator = (className&&) = default;

#define WM_SGFRAMEWORK_BEGIN (WM_APP + 0x2000)
#define WM_SGFRAMEWORK_UPDATE_INPUT (WM_SGFRAMEWORK_BEGIN)
#define WM_SGFRAMEWORK_SET_WINDOW_NAME (WM_SGFRAMEWORK_BEGIN + 1)
#define WM_SGFRAMEWORK_INVALID_MESSAGE (WM_SGFRAMEWORK_BEGIN + 2)
#define WM_SGFRAMEWORK_UPDATE_GUI (WM_SGFRAMEWORK_BEGIN + 3)
#define WM_SGFRAMEWORK_END (WM_APP + 0x20010);

//Framework namespace
namespace SGFramework
{
	//using
	using XmMatrix = DirectX::XMMATRIX;
	using XmVector = DirectX::XMVECTOR;
	using byte = unsigned char;
	using wchar = wchar_t;
	using uchar = unsigned char;
	using ushort = unsigned short;
	using uint = unsigned int;
	using ulong = unsigned long;
	using ulonglong = unsigned long long;
	using int64 = long long;
	using uint64 = unsigned long long;
	namespace GPGPU
	{
		template <class TKey, int dimension>
		using ArrayCpu = concurrency::array_view<TKey, dimension>;
		template <class TKey, int dimension>
		using ArrayGpu = concurrency::array<TKey, dimension>;
		template <class TKey, int dimension>
		using ArrayViewGpu = concurrency::array_view<TKey, dimension>;
	}
	//Fowler-Noll-Vo�p�萔
	namespace Hash
	{
		constexpr size_t cFowlerNollVoPrime = 1099511628211u;
		constexpr size_t cFowlerNollVoOffsetBasis = 14695981039346656037u;
	}

	//�l���p�b�N����Value struct
	//Value�Z�Z�Ƃ��ĕʖ���`����Ă��� (ValueInt�Ȃ�)
	template <typename T>
	struct Value
	{
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//���������s��
		//����1: value
		inline constexpr Value(T value) : value(value) {}

		//T operator
		inline constexpr operator T () const { return value; }
		//() operator return T
		inline constexpr T operator () () const { return value; }
		
		T value;	//value
	};
	//using
	using ValueBool = Value<bool>;
	using ValueChar = Value<char>;
	using ValueShort = Value<short>;
	using ValueInt = Value<int>;
	using ValueLong = Value<long>;
	using ValueLongLong = Value<long long>;
	using ValueInt64 = Value<int64>;
	using ValueByte = Value<byte>;
	using ValueUchar = Value<uchar>;
	using ValueUshort = Value<ushort>;
	using ValueUint = Value<uint>;
	using ValueUlong = Value<ulong>;
	using ValueUlongLong = Value<ulonglong>;
	using ValueInt64 = Value<int64>;
	using ValueSizeT = Value<size_t>;
	using ValueFloat = Value<float>;
	using ValueDouble = Value<double>;

	//����
	struct Dimension
	{
		static constexpr ValueByte c2D = 0x1;	//2D
		static constexpr ValueByte c3D = 0x2;	//3D
	};

	//�v�Z���C�u����
	namespace MathAF
	{
		//�ő�degree
		constexpr float cMaxDegree = 180.0f;
		//�ŏ�degree
		constexpr float cMinDegree = -180.0f;
		//�~����
		constexpr float cPi = 3.141592654f;
		//�~���� x 2
		constexpr float cPix2 = 6.283185307f;
		//�~���� / 2
		constexpr float cPiDiv2 = 1.570796327f;
		//�~���� / 4
		constexpr float cPiDiv4 = 0.785398163f;
		//���̖�����
		constexpr float cInfinity = std::numeric_limits<float>::infinity();
		//���̖�����
		constexpr float cNegativeInfinity = -std::numeric_limits<float>::infinity();
		//Nan
		constexpr float cNaN = std::numeric_limits<float>::quiet_NaN();
		//degree->radian, deg * this
		constexpr float cDegToRad = (cPi * 2) / 360.0f;
		//radian->degree, rad * this
		constexpr float cRadToDeg = 360 / (cPi * 2);

		//float�̌덷
		constexpr float cEpsilon = std::numeric_limits<float>::epsilon();
		//double�̌덷
		constexpr double cDoubleEpsilon = std::numeric_limits<double>::epsilon();
		
		//----------------------------------------------------------------------------------
		//[EquivalentWithEpsilon]
		//�덷�����==���Z���s��
		//return: val1 - val2���덷�ȉ��Ȃ�true��ԋp
		//����1: val1
		//����2: val2
		inline constexpr bool IsEqual(float val1, float val2) { return (val1 - val2) < 0.0f ? (-(val1 - val2) <= cEpsilon) : ((val1 - val2) <= cEpsilon); }
		//----------------------------------------------------------------------------------
		//[EquivalentWithEpsilon]
		//�덷�����==���Z���s��
		//return: val1 - val2���덷�ȉ��Ȃ�true��ԋp
		//����1: val1
		//����2: val2
		inline constexpr bool IsEqual(double val1, double val2) { return val1 - val2 < 0 ? (-(val1 - val2) <= cDoubleEpsilon) : (val1 - val2 <= cDoubleEpsilon); }
		//----------------------------------------------------------------------------------
		//[NonEquivalentWithEpsilon]
		//�덷�����!=���Z���s��
		//return: val1 - val2���덷���߂Ȃ�true��ԋp
		//����1: val1
		//����2: val2
		inline constexpr bool IsNotEqual(float val1, float val2) { return val1 - val2 < 0 ? (-(val1 - val2) > cEpsilon) : (val1 - val2 > cEpsilon); }
		//----------------------------------------------------------------------------------
		//[NonEquivalentWithEpsilon]
		//�덷�����!=���Z���s��
		//return: val1 - val2���덷���߂Ȃ�true��ԋp
		//����1: val1
		//����2: val2
		inline constexpr bool IsNotEqual(double val1, double val2) { return val1 - val2 < 0 ? (-(val1 - val2) > cDoubleEpsilon) : (val1 - val2 > cDoubleEpsilon); }
	}

	//�t�@�C���T�C�Y�ϊ�
	namespace DataVolumeConversion
	{
		//byte->kilobyte, byte / this
		constexpr float cByteToKiloByteFloat = 1024.0f;
		//byte->megabyte, byte / this
		constexpr float cByteToMegaByteFloat = 1024.0f * 1024.0f;
		//byte->gigabyte, byte / this
		constexpr float cByteToGigaByteFloat = 1024.0f * 1024.0f * 1024.0f;

		//kilobyte->byte, byte * this
		constexpr float cKiloByteToByteFloat = 1024.0f;
		//kilobyte->megabyte, byte / this
		constexpr float cKiloByteToMegaByteFloat = 1024.0f;
		//kilobyte->gigabyte, byte / this
		constexpr float cKiloByteToGigaByteFloat = 1024.0f * 1024.0f;

		//megabyte->byte, byte * this
		constexpr float cMegaByteToByteFloat = 1024.0f * 1024.0f;
		//megabyte->kilobyte, byte * this
		constexpr float cMegaByteToKiloByteFloat = 1024.0f;
		//megabyte->gigabyte, byte / this
		constexpr float cMegaByteToGigaByteFloat = 1024.0f;

		//gigabyte->byte, byte * this
		constexpr float cGigaByteToByteFloat = 1024.0f * 1024.0f * 1024.0f;
		//gigabyte->kilobyte, byte * this
		constexpr float cGigaByteToKiloByteFloat = 1024.0f * 1024.0f;
		//gigabyte->megabyte, byte * this
		constexpr float cGigaByteToMegaByteFloat = 1024.0f;

		//byte->kilobyte, byte / this
		constexpr uint64 cByteToKiloByte = 1024;
		//byte->megabyte, byte / this
		constexpr uint64 cByteToMegaByte = 1024 * 1024;
		//byte->gigabyte, byte / this
		constexpr uint64 cByteToGigaByte = 1024 * 1024 * 1024;

		//kilobyte->byte, byte * this
		constexpr uint64 cKiloByteToByte = 1024;
		//kilobyte->megabyte, byte / this
		constexpr uint64 cKiloByteToMegaByte = 1024;
		//kilobyte->gigabyte, byte / this
		constexpr uint64 cKiloByteToGigaByte = 1024 * 1024;

		//megabyte->byte, byte * this
		constexpr uint64 cMegaByteToByte = 1024 * 1024;
		//megabyte->kilobyte, byte * this
		constexpr uint64 cMegaByteToKiloByte = 1024;
		//megabyte->gigabyte, byte / this
		constexpr uint64 cMegaByteToGigaByte = 1024;

		//gigabyte->byte, byte * this
		constexpr uint64 cGigaByteToByte = 1024 * 1024 * 1024;
		//gigabyte->kilobyte, byte * this
		constexpr uint64 cGigaByteToKiloByte = 1024 * 1024;
		//gigabyte->megabyte, byte * this
		constexpr uint64 cGigaByteToMegaByte = 1024;
	}


	//�t���[�����[�N�̊Ǘ�������Administrator namespace
	namespace Administrator
	{
		//----------------------------------------------------------------------------------
		//[GetRefreshRate]
		//return: 30, 60, 120, 144, 244�̒�����GetDeviceCaps�Ŏ擾�������t���b�V�����[�g�ɍł��߂��l
		//����1: GetDeviceCaps�Ŏg�p����HWND
		inline float GetRefreshRate(HWND hWnd)
		{
			//�ݒ胊�t���b�V�����[�g
			constexpr std::array<int, 5> cRefreshs = { 30, 60, 120, 144, 244 };

			int result;
			HDC hdc = GetDC(hWnd);
			bool isEqual = false;

			//Get
			result = GetDeviceCaps(hdc, VREFRESH);
			ReleaseDC(hWnd, hdc);

			//�ݒ胊�t���b�V�����[�g�ƍ��v���邩�m�F
			for (const auto& e : cRefreshs)
				if (result == e) isEqual = true;
			//�ꏏ�Ȃ炻�̂܂ܕԂ�
			if (IS_TRUE(isEqual)) return static_cast<float>(result);

			//��ԋ߂��l��T���p
			int fasts[2] = { 100, -1 };
			//�T�����[�v
			for (int i = 0; i < 5; i++)
			{
				int distance = cRefreshs[i] - result;
				if (distance < 0) distance *= - 1;
				if (fasts[0] > distance)
				{
					fasts[0] = distance;
					fasts[1] = i;
				}
			}

			//��ԋ߂��l�ԋp
			return static_cast<float>(cRefreshs[fasts[1]]);
		}
	}
}
#endif // !SGFRAMEWORK_HEADER_CONST_AND_USING_HPP_