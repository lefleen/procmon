#pragma once
#include <dirent.h>
#include <memory>
#include <fcntl.h>
#include <chrono>
#include <iostream>

#include "SharedSpaceLinux.h"
#include "types.h"
#include "ProcessDescriptorRAII.h"
#include "Process.h"

namespace ProcmonLogic 
{
     namespace SharedSpace
     {
         Result parse_string(const int num_element, const std::string& file_data, std::string& str);
     }

     namespace DescriptorProc
     {
         Result get(ProcessDescriptorRAII& descriptor_process, Process& process);
     }

     namespace NameProc
     {
         Result get(const ProcessDescriptorRAII& descriptor_process, Process& process);
     }

     namespace TimeProc
     {
         Result get_working_time_pc(long double& work_time_system);

         Result get_start_work_time_proc(const ProcessDescriptorRAII& descriptor_process, long double& res);

         Result get_work_time_proc(const long double work_time_system, const long double start_work_time_proc, long double& work_time_proc);

         Result seconds_to_my_tm(long double input_time, struct my_tm& output_time);

         Result get(const ProcessDescriptorRAII& descriptor_process, Process& process);

     }
     
     namespace MemoryProc
     {
         Result get(const ProcessDescriptorRAII& descriptor_process, Process& process);
     }

     namespace Manage
     {
          Result get_parameters_processes(parameters_process& params);

          bool _isdigit(const std::string& str_pid);
     }
     
     namespace AllData 
     {
         Result get_all_data_process(ProcessDescriptorRAII& descriptor_process, const parameters_process& params, Process& process);
     }

}
