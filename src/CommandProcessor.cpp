#include "CommandProcessor.h"
#include <iostream>

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
            /*UserInterface::show_help()*/;
            return Result::successful;
        }

        if(option == "set")
        {
            if(index + 2 >= size) return Result::invalid_arguments;

            const str_t metrick = args[index + 1];
            const str_t setting = args[index + 2];

            if(metrick == "name" && Name::set(procmon_settings, setting) == Result::invalid_arguments) return Result::invalid_arguments;
            else return Result::invalid_arguments;

            index += 2;
            continue;
        }

        else if(args[index] == "get")
        {
            if(index + 1 >= size) return Result::invalid_arguments;

            const str_t metrick = args[index + 1];

            if(metrick == "name") /*UserInterface::show_name_settings()*/;
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

