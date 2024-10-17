CC = g++

CFLAGS = -I ./include
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr
USELIBS = ./Image.cpp log.cpp
UTILITIES = ./utils/Config.cpp
SCENES = ./scene/SceneManager.cpp
PERSONALFILES = ./efarmer.cpp ./mguillory.cpp ./bmartinez.cpp ./emedrano.cpp
MATHLIBS = ./math/Math.cpp

OURFILES = $(UTILITIES) $(SCENES) $(PERSONALFILES) $(MATHLIBS)

FILES = $(USELIBS) $(OURFILES)


all: walk2

walk2: walk2.cpp log.cpp $(FILES) 
	$(CC) $(CFLAGS) walk2.cpp $(FILES) libggfonts.a -Wall -Wextra $(LFLAGS) -o walk2

clean:
	rm -f walk2
	rm -f *.o

