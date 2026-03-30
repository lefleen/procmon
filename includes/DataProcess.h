#pragma once

#include "types.h"
#include "ProcessDescriptorRAII.h"

class DataProcess
{
public:
	DWORD pid = 0;

#ifdef _WIN32
	wstr_t name = L"";
#elif defined __linux__
    str_t name = "";
#endif

	my_tm work_time = { };
	long double using_memory = 0;
	double total_using_cpu = 0;
	double interval_using_cpu = 0;

	Result update(ProcessDescriptorRAII& descriptor_process, const parameters_process& params);

	DataProcess() noexcept = default;
	DataProcess(const DataProcess& other) = default;
};
