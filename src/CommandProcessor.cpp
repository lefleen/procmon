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
    {
        UserInterface::Errors::show("open file 'procmon_config'", res_file_load);
        return res_file_load;
    }

    Result res_set_array_data;
    if ((res_set_array_data = Command::Set::array_data(procmon_settings_table, procmon_settings_view, file_config)) != Result::successful)
        return res_set_array_data;

    file_config.clear();

    size_t index = 0;

    str_t option = "";
    str_t metric = "";
    str_t setting = "";

    while (index < size)
    {
        Result res_parse_string;

        if ((res_parse_string = ParseUtility::parse_command_line(procmon_settings_table, argc, argv, option, metric, setting, index)) != Result::successful)
        {
            UserInterface::Errors::show("parse command line", res_parse_string);
            return res_parse_string;
        }

        if (option == "help")
        {
            
        }
        else if (option == "set")
        {
            Result res;

            if ((res = Command::Set::manage(procmon_settings_table, procmon_settings_view, metric, setting)) != Result::successful)
                return res;

            if((res = Command::Get::manage(procmon_settings_table, metric)) != Result::successful)
                return res;

            if (setting == "off" && metric == "time");
        }
        else if (option == "get")
        {    
            Result res;

            if ((res = Command::Get::manage(procmon_settings_table, metric)) != Result::successful)
                return res;
        }
    }

    Result res_file_save;
    if (option == "set")
    {
        if ((res_file_save = FileUtility::Config::manage(procmon_settings_table, procmon_settings_view, file_config, FileUtility::save_config_file)) != Result::successful)
        {
            UserInterface::Errors::show("open file 'processes data'", res_file_save);
            return res_file_save;
        }
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
                {
                    UserInterface::Errors::show(("undefined format: " + setting), res_view_time);
                    return res_view_time;
                }
            }
            else if (it.metric_type == MetricType::memory_view)
            {
                Result res_view_mem;
                if((res_view_mem = ParseUtility::set_settings_view_memory_in_procmon_settings(setting, *procmon_settings_view.memory_view_settings)) != Result::successful)
                {
                    UserInterface::Errors::show(("undefined format: " + setting), res_view_mem);
                    return res_view_mem;;
                }
            }
            else
            {
                if (setting == "on") *it.status = true;
                else if (setting == "off") *it.status = false;
                else 
                {
                    UserInterface::Errors::show(("undefined format: " + setting), Result::invalid_arguments);
                    return Result::invalid_arguments;
                }
            }
            return Result::successful;
        }
    }

    return Result::invalid_arguments;
}

Result CommandProcessor::Command::Get::manage(const vec_t<ProcmonSettingsTable>& procmon_settings_table, const str_t& metric)
{
    if(metric == "data")
    {
        str_t data = "";

        Result res_load_data;
        if((res_load_data = FileUtility::Data::manage(data, FileUtility::load_data_process_file)) != Result::successful)
        {
            UserInterface::Errors::show("open file 'processes data'", res_load_data);
            return res_load_data;
        }
    }

    return Result::successful;
}
