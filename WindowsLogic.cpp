#include "WindowsLogic.h"

// Получение ID
Result ProcmonLogic::DescriptorProc::get(ProcessDescriptorRAII& descriptor_process, DWORD pid_process)
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
Result ProcmonLogic::NameProc::get(const ProcessDescriptorRAII& descriptor_process, DWORD count_bytes_needed, Process& current_process)
{
	// Хэндл модуля процесса
	HMODULE hmodule_process = { };

	if (EnumProcessModules(descriptor_process.get(), &hmodule_process, sizeof(hmodule_process),
		&count_bytes_needed))
	{
		wchar_t process_name[256];

		// Получение имени
		size_t len = 0;
		if ((len = GetModuleBaseNameW(descriptor_process.get(), hmodule_process, process_name,
			256)) == 0) return Result::failure;

		current_process.name = wstr_t(process_name, len);

		return Result::successful;
	}

	return Result::failure;
}

	// FILETIME в time_t
Result ProcmonLogic::TimeProc::filetime_to_time_t(time_t& time, const process_time f_time)
{
	ULARGE_INTEGER ull;
#ifdef _WIN32
	ull.LowPart = f_time.dwLowDateTime;
	ull.HighPart = f_time.dwHighDateTime;
#elif __linux__
    ull.QuadPart = f_time.QuadPart;
#endif
	time = static_cast<time_t>(ull.QuadPart / 10000000ULL - 11644473600ULL);

	return Result::successful;
}

	// Получение FILETIME в формате time_t
Result ProcmonLogic::TimeProc::get_create_time_process(const ProcessDescriptorRAII& descriptor_process, time_t& create_time_process, const int choose)
{
	process_time creation_ftime_process = { };
	process_time kernel_ftime_process = { };
	process_time exit_ftime_process = { };
	process_time user_ftime_process = { };

	if (!GetProcessTimes(descriptor_process.get(), &creation_ftime_process, &exit_ftime_process, &kernel_ftime_process, &user_ftime_process)) return Result::failure;

	// Преобразование в системное время
	switch (choose)
	{
	case macCreateTimeProcess: if (filetime_to_time_t(create_time_process, creation_ftime_process) == Result::failure) return Result::failure; break;
	case macExitTimeProcess: if (filetime_to_time_t(create_time_process, exit_ftime_process) == Result::failure) return Result::failure; break;
	case macKernelTimeProcess: if (filetime_to_time_t(create_time_process, kernel_ftime_process) == Result::failure) return Result::failure; break;
	case macUserTimeProcess: if (filetime_to_time_t(create_time_process, user_ftime_process) == Result::failure) return Result::failure; break;
	default:;
	}

	return Result::successful;
}

	// Время работы процесса
Result ProcmonLogic::TimeProc::calculate_work_time_process(time_t& work_time_process, const time_t create_time_process)
{

	time_t current_time = time(NULL);

	if (current_time == -1) return Result::failure;

	// Разность времени текущего и времени старта
	work_time_process = current_time - create_time_process;

	if (work_time_process <= 0) return  Result::failure;

	return Result::successful;
}

	// Преобразование времени в правильные форма
Result ProcmonLogic::TimeProc::time_t_to_my_tm(time_t input_time, struct my_tm& output_time)
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
Result ProcmonLogic::TimeProc::get(const ProcessDescriptorRAII& descriptor_process, Process& process)
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
Result ProcmonLogic::MemoryProc::get(const ProcessDescriptorRAII& descriptor_process, Process& process)
{
	PROCESS_MEMORY_COUNTERS pmc = { };

	if (!GetProcessMemoryInfo(descriptor_process.get(), &pmc, sizeof(pmc))) return Result::failure;

	long double memory = static_cast<long double>(pmc.WorkingSetSize);

	if (memory <= 0) return Result::failure;

	process.using_memory = memory;

	return Result::successful;
}

Result ProcmonLogic::Manage::get_parameters_processes(parameters_process& params)
{
	size_t size = 128;
	params.pids_processes.resize(size);

	// Получение PID
	while (true)
	{
		if (!EnumProcesses(params.pids_processes.data(), static_cast<DWORD>(params.pids_processes.size()) * sizeof(DWORD), &params.count_bytes_needed))
			return Result::failure;

		if (params.pids_processes.size() * sizeof(DWORD) != params.count_bytes_needed)
		{
			break;
		}

		size *= 2;
		params.pids_processes.resize(size);		
	}

    params.count_processes = params.count_bytes_needed / sizeof(DWORD);
    params.pids_processes.resize(params.count_processes);

	// Если количество процессов равно 0
	if (!params.count_processes) return Result::failure;

	return Result::successful;
}

Result ProcmonLogic::AllData::get_all_data_process(ProcessDescriptorRAII& descriptor_process, const parameters_process& params, Process& process)
{
    if (ProcmonLogic::DescriptorProc::get(descriptor_process, process.pid) == Result::failure) return Result::failure; 
    if (ProcmonLogic::NameProc::get(descriptor_process, params.count_bytes_needed, process) == Result::failure) process.name = L"NoName";
    if (ProcmonLogic::TimeProc::get(descriptor_process, process) == Result::failure) process.work_time = { };
    if (ProcmonLogic::MemoryProc::get(descriptor_process, process) == Result::failure) process.using_memory = NULL;

	return Result::successful;
}
