#include "FileUtility.h"

Result FileUtility::Universal::save_data_in_file(const DescriptorRAII& descriptor_file, const str_t& data)
{
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

Result FileUtility::Config::parse_data_line(const str_t& data, map_t<str_t, str_t>& output_data)
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

Result FileUtility::Config::load(vec_t<ProcmonSettingsTable>& procmon_settings_table, DescriptorRAII& descriptor_file, map_t<str_t, str_t>& output_data, const char* file_name)
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
        str_t data = "";

        Result res_convert_to_str;
        if ((res_convert_to_str = ParseUtility::convert_procmon_settings_to_string(procmon_settings_table, data)) != Result::successful)
            return Result::failure;

        Result res_save_base_parameters;
        if ((res_save_base_parameters = Universal::save_data_in_file(descriptor_file, data)) != Result::successful)
            return res_save_base_parameters;

        return Result::successful;
    }

    if (!ReadFile(descriptor_file.get(), (void*)(buffer), BUFFER_SIZE, &REAL_BUFFER_SIZE, NULL))
        return Result::failure;

#elif defined (__linux__)
    int REAL_BUFFER_SIZE = 0;

    descriptor_file = open(file_name, O_CREAT | O_RDWR | O_EXCL, 00777);
    if (descriptor_file.get() == -1 && errno == EEXIST)
    {
        descriptor_file = open(file_name, O_RDWR, 00777);
        if(descriptor_file.get() == -1)
            return Result::failure;

        Result res_save_base_parameters;
        if ((res_save_base_parameters = save_config_in_file(procmon_settings_table, descriptor_file)) != Result::successful)
            return res_save_base_parameters;
    }
    if (descriptor_file.get() == -1)
        return Result::failure;

    if ((REAL_BUFFER_SIZE = read(descriptor_file.get(), buffer, BUFFER_SIZE)) == -1)
        return Result::failure;

#endif

    str_t data_file = str_t(buffer, REAL_BUFFER_SIZE);

    Result res_load_parameters;
    if ((res_load_parameters = Config::parse_data_line(data_file, output_data)) != Result::successful)
        return res_load_parameters;

    return Result::successful;
}

Result FileUtility::Universal::save(DescriptorRAII& descriptor_file, const str_t& data, const char* file_name)
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
    if ((res_save = save_data_in_file(descriptor_file, data)) != Result::successful)
        return res_save;

    return Result::successful;
}

Result FileUtility::Config::manage(vec_t<ProcmonSettingsTable>& procmon_settings_table, map_t<str_t, str_t>& file_config, const int param)
{
    DescriptorRAII descriptor_file{ };

    if (param == load_config_file)
    {
        const char* file_name = "procmon_config";

        Result res_load;
        if ((res_load = load(procmon_settings_table, descriptor_file, file_config, file_name)) != Result::successful)
            return res_load;
    }
    else if (param == save_config_file)
    {
        str_t file_name = "procmon_config";
        str_t data = "";

        Result res_convert_to_str;
        if((res_convert_to_str = ParseUtility::convert_procmon_settings_to_string(procmon_settings_table, data)) != Result::successful)
            return Result::failure;

        Result res_save;
        if ((res_save = Universal::save(descriptor_file, data, file_name.c_str())) != Result::successful)
            return res_save;
    }
    else return Result::failure;

    return Result::successful;
}

Result FileUtility::Data::manage(str_t& data, const int param)
{
    DescriptorRAII descriptor_file{ };

    if (param == load_data_process_file)
    {

    }
    else if (param == save_data_process_file)
    {
        str_t file_name = "processes_data";
        str_t data = "";

        Result res_save;
        if ((res_save = Universal::save(descriptor_file, data, file_name.c_str())) != Result::successful)
            return res_save;
    }
    else 
        return Result::failure;

    return Result::successful;
}