#include "WindowsLogic.h"


Result Process::update(const ProcessDescriptorRAII& descriptor_process, const parameters_process& params)
{
#ifdef _WIN32
	if (ProcmonLogic::NameProc::get(descriptor_process, params.count_bytes_needed, *this) == Result::failure) name = L"NoName";

	if (ProcmonLogic::TimeProc::get(descriptor_process, *this) == Result::failure) work_time = { };

	if (ProcmonLogic::MemoryProc::get(descriptor_process, *this) == Result::failure) using_memory = NULL;

#elif defined __linux__

#endif

	return Result::successful;
}
