SOURCES += \
    ToasterVM.cpp \
    Flags.cpp \
    Block.cpp \
    Heap.cpp \
    Stack.cpp \
    ManagedHeap.cpp \
    Machine.cpp

HEADERS += \
    Flags.hpp \
    Block.hpp \
    Heap.hpp \
    Stack.hpp \
    ManagedHeap.hpp \
    Machine.hpp

QMAKE_CXXFLAGS_DEBUG += -ansi -pedantic
QMAKE_CXXFLAGS_RELEASE += -ansi -pedantic
