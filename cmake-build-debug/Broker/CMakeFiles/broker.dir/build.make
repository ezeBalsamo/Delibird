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
include Broker/CMakeFiles/broker.dir/depend.make

# Include the progress variables for this target.
include Broker/CMakeFiles/broker.dir/progress.make

# Include the compile flags for this target's objects.
include Broker/CMakeFiles/broker.dir/flags.make

Broker/CMakeFiles/broker.dir/src/broker.c.o: Broker/CMakeFiles/broker.dir/flags.make
Broker/CMakeFiles/broker.dir/src/broker.c.o: ../Broker/src/broker.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/franco/utnso/tp-2020-1c-El-Kuelgue/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object Broker/CMakeFiles/broker.dir/src/broker.c.o"
	cd /home/franco/utnso/tp-2020-1c-El-Kuelgue/cmake-build-debug/Broker && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/broker.dir/src/broker.c.o   -c /home/franco/utnso/tp-2020-1c-El-Kuelgue/Broker/src/broker.c

Broker/CMakeFiles/broker.dir/src/broker.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/broker.dir/src/broker.c.i"
	cd /home/franco/utnso/tp-2020-1c-El-Kuelgue/cmake-build-debug/Broker && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/franco/utnso/tp-2020-1c-El-Kuelgue/Broker/src/broker.c > CMakeFiles/broker.dir/src/broker.c.i

Broker/CMakeFiles/broker.dir/src/broker.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/broker.dir/src/broker.c.s"
	cd /home/franco/utnso/tp-2020-1c-El-Kuelgue/cmake-build-debug/Broker && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/franco/utnso/tp-2020-1c-El-Kuelgue/Broker/src/broker.c -o CMakeFiles/broker.dir/src/broker.c.s

Broker/CMakeFiles/broker.dir/src/broker_deserialization.c.o: Broker/CMakeFiles/broker.dir/flags.make
Broker/CMakeFiles/broker.dir/src/broker_deserialization.c.o: ../Broker/src/broker_deserialization.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/franco/utnso/tp-2020-1c-El-Kuelgue/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object Broker/CMakeFiles/broker.dir/src/broker_deserialization.c.o"
	cd /home/franco/utnso/tp-2020-1c-El-Kuelgue/cmake-build-debug/Broker && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/broker.dir/src/broker_deserialization.c.o   -c /home/franco/utnso/tp-2020-1c-El-Kuelgue/Broker/src/broker_deserialization.c

Broker/CMakeFiles/broker.dir/src/broker_deserialization.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/broker.dir/src/broker_deserialization.c.i"
	cd /home/franco/utnso/tp-2020-1c-El-Kuelgue/cmake-build-debug/Broker && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/franco/utnso/tp-2020-1c-El-Kuelgue/Broker/src/broker_deserialization.c > CMakeFiles/broker.dir/src/broker_deserialization.c.i

Broker/CMakeFiles/broker.dir/src/broker_deserialization.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/broker.dir/src/broker_deserialization.c.s"
	cd /home/franco/utnso/tp-2020-1c-El-Kuelgue/cmake-build-debug/Broker && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/franco/utnso/tp-2020-1c-El-Kuelgue/Broker/src/broker_deserialization.c -o CMakeFiles/broker.dir/src/broker_deserialization.c.s

Broker/CMakeFiles/broker.dir/src/queue_message_manager.c.o: Broker/CMakeFiles/broker.dir/flags.make
Broker/CMakeFiles/broker.dir/src/queue_message_manager.c.o: ../Broker/src/queue_message_manager.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/franco/utnso/tp-2020-1c-El-Kuelgue/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object Broker/CMakeFiles/broker.dir/src/queue_message_manager.c.o"
	cd /home/franco/utnso/tp-2020-1c-El-Kuelgue/cmake-build-debug/Broker && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/broker.dir/src/queue_message_manager.c.o   -c /home/franco/utnso/tp-2020-1c-El-Kuelgue/Broker/src/queue_message_manager.c

Broker/CMakeFiles/broker.dir/src/queue_message_manager.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/broker.dir/src/queue_message_manager.c.i"
	cd /home/franco/utnso/tp-2020-1c-El-Kuelgue/cmake-build-debug/Broker && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/franco/utnso/tp-2020-1c-El-Kuelgue/Broker/src/queue_message_manager.c > CMakeFiles/broker.dir/src/queue_message_manager.c.i

Broker/CMakeFiles/broker.dir/src/queue_message_manager.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/broker.dir/src/queue_message_manager.c.s"
	cd /home/franco/utnso/tp-2020-1c-El-Kuelgue/cmake-build-debug/Broker && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/franco/utnso/tp-2020-1c-El-Kuelgue/Broker/src/queue_message_manager.c -o CMakeFiles/broker.dir/src/queue_message_manager.c.s

Broker/CMakeFiles/broker.dir/src/connection_handler.c.o: Broker/CMakeFiles/broker.dir/flags.make
Broker/CMakeFiles/broker.dir/src/connection_handler.c.o: ../Broker/src/connection_handler.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/franco/utnso/tp-2020-1c-El-Kuelgue/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object Broker/CMakeFiles/broker.dir/src/connection_handler.c.o"
	cd /home/franco/utnso/tp-2020-1c-El-Kuelgue/cmake-build-debug/Broker && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/broker.dir/src/connection_handler.c.o   -c /home/franco/utnso/tp-2020-1c-El-Kuelgue/Broker/src/connection_handler.c

Broker/CMakeFiles/broker.dir/src/connection_handler.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/broker.dir/src/connection_handler.c.i"
	cd /home/franco/utnso/tp-2020-1c-El-Kuelgue/cmake-build-debug/Broker && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/franco/utnso/tp-2020-1c-El-Kuelgue/Broker/src/connection_handler.c > CMakeFiles/broker.dir/src/connection_handler.c.i

Broker/CMakeFiles/broker.dir/src/connection_handler.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/broker.dir/src/connection_handler.c.s"
	cd /home/franco/utnso/tp-2020-1c-El-Kuelgue/cmake-build-debug/Broker && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/franco/utnso/tp-2020-1c-El-Kuelgue/Broker/src/connection_handler.c -o CMakeFiles/broker.dir/src/connection_handler.c.s

Broker/CMakeFiles/broker.dir/src/broker_logs_manager.c.o: Broker/CMakeFiles/broker.dir/flags.make
Broker/CMakeFiles/broker.dir/src/broker_logs_manager.c.o: ../Broker/src/broker_logs_manager.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/franco/utnso/tp-2020-1c-El-Kuelgue/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object Broker/CMakeFiles/broker.dir/src/broker_logs_manager.c.o"
	cd /home/franco/utnso/tp-2020-1c-El-Kuelgue/cmake-build-debug/Broker && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/broker.dir/src/broker_logs_manager.c.o   -c /home/franco/utnso/tp-2020-1c-El-Kuelgue/Broker/src/broker_logs_manager.c

Broker/CMakeFiles/broker.dir/src/broker_logs_manager.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/broker.dir/src/broker_logs_manager.c.i"
	cd /home/franco/utnso/tp-2020-1c-El-Kuelgue/cmake-build-debug/Broker && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/franco/utnso/tp-2020-1c-El-Kuelgue/Broker/src/broker_logs_manager.c > CMakeFiles/broker.dir/src/broker_logs_manager.c.i

Broker/CMakeFiles/broker.dir/src/broker_logs_manager.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/broker.dir/src/broker_logs_manager.c.s"
	cd /home/franco/utnso/tp-2020-1c-El-Kuelgue/cmake-build-debug/Broker && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/franco/utnso/tp-2020-1c-El-Kuelgue/Broker/src/broker_logs_manager.c -o CMakeFiles/broker.dir/src/broker_logs_manager.c.s

# Object files for target broker
broker_OBJECTS = \
"CMakeFiles/broker.dir/src/broker.c.o" \
"CMakeFiles/broker.dir/src/broker_deserialization.c.o" \
"CMakeFiles/broker.dir/src/queue_message_manager.c.o" \
"CMakeFiles/broker.dir/src/connection_handler.c.o" \
"CMakeFiles/broker.dir/src/broker_logs_manager.c.o"

# External object files for target broker
broker_EXTERNAL_OBJECTS =

../Broker/target/broker: Broker/CMakeFiles/broker.dir/src/broker.c.o
../Broker/target/broker: Broker/CMakeFiles/broker.dir/src/broker_deserialization.c.o
../Broker/target/broker: Broker/CMakeFiles/broker.dir/src/queue_message_manager.c.o
../Broker/target/broker: Broker/CMakeFiles/broker.dir/src/connection_handler.c.o
../Broker/target/broker: Broker/CMakeFiles/broker.dir/src/broker_logs_manager.c.o
../Broker/target/broker: Broker/CMakeFiles/broker.dir/build.make
../Broker/target/broker: ../Utils/target/libutils.so
../Broker/target/broker: Broker/CMakeFiles/broker.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/franco/utnso/tp-2020-1c-El-Kuelgue/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking C executable ../../Broker/target/broker"
	cd /home/franco/utnso/tp-2020-1c-El-Kuelgue/cmake-build-debug/Broker && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/broker.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Broker/CMakeFiles/broker.dir/build: ../Broker/target/broker

.PHONY : Broker/CMakeFiles/broker.dir/build

Broker/CMakeFiles/broker.dir/clean:
	cd /home/franco/utnso/tp-2020-1c-El-Kuelgue/cmake-build-debug/Broker && $(CMAKE_COMMAND) -P CMakeFiles/broker.dir/cmake_clean.cmake
.PHONY : Broker/CMakeFiles/broker.dir/clean

Broker/CMakeFiles/broker.dir/depend:
	cd /home/franco/utnso/tp-2020-1c-El-Kuelgue/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/franco/utnso/tp-2020-1c-El-Kuelgue /home/franco/utnso/tp-2020-1c-El-Kuelgue/Broker /home/franco/utnso/tp-2020-1c-El-Kuelgue/cmake-build-debug /home/franco/utnso/tp-2020-1c-El-Kuelgue/cmake-build-debug/Broker /home/franco/utnso/tp-2020-1c-El-Kuelgue/cmake-build-debug/Broker/CMakeFiles/broker.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Broker/CMakeFiles/broker.dir/depend

