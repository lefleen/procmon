#include "types.h"
#include "settingstypes.h"

namespace CommandProcessor
{   
    Result convert_char_to_string(vec_t<str_t>& res, const int argc, const char* argv[]);

    Result parse_string(ProcmonSettings& procmon_settings, const int argc, const char* argv[]);

    namespace Name
    {
        Result set(ProcmonSettings& procmon_settings, const str_t& value);
    }
}
