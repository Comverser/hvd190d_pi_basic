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
CMAKE_SOURCE_DIR = /home/pi/dev_s/hvd190d_pi_basic

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pi/dev_s/hvd190d_pi_basic/build

# Include any dependencies generated for this target.
include CMakeFiles/demo.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/demo.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/demo.dir/flags.make

CMakeFiles/demo.dir/src/apps/main.cpp.o: CMakeFiles/demo.dir/flags.make
CMakeFiles/demo.dir/src/apps/main.cpp.o: ../src/apps/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/dev_s/hvd190d_pi_basic/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/demo.dir/src/apps/main.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/demo.dir/src/apps/main.cpp.o -c /home/pi/dev_s/hvd190d_pi_basic/src/apps/main.cpp

CMakeFiles/demo.dir/src/apps/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/demo.dir/src/apps/main.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pi/dev_s/hvd190d_pi_basic/src/apps/main.cpp > CMakeFiles/demo.dir/src/apps/main.cpp.i

CMakeFiles/demo.dir/src/apps/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/demo.dir/src/apps/main.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pi/dev_s/hvd190d_pi_basic/src/apps/main.cpp -o CMakeFiles/demo.dir/src/apps/main.cpp.s

CMakeFiles/demo.dir/src/apps/main.cpp.o.requires:

.PHONY : CMakeFiles/demo.dir/src/apps/main.cpp.o.requires

CMakeFiles/demo.dir/src/apps/main.cpp.o.provides: CMakeFiles/demo.dir/src/apps/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/demo.dir/build.make CMakeFiles/demo.dir/src/apps/main.cpp.o.provides.build
.PHONY : CMakeFiles/demo.dir/src/apps/main.cpp.o.provides

CMakeFiles/demo.dir/src/apps/main.cpp.o.provides.build: CMakeFiles/demo.dir/src/apps/main.cpp.o


CMakeFiles/demo.dir/src/libs/hvd190d_pi_driv.cpp.o: CMakeFiles/demo.dir/flags.make
CMakeFiles/demo.dir/src/libs/hvd190d_pi_driv.cpp.o: ../src/libs/hvd190d_pi_driv.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/dev_s/hvd190d_pi_basic/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/demo.dir/src/libs/hvd190d_pi_driv.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/demo.dir/src/libs/hvd190d_pi_driv.cpp.o -c /home/pi/dev_s/hvd190d_pi_basic/src/libs/hvd190d_pi_driv.cpp

CMakeFiles/demo.dir/src/libs/hvd190d_pi_driv.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/demo.dir/src/libs/hvd190d_pi_driv.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pi/dev_s/hvd190d_pi_basic/src/libs/hvd190d_pi_driv.cpp > CMakeFiles/demo.dir/src/libs/hvd190d_pi_driv.cpp.i

CMakeFiles/demo.dir/src/libs/hvd190d_pi_driv.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/demo.dir/src/libs/hvd190d_pi_driv.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pi/dev_s/hvd190d_pi_basic/src/libs/hvd190d_pi_driv.cpp -o CMakeFiles/demo.dir/src/libs/hvd190d_pi_driv.cpp.s

CMakeFiles/demo.dir/src/libs/hvd190d_pi_driv.cpp.o.requires:

.PHONY : CMakeFiles/demo.dir/src/libs/hvd190d_pi_driv.cpp.o.requires

CMakeFiles/demo.dir/src/libs/hvd190d_pi_driv.cpp.o.provides: CMakeFiles/demo.dir/src/libs/hvd190d_pi_driv.cpp.o.requires
	$(MAKE) -f CMakeFiles/demo.dir/build.make CMakeFiles/demo.dir/src/libs/hvd190d_pi_driv.cpp.o.provides.build
.PHONY : CMakeFiles/demo.dir/src/libs/hvd190d_pi_driv.cpp.o.provides

CMakeFiles/demo.dir/src/libs/hvd190d_pi_driv.cpp.o.provides.build: CMakeFiles/demo.dir/src/libs/hvd190d_pi_driv.cpp.o


CMakeFiles/demo.dir/src/libs/hvd190d_pi_wf.cpp.o: CMakeFiles/demo.dir/flags.make
CMakeFiles/demo.dir/src/libs/hvd190d_pi_wf.cpp.o: ../src/libs/hvd190d_pi_wf.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/dev_s/hvd190d_pi_basic/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/demo.dir/src/libs/hvd190d_pi_wf.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/demo.dir/src/libs/hvd190d_pi_wf.cpp.o -c /home/pi/dev_s/hvd190d_pi_basic/src/libs/hvd190d_pi_wf.cpp

CMakeFiles/demo.dir/src/libs/hvd190d_pi_wf.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/demo.dir/src/libs/hvd190d_pi_wf.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pi/dev_s/hvd190d_pi_basic/src/libs/hvd190d_pi_wf.cpp > CMakeFiles/demo.dir/src/libs/hvd190d_pi_wf.cpp.i

CMakeFiles/demo.dir/src/libs/hvd190d_pi_wf.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/demo.dir/src/libs/hvd190d_pi_wf.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pi/dev_s/hvd190d_pi_basic/src/libs/hvd190d_pi_wf.cpp -o CMakeFiles/demo.dir/src/libs/hvd190d_pi_wf.cpp.s

CMakeFiles/demo.dir/src/libs/hvd190d_pi_wf.cpp.o.requires:

.PHONY : CMakeFiles/demo.dir/src/libs/hvd190d_pi_wf.cpp.o.requires

CMakeFiles/demo.dir/src/libs/hvd190d_pi_wf.cpp.o.provides: CMakeFiles/demo.dir/src/libs/hvd190d_pi_wf.cpp.o.requires
	$(MAKE) -f CMakeFiles/demo.dir/build.make CMakeFiles/demo.dir/src/libs/hvd190d_pi_wf.cpp.o.provides.build
.PHONY : CMakeFiles/demo.dir/src/libs/hvd190d_pi_wf.cpp.o.provides

CMakeFiles/demo.dir/src/libs/hvd190d_pi_wf.cpp.o.provides.build: CMakeFiles/demo.dir/src/libs/hvd190d_pi_wf.cpp.o


CMakeFiles/demo.dir/src/libs/koc_wf_gen.cpp.o: CMakeFiles/demo.dir/flags.make
CMakeFiles/demo.dir/src/libs/koc_wf_gen.cpp.o: ../src/libs/koc_wf_gen.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/dev_s/hvd190d_pi_basic/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/demo.dir/src/libs/koc_wf_gen.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/demo.dir/src/libs/koc_wf_gen.cpp.o -c /home/pi/dev_s/hvd190d_pi_basic/src/libs/koc_wf_gen.cpp

CMakeFiles/demo.dir/src/libs/koc_wf_gen.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/demo.dir/src/libs/koc_wf_gen.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pi/dev_s/hvd190d_pi_basic/src/libs/koc_wf_gen.cpp > CMakeFiles/demo.dir/src/libs/koc_wf_gen.cpp.i

CMakeFiles/demo.dir/src/libs/koc_wf_gen.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/demo.dir/src/libs/koc_wf_gen.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pi/dev_s/hvd190d_pi_basic/src/libs/koc_wf_gen.cpp -o CMakeFiles/demo.dir/src/libs/koc_wf_gen.cpp.s

CMakeFiles/demo.dir/src/libs/koc_wf_gen.cpp.o.requires:

.PHONY : CMakeFiles/demo.dir/src/libs/koc_wf_gen.cpp.o.requires

CMakeFiles/demo.dir/src/libs/koc_wf_gen.cpp.o.provides: CMakeFiles/demo.dir/src/libs/koc_wf_gen.cpp.o.requires
	$(MAKE) -f CMakeFiles/demo.dir/build.make CMakeFiles/demo.dir/src/libs/koc_wf_gen.cpp.o.provides.build
.PHONY : CMakeFiles/demo.dir/src/libs/koc_wf_gen.cpp.o.provides

CMakeFiles/demo.dir/src/libs/koc_wf_gen.cpp.o.provides.build: CMakeFiles/demo.dir/src/libs/koc_wf_gen.cpp.o


CMakeFiles/demo.dir/src/libs/rpi_reg_access.cpp.o: CMakeFiles/demo.dir/flags.make
CMakeFiles/demo.dir/src/libs/rpi_reg_access.cpp.o: ../src/libs/rpi_reg_access.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/dev_s/hvd190d_pi_basic/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/demo.dir/src/libs/rpi_reg_access.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/demo.dir/src/libs/rpi_reg_access.cpp.o -c /home/pi/dev_s/hvd190d_pi_basic/src/libs/rpi_reg_access.cpp

CMakeFiles/demo.dir/src/libs/rpi_reg_access.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/demo.dir/src/libs/rpi_reg_access.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pi/dev_s/hvd190d_pi_basic/src/libs/rpi_reg_access.cpp > CMakeFiles/demo.dir/src/libs/rpi_reg_access.cpp.i

CMakeFiles/demo.dir/src/libs/rpi_reg_access.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/demo.dir/src/libs/rpi_reg_access.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pi/dev_s/hvd190d_pi_basic/src/libs/rpi_reg_access.cpp -o CMakeFiles/demo.dir/src/libs/rpi_reg_access.cpp.s

CMakeFiles/demo.dir/src/libs/rpi_reg_access.cpp.o.requires:

.PHONY : CMakeFiles/demo.dir/src/libs/rpi_reg_access.cpp.o.requires

CMakeFiles/demo.dir/src/libs/rpi_reg_access.cpp.o.provides: CMakeFiles/demo.dir/src/libs/rpi_reg_access.cpp.o.requires
	$(MAKE) -f CMakeFiles/demo.dir/build.make CMakeFiles/demo.dir/src/libs/rpi_reg_access.cpp.o.provides.build
.PHONY : CMakeFiles/demo.dir/src/libs/rpi_reg_access.cpp.o.provides

CMakeFiles/demo.dir/src/libs/rpi_reg_access.cpp.o.provides.build: CMakeFiles/demo.dir/src/libs/rpi_reg_access.cpp.o


CMakeFiles/demo.dir/src/libs/rpi_tcp_server.cpp.o: CMakeFiles/demo.dir/flags.make
CMakeFiles/demo.dir/src/libs/rpi_tcp_server.cpp.o: ../src/libs/rpi_tcp_server.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/dev_s/hvd190d_pi_basic/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/demo.dir/src/libs/rpi_tcp_server.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/demo.dir/src/libs/rpi_tcp_server.cpp.o -c /home/pi/dev_s/hvd190d_pi_basic/src/libs/rpi_tcp_server.cpp

CMakeFiles/demo.dir/src/libs/rpi_tcp_server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/demo.dir/src/libs/rpi_tcp_server.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pi/dev_s/hvd190d_pi_basic/src/libs/rpi_tcp_server.cpp > CMakeFiles/demo.dir/src/libs/rpi_tcp_server.cpp.i

CMakeFiles/demo.dir/src/libs/rpi_tcp_server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/demo.dir/src/libs/rpi_tcp_server.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pi/dev_s/hvd190d_pi_basic/src/libs/rpi_tcp_server.cpp -o CMakeFiles/demo.dir/src/libs/rpi_tcp_server.cpp.s

CMakeFiles/demo.dir/src/libs/rpi_tcp_server.cpp.o.requires:

.PHONY : CMakeFiles/demo.dir/src/libs/rpi_tcp_server.cpp.o.requires

CMakeFiles/demo.dir/src/libs/rpi_tcp_server.cpp.o.provides: CMakeFiles/demo.dir/src/libs/rpi_tcp_server.cpp.o.requires
	$(MAKE) -f CMakeFiles/demo.dir/build.make CMakeFiles/demo.dir/src/libs/rpi_tcp_server.cpp.o.provides.build
.PHONY : CMakeFiles/demo.dir/src/libs/rpi_tcp_server.cpp.o.provides

CMakeFiles/demo.dir/src/libs/rpi_tcp_server.cpp.o.provides.build: CMakeFiles/demo.dir/src/libs/rpi_tcp_server.cpp.o


# Object files for target demo
demo_OBJECTS = \
"CMakeFiles/demo.dir/src/apps/main.cpp.o" \
"CMakeFiles/demo.dir/src/libs/hvd190d_pi_driv.cpp.o" \
"CMakeFiles/demo.dir/src/libs/hvd190d_pi_wf.cpp.o" \
"CMakeFiles/demo.dir/src/libs/koc_wf_gen.cpp.o" \
"CMakeFiles/demo.dir/src/libs/rpi_reg_access.cpp.o" \
"CMakeFiles/demo.dir/src/libs/rpi_tcp_server.cpp.o"

# External object files for target demo
demo_EXTERNAL_OBJECTS =

demo: CMakeFiles/demo.dir/src/apps/main.cpp.o
demo: CMakeFiles/demo.dir/src/libs/hvd190d_pi_driv.cpp.o
demo: CMakeFiles/demo.dir/src/libs/hvd190d_pi_wf.cpp.o
demo: CMakeFiles/demo.dir/src/libs/koc_wf_gen.cpp.o
demo: CMakeFiles/demo.dir/src/libs/rpi_reg_access.cpp.o
demo: CMakeFiles/demo.dir/src/libs/rpi_tcp_server.cpp.o
demo: CMakeFiles/demo.dir/build.make
demo: /usr/lib/arm-linux-gnueabihf/libpthread.so
demo: CMakeFiles/demo.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/pi/dev_s/hvd190d_pi_basic/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking CXX executable demo"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/demo.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/demo.dir/build: demo

.PHONY : CMakeFiles/demo.dir/build

CMakeFiles/demo.dir/requires: CMakeFiles/demo.dir/src/apps/main.cpp.o.requires
CMakeFiles/demo.dir/requires: CMakeFiles/demo.dir/src/libs/hvd190d_pi_driv.cpp.o.requires
CMakeFiles/demo.dir/requires: CMakeFiles/demo.dir/src/libs/hvd190d_pi_wf.cpp.o.requires
CMakeFiles/demo.dir/requires: CMakeFiles/demo.dir/src/libs/koc_wf_gen.cpp.o.requires
CMakeFiles/demo.dir/requires: CMakeFiles/demo.dir/src/libs/rpi_reg_access.cpp.o.requires
CMakeFiles/demo.dir/requires: CMakeFiles/demo.dir/src/libs/rpi_tcp_server.cpp.o.requires

.PHONY : CMakeFiles/demo.dir/requires

CMakeFiles/demo.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/demo.dir/cmake_clean.cmake
.PHONY : CMakeFiles/demo.dir/clean

CMakeFiles/demo.dir/depend:
	cd /home/pi/dev_s/hvd190d_pi_basic/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pi/dev_s/hvd190d_pi_basic /home/pi/dev_s/hvd190d_pi_basic /home/pi/dev_s/hvd190d_pi_basic/build /home/pi/dev_s/hvd190d_pi_basic/build /home/pi/dev_s/hvd190d_pi_basic/build/CMakeFiles/demo.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/demo.dir/depend

