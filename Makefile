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
LIBRARY_NAME = $(TARGET)
LIBRARY = $(BUILD_PATH)lib$(LIBRARY_NAME).so$(foreach v,$(LIBRARY_VERSION),.$(v))

all: $(BUILD_PATH) $(EXECUTABLE)

standalone: $(BUILD_PATH)
	$(CC) $(CFLAGS) -o$(EXECUTABLE) $(EXESOURCE) $(SOURCES) $(LIBS)

$(EXECUTABLE): $(LIBRARY)
	$(CC) $(CFLAGS) -o$@ $(EXESOURCE) -L$(BUILD_PATH) -l$(LIBRARY_NAME)

$(LIBRARY): $(OBJECTS)
	$(CC) -fPIC -shared -o$@ $(OBJECTS) $(LIBS)

$(BUILD_PATH)%.o: %.cpp
	$(CC) -fPIC -c $(CFLAGS) -o$@ $<

$(BUILD_PATH):
	mkdir -p $@

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE) $(LIBRARY)