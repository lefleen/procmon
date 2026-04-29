#pragma once

#include "types.h"
#include "DescriptorRAII.h"
#include "settings.h"

class DataProcess
{
public:
    DWORD pid = 0;
	str_t name = "";
	my_tm work_time = { };
	my_mem memory = { };
	double totalCPU = 0;
	double intervalCPU = 0;

	Result update(DescriptorRAII& descriptor_process, const parameters_process& params, const ProcmonSettings& procmon_settings);

	DataProcess() noexcept = default;
	DataProcess(const DataProcess& other) = default;
};
