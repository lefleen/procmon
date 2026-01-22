#pragma once
#include "libraries.h"

class UsingCpuProc
{
	time_t INTERVAL_CPU_TIME = 0;
	time_t OLD_CPU_TIME = 0;
	ULARGE_INTEGER FULL_TIME_WORK_PROCESS = { };
	
	double _interval_using_cpu = 0;
	double _total_using_cpu = 0;

	Result get_time_using_cpu(const HANDLE& handle_process, ULARGE_INTEGER& time_using_cpu);

	// Загруженность процессора за всё время существования
	Result calculate_total_using_cpu(unsigned long num_cores, HandleRAII& handle_process, double work_time_process);

	// Загруженность за конкретный интервал времени
	Result calculating_interval_using_cpu(unsigned long num_cores, long long pause_interval, HandleRAII& hProcess);

	Result update(HandleRAII& handle_process);
public:
	UsingCpuProc() noexcept = default;

	double get_total() noexcept;

	double get_interaval() noexcept;

	Result calculate(long long pause_interval, HandleRAII& handle_process, double work_time_process);
};