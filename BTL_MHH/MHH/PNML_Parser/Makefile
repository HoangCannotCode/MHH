# Compiler settings
CXX = g++
CXXFLAGS = -std=c++17 -Wall -g

# Output executable name
TARGET = PNML_Parser.exe

# Object files
OBJS = main.o PetriNet.o tinyxml2.o

# --- Targets ---

# Default target: build the executable
all: $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)
	@echo "Build successful! Run with: $(TARGET)"

# Compile main.cpp
main.o: main.cpp PetriNet.h
	$(CXX) $(CXXFLAGS) -c main.cpp

# Compile PetriNet.cpp
PetriNet.o: PetriNet.cpp PetriNet.h tinyxml2.h
	$(CXX) $(CXXFLAGS) -c PetriNet.cpp

# Compile tinyxml2.cpp
tinyxml2.o: tinyxml2.cpp tinyxml2.h
	$(CXX) $(CXXFLAGS) -c tinyxml2.cpp

# Clean up build files (Windows specific 'del' command)
clean:
	del $(OBJS) $(TARGET)