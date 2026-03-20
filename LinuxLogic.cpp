#include "LinuxLogic.h"

Result ProcmonLogic::DescriptorProc::get(ProcessDescriptorRAII& descriptor_process, Process& process)
{
    str_t path_to_process = "/proc/" + std::to_string(process.pid) +  "/stat";

    descriptor_process = open(path_to_process.c_str(), O_RDONLY);
    if(descriptor_process.get() == null_t) return Result::failure;

    return Result::successful;
}

Result ProcmonLogic::NameProc::get(const ProcessDescriptorRAII& descriptor_process, Process& process)
{
    constexpr int BUFFER_SIZE = 4096;
    char buffer[BUFFER_SIZE];

    str_t file_data = "";
    str_t process_name = "";
  
    ssize_t num_elements = 0;

    if(lseek(descriptor_process.get(), 0, SEEK_SET) == -1) return Result::failure;
    if((num_elements = read(descriptor_process.get(), buffer, BUFFER_SIZE)) <= 0) return Result::failure;

    file_data = str_t(buffer, static_cast<size_t>(num_elements));

    if(SharedSpaceLinux::parse_string(macNameProcess, file_data, process_name) == Result::failure) return Result::failure;
    
    process.name = process_name;

    return Result::successful;
}

Result ProcmonLogic::TimeProc::get_working_time_pc(long double& work_time_system)
{
    constexpr int BUFFER_SIZE = 2048;
    char buffer[BUFFER_SIZE];

    ProcessDescriptorRAII descriptor_process;

    str_t file_path = "/proc/uptime";
    str_t file_data = "";
    str_t _work_time_system = "";
    ssize_t num_elements = 0;

    descriptor_process = open(file_path.c_str(), O_RDONLY);
    if(descriptor_process.get() == -1) return Result::failure;
    if((num_elements = read(descriptor_process.get(), buffer, BUFFER_SIZE)) <= 0) return Result::failure;

    file_data = str_t(buffer, static_cast<size_t>(num_elements));

    size_t length_substr = file_data.find(' ');
    if(length_substr == str_t::npos) return Result::failure;

    _work_time_system = file_data.substr(0, length_substr);
    try 
    {
        work_time_system = std::stold(_work_time_system.c_str());
    }
    catch(...)
    {
        return Result::failure;
    }

    return Result::successful;
}

Result ProcmonLogic::TimeProc::get_start_work_time_proc(const ProcessDescriptorRAII& descriptor_process, long double& work_time_proc)
{
    constexpr int NUM_OF_WORK_TIME = 22;
    constexpr int BUFFER_SIZE = 4096;
    char buffer[BUFFER_SIZE];

    ssize_t size_file = 0;
    
    str_t res = "";
    str_t file_data = "";

    if(lseek(descriptor_process.get(), 0, SEEK_SET) == -1) return Result::failure;
    if((size_file = read(descriptor_process.get(), buffer, BUFFER_SIZE)) <= 0) return Result::failure;
    file_data = str_t(buffer, static_cast<size_t>(size_file));

    if(SharedSpaceLinux::parse_string(NUM_OF_WORK_TIME, file_data, res) == Result::failure) return Result::failure;
    try
    {
        work_time_proc = stold(res);
    }
    catch(...)
    {
        return Result::failure;
    }

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

Result ProcmonLogic::TimeProc::seconds_to_my_tm(long double input_time, struct my_tm& output_time)
{
    long _input_time = input_time;
    if(input_time - _input_time >= 0.5) _input_time += 1;

    output_time.work_time = input_time;

    auto chrono_seconds = std::chrono::seconds(_input_time);

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

Result ProcmonLogic::MemoryProc::get(const ProcessDescriptorRAII& descriptor_process, Process& process)
{
    constexpr int NUM_OF_RSS = 24;
    constexpr int BUFFER_SIZE = 4096;

    long double num_pages = 0;
    long double num_bytes_in_page = sysconf(_SC_PAGESIZE);
    long double num_kbytes = 0;
    long double num_bytes = 0;
    if(static_cast<long>(num_bytes_in_page == -1)) return Result::failure;
    char buffer[BUFFER_SIZE];

    str_t file_data = "";
    str_t res = "";

    ssize_t num_elements = 0;

    if(lseek(descriptor_process.get(), 0, SEEK_SET) == -1) return Result::failure;
    if((num_elements = read(descriptor_process.get(), buffer, BUFFER_SIZE)) <= 0) return Result::failure;
    file_data = str_t(buffer, static_cast<size_t>(num_elements));

    if(SharedSpaceLinux::parse_string(NUM_OF_RSS, file_data, res) == Result::failure) return Result::failure;

    try 
    {
        num_pages = stoi(res);
    }
    catch(...)
    {
        return Result::failure;
    }

    num_bytes = num_pages * num_bytes_in_page;

    process.using_memory = num_bytes;

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

        int pid = 0;
        try
        {
            pid = std::stoi(str_pid);
        }
        catch(...)
        {
            return Result::failure;
        }

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
    if(ProcmonLogic::MemoryProc::get(descriptor_process, process) == Result::failure) return Result::failure;

    return Result::successful; 
}
