# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /home/bob/WORKS/WAI_GIT/FC41D/fc4x-master/F01DR01/ql_tools/cmake-3.25.1-linux-x86_64/bin/cmake

# The command to remove a file.
RM = /home/bob/WORKS/WAI_GIT/FC41D/fc4x-master/F01DR01/ql_tools/cmake-3.25.1-linux-x86_64/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/bob/WORKS/WAI_GIT/FC41D/fc4x-master/F01DR01

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/bob/WORKS/WAI_GIT/FC41D/fc4x-master/F01DR01/ql_build/gccout

# Include any dependencies generated for this target.
include ql_application/CMakeFiles/ql_app.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include ql_application/CMakeFiles/ql_app.dir/compiler_depend.make

# Include the progress variables for this target.
include ql_application/CMakeFiles/ql_app.dir/progress.make

# Include the compile flags for this target's objects.
include ql_application/CMakeFiles/ql_app.dir/flags.make

ql_application/CMakeFiles/ql_app.dir/ql_app.c.obj: ql_application/CMakeFiles/ql_app.dir/flags.make
ql_application/CMakeFiles/ql_app.dir/ql_app.c.obj: /home/bob/WORKS/WAI_GIT/FC41D/fc4x-master/F01DR01/ql_application/ql_app.c
ql_application/CMakeFiles/ql_app.dir/ql_app.c.obj: ql_application/CMakeFiles/ql_app.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bob/WORKS/WAI_GIT/FC41D/fc4x-master/F01DR01/ql_build/gccout/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object ql_application/CMakeFiles/ql_app.dir/ql_app.c.obj"
	cd /home/bob/WORKS/WAI_GIT/FC41D/fc4x-master/F01DR01/ql_build/gccout/ql_application && /home/bob/WORKS/WAI_GIT/FC41D/fc4x-master/F01DR01/ql_tools/gcc-arm-none-eabi-5_4-2016q3/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT ql_application/CMakeFiles/ql_app.dir/ql_app.c.obj -MF CMakeFiles/ql_app.dir/ql_app.c.obj.d -o CMakeFiles/ql_app.dir/ql_app.c.obj -c /home/bob/WORKS/WAI_GIT/FC41D/fc4x-master/F01DR01/ql_application/ql_app.c

ql_application/CMakeFiles/ql_app.dir/ql_app.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/ql_app.dir/ql_app.c.i"
	cd /home/bob/WORKS/WAI_GIT/FC41D/fc4x-master/F01DR01/ql_build/gccout/ql_application && /home/bob/WORKS/WAI_GIT/FC41D/fc4x-master/F01DR01/ql_tools/gcc-arm-none-eabi-5_4-2016q3/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/bob/WORKS/WAI_GIT/FC41D/fc4x-master/F01DR01/ql_application/ql_app.c > CMakeFiles/ql_app.dir/ql_app.c.i

ql_application/CMakeFiles/ql_app.dir/ql_app.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/ql_app.dir/ql_app.c.s"
	cd /home/bob/WORKS/WAI_GIT/FC41D/fc4x-master/F01DR01/ql_build/gccout/ql_application && /home/bob/WORKS/WAI_GIT/FC41D/fc4x-master/F01DR01/ql_tools/gcc-arm-none-eabi-5_4-2016q3/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/bob/WORKS/WAI_GIT/FC41D/fc4x-master/F01DR01/ql_application/ql_app.c -o CMakeFiles/ql_app.dir/ql_app.c.s

# Object files for target ql_app
ql_app_OBJECTS = \
"CMakeFiles/ql_app.dir/ql_app.c.obj"

# External object files for target ql_app
ql_app_EXTERNAL_OBJECTS =

ql_application/libql_app.a: ql_application/CMakeFiles/ql_app.dir/ql_app.c.obj
ql_application/libql_app.a: ql_application/CMakeFiles/ql_app.dir/build.make
ql_application/libql_app.a: ql_application/CMakeFiles/ql_app.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/bob/WORKS/WAI_GIT/FC41D/fc4x-master/F01DR01/ql_build/gccout/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library libql_app.a"
	cd /home/bob/WORKS/WAI_GIT/FC41D/fc4x-master/F01DR01/ql_build/gccout/ql_application && $(CMAKE_COMMAND) -P CMakeFiles/ql_app.dir/cmake_clean_target.cmake
	cd /home/bob/WORKS/WAI_GIT/FC41D/fc4x-master/F01DR01/ql_build/gccout/ql_application && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ql_app.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
ql_application/CMakeFiles/ql_app.dir/build: ql_application/libql_app.a
.PHONY : ql_application/CMakeFiles/ql_app.dir/build

ql_application/CMakeFiles/ql_app.dir/clean:
	cd /home/bob/WORKS/WAI_GIT/FC41D/fc4x-master/F01DR01/ql_build/gccout/ql_application && $(CMAKE_COMMAND) -P CMakeFiles/ql_app.dir/cmake_clean.cmake
.PHONY : ql_application/CMakeFiles/ql_app.dir/clean

ql_application/CMakeFiles/ql_app.dir/depend:
	cd /home/bob/WORKS/WAI_GIT/FC41D/fc4x-master/F01DR01/ql_build/gccout && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/bob/WORKS/WAI_GIT/FC41D/fc4x-master/F01DR01 /home/bob/WORKS/WAI_GIT/FC41D/fc4x-master/F01DR01/ql_application /home/bob/WORKS/WAI_GIT/FC41D/fc4x-master/F01DR01/ql_build/gccout /home/bob/WORKS/WAI_GIT/FC41D/fc4x-master/F01DR01/ql_build/gccout/ql_application /home/bob/WORKS/WAI_GIT/FC41D/fc4x-master/F01DR01/ql_build/gccout/ql_application/CMakeFiles/ql_app.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : ql_application/CMakeFiles/ql_app.dir/depend
