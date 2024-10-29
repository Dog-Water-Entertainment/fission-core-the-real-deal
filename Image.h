#ifndef IMAGE_H
#define IMAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <GL/glx.h>

class Image {
public:
	int width, height;
	unsigned char *data;
	~Image();
	Image(const char *fname);
};

unsigned char *buildAlphaData(Image *img);

void loadTexture(const char *filename, GLuint &tex);

void loadTextureAlpha(const char *filename, GLuint &tex);

#endif