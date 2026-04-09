#ifdef _WIN32
#include <Windows.h>
#elif defined(__linux__)
#include <fcntl.h>
#include <unistd.h>

#endif

#include "types.h"
#include "settingstypes.h"
#include "DescriptorRAII.h"
#include "ParseUtility.h"

namespace FileUtility
{
    const int load_config_file = 100;
    const int save_config_file = 200;
    const int save_data_process_file = 300;

    Result manage(ProcmonSettings& procmon_settings, map_t<str_t, str_t>& file_config, const int param);

    Result load_config(ProcmonSettings& procmon_settings, DescriptorRAII& descriptor_file, map_t<str_t, str_t>& output_data, const char* file_name);

    Result save(const ProcmonSettings& procmon_settings, DescriptorRAII& descriptor_file, const char* file_name);

    Result save_config_in_file(const ProcmonSettings& procmon_settings, const DescriptorRAII& descriptor_file);

    Result parse_config_line(ProcmonSettings& procmon_settings, const str_t& data, map_t<str_t, str_t>& output_data);
}