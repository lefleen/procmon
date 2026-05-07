#pragma once

#include <iostream>

#include "types.h"
#include "settings.h"
#include "UserInterface.h"
#include "DescriptorRAII.h"
#include "FileUtility.h"

namespace CommandProcessor
{
    Result manage(vec_t<ProcmonSettingsTable>& procmon_settings_table, ProcmonSettingsView& procmon_settings_view, const int argc, const char* argv[]);

    namespace Command
    {
        namespace Set
        {
            Result array_data(vec_t<ProcmonSettingsTable>& procmon_settings_table, ProcmonSettingsView& procmon_settings_view, const map_t<str_t, str_t>& file_config);

            Result manage(vec_t<ProcmonSettingsTable>& procmon_settings_table, ProcmonSettingsView& procmon_settings_view, const str_t& metrick, const str_t& setting);
        }

        namespace Get
        {
            Result get_view_time_and_memory(const ProcmonSettingsView& procmon_settings_view, str_t& time_view, str_t& memory_view);

            Result data();

            Result all(const vec_t<ProcmonSettingsTable>& procmon_settings_table, const ProcmonSettingsView& procmon_settings_view);

            Result one(const vec_t<ProcmonSettingsTable>& procmon_settings_table, const ProcmonSettingsView& procmon_settings_view, const str_t& metric);

            Result manage(const vec_t<ProcmonSettingsTable>& procmon_settings_table, const ProcmonSettingsView& procmon_settings_view, const str_t& metrick);
        }

        namespace Help
        {
            Result manage(const str_t& setting);
        }
    }
}
