#pragma once
#include "types.h"

namespace LinuxProc 
{
     namespace ManageOS
     {
          Result get_parameters_processes(DWORD& count_bytes_needed, DWORD& count_processes, vec_t<DWORD>& pids_processes);
     };
}
