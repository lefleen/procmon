#include "types.h"
#include "settings.h"

namespace ParseUtility
{
    Result convert_char_to_string(vec_t<str_t>& res, const int argc, const char* argv[]);

    Result parse_command_line(vec_t<ProcmonSettingsTable>& procmon_settings_table, const int argc, const char* argv[], str_t& option, str_t& metrick, str_t& setting);

    Result set_setting_view_time_in_string(const TimeViewSettings& time_view_settings, str_t& out);

    Result set_settings_view_memory_in_string(const MemoryViewSettings& memory_view_settings, str_t& out);

    Result convert_procmon_settings_to_string(const vec_t<ProcmonSettingsTable>& procmon_settings_table, const ProcmonSettingsView& procmon_settings_view, str_t& out);

    Result set_settings_view_time_in_procmon_settings(const str_t& setting, TimeViewSettings& time_view_settings);

    Result set_settings_view_memory_in_procmon_settings(const str_t& setting, MemoryViewSettings& memory_view_settings);
}
