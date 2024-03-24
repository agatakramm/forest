CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = -lgif
OBJ = main.o gif_creation.o forest_fire.o tools.o

# Le nom de l'exécutable
EXEC = feu_de_foret

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

main.o: main.c headers.h
	$(CC) -c $(CFLAGS) $<

gif_creation.o: gif_creation.c headers.h
	$(CC) -c $(CFLAGS) $<

forest_fire.o: forest_fire.c headers.h
	$(CC) -c $(CFLAGS) $<

tools.o: tools.c headers.h
	$(CC) -c $(CFLAGS) $<

clean:
	rm -f $(OBJ) $(EXEC)

.PHONY: all clean
