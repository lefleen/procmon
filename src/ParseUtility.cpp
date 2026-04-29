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

Result ParseUtility::set_setting_view_time_in_string(const TimeViewSettings& time_view_settings, str_t& out)
{
    switch (time_view_settings)
    {
    case TimeViewSettings::seconds: out += "seconds";
        break;

    case TimeViewSettings::minutes: out += "minutes";
        break;

    case TimeViewSettings::hours: out += "hours";
        break;

    case TimeViewSettings::days: out += "days";
        break;

    default: 
        return Result::failure;
    }
};

Result ParseUtility::set_settings_view_memory_in_string(const MemoryViewSettings& memory_view_settings, str_t& out) 
{
    switch (memory_view_settings)
    {
    case MemoryViewSettings::B: out += "B";
        break;

    case MemoryViewSettings::KiB: out += "KiB";
        break;

    case MemoryViewSettings::MiB: out += "MiB";
        break;

    case MemoryViewSettings::GiB: out += "GiB";
        break;

    case MemoryViewSettings::TiB: out += "TiB";
        break;

    default: 
        return Result::failure;
    }
};

Result ParseUtility::convert_procmon_settings_to_string(const vec_t<ProcmonSettingsTable>& procmon_settings_table, const ProcmonSettingsView& procmon_settings_view, str_t& out)
{
    for (auto& it : procmon_settings_table)
    {
        out += it.name + ":";
        if (it.name == "time_view")
        {
            Result res_set_time;
            if((res_set_time = set_setting_view_time_in_string(*procmon_settings_view.time_view_settings, out)) != Result::successful)
                return res_set_time;
        }
        else if (it.name == "memory_view")
        {
            Result res_set_mem;
            if((res_set_mem = set_settings_view_memory_in_string(*procmon_settings_view.memory_view_settings, out)) != Result::successful)
                return res_set_mem;
        }
        else
        {
            if (*it.status == true) out += "on";
            else out += "off";
        }
        out += ";\n";
    }

    return Result::successful;
}

Result ParseUtility::set_settings_view_time_in_procmon_settings(const str_t& setting, TimeViewSettings& time_view_settings) 
{
    if (setting == "seconds")
        time_view_settings = TimeViewSettings::seconds;
    else if (setting == "minutes")
        time_view_settings = TimeViewSettings::minutes;
    else if (setting == "hours")
        time_view_settings = TimeViewSettings::hours;
    else if (setting == "days")
        time_view_settings = TimeViewSettings::days;
    else
        return Result::failure;
}

Result ParseUtility::set_settings_view_memory_in_procmon_settings(const str_t& setting, MemoryViewSettings& memory_view_settings)
{
    if (setting == "B")
        memory_view_settings = MemoryViewSettings::B;
    else if (setting == "KiB")
        memory_view_settings = MemoryViewSettings::KiB;
    else if (setting == "MiB")
        memory_view_settings = MemoryViewSettings::MiB;
    else if (setting == "GiB")
        memory_view_settings = MemoryViewSettings::GiB;
    else if (setting == "TiB")
        memory_view_settings = MemoryViewSettings::TiB;
    else
        return Result::failure;
}