#include "lib.h"

#define FRAMERATE 60

struct timeval tv;
double frame = 0;

void* mod_loop(Display* d, void*(*setup)(), void* (*draw)(Mouse m, double f), void*(*free_game)()) {
	(*setup)();

	int x11_fd;
	fd_set fd_con_set;

	x11_fd = ConnectionNumber(d);
	
	XEvent e;

	double delta = 1/(float)(FRAMERATE);

	Mouse mouse = (Mouse){.x = 0, .y = 0,
			      .tx = 0., .ty = 0.,
			      .pressed = -1};
	mouse.pressed = -1;
	int fd_state;
	long int mod_time = 0;

	struct timespec ftime;
	clock_gettime(CLOCK_MONOTONIC, &ftime);
	long int start_time = (ftime.tv_sec * 1000) + (ftime.tv_nsec/1000000);
	for(;;) {
		struct stat info_stat;
		fd_state = open("src/libgame.c", 0, O_RDWR);
		fstat(fd_state, &info_stat);
		if(mod_time == 0) {
			mod_time = info_stat.st_mtime;
		} else if(mod_time != info_stat.st_mtime) {
			return NULL;
		}
		close(fd_state);

		char buf[32];

		FD_ZERO(&fd_con_set);
		FD_SET(x11_fd, &fd_con_set);
	
		tv.tv_usec = 16660;
		tv.tv_sec = 0;

		clock_gettime(CLOCK_MONOTONIC, &ftime);
		long int current_time = ((ftime.tv_sec * 1000) + (ftime.tv_nsec/1000000)) - start_time;
		select(x11_fd + 1, &fd_con_set, NULL, NULL, &tv);
		frame += delta;
		while(XPending(d)) {
			XNextEvent(d, &e);
			switch(e.type) {
				case MotionNotify:
					// -1 + (2 + val)
					int x = e.xmotion.x; int y = e.xmotion.y;
					float tx = (float)e.xmotion.x/WIDTH; float ty = (float)e.xmotion.y/HEIGHT;
					mouse.x = x; mouse.y = y;
					mouse.tx = (-1 + (2*tx)); mouse.ty = (1 - (2*ty)); 
					break;
				case ConfigureNotify:
					glViewport(0, 0, WIDTH, HEIGHT);
					break;
				case KeyPress:
					KeySym k;
					XLookupString(&e.xkey, buf, sizeof(buf), &k, NULL);
					switch(k) {
						case XK_q:
							exit(EXIT_SUCCESS);
							(*free_game)();
							break;
					}
					break;
				case ButtonPress:
					mouse.pressed = e.xbutton.button;
					break;
				case ButtonRelease:
					mouse.pressed = -1;
					break;
			}

		}
		if(current_time > (delta * 1000)) {
			start_time += current_time;
			(*draw)(mouse, frame);
		}
	}

}
