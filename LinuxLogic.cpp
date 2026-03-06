#include "LinuxLogic.h"

Result ProcmonLogic::DescriptorProc::get(ProcessDescriptorRAII& descriptor_process, DWORD pid_process)
{
    descriptor_process = pid_process;
    return Result::successful;
}

Result ProcmonLogic::NameProc::get(const ProcessDescriptorRAII& descriptor_process)
{
    const int BUFFER_SIZE = 4096;
    char buffer[BUFFER_SIZE];
    std::string path_to_process = "/proc/" + std::to_string(descriptor_process.get()) +  "/stat";
    int input_file_descriptor = 0;

    if((input_file_descriptor = open(path_to_process.c_str(), O_RDONLY))) return Result::failure;
    if(read(input_file_descriptor, &buffer, BUFFER_SIZE)) return Result::failure;

    return Result::successful;
}

bool ProcmonLogic::Manage::_isdigit(const std::string& str_pid)
{
    if(str_pid.empty()) return false;
    for(size_t index = 0; index < str_pid.length(); ++index)
        if(str_pid[index] < '0' || str_pid[index] > '9') return false;
    return true;
}

Result ProcmonLogic::Manage::get_parameters_processes(parameters_process& params) 
{
    const char* path_to_dir = "/proc/";

    dirent* inf_dir = nullptr;
    std::unique_ptr<DIR, int (*)(DIR*)> dir(opendir(path_to_dir), closedir);

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

Result ProcmonLogic::AllData::get_all_data_process(ProcessDescriptorRAII& descriptor_process, const parameters_process& params, Process& process)
{
    ProcmonLogic::DescriptorProc::get(descriptor_process, process.pid);
    if(ProcmonLogic::NameProc::get(descriptor_process) == Result::failure) process.name = L"NoName\0";

    return Result::successful;
}
