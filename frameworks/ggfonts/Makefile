## Gordon Griesel
## Creation of fonts for OpenGL
## A library will be created here in Makefile
## You will link to it in your C++ compile line
## You will call the font functions within your C++ program code.
##

## note: -fpic flag allows for a shared library to be created.
CFLAGS = -pthread -Wall -Wextra -Werror
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr
##MYLIB  = ./libggfonts.so

##--------------------------------------------------------
## to build a static library
##
## $ ar -cvq libggfonts.a fonts.o fonttex.o
##
## then when compiling, put libggfonts.a on compile line
##--------------------------------------------------------

all: fonts.o fonttex.o staticLib ggtest

fonts.o: fonts.cpp fonts.h defs.h
	g++ -c -Wall -Wextra fonts.cpp

fonttex.o: fonttex.cpp
	g++ -c -Wall -Wextra fonttex.cpp


## No, let's not do it this way.
##
##makelib: fonts.o fonttex.o fonts.h
##	g++ -shared -o $(MYLIB) fonts.o fonttex.o


## Creating static library twice?
## If you build the file while it already exists, it grows larger.
## But, you cannot remove the file if it is not there.
## So, create it, remove it, then create it.
staticLib: fonts.o fonttex.o
	ar -cvq libggfonts.a fonts.o fonttex.o
	rm libggfonts.a
	ar -cvq libggfonts.a fonts.o fonttex.o

## A program to test the fonts
ggtest: ggtest.cpp fonts.o fonttex.o fonts.h log.cpp defs.h
	g++ $(CFLAGS) ggtest.cpp log.cpp fonts.o fonttex.o $(LFLAGS) -o ggtest

clean:
	rm -f ggtest
	rm -f *.o
	rm -f *.so

