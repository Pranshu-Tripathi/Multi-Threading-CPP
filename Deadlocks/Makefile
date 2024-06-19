# Compiler to use
CC = g++

# Compiler flags
CFLAGS = -Wall -Wextra -pedantic -std=c++17

# Source, Object, and Binary directories
SRCDIR = ./
OBJDIR = ~/Desktop/Pranshu/obj
BINDIR = ~/Desktop/Pranshu/bin

# Target source file
TARGET ?= main.cpp  # Default to main.cpp if no TARGET is specified

# Generate object file name from target source file
TARGET_NAME = $(basename $(notdir $(TARGET)))
OBJECTS = $(OBJDIR)/$(TARGET_NAME).o
EXECUTABLE = $(BINDIR)/$(TARGET_NAME)

# Phony targets to avoid conflicts with files of the same name
.PHONY: all clean

# Default target: build the executable from the specified TARGET
all: $(EXECUTABLE)

# Link object file to create the executable
$(EXECUTABLE): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $^ -o $@

# Compile the target source file to object file
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	@rm -rf $(OBJDIR) $(BINDIR)
