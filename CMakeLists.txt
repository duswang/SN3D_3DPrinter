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

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED TRUE)
set (CMAKE_CXX_EXTENSIONS FALSE)

# Project Info  #
project (sn3d_project)
set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)


# Paths
set(PRJ_ROOT_PATH .)
set(APP_PATH ${PRJ_ROOT_PATH}/APP)
set(MODUEL_PATH ${PRJ_ROOT_PATH}/MODUEL)
set(SYSTEM_PATH ${PRJ_ROOT_PATH}/SYSTEM)
set(LIBRARY_PATH ${PRJ_ROOT_PATH}/LIBRARY)


# Cmkae Module #
set(CMAKE_MODULE_PATH "${PRJ_ROOT_PATH}/cmake/")

# Library #
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(libzip REQUIRED)

# Build Option #
set (CMAKE_CXX_STANDARD 11)
#set (CMAKE_CXX_FLAGS -D_POSIX_C_SOURCE=199309L)

# Output Set
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PRJ_ROOT_PATH}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PRJ_ROOT_PATH}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PRJ_ROOT_PATH}/lib)

# Include
include_directories( 
	${PRJ_ROOT_PATH}/INCLUDE
	${APP_PATH}/INCLUDE
	${MODUEL_PATH}/INCLUDE
	${SYSTEM_PATH}/INCLUDE
	${LIBRARY_PATH}/libusbp/include
)
# Include - Library
include_directories(
	${SDL2_INCLUDE_DIRS}
	${SDL2_IMAGE_INCLUDE_DIRS}
	${LIBZIP_INCLUDE_DIRS}
)
# Build Target

set(
	# Target Name #
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

add_executable(Project ${Source})

# CFlags 
target_compile_options(Project PRIVATE ${CMAKE_CXX_FLAGS})
target_compile_definitions(Project PRIVATE BOOST_ERROR_CODE_HEADER_ONLY)

# Library
target_link_libraries(Project pthread usb-1.0 ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${LIBZIP_LIBRARIES})


# Message
message("Project Name   :: SN3D Project.")
message("Version        :: ${VERSION_MAJOR}.${VERSION_MINOR}v")
message("Build Set      :: ${CMAKE_BUILD_TYPE}")
message("Build Target   :: ${CMAKE_BUILD_TARGET}")
message("Build Option   :: C${CMAKE_CXX_STANDARD} STANDARD ")
message("Flags(DEBUG)   :: CMAKE_CXX_FLAGS_DEBUG is ${CMAKE_CXX_FLAGS_DEBUG}")
message("Flags(RELEASE) :: CMAKE_CXX_FLAGS_RELEASE is ${CMAKE_CXX_FLAGS_RELEASE}")
message("Flags          :: CMAKE_CXX_FLAGS is ${CMAKE_CXX_FLAGS}")


