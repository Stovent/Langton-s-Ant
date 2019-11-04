cmake_minimum_required(VERSION 3.0)
cmake_policy(SET CMP0004 OLD)

project(Langtons_ant)

set(SRCS main.c)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin/")
set(CMAKE_SOURCE_DIRECTORY ".")

if(UNIX)
	set(CMAKE_GENERATOR "Unix Makefiles")
endif()

find_package(SDL2)

if(SDL2_FOUND)
	add_executable(Langtons_ant ${SRCS})
	target_include_directories(Langtons_ant PUBLIC ${SDL2_INCLUDE_DIRS})
	target_link_libraries(Langtons_ant ${SDL2_LIBRARIES})
	set(CMAKE_C_FLAGS_DEBUG "-g")
	set(CMAKE_C_FLAGS_RELEASE "-s -O3")
else()
	message(FATAL_ERROR "SDL2 not found")
endif()