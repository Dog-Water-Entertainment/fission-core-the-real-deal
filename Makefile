CFLAGS = -I ./include
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr
USELIBS = ./Image.cpp
PERSONALFILES = ./efarmer.cpp ./mguillory.cpp ./bmartinez.cpp

all: walk2

walk2: walk2.cpp log.cpp $(USELIBS) $(PERSONALFILES)
	g++ $(CFLAGS) walk2.cpp $(USELIBS) $(PERSONALFILES) log.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -o walk2

clean:
	rm -f walk2
	rm -f *.o

