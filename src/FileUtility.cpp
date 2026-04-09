#include "FileUtility.h"

Result FileUtility::save_config_in_file(const ProcmonSettings& procmon_settings, const DescriptorRAII& descriptor_file)
{
    str_t data = "";

    ParseUtility::convert_procmon_settings_to_string(procmon_settings, data);

#ifdef _WIN32
    DWORD count_bytes = data.size() * sizeof(char);
    DWORD REAL_SIZE = 0;

    bool flag = WriteFile(descriptor_file.get(), data.c_str(), count_bytes, &REAL_SIZE, NULL);

    if (!flag) return Result::failure;

#elif defined(__linux__)
    int REAL_SIZE = 0;
    if ((REAL_SIZE = write(descriptor_file.get(), data.c_str(), data.size())) == -1)
        return Result::failure;

#endif 

    return Result::successful;
}

Result FileUtility::parse_config_line(ProcmonSettings& procmon_settings, const str_t& data, map_t<str_t, str_t>& output_data)
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

        start_line = end_line + 1;

        output_data[metrick] = setting;
    }

    return Result::successful;
}

Result FileUtility::load_config(ProcmonSettings& procmon_settings, DescriptorRAII& descriptor_file, map_t<str_t, str_t>& output_data, const char* file_name)
{
    const int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];

#ifdef _WIN32
    DWORD REAL_BUFFER_SIZE = 0;

    descriptor_file = CreateFile(file_name, GENERIC_WRITE | GENERIC_READ, 0,
        NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (descriptor_file.get() == INVALID_HANDLE_VALUE)
        return Result::failure;;

    if (GetLastError() != ERROR_ALREADY_EXISTS)
    {
        Result res_save_base_parameters;

        if ((res_save_base_parameters = save_config_in_file(procmon_settings, descriptor_file)) != Result::successful)
            return res_save_base_parameters;

        return Result::successful;
    }

    if (!ReadFile(descriptor_file.get(), (void*)(buffer), BUFFER_SIZE, &REAL_BUFFER_SIZE, NULL))
        return Result::failure;

#elif defined (__linux__)
    int REAL_BUFFER_SIZE = 0;

    descriptor_file = open(file_name, O_CREAT | O_RDWR, 00777);
    if (descriptor_file.get() == EEXIST)
    {
        Result res_save_base_parameters;
        if ((res_save_base_parameters = save_parameters_in_file(procmon_settings, descriptor_file)) != Result::successful)
            return res_save_base_parameters;
    }
    if (descriptor_file.get() == -1)
        return Result::failure;

    if ((REAL_BUFFER_SIZE = read(descriptor_file.get(), buffer, BUFFER_SIZE)) == -1)
        return Result::failure;

#endif

    Result res_load_parameters;
    str_t data_file = str_t(buffer, REAL_BUFFER_SIZE);
    if ((res_load_parameters = parse_config_line(procmon_settings, data_file, output_data)) != Result::successful)
        return res_load_parameters;

    return Result::successful;
}

Result FileUtility::save(const ProcmonSettings& procmon_settings, DescriptorRAII& descriptor_file, const char* file_name)
{
#ifdef _WIN32
    descriptor_file = CreateFile(file_name, GENERIC_WRITE | GENERIC_READ, 0,
        NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (descriptor_file.get() == INVALID_HANDLE_VALUE)
        return Result::failure;

#elif defined (__linux__)
    descriptor_file = open(file_name, O_RDWR | O_TRUNC, 00777);
    if (descriptor_file.get() == -1)
        return Result::failure;

#endif

    Result res_save;
    if ((res_save = save_config_in_file(procmon_settings, descriptor_file)) != Result::successful)
        return res_save;

    return Result::successful;
}

Result FileUtility::manage(ProcmonSettings& procmon_settings, map_t<str_t, str_t>& file_config, const int param)
{
    DescriptorRAII descriptor_file{ };

    const char* file_name = "procmon_config";

    if (param == load_file)
    {
        Result res_load;
        if ((res_load = load_config(procmon_settings, descriptor_file, file_config, file_name)) != Result::successful)
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