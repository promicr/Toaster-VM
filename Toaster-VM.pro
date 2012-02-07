SOURCES += \
    ToasterVM.cpp \
    Block.cpp \
    Heap.cpp \
    Stack.cpp \
    ManagedHeap.cpp \
    Machine.cpp \
    ComparisonFlagRegister.cpp \
    ExtensionFunction.cpp \
    Interpreter.cpp \
    Lexer.cpp \
    Token.cpp

HEADERS += \
    Block.hpp \
    Heap.hpp \
    Stack.hpp \
    ManagedHeap.hpp \
    Machine.hpp \
    ComparisonFlagRegister.hpp \
    ExtensionFunction.hpp \
    Interpreter.hpp \
    Lexer.hpp \
    Token.hpp \
    TypeWrappers.hpp \
    Instruction.hpp

QMAKE_CXXFLAGS_DEBUG += -ansi -pedantic
QMAKE_CXXFLAGS_RELEASE += -ansi -pedantic

LIBS += -ldl
