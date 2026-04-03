#include "DataProcess.h"

#ifdef _WIN32
#include "WindowsLogic.h"
#elif defined __linux__
#include "LinuxLogic.h"
#endif

Result DataProcess::update(ProcessDescriptorRAII& descriptor_process, const parameters_process& params, const ProcmonSettings& procmon_settings)
{
	if (ProcmonLogic::AllData::get_all_data_process(descriptor_process, params, *this, procmon_settings) == Result::failure) return Result::failure;

	return Result::successful;
}
