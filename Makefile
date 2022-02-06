-CXX       		:= g++
CXX_FLAGS 		:= -std=c++17 $(EXT_FLAGS)
LIBRARIES   	:= -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
BIN_DIR     	:= bin
SRC_DIR     	:= src
OBJ_DIR			:= obj
LIB_DIR			:= lib
INCLUDE_DIR 	:= include

EXECUTABLE  := main.exe

# Get full path to all cpp files. Then, remove the path to current directory,
# changing it to relative path.
SRC_FILES := $(patsubst $(shell cd)\\%.cpp, %.cpp, $(shell dir src\*.cpp /B/S))

OBJS := $(patsubst $(SRC_DIR)\\%.cpp, $(OBJ_DIR)\\%.o, $(SRC_FILES))

all: $(BIN_DIR)/$(EXECUTABLE)

# Reference for future
# Add -mwindows to end of linker command to create proper desktop app
# Create main executable using all our object files
$(BIN_DIR)/$(EXECUTABLE): $(OBJS)
	$(CXX) -o $@ $^ -L$(LIB_DIR) $(LIBRARIES)

# Create object files as needed.
$(OBJ_DIR)\\%.o : $(SRC_DIR)\%.cpp
	if not exist $(@D) mkdir $(@D)
	$(CXX) $(CXX_FLAGS) -I$(SRC_DIR) -I$(INCLUDE_DIR) -c -o $@ $^

CXX_FLAGS += -MMD
-include $(OBJ_FILES:.o=.d)