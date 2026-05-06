#pragma once

#include <iostream>

#include "types.h"
#include "settings.h"

namespace UserInterface
{
    namespace Commands
    {
        void set();
        void get();
        void help();
    }
    namespace Data
    {
        void get(const str_t& data);
    }
    namespace Metrics
    {
        void all(const vec_t<ProcmonSettingsTable>& procmon_settings_table, const str_t& view_name, const str_t& view_memory);
        void one(const ProcmonSettingsTable& procmon_settings_table, const MetricType& metric_type, const str_t& view_time, const str_t& view_memory);
    }
    namespace Assert
    {
        void time();
    }
    namespace Errors
    {
        void show(const str_t& what, const Result);
    }
}
