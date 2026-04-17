#include "UpdateUtility.h"

Result UpdateUtility::Config::update(vec_t<ProcmonSettingsTable>& procmon_settings_table)
{
	map_t<str_t, str_t> file_config{ };

	Result res_file_load;
	if ((res_file_load = FileUtility::Config::manage(procmon_settings_table, file_config, FileUtility::load_config_file)) != Result::successful)
		return res_file_load;

	Result res_set_config;
	if ((res_set_config = CommandProcessor::Command::Set::array_data(procmon_settings_table, file_config)) != Result::successful)
		return Result::failure;

	return Result::successful;
}

void UpdateUtility::Data::Setters::set_off(const vec_t<DataProcess>& process, vec_t<ColumnData>& container_column_data, const MetricType metric_type)
{
	vec_t <str_t> data;

	for (auto& it : container_column_data)
	{
		data.push_back("off");
	}

	container_column_data.push_back({ metric_type, data });
}

void UpdateUtility::Data::Setters::set_pid(const vec_t<DataProcess>& process, vec_t<ColumnData>& container_column_data)
{
	vec_t <str_t> data;

	for (auto &it : process)
	{
		data.push_back(std::to_string(it.pid));
	}

	container_column_data.push_back({ MetricType::pid, data });
}

void UpdateUtility::Data::Setters::set_name(const vec_t<DataProcess>& process, vec_t<ColumnData>& container_column_data)
{
	vec_t <str_t> data;

	for (auto& it : process)
	{
		data.push_back(it.name);
	}

	container_column_data.push_back({ MetricType::name, data });
}

void UpdateUtility::Data::Setters::set_time(const vec_t<DataProcess>& process, vec_t<ColumnData>& container_column_data)
{
	vec_t <str_t> data;

	for (auto& it : process)
	{
		data.push_back(std::to_string(it.work_time.work_time));
	}

	container_column_data.push_back({ MetricType::time, data });
}

void UpdateUtility::Data::Setters::set_memory(const vec_t<DataProcess>& process, vec_t<ColumnData>& container_column_data)
{
	vec_t <str_t> data;

	for (auto& it : process)
	{
		data.push_back(std::to_string(it.memory));
	}

	container_column_data.push_back({ MetricType::memory, data });
}

void UpdateUtility::Data::Setters::set_totalCPU(const vec_t<DataProcess>& process, vec_t<ColumnData>& container_column_data)
{
	vec_t <str_t> data;

	for (auto& it : process)
	{
		data.push_back(std::to_string(it.totalCPU));
	}

	container_column_data.push_back({ MetricType::totalCPU, data });
}

void UpdateUtility::Data::Setters::set_intervalCPU(const vec_t<DataProcess>& process, vec_t<ColumnData>& container_column_data)
{
	vec_t <str_t> data;

	for (auto& it : process)
	{
		data.push_back(std::to_string(it.intervalCPU));
	}

	container_column_data.push_back({ MetricType::intervalCPU, data });
}

Result UpdateUtility::Data::set_data_settings(const vec_t<ProcmonSettingsTable>& procmon_settings_table, vec_t<ColumnData>& container_column_data, const vec_t<DataProcess>& process, str_t& data)
{
	Setters::set_pid(process, container_column_data);

	for (auto& it : procmon_settings_table)
	{
		if (*it.status)
		{
			if (it.metric_type == MetricType::name)
			{
				Setters::set_name(process, container_column_data);
			}
			else if (it.metric_type == MetricType::time)
			{
				Setters::set_time(process, container_column_data);
			}
			else if (it.metric_type == MetricType::memory)
			{
				Setters::set_memory(process, container_column_data);
			}
			else if (it.metric_type == MetricType::totalCPU)
			{
				Setters::set_totalCPU(process, container_column_data);
			}
			else if (it.metric_type == MetricType::intervalCPU)
			{
				Setters::set_intervalCPU(process, container_column_data);
			}
			else return Result::invalid_arguments;
		}
		else
		{
			Setters::set_off(process, container_column_data, it.metric_type);
		}
	}

	return Result::successful;
}

Result UpdateUtility::Data::convert_column_data_to_str(const vec_t<ColumnData>& column_data, str_t& data) 
{
    const size_t size_one_param = 20;
    std::stringstream s_data;

	for (auto& it : column_data)
	{
		if (it.metric_type == MetricType::name) 
		{

		}
	}

    return Result::successful;
}

Result UpdateUtility::Data::convert_container_processes_to_str(const vec_t<vec_t<DataProcess>>& processes, const vec_t<ProcmonSettingsTable>& procmon_settings_table, str_t& data)
{
	vec_t<ColumnData> container_column_data;

	for (auto& it_1 : processes)
	{
		Result res_set_data;
		if ((res_set_data = set_data_settings(procmon_settings_table, container_column_data, it_1, data)) != Result::successful)
			return res_set_data;
	}



	return Result::successful;
}

Result UpdateUtility::Data::update(const vec_t<ProcmonSettingsTable>& procmon_settings_table, const vec_t<vec_t<DataProcess>>& processes)
{
	vec_t<ProcmonSettings> full_processes;
	str_t data = "|pid\t\t|name\t\t|time\t\t|memory\t\t|totalCPU\t\t|intervalCPU\t\t|\n";

	Result res_vec_to_str;
	if ((res_vec_to_str = convert_container_processes_to_str(processes, procmon_settings_table, data)) != Result::successful)
		return res_vec_to_str;

	std::cout << data << std::endl;
	// TODO: file function

	return Result::successful;
}
