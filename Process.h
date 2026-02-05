#pragma once
#include "libraries.h"


class Process
{
public:

	DWORD pid = 0;
	wstr_t name = L"";
	my_tm work_time = { };
	long double using_memory = 0;
	double total_using_cpu = 0;
	double interval_using_cpu = 0;

	Result update(const ProcessDescriptorRAII& descriptor_process, DWORD count_bytes_needed);

	Process() noexcept = default;
	Process(const Process&) = default;
};