cmake_minimum_required(VERSION 3.4)

project(fcloth)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
option(USEOPENMP "enable openmp threading" ON)
if(USEOPENMP)
    find_package(OpenMP)
    if (OPENMP_FOUND)
        set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    endif()
endif()

include_directories(
	"src/"
	)

set(CMAKE_C_STANDARD 11)

add_library(fcloth SHARED cloth.c)
add_library(fclothloader SHARED libloader.c)
target_link_libraries(fclothloader
	dl
	)
