#ifndef GUTIL 
#define GUTIL
#include <GL/glx.h>
#include <GL/glext.h>
#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include "cglm/include/cglm/cglm.h"

void circle(int detail_f, vec3* arr);
void sphere(int detail_f, vec3* arr);
void sphere_test(int detail_f, vec3* arr);
void pyramid_nbase(int base, float height, vec3* arr);

void plane(int width, int div, vec3* arr);
#endif
