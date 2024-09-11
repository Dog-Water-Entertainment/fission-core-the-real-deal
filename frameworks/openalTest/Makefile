#
# To disable OpenAL sound, place comments on the #define
# and library statements.
#
# Like this...
#
#   #-D USE_OPENAL_SOUND \
#   #/usr/lib/x86_64-linux-gnu/libopenal.so \
#   #/usr/lib/x86_64-linux-gnu/libalut.so


all: oalTest oalTest2

oalTest: oalTest.cpp
	g++ oalTest.cpp -ootest \
	-D USE_OPENAL_SOUND \
	/usr/lib/x86_64-linux-gnu/libopenal.so \
	/usr/lib/x86_64-linux-gnu/libalut.so

oalTest2: oalTest2.cpp
	g++ oalTest2.cpp -ootest2 \
	-D USE_OPENAL_SOUND \
	/usr/lib/x86_64-linux-gnu/libopenal.so \
	/usr/lib/x86_64-linux-gnu/libalut.so

clean:
	rm otest otest2

