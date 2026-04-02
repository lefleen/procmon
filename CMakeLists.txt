cmake_minimum_required(VERSION 3.15) 
project(procmon)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(PATH_TO_INCLUDE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/includes)
set(PATH_TO_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(${PROJECT_NAME}
	"${PATH_TO_SOURCE_FILES}/main.cpp"
	"${PATH_TO_SOURCE_FILES}/ProcessDescriptorRAII.cpp" 
	"${PATH_TO_SOURCE_FILES}/UsingCpu.cpp" 
	"${PATH_TO_SOURCE_FILES}/DataProcess.cpp" 
	"${PATH_TO_SOURCE_FILES}/ManageProgramm.cpp" 
    "${PATH_TO_SOURCE_FILES}/CommandProcessor.cpp"
)

target_include_directories(${PROJECT_NAME} PRIVATE ${PATH_TO_INCLUDE_FILES})

if(UNIX)
    target_sources(${PROJECT_NAME} PRIVATE ${PATH_TO_SOURCE_FILES}/LinuxLogic.cpp)
    target_sources(${PROJECT_NAME} PRIVATE ${PATH_TO_SOURCE_FILES}/SharedSpaceLinux.cpp)
elseif(WIN32)
    target_sources(${PROJECT_NAME} PRIVATE ${PATH_TO_SOURCE_FILES}/WindowsLogic.cpp)
endif()
