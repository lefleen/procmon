#include "ManageProgramm.h"

template<typename... T>
void ManageProgramm::clear_thread_resources(T&... containers)
{
    (containers.clear(), ...);
}

Result ManageProgramm::get_start_and_end_points(const unsigned int max_threads, size_t num_thread, const DWORD count_processes, size_t& num_elements)
{
    DWORD max_processes_on_this_thread = count_processes / max_threads;

	size_t start_index_process = num_thread * max_processes_on_this_thread;
	size_t end_index_process = (num_thread + 1) * max_processes_on_this_thread;

	if (num_thread == max_threads - 1)
		end_index_process += count_processes - max_processes_on_this_thread * max_threads;

	return Result::successful;
}

Result ManageProgramm::get_information_about_processes( parameters_process& params,
	size_t max_threads, size_t num_thread, vec_t<Process>& processes)
{
    size_t num_elements = 0;
	get_start_and_end_points(max_threads, num_thread, params.count_processes, num_elements);
	processes.reserve(num_elements);

	for (size_t index = 0; index < num_elements; ++index)
	{
		Process current_process { };
        ProcessDescriptorRAII descriptor_process { };

		current_process.pid = params.pids_processes[index];

		if(current_process.update(descriptor_process, params) == Result::failure) continue;

		processes.push_back(std::move(current_process));
	}

	return Result::successful;
}

Result ManageProgramm::start_threads(size_t max_threads, vec_t<vec_t<Process>>& processes)
{
	parameters_process params { };
    vec_t<std::thread> threads(max_threads);

	int count = 0;
	for (; count <= 5; ++count)
		if(ProcmonLogic::Manage::get_parameters_processes(params) == Result::successful) break;
	if(count == 6) return Result::failure;


	for (size_t num_thread = 0; num_thread < max_threads; ++num_thread)
	{
		threads[num_thread] = std::thread([&params, max_threads, num_thread, &processes]() {
			if (get_information_about_processes(params, max_threads, num_thread, processes[num_thread]) == Result::failure) 
                clear_thread_resources(processes[num_thread]);
		});
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
	if (max_threads == 0) max_threads = 1;

	vec_t<vec_t<Process>> processes{ };

	processes.resize(max_threads);

	while (true)
	{
		if (start_threads(max_threads, processes) == Result::failure)
			return Result::failure;

		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		
		for (auto& vec : processes)
			vec.clear();
    }

	return Result::successful;
}
