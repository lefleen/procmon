cmake_minimum_required(VERSION 3.15) 
project(procmon)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME}
	"main.cpp"
	"macro.h"
	"ProcessDescriptorRAII.cpp" 
	"ProcessDescriptorRAII.h" 
	"UsingCpu.cpp" 
	"UsingCpu.h" 
	"Process.cpp" 
    "Process.h"
    "ManageProgramm.cpp" 
    "ManageProgramm.h"
)

if(UNIX)
    target_sources(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/LinuxLogic.cpp)
    target_sources(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/LinuxLogic.h)
    target_sources(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/SharedSpaceLinux.cpp)
    target_sources(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/SharedSpaceLinux.h)
elseif(WIN32)
    target_sources(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/WindowsLogic.cpp)
    target_sources(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/WindowsLogic.h)
endif()



