#ifndef TEX_UTIL
#define TEX_UTIL

#include <GL/glx.h>
#include <GL/glext.h>
#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include "cglm/include/cglm/cglm.h"

#include "shader_util.h"

unsigned char* load_image(char* path, int* width, int* height);
void free_image(unsigned char* data);
void create_texture(unsigned int* texture, unsigned char* data, int width, int height, char* id, Shader sp, int bind_id);
void load_cubemap(unsigned int* texture, char* texture_arr[], int cubemap_len);

#endif
