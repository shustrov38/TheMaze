# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.17

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

# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "D:\For apps\CLion 2020.2.1\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "D:\For apps\CLion 2020.2.1\bin\cmake\win\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:\AllCodes\TheMaze

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\AllCodes\TheMaze\cmake-build-debug

# Include any dependencies generated for this target.
include src/server/CMakeFiles/Server.dir/depend.make

# Include the progress variables for this target.
include src/server/CMakeFiles/Server.dir/progress.make

# Include the compile flags for this target's objects.
include src/server/CMakeFiles/Server.dir/flags.make

src/server/CMakeFiles/Server.dir/server.c.obj: src/server/CMakeFiles/Server.dir/flags.make
src/server/CMakeFiles/Server.dir/server.c.obj: ../src/server/server.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\AllCodes\TheMaze\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object src/server/CMakeFiles/Server.dir/server.c.obj"
	cd /d D:\AllCodes\TheMaze\cmake-build-debug\src\server && "D:\For apps\mingw64\bin\gcc.exe" $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\Server.dir\server.c.obj   -c D:\AllCodes\TheMaze\src\server\server.c

src/server/CMakeFiles/Server.dir/server.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Server.dir/server.c.i"
	cd /d D:\AllCodes\TheMaze\cmake-build-debug\src\server && "D:\For apps\mingw64\bin\gcc.exe" $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E D:\AllCodes\TheMaze\src\server\server.c > CMakeFiles\Server.dir\server.c.i

src/server/CMakeFiles/Server.dir/server.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Server.dir/server.c.s"
	cd /d D:\AllCodes\TheMaze\cmake-build-debug\src\server && "D:\For apps\mingw64\bin\gcc.exe" $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S D:\AllCodes\TheMaze\src\server\server.c -o CMakeFiles\Server.dir\server.c.s

src/server/CMakeFiles/Server.dir/MazeGenerator.c.obj: src/server/CMakeFiles/Server.dir/flags.make
src/server/CMakeFiles/Server.dir/MazeGenerator.c.obj: ../src/server/MazeGenerator.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\AllCodes\TheMaze\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object src/server/CMakeFiles/Server.dir/MazeGenerator.c.obj"
	cd /d D:\AllCodes\TheMaze\cmake-build-debug\src\server && "D:\For apps\mingw64\bin\gcc.exe" $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\Server.dir\MazeGenerator.c.obj   -c D:\AllCodes\TheMaze\src\server\MazeGenerator.c

src/server/CMakeFiles/Server.dir/MazeGenerator.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Server.dir/MazeGenerator.c.i"
	cd /d D:\AllCodes\TheMaze\cmake-build-debug\src\server && "D:\For apps\mingw64\bin\gcc.exe" $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E D:\AllCodes\TheMaze\src\server\MazeGenerator.c > CMakeFiles\Server.dir\MazeGenerator.c.i

src/server/CMakeFiles/Server.dir/MazeGenerator.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Server.dir/MazeGenerator.c.s"
	cd /d D:\AllCodes\TheMaze\cmake-build-debug\src\server && "D:\For apps\mingw64\bin\gcc.exe" $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S D:\AllCodes\TheMaze\src\server\MazeGenerator.c -o CMakeFiles\Server.dir\MazeGenerator.c.s

# Object files for target Server
Server_OBJECTS = \
"CMakeFiles/Server.dir/server.c.obj" \
"CMakeFiles/Server.dir/MazeGenerator.c.obj"

# External object files for target Server
Server_EXTERNAL_OBJECTS =

src/server/Server.exe: src/server/CMakeFiles/Server.dir/server.c.obj
src/server/Server.exe: src/server/CMakeFiles/Server.dir/MazeGenerator.c.obj
src/server/Server.exe: src/server/CMakeFiles/Server.dir/build.make
src/server/Server.exe: src/server/CMakeFiles/Server.dir/linklibs.rsp
src/server/Server.exe: src/server/CMakeFiles/Server.dir/objects1.rsp
src/server/Server.exe: src/server/CMakeFiles/Server.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=D:\AllCodes\TheMaze\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable Server.exe"
	cd /d D:\AllCodes\TheMaze\cmake-build-debug\src\server && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\Server.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/server/CMakeFiles/Server.dir/build: src/server/Server.exe

.PHONY : src/server/CMakeFiles/Server.dir/build

src/server/CMakeFiles/Server.dir/clean:
	cd /d D:\AllCodes\TheMaze\cmake-build-debug\src\server && $(CMAKE_COMMAND) -P CMakeFiles\Server.dir\cmake_clean.cmake
.PHONY : src/server/CMakeFiles/Server.dir/clean

src/server/CMakeFiles/Server.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\AllCodes\TheMaze D:\AllCodes\TheMaze\src\server D:\AllCodes\TheMaze\cmake-build-debug D:\AllCodes\TheMaze\cmake-build-debug\src\server D:\AllCodes\TheMaze\cmake-build-debug\src\server\CMakeFiles\Server.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : src/server/CMakeFiles/Server.dir/depend

