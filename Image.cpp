#include "Image.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "efarmer.h"

Image::~Image() { delete [] data; };

Image::Image(const char *fname)
{
    if (fname[0] == '\0')
        return;
    // printf("fname **%s**\n", fname);
    int ppmFlag = 0;
    char name[40];
    strcpy(name, fname);
    int slen = strlen(name);
    char ppmname[80];
    if (strncmp(name + (slen - 4), ".ppm", 4) == 0)
        ppmFlag = 1;
    if (ppmFlag) {
        strcpy(ppmname, name);
    } else {
        name[slen - 4] = '\0';
        // printf("name **%s**\n", name);
        sprintf(ppmname, "%s.ppm", name);
        // printf("ppmname **%s**\n", ppmname);
        char ts[100];
        // system("convert eball.jpg eball.ppm");
        sprintf(ts, "convert %s %s", fname, ppmname);
        system(ts);
    }
    // sprintf(ts, "%s", name);
    // printf("read ppm **%s**\n", ppmname); fflush(stdout);
    FILE *fpi = fopen(ppmname, "r");
    if (fpi) {
        char line[200];
        fgets(line, 200, fpi);
        fgets(line, 200, fpi);
        // skip comments and blank lines
        while (line[0] == '#' || strlen(line) < 2)
            fgets(line, 200, fpi);
        sscanf(line, "%i %i", &width, &height);
        fgets(line, 200, fpi);
        // get pixel data
        int n = width * height * 3;
        data = new unsigned char[n];
        for (int i = 0; i < n; i++)
            data[i] = fgetc(fpi);
        fclose(fpi);
    } else {
        printf("ERROR opening image: %s\n", ppmname);
        Termination::Terminate();
        return;
    }
    if (!ppmFlag)
        unlink(ppmname);
}

unsigned char *buildAlphaData(Image *img) {
	//add 4th component to RGB stream...
	int i;
	unsigned char *newdata, *ptr;
	unsigned char *data = (unsigned char *)img->data;
	newdata = (unsigned char *)malloc(img->width * img->height * 4);
	ptr = newdata;
	unsigned char a,b,c;
	//use the first pixel in the image as the transparent color.
	unsigned char t0 = *(data+0);
	unsigned char t1 = *(data+1);
	unsigned char t2 = *(data+2);
	for (i=0; i<img->width * img->height * 3; i+=3) {
		a = *(data+0);
		b = *(data+1);
		c = *(data+2);
		*(ptr+0) = a;
		*(ptr+1) = b;
		*(ptr+2) = c;
		*(ptr+3) = 1;
		if (a==t0 && b==t1 && c==t2)
			*(ptr+3) = 0;
		//-----------------------------------------------
		ptr += 4;
		data += 3;
	}
	return newdata;
}

void loadTextureAlpha(const char *filename, GLuint &tex) {
    int w, h;
    Image *img = new Image(filename);
    if (img == NULL)
        return;
    w = img->width;
    h = img->height;
    unsigned char *data = buildAlphaData(img);
    if (data == NULL)
        return;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    free(data);
    delete img;
}

void loadTexture(const char *filename, GLuint &tex) {
    int w, h;
    Image *img = new Image(filename);
    if (img == NULL)
        return;
    w = img->width;
    h = img->height;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->data);
    delete img;
}