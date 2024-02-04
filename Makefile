SRC_DIR = src
BUILD_DIR = build/debug
CC = g++
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp $(SRC_DIR)/ECS/*.cpp)
OBJ_NAME = play
INCLUDE_PATHS = -I include
LIBRARY_PATHS = -L lib
COMPILER_FLAGS = -std=c++14 -Wall -O3 -g
LINKER_FLAGS = -l SDL2

all:
	$(CC) $(COMPILER_FLAGS) $(LINKER_FLAGS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(SRC_FILES) -o $(BUILD_DIR)/$(OBJ_NAME)