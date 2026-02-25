#pragma once
#include <thread>

#include "types.h"
#include "UsingCpu.h"

namespace ManageProgramm
{
	Result get_start_and_end_points(size_t& start_index_process, size_t& end_index_process, const unsigned int max_threads, size_t num_thread,
		DWORD max_process_on_this_thread, const DWORD count_processes);

	Result clear_using_cpu_vec(map_t<DWORD, UsingCpuProc>& using_cpu_processes, vec_t<DWORD>& pids_processes, size_t start_index_process, size_t end_index_process);

	Result get_information_about_processes(DWORD bytes_needed, DWORD count_processes, vec_t<DWORD>& pids_processes, size_t max_threads, size_t num_thread,
		double pause_interval, vec_t<Process>& processes, map_t<DWORD, UsingCpuProc>& using_cpu_processes);

	Result start_threads(size_t max_threads, double pause_interval, vec_t<vec_t<Process>>& processes, vec_t<map_t<DWORD, UsingCpuProc>>& using_cpu_processes);

	Result start_programm();
}
