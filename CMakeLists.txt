cmake_minimum_required(VERSION 3.15) 
project(procmon)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME}
	"main.cpp"
	"macro.h"
	"Windows.cpp" 
	"ProcessDescriptorRAII.cpp" 
	"ProcessDescriptorRAII.h" 
	"UsingCpu.cpp" 
	"UsingCpu.h" 
	"Process.cpp" 
	"Windows.h" 
 "ManageProgramm.cpp" "ManageProgramm.h" "Linux.h" "Linux.cpp")
