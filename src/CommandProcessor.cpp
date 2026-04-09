#include "CommandProcessor.h"

Result CommandProcessor::manage(ProcmonSettings& procmon_settings, const int argc, const char* argv[])
{ 
    map_t<str_t, str_t> file_config;

    Result res_file_load;
    if ((res_file_load = FileUtility::manage(procmon_settings, file_config, FileUtility::load_file)) != Result::successful) 
        return res_file_load;

    Result res_set_array_data;
    if ((res_set_array_data = Command::Set::array_data(procmon_settings, file_config)) != Result::successful)
        return Result::successful;

    file_config.clear();


    size_t size = argc - 1;
    if (size == 0)
    {
        Result res_get_manage;
        if ((res_get_manage = Command::Get::manage(procmon_settings, "all")) != Result::successful)
            return res_get_manage;

        return Result::no_arguments;
    }
    else if (size < 0) 
        return Result::failure;

    size_t index = 0;

    str_t option = "";
    str_t metrick = "";
    str_t setting = "";

    while (index < size)
    {
        Result res_parse_string;

        if ((res_parse_string = ParseUtility::parse_command_string(procmon_settings, argc, argv, option, metrick, setting, index)) != Result::successful)
            return res_parse_string;
        
        if (option == "help")
        {
            UserInterface::ShowHelp::full_help();
        }
        else if (option == "set")
        {
            Result res;

            if ((res = Command::Set::manage(procmon_settings, metrick, setting)) != Result::successful)
                return res;

            if((res = Command::Get::manage(procmon_settings, metrick)) != Result::successful)
                return res;

            if (setting == "off" && metrick == "time")
                UserInterface::ShowAssert::time_off();
        }
        else if (option == "get")
        {    
            Result res;

            if ((res = Command::Get::manage(procmon_settings, metrick)) != Result::successful)
                return res;
        }
    }

    Result res_file_save;
    if (option == "set")
    {
        if ((res_file_save = FileUtility::manage(procmon_settings, file_config, FileUtility::save_file)) != Result::successful)
            return res_file_save;
    }

    return Result::successful;
}

Result CommandProcessor::Command::Set::array_data(ProcmonSettings& procmon_settings, const map_t<str_t, str_t>& file_config)
{
    Result res;
    for (auto& itr : file_config)
    {
        if((res = manage(procmon_settings, itr.first, itr.second)) != Result::successful)
            return Result::failure;
    }

    return Result::successful;
}

Result CommandProcessor::Command::Set::manage(ProcmonSettings& procmon_settings, const str_t& metrick, const str_t& setting) 
{
    Result res;
    if (metrick == "name") res = Name::set(procmon_settings, setting);
    else if (metrick == "time") res = Time::set(procmon_settings, setting);
    else if (metrick == "memory") res = Memory::set(procmon_settings, setting);
    else if (metrick == "totalCPU") res = TotalCPU::set(procmon_settings, setting);
    else if (metrick == "intervalCPU") res = IntervalCPU::set(procmon_settings, setting);
    else return Result::invalid_arguments;

    if (res != Result::successful) 
        return res;

    return Result::successful;
}

Result CommandProcessor::Command::Get::programm_config(const ProcmonSettings& procmon_settings, const str_t& metrick)
{
    UserInterface::ShowSettings::name(procmon_settings.name);
    UserInterface::ShowSettings::time(procmon_settings.time);
    UserInterface::ShowSettings::memory(procmon_settings.memory);
    UserInterface::ShowSettings::total_cpu(procmon_settings.total_cpu);
    UserInterface::ShowSettings::interval_cpu(procmon_settings.interval_cpu);

    return Result::successful;
}

Result CommandProcessor::Command::Get::manage(const ProcmonSettings& procmon_settings, const str_t& metrick)
{
    if (metrick == "name") UserInterface::ShowSettings::name(procmon_settings.name);
    else if (metrick == "time") UserInterface::ShowSettings::time(procmon_settings.time);
    else if (metrick == "memory") UserInterface::ShowSettings::memory(procmon_settings.memory);
    else if (metrick == "totalCPU") UserInterface::ShowSettings::total_cpu(procmon_settings.total_cpu);
    else if (metrick == "intervalCPU") UserInterface::ShowSettings::interval_cpu(procmon_settings.interval_cpu);
    else if (metrick == "all") programm_config(procmon_settings, metrick);
    else return Result::invalid_arguments;

    return Result::successful;
}

Result CommandProcessor::Name::set(ProcmonSettings& procmon_settings, const str_t& setting)
{
    if (setting == "on") procmon_settings.name = true;
    else if(setting == "off") procmon_settings.name = false;
    else  return Result::invalid_arguments;

    return Result::successful;
}

Result CommandProcessor::Time::set(ProcmonSettings& procmon_settings, const str_t& setting)
{
    if (setting == "on") procmon_settings.time = true;
    else if (setting == "off") procmon_settings.time = false;
    else  return Result::invalid_arguments;

    return Result::successful;
}

Result CommandProcessor::Memory::set(ProcmonSettings& procmon_settings, const str_t& setting)
{
    if (setting == "on") procmon_settings.memory= true;
    else if (setting == "off") procmon_settings.memory = false;
    else  return Result::invalid_arguments;

    return Result::successful;
}

Result CommandProcessor::TotalCPU::set(ProcmonSettings& procmon_settings, const str_t& setting)
{
    if (setting == "on") procmon_settings.total_cpu = true;
    else if (setting == "off") procmon_settings.total_cpu = false;
    else  return Result::invalid_arguments;

    return Result::successful;
}

Result CommandProcessor::IntervalCPU::set(ProcmonSettings& procmon_settings, const str_t& setting)
{
    if (setting == "on") procmon_settings.interval_cpu = true;
    else if (setting == "off") procmon_settings.interval_cpu = false;
    else  return Result::invalid_arguments;

    return Result::successful;
}
