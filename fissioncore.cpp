//3350
//program: walk2.cpp
//author:  Gordon Griesel
//date:    summer 2017
//         spring 2018
//
//Walk cycle using a sprite sheet.
//images courtesy: http://games.ucla.edu/resource/walk-cycles/
//
//This program includes:
//  multiple sprite-sheet animations
//  a level tiling system
//  parallax scrolling of backgrounds
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include <set>
#include <chrono>
#include "log.h"
//#include "ppm.h"
#include "Image.h"
#include "fonts.h"
#include "efarmer.h"
#include "mguillory.h"
#include "bmartinez.h"
#include "emedrano.h"
#include "math/Math.h"

// Utils Include
#include "utils/Config.h"

// Scene Includes
#include "scene/SceneManager.h"

//defined types
typedef double Flt;
typedef double Vec[3];
typedef Flt	Matrix[4][4];

//macros
#define rnd() (((double)rand())/(double)RAND_MAX)
#define random(a) (rand()%a)
#define MakeVector(v, x, y, z) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]
#define VecDot(a,b)	((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0]; \
                             (c)[1]=(a)[1]-(b)[1]; \
(c)[2]=(a)[2]-(b)[2]
//constants
const float timeslice = 1.0f;
const float gravity = -0.2f;

// TODO: Implement a config file to set movement rate
const float movement_speed = 0.1f;

#define ALPHA 1

//function prototypes
void initOpengl();
void checkMouse(XEvent *e);
int checkKeys(XEvent *e);
bool is_movement_key(int key);
void init();
void physics();
void render();

//-----------------------------------------------------------------------------
//Setup timers
class Timers {
    public:
        double physicsRate;
        double oobillion;
        struct timespec timeStart, timeEnd, timeCurrent;
        struct timespec walkTime;
        Timers() {
            physicsRate = 1.0 / 30.0;
            oobillion = 1.0 / 1e9;
        }
        double timeDiff(struct timespec *start, struct timespec *end) {
            return (double)(end->tv_sec - start->tv_sec ) +
                (double)(end->tv_nsec - start->tv_nsec) * oobillion;
        }
        void timeCopy(struct timespec *dest, struct timespec *source) {
            memcpy(dest, source, sizeof(struct timespec));
        }
        void recordTime(struct timespec *t) {
            clock_gettime(CLOCK_REALTIME, t);
        }
} timers;
//-----------------------------------------------------------------------------


class Sprite {
    public:
        int onoff;
        int frame;
        double delay;
        Vec pos;
        Image *image;
        GLuint tex;
        struct timespec time;
        Sprite() {
            onoff = 0;
            frame = 0;
            image = NULL;
            delay = 0.1;
        }
};

class Global {
    public:
        bool gameStarted;
        std::set<int> move_keys;
        std::set<int> pressed_move_keys;
        StartScreen startScreen;

        // Added Things
        MapLoader mapCtx;	

        ConfigLoader cnfg = ConfigLoader("./config/main.config");

        unsigned int TARGET_FPS = 60;
        unsigned int fps;
        unsigned char keys[65536];
        bool walking_left;
        bool walking;
        bool dark;
        int xres, yres;
        int movie, movieStep;
        int walk;
        int walkFrame;
        double delay;
        Image *walkImage;
        GLuint walkTexture;
        Vec box[20];
        Sprite exp;
        Sprite exp44;
        Vec ball_pos;
        Vec ball_vel;
        //camera is centered at (0,0) lower-left of screen. 
        Flt camera[2];
        ~Global() {
            logClose();
        }
        Global() {
            pressed_move_keys = {};
            move_keys = {XK_Up, XK_Down, XK_Left, XK_Right};
            walking = false;
            walking_left = false;
            dark = false;
            gameStarted = false;
            logOpen();
            camera[0] = camera[1] = 0.0;
            movie=0;
            movieStep=2;
            xres=800;
            yres=600;
            walk=0;
            walkFrame=0;
            walkImage=NULL;
            MakeVector(ball_pos, 520.0, 0, 0);
            MakeVector(ball_vel, 0, 0, 0);
            delay = 0.1;
            exp.onoff=0;
            exp.frame=0;
            exp.image=NULL;
            exp.delay = 0.02;
            exp44.onoff=0;
            exp44.frame=0;
            exp44.image=NULL;
            exp44.delay = 0.022;
            for (int i=0; i<20; i++) {
                box[i][0] = rnd() * xres;
                box[i][1] = rnd() * (yres-220) + 220.0;
                box[i][2] = 0.0;
            }
            memset(keys, 0, 65536);
        }
} gl;

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
        void setTitle() {
            //Set the window title bar.
            XMapWindow(dpy, win);
            XStoreName(dpy, win, "Fission Core");
        }
        void setupScreenRes(const int w, const int h) {
            gl.xres = w;
            gl.yres = h;
        }
        X11_wrapper() {
            GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
            //GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, None };
            XSetWindowAttributes swa;
            setupScreenRes(gl.xres, gl.yres);
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
            win = XCreateWindow(dpy, root, 0, 0, gl.xres, gl.yres, 0,
                    vi->depth, InputOutput, vi->visual,
                    CWColormap | CWEventMask, &swa);
            GLXContext glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
            glXMakeCurrent(dpy, win, glc);
            setTitle();
        }
        void reshapeWindow(int width, int height) {
            //window has been resized.
            setupScreenRes(width, height);
            glViewport(0, 0, (GLint)width, (GLint)height);
            glMatrixMode(GL_PROJECTION); glLoadIdentity();
            glMatrixMode(GL_MODELVIEW); glLoadIdentity();
            glOrtho(0, gl.xres, 0, gl.yres, -1, 1);
            setTitle();
        }
        void checkResize(XEvent *e) {
            //The ConfigureNotify is sent by the
            //server if the window is resized.
            if (e->type != ConfigureNotify)
                return;
            XConfigureEvent xce = e->xconfigure;
            if (xce.width != gl.xres || xce.height != gl.yres) {
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

Image img[3] = {
    "./images/walk.gif",
    "./images/exp.png",
    "./images/exp44.png" };


int main(void)
{

    auto startTime = std::chrono::high_resolution_clock::now();
    int frameCount = 0;

    //Example usage of a config loading
    std::cout << gl.cnfg.getString("key") << std::endl;

    int interFPS = gl.cnfg.getInt("targetFPS");
    if(interFPS != -1) {
        std::cout << "doin sum" << std::endl;
        gl.TARGET_FPS = interFPS;
    }


    const int FRAME_TIME = 1000.0 / gl.TARGET_FPS;

    initOpengl();
    init();
    int done = 0;
    while (!done) {
        while (x11.getXPending()) {
            XEvent e = x11.getXNextEvent();
            x11.checkResize(&e);
            checkMouse(&e);
            checkKeys(&e);
        }

        physics();
        render();
        x11.swapBuffers();

        frameCount++;

        auto endTime = std::chrono::high_resolution_clock::now();
        auto msDuration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

        auto elapsedTime = msDuration.count() / 1000.0;


        // TODO: Implement a config file to set fram rate
        //       and bool for should cap frame rate.
        if (elapsedTime < FRAME_TIME ) {
            usleep((FRAME_TIME - elapsedTime) * 1000);
        }

        if (elapsedTime >= 1.0) {
            double fps = frameCount / elapsedTime;
            gl.fps = fps;



            frameCount = 0;
            startTime = endTime;
        }
    }
    cleanup_fonts();

    return 0;
}

void initOpengl(void)
{
    //OpenGL initialization
    glViewport(0, 0, gl.xres, gl.yres);
    //Initialize matrices
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    //This sets 2D mode (no perspective)
    glOrtho(0, gl.xres, 0, gl.yres, -1, 1);
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
    //load the images file into a ppm structure.
    //
    int w = img[0].width;
    int h = img[0].height;
    //
    //create opengl texture elements
    glGenTextures(1, &gl.walkTexture);
    //-------------------------------------------------------------------------
    //silhouette
    //this is similar to a sprite graphic
    //
    glBindTexture(GL_TEXTURE_2D, gl.walkTexture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    //
    //must build a new set of data...
    unsigned char *walkData = buildAlphaData(&img[0]);	
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, walkData);
    free(walkData);
    //-------------------------------------------------------------------------
    //create opengl texture elements
    w = img[1].width;
    h = img[1].height;
    glGenTextures(1, &gl.exp.tex);
    //-------------------------------------------------------------------------
    //this is similar to a sprite graphic
    glBindTexture(GL_TEXTURE_2D, gl.exp.tex);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    //must build a new set of data...
    unsigned char *xData = buildAlphaData(&img[1]);	
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, xData);
    free(xData);
    //-------------------------------------------------------------------------
    w = img[2].width;
    h = img[2].height;
    //create opengl texture elements
    glGenTextures(1, &gl.exp44.tex);
    //-------------------------------------------------------------------------
    //this is similar to a sprite graphic
    glBindTexture(GL_TEXTURE_2D, gl.exp44.tex);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    //must build a new set of data...
    xData = buildAlphaData(&img[2]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, xData);
    free(xData);
}


// Another Useful thing
void init() {
    gl.mapCtx.setFileName("test.map");
    gl.mapCtx.LoadMapFile();
    gl.mapCtx.loadTextures();
}

void checkMouse(XEvent *e)
{
    //printf("checkMouse()...\n"); fflush(stdout);
    //Did the mouse move?
    //Was a mouse button clicked?
    static int savex = 0;
    static int savey = 0;
    //
    if (e->type != ButtonRelease && e->type != ButtonPress &&
            e->type != MotionNotify)
        return;
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
    if (e->type == MotionNotify) {
        if (savex != e->xbutton.x || savey != e->xbutton.y) {
            //Mouse moved
            savex = e->xbutton.x;
            savey = e->xbutton.y;
        }
    }
}

bool is_movement_key(int key)
{
    return gl.move_keys.find(key) != gl.move_keys.end();
}


void screenCapture()
{
    static int fnum = 0;
    static int vid = 0;
    if (!vid) {
        system("mkdir ./vid");
        vid = 1;
    }
    unsigned char *data = (unsigned char *)malloc(gl.xres * gl.yres * 3);
    glReadPixels(0, 0, gl.xres, gl.yres, GL_RGB, GL_UNSIGNED_BYTE, data);
    char ts[32];
    sprintf(ts, "./vid/pic%03i.ppm", fnum);
    FILE *fpo = fopen(ts,"w");	
    if (fpo) {
        fprintf(fpo, "P6\n%i %i\n255\n", gl.xres, gl.yres);
        unsigned char *p = data;
        //go backwards a row at a time...
        p = p + ((gl.yres-1) * gl.xres * 3);
        unsigned char *start = p;
        for (int i=0; i<gl.yres; i++) {
            for (int j=0; j<gl.xres*3; j++) {
                fprintf(fpo, "%c",*p);
                ++p;
            }
            start = start - (gl.xres*3);
            p = start;
        }
        fclose(fpo);
        char s[256];
        sprintf(s, "convert ./vid/pic%03i.ppm ./vid/pic%03i.gif", fnum, fnum);
        system(s);
        unlink(ts);
    }
    ++fnum;
}

int checkKeys(XEvent *e)
{
    //keyboard input?
    static int shift=0;
    if (e->type != KeyPress && e->type != KeyRelease)
        return 0;
    int key = XLookupKeysym(&e->xkey, 0);
    gl.keys[key]=1;
    bool move_key = is_movement_key(key);

    if (e->type == KeyRelease) {
        if (move_key)
        {
            gl.pressed_move_keys.erase(key);
        }

        gl.keys[key]=0;
        if (key == XK_Shift_L || key == XK_Shift_R)
            shift=0;
        return 0;
    }

    if (move_key)
    {
        gl.pressed_move_keys.insert(key);
    }

    gl.keys[key]=1;
    if (key == XK_Shift_L || key == XK_Shift_R) {
        shift=1;
        return 0;
    }

    (void)shift;
    switch (key) {
        case XK_Return:
            if (!gl.gameStarted) {
                gl.gameStarted = true;
            }
            if (PauseMenu::isPaused())
                PauseMenu::selectOption(PauseMenu::getSelectedOption());
            break;
        case XK_s:
            screenCapture();
            break;
        case XK_m:
            gl.movie ^= 1;
            break;
        case XK_n:
            gl.dark = !gl.dark;
            break;
        case XK_w:
            timers.recordTime(&timers.walkTime);
            gl.walk ^= 1;
            break;
        case XK_e:
            gl.exp.pos[0] = 200.0;
            gl.exp.pos[1] = -60.0;
            gl.exp.pos[2] =   0.0;
            timers.recordTime(&gl.exp.time);
            gl.exp.onoff ^= 1;
            break;
        case XK_f:
            gl.exp44.pos[0] = 200.0;
            gl.exp44.pos[1] = -60.0;
            gl.exp44.pos[2] =   0.0;
            timers.recordTime(&gl.exp44.time);
            gl.exp44.onoff ^= 1;
            break;
        case XK_Left:
            break;
        case XK_Right:
            break;
        case XK_Up:
            using option = PauseMenu::PauseMenuOption;
            if (PauseMenu::isPaused()) {
                switch (PauseMenu::getSelectedOption()) {
                    case option::OPTIONS:
                        PauseMenu::setSelectedOption(option::RESUME);
                        break;
                    case option::QUIT:
                        PauseMenu::setSelectedOption(option::OPTIONS);
                        break;
                    default:
                        break;
                }

            }
            break;
        case XK_Down:
            using option = PauseMenu::PauseMenuOption;

            if (PauseMenu::isPaused()) {
                switch (PauseMenu::getSelectedOption()) {
                    case option::RESUME:
                        PauseMenu::setSelectedOption(option::OPTIONS);
                        break;
                    case option::OPTIONS:
                        PauseMenu::setSelectedOption(option::QUIT);
                        break;
                    default:
                        break;
                }

            }

            break;
        case XK_equal:
            gl.delay -= 0.005;
            if (gl.delay < 0.005)
                gl.delay = 0.005;
            break;
        case XK_minus:
            gl.delay += 0.005;
            break;
        case XK_Escape:
            if (!PauseMenu::isPaused())
                PauseMenu::pause();
            else
                PauseMenu::resume();

            break;
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
        MakeVector(vec, 0.0, 0.0, 1.0);
        return 1.0;
    }
    len = sqrt(len);
    tlen = 1.0 / len;
    vec[0] = xlen * tlen;
    vec[1] = ylen * tlen;
    vec[2] = zlen * tlen;
    return(len);
}

void physics(void)
{
    if (!gl.gameStarted) { //Game doesn't run until started
        return;
    }
    if (PauseMenu::isPaused())
    {
        return;
    }

    else if (!gl.walk && gl.pressed_move_keys.empty())
    {
        gl.walkFrame = 4;
    }
    else {
        if (gl.keys[XK_Right])
            gl.walking_left = false;
        else if (gl.keys[XK_Left])
            gl.walking_left = true;

        //man is walking...
        //when time is up, advance the frame.
        timers.recordTime(&timers.timeCurrent);
        double timeSpan = timers.timeDiff(&timers.walkTime, &timers.timeCurrent);
        if (timeSpan > gl.delay) {
            //advance
            ++gl.walkFrame;
            if (gl.walkFrame >= 16)
                gl.walkFrame -= 16;
            timers.recordTime(&timers.walkTime);
        }

        //
        // Check for movement keys to move map
        //
        if(gl.keys[XK_Left]) {
            gl.mapCtx.setPlayerPos(Vec2(gl.mapCtx.getPlayerPos().x - movement_speed,
                        gl.mapCtx.getPlayerPos().y));
        }
        if(gl.keys[XK_Right]) {
            gl.mapCtx.setPlayerPos(Vec2(gl.mapCtx.getPlayerPos().x + movement_speed,
                        gl.mapCtx.getPlayerPos().y));
        }
        if(gl.keys[XK_Up]) {
            gl.mapCtx.setPlayerPos(Vec2(gl.mapCtx.getPlayerPos().x,
                        gl.mapCtx.getPlayerPos().y - movement_speed));
        }
        if(gl.keys[XK_Down]) {
            gl.mapCtx.setPlayerPos(Vec2(gl.mapCtx.getPlayerPos().x,
                        gl.mapCtx.getPlayerPos().y + movement_speed));
        }


        if (gl.exp.onoff) {
            gl.exp.pos[0] -= 2.0 * (0.05 / gl.delay);
        }
        if (gl.exp44.onoff) {
            gl.exp44.pos[0] -= 2.0 * (0.05 / gl.delay);
        }
    }
    if (gl.exp.onoff) {
        //explosion is happening
        timers.recordTime(&timers.timeCurrent);
        double timeSpan = timers.timeDiff(&gl.exp.time, &timers.timeCurrent);
        if (timeSpan > gl.exp.delay) {
            //advance explosion frame
            ++gl.exp.frame;
            if (gl.exp.frame >= 23) {
                //explosion is done.
                gl.exp.onoff = 0;
                gl.exp.frame = 0;
            } else {
                timers.recordTime(&gl.exp.time);
            }
        }
    }
    if (gl.exp44.onoff) {
        //explosion is happening
        timers.recordTime(&timers.timeCurrent);
        double timeSpan = timers.timeDiff(&gl.exp44.time, &timers.timeCurrent);
        if (timeSpan > gl.exp44.delay) {
            //advance explosion frame
            ++gl.exp44.frame;
            if (gl.exp44.frame >= 16) {
                //explosion is done.
                gl.exp44.onoff = 0;
                gl.exp44.frame = 0;
            } else {
                timers.recordTime(&gl.exp44.time);
            }
        }
    }
}


void render(void)
{
    Rect r;
    //Clear the screen
    glClearColor(0.1, 0.1, 0.1, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    if (!gl.gameStarted) { //Render the start screen until game starts
        gl.startScreen.renderStartScreen(gl.xres, gl.yres);
        return;
    }

    float cx = gl.xres/2.0;
    float cy = gl.yres/2.0;

    // Map Rendering
    gl.mapCtx.render();


    //--------------------------------------
    //
    //#define SHOW_FAKE_SHADOW
#ifdef SHOW_FAKE_SHADOW
    glColor3f(0.25, 0.25, 0.25);
    glBegin(GL_QUADS);
    glVertex2i(cx-60, 150);
    glVertex2i(cx+50, 150);
    glVertex2i(cx+50, 130);
    glVertex2i(cx-60, 130);
    glEnd();
#endif
    //
    // Walk texture
    float h = 200.0;
    float w = h * 0.5;
    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, gl.walkTexture);
    //
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);
    int ix = gl.walkFrame % 8;
    int iy = 0;
    if (gl.walkFrame >= 8)
        iy = 1;
    float fx = (float)ix / 8.0;
    float fy = (float)iy / 2.0;
    glBegin(GL_QUADS);
    if (gl.walking_left) {
        glTexCoord2f(fx+.125, fy+.5); glVertex2i(cx-w, cy-h);
        glTexCoord2f(fx+.125, fy);    glVertex2i(cx-w, cy+h);
        glTexCoord2f(fx,      fy);    glVertex2i(cx+w, cy+h);
        glTexCoord2f(fx,      fy+.5); glVertex2i(cx+w, cy-h);
    } else {
        glTexCoord2f(fx,      fy+.5); glVertex2i(cx-w, cy-h);
        glTexCoord2f(fx,      fy);    glVertex2i(cx-w, cy+h);
        glTexCoord2f(fx+.125, fy);    glVertex2i(cx+w, cy+h);
        glTexCoord2f(fx+.125, fy+.5); glVertex2i(cx+w, cy-h);
    }


    glEnd();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_ALPHA_TEST);
    //
    //
    if (gl.exp.onoff) {
        h = 80.0;
        w = 80.0;
        glPushMatrix();
        glColor3f(1.0, 1.0, 1.0);
        glBindTexture(GL_TEXTURE_2D, gl.exp.tex);
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.0f);
        glColor4ub(255,255,255,255);
        glTranslated(gl.exp.pos[0], gl.exp.pos[1], gl.exp.pos[2]);
        int ix = gl.exp.frame % 5;
        int iy = gl.exp.frame / 5;
        float tx = (float)ix / 5.0;
        float ty = (float)iy / 5.0;
        glBegin(GL_QUADS);
        glTexCoord2f(tx,     ty+0.2); glVertex2i(cx-w, cy-h);
        glTexCoord2f(tx,     ty);     glVertex2i(cx-w, cy+h);
        glTexCoord2f(tx+0.2, ty);     glVertex2i(cx+w, cy+h);
        glTexCoord2f(tx+0.2, ty+0.2); glVertex2i(cx+w, cy-h);
        glEnd();
        glPopMatrix();
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_ALPHA_TEST);
    }
    //
    //
    if (gl.exp44.onoff) {
        h = 80.0;
        w = 80.0;
        glPushMatrix();
        glColor3f(1.0, 1.0, 1.0);
        glBindTexture(GL_TEXTURE_2D, gl.exp44.tex);
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.0f);
        glColor4ub(255,255,255,255);
        glTranslated(gl.exp44.pos[0], gl.exp44.pos[1], gl.exp44.pos[2]);
        int ix = gl.exp44.frame % 4;
        int iy = gl.exp44.frame / 4;
        float tx = (float)ix / 4.0;
        float ty = (float)iy / 4.0;
        glBegin(GL_QUADS);
        glTexCoord2f(tx,      ty+0.25); glVertex2i(cx-w, cy-h);
        glTexCoord2f(tx,      ty);      glVertex2i(cx-w, cy+h);
        glTexCoord2f(tx+0.25, ty);      glVertex2i(cx+w, cy+h);
        glTexCoord2f(tx+0.25, ty+0.25); glVertex2i(cx+w, cy-h);
        glEnd();
        glPopMatrix();
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_ALPHA_TEST);
    }
    darkMode(gl.dark, gl.xres, gl.yres);
    unsigned int c = 0x00ffff44;
    r.bot = gl.yres - 20;
    r.left = 10;
    r.center = 0;
    ggprint8b(&r, 16, c, "W   Walk cycle");
    ggprint8b(&r, 16, c, "E   Explosion");
    ggprint8b(&r, 16, c, "N   Dim lights");
    ggprint8b(&r, 16, c, "+   faster");
    ggprint8b(&r, 16, c, "-   slower");
    ggprint8b(&r, 16, c, "right arrow -> walk right");
    ggprint8b(&r, 16, c, "left arrow  <- walk left");
    ggprint8b(&r, 16, c, "frame: %i", gl.walkFrame);
    ggprint8b(&r, 16, c, "fps: %d", gl.fps);
    if (gl.movie) {
        screenCapture();
    }

    PauseMenu::render(gl.xres, gl.yres);	
}

