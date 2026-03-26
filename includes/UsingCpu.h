#pragma once
#include <chrono>
#include <thread>

#ifdef __linux__
#include <unistd.h>

#include "SharedSpaceLinux.h"

#endif

#include "types.h"
#include "ProcessDescriptorRAII.h"
#include "DataProcess.h"

class UsingCpuProc
{
#ifdef _WIN32
    const long double NUM_TICKS = 1.0E7;

#elif defined __linux__ 
    const long double NUM_TICKS = sysconf(_SC_CLK_TCK);

#endif

	double CURRENT_PROCESS_TIME = 0;
	double PREVIOUS_PROCESS_TIME = 0;
	ULARGE_INTEGER FULL_PROCESS_TIME = { };
	
	double _interval_using_cpu = 0;
	double _total_using_cpu = 0;

	Result get_time_using_cpu(const ProcessDescriptorRAII& descriptor_process, ULARGE_INTEGER& current_work_time);

	// Загруженность процессора за всё время существования
	Result calculate_total_using_cpu(unsigned long num_cores, const ProcessDescriptorRAII& descriptor_process, double work_time_process);

	// Загруженность за конкретный интервал времени
	Result calculating_interval_using_cpu(unsigned long num_cores, const ProcessDescriptorRAII& descriptor_process);

	Result update_current_time();

    Result get_interval_cpu_usage_time(const ProcessDescriptorRAII& descriptor_process, double& interval_cpu_usage_time, ULARGE_INTEGER& delta_process_cpu_time);

public:
	UsingCpuProc() noexcept = default;

	UsingCpuProc(const UsingCpuProc& other) noexcept = default;

    UsingCpuProc& operator=(UsingCpuProc&& other) noexcept;

    Result update(const ProcessDescriptorRAII& descriptor_process, Process& process);

	double get_total() const noexcept;

	double get_interval() const noexcept;

	Result calculate(const ProcessDescriptorRAII& descriptor_process, double work_time_process);
};
