#pragma once

#include "types.h"

struct ProcmonSettings
{
    bool name = true;
    bool time = true;
    bool memory = true;
    bool total_cpu = true;
    bool interval_cpu = true;
};

struct ProcmonSettingsTable 
{
    str_t name;
    bool* status;
};

void set_table_settings(vec_t<ProcmonSettingsTable>& procmon_setting_table, ProcmonSettings& procmon_settings);