CXX := g++
SRC_DIR := ./src/executables
OBJ_DIR := ./obj
BIN_DIR := ./bin
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
LDLIBS := -lntl -lgmp -lgmpxx -pthread
#LDFLAGS := ...
#CPPFLAGS := ...
CXXFLAGS := -Wall -g -march=native -O3


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

$(BIN_DIR)/main: $(OBJ_FILES)
	g++ $^ -o $@ $(LDFLAGS) $(LDLIBS)

.PHONY : clean
clean:
	-rm $(BIN_DIR)/*
	-rm $(OBJ_DIR)/*