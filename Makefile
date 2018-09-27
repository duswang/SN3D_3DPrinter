# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.7

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


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

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "No interactive CMake dialog available..."
	/usr/bin/cmake -E echo No\ interactive\ CMake\ dialog\ available.
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/pi/Documents/Workspace/sn3d-project/CMakeFiles /home/pi/Documents/Workspace/sn3d-project/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/pi/Documents/Workspace/sn3d-project/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named Source

# Build rule for target.
Source: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 Source
.PHONY : Source

# fast build rule for target.
Source/fast:
	$(MAKE) -f CMakeFiles/Source.dir/build.make CMakeFiles/Source.dir/build
.PHONY : Source/fast

APP/main.o: APP/main.c.o

.PHONY : APP/main.o

# target to build an object file
APP/main.c.o:
	$(MAKE) -f CMakeFiles/Source.dir/build.make CMakeFiles/Source.dir/APP/main.c.o
.PHONY : APP/main.c.o

APP/main.i: APP/main.c.i

.PHONY : APP/main.i

# target to preprocess a source file
APP/main.c.i:
	$(MAKE) -f CMakeFiles/Source.dir/build.make CMakeFiles/Source.dir/APP/main.c.i
.PHONY : APP/main.c.i

APP/main.s: APP/main.c.s

.PHONY : APP/main.s

# target to generate assembly for a file
APP/main.c.s:
	$(MAKE) -f CMakeFiles/Source.dir/build.make CMakeFiles/Source.dir/APP/main.c.s
.PHONY : APP/main.c.s

MODUELS/MODUEL_3D_PRINTER.o: MODUELS/MODUEL_3D_PRINTER.c.o

.PHONY : MODUELS/MODUEL_3D_PRINTER.o

# target to build an object file
MODUELS/MODUEL_3D_PRINTER.c.o:
	$(MAKE) -f CMakeFiles/Source.dir/build.make CMakeFiles/Source.dir/MODUELS/MODUEL_3D_PRINTER.c.o
.PHONY : MODUELS/MODUEL_3D_PRINTER.c.o

MODUELS/MODUEL_3D_PRINTER.i: MODUELS/MODUEL_3D_PRINTER.c.i

.PHONY : MODUELS/MODUEL_3D_PRINTER.i

# target to preprocess a source file
MODUELS/MODUEL_3D_PRINTER.c.i:
	$(MAKE) -f CMakeFiles/Source.dir/build.make CMakeFiles/Source.dir/MODUELS/MODUEL_3D_PRINTER.c.i
.PHONY : MODUELS/MODUEL_3D_PRINTER.c.i

MODUELS/MODUEL_3D_PRINTER.s: MODUELS/MODUEL_3D_PRINTER.c.s

.PHONY : MODUELS/MODUEL_3D_PRINTER.s

# target to generate assembly for a file
MODUELS/MODUEL_3D_PRINTER.c.s:
	$(MAKE) -f CMakeFiles/Source.dir/build.make CMakeFiles/Source.dir/MODUELS/MODUEL_3D_PRINTER.c.s
.PHONY : MODUELS/MODUEL_3D_PRINTER.c.s

MODUELS/MODUEL_DISPLAY.o: MODUELS/MODUEL_DISPLAY.c.o

.PHONY : MODUELS/MODUEL_DISPLAY.o

# target to build an object file
MODUELS/MODUEL_DISPLAY.c.o:
	$(MAKE) -f CMakeFiles/Source.dir/build.make CMakeFiles/Source.dir/MODUELS/MODUEL_DISPLAY.c.o
.PHONY : MODUELS/MODUEL_DISPLAY.c.o

MODUELS/MODUEL_DISPLAY.i: MODUELS/MODUEL_DISPLAY.c.i

.PHONY : MODUELS/MODUEL_DISPLAY.i

# target to preprocess a source file
MODUELS/MODUEL_DISPLAY.c.i:
	$(MAKE) -f CMakeFiles/Source.dir/build.make CMakeFiles/Source.dir/MODUELS/MODUEL_DISPLAY.c.i
.PHONY : MODUELS/MODUEL_DISPLAY.c.i

MODUELS/MODUEL_DISPLAY.s: MODUELS/MODUEL_DISPLAY.c.s

.PHONY : MODUELS/MODUEL_DISPLAY.s

# target to generate assembly for a file
MODUELS/MODUEL_DISPLAY.c.s:
	$(MAKE) -f CMakeFiles/Source.dir/build.make CMakeFiles/Source.dir/MODUELS/MODUEL_DISPLAY.c.s
.PHONY : MODUELS/MODUEL_DISPLAY.c.s

MODUELS/SERIAL_COMM.o: MODUELS/SERIAL_COMM.c.o

.PHONY : MODUELS/SERIAL_COMM.o

# target to build an object file
MODUELS/SERIAL_COMM.c.o:
	$(MAKE) -f CMakeFiles/Source.dir/build.make CMakeFiles/Source.dir/MODUELS/SERIAL_COMM.c.o
.PHONY : MODUELS/SERIAL_COMM.c.o

MODUELS/SERIAL_COMM.i: MODUELS/SERIAL_COMM.c.i

.PHONY : MODUELS/SERIAL_COMM.i

# target to preprocess a source file
MODUELS/SERIAL_COMM.c.i:
	$(MAKE) -f CMakeFiles/Source.dir/build.make CMakeFiles/Source.dir/MODUELS/SERIAL_COMM.c.i
.PHONY : MODUELS/SERIAL_COMM.c.i

MODUELS/SERIAL_COMM.s: MODUELS/SERIAL_COMM.c.s

.PHONY : MODUELS/SERIAL_COMM.s

# target to generate assembly for a file
MODUELS/SERIAL_COMM.c.s:
	$(MAKE) -f CMakeFiles/Source.dir/build.make CMakeFiles/Source.dir/MODUELS/SERIAL_COMM.c.s
.PHONY : MODUELS/SERIAL_COMM.c.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... rebuild_cache"
	@echo "... edit_cache"
	@echo "... Source"
	@echo "... APP/main.o"
	@echo "... APP/main.i"
	@echo "... APP/main.s"
	@echo "... MODUELS/MODUEL_3D_PRINTER.o"
	@echo "... MODUELS/MODUEL_3D_PRINTER.i"
	@echo "... MODUELS/MODUEL_3D_PRINTER.s"
	@echo "... MODUELS/MODUEL_DISPLAY.o"
	@echo "... MODUELS/MODUEL_DISPLAY.i"
	@echo "... MODUELS/MODUEL_DISPLAY.s"
	@echo "... MODUELS/SERIAL_COMM.o"
	@echo "... MODUELS/SERIAL_COMM.i"
	@echo "... MODUELS/SERIAL_COMM.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

