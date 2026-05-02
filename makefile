CC = gcc
CFLAGS = -Wall -I./include
LIBS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lopengl32 -lglu32
TARGET = program.exe

# Források
SOURCES = src/main.c src/texture.c src/model_loader.c src/collision.c src/math3d.c src/shadow_simple.c src/rain.c src/app.c

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET) $(LIBS)

clean:
	if exist $(TARGET) del $(TARGET)

run: $(TARGET)
	$(TARGET)

.PHONY: all clean run