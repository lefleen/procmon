#pragma once
#include <chrono>
#include <thread>

#ifdef __linux__
#include <unistd.h>
#endif

#include "types.h"
#include "ProcessDescriptorRAII.h"
#include "LinuxLogic.h"

class UsingCpuProc
{
#ifdef _WIN32
    const long double NUM_TICKS = 1.0E7;

#elif defined __linux__ 
    const long double NUM_TICKS = sysconf(_SC_CLK_TCK);

#endif

	time_t NEW_TIME = 0;
	time_t OLD_TIME = 0;
	ULARGE_INTEGER FULL_TIME_WORK_PROCESS = { };
	
	double _interval_using_cpu = 0;
	double _total_using_cpu = 0;

	Result get_time_using_cpu(const ProcessDescriptorRAII& descriptor_process, ULARGE_INTEGER& time_using_cpu);

	// Загруженность процессора за всё время существования
	Result calculate_total_using_cpu(unsigned long num_cores, ProcessDescriptorRAII& descriptor_process, double work_time_process);

	// Загруженность за конкретный интервал времени
	Result calculating_interval_using_cpu(unsigned long num_cores, long long pause_interval, ProcessDescriptorRAII& descriptor_process);

	Result update_new_time();

    Result update_old_time();

    Result update_full_time_work_process(const ProcessDescriptorRAII& descriptor_process);

public:
	UsingCpuProc() noexcept = default;

    UsingCpuProc& operator=(UsingCpuProc&& other) noexcept;

	double get_total() const noexcept;

	double get_interaval() const noexcept;

	Result calculate(long long pause_interval, ProcessDescriptorRAII& descriptor_process, double work_time_process);
};
