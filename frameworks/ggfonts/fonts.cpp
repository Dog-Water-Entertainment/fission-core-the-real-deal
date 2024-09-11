//Fonts for use with OpenGL
//Gordon Griesel
//2007 - 2017
//
//Texture-mapped fonts.
//Look at the code and .ppm files to see how you might add fonts of your own.
//
#include "defs.h"
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <malloc.h>
#include <stdarg.h>
#include <unistd.h>
#include <GL/glx.h>
//#include "ppm.h"
#include "log.h"


//To save compressed files to fonttex.cpp...
// 1. deactivate read-from-compressed
//    activate show-compressed-printout
// 2. compile and run ggtest
// 3. rename x.x to fonttex.cpp
// 4. activate read-from-compressed
//    deactivate show-compressed-printout

//#define SHOW_COMPRESSED_PRINTOUT
#define READ_FROM_COMPRESSED


//These are macros
#define RGB(r,g,b)     ((int)r + (int)((int)g<<8) + (int)((int)b<<16))
#define GetRValue(rgb) (unsigned char)((unsigned int)rgb&0x000000ff)
#define GetGValue(rgb) (unsigned char)((unsigned int) \
                       ((unsigned int)rgb&0x0000ff00) >>  8)
#define GetBValue(rgb) (unsigned char)((unsigned int) \
                       ((unsigned int)rgb&0x00ff0000) >> 16)

//Each font set has 128 characters.
//Each character has a start position and a length.
int cstart_a06[128], clen_a06[128];
int cstart_a07[128], clen_a07[128];
int cstart_a08[128], clen_a08[128];
int cstart_a8b[128], clen_a8b[128];
int cstart_a12[128], clen_a12[128];
int cstart_a13[128], clen_a13[128];
int cstart_a10[128], clen_a10[128];
int cstart_a16[128], clen_a16[128];
int cstart_c17[128], clen_c17[128];
int cstart_a40[128], clen_a40[128];
//These arrays store the texture coordinates for each character.
//Start and end x, start and end y.
//Used to build a QUAD that will draw the character. 
float tx_a06[128][2], ty_a06[128][2];
float tx_a07[128][2], ty_a07[128][2];
float tx_a08[128][2], ty_a08[128][2];
float tx_a8b[128][2], ty_a8b[128][2];
float tx_a10[128][2], ty_a10[128][2];
float tx_a12[128][2], ty_a12[128][2];
float tx_a13[128][2], ty_a13[128][2];
float tx_a16[128][2], ty_a16[128][2];
float tx_c17[128][2], ty_c17[128][2];
float tx_a40[128][2], ty_a40[128][2];
//Texture IDs
unsigned int a06_texture_no;
unsigned int a07_texture_no;
unsigned int a08_texture_no;
unsigned int a8b_texture_no;
unsigned int a10_texture_no;
unsigned int a12_texture_no;
unsigned int a13_texture_no;
unsigned int a16_texture_no;
unsigned int c17_texture_no;
unsigned int a40_texture_no;
//
void initialize_fonts(void);
void cleanup_fonts(void);
void get_compressed_tm(Texmap *tm, unsigned char *ptr1);
void build_gl_texmap(Texmap *tm, unsigned int *texnum);
void load_ggfont(int psize);
void ggprint06(Rect *r, int advance, int cref, const char *fmt, ...);
void ggprint07(Rect *r, int advance, int cref, const char *fmt, ...);
void ggprint08(Rect *r, int advance, int cref, const char *fmt, ...);
void ggprint8b(Rect *r, int advance, int cref, const char *fmt, ...);
void ggprint10(Rect *r, int advance, int cref, const char *fmt, ...);
void ggprint12(Rect *r, int advance, int cref, const char *fmt, ...);
void ggprint13(Rect *r, int advance, int cref, const char *fmt, ...);
void ggprint16(Rect *r, int advance, int cref, const char *fmt, ...);
void ggprint17(Rect *r, int advance, int cref, const char *fmt, ...);
void ggprint40(Rect *r, int advance, int cref, const char *fmt, ...);
//void ggprint8b_sz(Rect *r, int adv, int cref, int sz, const char *fmt, ...);
void ggprint16_sz(Rect *r, int adve, int cref, float sz, const char *fmt, ...);
int ggprint8b_chat(Rect *r, int advance, int cref, const char *fmt, ...);
int ggprint8b_wordwrap(Rect *r, int advance, int cref, const char *fmt, ...);
void ggprint13nb(Rect *r, int advance, int cref, const char *fmt, ...);
void ggprint06nb(Rect *r, int advance, int cref, const char *fmt, ...);
void ggprint8b_nb(Rect *r, int advance, int cref, const char *fmt, ...);
int get_length(const char *fmt, ...);
int find_next_white(Texmap *tm, int *xpos);
int find_next_yellow(Texmap *tm, int *xpos);
int tex_read_ggfont_bmp(char *fileName, Texmap *tm);
int read_texture_from_ppm(const char *fileName);

//#define STOP_ALL_FONT_WRITING

// !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_'abcdefghijklmnopqrstuvwxyz{|}~
//
extern unsigned char ams6ppm[];
extern unsigned char av7ppm[];
extern unsigned char arial8ppm[];
extern unsigned char arial8bppm[];
extern unsigned char arial10ppm[];
extern unsigned char arial12ppm[];
extern unsigned char arial13ppm[];
extern unsigned char arial16ppm[];
extern unsigned char courier16ppm[];
extern unsigned char arial40ppm[];

unsigned char *tempbmp;
unsigned char *tempptr;


//the ppm functions brought in from ppm utilities.

typedef struct t_ppmimage {
	int width;
	int height;
	void *data;
} Ppmimage;

Ppmimage *ppm6GetImage_local(const char *filename)
{
	int i, j, width, height, size, maxval, ntries;
	char ts[4096];
	unsigned char c;
	unsigned char *p;
	FILE *fpi;
	Ppmimage *image = (Ppmimage *)malloc(sizeof(Ppmimage));
	if (!image) {
		printf("ERROR: out of memory\n");
		exit(EXIT_FAILURE);
	}
	fpi = fopen(filename, "r");
	if (!fpi) {
		printf("ERROR: cannot open file **%s** for reading.\n", filename);
		exit(EXIT_FAILURE);
	}
	image->data=NULL;
	char *x = fgets(ts, 4, fpi);
	if (x){}
	if (strncmp(ts, "P6" ,2) != 0) {
		printf("ERROR: File is not ppm RAW format.\n");
		exit(EXIT_FAILURE);
	}
	//comments?
	while(1) {
		c = fgetc(fpi);
		if (c != '#')
			break;
		//read until newline
		ntries=0;
		while(1) {
			//to avoid infinite loop...
			if (++ntries > 10000) {
				printf("ERROR: too many blank lines in **%s**\n", filename);
				exit(EXIT_FAILURE);
			}
			c = fgetc(fpi);
			if (c == '\n')
				break;
		}
	}
	ungetc(c, fpi);
	int r = fscanf(fpi, "%u%u%u", &width, &height, &maxval);
	if (r){}
	//
	//get past any newline or carrage-return
	ntries=0;
	while(1) {
		//to avoid infinite loop...
		if (++ntries > 10000) {
			printf("ERROR: too many blank lines in **%s**\n", filename);
			exit(EXIT_FAILURE);
		}
		c = fgetc(fpi);
		if (c != 10 && c != 13) {
			ungetc(c, fpi);
			break;
		}
	}
	//
	size = width * height * 3;
	image->data = (unsigned char *)malloc(size);
	if (!image->data) {
		printf("ERROR: no memory for image data.\n");
		exit(EXIT_FAILURE);
	}
	image->width = width;
	image->height = height;
	p = (unsigned char *)image->data;
	for (i=0; i<height; i++) {
		for (j=0; j<width; j++) {
			*p = fgetc(fpi); p++;
			*p = fgetc(fpi); p++;
			*p = fgetc(fpi); p++;
		}
	}
	fclose(fpi);
	return image;
}

void ppm6CleanupImage_local(Ppmimage *image)
{
	if (image != NULL) {
		if (image->data != NULL)
			free(image->data);
		free(image);
	}
}

int texState()
{
	#ifdef READ_FROM_COMPRESSED
	return 0;
	#else //READ_FROM_COMPRESSED
	return 1;
	#endif //READ_FROM_COMPRESSED
}

int get_length(const char *fmt, ...)
{
	int k;

	char text[512];
	va_list ap;
	if (fmt == NULL)
		return 0;
	va_start(ap, fmt);
	vsprintf(text, fmt, ap);
	va_end(ap);
	//
	const int slen = strlen(text);
	int fx = 0;
	for (k=0; k<slen; ++k) {
		fx += ((clen_a8b[(text[k]-32)])) + 1;
	}
	return fx;
}

void ggprint16_sz(Rect *r, int advance, int cref, float sz, const char *fmt, ...)
{
	int k, ascii;
	float tx[2],ty[2];
	float fx = (float)(r->left);
	float fy = (float)(r->bot);
	float lp;
	//height of the @ symbol is 46 (span of the largest symbol)
	const float hp = 46.0f;
	//height of the @ symbol is 46 (span of the largest symbol)
	//const float hp = 20.0f;

	//printf("ggprint16_sz()...\n");

	#ifdef STOP_ALL_FONT_WRITING
	return;
	#endif //STOP_ALL_FONT_WRITING
	char text[512];
	va_list ap;
	if (fmt == NULL)
		return;
	va_start(ap, fmt);
	vsprintf(text, fmt, ap);
	va_end(ap);
	//
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(GetBValue(cref),GetGValue(cref),GetRValue(cref), 255);
	glBindTexture(GL_TEXTURE_2D, a16_texture_no);
	const int slen = strlen(text);
	if (r->center) {
		for (k=0; k<slen; ++k) {
			fx += ((float)(clen_a16[(text[k]-32)]*sz)) + 4.0f*sz;
		}
		//backup text start position.
		const int tlen = (int)fx - r->left;
		fx = (float)(r->left - (tlen>>1));
	}
	glBegin(GL_QUADS);
	for (k=0; k<slen; ++k) {
		ascii = text[k] - 32;
		lp = (float)(clen_a16[ascii]);
		tx[0] = tx_a16[ascii][0];
		tx[1] = tx_a16[ascii][1];
		ty[0] = ty_a16[ascii][0];
		ty[1] = ty_a16[ascii][1];
		glTexCoord2f(tx[0],ty[0]); glVertex2f(fx, fy);
		glTexCoord2f(tx[0],ty[1]); glVertex2f(fx, fy+hp*sz);
		glTexCoord2f(tx[1],ty[1]); glVertex2f(fx+lp*sz,fy+hp*sz);
		glTexCoord2f(tx[1],ty[0]); glVertex2f(fx+lp*sz,fy);
		fx += (lp*sz + 4.0f*sz);
	}
	glEnd();
	//printf("tlen:%f\n",tlen);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_ALPHA_TEST);
	r->bot -= (int)( (float)advance * sz );
}


void ggprint8b(Rect *r, int advance, int cref, const char *fmt, ...)
{
	int k, ascii;
	float tx[2],ty[2];
	float fx = (float)(r->left);
	float fy = (float)(r->bot);
	float lp;
	const float hp = 10.0f;  //max height of tallest symbol

	#ifdef STOP_ALL_FONT_WRITING
	return;
	#endif //STOP_ALL_FONT_WRITING
	char text[512];
	va_list ap;
	if (fmt == NULL)
		return;
	va_start(ap, fmt);
	vsprintf(text, fmt, ap);
	va_end(ap);
	//
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(GetBValue(cref),GetGValue(cref),GetRValue(cref), 255);
	glBindTexture(GL_TEXTURE_2D, a8b_texture_no);
	const int slen = strlen(text);
	if (r->center) {
		for (k=0; k<slen; ++k) {
			fx += ((float)(clen_a8b[(text[k]-32)])) + 1.0f;
		}
		//backup text start position.
		const int tlen = (int)fx - r->left;
		fx = (float)(r->left - (tlen>>1));
	}
	glBegin(GL_QUADS);
	for (k=0; k<slen; ++k) {
		ascii = text[k] - 32;
		lp = (float)(clen_a8b[ascii]);
		tx[0] = tx_a8b[ascii][0];
		tx[1] = tx_a8b[ascii][1];
		ty[0] = ty_a8b[ascii][0];
		ty[1] = ty_a8b[ascii][1];
		glTexCoord2f(tx[0],ty[0]); glVertex2f(fx, fy);
		glTexCoord2f(tx[0],ty[1]); glVertex2f(fx, fy+hp);
		glTexCoord2f(tx[1],ty[1]); glVertex2f(fx+lp,fy+hp);
		glTexCoord2f(tx[1],ty[0]); glVertex2f(fx+lp,fy);
		fx += (lp + 1.0f);
	}
	glEnd();
	//printf("tlen:%f\n",tlen);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_ALPHA_TEST);
	r->bot -= advance;
}


int ggprint8b_wordwrap(Rect *r, int advance, int cref, const char *fmt, ...)
{
	//returns 2 if line had to be wrapped.
	int k, ascii;
	float tx[2],ty[2];
	float fx = (float)(r->left);
	float fy = (float)(r->bot);
	float lp;
	const float hp = 10.0f;  //max height of tallest symbol

	const int rmargin = r->right;
	const int lmargin = r->top;

	int ret=1;

	//printf("ggprint8b_wordwrap(rmargin: %i)...\n",rmargin);

	#ifdef STOP_ALL_FONT_WRITING
	return;
	#endif //STOP_ALL_FONT_WRITING
	char text[512];
	va_list ap;
	if (fmt == NULL)
		return 0;
	va_start(ap, fmt);
	vsprintf(text, fmt, ap);
	va_end(ap);
	//
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(GetBValue(cref),GetGValue(cref),GetRValue(cref), 255);
	glBindTexture(GL_TEXTURE_2D, a8b_texture_no);
	const int slen = strlen(text);
	//Will the text overrun the right margin???
	int bspot    =0;
	int lastspace=0;
	float tfx = fx;
	for (k=0; k<slen; ++k) {
		if (text[k] == 32)
			lastspace = k;
		tfx += ((float)(clen_a8b[(text[k]-32)])) + 1.0f;
		if ((int)tfx > rmargin) {
			bspot = k;
			break;
		}
	}
	glBegin(GL_QUADS);
	if (bspot) {
		//Text overruns the right margin, at bspot.
		//printf("Text overruns the right margin.
		//bspot: %i   lastspace: %i\n",bspot,lastspace);
		if (!lastspace) {
			//No spaces were found before the overrun.
			//User typed a long continuous string of characters.
			//Just print it, and look for any space following it.
			//Start next string there.
			for (k=0; k<bspot; ++k) {
				ascii = text[k] - 32;
				lp = (float)(clen_a8b[ascii]);
				tx[0] = tx_a8b[ascii][0];
				tx[1] = tx_a8b[ascii][1];
				ty[0] = ty_a8b[ascii][0];
				ty[1] = ty_a8b[ascii][1];
				glTexCoord2f(tx[0],ty[0]); glVertex2f(fx, fy);
				glTexCoord2f(tx[0],ty[1]); glVertex2f(fx, fy+hp);
				glTexCoord2f(tx[1],ty[1]); glVertex2f(fx+lp,fy+hp);
				glTexCoord2f(tx[1],ty[0]); glVertex2f(fx+lp,fy);
				fx += (lp + 1.0f);
			}
			//look for a space in the string before a null.
			int start=0;
			for (k=bspot; k<slen; ++k) {
				if (text[k] == 32) {
					start = k;
					break;
				}
			}
			if (start) {
				//printf("found a space after the break.\n");
				//There is some more string to print out.
				fx = (float)lmargin;
				r->bot -= advance;
				fy = (float)(r->bot);
				for (k=bspot; k<slen; ++k) {
					ascii = text[k] - 32;
					lp = (float)(clen_a8b[ascii]);
					tx[0] = tx_a8b[ascii][0];
					tx[1] = tx_a8b[ascii][1];
					ty[0] = ty_a8b[ascii][0];
					ty[1] = ty_a8b[ascii][1];
					glTexCoord2f(tx[0],ty[0]); glVertex2f(fx, fy);
					glTexCoord2f(tx[0],ty[1]); glVertex2f(fx, fy+hp);
					glTexCoord2f(tx[1],ty[1]); glVertex2f(fx+lp,fy+hp);
					glTexCoord2f(tx[1],ty[0]); glVertex2f(fx+lp,fy);
					fx += (lp + 1.0f);
				}
			}
		}
		else {
			//A space was found before the overrun.
			//Print the line up to that space.
			for (k=0; k<lastspace; ++k) {
				ascii = text[k] - 32;
				lp = (float)(clen_a8b[ascii]);
				tx[0] = tx_a8b[ascii][0];
				tx[1] = tx_a8b[ascii][1];
				ty[0] = ty_a8b[ascii][0];
				ty[1] = ty_a8b[ascii][1];
				glTexCoord2f(tx[0],ty[0]); glVertex2f(fx, fy);
				glTexCoord2f(tx[0],ty[1]); glVertex2f(fx, fy+hp);
				glTexCoord2f(tx[1],ty[1]); glVertex2f(fx+lp,fy+hp);
				glTexCoord2f(tx[1],ty[0]); glVertex2f(fx+lp,fy);
				fx += (lp + 1.0f);
			}
			//There is some more string to print out.
			fx = (float)lmargin;
			r->bot -= advance;
			fy = (float)(r->bot);
			for (k=lastspace+1; k<slen; ++k) {
				ascii = text[k] - 32;
				lp = (float)(clen_a8b[ascii]);
				tx[0] = tx_a8b[ascii][0];
				tx[1] = tx_a8b[ascii][1];
				ty[0] = ty_a8b[ascii][0];
				ty[1] = ty_a8b[ascii][1];
				glTexCoord2f(tx[0],ty[0]); glVertex2f(fx, fy);
				glTexCoord2f(tx[0],ty[1]); glVertex2f(fx, fy+hp);
				glTexCoord2f(tx[1],ty[1]); glVertex2f(fx+lp,fy+hp);
				glTexCoord2f(tx[1],ty[0]); glVertex2f(fx+lp,fy);
				fx += (lp + 1.0f);
			}
		}
		ret=2;
	}
	else {
		for (k=0; k<slen; ++k) {
			ascii = text[k] - 32;
			lp = (float)(clen_a8b[ascii]);
			tx[0] = tx_a8b[ascii][0];
			tx[1] = tx_a8b[ascii][1];
			ty[0] = ty_a8b[ascii][0];
			ty[1] = ty_a8b[ascii][1];
			glTexCoord2f(tx[0],ty[0]); glVertex2f(fx, fy);
			glTexCoord2f(tx[0],ty[1]); glVertex2f(fx, fy+hp);
			glTexCoord2f(tx[1],ty[1]); glVertex2f(fx+lp,fy+hp);
			glTexCoord2f(tx[1],ty[0]); glVertex2f(fx+lp,fy);
			fx += (lp + 1.0f);
		}
	}
	glEnd();
	//printf("tlen:%f\n",tlen);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_ALPHA_TEST);
	r->bot -= advance;
	return ret;
}


int ggprint8b_chat(Rect *r, int advance, int cref, const char *fmt, ...)
{
	//returns the last position, so we can show a blinking cursor.
	int k, ascii;
	float tx[2],ty[2];
	float fx = (float)(r->left);
	float fy = (float)(r->bot);
	float lp;
	const float hp = 10.0f;  //max height of tallest symbol

	#ifdef STOP_ALL_FONT_WRITING
	return;
	#endif //STOP_ALL_FONT_WRITING
	char text[512];
	va_list ap;
	if (fmt == NULL)
		return 0;
	va_start(ap, fmt);
	vsprintf(text, fmt, ap);
	va_end(ap);
	//
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(GetBValue(cref),GetGValue(cref),GetRValue(cref), 255);
	glBindTexture(GL_TEXTURE_2D, a8b_texture_no);
	const int slen = strlen(text);
	if (r->center) {
		for (k=0; k<slen; ++k) {
			//ascii = text[k] - 32;
			//fx += ((float)(clen_a8b[ascii])) + 1.0f;
			fx += ((float)(clen_a8b[(text[k]-32)])) + 1.0f;
		}
		//backup text start position.
		const int tlen = (int)fx - r->left;
		fx = (float)(r->left - (tlen>>1));
	}
	glBegin(GL_QUADS);
	for (k=0; k<slen; ++k) {
		ascii = text[k] - 32;
		lp = (float)(clen_a8b[ascii]);
		tx[0] = tx_a8b[ascii][0];
		tx[1] = tx_a8b[ascii][1];
		ty[0] = ty_a8b[ascii][0];
		ty[1] = ty_a8b[ascii][1];
		glTexCoord2f(tx[0],ty[0]); glVertex2f(fx, fy);
		glTexCoord2f(tx[0],ty[1]); glVertex2f(fx, fy+hp);
		glTexCoord2f(tx[1],ty[1]); glVertex2f(fx+lp,fy+hp);
		glTexCoord2f(tx[1],ty[0]); glVertex2f(fx+lp,fy);
		fx += (lp + 1.0f);
	}
	glEnd();
	//printf("tlen:%f\n",tlen);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_ALPHA_TEST);
	r->bot -= advance;
	return (int)fx - r->left;
}

void ggprint8b_nb(Rect *r, int advance, int cref, const char *fmt, ...)
{
	int k, ascii;
	float tx[2],ty[2];
	float fx = (float)(r->left);
	const float fy = (float)(r->bot);
	float lp;
	const float hp = 10.0f;  //max height of tallest symbol

	#ifdef STOP_ALL_FONT_WRITING
	return;
	#endif //STOP_ALL_FONT_WRITING
	char text[512];
	va_list ap;
	if (fmt == NULL)
		return;
	va_start(ap, fmt);
	vsprintf(text, fmt, ap);
	va_end(ap);
	//
	glColor4ub(GetBValue(cref),GetGValue(cref),GetRValue(cref), 255);
	const int slen = strlen(text);
	if (r->center) {
		for (k=0; k<slen; ++k) {
			fx += ((float)(clen_a8b[(text[k] - 32)])) + 1.0f;
		}
		//backup text start position.
		const int tlen = (int)fx - r->left;
		fx = (float)(r->left - (tlen>>1));
	}
	glBegin(GL_QUADS);
	for (k=0; k<slen; ++k) {
		ascii = text[k] - 32;
		lp = (float)(clen_a8b[ascii]);
		tx[0] = tx_a8b[ascii][0];
		tx[1] = tx_a8b[ascii][1];
		ty[0] = ty_a8b[ascii][0];
		ty[1] = ty_a8b[ascii][1];
		glTexCoord2f(tx[0],ty[0]); glVertex2f(fx, fy);
		glTexCoord2f(tx[0],ty[1]); glVertex2f(fx, fy+hp);
		glTexCoord2f(tx[1],ty[1]); glVertex2f(fx+lp,fy+hp);
		glTexCoord2f(tx[1],ty[0]); glVertex2f(fx+lp,fy);
		fx += (lp + 1.0f);
	}
	glEnd();
	r->bot -= advance;
}


void ggprint06(Rect *r, int advance, int cref, const char *fmt, ...)
{
	int k, ascii;
	float tx[2],ty[2];
	float fx = (float)(r->left);
	float fy = (float)(r->bot);
	float lp;
	const float hp = 10.0f;  //max height of tallest symbol

	#ifdef STOP_ALL_FONT_WRITING
	return;
	#endif //STOP_ALL_FONT_WRITING
	char text[512];
	va_list ap;
	if (fmt == NULL)
		return;
	va_start(ap, fmt);
	vsprintf(text, fmt, ap);
	va_end(ap);
	//
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(GetBValue(cref),GetGValue(cref),GetRValue(cref), 255);
	glBindTexture(GL_TEXTURE_2D, a06_texture_no);
	const int slen = strlen(text);
	glBegin(GL_QUADS);
	for (k=0; k<slen; ++k) {
		ascii = text[k] - 32;
		lp = (float)(clen_a06[ascii]);
		tx[0] = tx_a06[ascii][0];
		tx[1] = tx_a06[ascii][1];
		ty[0] = ty_a06[ascii][0];
		ty[1] = ty_a06[ascii][1];
		glTexCoord2f(tx[0],ty[0]); glVertex2f(fx, fy);
		glTexCoord2f(tx[0],ty[1]); glVertex2f(fx, fy+hp);
		glTexCoord2f(tx[1],ty[1]); glVertex2f(fx+lp,fy+hp);
		glTexCoord2f(tx[1],ty[0]); glVertex2f(fx+lp,fy);
		fx += lp + 1.0f;
	}
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_ALPHA_TEST);
	r->bot -= advance;
}


void ggprint06nb(Rect *r, int advance, int cref, const char *fmt, ...)
{
	int k, ascii;
	float tx[2],ty[2];
	float fx = (float)(r->left);
	float fy = (float)(r->bot);
	float lp;
	const float hp = 10.0f;  //max height of tallest symbol

	#ifdef STOP_ALL_FONT_WRITING
	return;
	#endif //STOP_ALL_FONT_WRITING
	char text[512];
	va_list ap;
	if (fmt == NULL)
		return;
	va_start(ap, fmt);
	vsprintf(text, fmt, ap);
	va_end(ap);
	//
	glColor4ub(GetBValue(cref),GetGValue(cref),GetRValue(cref), 255);
	const int slen = strlen(text);
	glBegin(GL_QUADS);
	for (k=0; k<slen; ++k) {
		ascii = text[k] - 32;
		lp = (float)(clen_a06[ascii]);
		tx[0] = tx_a06[ascii][0];
		tx[1] = tx_a06[ascii][1];
		ty[0] = ty_a06[ascii][0];
		ty[1] = ty_a06[ascii][1];
		glTexCoord2f(tx[0],ty[0]); glVertex2f(fx, fy);
		glTexCoord2f(tx[0],ty[1]); glVertex2f(fx, fy+hp);
		glTexCoord2f(tx[1],ty[1]); glVertex2f(fx+lp,fy+hp);
		glTexCoord2f(tx[1],ty[0]); glVertex2f(fx+lp,fy);
		fx += lp + 1.0f;
	}
	glEnd();
	r->bot -= advance;
}


void ggprint07(Rect *r, int advance, int cref, const char *fmt, ...)
{
	int k, ascii;
	float tx[2],ty[2];
	float fx = (float)(r->left);
	float fy = (float)(r->bot);
	float lp;
	const float hp = 10.0f;  //max height of tallest symbol

	#ifdef STOP_ALL_FONT_WRITING
	return;
	#endif //STOP_ALL_FONT_WRITING
	char text[512];
	va_list ap;
	if (fmt == NULL)
		return;
	va_start(ap, fmt);
	vsprintf(text, fmt, ap);
	va_end(ap);
	//
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(GetBValue(cref),GetGValue(cref),GetRValue(cref), 255);
	glBindTexture(GL_TEXTURE_2D, a07_texture_no);
	const int slen = strlen(text);
	glBegin(GL_QUADS);
	for (k=0; k<slen; ++k) {
		ascii = text[k] - 32;
		lp = (float)(clen_a07[ascii]);
		tx[0] = tx_a07[ascii][0];
		tx[1] = tx_a07[ascii][1];
		ty[0] = ty_a07[ascii][0];
		ty[1] = ty_a07[ascii][1];
		glTexCoord2f(tx[0],ty[0]); glVertex2f(fx, fy);
		glTexCoord2f(tx[0],ty[1]); glVertex2f(fx, fy+hp);
		glTexCoord2f(tx[1],ty[1]); glVertex2f(fx+lp,fy+hp);
		glTexCoord2f(tx[1],ty[0]); glVertex2f(fx+lp,fy);
		fx += lp + 1.0f;
	}
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_ALPHA_TEST);
	r->bot -= advance;
}


void ggprint08(Rect *r, int advance, int cref, const char *fmt, ...)
{
	int k, ascii;
	float tx[2],ty[2];
	float fx = (float)(r->left);
	float fy = (float)(r->bot);
	float lp;
	const float hp = 10.0f;  //max height of tallest symbol

	#ifdef STOP_ALL_FONT_WRITING
	return;
	#endif //STOP_ALL_FONT_WRITING
	char text[512];
	va_list ap;
	if (fmt == NULL)
		return;
	va_start(ap, fmt);
	vsprintf(text, fmt, ap);
	va_end(ap);
	//
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(GetBValue(cref),GetGValue(cref),GetRValue(cref), 255);
	glBindTexture(GL_TEXTURE_2D, a08_texture_no);
	const int slen = strlen(text);
	glBegin(GL_QUADS);
	for (k=0; k<slen; ++k) {
		ascii = text[k] - 32;
		lp = (float)(clen_a08[ascii]);
		tx[0] = tx_a08[ascii][0];
		tx[1] = tx_a08[ascii][1];
		ty[0] = ty_a08[ascii][0];
		ty[1] = ty_a08[ascii][1];
		glTexCoord2f(tx[0],ty[0]); glVertex2f(fx, fy);
		glTexCoord2f(tx[0],ty[1]); glVertex2f(fx, fy+hp);
		glTexCoord2f(tx[1],ty[1]); glVertex2f(fx+lp,fy+hp);
		glTexCoord2f(tx[1],ty[0]); glVertex2f(fx+lp,fy);
		fx += lp + 1.0f;
	}
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_ALPHA_TEST);
	r->bot -= advance;
}

void ggprint10(Rect *r, int advance, int cref, const char *fmt, ...)
{
	int k, ascii;
	float tx[2],ty[2];
	float fx = (float)(r->left);
	float fy = (float)(r->bot);
	float lp;
	const float hp = 13.0f;  //max height of tallest symbol

	#ifdef STOP_ALL_FONT_WRITING
	return;
	#endif //STOP_ALL_FONT_WRITING
	char text[512];
	va_list ap;
	if (fmt == NULL)
		return;
	va_start(ap, fmt);
	vsprintf(text, fmt, ap);
	va_end(ap);
	//
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(GetBValue(cref),GetGValue(cref),GetRValue(cref), 255);
	glBindTexture(GL_TEXTURE_2D, a10_texture_no);
	const int slen = strlen(text);
	glBegin(GL_QUADS);
	for (k=0; k<slen; ++k) {
		ascii = text[k] - 32;
		lp = (float)(clen_a10[ascii]);
		tx[0] = tx_a10[ascii][0];
		tx[1] = tx_a10[ascii][1];
		ty[0] = ty_a10[ascii][0];
		ty[1] = ty_a10[ascii][1];
		glTexCoord2f(tx[0],ty[0]); glVertex2f(fx, fy);
		glTexCoord2f(tx[0],ty[1]); glVertex2f(fx, fy+hp);
		glTexCoord2f(tx[1],ty[1]); glVertex2f(fx+lp,fy+hp);
		glTexCoord2f(tx[1],ty[0]); glVertex2f(fx+lp,fy);
		fx += lp + 1.0f;
	}
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_ALPHA_TEST);
	r->bot -= advance;
}


void ggprint13(Rect *r, int advance, int cref, const char *fmt, ...)
{
	//this is a bold 12 point font.
	int k, ascii;
	float tx[2],ty[2];
	float fx = (float)r->left;
	float fy = (float)r->bot;
	float lp;
	const float hp = 16.0f;  //max height of tallest symbol

	#ifdef STOP_ALL_FONT_WRITING
	return;
	#endif //STOP_ALL_FONT_WRITING
	char text[512];
	va_list ap;
	if (fmt == NULL)
		return;
	va_start(ap, fmt);
	vsprintf(text, fmt, ap);
	va_end(ap);
	//
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(GetBValue(cref),GetGValue(cref),GetRValue(cref), 255);
	glBindTexture(GL_TEXTURE_2D, a13_texture_no);
	const int slen = strlen(text);
	if (slen > 200)
		return;
	if (r->center) {
		for (k=0; k<slen; ++k) {
			ascii = text[k] - 32;
			fx += ((float)(clen_a13[ascii])) + 1.0f;
		}
		//backup text start position.
		const int tlen = (int)fx - r->left;
		fx = (float)(r->left - (tlen>>1));
	}
	glBegin(GL_QUADS);
	for (k=0; k<slen; ++k) {
		ascii = text[k] - 32;
		lp = (float)(clen_a13[ascii]);
		tx[0] = tx_a13[ascii][0];
		tx[1] = tx_a13[ascii][1];
		ty[0] = ty_a13[ascii][0];
		ty[1] = ty_a13[ascii][1];
		glTexCoord2f(tx[0],ty[0]); glVertex2f(fx,fy);
		glTexCoord2f(tx[0],ty[1]); glVertex2f(fx,fy+hp);
		glTexCoord2f(tx[1],ty[1]); glVertex2f(fx+lp,fy+hp);
		glTexCoord2f(tx[1],ty[0]); glVertex2f(fx+lp,fy);
		fx += lp + 1.0f;
	}
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_ALPHA_TEST);
	r->bot -= advance;
}


void ggprint13nb(Rect *r, int advance, int cref, const char *fmt, ...)
{
	//do not bind the texture.
	//this is a bold 12 point font.
	int k, ascii;
	float tx[2],ty[2];
	float fx = (float)r->left;
	float fy = (float)r->bot;
	float lp;
	const float hp = 16.0f;  //max height of tallest symbol

	#ifdef STOP_ALL_FONT_WRITING
	return;
	#endif //STOP_ALL_FONT_WRITING
	char text[512];
	va_list ap;
	if (fmt == NULL)
		return;
	va_start(ap, fmt);
	vsprintf(text, fmt, ap);
	va_end(ap);
	//
	glColor4ub(GetBValue(cref),GetGValue(cref),GetRValue(cref), 255);
	const int slen = strlen(text);
	if (slen > 200)
		return;
	if (r->center) {
		for (k=0; k<slen; ++k) {
			ascii = text[k] - 32;
			fx += ((float)(clen_a13[ascii])) + 1.0f;
		}
		//backup text start position.
		const int tlen = (int)fx - r->left;
		fx = (float)(r->left - (tlen>>1));
	}
	glBegin(GL_QUADS);
	for (k=0; k<slen; ++k) {
		ascii = text[k] - 32;
		lp = (float)(clen_a13[ascii]);
		tx[0] = tx_a13[ascii][0];
		tx[1] = tx_a13[ascii][1];
		ty[0] = ty_a13[ascii][0];
		ty[1] = ty_a13[ascii][1];
		glTexCoord2f(tx[0],ty[0]); glVertex2f(fx,fy);
		glTexCoord2f(tx[0],ty[1]); glVertex2f(fx,fy+hp);
		glTexCoord2f(tx[1],ty[1]); glVertex2f(fx+lp,fy+hp);
		glTexCoord2f(tx[1],ty[0]); glVertex2f(fx+lp,fy);
		fx += lp + 1.0f;
	}
	glEnd();
	r->bot -= advance;
}

void ggprint12(Rect *r, int advance, int cref, const char *fmt, ...)
{
	int k, ascii;
	float tx[2],ty[2];
	float fx = (float)r->left;
	float fy = (float)r->bot;
	float lp;
	const float hp = 16.0f;  //max height of tallest symbol

	#ifdef STOP_ALL_FONT_WRITING
	return;
	#endif //STOP_ALL_FONT_WRITING
	char text[512];
	va_list ap;
	if (fmt == NULL)
		return;
	va_start(ap, fmt);
	vsprintf(text, fmt, ap);
	va_end(ap);
	//
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(GetBValue(cref),GetGValue(cref),GetRValue(cref), 255);
	glBindTexture(GL_TEXTURE_2D, a12_texture_no);
	const int slen = strlen(text);
	//
	//old code
	//glBegin(GL_QUADS);
	//for (k=0; k<slen; ++k) {
	//ascii = text[k] - 32;
	//lp = (float)(clen_a12[ascii]);
	//tx[0] = tx_a12[ascii][0];
	//tx[1] = tx_a12[ascii][1];
	//ty[0] = ty_a12[ascii][0];
	//ty[1] = ty_a12[ascii][1];
	//glTexCoord2f(tx[0],ty[0]);
	//glVertex2f(fx,fy);
	//glTexCoord2f(tx[0],ty[1]);
	//glVertex2f(fx,fy+hp);
	//glTexCoord2f(tx[1],ty[1]);
	//glVertex2f(fx+lp,fy+hp);
	//glTexCoord2f(tx[1],ty[0]);
	//glVertex2f(fx+lp,fy);
	//fx += lp + 1.0f;
	//}
	//
	if (r->center) {
		for (k=0; k<slen; ++k) {
			ascii = text[k] - 32;
			fx += ((float)(clen_a12[ascii])) + 1.0f;
		}
		//backup text start position.
		const int tlen = (int)fx - r->left;
		fx = (float)(r->left - (tlen>>1));
	}
	glBegin(GL_QUADS);
	for (k=0; k<slen; ++k) {
		ascii = text[k] - 32;
		lp = (float)(clen_a12[ascii]);
		tx[0] = tx_a12[ascii][0];
		tx[1] = tx_a12[ascii][1];
		ty[0] = ty_a12[ascii][0];
		ty[1] = ty_a12[ascii][1];
		glTexCoord2f(tx[0],ty[0]); glVertex2f(fx,fy);
		glTexCoord2f(tx[0],ty[1]); glVertex2f(fx,fy+hp);
		glTexCoord2f(tx[1],ty[1]); glVertex2f(fx+lp,fy+hp);
		glTexCoord2f(tx[1],ty[0]); glVertex2f(fx+lp,fy);
		fx += lp + 1.0f;
	}
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_ALPHA_TEST);
	r->bot -= advance;
}


void ggprint16(Rect *r, int advance, int cref, const char *fmt, ...)
{
	int k, ascii;
	float tx[2],ty[2];
	float fx = (float)(r->left);
	float fy = (float)(r->bot);
	float lp;
	const float hp = 20.0f;  //max height of tallest symbol

	#ifdef STOP_ALL_FONT_WRITING
	return;
	#endif //STOP_ALL_FONT_WRITING
	char text[512];
	va_list ap;
	if (fmt == NULL)
		return;
	va_start(ap, fmt);
	vsprintf(text, fmt, ap);
	va_end(ap);
	//
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(GetBValue(cref),GetGValue(cref),GetRValue(cref), 255);
	glBindTexture(GL_TEXTURE_2D, a16_texture_no);
	const int slen = strlen(text);
	if (r->center) {
		for (k=0; k<slen; ++k) {
			ascii = text[k] - 32;
			fx += ((float)(clen_a16[ascii])) + 1.0f;
		}
		//backup text start position.
		const int tlen = (int)fx - r->left;
		fx = (float)(r->left - (tlen>>1));
	}
	glBegin(GL_QUADS);
	for (k=0; k<slen; ++k) {
		ascii = text[k] - 32;
		lp = (float)(clen_a16[ascii]);
		tx[0] = tx_a16[ascii][0];
		tx[1] = tx_a16[ascii][1];
		ty[0] = ty_a16[ascii][0];
		ty[1] = ty_a16[ascii][1];
		glTexCoord2f(tx[0],ty[0]); glVertex2f(fx,fy);
		glTexCoord2f(tx[0],ty[1]); glVertex2f(fx,fy+hp);
		glTexCoord2f(tx[1],ty[1]); glVertex2f(fx+lp,fy+hp);
		glTexCoord2f(tx[1],ty[0]); glVertex2f(fx+lp,fy);
		fx += lp + 1.0f;
	}
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_ALPHA_TEST);
	r->bot -= advance;
}


void ggprint17(Rect *r, int advance, int cref, const char *fmt, ...)
{
	int k, ascii;
	float tx[2],ty[2];
	float fx = (float)(r->left);
	float fy = (float)(r->bot);
	float lp;
	//max height of tallest symbol
	//3 from bottom and 5 from top
	const float hp = 24.0f;

	#ifdef STOP_ALL_FONT_WRITING
	return;
	#endif //STOP_ALL_FONT_WRITING
	char text[512];
	va_list ap;
	if (fmt == NULL)
		return;
	va_start(ap, fmt);
	vsprintf(text, fmt, ap);
	va_end(ap);
	//
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(GetBValue(cref),GetGValue(cref),GetRValue(cref), 255);
	glBindTexture(GL_TEXTURE_2D, c17_texture_no);
	const int slen = strlen(text);
	if (r->center) {
		for (k=0; k<slen; ++k) {
			ascii = text[k] - 32;
			fx += ((float)(clen_c17[ascii])) + 1.0f;
		}
		//backup text start position.
		const int tlen = (int)fx - r->left;
		fx = (float)(r->left - (tlen>>1));
	}
	glBegin(GL_QUADS);
	for (k=0; k<slen; ++k) {
		ascii = text[k] - 32;
		lp = (float)(clen_c17[ascii]);
		tx[0] = tx_c17[ascii][0];
		tx[1] = tx_c17[ascii][1];
		ty[0] = ty_c17[ascii][0];
		ty[1] = ty_c17[ascii][1];
		glTexCoord2f(tx[0],ty[0]); glVertex2f(fx,fy);
		glTexCoord2f(tx[0],ty[1]); glVertex2f(fx,fy+hp);
		glTexCoord2f(tx[1],ty[1]); glVertex2f(fx+lp,fy+hp);
		glTexCoord2f(tx[1],ty[0]); glVertex2f(fx+lp,fy);
		fx += lp + 1.0f;
	}
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_ALPHA_TEST);
	r->bot -= advance;
}

void ggprint40(Rect *r, int advance, int cref, const char *fmt, ...)
{
	int k, ascii;
	float tx[2],ty[2];
	float fx = (float)r->left;
	float fy = (float)r->bot;
	float lp;
  	//height of the @ symbol is 46 (span of the largest symbol)
	float hp = 46.0f;

	#ifdef STOP_ALL_FONT_WRITING
	return;
	#endif //STOP_ALL_FONT_WRITING
	char text[512];
	va_list ap;
	if (fmt == NULL)
		return;
	va_start(ap, fmt);
	vsprintf(text, fmt, ap);
	va_end(ap);
	//
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(GetBValue(cref),GetGValue(cref),GetRValue(cref), 255);
	glBindTexture(GL_TEXTURE_2D, a40_texture_no);
	const int slen = strlen(text);
	if (r->center) {
		for (k=0; k<slen; ++k) {
			ascii = text[k] - 32;
			fx += ((float)(clen_a40[ascii])) + 1.0f;
		}
		//backup text start position.
		const int tlen = (int)fx - r->left;
		fx = (float)(r->left - (tlen>>1));
	}
	glBegin(GL_QUADS);
	for (k=0; k<slen; ++k) {
		ascii = text[k] - 32;
		lp = (float)(clen_a40[ascii]);
		tx[0] = tx_a40[ascii][0];
		tx[1] = tx_a40[ascii][1];
		ty[0] = ty_a40[ascii][0];
		ty[1] = ty_a40[ascii][1];
		glTexCoord2f(tx[0],ty[0]); glVertex2f(fx,fy);
		glTexCoord2f(tx[0],ty[1]); glVertex2f(fx,fy+hp);
		glTexCoord2f(tx[1],ty[1]); glVertex2f(fx+lp,fy+hp);
		glTexCoord2f(tx[1],ty[0]); glVertex2f(fx+lp,fy);
		fx += lp + 1.0f;
	}
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_ALPHA_TEST);
	r->bot -= advance;
}


void writeMessage()
{
	#ifdef SHOW_COMPRESSED_PRINTOUT
	Log("//Compressed font texture files.\n");
	Log("//white=1 yellow=2 black=0 followed by run\n");
	#endif //SHOW_COMPRESSED_PRINTOUT
}


void initialize_fonts(void)
{
	writeMessage();
	load_ggfont(6);
	load_ggfont(7);
	load_ggfont(8);
	load_ggfont(108); //8-bold
	load_ggfont(10);
	load_ggfont(12);
	load_ggfont(13);
	load_ggfont(16);
	load_ggfont(17);
	load_ggfont(40);
}


void get_compressed_tm(Texmap *tm, unsigned char *ptr1)
{
	//build a tm structure from the saved compressed data.
	//Compressed data:
	//  black pixels = 0 followed by run length
	//  white pixels = 255
	//  yellow pixels = 254
	//
	unsigned char *ptr = tm->c;
	//
	//printf("get_compressed_tm()...\n");
	int j,k;
	while (1) {
		if (*ptr1 == 0) {
			ptr1++;
			k = (int)(*ptr1);
			for (j=0; j<k; j++) {
 				*ptr     =
				*(ptr+1) =
				*(ptr+2) =
				*(ptr+3) = 0;
				ptr+=4;
			}
			ptr1++;
			continue;
		}
		if (*ptr1 == 1) {
			*ptr     =
			*(ptr+1) =
			*(ptr+2) =
			*(ptr+3) = 255;
			ptr += 4;
			ptr1++;
			continue;
		}
		if (*ptr1 == 2) {
			*ptr     = 255;
			*(ptr+1) = 255;
			*(ptr+2) = 0;
			*(ptr+3) = 255;
			ptr += 4;
			ptr1++;
			continue;
		}
		if (*ptr1 == 99) {
			//printf("found 99\n");
			break;
		}
		//else {
		//printf("ERROR - un-handled character!!! %i\n", (int)(*ptr1));
		break;
	}
}

void build_gl_texmap(Texmap *tm, unsigned int *texnum)
{
	//printf("build_gl_texmap()...\n");
	glBindTexture(GL_TEXTURE_2D, 0);
	glGenTextures(1, texnum);
	glBindTexture(GL_TEXTURE_2D, *texnum);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
	             tm->xres, tm->yres, 0, GL_RGBA, GL_UNSIGNED_BYTE, tm->c);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void load_ggfont(int psize)
{
	int i;
	int n_characters;
	int xpos;
	int cstart[128];
	int   clen[128];
	float   tx[128][2];
	float   ty[128][2];
	float ftmx,ftmy;
	float ftop,fbot;
	Texmap tm;
	tm.c = NULL;

	//printf("load_ggfont(%i)...\n",psize);
	//Load the bitmap into an Opengl texture map.
	switch (psize) {
		case 6:
		{
			//MS San Serif font.
			tm.xres = 512;
			tm.yres = 16;
			tm.c = (unsigned char *)malloc(tm.xres * tm.yres * 4);
			#ifdef READ_FROM_COMPRESSED
				get_compressed_tm(&tm, ams6ppm);
			#else //READ_FROM_COMPRESSED
				tempbmp = (unsigned char *)malloc(tm.xres * tm.yres * 4);
				tempptr = tempbmp;
				//if (read_tex("ams6.bmp"))
				if (read_texture_from_ppm("ams6.ppm"))
					printf("ERROR\n");
				get_compressed_tm(&tm, tempbmp);
				free(tempbmp);
			#endif //READ_FROM_COMPRESSED
			build_gl_texmap(&tm, &a06_texture_no);
			fbot = 2.0f;
			ftop = 12.0f;
			break;
		}
		case 7:
		{
			//Verdana font
			tm.xres = 512;
			tm.yres = 16;
			tm.c = (unsigned char *)malloc(tm.xres * tm.yres * 4);
			#ifdef READ_FROM_COMPRESSED
				get_compressed_tm(&tm, av7ppm);
			#else //READ_FROM_COMPRESSED
				tempbmp = (unsigned char *)malloc(tm.xres * tm.yres * 4);
				tempptr = tempbmp;
				//if (read_tex("av7.bmp"))
				if (read_texture_from_ppm("av7.ppm"))
					printf("ERROR\n");
				get_compressed_tm(&tm, tempbmp);
				free(tempbmp);
			#endif //READ_FROM_COMPRESSED
			build_gl_texmap(&tm, &a07_texture_no);
			fbot = 2.0f;
			ftop = 12.0f;
			break;
		}
		case 8:
		{
			tm.xres = 512;
			tm.yres = 16;
			tm.c = (unsigned char *)malloc(tm.xres * tm.yres * 4);
			#ifdef READ_FROM_COMPRESSED
				get_compressed_tm(&tm, arial8ppm);
			#else //READ_FROM_COMPRESSED
				tempbmp = (unsigned char *)malloc(tm.xres * tm.yres * 4);
				tempptr = tempbmp;
				//if (read_tex("arial8.bmp"))
				if (read_texture_from_ppm("arial8.ppm"))
					printf("ERROR\n");
				get_compressed_tm(&tm, tempbmp);
				free(tempbmp);
			#endif //READ_FROM_COMPRESSED
			build_gl_texmap(&tm, &a08_texture_no);
			fbot = 2.0f;
			ftop = 12.0f;
			break;
		}
		case 108:
		{
			//MS San Serif font.
			tm.xres = 512;
			tm.yres = 16;
			tm.c = (unsigned char *)malloc(tm.xres * tm.yres * 4);
			#ifdef READ_FROM_COMPRESSED
				get_compressed_tm(&tm, arial8bppm);
			#else //READ_FROM_COMPRESSED
				tempbmp = (unsigned char *)malloc(tm.xres * tm.yres * 4);
				tempptr = tempbmp;
				//if (read_tex("arial8b.bmp"))
				if (read_texture_from_ppm("arial8b.ppm"))
					printf("ERROR\n");
				get_compressed_tm(&tm, tempbmp);
				free(tempbmp);
			#endif //READ_FROM_COMPRESSED
			build_gl_texmap(&tm, &a8b_texture_no);
			fbot = 2.0f;
			ftop = 12.0f;
			break;
		}
		case 10:
		{
			tm.xres = 512;
			tm.yres = 16;
			tm.c = (unsigned char *)malloc(tm.xres * tm.yres * 4);
			#ifdef READ_FROM_COMPRESSED
				get_compressed_tm(&tm, arial10ppm);
			#else //READ_FROM_COMPRESSED
				tempbmp = (unsigned char *)malloc(tm.xres * tm.yres * 4);
				tempptr = tempbmp;
				//if (read_tex("arial10.bmp"))
				if (read_texture_from_ppm("arial10.ppm"))
					printf("ERROR\n");
				get_compressed_tm(&tm, tempbmp);
				free(tempbmp);
			#endif //READ_FROM_COMPRESSED
			build_gl_texmap(&tm, &a10_texture_no);
			fbot = (float)(16 - 16);
			ftop = (float)(16 - 3);
			break;
		}
		case 12:
		{
			tm.xres = 1024;
			tm.yres = 32;
			tm.c = (unsigned char *)malloc(tm.xres * tm.yres * 4);
			#ifdef READ_FROM_COMPRESSED
				get_compressed_tm(&tm, arial12ppm);
			#else //READ_FROM_COMPRESSED
				tempbmp = (unsigned char *)malloc(tm.xres * tm.yres * 4);
				tempptr = tempbmp;
				//if (read_tex("arial12.bmp"))
				if (read_texture_from_ppm("arial12.ppm"))
					printf("ERROR\n");
				get_compressed_tm(&tm, tempbmp);
				free(tempbmp);
			#endif //READ_FROM_COMPRESSED
			build_gl_texmap(&tm, &a12_texture_no);
			fbot = (float)(32 - 20);
			ftop = (float)(32 - 4);
			break;
		}
		case 13:
		{
			tm.xres = 1024;
			tm.yres = 32;
			tm.c = (unsigned char *)malloc(tm.xres * tm.yres * 4);
			#ifdef READ_FROM_COMPRESSED
				get_compressed_tm(&tm, arial13ppm);
			#else //READ_FROM_COMPRESSED
				tempbmp = (unsigned char *)malloc(tm.xres * tm.yres * 4);
				tempptr = tempbmp;
				//if (read_tex("arial13.bmp"))
				if (read_texture_from_ppm("arial13.ppm"))
					printf("ERROR\n");
				get_compressed_tm(&tm, tempbmp);
				free(tempbmp);
			#endif //READ_FROM_COMPRESSED
			build_gl_texmap(&tm, &a13_texture_no);
			fbot = (float)(32 - 20);
			ftop = (float)(32 - 4);
			break;
		}
		case 16:
		{
			tm.xres = 1024;
			tm.yres = 32;
			tm.c = (unsigned char *)malloc(tm.xres * tm.yres * 4);
			#ifdef READ_FROM_COMPRESSED
				get_compressed_tm(&tm, arial16ppm);
			#else //READ_FROM_COMPRESSED
				tempbmp = (unsigned char *)malloc(tm.xres * tm.yres * 4);
				tempptr = tempbmp;
				//if (read_tex("arial16.bmp"))
				if (read_texture_from_ppm("arial16.ppm"))
					printf("ERROR\n");
				get_compressed_tm(&tm, tempbmp);
				free(tempbmp);
			#endif //READ_FROM_COMPRESSED
			build_gl_texmap(&tm, &a16_texture_no);
			fbot = (float)(32 - 24);
			ftop = (float)(32 - 4);
			break;
		}
		case 17:
		{
			tm.xres = 2048;
			tm.yres = 32;
			tm.c = (unsigned char *)malloc(tm.xres * tm.yres * 4);
			#ifdef READ_FROM_COMPRESSED
				get_compressed_tm(&tm, courier16ppm);
			#else //READ_FROM_COMPRESSED
				tempbmp = (unsigned char *)malloc(tm.xres * tm.yres * 4);
				tempptr = tempbmp;
				if (read_texture_from_ppm("courier16.ppm"))
					printf("ERROR reading ppm.\n");
				get_compressed_tm(&tm, tempbmp);
				free(tempbmp);
			#endif //READ_FROM_COMPRESSED
			build_gl_texmap(&tm, &c17_texture_no);
			fbot = (float)(32 - 29);
			ftop = (float)(32 - 5);
			break;
		}
		case 40:
		{
			tm.xres = 2048;
			tm.yres = 64;
			tm.c = (unsigned char *)malloc(tm.xres * tm.yres * 4);
			#ifdef READ_FROM_COMPRESSED
				get_compressed_tm(&tm, arial40ppm);
			#else  //READ_FROM_COMPRESSED
				tempbmp = (unsigned char *)malloc(tm.xres * tm.yres * 4);
				tempptr = tempbmp;
				//if (read_tex("arial40.bmp"))
				if (read_texture_from_ppm("arial40.ppm"))
					printf("ERROR\n");
				get_compressed_tm(&tm, tempbmp);
				free(tempbmp);
			#endif //READ_FROM_COMPRESSED
			build_gl_texmap(&tm, &a40_texture_no);
			fbot = 14.0f;
			ftop = 60.0f;
			break;
		}
		default:
		{
			printf("ERROR - wrong point size submitted.\n");
			return;
		}
	}
	//
	ftmx = 1.0f / (float)tm.xres;
	ftmy = 1.0f / (float)tm.yres;
	//
	//Look through the image pixels to find the start and length of each
	//font character. The information is in the first 2 rows.
	//
	//white marks the start of each character. (row 1)
	//yellow marks the end of each character. (row 0)
	//text starts in row 4.
	//
	xpos = 0;
	n_characters = 0;
	//
	while (xpos < tm.xres) {
		if (find_next_white(&tm, &xpos))
			break;
		cstart[n_characters] = xpos;
		if (find_next_yellow(&tm, &xpos))
			break;
		clen[n_characters] = xpos - cstart[n_characters] + 1;
		//
		tx[n_characters][0] = ((float)cstart[n_characters] * ftmx);
		tx[n_characters][1] = ((float)cstart[n_characters] * ftmx) +
		                      ((float)(clen[n_characters]) * ftmx);
		ty[n_characters][0] = fbot * ftmy;
		ty[n_characters][1] = ftop * ftmy;
		//
		n_characters++;
		xpos++;
		//if (xpos >= tm.xres) break;
	}
	//
	//old code
	//#ifdef READ_FROM_COMPRESSED
	//if (v7) {
	//	free(v7);
	//	tm.c = NULL;
	//}
	//#else  READ_FROM_COMPRESSED
	//#endif READ_FROM_COMPRESSED
	//printf("n_characters: %i\n", n_characters);
	//replaces switch below.
	//int *cs2 = cstart_a40;
	//int *cl2 = clen_a40;
	//float *tx2 = (float *)tx_a40;
	//float *ty2 = (float *)ty_a40;
	//for (i=0; i<n_characters; i++) {
	//	cs2[i] = cstart[i];
	//	cl2[i] = clen[i];
	//	*(tx2+(i*2+0)) = tx[i][0];
	//	*(tx2+(i*2+1)) = tx[i][1];
	//	*(ty2+(i*2+0)) = ty[i][0];
	//	*(ty2+(i*2+1)) = ty[i][1];
	//}
	//
	switch (psize) {
		case 108:
		{
			for (i=0; i<n_characters; i++) {
				cstart_a8b[i] = cstart[i];
				clen_a8b[i] = clen[i];
				tx_a8b[i][0] = tx[i][0];
				tx_a8b[i][1] = tx[i][1];
				ty_a8b[i][0] = ty[i][0];
				ty_a8b[i][1] = ty[i][1];
			}
			break;
		}
		case 6:
		{
			for (i=0; i<n_characters; i++) {
				cstart_a06[i] = cstart[i];
				clen_a06[i] = clen[i];
				tx_a06[i][0] = tx[i][0];
				tx_a06[i][1] = tx[i][1];
				ty_a06[i][0] = ty[i][0];
				ty_a06[i][1] = ty[i][1];
			}
			break;
		}
		case 7:
		{
			for (i=0; i<n_characters; i++) {
				cstart_a07[i] = cstart[i];
				clen_a07[i] = clen[i];
				tx_a07[i][0] = tx[i][0];
				tx_a07[i][1] = tx[i][1];
				ty_a07[i][0] = ty[i][0];
				ty_a07[i][1] = ty[i][1];
			}
			break;
		}
		case 8:
		{
			for (i=0; i<n_characters; i++) {
				cstart_a08[i] = cstart[i];
				clen_a08[i] = clen[i];
				tx_a08[i][0] = tx[i][0];
				tx_a08[i][1] = tx[i][1];
				ty_a08[i][0] = ty[i][0];
				ty_a08[i][1] = ty[i][1];
			}
			break;
		}
		case 10:
		{
			for (i=0; i<n_characters; i++) {
				cstart_a10[i] = cstart[i];
				clen_a10[i] = clen[i];
				tx_a10[i][0] = tx[i][0];
				tx_a10[i][1] = tx[i][1];
				ty_a10[i][0] = ty[i][0];
				ty_a10[i][1] = ty[i][1];
			}
			break;
		}
		case 13:
		{
			for (i=0; i<n_characters; i++) {
				cstart_a13[i] = cstart[i];
				clen_a13[i] = clen[i];
				tx_a13[i][0] = tx[i][0];
				tx_a13[i][1] = tx[i][1];
				ty_a13[i][0] = ty[i][0];
				ty_a13[i][1] = ty[i][1];
			}
			break;
		}
		case 12:
		{
			for (i=0; i<n_characters; i++) {
				cstart_a12[i] = cstart[i];
				clen_a12[i] = clen[i];
				tx_a12[i][0] = tx[i][0];
				tx_a12[i][1] = tx[i][1];
				ty_a12[i][0] = ty[i][0];
				ty_a12[i][1] = ty[i][1];
			}
			break;
		}
		case 16:
		{
			for (i=0; i<n_characters; i++) {
				cstart_a16[i] = cstart[i];
				clen_a16[i] = clen[i];
				tx_a16[i][0] = tx[i][0];
				tx_a16[i][1] = tx[i][1];
				ty_a16[i][0] = ty[i][0];
				ty_a16[i][1] = ty[i][1];
			}
			break;
		}
		case 17:
		{
			for (i=0; i<n_characters; i++) {
				cstart_c17[i] = cstart[i];
				clen_c17[i] = clen[i];
				tx_c17[i][0] = tx[i][0];
				tx_c17[i][1] = tx[i][1];
				ty_c17[i][0] = ty[i][0];
				ty_c17[i][1] = ty[i][1];
			}
			break;
		}
		case 40:
		{
			for (i=0; i<n_characters; i++) {
				cstart_a40[i] = cstart[i];
				clen_a40[i] = clen[i];
				tx_a40[i][0] = tx[i][0];
				tx_a40[i][1] = tx[i][1];
				ty_a40[i][0] = ty[i][0];
				ty_a40[i][1] = ty[i][1];
			}
			break;
		}
	}
	//
	if (tm.c)
		free(tm.c);
}



int find_next_white(Texmap *tm, int *xpos)
{
	//return 1 if found, else 0.
	int sz34 = 4;
	int cref;
	unsigned char r, g, b;
	int xp;
	int xr  = tm->xres;
	int xr3 = (tm->yres - 2) * xr * sz34;
	while (1) {
		//move right until a color white is found (in row 1).
		xp = (*xpos) * sz34;
		r = tm->c[xr3+xp  ];
		g = tm->c[xr3+xp+1];
		b = tm->c[xr3+xp+2];
		cref = RGB(r,g,b);
		//printf("test for white %x\n", cref);
		if (cref == 0x00ffffff)
			break;
		*xpos += 1;
		if (*xpos >= xr)
			return 1;
	}
	return 0;
}

int find_next_yellow(Texmap *tm, int *xpos)
{
	//return 1 if found, else 0.
	int sz34 = 4;
	int cref;
	unsigned char r, g, b;
	int xp;
	int xr  = tm->xres;
	int xr3 = (tm->yres - 1) * xr * sz34;
	while (1) {
		//move right until a color yellow is found (in row 0).
		xp = (*xpos) * sz34;
		r = tm->c[xr3+xp  ];
		g = tm->c[xr3+xp+1];
		b = tm->c[xr3+xp+2];
		cref = RGB(r,g,b);
		//printf("test for yellow %x\n", cref);
		if (cref == 0x0000ffff)
			break;
		*xpos += 1;
		if (*xpos >= xr)
			return 1;
	}
	return 0;
}

int read_texture_from_ppm(const char *fileName)
{
	//This is working.
	//return 1 if error, 0 if good.
	int i,j,w,h;
	Texmap tm;
	printf("read_texture_from_ppm(%s)...\n", fileName);
	Ppmimage *image = ppm6GetImage_local(fileName);
	w = image->width;
	h = image->height;
	printf("wh: %i %i\n",w,h);
	tm.c = (unsigned char *)malloc(sizeof(char) * h * w * 4);
	//
	//Colors are RGB
	//The file must be processed bottom-up.
	//
	//  line 6      xxxxxxxxxxxxxxxxxxxxxxxxx
	//  line 5      xxxxxxxxxxxxxxxxxxxxxxxxx
	//  line 4      xxxxxxxxxxxxxxxxxxxxxxxxx
	//  line 3      xxxxxxxxxxxxxxxxxxxxxxxxx
	//  line 2      xxxxxxxxxxxxxxxxxxxxxxxxx
	//  line 1      xxxxxxxxxxxxxxxxxxxxxxxxx
	//
	int c1,c2,c3,c4;
	int lcount=0, zcount=0, wcount=0, ccount=0;
	unsigned char *ptr = tm.c;
	//printf("24-bit bitmap present.
	//bytesPerLine: %i  w: %i\n",bytesPerLine,w);
	#ifdef SHOW_COMPRESSED_PRINTOUT
			//A font bitmap has only 3 colors black, white, and yellow.
			//black=0    (0,0,0)
			//white=1    (255,255,255)
			//yellow=2   (0,255,255)
			//Log("//Compressed font texture files.\n");
			//Log("//white=1 yellow=2 black=0 followed by run\n");
			char ts[256];
			//remove the . in the file extension.
			strcpy(ts, fileName);
			int slen = strlen(ts);
			for (i=slen-1; i>0; i--) {
				if (ts[i]=='.') {
					ts[i+0]=ts[i+1];
					ts[i+1]=ts[i+2];
					ts[i+2]=ts[i+3];
					ts[i+3]='\0';
					break;
				}
			}
			Log("\nunsigned char %s[]={\n", ts);
	#endif //SHOW_COMPRESSED_PRINTOUT
	unsigned char *p = (unsigned char *)image->data;
	for (j=h-1; j>=0; j--) {
		p = (unsigned char *)image->data + (j*w*3);
		for (i=0; i<w; i++) {
			//RGB
			c1 = *(p+0);
			c2 = *(p+1);
			c3 = *(p+2);
			p += 3;
			*(ptr+0) = c1;
			*(ptr+1) = c2;
			*(ptr+2) = c3;
			c4 = (c1+c2+c3 == 0) ? 0 : 255;
			*(ptr+3) = (unsigned char)c4;
			ptr += 4;
			//----------------------
			if (c4==0) {
				//value is zero.
				zcount++;
				if (zcount == 1) {
					#ifdef SHOW_COMPRESSED_PRINTOUT
					Log("0,"); lcount++; ccount+=2;
					#endif //SHOW_COMPRESSED_PRINTOUT
					*tempptr=0; tempptr++;
				} else {
					if (zcount == 250) {
						//250 is the max run-length of black color
						#ifdef SHOW_COMPRESSED_PRINTOUT
						Log("250,"); lcount++; ccount+=4;
						#endif //SHOW_COMPRESSED_PRINTOUT
						*tempptr=250; tempptr++;
						zcount=0;
					}
				}
			} else {
				//Not black pixel.
				if (zcount) {
					//Write run-lenth of black.
					#ifdef SHOW_COMPRESSED_PRINTOUT
					sprintf(ts,"%i,",zcount);
					Log("%s",ts); lcount++; ccount+=strlen(ts);
					#endif //SHOW_COMPRESSED_PRINTOUT
					*tempptr=zcount; tempptr++;
					zcount=0;
				}
				if (c1==255 && c2==255) {
					//white=1 yellow=2
					if (c3==0) {
						//yellow pixel.
						#ifdef SHOW_COMPRESSED_PRINTOUT
						Log("2,"); lcount++; ccount+=2;
						#endif //SHOW_COMPRESSED_PRINTOUT
						*tempptr=2; tempptr++;
					} else {
					//if (c3==255) {
						//white pixel.
						#ifdef SHOW_COMPRESSED_PRINTOUT
						Log("1,"); lcount++; ccount+=2;
						#endif //SHOW_COMPRESSED_PRINTOUT
						*tempptr=1; tempptr++;
					}
				}
			}
			if (ccount > 72) {
				#ifdef SHOW_COMPRESSED_PRINTOUT
				Log("\n");
				#endif //SHOW_COMPRESSED_PRINTOUT
				wcount += lcount;
				lcount=0;
				ccount=0;
			}
		}
	}
	if (zcount) {
		#ifdef SHOW_COMPRESSED_PRINTOUT
		sprintf(ts,"%i,",zcount);
		Log("%s",ts); lcount++; ccount+=strlen(ts);
		#endif //SHOW_COMPRESSED_PRINTOUT
		*tempptr=zcount; tempptr++;
	}
	#ifdef SHOW_COMPRESSED_PRINTOUT
	Log("99};\n"); lcount++; ccount+=4;
	#endif //SHOW_COMPRESSED_PRINTOUT
	*tempptr=99; tempptr++;
	if (lcount)
		wcount += lcount;
	#ifdef SHOW_COMPRESSED_PRINTOUT
	Log("//array size: %i\n", wcount);
	#endif //SHOW_COMPRESSED_PRINTOUT

	#ifdef SHOW_COMPRESSED_PRINTOUT
	Log("\n");
	#endif //SHOW_COMPRESSED_PRINTOUT
	printf("setting wh: %i %i\n",w,h);
	tm.xres = w;
	tm.yres = h;
	ppm6CleanupImage_local(image);
	if (tm.c)
		free(tm.c);
	return 0;
}

void cleanup_fonts(void)
{
	glDeleteTextures(1, &a06_texture_no);
	glDeleteTextures(1, &a07_texture_no);
	glDeleteTextures(1, &a08_texture_no);
	glDeleteTextures(1, &a8b_texture_no);
	glDeleteTextures(1, &a10_texture_no);
	glDeleteTextures(1, &a12_texture_no);
	glDeleteTextures(1, &a13_texture_no);
	glDeleteTextures(1, &a16_texture_no);
	glDeleteTextures(1, &c17_texture_no);
	glDeleteTextures(1, &a40_texture_no);
}

