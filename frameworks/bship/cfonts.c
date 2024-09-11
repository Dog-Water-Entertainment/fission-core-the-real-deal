#include "defs.h"
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <malloc.h>
#include <stdarg.h>

#include "glfw.h"

#define RGB(r,g,b)     ( (int)r + (int)((int)g<<8) + (int)((int)b<<16) )
#define GetRValue(rgb) (unsigned char)( (unsigned int)rgb&0x000000ff )
#define GetGValue(rgb) (unsigned char)( (unsigned int)((unsigned int)rgb&0x0000ff00) >>  8)
#define GetBValue(rgb) (unsigned char)( (unsigned int)((unsigned int)rgb&0x00ff0000) >> 16)

#define COURIER

typedef struct t_Bmphead {
	char id1;		// 1     1
	char id2;		// 1     2
	long filesize;		// 4     6
	short reserved1;	// 2     8
	short reserved2;	// 2    10
	long headersize;	// 4    14
	long infoSize;		// 4    18
	long width;		// 4    22
	long depth;		// 4    26
	short biPlanes;		// 2    28
	short bits;		// 2    30
	long biCompression;	// 4    34
	long biSizeImage;	// 4    38
	long biXPelsPerMeter;	// 4    42
	long biYPelsPerMeter;	// 4    46
	long biClsUsed;		// 4    50
	long biClrImportant;	// 4    54
} Bmphead;

void GetBmphead(int fh, Bmphead *bmphead) {
	int br;
	br = read(fh, &bmphead->id1,1);
	br = read(fh, &bmphead->id2,1);
	br = read(fh, &bmphead->filesize,4);
	br = read(fh, &bmphead->reserved1,2);
	br = read(fh, &bmphead->reserved2,2);
	br = read(fh, &bmphead->headersize,4);
	br = read(fh, &bmphead->infoSize,4);
	br = read(fh, &bmphead->width,4);
	br = read(fh, &bmphead->depth,4);
	br = read(fh, &bmphead->biPlanes,2);
	br = read(fh, &bmphead->bits,2);
	br = read(fh, &bmphead->biCompression,4);
	br = read(fh, &bmphead->biSizeImage,4);
	br = read(fh, &bmphead->biXPelsPerMeter,4);
	br = read(fh, &bmphead->biYPelsPerMeter,4);
	br = read(fh, &bmphead->biClsUsed,4);
	br = read(fh, &bmphead->biClrImportant,4);
}

int fonts_are_initialized;
int n_fonts;
//int cstart_a40[128], clen_a40[128];//, top_a40[128], bot_a40[128];
#ifdef COURIER
int cstart_c17[128], clen_c17[128];//
#endif //COURIER
int cstart_a16[128], clen_a16[128];//, top_a16[128], bot_a16[128];
int cstart_a12[128], clen_a12[128];//, top_a12[128], bot_a12[128];
int cstart_a13[128], clen_a13[128];//, top_a13[128], bot_a13[128];
int cstart_a10[128], clen_a10[128];//, top_a10[128], bot_a10[128];
int cstart_a08[128], clen_a08[128];//, top_a08[128], bot_a08[128];
int cstart_a07[128], clen_a07[128];//, top_a07[128], bot_a07[128];
int cstart_a06[128], clen_a06[128];//, top_a06[128], bot_a06[128];
int cstart_a8b[128], clen_a8b[128];//, top_a06[128], bot_a06[128];
//float tx_a40[128][2], ty_a40[128][2];
#ifdef COURIER
float tx_c17[128][2], ty_c17[128][2];
#endif //COURIER
float tx_a16[128][2], ty_a16[128][2];
float tx_a12[128][2], ty_a12[128][2];
float tx_a13[128][2], ty_a13[128][2];
float tx_a10[128][2], ty_a10[128][2];
float tx_a08[128][2], ty_a08[128][2];
float tx_a07[128][2], ty_a07[128][2];
float tx_a06[128][2], ty_a06[128][2];
float tx_a8b[128][2], ty_a8b[128][2];
//
//unsigned int a40_texture_no;
#ifdef COURIER
unsigned int c17_texture_no;
#endif //COURIER
unsigned int a16_texture_no;
unsigned int a12_texture_no;
unsigned int a13_texture_no;
unsigned int a10_texture_no;
unsigned int a08_texture_no;
unsigned int a07_texture_no;
unsigned int a06_texture_no;
unsigned int a8b_texture_no;
//
void initialize_fonts(void);
void cleanup_fonts(void);
//void get_compressed_tm(int csize, int tmsize);
void get_compressed_tm(Texmap *tm, unsigned char *ptr1);
void build_gl_texmap(Texmap *tm, unsigned int *texnum);
void load_ggfont(int psize);
//void ggprint8b_sz(Rect *r, int advance, int cref, int sz, const char *fmt, ...);
void ggprint16_sz(Rect *r, int advance, int cref, float sz, const char *fmt, ...);
//void ggprint40(Rect *r, int advance, int cref, const char *fmt, ...);
#ifdef COURIER
void ggprint17(Rect *r, int advance, int cref, const char *fmt, ...);
#endif //COURIER
void ggprint16(Rect *r, int advance, int cref, const char *fmt, ...);
void ggprint12(Rect *r, int advance, int cref, const char *fmt, ...);
void ggprint13(Rect *r, int advance, int cref, const char *fmt, ...);
void ggprint10(Rect *r, int advance, int cref, const char *fmt, ...);
void ggprint08(Rect *r, int advance, int cref, const char *fmt, ...);
void ggprint07(Rect *r, int advance, int cref, const char *fmt, ...);
void ggprint06(Rect *r, int advance, int cref, const char *fmt, ...);
void ggprint8b(Rect *r, int advance, int cref, const char *fmt, ...);
int get_length(const char *fmt, ...);
int ggprint8b_chat(Rect *r, int advance, int cref, const char *fmt, ...);
int ggprint8b_wordwrap(Rect *r, int advance, int cref, const char *fmt, ...);
void ggprint13nb(Rect *r, int advance, int cref, const char *fmt, ...);
void ggprint06nb(Rect *r, int advance, int cref, const char *fmt, ...);
void ggprint8b_nb(Rect *r, int advance, int cref, const char *fmt, ...);
int find_next_white(Texmap *tm, int *xpos, int ypos);
int find_next_yellow(Texmap *tm, int *xpos, int ypos);
int tex_read_ggfont_bmp(char *fileName, Texmap *tm);
int read_tex(char *fileName, int texn);

//#define STOP_ALL_FONT_WRITING

// !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_'abcdefghijklmnopqrstuvwxyz{|}~
//int show_debug_printout=0;
void GetBmphead(int fh, Bmphead *bmphead);
extern unsigned char arial8bbmp[];
extern unsigned char v7bmp[];
extern unsigned char ms6bmp[];
extern unsigned char arial8bmp[];
extern unsigned char arial10bmp[];
extern unsigned char arial12bmp[];
extern unsigned char arial13bmp[];
extern unsigned char arial16bmp[];
extern unsigned char courier16bmp[];
//extern unsigned char arial40bmp[];

unsigned char *tempbmp;
unsigned char *tempptr;



int get_length(const char *fmt, ...)
{
	int k;

	char text[512];
	va_list ap;
	if (fmt == NULL) return 0;
	va_start(ap, fmt);
	vsprintf(text, fmt, ap);
	va_end(ap);
	//
	const int slen = strlen(text);
	int fx = 0;
	for (k=0; k<slen; ++k) { fx += ((clen_a8b[(text[k]-32)])) + 1; }
	return fx;
}

void ggprint16_sz(Rect *r, int advance, int cref, float sz, const char *fmt, ...)
{
	int k, ascii;
	float tx[2],ty[2];
	float fx = (float)(r->left);
	float fy = (float)(r->bot);
	float lp;
	const float hp = 46.0f;  //height of the @ symbol is 46 (span of the largest symbol)
	//const float hp = 20.0f;  //height of the @ symbol is 46 (span of the largest symbol)

	//printf("ggprint16_sz()...\n");

	#ifdef STOP_ALL_FONT_WRITING
	return;
	#endif //STOP_ALL_FONT_WRITING
	char text[512];
	va_list ap;
	if (fmt == NULL) return;
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
	if (fmt == NULL) return;
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
		glTexCoord2f(tx[0],ty[0]);
		glVertex2f(fx, fy);
		glTexCoord2f(tx[0],ty[1]);
		glVertex2f(fx, fy+hp);
		glTexCoord2f(tx[1],ty[1]);
		glVertex2f(fx+lp,fy+hp);
		glTexCoord2f(tx[1],ty[0]);
		glVertex2f(fx+lp,fy);
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
	if (fmt == NULL) return 0;
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
		if (text[k] == 32) lastspace = k;
		tfx += ((float)(clen_a8b[(text[k]-32)])) + 1.0f;
		if ( (int)tfx > rmargin) { bspot = k; break; }
	}
	glBegin(GL_QUADS);
	if ( bspot ) {
		//Text overruns the right margin, at bspot.
		//printf("Text overruns the right margin.  bspot: %i   lastspace: %i\n",bspot,lastspace);
		if (!lastspace) {
			//No spaces were found before the overrun.
			//User typed a long continuous string of characters.
			//Just print it, and look for any space following it.  Start next string there.
			for (k=0; k<bspot; ++k) {
				ascii = text[k] - 32;
				lp = (float)(clen_a8b[ascii]);
				tx[0] = tx_a8b[ascii][0];
				tx[1] = tx_a8b[ascii][1];
				ty[0] = ty_a8b[ascii][0];
				ty[1] = ty_a8b[ascii][1];
				glTexCoord2f(tx[0],ty[0]);
				glVertex2f(fx, fy);
				glTexCoord2f(tx[0],ty[1]);
				glVertex2f(fx, fy+hp);
				glTexCoord2f(tx[1],ty[1]);
				glVertex2f(fx+lp,fy+hp);
				glTexCoord2f(tx[1],ty[0]);
				glVertex2f(fx+lp,fy);
				fx += (lp + 1.0f);
			}
			//look for a space in the string before a null.
			int start=0;
			for (k=bspot; k<slen; ++k) {
				if (text[k] == 32) { start = k; break; }
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
					glTexCoord2f(tx[0],ty[0]);
					glVertex2f(fx, fy);
					glTexCoord2f(tx[0],ty[1]);
					glVertex2f(fx, fy+hp);
					glTexCoord2f(tx[1],ty[1]);
					glVertex2f(fx+lp,fy+hp);
					glTexCoord2f(tx[1],ty[0]);
					glVertex2f(fx+lp,fy);
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
				glTexCoord2f(tx[0],ty[0]);
				glVertex2f(fx, fy);
				glTexCoord2f(tx[0],ty[1]);
				glVertex2f(fx, fy+hp);
				glTexCoord2f(tx[1],ty[1]);
				glVertex2f(fx+lp,fy+hp);
				glTexCoord2f(tx[1],ty[0]);
				glVertex2f(fx+lp,fy);
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
				glTexCoord2f(tx[0],ty[0]);
				glVertex2f(fx, fy);
				glTexCoord2f(tx[0],ty[1]);
				glVertex2f(fx, fy+hp);
				glTexCoord2f(tx[1],ty[1]);
				glVertex2f(fx+lp,fy+hp);
				glTexCoord2f(tx[1],ty[0]);
				glVertex2f(fx+lp,fy);
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
			glTexCoord2f(tx[0],ty[0]);
			glVertex2f(fx, fy);
			glTexCoord2f(tx[0],ty[1]);
			glVertex2f(fx, fy+hp);
			glTexCoord2f(tx[1],ty[1]);
			glVertex2f(fx+lp,fy+hp);
			glTexCoord2f(tx[1],ty[0]);
			glVertex2f(fx+lp,fy);
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
	if (fmt == NULL) return 0;
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
		glTexCoord2f(tx[0],ty[0]);
		glVertex2f(fx, fy);
		glTexCoord2f(tx[0],ty[1]);
		glVertex2f(fx, fy+hp);
		glTexCoord2f(tx[1],ty[1]);
		glVertex2f(fx+lp,fy+hp);
		glTexCoord2f(tx[1],ty[0]);
		glVertex2f(fx+lp,fy);
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
	if (fmt == NULL) return;
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
		glTexCoord2f(tx[0],ty[0]);\
		glVertex2f(fx, fy);
		glTexCoord2f(tx[0],ty[1]);
		glVertex2f(fx, fy+hp);
		glTexCoord2f(tx[1],ty[1]);
		glVertex2f(fx+lp,fy+hp);
		glTexCoord2f(tx[1],ty[0]);
		glVertex2f(fx+lp,fy);
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
	if (fmt == NULL) return;
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
		glTexCoord2f(tx[0],ty[0]);
		glVertex2f(fx, fy);
		glTexCoord2f(tx[0],ty[1]);
		glVertex2f(fx, fy+hp);
		glTexCoord2f(tx[1],ty[1]);
		glVertex2f(fx+lp,fy+hp);
		glTexCoord2f(tx[1],ty[0]);
		glVertex2f(fx+lp,fy);
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
	if (fmt == NULL) return;
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
		glTexCoord2f(tx[0],ty[0]);
		glVertex2f(fx, fy);
		glTexCoord2f(tx[0],ty[1]);
		glVertex2f(fx, fy+hp);
		glTexCoord2f(tx[1],ty[1]);
		glVertex2f(fx+lp,fy+hp);
		glTexCoord2f(tx[1],ty[0]);
		glVertex2f(fx+lp,fy);
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
	if (fmt == NULL) return;
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
		glTexCoord2f(tx[0],ty[0]);
		glVertex2f(fx, fy);
		glTexCoord2f(tx[0],ty[1]);
		glVertex2f(fx, fy+hp);
		glTexCoord2f(tx[1],ty[1]);
		glVertex2f(fx+lp,fy+hp);
		glTexCoord2f(tx[1],ty[0]);
		glVertex2f(fx+lp,fy);
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
	if (fmt == NULL) return;
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
		glTexCoord2f(tx[0],ty[0]);
		glVertex2f(fx, fy);
		glTexCoord2f(tx[0],ty[1]);
		glVertex2f(fx, fy+hp);
		glTexCoord2f(tx[1],ty[1]);
		glVertex2f(fx+lp,fy+hp);
		glTexCoord2f(tx[1],ty[0]);
		glVertex2f(fx+lp,fy);
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
	if (fmt == NULL) return;
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
		glTexCoord2f(tx[0],ty[0]);
		glVertex2f(fx, fy);
		glTexCoord2f(tx[0],ty[1]);
		glVertex2f(fx, fy+hp);
		glTexCoord2f(tx[1],ty[1]);
		glVertex2f(fx+lp,fy+hp);
		glTexCoord2f(tx[1],ty[0]);
		glVertex2f(fx+lp,fy);
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
	if (fmt == NULL) return;
	va_start(ap, fmt);
	vsprintf(text, fmt, ap);
	va_end(ap);
	//
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(GetBValue(cref),GetGValue(cref),GetRValue(cref), 255);
	glBindTexture(GL_TEXTURE_2D, a13_texture_no);
	const int slen = strlen(text);
	if (slen > 200) return;
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
		glTexCoord2f(tx[0],ty[0]);
		glVertex2f(fx,fy);
		glTexCoord2f(tx[0],ty[1]);
		glVertex2f(fx,fy+hp);
		glTexCoord2f(tx[1],ty[1]);
		glVertex2f(fx+lp,fy+hp);
		glTexCoord2f(tx[1],ty[0]);
		glVertex2f(fx+lp,fy);
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
	if (fmt == NULL) return;
	va_start(ap, fmt);
	vsprintf(text, fmt, ap);
	va_end(ap);
	//
	glColor4ub(GetBValue(cref),GetGValue(cref),GetRValue(cref), 255);
	const int slen = strlen(text);
	if (slen > 200) return;
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
		glTexCoord2f(tx[0],ty[0]);
		glVertex2f(fx,fy);
		glTexCoord2f(tx[0],ty[1]);
		glVertex2f(fx,fy+hp);
		glTexCoord2f(tx[1],ty[1]);
		glVertex2f(fx+lp,fy+hp);
		glTexCoord2f(tx[1],ty[0]);
		glVertex2f(fx+lp,fy);
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
	if (fmt == NULL) return;
	va_start(ap, fmt);
	vsprintf(text, fmt, ap);
	va_end(ap);
	//
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(GetBValue(cref),GetGValue(cref),GetRValue(cref), 255);
	glBindTexture(GL_TEXTURE_2D, a12_texture_no);
	const int slen = strlen(text);
//	glBegin(GL_QUADS);
//	for (k=0; k<slen; ++k) {
//		ascii = text[k] - 32;
//		lp = (float)(clen_a12[ascii]);
//		tx[0] = tx_a12[ascii][0]; tx[1] = tx_a12[ascii][1]; ty[0] = ty_a12[ascii][0]; ty[1] = ty_a12[ascii][1];
//		glTexCoord2f(tx[0],ty[0]); glVertex2f(fx,fy);glTexCoord2f(tx[0],ty[1]); glVertex2f(fx,fy+hp);glTexCoord2f(tx[1],ty[1]); glVertex2f(fx+lp,fy+hp);glTexCoord2f(tx[1],ty[0]); glVertex2f(fx+lp,fy);
//		fx += lp + 1.0f;
//	}

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
		glTexCoord2f(tx[0],ty[0]);
		glVertex2f(fx,fy);
		glTexCoord2f(tx[0],ty[1]);
		glVertex2f(fx,fy+hp);
		glTexCoord2f(tx[1],ty[1]);
		glVertex2f(fx+lp,fy+hp);
		glTexCoord2f(tx[1],ty[0]);
		glVertex2f(fx+lp,fy);
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
	if (fmt == NULL) return;
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
		glTexCoord2f(tx[0],ty[0]);
		glVertex2f(fx,fy);
		glTexCoord2f(tx[0],ty[1]);
		glVertex2f(fx,fy+hp);
		glTexCoord2f(tx[1],ty[1]);
		glVertex2f(fx+lp,fy+hp);
		glTexCoord2f(tx[1],ty[0]);
		glVertex2f(fx+lp,fy);
		fx += lp + 1.0f;
	}
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_ALPHA_TEST);
	r->bot -= advance;
}


#ifdef COURIER
void ggprint17(Rect *r, int advance, int cref, const char *fmt, ...)
{
	int k, ascii;
	float tx[2],ty[2];
	float fx = (float)(r->left);
	float fy = (float)(r->bot);
	float lp;
	const float hp = 22.0f;  //max height of tallest symbol

	#ifdef STOP_ALL_FONT_WRITING
	return;
	#endif //STOP_ALL_FONT_WRITING
	char text[512];
	va_list ap;
	if (fmt == NULL) return;
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
		glTexCoord2f(tx[0],ty[0]);
		glVertex2f(fx,fy);
		glTexCoord2f(tx[0],ty[1]);
		glVertex2f(fx,fy+hp);
		glTexCoord2f(tx[1],ty[1]);
		glVertex2f(fx+lp,fy+hp);
		glTexCoord2f(tx[1],ty[0]);
		glVertex2f(fx+lp,fy);
		fx += lp + 1.0f;
	}
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_ALPHA_TEST);
	r->bot -= advance;
}
#endif //COURIER

//void ggFonts::ggprint40(Rect *r, int advance, int cref, const char *fmt, ...)
//{
//	int k, ascii;
//	float tx[2],ty[2];
//	float fx = (float)r->left;
//	float fy = (float)r->bot;
//	float lp;
//	float hp = 46.0f;  //height of the @ symbol is 46 (span of the largest symbol)
//
//	#ifdef STOP_ALL_FONT_WRITING
//	return;
//	#endif STOP_ALL_FONT_WRITING
//	char text[512];
//	va_list ap;
//	if (fmt == NULL) return;
//	va_start(ap, fmt);
//	vsprintf(text, fmt, ap);
//	va_end(ap);
//	//
//	//fprintf(fpxx,"ggarial_na40()...\n"); fflush(fpxx);
//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
//	glColor4ub(GetRValue(cref),GetGValue(cref),GetBValue(cref), 255);
//	glBindTexture(GL_TEXTURE_2D, a40_texture_no);
//	int slen = strlen(text);
//	for (k=0; k<slen; ++k) {
//		ascii = text[k] - 32;
//		lp = (float)(clen_a40[ascii]);
//		tx[0] = tx_a40[ascii][0]; tx[1] = tx_a40[ascii][1]; ty[0] = ty_a40[ascii][0]; ty[1] = ty_a40[ascii][1];
//		glBegin(GL_QUADS); glTexCoord2f(tx[0],ty[0]); glVertex2f(fx,fy);glTexCoord2f(tx[0],ty[1]); glVertex2f(fx,fy+hp);glTexCoord2f(tx[1],ty[1]); glVertex2f(fx+lp,fy+hp);glTexCoord2f(tx[1],ty[0]); glVertex2f(fx+lp,fy); glEnd();
//		fx += lp + 2.0f;
//	}
//	glBindTexture(GL_TEXTURE_2D, 0);
//	glDisable(GL_BLEND);
//	r->bot -= advance;
//}


void initialize_fonts(void)
{
	//load_ggfont(40);
	#ifdef COURIER
	load_ggfont(17);
	#endif //COURIER
	load_ggfont(12);
	load_ggfont(16);
	load_ggfont(13);
	load_ggfont(10);
	load_ggfont(8);
	load_ggfont(7);
	load_ggfont(6);
	load_ggfont(108); //8-bold
}


void get_compressed_tm(Texmap *tm, unsigned char *ptr1)
{
	unsigned char *ptr = tm->c;
	int tmsize = tm->xres*tm->yres;
	//
	//printf("ggFonts::get_compressed_tm()...\n");
	//printf("ptr: %c\n", *ptr);
	int j,k,n=0;
	int i=0;
	while (1) {
		if (*ptr1 == 0) {
			ptr1++;
			k = (int)( *ptr1 );
			//printf("%i blacks\n",k);
			//if (k==0) printf("ERROR - must be at least 1 zero!\n");
			//printf("looping %i times\n",k);
			for (j=0; j<k; j++) {
				//printf("ptr: %c\n", *ptr);
 				*ptr     = 0;
				*(ptr+1) = 0;
				*(ptr+2) = 0;
				*(ptr+3) = 0;
				ptr+=4;
				n++;
			}
			i+=2;
			ptr1++;
			continue;
		}
		//else {
		if (*ptr1 == 255) {
			//printf("white\n");
			*ptr     = 255;
			*(ptr+1) = 255;
			*(ptr+2) = 255;
			*(ptr+3) = 255;
			ptr += 4;
			i++;
			n++;
			ptr1++;
			continue;
		}
		//else {
		if (*ptr1 == 254) {
			//printf("yellow\n");
			*ptr     = 255;
			*(ptr+1) = 255;
			*(ptr+2) = 0;
			*(ptr+3) = 255;
			ptr += 4;
			i++;
			n++;
			ptr1++;
			continue;
		}
		if (*ptr1 == 99) {
			//printf("found 99\n");
			//end.
			break;
		}
		//else {
		//printf("ERROR - un-handled character!!! %i\n", (int)(*ptr1));
		break;
		//ptr1++;
		//printf("%i\n",i);
		//if (i>=csize) break;
	}
	//if (n != tmsize) {
	//	printf("ERROR - ggFonts::get_compressed_tm n:%i   should be %i   diff: %i\n",n,tmsize,n-tmsize);
	//}
}

void build_gl_texmap(Texmap *tm, unsigned int *texnum)
{
	//return;
	//printf("build_gl_texmap()...\n");
	glGenTextures(1, texnum);
	glBindTexture(GL_TEXTURE_2D, *texnum);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, tm->xres, tm->yres, 0, GL_RGBA, GL_UNSIGNED_BYTE, tm->c );
	//game.texture_mem += (tm->xres * tm->yres * 4);
	glBindTexture(GL_TEXTURE_2D, 0);
}


#define SHOW_COMPRESSED_PRINTOUT
#define READ_FROM_COMPRESSED

void load_ggfont(int psize)
{
	int i;
	int n_characters;
	int xpos, ypos, cspot;
	int cstart[128];
	int   clen[128];
	float   tx[128][2];
	float   ty[128][2];
	float ftmx,ftmy;
	float ftop,fbot;
	Texmap tm;
	tm.c = NULL;

	//printf("load_ggfont(%i)...\n",psize);
	//first thing, load the bitmap into an Opengl texture map.
	switch ( psize ) {
		#ifdef COURIER
		case 17:
		{
			#ifdef READ_FROM_COMPRESSED
				//tm.xres = 2048;
				tm.xres = 1360;
				tm.yres = 32;
				tm.c = (unsigned char *)malloc(tm.xres * tm.yres * 4);
				get_compressed_tm(&tm, courier16bmp);
				build_gl_texmap(&tm, &c17_texture_no);
			#else //READ_FROM_COMPRESSED
				//this works.
				//if you want to load from files, use this code structure below.
				int ret;
				char ts[32];
				tm.xres = 1360;
				tm.yres = 32;
				tm.c = (unsigned char *)malloc(tm.xres * tm.yres * 4);
				tempbmp = (unsigned char *)malloc(tm.xres * tm.yres);
				glGenTextures(1, &c17_texture_no);
				sprintf(ts,"courier16.bmp");
				tempptr = tempbmp;
				ret = read_tex(ts, c17_texture_no);
				get_compressed_tm(&tm, tempbmp);
				build_gl_texmap(&tm, &c17_texture_no);
				free(tempbmp);
			#endif //READ_FROM_COMPRESSED
			fbot = (float)(32 - 28);
			ftop = (float)(32 - 5);
			break;
		}
		#endif //COURIER
		case 108:
		{
			//MS San Serif font.
			#ifdef READ_FROM_COMPRESSED
				tm.xres = 512;
				tm.yres = 16;
				tm.c = (unsigned char *)malloc(tm.xres * tm.yres * 4);
				get_compressed_tm(&tm, arial8bbmp);
				build_gl_texmap(&tm, &a8b_texture_no);
			#else //READ_FROM_COMPRESSED
				glGenTextures(1, &a8b_texture_no);
				//int ret = read_tex("arial8b.bmp", a8b_texture_no);
				char ts[MYMAX_PATH];
				sprintf(ts,"%s\\tex\\arial8b.bmp",game.current_path);
				int ret = read_tex(ts, a8b_texture_no);
				if (!ret) { printf("ERROR - loading texmap arial8b.bmp\n"); return; }
				//also read the bitmap into a tm structure, for examination and parsing.
				ret = tex_read_ggfont_bmp(ts, &tm);
				if (!ret) { printf("ERROR - loading texmap arial8b.bmp\n"); return; }
			#endif //READ_FROM_COMPRESSED
			fbot = 2.0f;
			ftop = 12.0f;
			break;
		}
		case 6:
		{
			//MS San Serif font.
			#ifdef READ_FROM_COMPRESSED
				tm.xres = 512;
				tm.yres = 16;
				tm.c = (unsigned char *)malloc(tm.xres * tm.yres * 4);
				get_compressed_tm(&tm, ms6bmp);
				build_gl_texmap(&tm, &a06_texture_no);
			#else //READ_FROM_COMPRESSED
				glGenTextures(1, &a06_texture_no);
				char ts[MYMAX_PATH];
				sprintf(ts,"%s\\tex\\ms6.bmp",game.current_path);
				int ret = read_tex(ts, a06_texture_no);
				if (!ret) { printf("ERROR - loading texmap ms6.bmp\n"); return; }
				//also read the bitmap into a tm structure, for examination and parsing.
				ret = tex_read_ggfont_bmp(ts, &tm);
				if (!ret) { printf("ERROR - loading texmap ms6.bmp\n"); return; }
			#endif //READ_FROM_COMPRESSED
			fbot = 2.0f;
			ftop = 12.0f;
			break;
		}
		case 7:
		{
			//Verdana font
			#ifdef READ_FROM_COMPRESSED
				tm.xres = 512;
				tm.yres = 16;
				tm.c = (unsigned char *)malloc(tm.xres * tm.yres * 4);
				get_compressed_tm(&tm, v7bmp);
				build_gl_texmap(&tm, &a07_texture_no);
			#else //READ_FROM_COMPRESSED
				glGenTextures(1, &a07_texture_no);
				char ts[MYMAX_PATH];
				sprintf(ts,"%s\\tex\\v7.bmp",game.current_path);
				int ret = read_tex(ts, a07_texture_no);
				if (!ret) { printf("ERROR - loading texmap v7.bmp\n"); return; }
				//also read the bitmap into a tm structure, for examination and parsing.
				ret = tex_read_ggfont_bmp(ts, &tm);
				if (!ret) { printf("ERROR - loading texmap v7.bmp\n"); return; }
			#endif //READ_FROM_COMPRESSED
			fbot = 2.0f;
			ftop = 12.0f;
			break;
		}
		case 8:
		{
			#ifdef READ_FROM_COMPRESSED
				tm.xres = 512;
				tm.yres = 16;
				tm.c = (unsigned char *)malloc(tm.xres * tm.yres * 4);
				get_compressed_tm(&tm, arial8bmp);
				build_gl_texmap(&tm, &a08_texture_no);
			#else //READ_FROM_COMPRESSED
				glGenTextures(1, &a08_texture_no);
				char ts[MYMAX_PATH];
				sprintf(ts,"%s\\tex\\arial8.bmp",game.current_path);
				int ret = read_tex(ts, a08_texture_no);
				if (!ret) { printf("ERROR - loading texmap arial8.bmp\n"); return; }
				//also read the bitmap into a tm structure, for examination and parsing.
				ret = tex_read_ggfont_bmp(ts, &tm);
				if (!ret) { printf("ERROR - loading texmap arial8.bmp\n"); return; }
			#endif //READ_FROM_COMPRESSED
			fbot = 2.0f;
			ftop = 12.0f;
			break;
		}
		case 10:
		{
			#ifdef READ_FROM_COMPRESSED
				tm.xres = 512;
				tm.yres = 16;
				tm.c = (unsigned char *)malloc(tm.xres * tm.yres * 4);
				get_compressed_tm(&tm, arial10bmp);
				build_gl_texmap(&tm, &a10_texture_no);
			#else //READ_FROM_COMPRESSED
				glGenTextures(1, &a10_texture_no);
				char ts[MYMAX_PATH];
				sprintf(ts,"%s\\tex\\arial10.bmp",game.current_path);
				int ret = read_tex(ts, a10_texture_no);
				if (!ret) { printf("ERROR - loading texmap arial10.bmp\n"); return; }
				//also read the bitmap into a tm structure, for examination and parsing.
				ret = tex_read_ggfont_bmp(ts, &tm);
				if (!ret) { printf("ERROR - loading texmap arial10.bmp\n"); return; }
			#endif //READ_FROM_COMPRESSED
			fbot = (float)(16 - 16);
			ftop = (float)(16 - 3);
			break;
		}
		case 12:
		{
			#ifdef READ_FROM_COMPRESSED
				tm.xres = 1024;
				tm.yres = 32;
				tm.c = (unsigned char *)malloc(tm.xres * tm.yres * 4);
				get_compressed_tm(&tm, arial12bmp);
				build_gl_texmap(&tm, &a12_texture_no);
			#else //READ_FROM_COMPRESSED
				glGenTextures(1, &a12_texture_no);
				char ts[MYMAX_PATH];
				sprintf(ts,"%s\\tex\\arial12.bmp",game.current_path);
				int ret = read_tex(ts, a12_texture_no);
				if (!ret) { printf("ERROR - loading texmap arial12.bmp\n"); return; }
				//also read the bitmap into a tm structure, for examination and parsing.
				ret = tex_read_ggfont_bmp(ts, &tm);
				if (!ret) { printf("ERROR - loading texmap arial12.bmp\n"); return; }
			#endif //READ_FROM_COMPRESSED
			fbot = (float)(32 - 20);
			ftop = (float)(32 - 4);
			break;
		}
		case 13:
		{
			#ifdef READ_FROM_COMPRESSED
				tm.xres = 1024;
				tm.yres = 32;
				tm.c = (unsigned char *)malloc(tm.xres * tm.yres * 4);
				get_compressed_tm(&tm, arial13bmp);
				build_gl_texmap(&tm, &a13_texture_no);
			#else //READ_FROM_COMPRESSED
				glGenTextures(1, &a13_texture_no);
				char ts[MYMAX_PATH];
				sprintf(ts,"%s\\tex\\arial13.bmp",game.current_path);
				int ret = read_tex(ts, a13_texture_no);
				if (!ret) { printf("ERROR - loading texmap arial13.bmp\n"); return; }
				//also read the bitmap into a tm structure, for examination and parsing.
				ret = tex_read_ggfont_bmp(ts, &tm);
				if (!ret) { printf("ERROR - loading texmap arial13.bmp\n"); return; }
			#endif //READ_FROM_COMPRESSED
			fbot = (float)(32 - 20);
			ftop = (float)(32 - 4);
			break;
		}
		case 16:
		{
			#ifdef READ_FROM_COMPRESSED
				tm.xres = 1024;
				tm.yres = 32;
				tm.c = (unsigned char *)malloc(tm.xres * tm.yres * 4);
				get_compressed_tm(&tm, arial16bmp);
				build_gl_texmap(&tm, &a16_texture_no);
			#else //READ_FROM_COMPRESSED
				glGenTextures(1, &a16_texture_no);
				char ts[MYMAX_PATH];
				sprintf(ts,"%s\\tex\\arial16.bmp",game.current_path);
				int ret = read_tex(ts, a16_texture_no);
				if (!ret) { printf("ERROR - loading texmap arial16.bmp\n"); return; }
				//also read the bitmap into a tm structure, for examination and parsing.
				ret = tex_read_ggfont_bmp(ts, &tm);
				if (!ret) { printf("ERROR - loading texmap arial16.bmp\n"); return; }
			#endif //READ_FROM_COMPRESSED
			fbot = (float)(32 - 24);
			ftop = (float)(32 - 4);
			break;
		}
		//case 40:
		//{
		//	#ifdef READ_FROM_COMPRESSED
		//		tm.xres = 2048;
		//		tm.yres = 64;
		//		tm.c = (unsigned char *)malloc(tm.xres * tm.yres * 4);
		//		get_compressed_tm(&tm, arial40bmp);
		//		build_gl_texmap(&tm, &a40_texture_no);
		//	#else  READ_FROM_COMPRESSED
		//		glGenTextures(1, &a40_texture_no);
		//		char ts[MYMAX_PATH];
		//		sprintf(ts,"%s\\tex\\arial40.bmp",game.current_path);
		//		int ret = read_tex(ts, a40_texture_no);
		//		if (!ret) { printf("ERROR - loading texmap arial40.bmp\n"); return; }
		//		//also read the bitmap into a tm structure, for examination and parsing.
		//		ret = tex_read_ggfont_bmp(ts, &tm);
		//		if (!ret) { printf("ERROR - loading texmap arial40.bmp\n"); return; }
		//	#endif READ_FROM_COMPRESSED
		//	fbot = 14.0f;
		//	ftop = 60.0f;
		//	break;
		//}
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
	//white marks the start of each character. (row 1)
	//yellow marks the end of each character. (row 0)
	//text starts in row 4.
	//
	xpos = 0;
	ypos = 0;
	n_characters = 0;
	//
	cspot = 0;
	while(1) {
		if ( find_next_white(&tm, &xpos, ypos) ) break;
		cstart[n_characters] = xpos;
		if ( find_next_yellow(&tm, &xpos, ypos) ) break;
		clen[n_characters] = xpos - cstart[n_characters] + 1;
		//
		tx[n_characters][0] = ((float)cstart[n_characters] * ftmx);
		tx[n_characters][1] = ((float)cstart[n_characters] * ftmx) + ((float)(clen[n_characters]) * ftmx);
		ty[n_characters][0] = fbot * ftmy;
		ty[n_characters][1] = ftop * ftmy;
		//
		n_characters++;
		//
		xpos += 1;
		if (xpos >= tm.xres) break;
	}
	//
	//#ifdef READ_FROM_COMPRESSED
	//if (v7) {
	//	free(v7);
	//	tm.c = NULL;
	//}
	//#else  READ_FROM_COMPRESSED
	//#endif READ_FROM_COMPRESSED

	//printf("n_characters: %i\n", n_characters);
	switch ( psize ) {
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
		//case 40:
		//{
		//	for (i=0; i<n_characters; i++) {
		//		//printf("%i %i\n", cstart[i], clen[i]);
		//		cstart_a40[i] = cstart[i];
		//		clen_a40[i] = clen[i];
		//		tx_a40[i][0] = tx[i][0];
		//		tx_a40[i][1] = tx[i][1];
		//		ty_a40[i][0] = ty[i][0];
		//		ty_a40[i][1] = ty[i][1];
		//	}
		//	break;
		//}
	}
	if (tm.c) free(tm.c);
}


#ifdef READ_FROM_COMPRESSED
#else  //READ_FROM_COMPRESSED
#endif //READ_FROM_COMPRESSED



int find_next_white(Texmap *tm, int *xpos, int ypos)
{
	#ifdef READ_FROM_COMPRESSED
	int sz34=4;
	#else //READ_FROM_COMPRESSED
	int sz34=3;
	#endif //READ_FROM_COMPRESSED
	int cref;
	unsigned char r,g,b;
	int xp;
	int xr  = tm->xres;
	int xr3 = (tm->yres - 2) * xr * sz34;

	//printf("find_next_white()...\n");
	while(1) {
		//move right until a color white is found (in row 1).
		xp = (*xpos) * sz34;
		r = tm->c[xr3+xp  ];
		g = tm->c[xr3+xp+1];
		b = tm->c[xr3+xp+2];
		cref = RGB(r,g,b);
		//printf("test for white %x\n", cref);
		if (cref == 0x00ffffff) {
			//printf("found white at: %i\n", *xpos);
			break;
		}
		*xpos += 1;
		if (*xpos >= xr) {
			return 1;
		}
	}
	return 0;
}
int find_next_yellow(Texmap *tm, int *xpos, int ypos)
{
	#ifdef READ_FROM_COMPRESSED
	int sz34=4;
	#else //READ_FROM_COMPRESSED
	int sz34=3;
	#endif //READ_FROM_COMPRESSED
	int cref;
	unsigned char r,g,b;
	int xp;
	int xr  = tm->xres;
	int xr3 = (tm->yres - 1) * xr * sz34;

	//printf("find_next_yellow()...\n");
	while(1) {
		//move right until a color yellow is found (in row 0).
		xp = (*xpos) * sz34;
		r = tm->c[xr3+xp  ];
		g = tm->c[xr3+xp+1];
		b = tm->c[xr3+xp+2];
		cref = RGB(r,g,b);
		//printf("test for yellow %x\n", cref);
		if (cref == 0x0000ffff) {
			//printf("found yellow at: %i\n", *xpos);
			break;
		}
		*xpos += 1;
		if (*xpos >= xr) {
			return 1;
		}
	}
	return 0;
}

int tex_read_ggfont_bmp(char *fileName, Texmap *tm)
{
	int i,j,w,h;
	unsigned char *rbuf;
	int fh,k,bytesPerLine;
	Bmphead bmphead;

	printf("tex_readgl_bmp(%s)...\n",fileName);
	fh = open(fileName, O_RDWR);
	if (fh < 0) { printf("ERROR - opening font file **%s**",fileName); return 0; }
	if (fh >= 0) {
		//Image is a BMP bitmap.
		GetBmphead(fh, &bmphead);
		w = (int)bmphead.width;
		h = (int)bmphead.depth;
		if (bmphead.bits == 4) { printf("ERROR - 4bit\n"); return 0; }
		if (bmphead.bits == 8) { printf("ERROR - 8bit\n"); return 0; }
		if (bmphead.bits == 24) { bytesPerLine = w * 3; }
		while(bytesPerLine % 4 != 0) { bytesPerLine++; }
		//
		tm->c = (unsigned char *)malloc( h * w * 4 );
		//
		//printf( "bytesPerLine: %i\n",bytesPerLine);
		rbuf = (unsigned char *)malloc(bytesPerLine);
		//
		unsigned char *ptr = tm->c;
		for(j=h-1; j>=0; j--) {
			read(fh, rbuf, bytesPerLine);
			k = 0;
			for (i=0; i<w; i++) {
				*(ptr+2) = rbuf[k++];
				*(ptr+1) = rbuf[k++];
				*(ptr  ) = rbuf[k++];
				ptr+=3;
			}
		}
		close(fh);
		tm->xres = w;
		tm->yres = h;
		free(rbuf);
		return 1;
	}
	return 0;
}

int read_tex(char *fileName, int texn)
{
	//return 0 if error.
	int i,j,w,h;
	unsigned char *rbuf;
	int fh,k,bytesPerLine;
	Bmphead bmphead;
	Texmap tm;

	//printf("ggFonts::read_tex(%s, %i)...\n", fileName, texn);
	//
	fh = open(fileName, O_RDWR);
	if (fh < 0) { printf("ERROR - opening font file **%s**\n",fileName); return 0; }
	if (fh >= 0) {
		GetBmphead(fh, &bmphead);
		w = (int)bmphead.width;
		h = (int)bmphead.depth;
		bytesPerLine = w * 3;
		while(bytesPerLine % 4 != 0) { bytesPerLine++; }
		//
		// alpha_channel
		tm.c = (unsigned char *)malloc( sizeof(char) * h * w * 4 );
		rbuf = (unsigned char *)malloc(bytesPerLine);
		//
		int p = 0;
		int c1,c2,c3,c4;
		int lcount=0;
		#ifdef SHOW_COMPRESSED_PRINTOUT
		int zcount=0;
		int wcount=0;
		#endif //SHOW_COMPRESSED_PRINTOUT
		unsigned char *ptr = tm.c;
		//printf("24-bit bitmap present.  bytesPerLine: %i  w: %i\n",bytesPerLine,w);
		#ifdef SHOW_COMPRESSED_PRINTOUT
			//A font bitmap has only 3 colors black, white, and yellow.
			//black=0  (0,0,0)
			//white=1  (255,255,255)
			//yellow=2 (255,255,0)
			//
			printf("\nunsigned char %s[]={\n", fileName);
		#endif //SHOW_COMPRESSED_PRINTOUT
		for(j=h-1; j>=0; j--) {
			read(fh, rbuf, bytesPerLine);
			k = 0;
			for (i=0; i<w; i++) {
				c3 = rbuf[k++];
				c2 = rbuf[k++];
				c1 = rbuf[k++];
				*ptr     = c1;
				*(ptr+1) = c2;
				*(ptr+2) = c3;
				//if (trans_color >= 0) {
				c4 = 255;
				//if ( RGB((unsigned char)c1,(unsigned char)c2,(unsigned char)c3) == 0) c4 = 0;
				if ( c1==0 && c2==0 && c3==0 ) c4 = 0;
				*(ptr+3) = (unsigned char)c4;
				ptr += 4;
				//----------------------
				#ifdef SHOW_COMPRESSED_PRINTOUT
				if (c4==0) {
					//if (c1==0 && c2==0 && c3==0) {
					//value is zero.
					zcount++;
					if (zcount == 1) {
						printf("0,"); lcount++;
						*tempptr=0; tempptr++;
					}
					else {
						if (zcount == 250) {
							printf("250,"); lcount++;
							*tempptr=250; tempptr++;
							zcount=0;
						}
					}
				}
				else {
					if (zcount) {
						printf("%i,",zcount);
						*tempptr=zcount; tempptr++;
						lcount++;
						zcount=0;
					}
					if (c1==255 && c2==255) {
						if (c3==0  ) {
							printf("254,"); lcount++;
							*tempptr=254; tempptr++;
						}
						if (c3==255) {
							printf("255,"); lcount++;
							*tempptr=255; tempptr++;
						}
					}
				}
				if (lcount > 200) {
					printf("\n");
					wcount += lcount;
					lcount=0;
				}
				#endif //SHOW_COMPRESSED_PRINTOUT
			}
		}
		#ifdef SHOW_COMPRESSED_PRINTOUT
		if (zcount) {
			printf("%i,",zcount); lcount++;
			*tempptr=zcount; tempptr++;
		}
		printf("99};\n"); lcount++;
		*tempptr=99; tempptr++;
		if (lcount) wcount += lcount;
		printf("unsigned char %s[%i]={\n", fileName, wcount);
		#endif //SHOW_COMPRESSED_PRINTOUT
	}
	#ifdef SHOW_COMPRESSED_PRINTOUT
	printf("\n");
	#endif //SHOW_COMPRESSED_PRINTOUT
	close(fh);
	tm.xres = w;
	tm.yres = h;
	free(rbuf);
	//
	//if (show_debug_printout) {
	//	unsigned char *ptr = tm.c;
	//	for (i=0; i<8192; i++) {
	//		printf("%i %i %i %i\n",*ptr,*(ptr+1),*(ptr+2),*(ptr+3));
	//		ptr+=4;
	//	}
	//}
	//printf("writing texture to gl  x: %i  y: %i\n",tm.xres, tm.yres);
	glBindTexture(GL_TEXTURE_2D, texn);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, tm.xres, tm.yres, 0, GL_RGBA, GL_UNSIGNED_BYTE, tm.c );
	free( tm.c );
	return 1;
}

void cleanup_fonts(void)
{
	//glDeleteTextures(1, &a40_texture_no);
	#ifdef COURIER
	glDeleteTextures(1, &c17_texture_no);
	#endif //COURIER
	glDeleteTextures(1, &a16_texture_no);
	glDeleteTextures(1, &a12_texture_no);
	glDeleteTextures(1, &a13_texture_no);
	glDeleteTextures(1, &a10_texture_no);
	glDeleteTextures(1, &a08_texture_no);
	glDeleteTextures(1, &a07_texture_no);
	glDeleteTextures(1, &a06_texture_no);
	glDeleteTextures(1, &a8b_texture_no);
}

