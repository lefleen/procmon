#pragma once
#include <string>
#include <vector>
#include <map>

#include "macro.h"

#ifdef _WIN32
#include <windows.h>
#include <psapi.h>

using descriptor_process_t = void*;
using process_time = FILETIME;

constexpr descriptor_process_t null_t = nullptr;

#elif defined __linux__
using DWORD = long long;
using descriptor_process_t = long long;
using process_time = unsigned long long;

constexpr descriptor_process_t null_t = 0;

struct ULARGE_INTEGER { unsigned long QuadPart; };

#endif

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
	time_t work_time = 0;
};

#ifdef _WIN32
struct parameters_process
{
	DWORD count_bytes_needed;
	DWORD count_processes;
	vec_t<DWORD> pids_processes;
};

#elif defined __linux__
struct parameters_process
{
    DWORD count_processes;
    vec_t<DWORD> pids_processes;
};

#endif


