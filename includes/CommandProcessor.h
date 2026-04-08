#pragma once

#ifdef _WIN32
#include <Windows.h>
#elif defined(__linux__)
#include <fcntl.h>
#include <unistd.h>

#endif

#include <iostream>

#include "types.h"
#include "settingstypes.h"
#include "UserInterface.h"
#include "DescriptorRAII.h"

namespace CommandProcessor
{   
    Result manage(ProcmonSettings& procmon_settings, const int argc, const char* argv[]);

    namespace FileUtility 
    {
        const int load_file = 100;
        const int save_file = 200;

        Result manage(ProcmonSettings& procmon_settings, const int param);

        Result load(ProcmonSettings& procmon_settings, DescriptorRAII& descriptor_file, const char* file_name);

        Result save(const ProcmonSettings& procmon_settings, DescriptorRAII& descriptor_file, const char* file_name);

        Result save_parameters_in_file(const ProcmonSettings& procmon_settings, const DescriptorRAII& descriptor_file);

        Result load_parameters(ProcmonSettings& procmon_settings, const str_t& data);
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
