#include "ManageProgramm.h"

#ifdef __WIN32
namespace CurrentOS = WinowsProc;
#elif defined __linux__
namespace CurrentOS = LinuxProc;
#endif

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

	ProcessDescriptorRAII descriptor_process{ };

	size_t start_index_process = 0;
	size_t end_index_process = 0;
	get_start_and_end_points(start_index_process, end_index_process, max_threads, num_thread, max_processes_on_this_thread, count_processes);
	size_t num_elements = end_index_process - start_index_process;
	processes.reserve(num_elements);

	Process current_process;

	for (size_t index = start_index_process; index < end_index_process; ++index)
	{
		current_process.pid = pids_processes[index];
		pid = current_process.pid;
		current_process.update(descriptor_process, count_bytes_needed);

		using_cpu_processes[pid].calculate(pause_interval, descriptor_process, current_process.work_time.work_time);
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
	vec_t<DWORD> pids_processes{ };

    CurrentOS::ManageOS::get_parameters_processes(count_bytes_needed, count_processes, pids_processes);

	vec_t<std::thread> threads{ };
	threads.resize(max_threads);
	using_cpu_processes.resize(max_threads);

	for (size_t num_thread = 0; num_thread < max_threads; ++num_thread)
	{
		threads[num_thread] = std::thread(get_information_about_processes, count_bytes_needed, count_processes, std::ref(pids_processes),
			max_threads, num_thread, pause_interval, std::ref(processes[num_thread]), std::ref(using_cpu_processes[num_thread]));
	}

	for (auto& th : threads)
	{
		if (th.joinable())
			th.join();
	}

	return Result::successful;
}

Result ManageProgramm::start_programm()
{
	size_t interval_pause = 1000;

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
