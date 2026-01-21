#pragma once

#include <windows.h>
#include <psapi.h>
#include <string>
#include <vector>
#include <thread>
#include <math.h>
#include <iostream>
#include <mutex>


using str_t = std::string;
using wstr_t = std::wstring;

template <typename T>
using vec_t = std::vector<T>;