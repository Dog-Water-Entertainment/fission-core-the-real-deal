CC = g++

CFLAGS = -I ./include
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr
EFLAGS = -Wall -Wextra

USELIBS = ./Image.cpp log.cpp
UTILITIES = ./utils/Config.cpp
INPUTS = ./inputs/Inputs.cpp
SCENES = ./scene/SceneManager.cpp ./scene/scenes/MapScreen/MapScreen.cpp
PERSONALFILES = ./efarmer.cpp ./mguillory.cpp ./bmartinez.cpp ./emedrano.cpp
MATHLIBS = ./math/Math.cpp
FONTS = libggfonts.a


OURFILES = $(UTILITIES) $(SCENES) $(PERSONALFILES) $(MATHLIBS) $(INPUTS)

FILES = $(USELIBS) $(OURFILES) $(FONTS)


all: fissioncore 

fissioncore: fissioncore.cpp $(FILES) 
	$(CC) $(CFLAGS) fissioncore.cpp $(FILES) $(EFLAGS) $(LFLAGS) -o fissioncore 

clean:
	rm -f fissioncore 
	rm -f *.o

