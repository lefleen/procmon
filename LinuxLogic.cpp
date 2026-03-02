#include "LinuxLogic.h"

Result ProcmonLogic::DescriptorProc::get(ProcessDescriptorRAII& descriptor_process, DWORD pid_process)
{
    descriptor_process = pid_process;
    return Result::successful;
}

Result ProcmonLogic::NameProc::get(ProcessDescriptorRAII& descriptor_process)
{

    return Result::successful;
}

bool ProcmonLogic::Manage::_isdigit(const std::string& str_pid)
{
    if(str_pid.empty()) return Result::failure;
    for(size_t index = 0; index < str_pid.length(); ++index)
        if(str_pid[index] < '0' || str_pid[index] > '9') return false;
    return true;
}

Result ProcmonLogic::Manage::get_parameters_processes(parameters_process& params) 
{
    const char* path_to_proc = "/proc/";

    dirent* inf_dir = nullptr;
    std::unique_ptr<DIR, int (*)(DIR*)> dir(opendir(path_to_proc), closedir);

    if(dir == nullptr) 
        return Result::failure;

    while((inf_dir = readdir(dir.get())) != nullptr)
    {
        std::string str_pid = std::string(inf_dir->d_name);
        if(!_isdigit(str_pid)) continue;

        int pid = std::stoi(str_pid);
        params.pids_processes.push_back(pid);
    }

    params.count_processes = params.pids_processes.size();
    
    if(!params.count_processes) return Result::failure;

    return Result::successful; 
}

Result ProcmonLogic::AllData::get_all_data_process(const ProcessDescriptorRAII& descriptor_process, const parameters_process& params, Process& process)
{
    return Result::successful;
}
