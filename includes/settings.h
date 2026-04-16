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
    bool* status;
    str_t name = "";
    MetricType metric_type;
};

void set_table_settings(vec_t<ProcmonSettingsTable>& procmon_settings_table, ProcmonSettings& procmon_settings);
    