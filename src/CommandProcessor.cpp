#include "CommandProcessor.h"

Result CommandProcessor::convert_char_to_string(vec_t<str_t>& res, const int argc, const char* argv[])
{
    for(int index = 1; index < argc; ++index)
    {
        res[index - 1] = str_t(argv[index]);
    }

    return Result::successful;
}

Result CommandProcessor::parse_string(ProcmonSettings& procmon_settings, const int argc, const char* argv[])
{
    if(argc - 1 <= 0) return Result::no_arguments; 

    vec_t<str_t> args(argc - 1);
    if(convert_char_to_string(args, argc, argv) == Result::failure) return Result::failure;
    const size_t size = args.size();

    for(size_t index = 0; index < size; ++index)
    {
        const str_t option = args[index];

        if(option == "--help") 
        {
            UserInterface::ShowHelp::all();
            return Result::successful;
        }

        if(option == "set")
        {
            if(index + 2 >= size) return Result::invalid_arguments;

            Result res;
            const str_t metrick = args[index + 1];
            const str_t setting = args[index + 2];

            if (metrick == "name") res = Name::set(procmon_settings, setting);
            else if (metrick == "time") res = Time::set(procmon_settings, setting);
            else if (metrick == "memory") res = Memory::set(procmon_settings, setting);
            else if (metrick == "total_cpu") res = TotalCPU::set(procmon_settings, setting);
            else if (metrick == "interval_cpu") res = IntervalCPU::set(procmon_settings, setting);
            else return Result::invalid_arguments;
            if (res == Result::invalid_arguments) return res;

            index += 2;
            continue;
        }

        else if(args[index] == "get")
        {
            if(index + 1 >= size) return Result::invalid_arguments;

            const str_t metrick = args[index + 1];

            if (metrick == "name") UserInterface::ShowSettings::name();
            else if (metrick == "time") UserInterface::ShowSettings::time();
            else if (metrick == "memory") UserInterface::ShowSettings::memory();
            else if (metrick == "total_cpu") UserInterface::ShowSettings::total_cpu();
            else if (metrick == "interval_cpu") UserInterface::ShowSettings::interval_cpu();
            else return Result::invalid_arguments;

            index += 1;
            continue;
        }

        else return Result::invalid_arguments;
    }

    return Result::successful;
}

Result CommandProcessor::Name::set(ProcmonSettings& procmon_settings, const str_t& setting)
{
    if(setting == "on") procmon_settings.name = true;
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