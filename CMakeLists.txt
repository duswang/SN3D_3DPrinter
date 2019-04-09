#
# ============================================================================
# Name        : sn3d_project
# Author      : bato
# ============================================================================
#
######################## Base Info Setting ########################
# Header #
cmake_minimum_required(VERSION 3.7)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS FALSE)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_VERBOSE_MAKEFILE false)

# Project Info  #
project(sn3d_project)

set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)

set(PRJ_BINARY_NAME sn3d)

# Paths #
set(PRJ_INSTALLED_PATH "/SN3D/sn3d-project")

set(PRJ_ROOT_PATH .)
set(PRJ_SOURCE_PATH ${PRJ_ROOT_PATH}/src)
set(PRJ_BINARY_PATH ${PRJ_ROOT_PATH}/bin)

set(APP_PATH      ${PRJ_SOURCE_PATH}/APP)
set(APPS_PATH     ${APP_PATH}/APPS)

set(API_PATH      ${PRJ_SOURCE_PATH}/SN_API)
set(MODULE_PATH   ${API_PATH}/MODULE)
set(SYSTEM_PATH   ${API_PATH}/SYSTEM)

# Output Paths #
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PRJ_BINARY_PATH}/build)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PRJ_BINARY_PATH}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PRJ_BINARY_PATH}/lib)

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
	${PRJ_SOURCE_PATH}/INCLUDE
	
	# System #
	${SYSTEM_PATH}/INCLUDE
	
	# Module #
	${MODULE_PATH}/INCLUDE
	
	# Module :: File System #
	${MODULE_PATH}/FILE_SYSTEM/INCLUDE
	
	# Module :: Image Viewer #
	${MODULE_PATH}/IMAGE_VIEWER/INCLUDE
	
	# App #
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
	${LIBICONV_INCLUDE_DIRS}
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
	${APPS_PATH}/APP_INFO.c

	# Moduel :: 3D Printer #
	${MODULE_PATH}/SN_MODULE_3D_PRINTER.c
	
	# Module :: Display #
	${MODULE_PATH}/SN_MODULE_DISPLAY.c


	# Module :: File System #
	${MODULE_PATH}/SN_MODULE_FILE_SYSTEM.c
		
	${MODULE_PATH}/FILE_SYSTEM/FILE_SYSTEM_FCTL.c
	${MODULE_PATH}/FILE_SYSTEM/FILE_SYSTEM_XML.c
	${MODULE_PATH}/FILE_SYSTEM/FILE_SYSTEM_PAGE.c
	
	# Module :: Image Viewer #
	${MODULE_PATH}/SN_MODULE_IMAGE_VIEWER.c
	
	${MODULE_PATH}/IMAGE_VIEWER/IMAGE_VIEWER_FB.c
	${MODULE_PATH}/IMAGE_VIEWER/IMAGE_VIEWER_SDL.c
	
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

	# openssl #
	ssl 
	crypto
	
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
	-W
	

)

set(CMAKE_SHARED_LINKER_FLAGS
	# Map #
	-Wl
	 -Xlinker -map=output.map
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

	# IMAGE #
	-DIMAGE_VIEWER_OFF=false

    # 3D PRINTER #
    -DPRINTER_RX_DEBUG=false
    -DPRINTER_TX_DEBUG=false
	-DMOTOR_DISALBE=false

    # DISPLAY #
    -DDISPLAY_RX_DEBUG=false
	-DDISPLAY_TX_DEBUG=false
    
    # IMAGE VIEWER #
    -DIMAGE_VIEWER_FIX_RESOLUTION=true
    -DIMAGE_VIEWER_USE_SDL=false
    
    # Mode #
    ${Mode}
)

# Command #
add_custom_target(gdb
    make all &&
    sudo gdb ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PRJ_BINARY_NAME}
)
add_custom_target(run
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PRJ_BINARY_NAME}
)

add_custom_target(start 
    sudo systemctl restart SN3D.service
)

add_custom_target(stop
    sudo systemctl stop SN3D.service
)

add_custom_target(doxygen
    doxygen ${PRJ_ROOT_PATH}/lib/sn3d_doxygen
)

add_custom_target(all_clean 
    sudo rm -rf 
    ${PRJ_ROOT_PATH}/CMakeFiles 
    ${PRJ_ROOT_PATH}/CMakeCache.txt 
    ${PRJ_ROOT_PATH}/cmake_install.cmake 
    ${PRJ_ROOT_PATH}/Makefile 
    ${PRJ_ROOT_PATH}/res 
    ${PRJ_ROOT_PATH}/FirmwareFiles 
)

add_custom_target(sn3d_service_install 
    sudo cp ${PRJ_ROOT_PATH}/lib/service/SN3D.service /etc/systemd/system/SN3D.service &&
    sudo cp ${PRJ_ROOT_PATH}/lib/service/SN3D_Bootloader.service /etc/systemd/system/SN3D_Bootloader.service &&
    sudo systemctl daemon-reload && 
    sudo systemctl enable SN3D.service &&
    sudo systemctl enable SN3D_Bootloader.service
)

add_custom_target(sn3d_service_uninstall 
    sudo systemctl disable SN3D.service &&
    sudo systemctl disable SN3D_Bootloader.service &&
    sudo rm -rf /etc/systemd/system/SN3D.service &&
    sudo rm -rf /etc/systemd/system/SN3D_Bootloader.service &&
    sudo systemctl daemon-reload
)

add_custom_target(sn3d_resize2fs_install
    sudo cp ${PRJ_ROOT_PATH}/lib/service/resize2fs_once /etc/init.d/resize2fs_once &&
    sudo chmod +x /etc/init.d/resize2fs_once &&
    sudo systemctl enable resize2fs_once
)

add_custom_target(sn3d_resize2fs_uninstall
    sudo systemctl disable resize2fs_once &&
    sudo update-rc.d resize2fs_once remove &&
    sudo rm -rf /etc/init.d/resize2fs_once
)

add_custom_target(sn3d_install
    sudo make install &&
    sudo make sn3d_service_install
)

# install # 

install( DIRECTORY ${PRJ_ROOT_PATH}
    DESTINATION ${PRJ_INSTALLED_PATH}
    COMPONENT sn3d_install
    PATTERN .svn EXCLUDE
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
message("# Project Name   :: SN3D Project.")
message("# exe File       :: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message("# Version        :: ${VERSION_MAJOR}.${VERSION_MINOR}v")
message("# Build Option   :: C ${CMAKE_CXX_STANDARD} STANDARD ")
message("# Flags          :: ${CMAKE_CXX_FLAGS}")
message("# Configs        :: ${Config}")
message("#")
message("###########################################")
