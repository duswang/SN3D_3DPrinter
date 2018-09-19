#
# ============================================================================
# Name        : Nextion_Project
# Author      : bato
# Version     : 0.1
# Copyright   :
# Description :
# ============================================================================
#
#

# Header #
cmake_minimum_required (VERSION 2.6)
project (Nextion_Project)

# The version number.
set(VERSION_MAJOR 1)
set(VERSION_MINOR 0)

# Paths
set(PRJ_ROOT_PATH .)

set(APP_PATH ${PRJ_ROOT_PATH}/APP)
set(MODUELS_PATH ${PRJ_ROOT_PATH}/MODUELS)

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