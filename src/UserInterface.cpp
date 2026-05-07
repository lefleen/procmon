#include "UserInterface.h"

void UserInterface::Commands::set()
{
    std::cout << "\nThe command 'set' allow set on or off calculate metrics and also set format view metric\n\n";
    std::cout << "Calculate metrics:\n";
    std::cout << "\t[name], [time], [memory], [iCPU] (interval CPU), [tCPU] (total CPU).\n";
    std::cout << "settings: [on], [off]\n\n";
    std::cout << "Metrics view:\n";
    std::cout << "\t[time_view]. settings: [seconds], [minutes], [hours], [days]\n";
    std::cout << "\t[memory_view]. settings [B], [KiB], [MiB], [GiB], [TiB]\n\n";
    std::cout << "Syntax:\n";
    std::cout << "\tprocmon set [metric|metric_view] [setting|setting_view]\n\n";
    std::cout << "Examples:\n";
    std::cout << "\tprocmon set name on\n";
    std::cout << "\tprocmon set time_view minutes\n\n";
}

void UserInterface::Commands::get()
{
    std::cout << "\nThe command 'get' allow show procmon config or data processes\n\n";
    std::cout << "Show calculate and view settings:\n";
    std::cout << "\t[name], [time], [memory], [iCPU] (interval CPU), [tCPU] (total CPU), [time_view], [memory_view].\n";
    std::cout << "\tshow data processes: [data]\n\n";
    std::cout << "Syntax:\n";
    std::cout << "\tprocmon get [calculate setting|view setting]\n\n";
    std::cout << "Examples:\n";
    std::cout << "\tprocmon get name\n";
    std::cout << "\tprocmon get data\n";
    std::cout << "\tprocmon get time_view\n\n";
}

void UserInterface::Commands::help()
{
    std::cout << "\n\tprocmon (hereinafter referred to as the utility) is a cross-platform console utility intended for process monitoring. This utility collects information about running processes and displays the main process metrics, such as CPU usage, memory usage, and other important metrics. It is also worth noting the provided ability to configure the displayed and calculated metrics for your own tasks and goals. It is planned to continue developing the utility further by adding new functions to it, such as creating logs, that is, recording all previous metrics over a certain period of time.\n\n";
    std::cout << "Commands:\n";
    std::cout << "\t[set], [get]";
    std::cout << "\n\nFor more details write:\n";
    std::cout << "\tprocmon help [command]\n\n";
}

void UserInterface::Data::get(const str_t& data)
{
    std::cout << data << std::endl;
}

void UserInterface::Metrics::all(const vec_t<ProcmonSettingsTable>& procmon_settings_table, const str_t& view_time, const str_t& view_memory)
{
    str_t out = "";

    for(auto& it : procmon_settings_table)
    {
        if(it.metric_type == MetricType::time_view)
        {
           out += "time_view:" + view_time + "\n";
        }
        else if(it.metric_type == MetricType::memory_view)
        {
            out += "memory_view:" + view_memory + "\n";
        }
        else 
        {
            out += it.name + ":";
            if(*it.status) out += "on";
            else out += "off";
            out += "\n";  
        }
    }

    std::cout << out;

}

void UserInterface::Metrics::one(const ProcmonSettingsTable& procmon_setting_table, const MetricType& metric_type, const str_t& time_view, const str_t& memory_view)
{
    if(MetricType::time_view == metric_type)
        std::cout << time_view << "\n";
    else if (MetricType::memory_view == metric_type)
        std::cout << memory_view << "\n";
    else
    {
        if(*procmon_setting_table.status) std::cout << procmon_setting_table.name + ":on\n";
        else std::cout << procmon_setting_table.name + ":off\n";
    }
}

void UserInterface::Assert::time()
{
    std::cout << "if you off the time, tCPU off too.\n";
}

void UserInterface::Errors::show(const str_t& what, const Result res)
{
    str_t full_err = "";

    switch(res)
    {
        case Result::invalid_arguments: full_err = "invalid_arguments: ";
            break;

        case Result::err_load_config: full_err = "open file 'procmon_config' ";
            break;
                                      
        case Result::err_save_config: full_err = "save file 'procmon_config' ";
            break;

        case Result::err_load_data: full_err = "load file 'processes_data' ";
            break;

        case Result::err_parse_command_line: full_err = "parse command line ";
            break;

        default: full_err = "undefined error ";
    }

    full_err += what;

    std::cerr << "[procmon] error: " << full_err << std::endl;
}
