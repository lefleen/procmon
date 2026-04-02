#pragma once

enum class Result
{
	successful,
	failure,
    initialization,
    no_arguments,
    invalid_arguments
};

constexpr int  macCreateTimeProcess = 1001;
constexpr int  macExitTimeProcess   = 1002;
constexpr int  macKernelTimeProcess = 1003;
constexpr int  macUserTimeProcess =   1004;

constexpr int macNameProcess = 2;
