#include "CommandProcessor.h"
#include <iostream>

Result CommandProcessor::manage(vec_t<ProcmonSettingsTable>& procmon_settings_table, const int argc, const char* argv[])
{ 
    map_t<str_t, str_t> file_config;

    Result res_file_load;
    if ((res_file_load = FileUtility::manage(procmon_settings_table, file_config, FileUtility::load_config_file)) != Result::successful) 
        return res_file_load;
    Result res_set_array_data;
    if ((res_set_array_data = Command::Set::array_data(procmon_settings_table, file_config)) != Result::successful)
        return Result::successful;

    file_config.clear();

    size_t size = argc - 1;
    if (size == 0)
    {
        Result res_get_manage;
        if ((res_get_manage = Command::Get::manage(procmon_settings_table, "all")) != Result::successful)
            return res_get_manage;

        return Result::no_arguments;
    }
    else if (size < 0) 
    {
        return Result::failure;
    }

    size_t index = 0;

    str_t option = "";
    str_t metrick = "";
    str_t setting = "";

    while (index < size)
    {
        Result res_parse_string;
        std::cout << "123" << std::endl;
        if ((res_parse_string = ParseUtility::parse_command_string(procmon_settings_table, argc, argv, option, metrick, setting, index)) != Result::successful)
            return res_parse_string;

        if (option == "help")
        {
            
        }
        else if (option == "set")
        {
            Result res;

            if ((res = Command::Set::manage(procmon_settings_table, metrick, setting)) != Result::successful)
                return res;

            if((res = Command::Get::manage(procmon_settings_table, metrick)) != Result::successful)
                return res;

            if (setting == "off" && metrick == "time");
                
        }
        else if (option == "get")
        {    
            Result res;

            if ((res = Command::Get::manage(procmon_settings_table, metrick)) != Result::successful)
                return res;
        }
    }

    Result res_file_save;
    if (option == "set")
    {
        if ((res_file_save = FileUtility::manage(procmon_settings_table, file_config, FileUtility::save_config_file)) != Result::successful)
            return res_file_save;
    }

    return Result::successful;
}

Result CommandProcessor::Command::Set::array_data(vec_t<ProcmonSettingsTable>& procmon_settings_table, const map_t<str_t, str_t>& file_config)
{
    Result res;
    for (auto& itr : file_config)
    {
        if((res = manage(procmon_settings_table, itr.first, itr.second)) != Result::successful)
            return Result::failure;
    }

    return Result::successful;
}

Result CommandProcessor::Command::Set::manage(vec_t<ProcmonSettingsTable>& procmon_settings_table, const str_t& metrick, const str_t& setting)
{
    for (auto& it : procmon_settings_table)
    {
        if (it.name == metrick)
        {
            if (setting == "on") *it.status = true;
            else *it.status = false;
            return Result::successful;
        }
    }

    return Result::invalid_arguments;
}

Result CommandProcessor::Command::Get::manage(const vec_t<ProcmonSettingsTable>& procmon_settings_table, const str_t& metrick)
{
    return Result::successful;
}
