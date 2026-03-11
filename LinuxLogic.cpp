#include "LinuxLogic.h"


Result ProcmonLogic::SharedSpace::parse_string(const int num_element, const str_t& file_data, str_t& res)
{
    if (num_element <= 0 || file_data.empty()) return Result::failure;

    size_t found = 0;
    size_t first_pos_name = 0;
    size_t last_pos_name = 0;
    int _num_element = num_element;
    int num_symbols = 0;

    if (num_element == 2)
    {
        first_pos_name = file_data.find('(', 0);
        last_pos_name = file_data.find(')', 0);

        if (first_pos_name == str_t::npos || last_pos_name == str_t::npos)
            return Result::failure;

        res = file_data.substr(first_pos_name + 1, last_pos_name);

        return Result::successful;
    }

    while(--_num_element >= 1)
        found = file_data.find(' ', found + 1);

    if (found == str_t::npos) return Result::failure;

    first_pos_name = found + 1;
    if((last_pos_name = file_data.find(' ', first_pos_name)) == str_t::npos) return Result::failure;

    num_symbols = last_pos_name - first_pos_name;
    res = file_data.substr(first_pos_name, num_symbols);

    return Result::successful;
}

Result ProcmonLogic::NameProc::get(Process& process)
{
    const int BUFFER_SIZE = 4096;
    char buffer[BUFFER_SIZE];

    str_t path_to_process = "/proc/" + std::to_string(process.pid) +  "/stat";
    str_t file_data = "";
    str_t process_name = "";

    ProcessDescriptorRAII input_file_descriptor = { };
    
    ssize_t num_elements = 0;

    input_file_descriptor = open(path_to_process.c_str(), O_RDONLY);
    if(input_file_descriptor.get() == -1) return Result::failure;
    if((num_elements = read(input_file_descriptor.get(), &buffer, BUFFER_SIZE)) <= 0) return Result::failure;

    file_data = std::string(buffer, static_cast<size_t>(num_elements));
    file_data += '\0';

    if(ProcmonLogic::SharedSpace::parse_string(macNameProcess, file_data, process_name) == Result::failure) return Result::failure;
    
    process.name = process_name;

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
    if(ProcmonLogic::NameProc::get(process) == Result::failure) return Result::failure;

    return Result::successful;
}
