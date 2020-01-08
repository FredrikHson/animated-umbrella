cmake_minimum_required(VERSION 3.4)

project(fcloth)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

include_directories(
	"src/"
	)

set(CMAKE_C_STANDARD 11)

add_library(fcloth SHARED cloth.c)
add_library(fclothloader SHARED libloader.c)
target_link_libraries(fclothloader
	dl
	)
