CXX = g++
CFLAGS = -Wall -std=c++17 -O

SOURCES = $(wildcard src/*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = chessGame


all: $(EXECUTABLE)


$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(notdir $(OBJECTS)) -o $@


%.o: %.cpp
	$(CXX) $(CFLAGS) -fPIC -c $<


clean:
	rm -f $(notdir $(OBJECTS)) $(EXECUTABLE) $(MODS_DIR)/$(SHARED_LIBRARY)


.PHONY: all lib clean
