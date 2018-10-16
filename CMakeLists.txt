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
set(APP_PATH ${PRJ_ROOT_PATH}/APP)
set(MODUEL_PATH ${PRJ_ROOT_PATH}/MODUEL)
set(SYSTEM_PATH ${PRJ_ROOT_PATH}/SYSTEM)
set(LIBRARY_PATH ${PRJ_ROOT_PATH}/LIBRARY)

# Output Paths #
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PRJ_ROOT_PATH}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PRJ_ROOT_PATH}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PRJ_ROOT_PATH}/lib)


# Cmkae Module #
set(CMAKE_MODULE_PATH "${PRJ_ROOT_PATH}/cmake/")

################### Library & Source & Include Setting ###################
# Library #
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(libzip REQUIRED)

# Include - Local #
include_directories( 
	${PRJ_ROOT_PATH}/INCLUDE
	${APP_PATH}/INCLUDE
	${MODUEL_PATH}/INCLUDE
	${SYSTEM_PATH}/INCLUDE
)
# Include - Library #
include_directories(
	${SDL2_INCLUDE_DIRS}
	${SDL2_IMAGE_INCLUDE_DIRS}
	${LIBZIP_INCLUDE_DIRS}
)
# Source #
set(
	# Sources Name #
	Source
 
	# Applications #
	${APP_PATH}/APP_CONTROL.c
	${APP_PATH}/APP_FILE_SELECT.c
	${APP_PATH}/APP_MAIN.c
	${APP_PATH}/APP_PAUSE.c
	${APP_PATH}/APP_PRINTING.c
	${APP_PATH}/APP_WAITING.c
	${APP_PATH}/APP_INIT.c
	
	# Moduel #
	${MODUEL_PATH}/SN_MODUEL_3D_PRINTER.c
	${MODUEL_PATH}/SN_MODUEL_DISPLAY.c
	${MODUEL_PATH}/SN_MODUEL_FILE_SYSTEM.c
	${MODUEL_PATH}/SN_MODUEL_IMAGE_VIEWER.c
	
	# System #
	${SYSTEM_PATH}/SN_SYS_SERIAL_COMM.c
	${SYSTEM_PATH}/SN_SYS_MESSAGE_Q.c
	${SYSTEM_PATH}/SN_SYS_INIT.c
	${SYSTEM_PATH}/SN_SYS_USB_DRIVER.c
	${SYSTEM_PATH}/SN_SYS_TIMER.c
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
)

# Flags #
set(
	# Flags Name #
	CMAKE_CXX_FLAGS
	
	
)

# Config #

set(
	# Config Name #
	Config
	
	# IMAGE #
	-DIMAGE_VIEWER_IS_ON=false
	-DIMAGE_VIEWER_IS_FULL_SCREEN=false

    # 3D PRINTER #
    -DPRINTER_RX_DEBUG=true
    -DPRINTER_TX_DEBUG=false
    
    # DISPLAY #
    -DDISPLAY_RX_DEBUG=false
    -DDISPLAY_TX_DEBUG=false
    
)
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
message("# exe File       :: ./build/exe")
message("# Version        :: ${VERSION_MAJOR}.${VERSION_MINOR}v")
message("# Build Option   :: C ${CMAKE_CXX_STANDARD} STANDARD ")
message("# Flags          :: ${CMAKE_CXX_FLAGS}")
message("# Configs        :: ${Config}")
message("#")
message("###########################################")
