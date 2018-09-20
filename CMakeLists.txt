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

# Header #
cmake_minimum_required (VERSION 2.6)
project (sn3d_project)

# The version number.
set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)

# Paths
set(PRJ_ROOT_PATH .)

set(APP_PATH ${PRJ_ROOT_PATH}/APP)
set(MODUELS_PATH ${PRJ_ROOT_PATH}/MODUELS)

# Output Set
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PRJ_ROOT_PATH}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PRJ_ROOT_PATH}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PRJ_ROOT_PATH}/lib)

#Include
include_directories( ${PRJ_ROOT_PATH}/INCLUDE )

#Build Target
add_executable(Source
 
	# Applications #
	${APP_PATH}/main.c
	
	# Moduels #
	${MODUELS_PATH}/MODUEL_HW_UART.c
	
	#Common Moduels #
	${MODUELS_PATH}/serial.c
)