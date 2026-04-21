#pragma once

#ifdef _WIN32
#include <Windows.h>
#elif defined(__linux__)
#include <fcntl.h>
#include <unistd.h>

#endif

#include "types.h"
#include "settings.h"
#include "DescriptorRAII.h"
#include "ParseUtility.h"

namespace FileUtility
{
    const int load_config_file = 100;
    const int save_config_file = 200;
    const int save_data_process_file = 300;
    const int load_data_process_file = 400;

    namespace Config
    {
        Result load(vec_t<ProcmonSettingsTable>& procmon_settings_table, DescriptorRAII& descriptor_file, map_t<str_t, str_t>& output_data, const char* file_name);

        Result parse_data_line(const str_t& data, map_t<str_t, str_t>& output_data);

        Result manage(vec_t<ProcmonSettingsTable>& procmon_settings_table, map_t<str_t, str_t>& file_config, const int param);
    }

    namespace Data 
    {
        Result manage(str_t& data, const int param);
    }

    namespace Universal
    { 
        Result save_data_in_file(const DescriptorRAII& descriptor_file, const str_t& data);

        Result save(DescriptorRAII& descriptor_file, const str_t& data, const char* file_name);
    }
}