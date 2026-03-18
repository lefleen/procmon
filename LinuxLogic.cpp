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

        num_symbols = last_pos_name - (first_pos_name + 1);
        res = file_data.substr(first_pos_name + 1, num_symbols);

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

Result ProcmonLogic::DescriptorProc::get(ProcessDescriptorRAII& descriptor_process, Process& process)
{
    str_t path_to_process = "/proc/" + std::to_string(process.pid) +  "/stat";

    descriptor_process = open(path_to_process.c_str(), O_RDONLY);
    if(descriptor_process.get() == null_t) return Result::failure;

    return Result::successful;
}

Result ProcmonLogic::NameProc::get(const ProcessDescriptorRAII& descriptor_process, Process& process)
{
    const int BUFFER_SIZE = 4096;
    char buffer[BUFFER_SIZE];

    str_t file_data = "";
    str_t process_name = "";
  
    ssize_t num_elements = 0;

    if((num_elements = read(descriptor_process.get(), &buffer, BUFFER_SIZE)) <= 0) return Result::failure;

    file_data = str_t(buffer, static_cast<size_t>(num_elements));
    file_data += '\0';

    if(ProcmonLogic::SharedSpace::parse_string(macNameProcess, file_data, process_name) == Result::failure) return Result::failure;
    
    process.name = process_name;

    return Result::successful;
}

Result ProcmonLogic::TimeProc::get_working_time_pc(long double& work_time_system)
{
    const int BUFFER_SIZE = 2048;
    char buffer[BUFFER_SIZE];

    ProcessDescriptorRAII descriptor_process;

    str_t file_path = "/proc/uptime";
    str_t file_data = "";
    str_t _work_time_system = "";
    ssize_t num_elements = 0;

    descriptor_process = open(file_path.c_str(), O_RDONLY);
    if(descriptor_process.get() == -1) return Result::failure;
    if((num_elements = read(descriptor_process.get(), &buffer, BUFFER_SIZE)) <= 0) return Result::failure;

    file_data = str_t(buffer, static_cast<size_t>(num_elements));
    file_data += '\0';

    int length_substr = file_data.find(' ');
    if(length_substr == str_t::npos) return Result::failure;

    _work_time_system = file_data.substr(0, length_substr);
    work_time_system = std::stold(_work_time_system.c_str());

    return Result::successful;
}

Result ProcmonLogic::TimeProc::get_start_work_time_proc(const ProcessDescriptorRAII& descriptor_process, long double& work_time_proc)
{
    constexpr int NUM_OF_WORK_TIME = 22;
    const int BUFFER_SIZE = 4096;
    char buffer[BUFFER_SIZE];

    ssize_t size_file = 0;
    
    str_t res = "";
    str_t file_data = "";
    if((size_file = read(descriptor_process.get(), &buffer, BUFFER_SIZE)) <= 0) return Result::failure;
    file_data = str_t(buffer, static_cast<size_t>(size_file));
    file_data += '\0';

    if(SharedSpace::parse_string(NUM_OF_WORK_TIME, file_data, res) == Result::failure) return Result::failure;
    work_time_proc = stold(res);

    return Result::successful;
}

Result ProcmonLogic::TimeProc::get_work_time_proc(const long double work_time_system, const long double start_work_time_proc, long double& work_time_proc)
{
    long double num_ticks_per_second = sysconf(_SC_CLK_TCK);
    if(num_ticks_per_second == -1) return Result::failure;

    work_time_proc = work_time_system - (start_work_time_proc / num_ticks_per_second);
    if(work_time_proc < 0) return Result::failure;

    return Result::successful;
}

Result ProcmonLogic::TimeProc::seconds_to_my_tm(long input_time, struct my_tm& output_time)
{
    output_time.work_time = input_time;
 
    auto chrono_seconds = std::chrono::seconds(input_time);

    auto num_days = std::chrono::duration_cast<std::chrono::hours>(chrono_seconds) / 24;
    output_time.num_days = num_days.count();

    auto num_hours = std::chrono::duration_cast<std::chrono::hours>(chrono_seconds) % 24;
    output_time.num_hours = num_hours.count();

    auto num_minutes = std::chrono::duration_cast<std::chrono::minutes>(chrono_seconds) % 60;
    output_time.num_minutes = num_minutes.count();

    auto num_seconds = std::chrono::duration_cast<std::chrono::seconds>(chrono_seconds) % 60;
    output_time.num_seconds = num_seconds.count();

    return Result::successful;

}

Result ProcmonLogic::TimeProc::get(const ProcessDescriptorRAII& descriptor_process, Process& process)
{
     long double work_time_system = 0;
     long double start_work_time_proc = 0;
     long double work_time_proc_in_sec = 0;
     my_tm work_time_proc = { };

     if(get_working_time_pc(work_time_system) == Result::failure) return Result::failure;
     if(get_start_work_time_proc(descriptor_process, start_work_time_proc) == Result::failure) return Result::failure;
     if(get_work_time_proc(work_time_system, start_work_time_proc, work_time_proc_in_sec) == Result::failure) return Result::failure;
     if(seconds_to_my_tm(work_time_proc_in_sec, work_time_proc) == Result::failure) return Result::failure; 

     process.work_time = work_time_proc;

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
    if(ProcmonLogic::DescriptorProc::get(descriptor_process, process) == Result::failure) return Result::failure;
    if(ProcmonLogic::NameProc::get(descriptor_process, process) == Result::failure) return Result::failure;
    if(ProcmonLogic::TimeProc::get(descriptor_process, process) == Result::failure) return Result::failure;
    return Result::successful;
}
