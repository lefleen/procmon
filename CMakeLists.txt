cmake_minimum_required(VERSION 3.15) 
project(procmon)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (UNIX)
set(CURRENT_OS_LOGIC_H "LinuxLogic.h")
set(CURRENT_OS_LOGIC_CPP "LinuxLogic.cpp")

elseif(WIN32)
set(CURRENT_OS_LOGIC_H "WindowsLogic.h")
set(CURRENT_OS_LOGIC_CPP "WindowsLogic.cpp")

endif()

add_executable(${PROJECT_NAME}
	"main.cpp"
	"macro.h"
	"ProcessDescriptorRAII.cpp" 
	"ProcessDescriptorRAII.h" 
	"UsingCpu.cpp" 
	"UsingCpu.h" 
	"Process.cpp" 
    "ManageProgramm.cpp" 
    "ManageProgramm.h"
    ${CURRENT_OS_LOGIC_CPP}
    ${CURRENT_OS_LOGIC_H}
)
