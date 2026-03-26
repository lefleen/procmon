#include "SharedSpaceLinux.h"

Result ProcmonLogic::SharedSpaceLinux::parse_string(const int num_element, const str_t& file_data, str_t& res)
{
    if (num_element <= 0 || file_data.empty()) return Result::failure;

    size_t found = 0;
    size_t first_pos_name = 0;
    size_t last_pos_name = 0;
    int _num_element = num_element;
    int num_symbols = 0;
 
    if (num_element >= 2)
    {
        first_pos_name = file_data.find('(', 0);
        last_pos_name = file_data.rfind(')');
 
        if (first_pos_name == str_t::npos || last_pos_name == str_t::npos)
            return Result::failure;
 
        if(num_element == 2)
        {
            num_symbols = last_pos_name - (first_pos_name + 1);
            res = file_data.substr(first_pos_name + 1, num_symbols);
 
            return Result::successful;
        }
 
        found = last_pos_name;
       --_num_element;
    }
 
    while(--_num_element >= 1)
       found = file_data.find(' ', found + 1);

    if (found == str_t::npos) return Result::failure;
 
    if(num_element > 1)
    {
        first_pos_name = found + 1;
        if((last_pos_name = file_data.find(' ', first_pos_name)) == str_t::npos) return Result::failure;
    }
    else
    {
        first_pos_name = 0;
        last_pos_name = file_data.find(' ', 0);
    }
 
    num_symbols = last_pos_name - first_pos_name;
    res = file_data.substr(first_pos_name, num_symbols);
 
    return Result::successful;
}
