CXX := g++
TARGET := server
INCLUDE_DIR := ./header_file
SRC_DIR := ./source_file

CXXFLAGS := -I$(INCLUDE_DIR) -std=c++11 -Wall
LDFLAGS := -L/usr/lib64/mysql -lmysqlclient -pthread

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
 # 把.cpp替换成.o，目标文件放到build目录
 OBJS := $(patsubst $(SRC_DIR)/%.cpp, build/%.o, $(SRCS))

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

build/%.o: $(SRC_DIR)/%.cpp | build
	$(CXX) $(CXXFLAGS) -c $< -o $@

build:
	mkdir -p build

clean:
	rm -rf build $(TARGET)

.PHONY: clean
