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

void UpdateUtility::Data::Setters::get_length_param(MetricType metric_type, size_t& length)
{
	switch (metric_type)
	{
	case MetricType::pid: length = WIDTH_PID + WIDTH_SPECIAL_INFORMATION; 
		break;

	case MetricType::name: length = WIDTH_NAME + WIDTH_SPECIAL_INFORMATION;
		break;

	case MetricType::time: length = WIDTH_TIME + WIDTH_SPECIAL_INFORMATION;
		break;

	case MetricType::memory: length = WIDTH_MEMORY + WIDTH_SPECIAL_INFORMATION;
		break;

	case MetricType::tCPU: length = WIDTH_tCPU + WIDTH_SPECIAL_INFORMATION;
		break;

	case MetricType::iCPU: length = WIDTH_iCPU + WIDTH_SPECIAL_INFORMATION;
		break;
	}
}

void UpdateUtility::Data::Setters::set_off(const vec_t<vec_t<DataProcess>>& processes, vec_t<ColumnData>& container_column_data, const MetricType metric_type, const int index_in_table)
{
	vec_t <str_t> data;
	size_t length = 0;

	get_length_param(metric_type, length);

	for (auto& it_processes : processes)
	{
		for (auto& it_cur_process : it_processes)
		{
			data.push_back("off");
		}
	}

	container_column_data.push_back({ metric_type, data,  index_in_table, length });
}

void UpdateUtility::Data::Setters::set_pid(const vec_t<vec_t<DataProcess>>& processes, vec_t<ColumnData>& container_column_data)
{
	vec_t <str_t> data;
	const int index_in_table = 0;
	size_t length = 0;

	get_length_param(MetricType::pid, length);

	for (auto& it_processes : processes)
	{
		for (auto& it_cur_process : it_processes)
		{
			data.push_back(std::to_string(it_cur_process.pid));
		}
	}

	container_column_data.push_back({ MetricType::pid, data, index_in_table, length });
}

void UpdateUtility::Data::Setters::set_name(const vec_t<vec_t<DataProcess>>& processes, vec_t<ColumnData>& container_column_data)
{
	vec_t <str_t> data;
	const int index_in_table = 1;
	size_t length = 0;

	get_length_param(MetricType::name, length);

	for (auto& it_processes : processes)
	{
		for (auto& it_cur_process : it_processes)
		{
			data.push_back(it_cur_process.name);
		}
	}

	container_column_data.push_back({ MetricType::name, data, index_in_table, length });
}

void UpdateUtility::Data::Setters::set_time(const vec_t<vec_t<DataProcess>>& processes, vec_t<ColumnData>& container_column_data, const ProcmonSettings& procmon_settings)
{
	vec_t <str_t> data;
	const int index_in_table = 2;
	size_t length = 0;

	get_length_param(MetricType::time, length);

	for (auto& it_processes : processes)
	{
		for (auto& it_cur_process : it_processes)
		{
            double rounded;
            std::stringstream s_data;

            switch(procmon_settings.time_view_setting)
            {
                case TimeViewSettings::seconds: rounded = std::round(it_cur_process.work_time.num_seconds); 
                    break;

                case TimeViewSettings::minutes: rounded = std::round(it_cur_process.work_time.num_minutes); 
                    break;

                case TimeViewSettings::hours: rounded = std::round(it_cur_process.work_time.num_hours); 
                    break;

                case TimeViewSettings::days: rounded = std::round(it_cur_process.work_time.num_days);
                    break;
            }

            s_data << std::fixed << std::setprecision(2) << rounded;
            data.push_back(s_data.str());
   		}
	}

	container_column_data.push_back({ MetricType::time, data, index_in_table, length });
}

void UpdateUtility::Data::Setters::set_memory(const vec_t<vec_t<DataProcess>>& processes, vec_t<ColumnData>& container_column_data)
{
	vec_t <str_t> data;
	const int index_in_table = 3;
	size_t length = 0;

	get_length_param(MetricType::memory, length);

	for (auto& it_processes : processes)
	{
		for (auto& it_cur_process : it_processes)
		{
			double rounded = std::round(it_cur_process.memory * 100.0) / 100.0;
			std::stringstream s_data;

			s_data << std::fixed << std::setprecision(2) << rounded;

			data.push_back(s_data.str());
		}
	}

	container_column_data.push_back({ MetricType::memory, data, index_in_table, length });
}

void UpdateUtility::Data::Setters::set_totalCPU(const vec_t<vec_t<DataProcess>>& processes, vec_t<ColumnData>& container_column_data)
{
	vec_t <str_t> data;
	const int index_in_table = 4;
	size_t length = 0;

	get_length_param(MetricType::tCPU, length);

	for (auto& it_processes : processes)
	{
		for (auto& it_cur_process : it_processes)
		{
			double rounded = std::round(it_cur_process.totalCPU * 100.0) / 100.0;
			std::stringstream s_data;

			s_data << std::fixed << std::setprecision(2) << rounded;

			data.push_back(s_data.str());
		}
	}

	container_column_data.push_back({ MetricType::tCPU, data, index_in_table, length });
}

void UpdateUtility::Data::Setters::set_intervalCPU(const vec_t<vec_t<DataProcess>>& processes, vec_t<ColumnData>& container_column_data)
{
	vec_t <str_t> data;
	const int index_in_table = 5;
	size_t length = 0;

	get_length_param(MetricType::iCPU, length);

	for (auto& it_processes : processes)
	{
		for (auto& it_cur_process : it_processes)
		{
			double rounded = std::round(it_cur_process.intervalCPU * 100.0) / 100.0;
			std::stringstream s_data;

			s_data << std::fixed << std::setprecision(2) << rounded;

			data.push_back(s_data.str());
		}
	}

	container_column_data.push_back({ MetricType::iCPU, data, index_in_table, length });
}

Result UpdateUtility::Data::set_data_settings(const vec_t<ProcmonSettingsTable>& procmon_settings_table, vec_t<ColumnData>& container_column_data, const vec_t<vec_t<DataProcess>>& processes, str_t& data, const ProcmonSettings&procmon_settings)
{
	for (auto& it : procmon_settings_table)
	{
		if (*it.status)
		{
			if (it.metric_type == MetricType::pid)
			{
				Setters::set_pid(processes, container_column_data);
			}
			else if (it.metric_type == MetricType::name)
			{
				Setters::set_name(processes, container_column_data);
			}
			else if (it.metric_type == MetricType::time)
			{
				Setters::set_time(processes, container_column_data, procmon_settings);
			}
			else if (it.metric_type == MetricType::memory)
			{
				Setters::set_memory(processes, container_column_data);
			}
			else if (it.metric_type == MetricType::tCPU)
			{
				Setters::set_totalCPU(processes, container_column_data);
			}
			else if (it.metric_type == MetricType::iCPU)
			{
				Setters::set_intervalCPU(processes, container_column_data);
			}
			else return Result::invalid_arguments;
		}
		else
		{
			Setters::set_off(processes, container_column_data, it.metric_type, it.table_index);
		}
	}

	return Result::successful;
}

void UpdateUtility::Data::StringUtility::insert_lines_in_data(const size_t num_lines, const size_t line_len, str_t& data)
{
	std::stringstream s_data;
	s_data << data;

	for (size_t index = 0; index < num_lines; ++index)
	{
		s_data << std::setw(line_len) << "\n";
	}

	data = s_data.str();
}

void UpdateUtility::Data::StringUtility::name_metrics_insert(std::stringstream& s_data, const vec_t<ProcmonSettingsTable>& procmon_settings_table, const vec_t<ColumnData>& container_column_data)
{
	for (int index = 0; index < procmon_settings_table.size(); ++index)
	{
		s_data << std::setw(container_column_data[index].length_one_param) << std::left << ("|" + procmon_settings_table[index].name);
	}
	s_data << "\n";
}

void UpdateUtility::Data::StringUtility::data_insert(str_t& data, const ColumnData& container, const size_t line_len, const size_t previous_pos_sum)
{
	size_t pos = line_len + previous_pos_sum;

	for (auto& it_data : container.data)
	{
		size_t it_data_size = it_data.size();
        size_t data_size = data.size();

        data[pos] = '|';
		for (size_t index_pos_string = pos + 1, index_line_data = 0; index_line_data < it_data_size && index_pos_string < data_size; ++index_pos_string, ++index_line_data)
		{
			data[index_pos_string] = it_data[index_line_data];
		}

		pos += line_len;
	}
}

Result UpdateUtility::Data::StringUtility::fill_table(const vec_t<ProcmonSettingsTable>& procmon_settings_table, const vec_t<ColumnData>& container_column_data, str_t& data)
{
	std::stringstream s_data;

	name_metrics_insert(s_data, procmon_settings_table, container_column_data);
	data = s_data.str();

	const size_t line_len = data.length();
	size_t previous_pos_sum = 0;
	size_t previous_index = 0;

	if (container_column_data.size() > 0)
		insert_lines_in_data(container_column_data[0].data.size(), line_len, data);
	else return Result::failure;

	for (auto& it : container_column_data)
	{
		if (previous_index > it.index_in_table)
			return Result::failure;

		data_insert(data, it, line_len, previous_pos_sum);
		previous_pos_sum += it.length_one_param;

		previous_index = it.index_in_table;
	}

	return Result::successful;
}

Result UpdateUtility::Data::convert_container_processes_to_str(const vec_t<vec_t<DataProcess>>& processes, const vec_t<ProcmonSettingsTable>& procmon_settings_table, str_t& data, const ProcmonSettings& procmon_settings)
{
	vec_t<ColumnData> container_column_data;
	
	Result res_set_data;
	if ((res_set_data = set_data_settings(procmon_settings_table, container_column_data, processes, data, procmon_settings)) != Result::successful)
		return res_set_data;
	
	Result res_fill_table;
	if ((res_fill_table = StringUtility::fill_table(procmon_settings_table, container_column_data, data)) != Result::successful)
		return res_fill_table;

	return Result::successful;
}

Result UpdateUtility::Data::update(const vec_t<ProcmonSettingsTable>& procmon_settings_table, const vec_t<vec_t<DataProcess>>& processes, const ProcmonSettings& procmon_settings)
{
	vec_t<ProcmonSettings> full_processes;
	str_t data = "";

	Result res_vec_to_str;
	if ((res_vec_to_str = convert_container_processes_to_str(processes, procmon_settings_table, data, procmon_settings)) != Result::successful)
		return res_vec_to_str;
	
	Result res_file_manage;
	if((res_file_manage = FileUtility::Data::manage(data, FileUtility::save_data_process_file)) != Result::successful)
		return res_file_manage;

	return Result::successful;
}
