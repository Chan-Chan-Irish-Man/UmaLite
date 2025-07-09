# Compiler to use
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -O2

# Target executable name
TARGET = UmaLite

# Source files
SOURCES = main.c generate.c generateRace.c race.c statUp.c inheritance.c ui.c

# Object files
OBJECTS = $(SOURCES:.c=.o)

# Default target
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

# Compile source files to object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	del /Q $(OBJECTS) $(TARGET).exe

# Run
honse:
	$(TARGET).exe

# Phony targets
.PHONY: all clean
