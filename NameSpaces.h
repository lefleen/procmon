#pragma once
#include "libraries.h"

// Получение ID процесса
namespace HandleProc
{
	Result get(HandleRAII& handle_process, DWORD pid_process);
};

namespace NameProc
{
	Result get(const HandleRAII& handle_process, DWORD count_bytes_needed, Process& current_process);
};

namespace TimeProc
{
	Result filetime_to_time_t(time_t& time, const FILETIME f_time);

	Result get_create_time_process(const HandleRAII& handle_process, time_t& create_time_process, const int choose);

	Result calculate_work_time_process(time_t& work_time_process, const time_t create_time_process);

	Result time_t_to_my_tm(time_t input_time, struct my_tm& output_time);

	Result get(const HandleRAII& handle_process, Process& process);
}

namespace MemoryProc
{
	Result get(const HandleRAII& handle_process, Process& process);
}

namespace ManageProgramm
{
	Result get_parameters_processes(DWORD& count_bytes_needed, DWORD& count_processes, vec_t<DWORD>& pids_processes);

	Result get_start_and_end_points(size_t& start_index_process, size_t& end_index_process, const unsigned int max_threads, size_t num_thread,
		DWORD max_process_on_this_thread, const DWORD count_processes);

	Result clear_using_cpu_vec(map_t<DWORD, UsingCpuProc>& using_cpu_processes, vec_t<DWORD>& pids_processes, size_t start_index_process, size_t end_index_process);

	Result get_information_about_processes(DWORD bytes_needed, DWORD count_processes, vec_t<DWORD>& pids_processes, size_t max_threads, size_t num_thread, 
		double pause_interval, vec_t<Process>& processes, map_t<DWORD, UsingCpuProc>& using_cpu_processes);

	Result start_threads(size_t max_threads, double pause_interval, vec_t<vec_t<Process>>& processes, vec_t<map_t<DWORD, UsingCpuProc>>& using_cpu_processes);

	Result start_programm(time_t pause = 1000);
}