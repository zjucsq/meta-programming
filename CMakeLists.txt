cmake_minimum_required(VERSION 3.24)
project(meta)

set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a")x

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB CPP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/other/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/simple/*.cpp")

set(NOT_COMPILE "")

foreach(file ${CPP_FILES})
    get_filename_component(filename ${file} NAME_WE)
    if (NOT ${filename} IN_LIST NOT_COMPILE)
        add_executable(${filename} ${file})
        message("add ${filename}")
    endif()
endforeach()

