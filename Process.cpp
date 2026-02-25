#include "Process.h"

Result Process::update(const ProcessDescriptorRAII& descriptor_process, DWORD count_bytes_needed)
{
#ifdef __WIN32
	if (WindowsProc::NameProc::get(descriptor_process, count_bytes_needed, *this) == Result::failure) name = L"NoName";

	if (WindowsProc::TimeProc::get(descriptor_process, *this) == Result::failure) work_time = { };

	if (WindowsProc::MemoryProc::get(descriptor_process, *this) == Result::failure) using_memory = NULL;

#elif defined __linux__

#endif

	return Result::successful;
}
