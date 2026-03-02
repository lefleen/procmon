#pragma once
#include <windows.h>
#include <psapi.h>
#include <time.h>
#include <chrono>

#include "types.h"
#include "Process.h"
#include "ProcessDescriptorRAII.h"

namespace ProcmonLogic
{
	// Получение ID процесса
	namespace DescriptorProc
	{
		Result get(ProcessDescriptorRAII& descriptor_process, DWORD pid_process);
	};

	namespace NameProc
	{
		Result get(const ProcessDescriptorRAII& descriptor_process, const parameters_process& params, Process& current_process);
	};

	namespace TimeProc
	{
		Result filetime_to_time_t(time_t& time, const process_time f_time);

		Result get_create_time_process(const ProcessDescriptorRAII& descriptor_process, time_t& create_time_process, const int choose);

		Result calculate_work_time_process(time_t& work_time_process, const time_t create_time_process);

		Result time_t_to_my_tm(time_t input_time, struct my_tm& output_time);

		Result get(const ProcessDescriptorRAII& descriptor_process, Process& process);
	}

	namespace MemoryProc
	{
		Result get(const ProcessDescriptorRAII& descriptor_process, Process& process);
	}

	namespace Manage
	{
		Result get_parameters_processes(parameters_process& params);
	}

    namespace AllData
    {
        Result get_all_data_process(const ProcessDescriptorRAII& descriptor_process, const parameters_process& params, Process& current_process);
    }
}
