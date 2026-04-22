#include "DataProcess.h"

#ifdef _WIN32
#include "WindowsLogic.h"
#elif defined __linux__
#include "LinuxLogic.h"
#endif

Result DataProcess::update(DescriptorRAII& descriptor_process, const parameters_process& params, const ProcmonSettings& procmon_settings)
{
	Result res_all_data;

	if ((res_all_data = ProcmonLogic::AllData::get_all_data_process(descriptor_process, params, *this, procmon_settings)) != Result::successful) 
		return res_all_data;

	return Result::successful;
}
