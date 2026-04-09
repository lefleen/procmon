#include "ParseUtility.h"

Result ParseUtility::convert_char_to_string(vec_t<str_t>& res, const int argc, const char* argv[])
{
    for (int index = 1; index < argc; ++index)
    {
        res[index - 1] = str_t(argv[index]);
    }

    return Result::successful;
}

Result ParseUtility::parse_command_string(ProcmonSettings& procmon_settings, const int argc, const char* argv[], str_t& option, str_t& metrick, str_t& setting, size_t& index)
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

Result ParseUtility::convert_procmon_settings_to_string(const ProcmonSettings& procmon_settings, str_t& out)
{
    if (procmon_settings.name) out = "name:on;\n";
    else out = "name:off;\n";

    if (procmon_settings.time) out += "time:on;\n";
    else out += "time:off;\n";

    if (procmon_settings.memory) out += "memory:on;\n";
    else out += "memory:off;\n";

    if (procmon_settings.total_cpu) out += "totalCPU:on;\n";
    else out += "totalCPU:off;\n";

    if (procmon_settings.interval_cpu) out += "intervalCPU:on;\n";
    else out += "intervalCPU:off;\n";

    return Result::successful;
}