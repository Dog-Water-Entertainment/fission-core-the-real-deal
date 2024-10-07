#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

class Image {
public:
	int width, height;
	unsigned char *data;
	~Image();
	Image(const char *fname);
};

unsigned char *buildAlphaData(Image *img);