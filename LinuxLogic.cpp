#include "LinuxLogic.h"

Result ProcmonLogic::Manage::get_parameters_processes(parameters_process& params) 
{
     return Result::successful; 
}

Result ProcmonLogic::AllData::get_all_data(ProcessDescriptorRAII& descriptor_process, const parameters_process& params, Process& process);
