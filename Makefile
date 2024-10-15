CC = g++

CFLAGS = -I ./include
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr
USELIBS = ./Image.cpp
UTILITIES = ./utils/Config.cpp
PERSONALFILES = ./efarmer.cpp ./mguillory.cpp ./bmartinez.cpp ./emedrano.cpp

all: walk2

walk2: walk2.cpp log.cpp $(USELIBS) $(PERSONALFILES)
	$(CC) $(CFLAGS) walk2.cpp $(USELIBS) $(PERSONALFILES) $(UTILITIES) log.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -o walk2

clean:
	rm -f walk2
	rm -f *.o

