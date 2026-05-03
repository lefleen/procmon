#include "CommandProcessor.h"

Result CommandProcessor::manage(vec_t<ProcmonSettingsTable>& procmon_settings_table, ProcmonSettingsView& procmon_settings_view, const int argc, const char* argv[])
{ 
    size_t size = argc - 1;
    if (size == 0)
    {
        Result res_get_manage;
        if ((res_get_manage = Command::Get::manage(procmon_settings_table, "all")) != Result::successful)
            return res_get_manage;

        return Result::no_arguments;
    }
    else if (size < 0)
        return Result::failure;

    map_t<str_t, str_t> file_config;

    Result res_file_load;
    if ((res_file_load = FileUtility::Config::manage(procmon_settings_table, procmon_settings_view, file_config, FileUtility::load_config_file)) != Result::successful) 
        return res_file_load;

    Result res_set_array_data;
    if ((res_set_array_data = Command::Set::array_data(procmon_settings_table, procmon_settings_view, file_config)) != Result::successful)
        return res_set_array_data;

    file_config.clear();

    size_t index = 0;

    str_t option = "";
    str_t metrick = "";
    str_t setting = "";

    while (index < size)
    {
        Result res_parse_string;

        if ((res_parse_string = ParseUtility::parse_command_string(procmon_settings_table, argc, argv, option, metrick, setting, index)) != Result::successful)
            return res_parse_string;

        if (option == "help")
        {
            
        }
        else if (option == "set")
        {
            Result res;

            if ((res = Command::Set::manage(procmon_settings_table, procmon_settings_view, metrick, setting)) != Result::successful)
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
        if ((res_file_save = FileUtility::Config::manage(procmon_settings_table, procmon_settings_view, file_config, FileUtility::save_config_file)) != Result::successful)
            return res_file_save;
    }

    return Result::successful;
}

Result CommandProcessor::Command::Set::array_data(vec_t<ProcmonSettingsTable>& procmon_settings_table, ProcmonSettingsView& procmon_settings_view, const map_t<str_t, str_t>& file_config)
{
    Result res;
    for (auto& it : file_config)
    {
        if ((res = manage(procmon_settings_table, procmon_settings_view, it.first, it.second)) != Result::successful)
            return res;
    }
    
    return Result::successful;
}

Result CommandProcessor::Command::Set::manage(vec_t<ProcmonSettingsTable>& procmon_settings_table, ProcmonSettingsView& procmon_settings_view, const str_t& metrick, const str_t& setting)
{
    for (auto& it : procmon_settings_table)
    {
        if (it.name == metrick)
        {   
            if (it.metric_type == MetricType::time_view)
            {
                Result res_view_time;
                if((res_view_time = ParseUtility::set_settings_view_time_in_procmon_settings(setting, *procmon_settings_view.time_view_settings)) != Result::successful)
                    return res_view_time;
            }
            else if (it.metric_type == MetricType::memory_view)
            {
                Result res_view_mem;
                if((res_view_mem = ParseUtility::set_settings_view_memory_in_procmon_settings(setting, *procmon_settings_view.memory_view_settings)) != Result::successful)
                    return res_view_mem;;
            }
            else
            {
                if (setting == "on") *it.status = true;
                else if (setting == "off") *it.status = false;
                else return Result::invalid_arguments;
            }
            return Result::successful;
        }
    }

    return Result::invalid_arguments;
}

Result CommandProcessor::Command::Get::manage(const vec_t<ProcmonSettingsTable>& procmon_settings_table, const str_t& metrick)
{
    return Result::successful;
}
