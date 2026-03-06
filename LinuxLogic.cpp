#include "LinuxLogic.h"
#include <iostream>

Result ProcmonLogic::DescriptorProc::get(ProcessDescriptorRAII& descriptor_process, DWORD pid_process)
{
    descriptor_process = pid_process;
    return Result::successful;
}

Result ProcmonLogic::SharedSpace::parse_string(const int num_element, const std::string& file_data, std::string& res)
{
    if(num_element <= 0) return Result::failure;

    size_t found = 0;
    int _num_element = num_element;

    while(--_num_element >= 1)
        found = file_data.find(' ', found + 1);

    if(found == std::string::npos) return Result::failure;

    std::cout << file_data[found + 1] << std::endl;

    return Result::successful;
}

Result ProcmonLogic::NameProc::get(const ProcessDescriptorRAII& descriptor_process, Process& process)
{
    const int BUFFER_SIZE = 4096;
    char buffer[BUFFER_SIZE];
    std::string path_to_process = "/proc/" + std::to_string(descriptor_process.get()) +  "/stat";
    std::string file_data = "";
    std::string process_name = "";
    int input_file_descriptor = 0;
    
    if((input_file_descriptor = open(path_to_process.c_str(), O_RDONLY)) == -1) return Result::failure;
    if(read(input_file_descriptor, &buffer, BUFFER_SIZE) <= 0) return Result::failure;
    file_data = std::string(buffer);

    if(ProcmonLogic::SharedSpace::parse_string(macNameProcess, file_data, process_name) == Result::failure) return Result::failure;

    close(input_file_descriptor);
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
    if(ProcmonLogic::NameProc::get(descriptor_process, process) == Result::failure) process.name = L"NoName\0";

    return Result::successful;
}
