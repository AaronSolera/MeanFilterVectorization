CC = gcc
NEON = -mfpu=neon
ALLEGRO_FLAGS = -lallegro -lallegro_image -lallegro_primitives

.PHONY: all

all: main

main:
	$(CC) -o ./bin/main ./src/main.c ./src/serial_mean_filter.c $(ALLEGRO_FLAGS) -lm && ./bin/main
