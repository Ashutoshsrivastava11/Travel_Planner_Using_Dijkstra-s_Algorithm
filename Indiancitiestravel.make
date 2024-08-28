all:
	g++ -o Indiancitiestravel Main.cpp Graphfunctiontocalculate.h Indianlocation.h Indianroute.h FileOperationsfortravel.h
	
# Object Files
OBJ = $(SRC:.cpp=.o)

# Default Target
all: $(TARGET)

# Link the final executable
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -f $(OBJ) $(TARGET)

# Phony targets
.PHONY: all clean