# Project Name (executable)
PROJECT = squarefighter
# Compiler
CC = gcc

# Run Options
COMMANDLINE_OPTIONS =
LD_LIBRARY_PATH=/lib:/usr/lib:/usr/local/lib

# Compiler options during compilation
COMPILE_OPTIONS =
LINK_OPTIONS =

#Header include directories
HEADERS = -D_REENTRANT
#Libraries for linking
LIBS = -lSDL2 -lSDL2_image -lchipmunk

# Dependency options
DEPENDENCY_OPTIONS = -MM

#-- Do not edit below this line --

# Subdirs to search for additional source files
SUBDIRS := $(shell ls -F | grep "\/" )
DIRS := ./ $(SUBDIRS)
SOURCE_FILES := $(foreach d, $(DIRS), $(wildcard $(d)*.c) )

# Create an object file of every c file
OBJECTS = $(patsubst %.c, %.o, $(SOURCE_FILES))

# Dependencies
DEPENDENCIES = $(patsubst %.c, %.d, $(SOURCE_FILES))

# Create .d files
%.d: %.c
	$(CC) $(DEPENDENCY_OPTIONS) $< -MT "$*.o $*.d" -MF $*.d

# Make $(PROJECT) the default target
all: $(DEPENDENCIES) $(PROJECT)

$(PROJECT): $(OBJECTS)
	$(CC) $(LINK_OPTIONS) -o $(PROJECT) $(OBJECTS) $(LIBS)

# Include dependencies (if there are any)
ifneq "$(strip $(DEPENDENCIES))" ""
  include $(DEPENDENCIES)
endif

# Compile every c file to an object
%.o: %.c
	$(CC) -c $(COMPILE_OPTIONS) -o $@ $< $(HEADERS)

# Build & Run Project
run: $(PROJECT)
	LD_LIBRARY_PATH=$(LD_LIBRARY_PATH) ./$(PROJECT) $(COMMANDLINE_OPTIONS)

# Clean & Debug
.PHONY: makefile-debug
makefile-debug:

.PHONY: clean
clean:
	rm -f $(PROJECT) $(OBJECTS)

.PHONY: depclean
depclean:
	rm -f $(DEPENDENCIES)

clean-all: clean depclean
