PROGRAM = main

SRC_DIR = ../..

OBJ_DIR = ./build/objs

TARGET = ./build/$(PROGRAM)

SRCS = $(wildcard $(SRC_DIR)/src/*.cpp) $(wildcard ./*.cpp)

OBJS = $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(notdir $(SRCS)))

INCLUDE_DIR =\
-I$(SRC_DIR)/src\

LIB_DIR =

CXXFLAGS = -std=gnu++11

LDFLAGS =

CXX = g++

all : $(TARGET)

$(TARGET) : $(OBJ_DIR) $(OBJS)
	$(CXX) $(LIB_DIR) $(OBJS) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o : $(SRC_DIR)/src/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIR) -c $< -o $@

$(OBJ_DIR)/%.o : %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIR) -c $< -o $@

$(OBJ_DIR) :
	mkdir -p $(OBJ_DIR)

clean_dummy :
	@echo "rm" $(OBJS)

clean :
	$(RM) $(OBJS)

.PHONY: all clean
.SUFFIXES: .cpp .o
