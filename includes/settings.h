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
    
struct ProcmonDataTable
{
    str_t name;
    bool* status;
    void(*add_element)(str_t& data, void* metrick);
};

namespace Settings
{
    void add_pid(str_t& data, const DataProcess& process);

    void add_name(str_t& data, const DataProcess& process);

    void add_time(str_t& data, const DataProcess& process);

    void add_memory(str_t& data, const DataProcess& process);

    void add_totalCPU(str_t& data, const DataProcess& process);

    void add_intervalCPU(str_t& data, const DataProcess& process);

    void set_table_settings(vec_t<ProcmonSettingsTable>& procmon_setting_table, ProcmonSettings& procmon_settings);

    void set_table_data(vec_t<ProcmonDataTable>& procmon_data_table, ProcmonSettings& procmon_settings);
}