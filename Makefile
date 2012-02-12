CC = g++
CFLAGS = -Wall -ansi -pedantic -O3
LIBS = -ldl
EXESOURCE = ToasterVM.cpp
TARGET = $(EXESOURCE:.cpp=)
SOURCES = $(filter-out $(EXESOURCE),$(wildcard *.cpp))
OBJECTS = $(foreach obj,$(SOURCES:.cpp=.o),$(BUILD_PATH)$(obj))
BUILD_PATH = bin/
EXECUTABLE = $(BUILD_PATH)$(TARGET)
LIBRARY_VERSION =
LIBRARY = $(BUILD_PATH)lib$(TARGET).so$(foreach v,$(LIBRARY_VERSION),.$(v))

all: $(BUILD_PATH) $(EXECUTABLE)

$(EXECUTABLE): $(LIBRARY)
	$(CC) $(CFLAGS) -o$(EXECUTABLE) $(EXESOURCE) -L$(BUILD_PATH) -lToasterVM

$(LIBRARY): $(OBJECTS)
	$(CC) -fPIC -shared -o$(LIBRARY) $(OBJECTS) $(LIBS)

$(BUILD_PATH)%.o: %.cpp
	$(CC) -fPIC -c $(CFLAGS) -o$@ $<

$(BUILD_PATH):
	mkdir -p $(BUILD_PATH:/=)

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE) $(LIBRARY)