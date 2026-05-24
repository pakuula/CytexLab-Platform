cmake_minimum_required(VERSION 3.20)

set(project_name CytexLab.CytexLab-Platform.UnicodeProcessor)
project(${project_name})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/dist/${CMAKE_CXX_PLATFORM_ID}/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/dist/${CMAKE_CXX_PLATFORM_ID}/${CMAKE_BUILD_TYPE})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/dist/${CMAKE_CXX_PLATFORM_ID}/${CMAKE_BUILD_TYPE})

set(COMPILE_BASE_FLAGS -nostdlib -nostdinc -nostdinc++ -ffreestanding -fno-exceptions -fno-rtti -fno-stack-protector -fno-builtin)

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  set(COMPILE_EXTRA_FLAGS -O0 -g)
else()
  set(COMPILE_EXTRA_FLAGS -O3 -g0 -fno-ident -ffile-prefix-map=.= -fno-asynchronous-unwind-tables -ffunction-sections)
endif()

set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_STANDARD 14)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)
add_library(${project_name} STATIC src/UnicodeProcessor.cpp)

target_compile_options(${project_name} PRIVATE ${COMPILE_BASE_FLAGS} ${COMPILE_EXTRA_FLAGS})

set_target_properties(${project_name} PROPERTIES PREFIX "")