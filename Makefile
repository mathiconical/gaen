# project name (output file name)
TARGET = gaen

# target.exe
OUTPUT := $(TARGET).exe

# include path
INCLUDE_PATH=-I./include

# lib path
export SDL2=-L./lib/SDL2/lib -lmingw32 -lSDL2main -lSDL2_ttf -lSDL2 -lSDL2_image -lSDL2_mixer -lopengl32 -llua53 -lglfw3 -lgdi32

# compiler
CXX = g++ -std=c++2a

# linker
LINKER = g++ -std=c++2a

# compiler flags
CXXFLAGS = $(INCLUDE_PATH) $(SDL2)

# linker flags
LFLAGS = -Wall -Wfatal-errors

# folders
SRCDIR = src
OBJDIR = obj
BINDIR = bin

SOURCES := $(wildcard $(SRCDIR)/*.cpp)
INCLUDES := $(wildcard $(SRCDIR)/*.hpp)
OBJECTS := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

$(BINDIR)/$(TARGET): $(OBJECTS)
	@$(LINKER) $(OBJECTS) $(LFLAGS) $(CXXFLAGS) -o $@
	@echo "Linked complete!"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	@$(CXX) $(CXXFLAGS) $(LFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

.PHONY: clean
clean:
	@del $(OBJDIR)\*.o
	@echo "Cleanup complete!"

.PHONY: remove
remove: clean
	@del $(BINDIR)\*.exe
	@echo "Executable removed!"

run: run
	@./$(BINDIR)/$(TARGET)