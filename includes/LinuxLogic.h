#pragma once

#include <dirent.h>
#include <memory>
#include <fcntl.h>
#include <chrono>
#include <iostream>

#include "SharedSpaceLinux.h"
#include "types.h"
#include "DescriptorRAII.h"
#include "DataProcess.h"

namespace ProcmonLogic 
{
     namespace DescriptorProc
     {
         Result get(DescriptorRAII& descriptor_process, DataProcess& process);
     }

     namespace NameProc
     {
         Result get(const DescriptorRAII& descriptor_process, DataProcess& process);
     }

     namespace TimeProc
     {
         Result get_working_time_pc(long double& work_time_system);

         Result get_start_work_time_proc(const DescriptorRAII& descriptor_process, long double& res);

         Result get_work_time_proc(const long double work_time_system, const long double start_work_time_proc, long double& work_time_proc);

         Result seconds_to_my_tm(long double input_time, struct my_tm& output_time);

         Result get(const DescriptorRAII& descriptor_process, DataProcess& process);

     }
     
     namespace MemoryProc
     {
         Result get(const DescriptorRAII& descriptor_process, DataProcess& process);

         void bytes_to_my_mem(const long double input_mem, my_mem& output_mem);
     }

     namespace Manage
     {
          Result get_parameters_processes(parameters_process& params);

          bool _isdigit(const std::string& str_pid);
     }

     namespace AllData 
     {
         Result get_all_data_process(DescriptorRAII& descriptor_process, const parameters_process& params, DataProcess& process, const ProcmonSettings& procmon_settings);
     }

}
