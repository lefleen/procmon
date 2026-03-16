#pragma once
#include <dirent.h>
#include <memory>
#include <fcntl.h>

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
         Result get_boot_time(const ProcessDescriptorRAII& descriptor_process, long double& work_time_system);

         Result get(const ProcessDescriptorRAII& descriptor_process);
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
