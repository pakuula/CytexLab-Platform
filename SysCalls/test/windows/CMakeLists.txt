cmake_minimum_required(VERSION 3.20)

set(project_name CytexLab.CytexLab-Platform.SysCalls.Test.Windows)
project(${project_name})
project(${project_name})
if(NOT DEFINED CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build." FORCE)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PLATFORM_NO_VERSIONED_SONAME}/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PLATFORM_NO_VERSIONED_SONAME}/${CMAKE_BUILD_TYPE})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PLATFORM_NO_VERSIONED_SONAME}/${CMAKE_BUILD_TYPE})

set(COMPILE_BASE_FLAGS -nostdlib -nostdinc -nostdinc++ -ffreestanding -fno-exceptions -fno-rtti -fno-stack-protector -fno-builtin)
set(LINK_BASE_FLAGS -nostdlib -Wl,--gc-sections,--entry=startup)

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  set(COMPILE_EXTRA_FLAGS -O0 -g)
else()
  set(COMPILE_EXTRA_FLAGS -O3
          -g0 -fno-ident -ffile-prefix-map=.= -fno-asynchronous-unwind-tables -ffunction-sections)
endif()

set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_STANDARD 14)

include_directories(../../../include)

# Register library subprojects so CMake can build them as dependencies in build/deps
add_subdirectory(../../lib/windows ${CMAKE_CURRENT_BINARY_DIR}/deps/SysCallsLibWindows)
add_subdirectory(../../../UnicodeProcessor ${CMAKE_CURRENT_BINARY_DIR}/deps/UnicodeProcessor)
add_subdirectory(../../../MemFunctions ${CMAKE_CURRENT_BINARY_DIR}/deps/MemFunctions)

add_executable(${project_name} src/main.cpp)

target_compile_options(${project_name} PRIVATE ${COMPILE_BASE_FLAGS} ${COMPILE_EXTRA_FLAGS})
target_link_options(${project_name} PRIVATE ${LINK_BASE_FLAGS})
target_link_libraries(${project_name} PRIVATE 
    CytexLab.CytexLab-Platform.SysCalls.Lib.Windows 
    CytexLab.CytexLab-Platform.UnicodeProcessor 
    CytexLab.CytexLab-Platform.MemFunctions)

set_target_properties(${project_name} PROPERTIES PREFIX "")