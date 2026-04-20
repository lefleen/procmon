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

void UpdateUtility::Data::Setters::set_off(const vec_t<DataProcess>& process, vec_t<ColumnData>& container_column_data, const MetricType metric_type, const int table_index)
{
	vec_t <str_t> data;

	for (auto& it : container_column_data)
	{
		data.push_back("off");
	}

	container_column_data.push_back({ metric_type, data,  table_index });
}

void UpdateUtility::Data::Setters::set_pid(const vec_t<DataProcess>& process, vec_t<ColumnData>& container_column_data)
{
	vec_t <str_t> data;

	for (const auto &it : process)
	{
		data.push_back(std::to_string(it.pid));
	}

	container_column_data.push_back({ MetricType::pid, data, 0 });
}

void UpdateUtility::Data::Setters::set_name(const vec_t<DataProcess>& process, vec_t<ColumnData>& container_column_data)
{
	vec_t <str_t> data;

	for (auto& it : process)
	{
		data.push_back(it.name);
	}

	container_column_data.push_back({ MetricType::name, data, 1 });
}

void UpdateUtility::Data::Setters::set_time(const vec_t<DataProcess>& process, vec_t<ColumnData>& container_column_data)
{
	vec_t <str_t> data;

	for (auto& it : process)
	{
		data.push_back(std::to_string(it.work_time.work_time));
	}

	container_column_data.push_back({ MetricType::time, data, 2 });
}

void UpdateUtility::Data::Setters::set_memory(const vec_t<DataProcess>& process, vec_t<ColumnData>& container_column_data)
{
	vec_t <str_t> data;

	for (auto& it : process)
	{
		data.push_back(std::to_string(it.memory));
	}

	container_column_data.push_back({ MetricType::memory, data, 3 });
}

void UpdateUtility::Data::Setters::set_totalCPU(const vec_t<DataProcess>& process, vec_t<ColumnData>& container_column_data)
{
	vec_t <str_t> data;

	for (auto& it : process)
	{
		data.push_back(std::to_string(it.totalCPU));
	}

	container_column_data.push_back({ MetricType::totalCPU, data, 4 });
}

void UpdateUtility::Data::Setters::set_intervalCPU(const vec_t<DataProcess>& process, vec_t<ColumnData>& container_column_data)
{
	vec_t <str_t> data;

	for (auto& it : process)
	{
		data.push_back(std::to_string(it.intervalCPU));
	}

	container_column_data.push_back({ MetricType::intervalCPU, data, 5 });
}

Result UpdateUtility::Data::set_data_settings(const vec_t<ProcmonSettingsTable>& procmon_settings_table, vec_t<ColumnData>& container_column_data, const vec_t<DataProcess>& process, str_t& data)
{
	Setters::set_pid(process, container_column_data);

	for (auto& it : procmon_settings_table)
	{
		/*if (*it.status)
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
			Setters::set_off(process, container_column_data, it.metric_type, it.table_index);
		}*/
	}

	return Result::successful;
}

void first_insert(std::stringstream& s_data, const vec_t<ProcmonSettingsTable>& procmon_settings_table, const size_t size_one_param)
{
	for (auto& it : procmon_settings_table) 
	{
		s_data << std::setw(size_one_param) << std::left << ("|" + it.name);
	}
	s_data << "\n";
}

void data_insert(std::stringstream& s_data, const ColumnData& container, size_t line_len, const size_t size_one_param)
{
	const size_t full_line_len = line_len + 2;

	size_t pos = full_line_len + size_one_param * container.index_in_table;

	for (auto& it_data : container.data)
	{
		str_t data = std::move(s_data.str());

		size_t it_data_size = it_data.size();

		for (size_t index_pos_string = pos, index_line_data = 0; index_line_data < it_data.size(); ++index_pos_string, ++index_line_data)
			data[index_pos_string] = it_data[index_line_data];

		s_data << data << std::setw(line_len) << "\n";

		pos += full_line_len;
	}
}

Result fill_table(const vec_t<ProcmonSettingsTable>& procmon_settings_table, const vec_t<ColumnData>& container_data_process, str_t& data)
{
	const size_t size_one_param = 20;
	std::stringstream s_data;

	first_insert(s_data, procmon_settings_table, size_one_param);

	const size_t line_len = s_data.str().length();

	for (auto& it : container_data_process)
	{
		data_insert(s_data, it, line_len, size_one_param);
	}

	data = s_data.str();

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

	Result res_fill_table;
	if ((res_fill_table = fill_table(procmon_settings_table, container_column_data, data)) == Result::successful)
		return res_fill_table;

	return Result::successful;
}

Result UpdateUtility::Data::update(const vec_t<ProcmonSettingsTable>& procmon_settings_table, const vec_t<vec_t<DataProcess>>& processes)
{
	vec_t<ProcmonSettings> full_processes;
	str_t data = "";

	Result res_vec_to_str;
	if ((res_vec_to_str = convert_container_processes_to_str(processes, procmon_settings_table, data)) != Result::successful)
		return res_vec_to_str;

	std::cout << data << std::endl;
	// TODO: file function

	return Result::successful;
}
