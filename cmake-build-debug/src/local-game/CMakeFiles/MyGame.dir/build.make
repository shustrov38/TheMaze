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
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2020.3\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2020.3\bin\cmake\win\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:\TheMaze

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\TheMaze\cmake-build-debug

# Include any dependencies generated for this target.
include src/local-game/CMakeFiles/MyGame.dir/depend.make

# Include the progress variables for this target.
include src/local-game/CMakeFiles/MyGame.dir/progress.make

# Include the compile flags for this target's objects.
include src/local-game/CMakeFiles/MyGame.dir/flags.make

src/local-game/CMakeFiles/MyGame.dir/game.c.obj: src/local-game/CMakeFiles/MyGame.dir/flags.make
src/local-game/CMakeFiles/MyGame.dir/game.c.obj: ../src/local-game/game.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\TheMaze\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object src/local-game/CMakeFiles/MyGame.dir/game.c.obj"
	cd /d D:\TheMaze\cmake-build-debug\src\local-game && C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\MyGame.dir\game.c.obj   -c D:\TheMaze\src\local-game\game.c

src/local-game/CMakeFiles/MyGame.dir/game.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/MyGame.dir/game.c.i"
	cd /d D:\TheMaze\cmake-build-debug\src\local-game && C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E D:\TheMaze\src\local-game\game.c > CMakeFiles\MyGame.dir\game.c.i

src/local-game/CMakeFiles/MyGame.dir/game.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/MyGame.dir/game.c.s"
	cd /d D:\TheMaze\cmake-build-debug\src\local-game && C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S D:\TheMaze\src\local-game\game.c -o CMakeFiles\MyGame.dir\game.c.s

src/local-game/CMakeFiles/MyGame.dir/playerInfo.c.obj: src/local-game/CMakeFiles/MyGame.dir/flags.make
src/local-game/CMakeFiles/MyGame.dir/playerInfo.c.obj: ../src/local-game/playerInfo.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\TheMaze\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object src/local-game/CMakeFiles/MyGame.dir/playerInfo.c.obj"
	cd /d D:\TheMaze\cmake-build-debug\src\local-game && C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\MyGame.dir\playerInfo.c.obj   -c D:\TheMaze\src\local-game\playerInfo.c

src/local-game/CMakeFiles/MyGame.dir/playerInfo.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/MyGame.dir/playerInfo.c.i"
	cd /d D:\TheMaze\cmake-build-debug\src\local-game && C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E D:\TheMaze\src\local-game\playerInfo.c > CMakeFiles\MyGame.dir\playerInfo.c.i

src/local-game/CMakeFiles/MyGame.dir/playerInfo.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/MyGame.dir/playerInfo.c.s"
	cd /d D:\TheMaze\cmake-build-debug\src\local-game && C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S D:\TheMaze\src\local-game\playerInfo.c -o CMakeFiles\MyGame.dir\playerInfo.c.s

src/local-game/CMakeFiles/MyGame.dir/maze_generator.c.obj: src/local-game/CMakeFiles/MyGame.dir/flags.make
src/local-game/CMakeFiles/MyGame.dir/maze_generator.c.obj: ../src/local-game/maze_generator.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\TheMaze\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object src/local-game/CMakeFiles/MyGame.dir/maze_generator.c.obj"
	cd /d D:\TheMaze\cmake-build-debug\src\local-game && C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\MyGame.dir\maze_generator.c.obj   -c D:\TheMaze\src\local-game\maze_generator.c

src/local-game/CMakeFiles/MyGame.dir/maze_generator.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/MyGame.dir/maze_generator.c.i"
	cd /d D:\TheMaze\cmake-build-debug\src\local-game && C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E D:\TheMaze\src\local-game\maze_generator.c > CMakeFiles\MyGame.dir\maze_generator.c.i

src/local-game/CMakeFiles/MyGame.dir/maze_generator.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/MyGame.dir/maze_generator.c.s"
	cd /d D:\TheMaze\cmake-build-debug\src\local-game && C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S D:\TheMaze\src\local-game\maze_generator.c -o CMakeFiles\MyGame.dir\maze_generator.c.s

src/local-game/CMakeFiles/MyGame.dir/utilities.c.obj: src/local-game/CMakeFiles/MyGame.dir/flags.make
src/local-game/CMakeFiles/MyGame.dir/utilities.c.obj: ../src/local-game/utilities.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\TheMaze\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object src/local-game/CMakeFiles/MyGame.dir/utilities.c.obj"
	cd /d D:\TheMaze\cmake-build-debug\src\local-game && C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\MyGame.dir\utilities.c.obj   -c D:\TheMaze\src\local-game\utilities.c

src/local-game/CMakeFiles/MyGame.dir/utilities.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/MyGame.dir/utilities.c.i"
	cd /d D:\TheMaze\cmake-build-debug\src\local-game && C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E D:\TheMaze\src\local-game\utilities.c > CMakeFiles\MyGame.dir\utilities.c.i

src/local-game/CMakeFiles/MyGame.dir/utilities.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/MyGame.dir/utilities.c.s"
	cd /d D:\TheMaze\cmake-build-debug\src\local-game && C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S D:\TheMaze\src\local-game\utilities.c -o CMakeFiles\MyGame.dir\utilities.c.s

src/local-game/CMakeFiles/MyGame.dir/__/client/client_server_interface/interface.c.obj: src/local-game/CMakeFiles/MyGame.dir/flags.make
src/local-game/CMakeFiles/MyGame.dir/__/client/client_server_interface/interface.c.obj: ../src/client/client_server_interface/interface.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\TheMaze\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object src/local-game/CMakeFiles/MyGame.dir/__/client/client_server_interface/interface.c.obj"
	cd /d D:\TheMaze\cmake-build-debug\src\local-game && C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\MyGame.dir\__\client\client_server_interface\interface.c.obj   -c D:\TheMaze\src\client\client_server_interface\interface.c

src/local-game/CMakeFiles/MyGame.dir/__/client/client_server_interface/interface.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/MyGame.dir/__/client/client_server_interface/interface.c.i"
	cd /d D:\TheMaze\cmake-build-debug\src\local-game && C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E D:\TheMaze\src\client\client_server_interface\interface.c > CMakeFiles\MyGame.dir\__\client\client_server_interface\interface.c.i

src/local-game/CMakeFiles/MyGame.dir/__/client/client_server_interface/interface.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/MyGame.dir/__/client/client_server_interface/interface.c.s"
	cd /d D:\TheMaze\cmake-build-debug\src\local-game && C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S D:\TheMaze\src\client\client_server_interface\interface.c -o CMakeFiles\MyGame.dir\__\client\client_server_interface\interface.c.s

# Object files for target MyGame
MyGame_OBJECTS = \
"CMakeFiles/MyGame.dir/game.c.obj" \
"CMakeFiles/MyGame.dir/playerInfo.c.obj" \
"CMakeFiles/MyGame.dir/maze_generator.c.obj" \
"CMakeFiles/MyGame.dir/utilities.c.obj" \
"CMakeFiles/MyGame.dir/__/client/client_server_interface/interface.c.obj"

# External object files for target MyGame
MyGame_EXTERNAL_OBJECTS =

src/local-game/MyGame.exe: src/local-game/CMakeFiles/MyGame.dir/game.c.obj
src/local-game/MyGame.exe: src/local-game/CMakeFiles/MyGame.dir/playerInfo.c.obj
src/local-game/MyGame.exe: src/local-game/CMakeFiles/MyGame.dir/maze_generator.c.obj
src/local-game/MyGame.exe: src/local-game/CMakeFiles/MyGame.dir/utilities.c.obj
src/local-game/MyGame.exe: src/local-game/CMakeFiles/MyGame.dir/__/client/client_server_interface/interface.c.obj
src/local-game/MyGame.exe: src/local-game/CMakeFiles/MyGame.dir/build.make
src/local-game/MyGame.exe: src/local-game/CMakeFiles/MyGame.dir/linklibs.rsp
src/local-game/MyGame.exe: src/local-game/CMakeFiles/MyGame.dir/objects1.rsp
src/local-game/MyGame.exe: src/local-game/CMakeFiles/MyGame.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=D:\TheMaze\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking C executable MyGame.exe"
	cd /d D:\TheMaze\cmake-build-debug\src\local-game && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\MyGame.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/local-game/CMakeFiles/MyGame.dir/build: src/local-game/MyGame.exe

.PHONY : src/local-game/CMakeFiles/MyGame.dir/build

src/local-game/CMakeFiles/MyGame.dir/clean:
	cd /d D:\TheMaze\cmake-build-debug\src\local-game && $(CMAKE_COMMAND) -P CMakeFiles\MyGame.dir\cmake_clean.cmake
.PHONY : src/local-game/CMakeFiles/MyGame.dir/clean

src/local-game/CMakeFiles/MyGame.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\TheMaze D:\TheMaze\src\local-game D:\TheMaze\cmake-build-debug D:\TheMaze\cmake-build-debug\src\local-game D:\TheMaze\cmake-build-debug\src\local-game\CMakeFiles\MyGame.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : src/local-game/CMakeFiles/MyGame.dir/depend

