#
# ============================================================================
# Name        : sn3d_bootloader
# Author      : bato
# ============================================================================
#
######################## Base Info Setting ########################
# Header #
cmake_minimum_required(VERSION 3.7)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS FALSE)

# Project Info  #
project(sn3d_project)

set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)

set(PRJ_BINARY_NAME sn3d_bootloader)


# Paths #
set(PRJ_ROOT_PATH ..)
set(BOOTLOADER_ROOT_PATH .)
set(BOOTLOADER_SOURCE_PATH ${BOOTLOADER_ROOT_PATH}/src)
set(BOOTLOADER_BINARY_PATH ${BOOTLOADER_ROOT_PATH}/bin)

set(API_PATH      ${PRJ_SOURCE_PATH}/SN_API)
set(MODULE_PATH   ${API_PATH}/MODULE)
set(SYSTEM_PATH   ${API_PATH}/SYSTEM)

# Output Paths #
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BOOTLOADER_ROOT_PATH}/bin/build)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BOOTLOADER_ROOT_PATH}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BOOTLOADER_ROOT_PATH}/lib)

# cmake #
set(CMAKE_PATH "${PRJ_ROOT_PATH}/cmake")
# cmake module #
set(CMAKE_MODULE_PATH "${CMAKE_PATH}/module")

################### Library & Source & Include Setting ###################

# Library Package #
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(png REQUIRED)
find_package(libzip REQUIRED)
find_package(libxml2 REQUIRED)
find_package(libiconv REQUIRED)


# Include - Local #
include_directories(
	# API #
	${BOOTLOADER_SOURCE_PATH}/INCLUDE
	${BOOTLOADER_SOURCE_PATH}/FILE_SYSTEM/INCLUDE
	${BOOTLOADER_SOURCE_PATH}/SYSTEM/INCLUDE
	
)
# Include - Library #
include_directories(
	${LIBZIP_INCLUDE_DIRS}
	${LIBXML2_INCLUDE_DIRS}
	${LIBICONV_INCLUDE_DIRS}
)
# Source #
set(
	# Sources Name #
	Source

	# App Main #
	${BOOTLOADER_SOURCE_PATH}/SN_BOOTLOADER_MAIN.c
	${BOOTLOADER_SOURCE_PATH}/SN_VERSION_CONTROL.c
	${BOOTLOADER_SOURCE_PATH}/SN_MACHINE_CONTROL.c
	
	# File System #
	${BOOTLOADER_SOURCE_PATH}/FILE_SYSTEM/FILE_SYSTEM_FCTL.c
	${BOOTLOADER_SOURCE_PATH}/FILE_SYSTEM/FILE_SYSTEM_XML.c
	
	# System #
	${BOOTLOADER_SOURCE_PATH}/SYSTEM/SN_SYS_ERROR.c
)

# Library #
set(
	# Library Name #
	Library

	# ptrhead #
	pthread

	# openssl #
	ssl 
	
	crypto
	
	# libusb #
	usb-1.0

	# libzip #
	${LIBZIP_LIBRARIES}
	
	# XML #
	${LIBXML2_LIBRARIES}
	
	# ICONV #
	${LIBICONV_LIBRARYS}
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

# Mode #
set(
	# Mode Name #
	Mode
	
	# Debug #
	-DDEBUG_MODE=true
	
	# Demo #
	-DDEMO_MODE=false
	
	# Release #
	-DRELEASE_MODE=false
)

# Config #
set(
	# Config Name #
	Config
    
    # Mode #
    ${Mode}
)

# Command #
add_custom_target(gdb
	sudo gdb ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PRJ_BINARY_NAME}
)
add_custom_target(run 
	sudo ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PRJ_BINARY_NAME}
)

add_custom_target(start 
	sudo systemctl restart SN3D_Bootloader.service
)

add_custom_target(stop 
	sudo systemctl stop SN3D_Bootloader.service
)

add_custom_target(all_clean 
    sudo rm -rf
    ${BOOTLOADER_ROOT_PATH}/CMakeFiles
    ${BOOTLOADER_ROOT_PATH}/CMakeCache.tx 
    ${BOOTLOADER_ROOT_PATH}/cmake_install.cmake
    ${BOOTLOADER_ROOT_PATH}/Makefile  
    ${BOOTLOADER_ROOT_PATH}/res
    ${BOOTLOADER_ROOT_PATH}/bin  
)


################### Target Setting ###################
# Install exe file #
add_executable(${PRJ_BINARY_NAME} ${Source})

# Compile Option  #
target_compile_options(${PRJ_BINARY_NAME} PRIVATE ${CMAKE_CXX_FLAGS})

# Compile Config #
target_compile_definitions(${PRJ_BINARY_NAME} PRIVATE ${Config} BOOST_ERROR_CODE_HEADER_ONLY)

# Link Library  #
target_link_libraries(${PRJ_BINARY_NAME} ${Library})

################### CMakeLists.txt Done ###################

################### Messgae ###################
message("###########################################")
message("#")
message("# Project Name   :: SN3D Bootloader.")
message("# exe File       :: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message("# Version        :: ${VERSION_MAJOR}.${VERSION_MINOR}v")
message("# Build Option   :: C ${CMAKE_CXX_STANDARD} STANDARD ")
message("# Flags          :: ${CMAKE_CXX_FLAGS}")
message("# Configs        :: ${Config}")
message("#")
message("###########################################")
