SRC_DIR = src
BUILD_DIR = build
CC = g++
SRC_FILES = $(shell find $(SRC_DIR) -name "*.cpp")	# So we don't have to specify all subdirectories
OBJ_NAME = game
INCLUDE_PATHS = -I include #-I/usr/local/include
LIBRARY_PATHS = -L lib
LINKER_FLAGS = -l SDL2 -l SDL2_image -l SDL2_ttf

# Version (debug/release):
DEBUG_DIR = debug
RELEASE_DIR = release
# Definition of the working directory:
# Eventually we should integrate assets etc. into the executable so this isn't necessary.
DEFINITIONS = -DWORK_DIR=\"${PWD}\"	
DEBUG_COMPILER_FLAGS = -std=c++17 -Wall -O0 -g -DDEBUG_MODE 
FAST_DEBUG_COMPILER_FLAGS = -std=c++17 -Wall -O3 -g -DDEBUG_MODE 
RELEASE_COMPILER_FLAGS = -std=c++17 -Wall -O3 -g

# Documentation related:
MAKEFILE_DIR = $(CURDIR)
DOXYGEN_FOLDER = docs/doxygen
SPHINX_FOLDER = docs/sphinx

debug:
	$(CC) $(DEBUG_COMPILER_FLAGS) $(DEFINITIONS) $(LINKER_FLAGS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(SRC_FILES) -o $(BUILD_DIR)/$(DEBUG_DIR)/$(OBJ_NAME)

fast_debug:
	$(CC) $(FAST_DEBUG_COMPILER_FLAGS) $(DEFINITIONS) $(LINKER_FLAGS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(SRC_FILES) -o $(BUILD_DIR)/$(DEBUG_DIR)/$(OBJ_NAME)

release:
	$(CC) $(RELEASE_COMPILER_FLAGS) $(DEFINITIONS) $(LINKER_FLAGS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(SRC_FILES) -o $(BUILD_DIR)/$(RELEASE_DIR)/$(OBJ_NAME)

documentation:
	cd $(DOXYGEN_FOLDER); Doxygen; cd $(MAKEFILE_DIR)	# For some reason, it would not work properly without moving into the correct folder
	sphinx-build -b html $(SPHINX_FOLDER) $(SPHINX_FOLDER)/_build/html