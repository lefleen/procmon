#include "types.h"
#include "settingstypes.h"

namespace ParseUtility
{
    Result convert_char_to_string(vec_t<str_t>& res, const int argc, const char* argv[]);

    Result parse_command_string(ProcmonSettings& procmon_settings, const int argc, const char* argv[], str_t& option, str_t& metrick, str_t& setting, size_t& index);

    Result convert_procmon_settings_to_string(const ProcmonSettings& procmon_settings, str_t& out);
}