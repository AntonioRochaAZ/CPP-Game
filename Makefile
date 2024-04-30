SRC_DIR = src
BUILD_DIR = build/debug
CC = g++
SRC_FILES = $(shell find $(SRC_DIR) -name "*.cpp")	# So we don't have to specify all subdirectories
OBJ_NAME = game
INCLUDE_PATHS = -I include
LIBRARY_PATHS = -L lib
COMPILER_FLAGS = -std=c++14 -Wall -O0 -g
LINKER_FLAGS = -l SDL2 -l SDL2_image -l SDL2_ttf

# Documentation related:
MAKEFILE_DIR = $(CURDIR)
DOXYGEN_FOLDER = docs/doxygen
SPHINX_FOLDER = docs/sphinx

all:
	$(CC) $(COMPILER_FLAGS) $(LINKER_FLAGS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(SRC_FILES) -o $(BUILD_DIR)/$(OBJ_NAME)

documentation:
	cd $(DOXYGEN_FOLDER); Doxygen; cd $(MAKEFILE_DIR)	# For some reason, it would not work properly without moving into the correct folder
	sphinx-build -b html $(SPHINX_FOLDER) $(SPHINX_FOLDER)/_build/html