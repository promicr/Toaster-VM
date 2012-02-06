SOURCES += \
    ToasterVM.cpp \
    Block.cpp \
    Heap.cpp \
    Stack.cpp \
    ManagedHeap.cpp \
    Machine.cpp \
    ComparisonFlagRegister.cpp \
    Parser.cpp \
    ExtensionFunction.cpp

HEADERS += \
    Block.hpp \
    Heap.hpp \
    Stack.hpp \
    ManagedHeap.hpp \
    Machine.hpp \
    ComparisonFlagRegister.hpp \
    Parser.hpp \
    ExtensionFunction.hpp

QMAKE_CXXFLAGS_DEBUG += -ansi -pedantic
QMAKE_CXXFLAGS_RELEASE += -ansi -pedantic

LIBS += -ldl
