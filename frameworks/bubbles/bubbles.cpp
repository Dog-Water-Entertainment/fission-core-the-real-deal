//3350
//demo of bubbles under water
//author:  Gordon Griesel
//date:    2017
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
//#include <X11/Xutil.h>
//#include <GL/gl.h>
//#include <GL/glu.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "fonts.h"

//defined types
typedef double Flt;
typedef double Vec[3];
typedef Flt	Matrix[4][4];

//macros
#define rnd() (((double)rand())/(double)RAND_MAX)
#define random(a) (rand()%a)
#define MakeVector(x, y, z, v) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecZero(a) (a)[0]=0.0;(a)[1]=0.0;(a)[2]=0.0
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]
#define VecDot(a,b)	((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0]; \
                      (c)[1]=(a)[1]-(b)[1]; \
                      (c)[2]=(a)[2]-(b)[2]
//constants
const float timeslice = 1.0f;
const float gravity = -0.2f;
#define ALPHA 1

class Image {
public:
	int width, height;
	unsigned char *data;
	~Image() { delete [] data; }
	Image(const char *fname) {
		if (fname[0] == '\0')
			return;
		//printf("fname **%s**\n", fname);
		int ppmFlag = 0;
		char name[40];
		strcpy(name, fname);
		int slen = strlen(name);
		char ppmname[80];
		if (strncmp(name+(slen-4), ".ppm", 4) == 0)
			ppmFlag = 1;
		if (ppmFlag) {
			strcpy(ppmname, name);
		} else {
			name[slen-4] = '\0';
			//printf("name **%s**\n", name);
			sprintf(ppmname,"%s.ppm", name);
			//printf("ppmname **%s**\n", ppmname);
			char ts[100];
			//system("convert eball.jpg eball.ppm");
			sprintf(ts, "convert %s %s", fname, ppmname);
			system(ts);
		}
		//sprintf(ts, "%s", name);
		FILE *fpi = fopen(ppmname, "r");
		if (fpi) {
			char line[200];
			fgets(line, 200, fpi);
			fgets(line, 200, fpi);
			//must skip comments and blank lines
			while (line[0] == '#' || strlen(line) < 2)
				fgets(line, 200, fpi);
			sscanf(line, "%i %i", &width, &height);
			fgets(line, 200, fpi);
			//get pixel data
			int n = width * height * 3;			
			data = new unsigned char[n];			
			for (int i=0; i<n; i++)
				data[i] = fgetc(fpi);
			fclose(fpi);
		} else {
			printf("ERROR opening image: %s\n",ppmname);
			exit(0);
		}
		if (!ppmFlag)
			unlink(ppmname);
	}
};
Image img = "./images/undersea.jpg";

typedef struct Bubble {
	Vec pos;
	Vec lastpos;
	Vec vel;
	Vec maxvel;
	Vec force;
	float radius;
	float color[3];
} Bubble;
const int MAX_BUBBLES = 1000;

class Global {
public:
	int xres, yres;
	GLuint underseaTexture;
	Bubble bubble[MAX_BUBBLES];
	int nbubbles;
	Global() {
		xres = 800;
		yres = 600;
		nbubbles = 0;
	}
} g;

//X Windows variables
class X11_wrapper {
private:
	Display *dpy;
	Window win;
public:
	~X11_wrapper() {
		XDestroyWindow(dpy, win);
		XCloseDisplay(dpy);
	}
	X11_wrapper() {
		GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
		//GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, None };
		XSetWindowAttributes swa;
		setupScreenRes(640, 480);
		dpy = XOpenDisplay(NULL);
		if (dpy == NULL) {
			printf("\n\tcannot connect to X server\n\n");
			exit(EXIT_FAILURE);
		}
		Window root = DefaultRootWindow(dpy);
		XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
		if (vi == NULL) {
			printf("\n\tno appropriate visual found\n\n");
			exit(EXIT_FAILURE);
		} 
		Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
		swa.colormap = cmap;
		swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
			StructureNotifyMask | SubstructureNotifyMask;
		win = XCreateWindow(dpy, root, 0, 0, g.xres, g.yres, 0,
			vi->depth, InputOutput, vi->visual,
			CWColormap | CWEventMask, &swa);
		GLXContext glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
		glXMakeCurrent(dpy, win, glc);
		setTitle();
	}
	void setTitle() {
		//Set the window title bar.
		XMapWindow(dpy, win);
		XStoreName(dpy, win, "Bubbly bubbles");
	}
	void setupScreenRes(const int w, const int h) {
		g.xres = w;
		g.yres = h;
	}
	void reshapeWindow(int width, int height) {
		//window has been resized.
		setupScreenRes(width, height);
		//
		glViewport(0, 0, (GLint)width, (GLint)height);
		glMatrixMode(GL_PROJECTION); glLoadIdentity();
		glMatrixMode(GL_MODELVIEW); glLoadIdentity();
		glOrtho(0, g.xres, 0, g.yres, -1, 1);
		setTitle();
	}
	void checkResize(XEvent *e) {
		//The ConfigureNotify is sent by the
		//server if the window is resized.
		if (e->type != ConfigureNotify)
			return;
		XConfigureEvent xce = e->xconfigure;
		if (xce.width != g.xres || xce.height != g.yres) {
			//Window size did change.
			reshapeWindow(xce.width, xce.height);
		}
	}
	bool getXPending() {
		return XPending(dpy);
	}
	XEvent getXNextEvent() {
		XEvent e;
		XNextEvent(dpy, &e);
		return e;
	}
	void swapBuffers() {
		glXSwapBuffers(dpy, win);
	}
} x11;

//function prototypes
void initOpengl(void);
void checkMouse(XEvent *e);
int checkKeys(XEvent *e);
void init();
void physics(void);
void render(void);

//-----------------------------------------------------------------------------
//Setup timers
const double physicsRate = 1.0 / 30.0;
const double oobillion = 1.0 / 1e9;
struct timespec timeStart, timeCurrent;
struct timespec timePause;
double physicsCountdown=0.0;
double timeSpan=0.0;
unsigned int upause=0;
double timeDiff(struct timespec *start, struct timespec *end) {
	return (double)(end->tv_sec - start->tv_sec ) +
			(double)(end->tv_nsec - start->tv_nsec) * oobillion;
}
void timeCopy(struct timespec *dest, struct timespec *source) {
	memcpy(dest, source, sizeof(struct timespec));
}
//-----------------------------------------------------------------------------



int main(void)
{
	initOpengl();
	init();
	clock_gettime(CLOCK_REALTIME, &timePause);
	clock_gettime(CLOCK_REALTIME, &timeStart);
	int done = 0;
	while (!done) {
		while (x11.getXPending()) {
			XEvent e = x11.getXNextEvent();
			x11.checkResize(&e);
			checkMouse(&e);
			done = checkKeys(&e);
		}
		clock_gettime(CLOCK_REALTIME, &timeCurrent);
		timeSpan = timeDiff(&timeStart, &timeCurrent);
		timeCopy(&timeStart, &timeCurrent);
		physicsCountdown += timeSpan;
		while(physicsCountdown >= physicsRate) {
			physics();
			physicsCountdown -= physicsRate;
		}
		render();
		x11.swapBuffers();
	}
	cleanup_fonts();
	return 0;
}

unsigned char *buildAlphaData(Image *img)
{
	//add 4th component to RGB stream...
	int i;
	int a,b,c;
	unsigned char *newdata, *ptr;
	unsigned char *data = (unsigned char *)img->data;
	newdata = (unsigned char *)malloc(img->width * img->height * 4);
	ptr = newdata;
	for (i=0; i<img->width * img->height * 3; i+=3) {
		a = *(data+0);
		b = *(data+1);
		c = *(data+2);
		*(ptr+0) = a;
		*(ptr+1) = b;
		*(ptr+2) = c;
		*(ptr+3) = (a|b|c);
		ptr += 4;
		data += 3;
	}
	return newdata;
}

void initOpengl(void)
{
	//OpenGL initialization
	glViewport(0, 0, g.xres, g.yres);
	//Initialize matrices
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	//This sets 2D mode (no perspective)
	glOrtho(0, g.xres, 0, g.yres, -1, 1);
	//
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_FOG);
	glDisable(GL_CULL_FACE);
	//
	//Clear the screen
	glClearColor(1.0, 1.0, 1.0, 1.0);
	//glClear(GL_COLOR_BUFFER_BIT);
	//Do this to allow fonts
	glEnable(GL_TEXTURE_2D);
	initialize_fonts();
	//
	//create opengl texture elements
	glGenTextures(1, &g.underseaTexture);
	int w = img.width;
	int h = img.height;
	//-------------------------------------------------------------------------
	//background
	glBindTexture(GL_TEXTURE_2D, g.underseaTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h,
		0, GL_RGB, GL_UNSIGNED_BYTE, img.data);
	//-------------------------------------------------------------------------
}

void initSounds(void)
{

}

void init() {

}

void checkMouse(XEvent *e)
{
	//Did the mouse move?
	//Was a mouse button clicked?
	static int savex = 0;
	static int savey = 0;
	//
	if (e->type == ButtonRelease) {
		return;
	}
	if (e->type == ButtonPress) {
		if (e->xbutton.button==1) {
			//Left button is down
		}
		if (e->xbutton.button==3) {
			//Right button is down
		}
	}
	if (savex != e->xbutton.x || savey != e->xbutton.y) {
		//Mouse moved
		savex = e->xbutton.x;
		savey = e->xbutton.y;
	}
}

int checkKeys(XEvent *e)
{
	//keyboard input?
	static int shift=0;
	if (e->type != KeyRelease && e->type != KeyPress)
		return 0;
	int key = XLookupKeysym(&e->xkey, 0);
	if (e->type == KeyRelease) {
		if (key == XK_Shift_L || key == XK_Shift_R)
			shift=0;
		return 0;
	}
	if (key == XK_Shift_L || key == XK_Shift_R) {
		shift=1;
		return 0;
	}
	(void)shift;
	switch (key) {
		case XK_b:
			break;
		case XK_Escape:
			return 1;
	}
	return 0;
}

Flt VecNormalize(Vec vec)
{
	Flt len, tlen;
	Flt xlen = vec[0];
	Flt ylen = vec[1];
	Flt zlen = vec[2];
	len = xlen*xlen + ylen*ylen + zlen*zlen;
	if (len == 0.0) {
		MakeVector(0.0,0.0,1.0,vec);
		return 1.0;
	}
	len = sqrt(len);
	tlen = 1.0 / len;
	vec[0] = xlen * tlen;
	vec[1] = ylen * tlen;
	vec[2] = zlen * tlen;
	return(len);
}

void cleanupBubbles()
{
	
}

void physics(void)
{
	//create a bubble
	if (g.nbubbles < MAX_BUBBLES) {
		g.bubble[g.nbubbles].pos[0] = rnd() * (g.xres-80.0) + 40.0;
		g.bubble[g.nbubbles].pos[1] = (float)g.yres/2.0 - 40.0;
		VecZero(g.bubble[g.nbubbles].vel);
		VecZero(g.bubble[g.nbubbles].force);
		//bubble radius
		g.bubble[g.nbubbles].radius = rnd() * 12.0 + 2.0;
		g.bubble[g.nbubbles].color[0] = 1.0;
		g.bubble[g.nbubbles].color[1] = 1.0;
		g.bubble[g.nbubbles].color[2] = 1.0;
		++g.nbubbles;
	}
	//move bubbles
	for (int i=0; i<g.nbubbles; i++) {
		g.bubble[i].force[0] = rnd() * 0.5 - 0.25;
		g.bubble[i].force[1] = 0.01 + g.bubble[i].radius*0.005;
		g.bubble[i].vel[0] += g.bubble[i].force[0];
		g.bubble[i].vel[1] += g.bubble[i].force[1];
		//constrain the x-movement of a bubble.
		if (g.bubble[i].vel[0] > 1.0)
			g.bubble[i].vel[0] = 1.0;
		if (g.bubble[i].vel[0] < -1.0)
			g.bubble[i].vel[0] = -1.0;
		g.bubble[i].pos[0] += g.bubble[i].vel[0];
		g.bubble[i].pos[1] += g.bubble[i].vel[1];
		//bubble radius slightly increases over its life.
		g.bubble[i].radius *= 1.002;
		//
		//Did bubble float off the top of the screen?
		if (g.bubble[i].pos[1] > g.yres+20) {
			//delete bubble from array list.
			--g.nbubbles;
			g.bubble[i] = g.bubble[g.nbubbles];
		}
		//Randomly make a bubble pop.
		if (rand() < 1000000) {
			//This will only happen now and then.
			//when it does, choose a bubble to pop.
			int bnum = rand() % g.nbubbles;
			--g.nbubbles;
			g.bubble[bnum] = g.bubble[g.nbubbles];
		}
	}
}

void render(void)
{
	//-----------------------------------------------------
	//This first thing creates components of a circle.
	//It happens only once, then never again.
	//-----------------------------------------------------
	static int firsttime=1;
	const int npts=12;
	static float vert[npts][2];
	if (firsttime) {
		firsttime=0;
		double ang = 0.0;
		double inc = (3.14159265 * 2.0) / (double)npts;
		for (int i=0; i<npts; i++) {
			vert[i][0] = cos(ang);
			vert[i][1] = sin(ang);
			ang += inc;
		}
	}
	//-----------------------------------------------------
	Rect r;
	//Clear the screen
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	//
	//background quad with texture
	glColor3f(1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, g.underseaTexture);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
		glTexCoord2f(0.0f, 0.0f); glVertex2i(0, g.yres);
		glTexCoord2f(1.0f, 0.0f); glVertex2i(g.xres, g.yres);
		glTexCoord2f(1.0f, 1.0f); glVertex2i(g.xres, 0);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	//draw bubbles
	for (int i=0; i<g.nbubbles; i++) {
		glPushMatrix();
		glTranslated(g.bubble[i].pos[0], g.bubble[i].pos[1], 0.0);
		#define BUBBLE_IS_CIRCLE
		#ifdef BUBBLE_IS_CIRCLE
		//draw bubble as a circle.
		float r;
		//glColor3f(1.0, 1.0, 1.0);
		glColor3fv(g.bubble[i].color);
		glBegin(GL_LINE_STRIP);
			r = g.bubble[i].radius;
			for (int j=0; j<npts; j++) {
				glVertex2f(vert[j][0]*r, vert[j][1]*r);
			}
			glVertex2f(vert[0][0]*r, vert[0][1]*r);
		glEnd();
		glBegin(GL_POINTS);
			r = g.bubble[i].radius * 0.4;
			glVertex2f(-r, r);
			glVertex2f(-r-1, r);
			glVertex2f(-r-1, r-1);
		glEnd();
		#else
		//bubble is a square
		glColor3f(1.0, 1.0, 1.0);
		float w = 4.0f;
		glBegin(GL_QUADS);
			glVertex2f(-w, -w);
			glVertex2f(-w,  w);
			glVertex2f( w,  w);
			glVertex2f( w, -w);
		glEnd();
		#endif
		glPopMatrix();
	}
	//
	//menu bar
	glColor3f(0.5, 1.0, 0.6);
	int midy = g.yres/2;
	glPushMatrix();
	glTranslated(0.0, -60.0, 0.0);
	glBegin(GL_QUADS);
		glVertex2i(40, midy-40);
		glVertex2i(40, midy+40);
		glVertex2i(g.xres-40, midy+40);
		glVertex2i(g.xres-40, midy-40);
	glEnd();
	glPopMatrix();
	//
	r.bot = g.yres - 20;
	r.left = 10;
	r.center = 0;
	ggprint8b(&r, 16, 0, "Bubbly bubbles demo program");
}

