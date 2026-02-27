#include "LinuxLogic.h"

//@TODO сделать запись в вектор, как в WindowsLogic.cpp
Result ProcmonLogic::Manage::get_parameters_processes(parameters_process& params) 
{
    const char* path_to_proc = "/proc/";
    DIR* dir = opendir(path_to_proc); 
    dirent* inf_dir;

    if(dir == nullptr) 
        return Result::failure;

    while((inf_dir = readdir(dir)) != NULL)
    {
        int pid = atoi(inf_dir->d_name);
        if(pid > 0)
            std::cout << pid << std::endl;
    }

    closedir(dir);
    return Result::successful; 
}

Result ProcmonLogic::AllData::get_all_data_process(const ProcessDescriptorRAII& descriptor_process, const parameters_process& params, Process& process)
{
    return Result::successful;
}
