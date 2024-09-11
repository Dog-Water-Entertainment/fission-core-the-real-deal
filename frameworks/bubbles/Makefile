CFLAGS = -I ./include
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: bubbles

bubbles: bubbles.cpp
	g++ $(CFLAGS) bubbles.cpp libggfonts.a -Wall $(LFLAGS) -obubbles

clean:
	rm -f bubbles

