#include "settings.h"

void set_table_settings(vec_t<ProcmonSettingsTable>& procmon_settings_table, ProcmonSettings& procmon_settings)
{
	procmon_settings_table.push_back({ "name", &procmon_settings.name });
	procmon_settings_table.push_back({ "time", &procmon_settings.time });
	procmon_settings_table.push_back({ "memory", &procmon_settings.memory });
	procmon_settings_table.push_back({ "totalCPU", &procmon_settings.total_cpu });
	procmon_settings_table.push_back({ "intervalCPU", &procmon_settings.interval_cpu });
}