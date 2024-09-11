//program: ggtest.cpp
//author:  Gordon Griesel
//date:    2014
//
//This program is used to test the font library under OpenGL
//
//
#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>
//#include <time.h>
//#include <math.h>
//#include <X11/Xutil.h>
//#include <GL/gl.h>
//#include <GL/glu.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "fonts.h"
#include "log.h"

//X Windows variables
Display *dpy;
Window win;

void initXWindows(void);
void init_opengl(void);
void cleanupXWindows(void);
void check_resize(XEvent *e);
void check_mouse(XEvent *e);
void check_keys(XEvent *e);
void physics(void);
void render(void);

int done=0;
int xres=640, yres=480;
float pos[3]={20.0,200.0,0.0};
float vel[3]={3.0,0.0,0.0};


int main(void) {
	logOpen();
	initXWindows();
	init_opengl();
	while(!done) {
		while(XPending(dpy)) {
			XEvent e;
			XNextEvent(dpy, &e);
			check_resize(&e);
			check_mouse(&e);
			check_keys(&e);
		}
		physics();
		render();
		glXSwapBuffers(dpy, win);
	}
	cleanupXWindows();
	cleanup_fonts();
	logClose();
	return 0;
}

void cleanupXWindows(void) {
	XDestroyWindow(dpy, win);
	XCloseDisplay(dpy);
}

void set_title(void) {
	//Set the window title bar.
	XMapWindow(dpy, win);
	int state = texState();
	char ts[200];
	sprintf(ts, "Various fonts shown below. (%s)",
	        ((state==0) ? "from compressed array" : "from bmp files") );
	XStoreName(dpy, win, ts);
}

void setup_screen_res(const int w, const int h) {
	xres = w;
	yres = h;
}

void initXWindows(void) {
	Window root;
	GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
	//GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, None };
	XVisualInfo *vi;
	Colormap cmap;
	XSetWindowAttributes swa;

	setup_screen_res(640, 480);
	dpy = XOpenDisplay(NULL);
	if(dpy == NULL) {
		printf("\n\tcannot connect to X server\n\n");
		exit(EXIT_FAILURE);
	}
	root = DefaultRootWindow(dpy);
	vi = glXChooseVisual(dpy, 0, att);
	if(vi == NULL) {
		printf("\n\tno appropriate visual found\n\n");
		exit(EXIT_FAILURE);
	} 
	//else {
	//	// %p creates hexadecimal output like in glxinfo
	//	printf("\n\tvisual %p selected\n", (void *)vi->visualid);
	//}
	cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
	swa.colormap = cmap;
	swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
						StructureNotifyMask | SubstructureNotifyMask;
	win = XCreateWindow(dpy, root, 0, 0, xres, yres, 0,
							vi->depth, InputOutput, vi->visual,
							CWColormap | CWEventMask, &swa);
	GLXContext glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
	glXMakeCurrent(dpy, win, glc);
	set_title();
}

void reshape_window(int width, int height) {
	//window has been resized.
	setup_screen_res(width, height);
	//
	glViewport(0, 0, (GLint)width, (GLint)height);
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	glOrtho(0, xres, 0, yres, -1, 1);
	set_title();
}

void init_opengl(void) {
	//OpenGL initialization
	glViewport(0, 0, xres, yres);
	//Initialize matrices
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	//This sets 2D mode (no perspective)
	glOrtho(0, xres, 0, yres, -1, 1);
	//Clear color
	glClearColor(1.0, 1.0, 1.0, 1.0);
	//Do this to allow fonts
	glEnable(GL_TEXTURE_2D);
	initialize_fonts();
}

void check_resize(XEvent *e) {
	//The ConfigureNotify is sent by the
	//server if the window is resized.
	if (e->type != ConfigureNotify)
		return;
	XConfigureEvent xce = e->xconfigure;
	if (xce.width != xres || xce.height != yres) {
		//Window size did change.
		reshape_window(xce.width, xce.height);
	}
}

void check_mouse(XEvent *e)
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

void check_keys(XEvent *e)
{
	//Was there input from the keyboard?
	if (e->type == KeyPress) {
		int key = XLookupKeysym(&e->xkey, 0);
		if (key == XK_Escape) {
			done=1;
			return;
		}
	}
}

void physics(void)
{
	//no physics
}

void render(void)
{
	const int skip = 20;
	Rect r;
	glClear(GL_COLOR_BUFFER_BIT);
	//
	char ts[100] = "ABCDEFGHIJK abcdefghijklmnopqrstuvwxyz";
	r.bot = yres - 20;
	r.left = 10;
	r.center = 0;
	ggprint06(&r, skip, 0, "ggprint06  %s", ts);
	ggprint07(&r, skip, 0, "ggprint07  %s", ts);
	ggprint08(&r, skip, 0, "ggprint08  %s", ts);
	ggprint8b(&r, skip, 0, "ggprint8b  %s", ts);
	ggprint10(&r, skip, 0, "ggprint10  %s", ts);
	ggprint12(&r, skip, 0, "ggprint12  %s", ts);
	ggprint13(&r, 30,   0, "ggprint13  %s", ts);
	ggprint16(&r, 30,   0, "ggprint16  %s", ts);
	ggprint17(&r, 50,   0, "ggprint17  %s", ts);
	ggprint40(&r, 0,    0, "ggprint40  %s", ts);
}

