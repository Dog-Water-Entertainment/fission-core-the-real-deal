CC = g++

CFLAGS = -I ./include
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

USELIBS = ./Image.cpp log.cpp
UTILITIES = ./utils/Config.cpp
INPUTS = ./inputs/Inputs.cpp
SCENES = ./scene/SceneManager.cpp ./scene/scenes/MapScreen/MapScreen.cpp
PERSONALFILES = ./efarmer.cpp ./mguillory.cpp ./bmartinez.cpp ./emedrano.cpp
MATHLIBS = ./math/Math.cpp

OURFILES = $(UTILITIES) $(SCENES) $(PERSONALFILES) $(MATHLIBS) $(INPUTS)

FILES = $(USELIBS) $(OURFILES)


all: fissioncore 

fissioncore: fissioncore.cpp log.cpp $(FILES) 
	$(CC) $(CFLAGS) fissioncore.cpp $(FILES) libggfonts.a -Wall -Wextra $(LFLAGS) -o fissioncore 

clean:
	rm -f walk2
	rm -f *.o

