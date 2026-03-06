#pragma once
#include "ProcessDescriptorRAII.h"
#include "types.h"

class Process
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

	Process() noexcept = default;
	Process(const Process&) = default;
    Process(Process&) = default;
};
