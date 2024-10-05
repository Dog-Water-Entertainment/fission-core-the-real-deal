CFLAGS = -I ./include
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: walk2

walk2: walk2.cpp log.cpp efarmer.cpp mguillory.cpp
	g++ $(CFLAGS) walk2.cpp efarmer.cpp mguillory.cpp log.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -o walk2

clean:
	rm -f walk2
	rm -f *.o

