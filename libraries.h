#pragma once
#include <windows.h>
#include <psapi.h>
#include <string>
#include <vector>
#include <thread>
#include <math.h>
#include <iostream>
#include <mutex>
#include <chrono>
#include <map>
#include "macro.h"

using str_t = std::string;
using wstr_t = std::wstring;

template <class T>
using vec_t = std::vector<T>;

template <class Key, class Type>
using map_t = std::map<Key, Type>;

struct my_tm
{
	time_t num_days = 0;
	time_t num_hours = 0;
	time_t num_minutes = 0;
	time_t num_seconds = 0;
	time_t work_time;
};

#include "HandleRAII.h"
#include "UsingCpu.h"
#include "Process.h"
#include "NameSpaces.h"