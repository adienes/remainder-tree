CXX := g++
SRC_DIR := ./examples
OBJ_DIR := ./obj
BIN_DIR := ./bin
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
LDLIBS := -lntl -lgmp -lgmpxx -pthread
#LDFLAGS := ...
#CPPFLAGS := ...
CXXFLAGS := -Wall -Wextra -g -march=native -O3 -std=c++11


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

$(BIN_DIR)/main: $(OBJ_FILES)
	@mkdir -p $(BIN_DIR)
	g++ $^ -o $@ $(LDFLAGS) $(LDLIBS)

.PHONY : clean
clean:
	-rm $(BIN_DIR)/*
	-rm $(OBJ_DIR)/*
