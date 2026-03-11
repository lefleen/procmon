#include "ManageProgramm.h"

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

Result ManageProgramm::get_information_about_processes( parameters_process& params,
	size_t max_threads, size_t num_thread, double pause_interval, vec_t<Process>& processes, map_t<DWORD, UsingCpuProc>& using_cpu_processes)
{

	DWORD max_processes_on_this_thread = params.count_processes / max_threads;

	ProcessDescriptorRAII descriptor_process{ };

	size_t start_index_process = 0;
	size_t end_index_process = 0;
	get_start_and_end_points(start_index_process, end_index_process, max_threads, num_thread, max_processes_on_this_thread, params.count_processes);
	size_t num_elements = end_index_process - start_index_process;
	processes.reserve(num_elements);

	for (size_t index = start_index_process; index < end_index_process; ++index)
	{
		Process current_process { };

		current_process.pid = params.pids_processes[index];
		DWORD pid = current_process.pid;

		if(current_process.update(descriptor_process, params) == Result::failure) continue;

		if(using_cpu_processes[pid].calculate(pause_interval, descriptor_process, current_process.work_time.work_time) == Result::failure) continue;

		current_process.interval_using_cpu = using_cpu_processes[pid].get_interaval();
		current_process.total_using_cpu = using_cpu_processes[pid].get_total();

		processes.push_back(std::move(current_process));
	}

	clear_using_cpu_vec(using_cpu_processes, params.pids_processes, start_index_process, end_index_process);

	return Result::successful;
}

Result ManageProgramm::start_threads(size_t max_threads, double pause_interval, vec_t<vec_t<Process>>& processes, vec_t<map_t<DWORD, UsingCpuProc>>& using_cpu_processes)
{
	parameters_process params { };

    vec_t<std::thread> threads{ };
	threads.resize(max_threads);
	using_cpu_processes.resize(max_threads);

	int count = 0;
	for (; count <= 5; ++count)
		if(ProcmonLogic::Manage::get_parameters_processes(params) == Result::successful) break;
	if(count == 6) return Result::failure;

	bool* err = new bool[max_threads];

	for (size_t num_thread = 0; num_thread < max_threads; ++num_thread)
	{
		err[num_thread] = false;

		threads[num_thread] = std::thread([&params, max_threads, num_thread, pause_interval, &processes, &using_cpu_processes, &err]() {
			if (get_information_about_processes(params, max_threads, num_thread, pause_interval, processes[num_thread], using_cpu_processes[num_thread]) == Result::failure) 
				err[num_thread] = true;
			});
	}

	for (auto& th : threads)
	{
		if (th.joinable())
			th.join();
	}

	for (int num_thread = 0; num_thread < max_threads; ++num_thread)
	{
		if (err[num_thread])
		{
			processes[num_thread].clear();
			using_cpu_processes[num_thread].clear();
			threads[num_thread].detach();
		}
	}

	delete[] err;

	return Result::successful;
}

Result ManageProgramm::start_programm()
{
	size_t interval_pause = 1000;

	size_t max_threads = std::thread::hardware_concurrency() / 2;
	if (max_threads == 0) max_threads = 1;

	vec_t<vec_t<Process>> processes{ };
	vec_t<map_t<DWORD, UsingCpuProc>> using_cpu_processes{ };

	processes.resize(max_threads);
	using_cpu_processes.reserve(max_threads);

	while (true)
	{
		if (start_threads(max_threads, interval_pause, processes, using_cpu_processes) == Result::failure)
			return Result::failure;

		std::this_thread::sleep_for(std::chrono::milliseconds(interval_pause));
		
		for (auto& vec : processes)
			vec.clear();
    }

	return Result::successful;
}
