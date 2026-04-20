#include "settings.h"

void set_table_settings(vec_t<ProcmonSettingsTable>& procmon_settings_table, ProcmonSettings& procmon_settings)
{
	procmon_settings_table.push_back({ nullptr, "pid",  MetricType::pid, 0 });
	procmon_settings_table.push_back({ &procmon_settings.name, "name",  MetricType::name, 1 });
	procmon_settings_table.push_back({ &procmon_settings.time, "time", MetricType::time, 2 });
	procmon_settings_table.push_back({ &procmon_settings.memory, "memory", MetricType::memory, 3 });
	procmon_settings_table.push_back({ &procmon_settings.total_cpu, "totalCPU", MetricType::totalCPU, 4 });
	procmon_settings_table.push_back({ &procmon_settings.interval_cpu, "intervalCPU", MetricType::intervalCPU, 5 });
}