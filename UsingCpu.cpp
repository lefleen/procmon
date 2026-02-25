#include "UsingCpu.h"

Result UsingCpuProc::get_time_using_cpu(const descriptor_process_t& descriptor_process, ULARGE_INTEGER& time_using_cpu)
{
	process_time creation_time_process = { };
	process_time exit_time_process = { };
    process_time kernel_time_process = { };
	process_time user_time_process = { };
	ULARGE_INTEGER kernel_unated_time = { };
	ULARGE_INTEGER user_unated_time = { };

#ifdef __WIN32
	if (!GetProcessTimes(descriptor_process, &creation_time_process, &exit_time_process, &kernel_time_process, &user_time_process)) return Result::failure;

	kernel_unated_time = { kernel_time_process.dwLowDateTime, kernel_time_process.dwHighDateTime };
	user_unated_time = { user_time_process.dwLowDateTime, user_time_process.dwHighDateTime };

	time_using_cpu.QuadPart = kernel_unated_time.QuadPart + user_unated_time.QuadPart;

#elif defined __linux__
    

#endif

	return Result::successful;
}

	// Загруженность процессора за всё время существования
Result UsingCpuProc::calculate_total_using_cpu(unsigned long num_cores, ProcessDescriptorRAII& descriptor_process, double work_time)
{
	ULARGE_INTEGER all_time_using_cpu = { };
	ULARGE_INTEGER all_time_after_create = { };

	double total_using_cpu = 0;

	all_time_after_create.QuadPart = work_time * 1.0E7;
	if (get_time_using_cpu(descriptor_process.get(), all_time_using_cpu) == Result::failure || all_time_after_create.QuadPart == 0) return Result::failure;

	total_using_cpu = static_cast<double>(all_time_using_cpu.QuadPart) / (static_cast<double>(num_cores) * static_cast<double>(all_time_after_create.QuadPart));
	total_using_cpu *= 100;

	_total_using_cpu = total_using_cpu;

	return Result::successful;
}

	// Загруженность за конкретный интервал времени
Result UsingCpuProc::calculating_interval_using_cpu(unsigned long num_cores, long long pause_interval, ProcessDescriptorRAII& descriptor_process)
{
	double interval_using_cpu = 0;
	double interval_cpu_time = INTERVAL_CPU_TIME * 1.0E7;
	ULARGE_INTEGER time_work_process = { };

	if (get_time_using_cpu(descriptor_process.get(), time_work_process) == Result::failure || INTERVAL_CPU_TIME == 0 || OLD_CPU_TIME == 0)
	{
		update(descriptor_process);
		return Result::failure;
	}

	time_work_process.QuadPart -= FULL_TIME_WORK_PROCESS.QuadPart;


	interval_using_cpu = static_cast<double>(time_work_process.QuadPart) / (static_cast<double>(num_cores) * static_cast<double>(interval_cpu_time));

	interval_using_cpu *= 100;

	_interval_using_cpu = interval_using_cpu;

	update(descriptor_process);

	return Result::successful;
}

Result UsingCpuProc::update(ProcessDescriptorRAII& descriptor_process)
{
	auto now = std::chrono::system_clock::now();
	INTERVAL_CPU_TIME = std::chrono::system_clock::to_time_t(now) - OLD_CPU_TIME;
	OLD_CPU_TIME = std::chrono::system_clock::to_time_t(now);

	get_time_using_cpu(descriptor_process.get(), FULL_TIME_WORK_PROCESS);

	return Result::successful;
}

Result UsingCpuProc::calculate(long long pause_interval, ProcessDescriptorRAII& descriptor_process, double work_time_process)
{
	// Количество количества логических потоков
	unsigned long num_cores = std::thread::hardware_concurrency();

	// ОБщее использоание CPU
	if (calculate_total_using_cpu(num_cores, descriptor_process, work_time_process) == Result::failure)
		return Result::failure;

	// За определенный интервал времени
	if (calculating_interval_using_cpu(num_cores, pause_interval, descriptor_process) == Result::failure)
		return Result::failure;

	return Result::successful;
}

double UsingCpuProc::get_total() noexcept 
{
	return _total_using_cpu;
}

double UsingCpuProc::get_interaval() noexcept
{
	return _interval_using_cpu;
}
