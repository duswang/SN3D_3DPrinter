#
# ============================================================================
# Name        : sn3d_project
# Author      : bato
# Copyright   :
# Description :
# ============================================================================
#
######################## Base Info Setting ########################
# Header #
cmake_minimum_required(VERSION 2.6)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS FALSE)

# Project Info  #
project(sn3d_project)

set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)

# Paths #
set(PRJ_ROOT_PATH .)
set(PRJ_SOURCE_PATH ${PRJ_ROOT_PATH}/src)
set(PRJ_BINARY_PATH ${PRJ_ROOT_PATH}/bin)

set(APP_PATH      ${PRJ_SOURCE_PATH}/APP)
set(APPS_PATH     ${APP_PATH}/APPS)

set(MODULE_PATH   ${PRJ_SOURCE_PATH}/MODULE)
set(SYSTEM_PATH   ${PRJ_SOURCE_PATH}/SYSTEM)
set(LIBRARY_PATH  ${PRJ_SOURCE_PATH}/LIBRARY)

# Output Paths #
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PRJ_BINARY_PATH}/build)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PRJ_BINARY_PATH}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PRJ_BINARY_PATH}/lib)

# Cmkae Module #
set(CMAKE_MODULE_PATH "${PRJ_ROOT_PATH}/cmake/CMAKE_MODULE")

################### Library & Source & Include Setting ###################
# Library #
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(png REQUIRED)
find_package(libzip REQUIRED)
find_package(libxml2 REQUIRED)

# Include - Local #
include_directories(
	# API #
	${PRJ_SOURCE_PATH}/INCLUDE
	
	# SYSTEM #
	${SYSTEM_PATH}/INCLUDE
	
	# MODULE #
	${MODULE_PATH}/INCLUDE
	
	# MODULE :: FILE SYSTEM #
	${MODULE_PATH}/FILE_SYSTEM/INCLUDE
	

	
	# APP #
	${APP_PATH}/INCLUDE
	${APPS_PATH}/INCLUDE
)
# Include - Library #
include_directories(
	${SDL2_INCLUDE_DIRS}
	${SDL2_IMAGE_INCLUDE_DIRS}
	${PNG_INCLUDE_DIRS}
	${LIBZIP_INCLUDE_DIRS}
	${LIBXML2_INCLUDE_DIRS}
)
# Source #
set(
	# Sources Name #
	Source

	# App Main #
	${APP_PATH}/APP_MAIN.c
	
	# Apps #
	${APPS_PATH}/APP_CONTROL.c
	${APPS_PATH}/APP_FILE_SELECT.c
	${APPS_PATH}/APP_PAUSE.c
	${APPS_PATH}/APP_PRINTING.c
	${APPS_PATH}/APP_WAITING.c
	${APPS_PATH}/APP_INIT.c

	# Moduel #
	${MODULE_PATH}/SN_MODULE_FILE_SYSTEM.c
	${MODULE_PATH}/SN_MODULE_IMAGE_VIEWER.c
	${MODULE_PATH}/SN_MODULE_3D_PRINTER.c
	${MODULE_PATH}/SN_MODULE_DISPLAY.c


	# Module :: File System #
	${MODULE_PATH}/FILE_SYSTEM/FILE_SYSTEM_XML.c
	${MODULE_PATH}/FILE_SYSTEM/FILE_SYSTEM_PAGE.c
	
	# Main #
	${SYSTEM_PATH}/SN_SYS_INIT.c
	
	# System #
	${SYSTEM_PATH}/SN_SYS_ERROR.c
	${SYSTEM_PATH}/SN_SYS_MESSAGE_Q.c
	${SYSTEM_PATH}/SN_SYS_SERIAL_COMM.c
	${SYSTEM_PATH}/SN_SYS_TIMER.c
	${SYSTEM_PATH}/SN_SYS_USB_DRIVER.c
	
)

# Library #
set(
	# Library Name #
	Library

	# ptrhead #
	pthread

	# libusb #
	usb-1.0

	# libzip #
	${LIBZIP_LIBRARIES}

	# SDL #
	${SDL2_LIBRARIES}
	${SDL2_IMAGE_LIBRARIES}
	
	# PNG #
	${PNG_LIBRARIES}
	
	# XML #
	${LIBXML2_LIBRARIES}
)

# Flags #
set(
	# Flags Name #
	CMAKE_CXX_FLAGS
	
	# GDB #
	-g
	
	# Warning #
	-Wall
)

# Config #
set(
	# Config Name #
	Config

	# IMAGE #
	-DIMAGE_VIEWER_OFF=false

    # 3D PRINTER #
    -DPRINTER_RX_DEBUG=false
	-DMOTOR_DISALBE=false

    # DISPLAY #
    -DDISPLAY_RX_DEBUG=true

    -DTX_DEBUG=false
    
    # IMAGE VIEWER #
    -DIMAGE_VIEWER_FIX_RESOLUTION=true
    -DIMAGE_VIEWER_USE_SDL=false
)

# Command #
add_custom_target(gdb gdb ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/exe)
add_custom_target(run ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/exe)

add_custom_target(start sudo systemctl restart SN3D.service)
add_custom_target(stop sudo systemctl stop SN3D.service)

add_custom_target(doxygen doxygen ${PRJ_ROOT_PATH}/sn3d_doxygen)

add_custom_target(all_clean rm -rf ${PRJ_ROOT_PATH}/CMakeFiles ${PRJ_ROOT_PATH}/cmake_install.cmake ${PRJ_ROOT_PATH}/CMakeCache.txt)

################### Target Setting ###################
# Install exe file #
add_executable(exe ${Source})

# Compile Option  #
target_compile_options(exe PRIVATE ${CMAKE_CXX_FLAGS})

# Compile Cong #
target_compile_definitions(exe PRIVATE ${Config} BOOST_ERROR_CODE_HEADER_ONLY)

# Link Library  #
target_link_libraries(exe ${Library})

################### CMakeLists.txt Done ###################

################### Messgae ###################
message("###########################################")
message("#")
message("# Project Name   :: SN3D Project.")
message("# exe File       :: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message("# Version        :: ${VERSION_MAJOR}.${VERSION_MINOR}v")
message("# Build Option   :: C ${CMAKE_CXX_STANDARD} STANDARD ")
message("# Flags          :: ${CMAKE_CXX_FLAGS}")
message("# Configs        :: ${Config}")
message("#")
message("###########################################")
