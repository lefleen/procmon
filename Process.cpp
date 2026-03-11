#ifdef _WIN32
#include "WindowsLogic.h"
#elif defined __linux__
#include "LinuxLogic.h"
#endif

Result Process::update(ProcessDescriptorRAII& descriptor_process, const parameters_process& params)
{
	if (ProcmonLogic::AllData::get_all_data_process(descriptor_process, params, *this) == Result::failure) return Result::failure;
	return Result::successful;
}
