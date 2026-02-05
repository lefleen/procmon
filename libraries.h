#pragma once

#ifdef _WIN32
#include <windows.h>
#include <psapi.h>

#elif defined (__linux__)


#endif

#include <string>
#include <vector>
#include <thread>
#include <math.h>
#include <iostream>
#include <mutex>
#include <chrono>
#include <map>
#include "macro.h"
#include "types.h"

#include "ProcessDescriptorRAII.h"
#include "UsingCpu.h"
#include "Process.h"
#include "NameSpaces.h"