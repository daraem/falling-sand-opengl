#define GL_GLEXT_PROTOTYPES
#include "lib.h"

Display* start() {
	int attributeList[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, None};

	Display* d;
	XVisualInfo *vi;
	XSetWindowAttributes swa;
	GLXContext cx;

	d = XOpenDisplay(0);
	vi = glXChooseVisual(d, DefaultScreen(d), attributeList);
	cx = glXCreateContext(d, vi, 0, GL_TRUE);

	swa.colormap = XCreateColormap(d, RootWindow(d, vi->screen), vi->visual, AllocNone);
	swa.event_mask = ExposureMask | StructureNotifyMask | KeyPressMask | PointerMotionMask | ButtonPressMask | ButtonReleaseMask;

	Window w = XCreateWindow(d, RootWindow(d, vi->screen), 0, 0, WIDTH, HEIGHT, 0, 
		      vi->depth, InputOutput, vi->visual, 
		      CWBorderPixel | CWColormap | CWEventMask, &swa);
	
	XStoreName(d, w, "-");
	XMapWindow(d, w);

	glXMakeCurrent(d, w, cx);

	return d;
}
