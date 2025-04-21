# Compiler and flags
CXX = g++
CXXFLAGS = -Iinclude -I./IXWebSocket/ixwebsocket -std=c++17
LDFLAGS = -Llib -lraylib -L./IXWebSocket/build -lixwebsocket -pthread
FRAMEWORKS = -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL

# Source files
SOURCES = main.cpp player.cpp
TARGET = main

# Default build target
default: $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET) $(LDFLAGS) $(FRAMEWORKS)

# Clean up
clean:
	rm -f $(TARGET)
