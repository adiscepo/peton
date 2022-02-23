# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/zandies/Dokumentoj/Kodingo/peton

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/zandies/Dokumentoj/Kodingo/peton

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	/usr/bin/ccmake -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake --regenerate-during-build -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/zandies/Dokumentoj/Kodingo/peton/CMakeFiles /home/zandies/Dokumentoj/Kodingo/peton//CMakeFiles/progress.marks
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/zandies/Dokumentoj/Kodingo/peton/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named peton

# Build rule for target.
peton: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 peton
.PHONY : peton

# fast build rule for target.
peton/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/peton.dir/build.make CMakeFiles/peton.dir/build
.PHONY : peton/fast

IP_Machine.o: IP_Machine.cpp.o
.PHONY : IP_Machine.o

# target to build an object file
IP_Machine.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/peton.dir/build.make CMakeFiles/peton.dir/IP_Machine.cpp.o
.PHONY : IP_Machine.cpp.o

IP_Machine.i: IP_Machine.cpp.i
.PHONY : IP_Machine.i

# target to preprocess a source file
IP_Machine.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/peton.dir/build.make CMakeFiles/peton.dir/IP_Machine.cpp.i
.PHONY : IP_Machine.cpp.i

IP_Machine.s: IP_Machine.cpp.s
.PHONY : IP_Machine.s

# target to generate assembly for a file
IP_Machine.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/peton.dir/build.make CMakeFiles/peton.dir/IP_Machine.cpp.s
.PHONY : IP_Machine.cpp.s

Interface.o: Interface.cpp.o
.PHONY : Interface.o

# target to build an object file
Interface.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/peton.dir/build.make CMakeFiles/peton.dir/Interface.cpp.o
.PHONY : Interface.cpp.o

Interface.i: Interface.cpp.i
.PHONY : Interface.i

# target to preprocess a source file
Interface.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/peton.dir/build.make CMakeFiles/peton.dir/Interface.cpp.i
.PHONY : Interface.cpp.i

Interface.s: Interface.cpp.s
.PHONY : Interface.s

# target to generate assembly for a file
Interface.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/peton.dir/build.make CMakeFiles/peton.dir/Interface.cpp.s
.PHONY : Interface.cpp.s

Link.o: Link.cpp.o
.PHONY : Link.o

# target to build an object file
Link.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/peton.dir/build.make CMakeFiles/peton.dir/Link.cpp.o
.PHONY : Link.cpp.o

Link.i: Link.cpp.i
.PHONY : Link.i

# target to preprocess a source file
Link.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/peton.dir/build.make CMakeFiles/peton.dir/Link.cpp.i
.PHONY : Link.cpp.i

Link.s: Link.cpp.s
.PHONY : Link.s

# target to generate assembly for a file
Link.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/peton.dir/build.make CMakeFiles/peton.dir/Link.cpp.s
.PHONY : Link.cpp.s

Machine.o: Machine.cpp.o
.PHONY : Machine.o

# target to build an object file
Machine.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/peton.dir/build.make CMakeFiles/peton.dir/Machine.cpp.o
.PHONY : Machine.cpp.o

Machine.i: Machine.cpp.i
.PHONY : Machine.i

# target to preprocess a source file
Machine.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/peton.dir/build.make CMakeFiles/peton.dir/Machine.cpp.i
.PHONY : Machine.cpp.i

Machine.s: Machine.cpp.s
.PHONY : Machine.s

# target to generate assembly for a file
Machine.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/peton.dir/build.make CMakeFiles/peton.dir/Machine.cpp.s
.PHONY : Machine.cpp.s

Packet.o: Packet.cpp.o
.PHONY : Packet.o

# target to build an object file
Packet.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/peton.dir/build.make CMakeFiles/peton.dir/Packet.cpp.o
.PHONY : Packet.cpp.o

Packet.i: Packet.cpp.i
.PHONY : Packet.i

# target to preprocess a source file
Packet.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/peton.dir/build.make CMakeFiles/peton.dir/Packet.cpp.i
.PHONY : Packet.cpp.i

Packet.s: Packet.cpp.s
.PHONY : Packet.s

# target to generate assembly for a file
Packet.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/peton.dir/build.make CMakeFiles/peton.dir/Packet.cpp.s
.PHONY : Packet.cpp.s

main.o: main.cpp.o
.PHONY : main.o

# target to build an object file
main.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/peton.dir/build.make CMakeFiles/peton.dir/main.cpp.o
.PHONY : main.cpp.o

main.i: main.cpp.i
.PHONY : main.i

# target to preprocess a source file
main.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/peton.dir/build.make CMakeFiles/peton.dir/main.cpp.i
.PHONY : main.cpp.i

main.s: main.cpp.s
.PHONY : main.s

# target to generate assembly for a file
main.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/peton.dir/build.make CMakeFiles/peton.dir/main.cpp.s
.PHONY : main.cpp.s

types.o: types.cpp.o
.PHONY : types.o

# target to build an object file
types.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/peton.dir/build.make CMakeFiles/peton.dir/types.cpp.o
.PHONY : types.cpp.o

types.i: types.cpp.i
.PHONY : types.i

# target to preprocess a source file
types.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/peton.dir/build.make CMakeFiles/peton.dir/types.cpp.i
.PHONY : types.cpp.i

types.s: types.cpp.s
.PHONY : types.s

# target to generate assembly for a file
types.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/peton.dir/build.make CMakeFiles/peton.dir/types.cpp.s
.PHONY : types.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... peton"
	@echo "... IP_Machine.o"
	@echo "... IP_Machine.i"
	@echo "... IP_Machine.s"
	@echo "... Interface.o"
	@echo "... Interface.i"
	@echo "... Interface.s"
	@echo "... Link.o"
	@echo "... Link.i"
	@echo "... Link.s"
	@echo "... Machine.o"
	@echo "... Machine.i"
	@echo "... Machine.s"
	@echo "... Packet.o"
	@echo "... Packet.i"
	@echo "... Packet.s"
	@echo "... main.o"
	@echo "... main.i"
	@echo "... main.s"
	@echo "... types.o"
	@echo "... types.i"
	@echo "... types.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

