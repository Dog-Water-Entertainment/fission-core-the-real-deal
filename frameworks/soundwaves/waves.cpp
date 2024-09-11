//
//program: waves.cpp
//author:  Gordon Griesel
//date:    2017
//
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>

//X Windows variables
Display *dpy;
Window win;
GLXContext glc;

void initXWindows(void);
void cleanupXWindows(void);
void init_opengl(void);
void check_resize(XEvent *e);
void check_mouse(XEvent *e);
int check_keys(XEvent *e);
void physics(void);
void render(void);

class Global {
public:
	int xres, yres;
	Global() {
		xres=640, yres=480;
	}
} g;

class Circle {
public:
	float center[2];
	float radius;
	float color[3];
	Circle(int x, int y, int r) {
		center[0] = (float)x;
		center[1] = (float)y;
		radius = (float)r;
		color[0] = 0.2;
		color[1] = 0.1;
		color[2] = 0.1;
	}
} wave(g.xres/2, g.yres-200, 10);


int main(void)
{
	initXWindows();
	init_opengl();
	int done=0;
	while (!done) {
		while (XPending(dpy)) {
			XEvent e;
			XNextEvent(dpy, &e);
			check_resize(&e);
			check_mouse(&e);
			done = check_keys(&e);
		}
		physics();
		render();
		glXSwapBuffers(dpy, win);
	}
	cleanupXWindows();
	return 0;
}

void cleanupXWindows(void)
{
	XDestroyWindow(dpy, win);
	XCloseDisplay(dpy);
}

void set_title(void)
{
	//Set the window title bar.
	XMapWindow(dpy, win);
	XStoreName(dpy, win, "waves from a transmitter");
}

void setup_screen_res(const int w, const int h)
{
	g.xres = w;
	g.yres = h;
}

void initXWindows(void)
{
	GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
	//GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, None };
	setup_screen_res(640, 480);
	dpy = XOpenDisplay(NULL);
	if(dpy == NULL) {
		printf("\n\tcannot connect to X server\n\n");
		exit(EXIT_FAILURE);
	}
	Window root = DefaultRootWindow(dpy);
	XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
	if(vi == NULL) {
		printf("\n\tno appropriate visual found\n\n");
		exit(EXIT_FAILURE);
	} 
	Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
	XSetWindowAttributes swa;
	swa.colormap = cmap;
	swa.event_mask =
		ExposureMask | KeyPressMask | KeyReleaseMask | PointerMotionMask |
		ButtonPressMask | ButtonReleaseMask |
		StructureNotifyMask | SubstructureNotifyMask;
	win = XCreateWindow(dpy, root, 0, 0, g.xres, g.yres, 0,
							vi->depth, InputOutput, vi->visual,
							CWColormap | CWEventMask, &swa);
	set_title();
	glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
	glXMakeCurrent(dpy, win, glc);
}

void reshape_window(int width, int height)
{
	//window has been resized.
	setup_screen_res(width, height);
	//
	glViewport(0, 0, (GLint)width, (GLint)height);
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	glOrtho(0, g.xres, 0, g.yres, -1, 1);
	set_title();
}

void init_opengl(void)
{
	//OpenGL initialization
	glViewport(0, 0, g.xres, g.yres);
	//Initialize matrices
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	//This sets 2D mode (no perspective)
	glOrtho(0, g.xres, 0, g.yres, -1, 1);
	//Clear the screen
	glClearColor(1.0, 1.0, 1.0, 1.0);
	//glClear(GL_COLOR_BUFFER_BIT);
}

void check_resize(XEvent *e)
{
	//The ConfigureNotify is sent by the
	//server if the window is resized.
	if (e->type != ConfigureNotify)
		return;
	XConfigureEvent xce = e->xconfigure;
	if (xce.width != g.xres || xce.height != g.yres) {
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

int check_keys(XEvent *e)
{
	//Was there input from the keyboard?
	if (e->type == KeyPress) {
		int key = XLookupKeysym(&e->xkey, 0);
		if (key == XK_Escape) {
			return 1;
		}
	}
	return 0;
}

void physics()
{
	//no physics
}

void drawTransmitter()
{
	glColor3ub(0,0,0);
	float base = 50.0;
	glLineWidth(4.0);
	glBegin(GL_LINES);
		glVertex2f(wave.center[0],      wave.center[1]);
		glVertex2f(wave.center[0]-base, 0.0);
		glVertex2f(wave.center[0],      wave.center[1]);
		glVertex2f(wave.center[0]+base, 0.0);
	glEnd();
	//draw shear supports
	//points along each side...
	float slope = base / wave.center[1];
	float pt[16][2];
	float step = wave.center[1] / 7.0;
	float level = 0.0;
	for (int i=0; i<7; i++) {
		pt[i][1] = level;
		pt[i][0] = pt[i][1] * slope;
		if (i > 0) {
			glLineWidth(2.0);
			glBegin(GL_LINES);
				//horizontal
				glVertex2f(wave.center[0] - base + pt[i][0], pt[i][1]);
				glVertex2f(wave.center[0] + base - pt[i][0], pt[i][1]);
			glEnd();
			glLineWidth(1.0);
			glBegin(GL_LINES);
				//criss-cross
				glVertex2f(wave.center[0] - base + pt[i-1][0], level-step);
				glVertex2f(wave.center[0] + base - pt[i][0], level);
				glVertex2f(wave.center[0] + base - pt[i-1][0], level-step);
				glVertex2f(wave.center[0] - base + pt[i][0], level);
			glEnd();
		}
		level += step;
	}
}

void drawWaves()
{
	int n=20;
	float ang=0.0;
	float inc = 3.14159*2.0/(float)n;
	float col[3];
	col[0] = wave.color[0] * (wave.radius/30.0);
	col[1] = wave.color[1] * (wave.radius/30.0);
	col[2] = wave.color[2] * (wave.radius/30.0);
	//clamp color values
	if (col[0] > 1.0) col[0] = 1.0;
	if (col[1] > 1.0) col[1] = 1.0;
	if (col[2] > 1.0) col[2] = 1.0;
	glColor3fv(col);
	glLineWidth(3.0);
	glPushMatrix();
	glTranslatef(wave.center[0], wave.center[1], 0.0);
	glBegin(GL_LINE_LOOP);
	for (int i=0; i<n; i++) {
		glVertex2f(cos(ang)*wave.radius, sin(ang)*wave.radius);
		ang += inc;
	}
	glEnd();
	glLineWidth(1.0);
	glPopMatrix();
	wave.radius += 5.0;
	//stop wave when color fades out
	if (col[0]+col[1]+col[2] >= 3.0)
		wave.radius = 4.0;
}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	drawTransmitter();
	drawWaves();
}














