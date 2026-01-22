#include "libraries.h"

Result Process::update(const HandleRAII& handle_process, DWORD count_bytes_needed)
{
	if (NameProc::get(handle_process, count_bytes_needed, *this) == Result::failure) name = L"NoName";

	if (TimeProc::get(handle_process, *this) == Result::failure) work_time = { };

	if (MemoryProc::get(handle_process, *this) == Result::failure) using_memory = NULL;

	return Result::successful;
}