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
    MemoryViewSettings memory_view_setting = MemoryViewSettings::B;
};

struct ProcmonSettingsTable
{
    bool* status;
    str_t name = "";
    MetricType metric_type = MetricType::undefined_metric;
    int table_index = -1;
};

struct ProcmonSettingsView
{
    TimeViewSettings* time_view_settings;
    MemoryViewSettings* memory_view_settings;
};

void set_table_settings(vec_t<ProcmonSettingsTable>& procmon_settings_table, ProcmonSettings& procmon_settings);

void set_table_view_settings(ProcmonSettingsView& procmon_settings_view, ProcmonSettings& procmon_settings);
    
