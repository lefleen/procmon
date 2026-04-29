#include "settings.h"

void set_table_settings(vec_t<ProcmonSettingsTable>& procmon_settings_table, ProcmonSettings& procmon_settings)
{
	procmon_settings_table.push_back({ &procmon_settings.pid, "pid",  MetricType::pid, 0 });
	procmon_settings_table.push_back({ &procmon_settings.name, "name",  MetricType::name, 1 });
	procmon_settings_table.push_back({ &procmon_settings.time, "time", MetricType::time, 2 });
	procmon_settings_table.push_back({ &procmon_settings.memory, "memory", MetricType::memory, 3 });
	procmon_settings_table.push_back({ &procmon_settings.total_cpu, "tCPU", MetricType::tCPU, 4 });
	procmon_settings_table.push_back({ &procmon_settings.interval_cpu, "iCPU", MetricType::iCPU, 5 });
	procmon_settings_table.push_back({ nullptr, "time_view", MetricType::time_view, 6 });
	procmon_settings_table.push_back({ nullptr, "memory_view", MetricType::memory_view, 7 });
}

void set_table_view_settings(ProcmonSettingsView& procmon_settings_view, ProcmonSettings& procmon_settings)
{
	procmon_settings_view.time_view_settings = &procmon_settings.time_view_setting;
	procmon_settings_view.memory_view_settings = &procmon_settings.memory_view_setting;
}