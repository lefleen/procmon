#ifdef _WIN32
#include "WindowsLogic.h"
#elif defined __linux___
#include "LinuxLogic.h"
#endif

Result Process::update(const ProcessDescriptorRAII& descriptor_process, const parameters_process& params)
{
    ProcmonLogic::AllData::get_all_data_process(descriptor_process, params, *this);
	return Result::successful;
}
