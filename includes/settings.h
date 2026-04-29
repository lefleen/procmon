#pragma once

#include "types.h"

struct ProcmonSettings
{
    bool pid = true;
    bool name = true;
    bool time = true;
    bool memory = true;
    bool total_cpu = true;
    bool interval_cpu = true;

    TimeViewSettings time_view_setting = TimeViewSettings::seconds;
    MemoryViewSettings memory_view_setting = MemoryViewSettings::bytes;
};

struct ProcmonSettingsTable
{
    bool* status;
    str_t name = "";
    MetricType metric_type = MetricType::undefined_metric;
    int table_index = -1;
};

void set_table_settings(vec_t<ProcmonSettingsTable>& procmon_settings_table, ProcmonSettings& procmon_settings);
    
