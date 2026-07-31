
#define GL_GLEXT_PROTOTYPES

#include <GL/glx.h>
#include <GL/glext.h>
#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <math.h>
#include <sys/stat.h>
#include "cglm/include/cglm/cglm.h"
#include <time.h>

#include "shader_util.h"
#include "geometry_util.h"
#include "vertex.h"
#include "texture_util.h"
#include "mouse.h"

#define WIDTH 800
#define HEIGHT 800

