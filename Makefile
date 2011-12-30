SOURCES=vid.c gradient.c
OBJECTS=$(SOURCES:.c=.o)
EXEC=perlin
MY_CFLAGS += `pkg-config sdl --cflags` -Wall -Werror -O0 -g
MY_LIBS += `pkg-config sdl --libs`

all: $(OBJECTS)
	$(CC) $(LIBS) $(LDFLAGS) $(OBJECTS) $(MY_LIBS) -o $(EXEC)

clean:
	rm -f $(EXEC) $(OBJECTS)

.c.o:
	$(CC) -c $(CFLAGS) $(MY_CFLAGS) $< -o $@

