#pragma once

#ifdef _WIN32
#include <Windows.h>
#elif defined(__linux__)

#endif

#include "types.h"
#include "settingstypes.h"
#include "UserInterface.h"

namespace CommandProcessor
{   
    Result manage(ProcmonSettings& procmon_settings, const int argc, const char* argv[]);

    namespace FileUtility 
    {
        Result main_load(ProcmonSettings& procmon_settings);

        Result save(const ProcmonSettings& procmon_settings);

        Result save_base_parameters(const ProcmonSettings& procmon_settings, void* h_file);

        Result load_parameters(const ProcmonSettings& procmon_settings, const char* buffer, DWORD REAL_SIZE);
    }

    namespace ParseUtility
    {
        Result convert_char_to_string(vec_t<str_t>& res, const int argc, const char* argv[]);

        Result parse_string(ProcmonSettings& procmon_settings, const int argc, const char* argv[], str_t& option, str_t& metrick, str_t& setting, size_t& index);

        Result convert_settings_to_string(const ProcmonSettings& procmon_settings, str_t& out);
    }

    namespace Command
    {
        namespace Set
        {
            Result manage(ProcmonSettings& procmon_settings, const str_t& metrick, const str_t& setting);
        }

        namespace Get
        {
            Result manage(const ProcmonSettings& procmon_settings, const str_t& metrick);
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
