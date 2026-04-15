#pragma once

#include <thread>

#include "DataProcess.h"
#include "types.h"
#include "UsingCpu.h"
#include "settings.h"
#include "FileUtility.h"
#include "CommandProcessor.h"
#include <iostream>

#ifdef _WIN32
#include "WindowsLogic.h"
#elif defined  __linux__
#include "LinuxLogic.h"
#endif

namespace ManageProgramm
{
    template<typename... T>
    void clear_thread_resources(T&... containers);

	Result clear_using_cpu_vec(map_t<DWORD, UsingCpuProc>& using_cpu_processes, const vec_t<DWORD>& pids_processes,
		size_t start_point, size_t end_point);

	Result calculate_start_end_points(const unsigned int max_threads, size_t num_thread, const DWORD count_processes, size_t& start_point, size_t& end_point);

	Result get_information_about_processes(const parameters_process& params, size_t max_threads, size_t num_thread, vec_t<DataProcess>& processes, map_t<DWORD, UsingCpuProc>& using_cpu_process, const ProcmonSettings& procmon_settings);

	Result start_threads(size_t max_threads, vec_t<vec_t<DataProcess>>& processes, vec_t<map_t<DWORD, UsingCpuProc>>& using_cpu_process, const ProcmonSettings& procmon_settings);

	Result update_config(vec_t<ProcmonSettingsTable>& procmon_settings_table);

	Result vec_to_str(const vec_t<vec_t<DataProcess>>& processes, const vec_t<ProcmonDataTable>& procmon_setting_table, str_t& data);

	Result update_data(const vec_t<ProcmonDataTable>& procon_settings, const vec_t<vec_t<DataProcess>>& processes);

	Result set_data_settings(const vec_t<ProcmonDataTable>& procmon_setting_table, const DataProcess& process, str_t& data);

	Result start_programm(ProcmonSettings& procmon_settings, vec_t<ProcmonSettingsTable>& procmon_settings_table);
}
