#include "ManageProgramm.h"

template<typename... T>
void ManageProgramm::clear_thread_resources(T&... containers)
{
    (containers.clear(), ...);
}

Result ManageProgramm::clear_using_cpu_vec(map_t<DWORD, UsingCpuProc>& using_cpu_processes, const vec_t<DWORD>& pids_processes,
	size_t start_point, size_t end_point)
{
	map_t<DWORD, UsingCpuProc> _using_cpu_processes;

	for (size_t index = start_point; index < end_point; ++index)
	{
		DWORD pid = pids_processes[index];
		_using_cpu_processes[pid] = std::move(using_cpu_processes[pid]);
	}

	using_cpu_processes = _using_cpu_processes;


	return Result::successful;
}

Result ManageProgramm::calculate_start_end_points(const unsigned int max_threads, size_t num_thread, const DWORD count_processes, size_t& start_point, size_t& end_point)
{
    DWORD max_processes_on_this_thread = count_processes / max_threads;

	start_point = num_thread * max_processes_on_this_thread;
	end_point = (num_thread + 1) * max_processes_on_this_thread;

	if (num_thread == max_threads - 1)
		end_point += count_processes - max_processes_on_this_thread * max_threads;

	return Result::successful;
}

Result ManageProgramm::get_information_about_processes(const parameters_process& params, size_t max_threads, size_t num_thread, vec_t<DataProcess>& processes,
        map_t<DWORD, UsingCpuProc>& using_cpu_process, const ProcmonSettings& procmon_settings)
{
    size_t start_point = 0;
    size_t end_point = 0;
	calculate_start_end_points(max_threads, num_thread, params.count_processes, start_point, end_point);
    size_t num_elements = end_point - start_point; 
	processes.reserve(num_elements);

	for (size_t index = start_point; index < end_point; ++index)
	{
		DataProcess current_process { };
        DescriptorRAII descriptor_process { };

		current_process.pid = params.pids_processes[index];

		if (current_process.update(descriptor_process, params, procmon_settings) == Result::failure) continue;
		if (using_cpu_process[current_process.pid].update(descriptor_process, current_process, procmon_settings) == Result::failure) continue;

		processes.push_back(current_process);
	}

	clear_using_cpu_vec(using_cpu_process, params.pids_processes, start_point, end_point);
	return Result::successful;
}

Result ManageProgramm::start_threads(size_t max_threads, vec_t<vec_t<DataProcess>>& processes, vec_t<map_t<DWORD, UsingCpuProc>>& using_cpu_process, const ProcmonSettings& procmon_settings)
{
	parameters_process params { };
    vec_t<std::thread> threads(max_threads);

	int count = 0;
	for (; count <= 5; ++count)
		if(ProcmonLogic::Manage::get_parameters_processes(params) == Result::successful) break;
	if(count == 6) return Result::failure;


	for (size_t num_thread = 0; num_thread < max_threads; ++num_thread)
	{
		threads[num_thread] = std::thread([&params, max_threads, num_thread, &processes, &using_cpu_process, &procmon_settings]() {
			if (get_information_about_processes(params, max_threads, num_thread, processes[num_thread], using_cpu_process[num_thread], procmon_settings) == Result::failure) 
                clear_thread_resources(processes[num_thread], using_cpu_process[num_thread]);
		});
	}

	for (auto& th : threads)
	{
		if (th.joinable())
			th.join();
	}

	return Result::successful;
}

Result ManageProgramm::update_config(vec_t<ProcmonSettingsTable>& procmon_settings_table)
{
	map_t<str_t, str_t> file_config{ };

	Result res_file_load;
	if ((res_file_load = FileUtility::Config::manage(procmon_settings_table, file_config, FileUtility::load_config_file)) != Result::successful)
		return res_file_load;

	Result res_set_config;
	if ((res_set_config = CommandProcessor::Command::Set::array_data(procmon_settings_table, file_config)) != Result::successful)
		return Result::failure;

	return Result::successful;
}

Result ManageProgramm::set_data_settings(const vec_t<ProcmonSettingsTable>& procmon_setting_table, const DataProcess& process, wstr_t& data) 
{
	data += L"|" + std::to_wstring(process.pid) + L"\t\t|";
	for (size_t index = 0; index < procmon_setting_table.size(); ++index)
	{
		if ((*procmon_setting_table[index].status))
		{
			if (procmon_setting_table[index].name == "name") data += process.name + L"\t\t|";
			else if (procmon_setting_table[index].name == "time") data += std::to_wstring(process.work_time.work_time) + L"\t\t|";
			else if (procmon_setting_table[index].name == "memory") data += std::to_wstring(process.using_memory) + L"\t\t|";
			else if (procmon_setting_table[index].name == "totalCPU") data += std::to_wstring(process.total_using_cpu) + L"\t\t|";
			else if (procmon_setting_table[index].name == "intervalCPU") data += std::to_wstring(process.interval_using_cpu) + L"\t\t|";
			else return Result::invalid_arguments;
		}
		else data += L"off\t\t|";
	}
	data += L"\n";

	return Result::successful;
}

Result ManageProgramm::vec_to_str(const vec_t<vec_t<DataProcess>>& processes, const vec_t<ProcmonSettingsTable>& procmon_setting_table, wstr_t& data)
{
	for (auto& it_1 : processes)
	{
		for (auto& it_2 : it_1)
		{
			Result res_set_data;
			if ((res_set_data = set_data_settings(procmon_setting_table, it_2, data)) != Result::successful)
				return res_set_data;
		}
	}

	return Result::successful;
}

Result ManageProgramm::update_data(const vec_t<ProcmonSettingsTable>& procmon_setting_table, const vec_t<vec_t<DataProcess>>& processes) 
{
	vec_t<ProcmonSettings> full_processes;
	wstr_t data = L"|pid\t\t|name\t\t|time\t\t\t|memory\t\t|totalCPU\t\t|intervalCPU\t\t|\n";

	Result res_vec_to_str;
	if ((res_vec_to_str = vec_to_str(processes, procmon_setting_table, data)) != Result::successful)
		return res_vec_to_str;


	std::wcout << data << std::endl;
	// TODO: file function

	return Result::successful;
}

Result ManageProgramm::start_programm(ProcmonSettings& procmon_settings, vec_t<ProcmonSettingsTable>& procmon_settings_table)
{
	size_t max_threads = std::thread::hardware_concurrency() / 2;
	if (max_threads == 0) max_threads = 1;

	vec_t<vec_t<DataProcess>> processes(max_threads);
    vec_t<map_t<DWORD, UsingCpuProc>> using_cpu_process(max_threads);

	while (true)
	{
		Result res_update_config;
		if ((res_update_config = update_config(procmon_settings_table)) != Result::successful)
			return res_update_config;

		if (start_threads(max_threads, processes, using_cpu_process, procmon_settings) == Result::failure)
			return Result::failure;

		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		
		Result res_update_data;
		if ((res_update_data = update_data(procmon_settings_table, processes)) != Result::successful)
			return res_update_data;

		for (auto& vec : processes)
			vec.clear();
    }

	return Result::successful;
}
