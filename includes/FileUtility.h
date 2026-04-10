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

    Result manage(vec_t<ProcmonSettingsTable>& procmon_settings_table, map_t<str_t, str_t>& file_config, const int param);

    Result load_config(vec_t<ProcmonSettingsTable>& procmon_settings_table, DescriptorRAII& descriptor_file, map_t<str_t, str_t>& output_data, const char* file_name);

    Result save(const vec_t<ProcmonSettingsTable>& procmon_settings_table, DescriptorRAII& descriptor_file, const char* file_name);

    Result save_config_in_file(const vec_t<ProcmonSettingsTable>& procmon_settings_table, const DescriptorRAII& descriptor_file);

    Result parse_config_line(vec_t<ProcmonSettingsTable>& procmon_settings_table, const str_t& data, map_t<str_t, str_t>& output_data);
}