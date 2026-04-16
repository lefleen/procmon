#include "settings.h"

void set_table_settings(vec_t<ProcmonSettingsTable>& procmon_settings_table, ProcmonSettings& procmon_settings)
{
	procmon_settings_table.push_back({ &procmon_settings.name, "name",  MetricType::name});
	procmon_settings_table.push_back({ &procmon_settings.time, "time", MetricType::time});
	procmon_settings_table.push_back({ &procmon_settings.memory, "memory", MetricType::memory});
	procmon_settings_table.push_back({ &procmon_settings.total_cpu, "totalCPU", MetricType::totalCPU});
	procmon_settings_table.push_back({ &procmon_settings.interval_cpu, "intervalCPU", MetricType::intervalCPU});
}