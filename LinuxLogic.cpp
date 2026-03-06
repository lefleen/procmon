#include "LinuxLogic.h"

Result ProcmonLogic::DescriptorProc::get(ProcessDescriptorRAII& descriptor_process, DWORD pid_process)
{
    descriptor_process = pid_process;
    return Result::successful;
}

Result ProcmonLogic::SharedSpace::parse_string(const int num_element, const str_t& file_data, str_t& res)
{
    if(num_element <= 0 || file_data.empty()) return Result::failure;

    size_t found = 0;
    size_t first_pos_name = 0;
    size_t last_pos_name = 0;
    int _num_element = num_element;
    int num_symbols = 0;

    while(--_num_element >= 1)
        found = file_data.find(' ', found + 1);
    first_pos_name += found + 2;
    last_pos_name = file_data.find(' ', first_pos_name);
    if(first_pos_name == std::string::npos || last_pos_name == std::string::npos)
        return Result::failure;
    last_pos_name -= 1;

    num_symbols = last_pos_name - first_pos_name;
    res = file_data.substr(first_pos_name, num_symbols);

    return Result::successful;
}

Result ProcmonLogic::NameProc::get(const ProcessDescriptorRAII& descriptor_process, Process& process)
{
    const int BUFFER_SIZE = 4096;
    char buffer[BUFFER_SIZE];
    str_t path_to_process = "/proc/" + std::to_string(descriptor_process.get()) +  "/stat";
    str_t file_data = "";
    str_t process_name = "";
    int input_file_descriptor = 0;
    
    if((input_file_descriptor = open(path_to_process.c_str(), O_RDONLY)) == -1) return Result::failure;
    if(read(input_file_descriptor, &buffer, BUFFER_SIZE) <= 0) return Result::failure;
    file_data = std::string(buffer);

    if(ProcmonLogic::SharedSpace::parse_string(macNameProcess, file_data, process_name) == Result::failure) return Result::failure;
    
    process.name = process_name;

    close(input_file_descriptor);
    return Result::successful;
}

bool ProcmonLogic::Manage::_isdigit(const str_t& str_pid)
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
        str_t str_pid = std::string(inf_dir->d_name);
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
    if(ProcmonLogic::NameProc::get(descriptor_process, process) == Result::failure) process.name = "NoName\0";

    return Result::successful;
}
