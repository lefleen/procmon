#include "UsingCpu.h"

UsingCpuProc& UsingCpuProc::operator=(UsingCpuProc&& other) noexcept
{
    if(this == &other) return *this;

    _interval_using_cpu = other._interval_using_cpu;
    _total_using_cpu = other._total_using_cpu;

    CURRENT_PROCESS_TIME = other.CURRENT_PROCESS_TIME;
    PREVIOUS_PROCESS_TIME = other.PREVIOUS_PROCESS_TIME;

    FULL_PROCESS_TIME = other.FULL_PROCESS_TIME;

    return *this;
}

Result UsingCpuProc::update(const ProcessDescriptorRAII& descriptor_process, DataProcess& process)
{
    Result res = Result::successful;

    if ((res = calculate(descriptor_process, process.work_time.work_time)) == Result::failure) return Result::failure;
    else if (res == Result::initialization) _interval_using_cpu = 0;

    process.interval_using_cpu = _interval_using_cpu;;
    process.total_using_cpu = _total_using_cpu;

    return Result::successful;
}

Result UsingCpuProc::get_time_using_cpu(const ProcessDescriptorRAII& descriptor_process, ULARGE_INTEGER& current_work_time)
{
	process_time creation_time_process = { };
	process_time exit_time_process = { };
    process_time kernel_time_process = { };
	process_time user_time_process = { };
	ULARGE_INTEGER kernel_time_value = { };
	ULARGE_INTEGER user_time_value = { };

#ifdef _WIN32
	if (!GetProcessTimes(descriptor_process.get(), &creation_time_process, &exit_time_process, &kernel_time_process, &user_time_process)) return Result::failure;

	kernel_time_value = { kernel_time_process.dwLowDateTime, kernel_time_process.dwHighDateTime };
	user_time_value = { user_time_process.dwLowDateTime, user_time_process.dwHighDateTime };

	current_work_time.QuadPart = kernel_time_value.QuadPart + user_time_value.QuadPart;

#elif defined __linux__
    constexpr int BUFFER_SIZE = 4096;
    constexpr int NUM_OF_UTIME = 14;
    constexpr int NUM_OF_STIME = 15;

    char buffer[BUFFER_SIZE];

    str_t file_data = "";
    str_t str_system_time_process = "";
    str_t str_user_time_process = "";

    ssize_t num_elements = 0;

    if(lseek(descriptor_process.get(), 0, SEEK_SET) == -1) return Result::failure;
    if((num_elements = read(descriptor_process.get(), buffer, BUFFER_SIZE)) <= 0) return Result::failure;

    file_data = str_t(buffer, static_cast<size_t>(num_elements));

    if(ProcmonLogic::SharedSpaceLinux::parse_string(NUM_OF_UTIME, file_data, str_user_time_process) == Result::failure) return Result::failure;
    if(ProcmonLogic::SharedSpaceLinux::parse_string(NUM_OF_STIME, file_data, str_system_time_process) == Result::failure) return Result::failure;

    try
    {
        user_time_process = std::stoull(str_user_time_process);
        kernel_time_process = std::stoull(str_system_time_process);
    }
    catch(...)
    {
        return Result::failure;
    }

    current_work_time.QuadPart = user_time_process + kernel_time_process;

#endif

	return Result::successful;
}

	// Загруженность процессора за всё время существования
Result UsingCpuProc::calculate_total_using_cpu(unsigned long num_cores, const ProcessDescriptorRAII& descriptor_process, double work_time_process)
{
	ULARGE_INTEGER current_work_time = { };
	ULARGE_INTEGER time_after_start = { };

	double total_using_cpu = 0;

    if(static_cast<long>(NUM_TICKS) == -1) return Result::failure;
	time_after_start.QuadPart = work_time_process * NUM_TICKS;

	if (get_time_using_cpu(descriptor_process, current_work_time) == Result::failure || time_after_start.QuadPart == 0) return Result::failure;

	total_using_cpu = static_cast<double>(current_work_time.QuadPart) / (static_cast<double>(num_cores) * static_cast<double>(time_after_start.QuadPart));
	total_using_cpu *= 100;

	_total_using_cpu = total_using_cpu;

	return Result::successful;
}

	// Загруженность за конкретный интервал времени
Result UsingCpuProc::calculating_interval_using_cpu(unsigned long num_cores, const ProcessDescriptorRAII& descriptor_process)
{
	double cpu_usage = 0;
    double interval_cpu_usage_time = 0;
    ULARGE_INTEGER current_process_cpu_time { };

    if(static_cast<long>(NUM_TICKS) == -1) return Result::failure;

	if (CURRENT_PROCESS_TIME == 0 || PREVIOUS_PROCESS_TIME == 0)
	{
        if(get_interval_cpu_usage_time(descriptor_process, interval_cpu_usage_time, current_process_cpu_time) == Result::failure) return Result::failure;

		return Result::initialization;
	}

    if(get_interval_cpu_usage_time(descriptor_process, interval_cpu_usage_time, current_process_cpu_time) == Result::failure) return Result::failure;
    if(interval_cpu_usage_time <= 0) return Result::failure;

    double interval_cpu_time = interval_cpu_usage_time * NUM_TICKS;

	cpu_usage = static_cast<double>(current_process_cpu_time.QuadPart) / (static_cast<double>(num_cores) * static_cast<double>(interval_cpu_time));

	cpu_usage *= 100;

	_interval_using_cpu = cpu_usage;

	return Result::successful;
}

Result UsingCpuProc::get_interval_cpu_usage_time(const ProcessDescriptorRAII& descriptor_process, double& interval_cpu_usage_time, ULARGE_INTEGER& delta_process_cpu_time)
{
    ULARGE_INTEGER current_work_time { };

    if(get_time_using_cpu(descriptor_process, current_work_time) == Result::failure) return Result::failure;

    if(update_current_time() == Result::failure) return Result::failure;

    interval_cpu_usage_time = CURRENT_PROCESS_TIME - PREVIOUS_PROCESS_TIME;

    if(current_work_time.QuadPart <= FULL_PROCESS_TIME.QuadPart) return Result::failure;
    delta_process_cpu_time.QuadPart = current_work_time.QuadPart - FULL_PROCESS_TIME.QuadPart; 

    FULL_PROCESS_TIME = current_work_time;
    PREVIOUS_PROCESS_TIME = CURRENT_PROCESS_TIME;

    return Result::successful;
}

Result UsingCpuProc::update_current_time()
{
	auto now = std::chrono::steady_clock::now();
    auto num_nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch());
	CURRENT_PROCESS_TIME = static_cast<double>(num_nanoseconds.count()) / 1.0E9;

	return Result::successful;
}

Result UsingCpuProc::calculate(const ProcessDescriptorRAII& descriptor_process, double work_time_process)
{
	// Количество количества логических потоков
	unsigned long num_cores = std::thread::hardware_concurrency();
	if (num_cores == 0) num_cores = 1;

	// ОБщее использоание CPU
    Result res;
	if (calculate_total_using_cpu(num_cores, descriptor_process, work_time_process) == Result::failure)
		return Result::failure;

	// За определенный интервал времени
	if ((res = calculating_interval_using_cpu(num_cores, descriptor_process)) == Result::failure)
		return Result::failure;
    else if(res == Result::initialization)
        return Result::initialization;

	return Result::successful;
}

double UsingCpuProc::get_total() const noexcept 
{
	return _total_using_cpu;
}

double UsingCpuProc::get_interval() const noexcept
{
	return _interval_using_cpu;
}
