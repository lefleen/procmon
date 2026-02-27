#pragma once
#include "types.h"
#include "ProcessDescriptorRAII.h"
#include "Process.h"

namespace ProcmonLogic 
{
     namespace Manage
     {
          Result get_parameters_processes(parameters_process& params);
     }

     namespace DescriptorProc
     {

     }

     namespace AllData 
     {
         Result get_all_data_process(const ProcessDescriptorRAII& descriptor_process, const parameters_process& params, Process& process);
     }

}
