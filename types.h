#pragma once

#ifdef _WIN32
using descriptor_process_t = void*;
constexpr descriptor_process_t null_t = nullptr;

#elif defined (__linux__)
using descriptor_process_t = int;
using DWORD = unsigned long;

constexpr descriptor_process_t null_t = NULL;

struct ULARGE_INTEGER { unsigned long QuardPart; };

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
	time_t work_time;
};

