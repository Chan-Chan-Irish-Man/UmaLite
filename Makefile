CC = gcc
CFLAGS = -Wall -Wextra -O2 -I"./include/"

SOURCES := $(wildcard ./src/*.c)
OBJECTS := $(patsubst ./src/%.c, ./src/%.o, $(SOURCES))
TARGET = UmaLite

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

./src/%.o: ./src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	cmd /C "del /Q $(subst /,\,$(OBJECTS)) $(TARGET).exe"
