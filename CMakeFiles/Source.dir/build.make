# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.7

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/pi/Documents/Workspace/sn3d-project

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pi/Documents/Workspace/sn3d-project

# Include any dependencies generated for this target.
include CMakeFiles/Source.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Source.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Source.dir/flags.make

CMakeFiles/Source.dir/APP/main.c.o: CMakeFiles/Source.dir/flags.make
CMakeFiles/Source.dir/APP/main.c.o: APP/main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/Documents/Workspace/sn3d-project/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/Source.dir/APP/main.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Source.dir/APP/main.c.o   -c /home/pi/Documents/Workspace/sn3d-project/APP/main.c

CMakeFiles/Source.dir/APP/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Source.dir/APP/main.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/pi/Documents/Workspace/sn3d-project/APP/main.c > CMakeFiles/Source.dir/APP/main.c.i

CMakeFiles/Source.dir/APP/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Source.dir/APP/main.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/pi/Documents/Workspace/sn3d-project/APP/main.c -o CMakeFiles/Source.dir/APP/main.c.s

CMakeFiles/Source.dir/APP/main.c.o.requires:

.PHONY : CMakeFiles/Source.dir/APP/main.c.o.requires

CMakeFiles/Source.dir/APP/main.c.o.provides: CMakeFiles/Source.dir/APP/main.c.o.requires
	$(MAKE) -f CMakeFiles/Source.dir/build.make CMakeFiles/Source.dir/APP/main.c.o.provides.build
.PHONY : CMakeFiles/Source.dir/APP/main.c.o.provides

CMakeFiles/Source.dir/APP/main.c.o.provides.build: CMakeFiles/Source.dir/APP/main.c.o


CMakeFiles/Source.dir/MODUELS/MODUEL_HW_UART.c.o: CMakeFiles/Source.dir/flags.make
CMakeFiles/Source.dir/MODUELS/MODUEL_HW_UART.c.o: MODUELS/MODUEL_HW_UART.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/Documents/Workspace/sn3d-project/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/Source.dir/MODUELS/MODUEL_HW_UART.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Source.dir/MODUELS/MODUEL_HW_UART.c.o   -c /home/pi/Documents/Workspace/sn3d-project/MODUELS/MODUEL_HW_UART.c

CMakeFiles/Source.dir/MODUELS/MODUEL_HW_UART.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Source.dir/MODUELS/MODUEL_HW_UART.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/pi/Documents/Workspace/sn3d-project/MODUELS/MODUEL_HW_UART.c > CMakeFiles/Source.dir/MODUELS/MODUEL_HW_UART.c.i

CMakeFiles/Source.dir/MODUELS/MODUEL_HW_UART.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Source.dir/MODUELS/MODUEL_HW_UART.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/pi/Documents/Workspace/sn3d-project/MODUELS/MODUEL_HW_UART.c -o CMakeFiles/Source.dir/MODUELS/MODUEL_HW_UART.c.s

CMakeFiles/Source.dir/MODUELS/MODUEL_HW_UART.c.o.requires:

.PHONY : CMakeFiles/Source.dir/MODUELS/MODUEL_HW_UART.c.o.requires

CMakeFiles/Source.dir/MODUELS/MODUEL_HW_UART.c.o.provides: CMakeFiles/Source.dir/MODUELS/MODUEL_HW_UART.c.o.requires
	$(MAKE) -f CMakeFiles/Source.dir/build.make CMakeFiles/Source.dir/MODUELS/MODUEL_HW_UART.c.o.provides.build
.PHONY : CMakeFiles/Source.dir/MODUELS/MODUEL_HW_UART.c.o.provides

CMakeFiles/Source.dir/MODUELS/MODUEL_HW_UART.c.o.provides.build: CMakeFiles/Source.dir/MODUELS/MODUEL_HW_UART.c.o


CMakeFiles/Source.dir/MODUELS/serial.c.o: CMakeFiles/Source.dir/flags.make
CMakeFiles/Source.dir/MODUELS/serial.c.o: MODUELS/serial.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/Documents/Workspace/sn3d-project/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/Source.dir/MODUELS/serial.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Source.dir/MODUELS/serial.c.o   -c /home/pi/Documents/Workspace/sn3d-project/MODUELS/serial.c

CMakeFiles/Source.dir/MODUELS/serial.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Source.dir/MODUELS/serial.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/pi/Documents/Workspace/sn3d-project/MODUELS/serial.c > CMakeFiles/Source.dir/MODUELS/serial.c.i

CMakeFiles/Source.dir/MODUELS/serial.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Source.dir/MODUELS/serial.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/pi/Documents/Workspace/sn3d-project/MODUELS/serial.c -o CMakeFiles/Source.dir/MODUELS/serial.c.s

CMakeFiles/Source.dir/MODUELS/serial.c.o.requires:

.PHONY : CMakeFiles/Source.dir/MODUELS/serial.c.o.requires

CMakeFiles/Source.dir/MODUELS/serial.c.o.provides: CMakeFiles/Source.dir/MODUELS/serial.c.o.requires
	$(MAKE) -f CMakeFiles/Source.dir/build.make CMakeFiles/Source.dir/MODUELS/serial.c.o.provides.build
.PHONY : CMakeFiles/Source.dir/MODUELS/serial.c.o.provides

CMakeFiles/Source.dir/MODUELS/serial.c.o.provides.build: CMakeFiles/Source.dir/MODUELS/serial.c.o


# Object files for target Source
Source_OBJECTS = \
"CMakeFiles/Source.dir/APP/main.c.o" \
"CMakeFiles/Source.dir/MODUELS/MODUEL_HW_UART.c.o" \
"CMakeFiles/Source.dir/MODUELS/serial.c.o"

# External object files for target Source
Source_EXTERNAL_OBJECTS =

build/Source: CMakeFiles/Source.dir/APP/main.c.o
build/Source: CMakeFiles/Source.dir/MODUELS/MODUEL_HW_UART.c.o
build/Source: CMakeFiles/Source.dir/MODUELS/serial.c.o
build/Source: CMakeFiles/Source.dir/build.make
build/Source: CMakeFiles/Source.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/pi/Documents/Workspace/sn3d-project/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking C executable build/Source"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Source.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Source.dir/build: build/Source

.PHONY : CMakeFiles/Source.dir/build

CMakeFiles/Source.dir/requires: CMakeFiles/Source.dir/APP/main.c.o.requires
CMakeFiles/Source.dir/requires: CMakeFiles/Source.dir/MODUELS/MODUEL_HW_UART.c.o.requires
CMakeFiles/Source.dir/requires: CMakeFiles/Source.dir/MODUELS/serial.c.o.requires

.PHONY : CMakeFiles/Source.dir/requires

CMakeFiles/Source.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Source.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Source.dir/clean

CMakeFiles/Source.dir/depend:
	cd /home/pi/Documents/Workspace/sn3d-project && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pi/Documents/Workspace/sn3d-project /home/pi/Documents/Workspace/sn3d-project /home/pi/Documents/Workspace/sn3d-project /home/pi/Documents/Workspace/sn3d-project /home/pi/Documents/Workspace/sn3d-project/CMakeFiles/Source.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Source.dir/depend
