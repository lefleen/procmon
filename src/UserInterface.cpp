#include "UserInterface.h"

void UserInterface::Commands::set()
{

}

void UserInterface::Commands::get()
{

}

void UserInterface::Commands::help()
{

}

void UserInterface::Metrics::all(const ProcmonSettings& procmon_settings)
{

}

void UserInterface::Metrics::one(const ProcmonSettings& procmon_settings, const MetricType& metric_type)
{

}

void UserInterface::Assert::time()
{

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
