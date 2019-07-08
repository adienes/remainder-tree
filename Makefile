CXX := g++
SRC_DIR := ./src
OBJ_DIR := ./obj
BIN_DIR := ./bin
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
LDLIBS := -lntl -lgmp -lgmpxx -pthread
#LDFLAGS := ...
#CPPFLAGS := ...
CXXFLAGS := -Wall -g -march=native -O3

$(BIN_DIR)/main: $(OBJ_FILES)
	g++ $^ -o $@ $(LDFLAGS) $(LDLIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<


.PHONY : clean
clean:
	-rm $(OBJ_FILES)
	-rm $(BIN_DIR)/*