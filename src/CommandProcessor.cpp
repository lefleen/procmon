#include "CommandProcessor.h"

Result CommandProcessor::manage(vec_t<ProcmonSettingsTable>& procmon_settings_table, ProcmonSettingsView& procmon_settings_view, const int argc, const char* argv[])
{ 
    size_t size = argc - 1;
    if (size == 0)
    {
        Result res_get_manage;
       if ((res_get_manage = Command::Get::manage(procmon_settings_table, procmon_settings_view, "all")) != Result::successful)
            return res_get_manage;

        return Result::no_arguments;
    }

    map_t<str_t, str_t> file_config;

    Result res_file_load;
    if ((res_file_load = FileUtility::Config::manage(procmon_settings_table, procmon_settings_view, file_config, FileUtility::load_config_file)) != Result::successful) 
    {
        UserInterface::Errors::show("", Result::err_load_config);
        return res_file_load;
    }

    Result res_set_array_data;
    if ((res_set_array_data = Command::Set::array_data(procmon_settings_table, procmon_settings_view, file_config)) != Result::successful)
        return res_set_array_data;

    file_config.clear();

    str_t option = "";
    str_t metric = "";
    str_t setting = "";
    bool status_set = false;

    Result res_parse_string;

    if ((res_parse_string = ParseUtility::parse_command_line(procmon_settings_table, argc, argv, option, metric, setting)) != Result::successful)
    {
        UserInterface::Errors::show("", Result::err_parse_command_line);
        return res_parse_string;
    }

    if (option == "help")
    {
        Result res_help_manage;

        if((res_help_manage = Command::Help::manage(metric)) != Result::successful)
            return res_help_manage;
    }
    else if (option == "set")
    {
        Result res;

        if ((res = Command::Set::manage(procmon_settings_table, procmon_settings_view, metric, setting)) != Result::successful)
            return res;

        if((res = Command::Get::manage(procmon_settings_table, procmon_settings_view, metric)) != Result::successful)
            return res;

        if (setting == "off" && metric == "time")
            UserInterface::Assert::time();

        status_set = true;
    }
    else if (option == "get")
    {    
        Result res;

        if ((res = Command::Get::manage(procmon_settings_table, procmon_settings_view, metric)) != Result::successful)
            return res;
    }

    Result res_file_save;
    if (status_set)
    {
        if ((res_file_save = FileUtility::Config::manage(procmon_settings_table, procmon_settings_view, file_config, FileUtility::save_config_file)) != Result::successful)
        {
            UserInterface::Errors::show("", Result::err_save_config);
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
                    UserInterface::Errors::show(setting, Result::invalid_arguments);
                    return res_view_time;
                }
            }
            else if (it.metric_type == MetricType::memory_view)
            {
                Result res_view_mem;
                if((res_view_mem = ParseUtility::set_settings_view_memory_in_procmon_settings(setting, *procmon_settings_view.memory_view_settings)) != Result::successful)
                {
                    UserInterface::Errors::show(setting, Result::invalid_arguments);
                    return res_view_mem;
                }
            }
            else
            {
                if (setting == "on") *it.status = true;
                else if (setting == "off") *it.status = false;
                else 
                {
                    UserInterface::Errors::show(setting, Result::invalid_arguments);
                    return Result::invalid_arguments;
                }
            }
            return Result::successful;
        }
    }

    return Result::invalid_arguments;
}

Result CommandProcessor::Command::Get::get_view_time_and_memory(const ProcmonSettingsView& procmon_settings_view, str_t& time_view, str_t& memory_view)
{   
    Result res_time_view;
    if((res_time_view = ParseUtility::set_setting_view_time_in_string(*procmon_settings_view.time_view_settings, time_view)) != Result::successful)
    {
        UserInterface::Errors::show("", Result::invalid_arguments);
        return res_time_view;
    }
    
    Result res_memory_view;
    if((res_memory_view = ParseUtility::set_settings_view_memory_in_string(*procmon_settings_view.memory_view_settings, memory_view)) != Result::successful)
    {
        UserInterface::Errors::show("", Result::invalid_arguments);
            return res_memory_view;
    }

    return Result::successful;    
}

Result CommandProcessor::Command::Get::data()
{
    str_t data = "";
    
    Result res_load_data;
    if((res_load_data = FileUtility::Data::manage(data, FileUtility::load_data_process_file)) != Result::successful)
    {
        UserInterface::Errors::show("", Result::err_load_data);
            return res_load_data;
    }

    UserInterface::Data::get(data);

    return Result::successful;
}

Result CommandProcessor::Command::Get::all(const vec_t<ProcmonSettingsTable>& procmon_settings_table, const ProcmonSettingsView& procmon_settings_view)
{
    str_t time_view = "";
    str_t memory_view = "";

    Result res_view_format;
    if((res_view_format = get_view_time_and_memory(procmon_settings_view, time_view, memory_view)) != Result::successful)
        return res_view_format;

    UserInterface::Metrics::all(procmon_settings_table, time_view, memory_view);

    return Result::successful;
}
Result CommandProcessor::Command::Get::one(const vec_t<ProcmonSettingsTable>& procmon_settings_table, const ProcmonSettingsView& procmon_settings_view, const str_t& metric)
{
    str_t time_view = "";
    str_t memory_view = "";

    Result res_view_format;
    if((res_view_format = get_view_time_and_memory(procmon_settings_view, time_view, memory_view)) != Result::successful)
        return res_view_format;

    bool is_valid_metric = false;

    for(auto& it : procmon_settings_table)
    {
        if(it.name == metric)
        {
            UserInterface::Metrics::one(it, it.metric_type, time_view, memory_view);

            is_valid_metric = true;
            break;
        }
    }
    if(!is_valid_metric)
    {
        UserInterface::Errors::show("", Result::invalid_arguments);
        return Result::failure;
    }

    return Result::successful;
}

Result CommandProcessor::Command::Get::manage(const vec_t<ProcmonSettingsTable>& procmon_settings_table, const ProcmonSettingsView& procmon_settings_view, const str_t& metric)
{
    if(metric == "data")
    {
        Result res_get_data;
        if((res_get_data = data()) != Result::successful)
            return res_get_data;
    }
    else if(metric == "all") 
    {
        Result res_get_all;
        if((res_get_all = all(procmon_settings_table, procmon_settings_view)) != Result::successful)
            return res_get_all;
    }
    else 
    {
       Result res_get_one;
       if((res_get_one = one(procmon_settings_table, procmon_settings_view, metric)) != Result::successful)
           return res_get_one;
    }

    return Result::successful;
}

Result CommandProcessor::Command::Help::manage(const str_t& setting)
{
    if(setting == "")
        UserInterface::Commands::help();
    else if(setting == "set")
        UserInterface::Commands::set();
    else if(setting == "get")
        UserInterface::Commands::get();
    else
    {
        UserInterface::Errors::show("", Result::invalid_arguments);
        return Result::invalid_arguments;
    }

    return Result::successful;
}
