#include "Windows.h"

// Получение ID
Result WindowsProc::DescriptorProc::get(ProcessDescriptorRAII& descriptor_process, DWORD pid_process)
{
	// Проверка PID процесса на существование
	if (pid_process != 0)
	{
		// Хэндл процесса
		descriptor_process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
			FALSE, pid_process);

		if (!descriptor_process.get()) return Result::failure;

		return Result::successful;
	}

	return Result::failure;
}

	// Получени имени процесса класса
Result WindowsProc::NameProc::get(const ProcessDescriptorRAII& descriptor_process, DWORD count_bytes_needed, Process& current_process)
{
	// Хэндл модуля процесса
	HMODULE hmodule_process = { };

	if (EnumProcessModules(descriptor_process.get(), &hmodule_process, sizeof(hmodule_process),
		&count_bytes_needed))
	{
		wchar_t process_name[256];

		// Получение имени
		GetModuleBaseNameW(descriptor_process.get(), hmodule_process, process_name,
			256);

		// Проверка. не пуста ли строка
		if (wcslen(process_name) == 0) return Result::failure;

		current_process.name = process_name;

		return Result::successful;
	}

	return Result::failure;
}

	// FILETIME в time_t
Result WindowsProc::TimeProc::filetime_to_time_t(time_t& time, const process_time f_time)
{
	ULARGE_INTEGER ull;
#ifdef __WIN32
	ull.LowPart = f_time.dwLowDateTime;
#elif __linux__
    ull.QuadPart = f_time.QuadPart;
#endif
	time = static_cast<time_t>(ull.QuadPart / 10000000ULL - 11644473600ULL);

	return Result::successful;
}

	// Получение FILETIME в формате time_t
Result WindowsProc::TimeProc::get_create_time_process(const ProcessDescriptorRAII& descriptor_process, time_t& create_time_process, const int choose)
{
	process_time creation_ftime_process = { };
	process_time kernel_ftime_process = { };
	process_time exit_ftime_process = { };
	process_time user_ftime_process = { };

	if (!GetProcessTimes(descriptor_process.get(), &creation_ftime_process, &exit_ftime_process, &kernel_ftime_process, &user_ftime_process)) return Result::failure;

	// Преобразование в системное время
	switch (choose)
	{
	case macCreateTimeProcess: filetime_to_time_t(create_time_process, creation_ftime_process); break;
	case macExitTimeProcess: filetime_to_time_t(create_time_process, exit_ftime_process); break;
	case macKernelTimeProcess: filetime_to_time_t(create_time_process, kernel_ftime_process); break;
	case macUserTimeProcess: filetime_to_time_t(create_time_process, user_ftime_process); break;
	default:;
	}

	return Result::successful;
}

	// Время работы процесса
Result WindowsProc::TimeProc::calculate_work_time_process(time_t& work_time_process, const time_t create_time_process)
{

	time_t current_time = time(NULL);

	if (current_time == -1) return Result::failure;

	// Разность времени текущего и времени старта
	work_time_process = current_time - create_time_process;

	if (work_time_process <= 0) return  Result::failure;

	return Result::successful;
}

	// Преобразование времени в правильные форма
Result WindowsProc::TimeProc::time_t_to_my_tm(time_t input_time, struct my_tm& output_time)
{
	// Всё количество секунд
	output_time.work_time = input_time;

	auto chrono_seconds = std::chrono::seconds(input_time);
	// Количество дней
	auto num_days = std::chrono::duration_cast<std::chrono::hours>(chrono_seconds) / 24;
	output_time.num_days = num_days.count();

	auto num_hours = std::chrono::duration_cast<std::chrono::hours>(chrono_seconds) % 24;
	output_time.num_hours = num_hours.count();

	auto num_minutes = std::chrono::duration_cast<std::chrono::minutes>(chrono_seconds) % 60;
	output_time.num_minutes = num_minutes.count();

	auto num_seconds = std::chrono::duration_cast<std::chrono::seconds>(chrono_seconds) % 60;
	output_time.num_seconds = num_seconds.count();

	return Result::successful;
}

	// Получение времени работы процесса
Result WindowsProc::TimeProc::get(const ProcessDescriptorRAII& descriptor_process, Process& process)
{
	my_tm tm_work_time_process = { };
	time_t create_time_process = 0;
	time_t work_time_process = { };

	// Время в формате time_t
	if (get_create_time_process(descriptor_process, create_time_process, macCreateTimeProcess) == Result::failure)
		return Result::failure;

	// Время работы процесса
	if (calculate_work_time_process(work_time_process, create_time_process) == Result::failure)
		return Result::failure;

	// time_t в my_tm
	time_t_to_my_tm(work_time_process, tm_work_time_process);

	process.work_time = tm_work_time_process;

	return Result::successful;
}

	// Получение ОЗУ процессора
Result WindowsProc::MemoryProc::get(const ProcessDescriptorRAII& descriptor_process, Process& process)
{
	PROCESS_MEMORY_COUNTERS pmc = { };

	if (!GetProcessMemoryInfo(descriptor_process.get(), &pmc, sizeof(pmc))) return Result::failure;

	long double memory = static_cast<long double>(pmc.WorkingSetSize);

	if (memory <= 0) return Result::failure;

	process.using_memory = memory;

	return Result::successful;
}

Result WindowsProc::ManageOS::get_parameters_processes(DWORD& count_bytes_needed, DWORD& count_processes, vec_t<DWORD>& pids_processes)
{
	size_t size = 128;
	pids_processes.resize(size);

	// Получение PID
	while (true)
	{
		if (!EnumProcesses(pids_processes.data(), static_cast<DWORD>(pids_processes.size()) * sizeof(DWORD), &count_bytes_needed))
			return Result::failure;

		if (pids_processes.size() * sizeof(DWORD) != count_bytes_needed)
		{
			break;
		}

		size *= 2;
		pids_processes.resize(size);		
	}

	count_processes = count_bytes_needed / sizeof(DWORD);

	// Если количество процессов равно 0
	if (!count_processes) return Result::failure;

	return Result::successful;
}
