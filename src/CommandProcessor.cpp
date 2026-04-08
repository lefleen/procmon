#include "CommandProcessor.h"

Result CommandProcessor::manage(ProcmonSettings& procmon_settings, const int argc, const char* argv[])
{ 
    Result res_file_load;

    if ((res_file_load = FileUtility::manage(procmon_settings, FileUtility::load_file)) != Result::successful) 
        return res_file_load;

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

        if ((res_parse_string = ParseUtility::parse_string(procmon_settings, argc, argv, option, metrick, setting, index)) != Result::successful)
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
        if ((res_file_save = FileUtility::manage(procmon_settings, FileUtility::save_file)) != Result::successful)
            return res_file_save;
    }

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

Result CommandProcessor::ParseUtility::convert_settings_to_string(const ProcmonSettings& procmon_settings, str_t& out)
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

Result CommandProcessor::FileUtility::save_parameters_in_file(const ProcmonSettings& procmon_settings, const descriptor_process_t h_file)
{
    str_t data = "";

    ParseUtility::convert_settings_to_string(procmon_settings, data);

#ifdef _WIN32
    DWORD REAL_SIZE = 0;
    DWORD count_bytes = data.size() * sizeof(char);

    bool flag = WriteFile(h_file, data.c_str(), count_bytes, &REAL_SIZE, NULL);

    if (!flag) return Result::failure;

#elif defined(__linux__)

#endif

    return Result::successful;
}

Result CommandProcessor::FileUtility::load_parameters(ProcmonSettings& procmon_settings, const str_t& data)
{
    size_t start_line = 0;
    
    while (start_line < data.size())
    {
        Result res_set_manage;

        size_t end_line = data.find("\n", start_line);

        str_t line = data.substr(start_line, end_line - start_line);
        
        size_t end_metrick = line.find(":", 0);
        size_t end_setting = line.find(";", end_metrick + 1);

        str_t metrick = line.substr(0, end_metrick);
        str_t setting = line.substr(end_metrick + 1, end_setting - end_metrick - 1);
        if (end_setting == str_t::npos || end_metrick == str_t::npos) break;

        if((res_set_manage = Command::Set::manage(procmon_settings, metrick, setting)) != Result::successful)
            return res_set_manage;

        start_line = end_line + 1;
    }

    return Result::successful;
}

Result CommandProcessor::FileUtility::load(ProcmonSettings& procmon_settings, DescriptorRAII& descriptor_file, const char* file_name) 
{
    const int BUFFER_SIZE = 1024;
    int REAL_BUFFER_SIZE = 0;
    char buffer[BUFFER_SIZE];

#ifdef _WIN32
    descriptor_file = CreateFile(file_name, GENERIC_WRITE | GENERIC_READ, 0,
        NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (descriptor_file.get() == INVALID_HANDLE_VALUE)
        return Result::failure;;

    if (GetLastError() != ERROR_ALREADY_EXISTS)
    {
        Result res_save_base_parameters;

        if ((res_save_base_parameters = save_parameters_in_file(procmon_settings, descriptor_file.get())) != Result::successful)
            return res_save_base_parameters;

        return Result::successful;
    }

    if (!ReadFile(descriptor_file.get(), (void*)(buffer), BUFFER_SIZE, &REAL_BUFFER_SIZE, NULL))
        return Result::failure;

#elif defined (__linux__)
    descriptor_file = open(file_name, O_CREAT | O_RDWR, 00777);
    if(descriptor_file.get() == -1)
        return Result::failure;

    if((REAL_BUFFER_SIZE = read(descriptor_file.get(), buffer, BUFFER_SIZE)) == -1)
        return Result::failure;

#endif

    Result res_load_parameters;
    str_t data = str_t(buffer, REAL_BUFFER_SIZE);
    if ((res_load_parameters = load_parameters(procmon_settings, data)) != Result::successful)
        return res_load_parameters;

    return Result::successful;
}

Result CommandProcessor::FileUtility::save(const ProcmonSettings& procmon_settings, DescriptorRAII& descriptor_file, const char* file_name)
{
#ifdef _WIN32
    descriptor_file = CreateFile(file_name, GENERIC_WRITE | GENERIC_READ, 0,
        NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (descriptor_file.get() == INVALID_HANDLE_VALUE)
        return Result::failure;

#elif defined (__linux__)

#endif

    Result res_save;
    if ((res_save = save_parameters_in_file(procmon_settings, descriptor_file.get())) != Result::successful)
        return res_save;

    return Result::successful;
}

Result CommandProcessor::FileUtility::manage(ProcmonSettings& procmon_settings, const int param)
{
    DescriptorRAII descriptor_file{ };

    const char* file_name = "procmon_config";
    
    if (param == load_file)
    {
        Result res_load;
        if ((res_load = load(procmon_settings, descriptor_file, file_name)) != Result::successful)
            return res_load;
    }
    else if (param == save_file)

    {
        Result res_save;
        if ((res_save = save(procmon_settings, descriptor_file, file_name)) != Result::successful)
            return res_save;
    }
    else return Result::failure;

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
