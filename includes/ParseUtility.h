#include "types.h"
#include "settings.h"

namespace ParseUtility
{
    Result convert_char_to_string(vec_t<str_t>& res, const int argc, const char* argv[]);

    Result parse_command_string(vec_t<ProcmonSettingsTable>& procmon_settings_table, const int argc, const char* argv[], str_t& option, str_t& metrick, str_t& setting, size_t& index);

    Result convert_procmon_settings_to_string(const vec_t<ProcmonSettingsTable>& procmon_settings_table, str_t& out);
}