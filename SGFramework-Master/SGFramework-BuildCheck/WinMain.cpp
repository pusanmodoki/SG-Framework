#include <Windows.h>
#include "../SGFramework-Master_ToExe/02_Variables/Variables.hpp"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	using namespace SGFramework;

	auto ex = Exception::InvalidArgumentException("stringf", "to_radix", "radix is invalid value.");

	auto v1 = stringf::to(10);
	auto v2 = stringf::to("DD");

	auto v3 = stringf::to_radix(10, stringf::radix::x16);
	auto v4 = stringf::to_radix(L"111", stringf::radix::x16);

	auto v5 = stringf::is_start_with_number(L" 123");

	return 0;
}