# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_COMMAND = /snap/clion/112/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /snap/clion/112/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/franco/utnso/tp-2020-1c-El-Kuelgue

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/franco/utnso/tp-2020-1c-El-Kuelgue/cmake-build-debug

# Include any dependencies generated for this target.
include Gameboy/CMakeFiles/entry_point_validator.dir/depend.make

# Include the progress variables for this target.
include Gameboy/CMakeFiles/entry_point_validator.dir/progress.make

# Include the compile flags for this target's objects.
include Gameboy/CMakeFiles/entry_point_validator.dir/flags.make

Gameboy/CMakeFiles/entry_point_validator.dir/src/entry_point_validator.c.o: Gameboy/CMakeFiles/entry_point_validator.dir/flags.make
Gameboy/CMakeFiles/entry_point_validator.dir/src/entry_point_validator.c.o: ../Gameboy/src/entry_point_validator.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/franco/utnso/tp-2020-1c-El-Kuelgue/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object Gameboy/CMakeFiles/entry_point_validator.dir/src/entry_point_validator.c.o"
	cd /home/franco/utnso/tp-2020-1c-El-Kuelgue/cmake-build-debug/Gameboy && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/entry_point_validator.dir/src/entry_point_validator.c.o   -c /home/franco/utnso/tp-2020-1c-El-Kuelgue/Gameboy/src/entry_point_validator.c

Gameboy/CMakeFiles/entry_point_validator.dir/src/entry_point_validator.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/entry_point_validator.dir/src/entry_point_validator.c.i"
	cd /home/franco/utnso/tp-2020-1c-El-Kuelgue/cmake-build-debug/Gameboy && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/franco/utnso/tp-2020-1c-El-Kuelgue/Gameboy/src/entry_point_validator.c > CMakeFiles/entry_point_validator.dir/src/entry_point_validator.c.i

Gameboy/CMakeFiles/entry_point_validator.dir/src/entry_point_validator.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/entry_point_validator.dir/src/entry_point_validator.c.s"
	cd /home/franco/utnso/tp-2020-1c-El-Kuelgue/cmake-build-debug/Gameboy && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/franco/utnso/tp-2020-1c-El-Kuelgue/Gameboy/src/entry_point_validator.c -o CMakeFiles/entry_point_validator.dir/src/entry_point_validator.c.s

# Object files for target entry_point_validator
entry_point_validator_OBJECTS = \
"CMakeFiles/entry_point_validator.dir/src/entry_point_validator.c.o"

# External object files for target entry_point_validator
entry_point_validator_EXTERNAL_OBJECTS =

Gameboy/libentry_point_validator.a: Gameboy/CMakeFiles/entry_point_validator.dir/src/entry_point_validator.c.o
Gameboy/libentry_point_validator.a: Gameboy/CMakeFiles/entry_point_validator.dir/build.make
Gameboy/libentry_point_validator.a: Gameboy/CMakeFiles/entry_point_validator.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/franco/utnso/tp-2020-1c-El-Kuelgue/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library libentry_point_validator.a"
	cd /home/franco/utnso/tp-2020-1c-El-Kuelgue/cmake-build-debug/Gameboy && $(CMAKE_COMMAND) -P CMakeFiles/entry_point_validator.dir/cmake_clean_target.cmake
	cd /home/franco/utnso/tp-2020-1c-El-Kuelgue/cmake-build-debug/Gameboy && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/entry_point_validator.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Gameboy/CMakeFiles/entry_point_validator.dir/build: Gameboy/libentry_point_validator.a

.PHONY : Gameboy/CMakeFiles/entry_point_validator.dir/build

Gameboy/CMakeFiles/entry_point_validator.dir/clean:
	cd /home/franco/utnso/tp-2020-1c-El-Kuelgue/cmake-build-debug/Gameboy && $(CMAKE_COMMAND) -P CMakeFiles/entry_point_validator.dir/cmake_clean.cmake
.PHONY : Gameboy/CMakeFiles/entry_point_validator.dir/clean

Gameboy/CMakeFiles/entry_point_validator.dir/depend:
	cd /home/franco/utnso/tp-2020-1c-El-Kuelgue/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/franco/utnso/tp-2020-1c-El-Kuelgue /home/franco/utnso/tp-2020-1c-El-Kuelgue/Gameboy /home/franco/utnso/tp-2020-1c-El-Kuelgue/cmake-build-debug /home/franco/utnso/tp-2020-1c-El-Kuelgue/cmake-build-debug/Gameboy /home/franco/utnso/tp-2020-1c-El-Kuelgue/cmake-build-debug/Gameboy/CMakeFiles/entry_point_validator.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Gameboy/CMakeFiles/entry_point_validator.dir/depend

