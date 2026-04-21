#include "ParseUtility.h"

Result ParseUtility::convert_char_to_string(vec_t<str_t>& res, const int argc, const char* argv[])
{
    for (int index = 1; index < argc; ++index)
    {
        res[index - 1] = str_t(argv[index]);
    }

    return Result::successful;
}

Result ParseUtility::parse_command_string(vec_t<ProcmonSettingsTable>& procmon_settings_table, const int argc, const char* argv[], str_t& option, str_t& metrick, str_t& setting, size_t& index)
{
    vec_t<str_t> args(argc - 1);

    if (convert_char_to_string(args, argc, argv) == Result::failure)
        return Result::failure;

    const size_t size = args.size();

    option = args[index];

    if (option == "help")
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

Result ParseUtility::convert_procmon_settings_to_string(const vec_t<ProcmonSettingsTable>& procmon_settings_table, str_t& out)
{
    out = "";

    for (auto& it : procmon_settings_table)
    {
        out += it.name + ":";
        if (*it.status == true) out += "on";
        else out += "off";
        out += ";\n";
    }

    return Result::successful;
}