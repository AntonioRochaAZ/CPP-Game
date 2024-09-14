SRC_DIR = src
BUILD_DIR = build
CC = g++
SRC_FILES = $(shell find $(SRC_DIR) -name "*.cpp")	# So we don't have to specify all subdirectories
EXE_NAME = game
INCLUDE_PATHS = -I include #-I/usr/local/include
LIBRARY_PATHS = -L lib
LINKER_FLAGS = -l SDL2 -l SDL2_image -l SDL2_ttf
OBJ_DIR = objs

# Version (debug/release):
DEBUG_DIR = debug
RELEASE_DIR = release
# Definition of the working directory:
# Eventually we should integrate assets etc. into the executable so this isn't necessary.
DEFINITIONS = -DWORK_DIR=\"${PWD}\"	
DEBUG_COMPILER_FLAGS = -std=c++17 -Wall -O0 -g -DDEBUG_MODE -c
RELEASE_COMPILER_FLAGS = -std=c++17 -Wall -O3 -c
LOG_FILE = build.log

# Documentation related:
MAKEFILE_DIR = $(CURDIR)
DOXYGEN_FOLDER = docs/doxygen
SPHINX_FOLDER = docs/sphinx

# ----- ACTUAL COMPILATION AND LINKING ----- #
OBJS_RELEASE = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/$(RELEASE_DIR)/$(OBJ_DIR)/%.o, $(SRC_FILES))
OBJS_DEBUG   = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/$(DEBUG_DIR)/$(OBJ_DIR)/%.o, $(SRC_FILES))

all: clean_log_release release
release: clean_log_release $(BUILD_DIR)/$(RELEASE_DIR)/$(EXE_NAME)
debug: clean_log_debug $(BUILD_DIR)/$(DEBUG_DIR)/$(EXE_NAME)
clean_release:
	rm -rf $(BUILD_DIR)/$(RELEASE_DIR)/$(OBJ_DIR)
clean_debug: 
	rm -rf $(BUILD_DIR)/$(DEBUG_DIR)/$(OBJ_DIR)

clean:
	rm -rf $(BUILD_DIR)/$(RELEASE_DIR)/$(OBJ_DIR)
	rm -rf $(BUILD_DIR)/$(DEBUG_DIR)/$(OBJ_DIR)

documentation:
	cd $(DOXYGEN_FOLDER); Doxygen; cd $(MAKEFILE_DIR)	# For some reason, it would not work properly without moving into the correct folder
	sphinx-build -b html $(SPHINX_FOLDER) $(SPHINX_FOLDER)/_build/html

$(BUILD_DIR)/$(RELEASE_DIR)/$(EXE_NAME): $(OBJS_RELEASE)
	@echo "" >> $(BUILD_DIR)/$(RELEASE_DIR)/$(LOG_FILE)
	@echo "Linking objects:" >> $(BUILD_DIR)/$(RELEASE_DIR)/$(LOG_FILE)
	@echo "--------------------------------------------------------------------------------" >> $(BUILD_DIR)/$(RELEASE_DIR)/$(LOG_FILE)
	@echo $(CC) $(LIBRARY_PATHS) $^ -o $@ $(LINKER_FLAGS) >> $(BUILD_DIR)/$(RELEASE_DIR)/$(LOG_FILE) 2>&1
	$(CC) $(LIBRARY_PATHS) $^ -o $@ $(LINKER_FLAGS) >> $(BUILD_DIR)/$(RELEASE_DIR)/$(LOG_FILE) 2>&1

$(BUILD_DIR)/$(DEBUG_DIR)/$(EXE_NAME): $(OBJS_DEBUG)
	@echo "" >> $(BUILD_DIR)/$(DEBUG_DIR)/$(LOG_FILE)
	@echo "Linking objects:" >> $(BUILD_DIR)/$(DEBUG_DIR)/$(LOG_FILE)
	@echo "--------------------------------------------------------------------------------" >> $(BUILD_DIR)/$(DEBUG_DIR)/$(LOG_FILE)
	@echo $(CC) $(LIBRARY_PATHS) $^ -o $@ $(LINKER_FLAGS) >> $(BUILD_DIR)/$(DEBUG_DIR)/$(LOG_FILE) 2>&1
	$(CC) $(LIBRARY_PATHS) $^ -o $@ $(LINKER_FLAGS) >> $(BUILD_DIR)/$(DEBUG_DIR)/$(LOG_FILE) 2>&1

$(BUILD_DIR)/$(RELEASE_DIR)/$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@echo $(CC) $(INCLUDE_PATHS) $(RELEASE_COMPILER_FLAGS) $(DEFINITIONS) $< -o $@ >> $(BUILD_DIR)/$(RELEASE_DIR)/$(LOG_FILE) 2>&1
	$(CC) $(INCLUDE_PATHS) $(RELEASE_COMPILER_FLAGS) $(DEFINITIONS) $< -o $@ >> $(BUILD_DIR)/$(RELEASE_DIR)/$(LOG_FILE) 2>&1

$(BUILD_DIR)/$(DEBUG_DIR)/$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@echo $(CC) $(INCLUDE_PATHS) $(DEBUG_COMPILER_FLAGS) $(DEFINITIONS) $< -o $@ >> $(BUILD_DIR)/$(DEBUG_DIR)/$(LOG_FILE) 2>&1
	$(CC) $(INCLUDE_PATHS) $(DEBUG_COMPILER_FLAGS) $(DEFINITIONS) $< -o $@ >> $(BUILD_DIR)/$(DEBUG_DIR)/$(LOG_FILE) 2>&1

clean_log_release:
	@mkdir -p $(BUILD_DIR)/$(RELEASE_DIR)/
	@echo "$(shell date)" > $(BUILD_DIR)/$(RELEASE_DIR)/$(LOG_FILE)
	@echo "Starting RELEASE compilation" >> $(BUILD_DIR)/$(RELEASE_DIR)/$(LOG_FILE)
	@echo "--------------------------------------------------------------------------------" >> $(BUILD_DIR)/$(RELEASE_DIR)/$(LOG_FILE)

clean_log_debug:
	@mkdir -p $(BUILD_DIR)/$(DEBUG_DIR)/
	@echo "$(shell date)" > $(BUILD_DIR)/$(DEBUG_DIR)/$(LOG_FILE)
	@echo "Starting DEBUG compilation" >> $(BUILD_DIR)/$(DEBUG_DIR)/$(LOG_FILE)
	@echo "--------------------------------------------------------------------------------" >> $(BUILD_DIR)/$(DEBUG_DIR)/$(LOG_FILE)