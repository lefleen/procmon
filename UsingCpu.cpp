#include "UsingCpu.h"

UsingCpuProc& UsingCpuProc::operator=(UsingCpuProc&& other) noexcept
{
    if(this == &other) return *this;

    _interval_using_cpu = other._interval_using_cpu;
    _total_using_cpu = other._total_using_cpu;

    NEW_TIME = other.NEW_TIME;
    OLD_TIME = other.OLD_TIME;

    FULL_TIME_WORK_PROCESS = other.FULL_TIME_WORK_PROCESS;

    return *this;
}

Result UsingCpuProc::get_time_using_cpu(const ProcessDescriptorRAII& descriptor_process, ULARGE_INTEGER& time_using_cpu)
{
	process_time creation_time_process = { };
	process_time exit_time_process = { };
    process_time kernel_time_process = { };
	process_time user_time_process = { };
	ULARGE_INTEGER kernel_unated_time = { };
	ULARGE_INTEGER user_unated_time = { };

#ifdef _WIN32
	if (!GetProcessTimes(descriptor_process.get(), &creation_time_process, &exit_time_process, &kernel_time_process, &user_time_process)) return Result::failure;

	kernel_unated_time = { kernel_time_process.dwLowDateTime, kernel_time_process.dwHighDateTime };
	user_unated_time = { user_time_process.dwLowDateTime, user_time_process.dwHighDateTime };

	time_using_cpu.QuadPart = kernel_unated_time.QuadPart + user_unated_time.QuadPart;

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

    time_using_cpu.QuadPart = user_time_process + kernel_time_process;

#endif

	return Result::successful;
}

	// Загруженность процессора за всё время существования
Result UsingCpuProc::calculate_total_using_cpu(unsigned long num_cores, ProcessDescriptorRAII& descriptor_process, double work_time_process)
{
	ULARGE_INTEGER all_time_using_cpu = { };
	ULARGE_INTEGER all_time_after_create = { };

	double total_using_cpu = 0;

    if(static_cast<long>(NUM_TICKS) == -1) return Result::failure;
	all_time_after_create.QuadPart = work_time_process * NUM_TICKS;

	if (get_time_using_cpu(descriptor_process, all_time_using_cpu) == Result::failure || all_time_after_create.QuadPart == 0) return Result::failure;

	total_using_cpu = static_cast<double>(all_time_using_cpu.QuadPart) / (static_cast<double>(num_cores) * static_cast<double>(all_time_after_create.QuadPart));
	total_using_cpu *= 100;

	_total_using_cpu = total_using_cpu;

	return Result::successful;
}

	// Загруженность за конкретный интервал времени
Result UsingCpuProc::calculating_interval_using_cpu(unsigned long num_cores, ProcessDescriptorRAII& descriptor_process)
{
	double interval_using_cpu = 0;

    if(static_cast<long>(NUM_TICKS) == -1) return Result::failure;

	ULARGE_INTEGER time_work_process = { };

	if (get_time_using_cpu(descriptor_process, time_work_process) == Result::failure || NEW_TIME == 0 || OLD_TIME == 0)
	{
        update_new_time();
        update_old_time();
        if(update_full_time_work_process(descriptor_process) == Result::failure) return Result::failure;

		return Result::failure;
	}

    update_new_time();
    double interval = NEW_TIME - OLD_TIME;
    update_old_time();
    ULARGE_INTEGER full_time_work_process = FULL_TIME_WORK_PROCESS;
    if(update_full_time_work_process(descriptor_process) == Result::failure) return Result::failure;

    if(interval <= 0 || full_time_work_process.QuadPart <= 0) return Result::failure;

    double interval_cpu_time = interval * NUM_TICKS;

	time_work_process.QuadPart -= full_time_work_process.QuadPart;

	interval_using_cpu = static_cast<double>(time_work_process.QuadPart) / (static_cast<double>(num_cores) * static_cast<double>(interval_cpu_time));

	interval_using_cpu *= 100;

	_interval_using_cpu = interval_using_cpu;

	return Result::successful;
}

Result UsingCpuProc::update_new_time()
{
	auto now = std::chrono::steady_clock::now();
    auto num_nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch());
	NEW_TIME = static_cast<double>(num_nanoseconds.count()) / 1.0E9;

	return Result::successful;
}

Result UsingCpuProc::update_old_time()
{
    auto now = std::chrono::steady_clock::now();
    auto num_nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch());
    OLD_TIME = static_cast<double>(num_nanoseconds.count()) / 1.0E9;

    return Result::successful;
}

Result UsingCpuProc::update_full_time_work_process(const ProcessDescriptorRAII& descriptor_process)
{
    if(get_time_using_cpu(descriptor_process, FULL_TIME_WORK_PROCESS) == Result::failure) return Result::failure;

    return Result::successful;
}

Result UsingCpuProc::calculate(ProcessDescriptorRAII& descriptor_process, double work_time_process)
{
	// Количество количества логических потоков
	unsigned long num_cores = std::thread::hardware_concurrency();
	if (num_cores == 0) num_cores = 1;

	// ОБщее использоание CPU
	if (calculate_total_using_cpu(num_cores, descriptor_process, work_time_process) == Result::failure)
		return Result::failure;

	// За определенный интервал времени
	if (calculating_interval_using_cpu(num_cores, descriptor_process) == Result::failure)
		return Result::failure;

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
