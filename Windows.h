#pragma once
#include "libraries.h"

namespace WindowsProc
{

	// Получение ID процесса
	namespace DescriptorProc
	{
		Result get(ProcessDescriptorRAII& descriptor_process, DWORD pid_process);
	};

	namespace NameProc
	{
		Result get(const ProcessDescriptorRAII& descriptor_process, DWORD count_bytes_needed, Process& current_process);
	};

	namespace TimeProc
	{
		Result filetime_to_time_t(time_t& time, const FILETIME f_time);

		Result get_create_time_process(const ProcessDescriptorRAII& descriptor_process, time_t& create_time_process, const int choose);

		Result calculate_work_time_process(time_t& work_time_process, const time_t create_time_process);

		Result time_t_to_my_tm(time_t input_time, struct my_tm& output_time);

		Result get(const ProcessDescriptorRAII& descriptor_process, Process& process);
	}

	namespace MemoryProc
	{
		Result get(const ProcessDescriptorRAII& descriptor_process, Process& process);
	}

	namespace ManageOS
	{
		Result get_parameters_processes(DWORD& count_bytes_needed, DWORD& count_processes, vec_t<DWORD>& pids_processes);
	}
}