#pragma once

#include "types.h"
#include "DescriptorRAII.h"
#include "settingstypes.h"

class DataProcess
{
public:
#ifdef _WIN32
	wstr_t name = L"";
#elif defined __linux__
    str_t name = "";
#endif

    DWORD pid = 0;
	my_tm work_time = { };
	long double using_memory = 0;
	double total_using_cpu = 0;
	double interval_using_cpu = 0;

	Result update(DescriptorRAII& descriptor_process, const parameters_process& params, const ProcmonSettings& procmon_settings);

	DataProcess() noexcept = default;
	DataProcess(const DataProcess& other) = default;
};
