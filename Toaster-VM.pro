SOURCES += \
    ToasterVM.cpp \
    Block.cpp \
    Heap.cpp \
    Stack.cpp \
    ManagedHeap.cpp \
    Machine.cpp \
    ComparisonFlagRegister.cpp \
    ExtensionFunction.cpp \
    Interpreter.cpp

HEADERS += \
    Block.hpp \
    Heap.hpp \
    Stack.hpp \
    ManagedHeap.hpp \
    Machine.hpp \
    ComparisonFlagRegister.hpp \
    ExtensionFunction.hpp \
    Interpreter.hpp

QMAKE_CXXFLAGS_DEBUG += -ansi -pedantic
QMAKE_CXXFLAGS_RELEASE += -ansi -pedantic

LIBS += -ldl
