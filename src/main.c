#include "lib.h"

int main() {
	while(system("./compile.sh"));
	void* dl = dlopen("./lib/window.so", RTLD_NOW);
	if(dl == NULL) printf("error dl: %s\n", dlerror());
	Display* (*start)();
	start = (Display*(*)(void)) dlsym(dl, "start");

	Display* d = (*start)();

	while(true) {
		while(system("./compile.sh"));
		void* dl_lp = dlopen("./lib/window_loop.so", RTLD_NOW);
		void* dl_gm = dlopen("./lib/libgame.so", RTLD_NOW);	

		void* (*setup)();
		void* (*draw)(Mouse, double);
		void* (*free_game)();
		setup = dlsym(dl_gm, "setup");
		draw = dlsym(dl_gm, "draw");
		free = dlsym(dl_gm, "free_game");

		void* (*mod_loop)(Display*, void*(*setup)(), void* (*draw)(Mouse, double), void* (*free_game)());
		
		mod_loop = dlsym(dl_lp, "mod_loop");
		if(mod_loop == NULL) printf("error\n");
		(*mod_loop)(d, setup, draw);
		dlclose(dl_lp);
		dlclose(dl_gm);
	}

	dlclose(dl);
	return 0;
}
