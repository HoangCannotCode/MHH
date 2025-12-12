CXX = g++
CXXFLAGS = -std=c++17 -mconsole -static -Wall -Wextra
INCLUDES = -I../reachable_marking_BFS_DFS

# Source files
SOURCES = main.cpp optimization.cpp ../reachable_marking_BFS_DFS/BFS.cpp ../reachable_marking_BFS_DFS/PetriNet.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = optimization.exe

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(OBJECTS) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

run: $(EXECUTABLE)
	./$(EXECUTABLE)

.PHONY: all clean run
