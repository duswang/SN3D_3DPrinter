#
# ============================================================================
# Name        : sn3d_project
# Author      : bato
# Version     : 0.1
# Copyright   :
# Description :
# ============================================================================
#
#

# Header
cmake_minimum_required (VERSION 2.6)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED TRUE)
set (CMAKE_CXX_EXTENSIONS FALSE)



# Project Info 
project (sn3d_project)
set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)


# CFlags Def
set(CMAKE_CXX_FLAGS -std=c11 -Wall)

# Message
message("Project Name   :: SN3D Project.")
message("Version        :: ${VERSION_MAJOR}.${VERSION_MINOR}v")
message("Build Set      :: CMAKE_BUILD_TYPE is ${CMAKE_BUILD_TYPE}")
message("Flags          :: CMAKE_CXX_FLAGS is ${CMAKE_CXX_FLAGS}")
message("Flags(DEBUG)   :: CMAKE_CXX_FLAGS_DEBUG is ${CMAKE_CXX_FLAGS_DEBUG}")
message("Flags(RELEASE) :: CMAKE_CXX_FLAGS_RELEASE is ${CMAKE_CXX_FLAGS_RELEASE}")


# Paths
set(PRJ_ROOT_PATH .)

set(APP_PATH ${PRJ_ROOT_PATH}/APP)
set(MODUELS_PATH ${PRJ_ROOT_PATH}/MODUELS)

# Output Set
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PRJ_ROOT_PATH}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PRJ_ROOT_PATH}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PRJ_ROOT_PATH}/lib)

# Include
include_directories( ${PRJ_ROOT_PATH}/INCLUDE )

# Build Target
add_executable(
	# Target Name #
	Source
 
	# Applications #
	${APP_PATH}/main.c
	
	# Moduels #
	${MODUELS_PATH}/MODUEL_3D_PRINTER.c
	${MODUELS_PATH}/MODUEL_DISPLAY.c
	
	#Common Moduels #
	${MODUELS_PATH}/SERIAL_COMM.c
)

# CFlags 
target_compile_options(Source PRIVATE ${CMAKE_CXX_FLAGS})
target_compile_definitions(Source PRIVATE BOOST_ERROR_CODE_HEADER_ONLY)

# Library
target_link_libraries(Source pthread)


