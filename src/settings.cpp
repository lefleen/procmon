#include "settings.h"

	void Settings::add_pid(str_t& data, const DataProcess& process)
	{
		data += std::to_string(process.pid);
	}

	void Settings::add_name(str_t& data, const DataProcess& process)
	{
		data += process.name;
	}

	void Settings::add_time(str_t& data, const DataProcess& process)
	{
		data += std::to_string(process.work_time.work_time);
	}

	void Settings::add_memory(str_t& data, const DataProcess& process)
	{
		data += std::to_string(process.memory));
	}

	void Settings::add_totalCPU(str_t& data, const DataProcess& process)
	{
		data += std::to_string(process.total_using_cpu));
	}

	void Settings::add_intervalCPU(str_t& data, const DataProcess& process)
	{
		data += std::to_string(*static_cast<double*>(interval_using_cpu));
	}

	void Settings::set_table_settings(vec_t<ProcmonSettingsTable>& procmon_settings_table, ProcmonSettings& procmon_settings)
	{
		procmon_settings_table.push_back({ "name", &procmon_settings.name });
		procmon_settings_table.push_back({ "time", &procmon_settings.time });
		procmon_settings_table.push_back({ "memory", &procmon_settings.memory });
		procmon_settings_table.push_back({ "totalCPU", &procmon_settings.total_cpu });
		procmon_settings_table.push_back({ "intervalCPU", &procmon_settings.interval_cpu });
	}

	void Settings::set_table_data(vec_t<ProcmonDataTable>& procmon_data_table, ProcmonSettings& procmon_settings) 
	{
		procmon_data_table.push_back({ "name", &procmon_settings.name, add_name });
		procmon_data_table.push_back({ "time", &procmon_settings.time, add_time });
		procmon_data_table.push_back({ "memory", &procmon_settings.memory, add_memory });
		procmon_data_table.push_back({ "totalCPU", &procmon_settings.total_cpu, add_totalCPU });
		procmon_data_table.push_back({ "intervalCPU", &procmon_settings.interval_cpu, add_intervalCPU });
	}