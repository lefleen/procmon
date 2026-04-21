#pragma once

#include <iomanip>
#include <sstream>

#include "CommandProcessor.h"
#include "FileUtility.h"
#include "types.h"
#include "DataProcess.h"
#include "settings.h"

struct ColumnData
{
	MetricType metric_type = MetricType::undefined_metric;
	vec_t<str_t> data{};
	int index_in_table = -1;
};

namespace UpdateUtility
{
	namespace Config
	{
		Result update(vec_t<ProcmonSettingsTable>& procmon_settings_table);
	}

	namespace Data
	{
		namespace StringUtility 
		{
			void insert_lines_in_data(const size_t num_lines, const size_t line_len, str_t& data);

			void first_insert(std::stringstream& s_data, const vec_t<ProcmonSettingsTable>& procmon_settings_table, const size_t size_one_param);

			void data_insert(str_t& data, const ColumnData& container, size_t line_len, const size_t size_one_param);

			Result fill_table(const vec_t<ProcmonSettingsTable>& procmon_settings_table, const vec_t<ColumnData>& container_data_process, str_t& data);
		}

		namespace Setters 
		{
			void set_off(const vec_t<vec_t<DataProcess>>& processes, vec_t<ColumnData>& container_column_data, const MetricType metric_type, const int table_index);

			void set_pid(const vec_t<vec_t<DataProcess>>& processes, vec_t<ColumnData>& container_column_data);

			void set_name(const vec_t<vec_t<DataProcess>>& processes, vec_t<ColumnData>& container_column_data);

			void set_time(const vec_t<vec_t<DataProcess>>& processes, vec_t<ColumnData>& container_column_data);

			void set_memory(const vec_t<vec_t<DataProcess>>& processes, vec_t<ColumnData>& container_column_data);

			void set_totalCPU(const vec_t<vec_t<DataProcess>>& processes, vec_t<ColumnData>& container_column_data);

			void set_intervalCPU(const vec_t<vec_t<DataProcess>>& processes, vec_t<ColumnData>& container_column_data);
		}

		Result convert_container_processes_to_str(const vec_t<vec_t<DataProcess>>& processes, const vec_t<ProcmonSettingsTable>& procmon_setting_table, str_t& data);

		Result set_data_settings(const vec_t<ProcmonSettingsTable>& procmon_setting_table, vec_t<ColumnData>& container_column_data, const vec_t<vec_t<DataProcess>>& processes, str_t& data);

		Result update(const vec_t<ProcmonSettingsTable>& procon_settings, const vec_t<vec_t<DataProcess>>& processes);
	}
}
