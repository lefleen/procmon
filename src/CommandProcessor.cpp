#include "CommandProcessor.h"

Result CommandProcessor::manage(ProcmonSettings& procmon_settings, const int argc, const char* argv[])
{
    size_t size = argc - 1;

    if (size == 0) 
        return Result::no_arguments;
    else if (size < 0) 
        return Result::failure;

    size_t index = 0;

    str_t option = "";
    str_t metrick = "";
    str_t setting = "";

    Result res_file_load;
    if ((res_file_load = FileUtility::load(procmon_settings)) != Result::successful) 
        return res_file_load;

    while (index < size)
    {
        Result res_parse_string;

        if ((res_parse_string = ParseUtility::parse_string(procmon_settings, argc, argv, option, metrick, setting, index)) != Result::successful)
            return res_parse_string;

        if (option == "--help")
        {
            UserInterface::ShowHelp::full_help();
        }
        else if (option == "set")
        {
            Result res;

            if ((res = Command::Set::manage(procmon_settings, metrick, setting)) != Result::successful)
                return res;
        }
        else if (option == "get")
        {    
            Result res;

            if ((res = Command::Get::manage(procmon_settings, metrick)) != Result::successful)
                return res;
        }
    }

    Result res_file_save;
    if(option == "set") 
        if((res_file_save = FileUtility::save(procmon_settings)) != Result::successful) 
            return res_file_save;

    return Result::successful;
}

Result CommandProcessor::ParseUtility::convert_char_to_string(vec_t<str_t>& res, const int argc, const char* argv[])
{
    for(int index = 1; index < argc; ++index)
    {
        res[index - 1] = str_t(argv[index]);
    }

    return Result::successful;
}

Result CommandProcessor::ParseUtility::parse_string(ProcmonSettings& procmon_settings, const int argc, const char* argv[], str_t& option, str_t& metrick, str_t& setting, size_t& index)
{
    vec_t<str_t> args(argc - 1);

    if (convert_char_to_string(args, argc, argv) == Result::failure) 
        return Result::failure;

    const size_t size = args.size();

    option = args[index];

    if (option == "--help")
    {
        index += 1;
    }
    else if (option == "set")
    {
        if (index + 2 >= size) 
            return Result::invalid_arguments;

        metrick = args[index + 1];
        setting = args[index + 2];

        index += 3;
    }
    else if (args[index] == "get")
    {
        if (index + 1 >= size) 
            return Result::invalid_arguments;

        metrick = args[index + 1];

        index += 2;
    }
    else return Result::invalid_arguments;

    return Result::successful;
}

Result CommandProcessor::FileUtility::load(ProcmonSettings& procmon_settings)
{
    return Result::successful;
}

Result CommandProcessor::FileUtility::save(const ProcmonSettings& procmon_settings)
{
    return Result::successful;
}

Result CommandProcessor::Command::Set::manage(ProcmonSettings& procmon_settings, const str_t& metrick, const str_t& setting) 
{
    Result res;
    if (metrick == "name") res = Name::set(procmon_settings, setting);
    else if (metrick == "time") res = Time::set(procmon_settings, setting);
    else if (metrick == "memory") res = Memory::set(procmon_settings, setting);
    else if (metrick == "total_cpu") res = TotalCPU::set(procmon_settings, setting);
    else if (metrick == "interval_cpu") res = IntervalCPU::set(procmon_settings, setting);
    else return Result::invalid_arguments;

    if (res != Result::successful) 
        return res;

    return Result::successful;
}

Result CommandProcessor::Command::Get::manage(const ProcmonSettings& procmon_settings, const str_t& metrick)
{
    if (metrick == "name") UserInterface::ShowSettings::name(procmon_settings.name);
    else if (metrick == "time") UserInterface::ShowSettings::time(procmon_settings.time);
    else if (metrick == "memory") UserInterface::ShowSettings::memory(procmon_settings.memory);
    else if (metrick == "total_cpu") UserInterface::ShowSettings::total_cpu(procmon_settings.total_cpu);
    else if (metrick == "interval_cpu") UserInterface::ShowSettings::interval_cpu(procmon_settings.interval_cpu);
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
    else if (setting == "off")
    {
        UserInterface::ShowAssert::time_off();
        procmon_settings.time = false;
    }
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