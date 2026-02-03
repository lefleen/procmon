#include "libraries.h"

Result Process::update(const HandleRAII& handle_process, DWORD count_bytes_needed)
{
	if (WindowsProc::NameProc::get(handle_process, count_bytes_needed, *this) == Result::failure) name = L"NoName";

	if (WindowsProc::TimeProc::get(handle_process, *this) == Result::failure) work_time = { };

	if (WindowsProc::MemoryProc::get(handle_process, *this) == Result::failure) using_memory = NULL;

	return Result::successful;
}