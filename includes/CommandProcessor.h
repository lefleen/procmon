#pragma once

#include "types.h"
#include "settingstypes.h"
#include "UserInterface.h"
#include "DescriptorRAII.h"
#include "FileUtility.h"

namespace CommandProcessor
{
    Result manage(ProcmonSettings& procmon_settings, const int argc, const char* argv[]);

    namespace Command
    {
        namespace Set
        {
            Result array_data(ProcmonSettings& procmon_settings, const map_t<str_t, str_t>& file_config);

            Result manage(ProcmonSettings& procmon_settings, const str_t& metrick, const str_t& setting);
        }

        namespace Get
        {
            Result manage(const ProcmonSettings& procmon_settings, const str_t& metrick);

            Result programm_config(const ProcmonSettings& procmon_settings, const str_t& metrick);
        }
    }

    namespace Name
    {
        
        Result set(ProcmonSettings& procmon_settings, const str_t& value);
    }

    namespace Time 
    {
        Result set(ProcmonSettings& procmon_settings, const str_t& value);
    }

    namespace Memory 
    {
        Result set(ProcmonSettings& procmon_settings, const str_t& value);
    }

    namespace TotalCPU 
    {
        Result set(ProcmonSettings& procmon_settings, const str_t& value);
    }

    namespace IntervalCPU 
    {
        Result set(ProcmonSettings& procmon_settings, const str_t& value);
    }
}