cmake_minimum_required(VERSION 3.20)

set(project_name CytexLab.CytexLab-Platform.SysCalls.Test.Windows)
project(${project_name})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/dist/${CMAKE_PLATFORM_NO_VERSIONED_SONAME}/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/dist/${CMAKE_PLATFORM_NO_VERSIONED_SONAME}/${CMAKE_BUILD_TYPE})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/dist/${CMAKE_PLATFORM_NO_VERSIONED_SONAME}/${CMAKE_BUILD_TYPE})

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
add_executable(${project_name} src/main.cpp)

target_compile_options(${project_name} PRIVATE ${COMPILE_BASE_FLAGS} ${COMPILE_EXTRA_FLAGS})
target_link_options(${project_name} PRIVATE ${LINK_BASE_FLAGS})
target_link_libraries(${project_name} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../lib/windows/dist/${CMAKE_BUILD_TYPE}/CytexLab.CytexLab-Platform.SysCalls.Lib.Windows.a
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../UnicodeProcessor/dist/${CMAKE_BUILD_TYPE}/CytexLab.CytexLab-Platform.UnicodeProcessor.a
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../MemFunctions/dist/${CMAKE_BUILD_TYPE}/CytexLab.CytexLab-Platform.MemFunctions.a)

set_target_properties(${project_name} PROPERTIES PREFIX "")