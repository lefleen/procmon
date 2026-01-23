#pragma once
#include "libraries.h"

	// Получение ID
Result HandleProc::get(HandleRAII& handle_process, DWORD pid_process)
{
	// Проверка PID процесса на существование
	if (pid_process != 0)
	{
		// Хэндл процесса
		handle_process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
			FALSE, pid_process);

		if (!handle_process.get()) return Result::failure;

		return Result::successful;
	}

	return Result::failure;
}

	// Получени имени процесса класса
Result NameProc::get(const HandleRAII& handle_process, DWORD count_bytes_needed, Process& current_process)
{
	// Хэндл модуля процесса
	HMODULE hmodule_process = { };

	if (EnumProcessModules(handle_process.get(), &hmodule_process, sizeof(hmodule_process),
		&count_bytes_needed))
	{
		wchar_t process_name[256];

		// Получение имени
		GetModuleBaseNameW(handle_process.get(), hmodule_process, process_name,
			256);

		// Проверка. не пуста ли строка
		if (wcslen(process_name) == 0) return Result::failure;

		current_process.name = process_name;

		return Result::successful;
	}

	return Result::failure;
}

	// FILETIME в time_t
Result TimeProc::filetime_to_time_t(time_t& time, const FILETIME f_time)
{
	ULARGE_INTEGER ull;
	ull.LowPart = f_time.dwLowDateTime;
	ull.HighPart = f_time.dwHighDateTime;
	time = static_cast<time_t>(ull.QuadPart / 10000000ULL - 11644473600ULL);

	return Result::successful;
}

	// Получение FILETIME в формате time_t
Result TimeProc::get_create_time_process(const HandleRAII& handle_process, time_t& create_time_process, const int choose)
{
	FILETIME creation_ftime_process = { };
	FILETIME kernel_ftime_process = { };
	FILETIME exit_ftime_process = { };
	FILETIME user_ftime_process = { };

	if (!GetProcessTimes(handle_process.get(), &creation_ftime_process, &exit_ftime_process, &kernel_ftime_process, &user_ftime_process)) return Result::failure;

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
Result TimeProc::calculate_work_time_process(time_t& work_time_process, const time_t create_time_process)
{

	time_t current_time = time(NULL);

	if (current_time == -1) return Result::failure;

	// Разность времени текущего и времени старта
	work_time_process = current_time - create_time_process;

	if (work_time_process <= 0) return  Result::failure;

	return Result::successful;
}

	// Преобразование времени в правильные форма
Result TimeProc::time_t_to_my_tm(time_t input_time, struct my_tm& output_time)
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
Result TimeProc::get(const HandleRAII& handle_process, Process& process)
{
	my_tm tm_work_time_process = { };
	time_t create_time_process = 0;
	time_t work_time_process = { };

	// Время в формате time_t
	if (get_create_time_process(handle_process, create_time_process, macCreateTimeProcess) == Result::failure)
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
Result MemoryProc::get(const HandleRAII& handle_process, Process& process)
{
	PROCESS_MEMORY_COUNTERS pmc = { };

	if (!GetProcessMemoryInfo(handle_process.get(), &pmc, sizeof(pmc))) return Result::failure;

	long double memory = static_cast<long double>(pmc.WorkingSetSize);

	if (memory <= 0) return Result::failure;

	process.using_memory = memory;

	return Result::successful;
}

Result ManageProgramm::get_parameters_processes(DWORD& count_bytes_needed, DWORD& count_processes, vec_t<DWORD>& pids_processes)
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

Result ManageProgramm::get_start_and_end_points(size_t& start_index_process, size_t& end_index_process, const unsigned int max_threads, size_t num_thread,
	DWORD max_process_on_this_thread, const DWORD count_processes)
{
	start_index_process = num_thread * max_process_on_this_thread;
	end_index_process = (num_thread + 1) * max_process_on_this_thread;

	if (num_thread == max_threads - 1)
		end_index_process += count_processes - max_process_on_this_thread * max_threads;

	return Result::successful;
}

Result ManageProgramm::clear_using_cpu_vec(map_t<DWORD, UsingCpuProc>& using_cpu_processes, vec_t<DWORD>& pids_processes, 
	size_t start_index_process, size_t end_index_process) 
{
	map_t<DWORD, UsingCpuProc> _using_cpu_processes;

	for (size_t index = start_index_process; index < end_index_process; ++index) 
	{
		DWORD pid = pids_processes[index];
		_using_cpu_processes[pid] = std::move(using_cpu_processes[pid]);
	}

	using_cpu_processes = std::move(_using_cpu_processes);

	return Result::successful;
}

Result ManageProgramm::get_information_about_processes(DWORD count_bytes_needed, DWORD count_processes, vec_t<DWORD>& pids_processes,
	size_t max_threads, size_t num_thread, double pause_interval, vec_t<Process>& processes, map_t<DWORD, UsingCpuProc>& using_cpu_processes)
{
	DWORD pid = 0;

	DWORD max_processes_on_this_thread = count_processes / max_threads;

	HandleRAII handle_process { };

	size_t start_index_process = 0;
	size_t end_index_process = 0;
	get_start_and_end_points(start_index_process, end_index_process, max_threads, num_thread, max_processes_on_this_thread, count_processes);
	size_t num_elements = end_index_process - start_index_process;
	processes.reserve(num_elements);

	Process current_process;

	for (size_t index = start_index_process; index < end_index_process; ++index)
	{
		if (HandleProc::get(handle_process, pids_processes[index]) == Result::failure) continue;
		
		current_process.pid = pids_processes[index];
		pid = current_process.pid;
		current_process.update(handle_process, count_bytes_needed);

		using_cpu_processes[pid].calculate(pause_interval, handle_process, current_process.work_time.work_time);
		current_process.interval_using_cpu = using_cpu_processes[pid].get_interaval();
		current_process.total_using_cpu = using_cpu_processes[pid].get_total();

		processes.push_back(std::move(current_process));
	}

	clear_using_cpu_vec(using_cpu_processes, pids_processes, start_index_process, end_index_process);

	return Result::successful;
}

Result ManageProgramm::start_threads(size_t max_threads, double pause_interval, vec_t<vec_t<Process>>& processes, vec_t<map_t<DWORD, UsingCpuProc>>& using_cpu_processes)
{
	DWORD count_bytes_needed = 0, count_processes = 0;
	vec_t<DWORD> pids_processes { };

	get_parameters_processes(count_bytes_needed, count_processes, pids_processes);

	vec_t<std::thread> threads { };
	threads.resize(max_threads);
	using_cpu_processes.resize(max_threads);

	for (size_t num_thread = 0; num_thread < max_threads; ++num_thread)
	{
		threads[num_thread] = std::thread(get_information_about_processes, count_bytes_needed, count_processes, std::ref(pids_processes), 
			max_threads, num_thread, pause_interval, std::ref(processes[num_thread]), std::ref(using_cpu_processes[num_thread]));
	}

	for (auto& th : threads)
	{
		if(th.joinable())
			th.join();
	}

	return Result::successful;
}

Result ManageProgramm::start_programm(time_t interval_pause)
{
	size_t max_threads = std::thread::hardware_concurrency() / 2;

	vec_t<vec_t<Process>> processes{ };
	vec_t<map_t<DWORD, UsingCpuProc>> using_cpu_processes{ };

	processes.resize(max_threads);
	using_cpu_processes.reserve(max_threads);

	while (true)
	{
		for (auto& vec : processes)
			vec.clear();

		start_threads(max_threads, interval_pause, processes, using_cpu_processes);
		std::this_thread::sleep_for(std::chrono::milliseconds(interval_pause));
	}

	return Result::successful;
}