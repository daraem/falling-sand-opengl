#ifndef SUTIL
#define SUTIL

#include <GL/glx.h>
#include <GL/glext.h>
#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include "cglm/include/cglm/cglm.h"

typedef struct {
	unsigned int sp;
	char* source_ptr;
} Shader;

const char* load_shader(char* file);
unsigned int create_shader(const char* source, GLenum type);
Shader create_program(unsigned int vs, unsigned int fs);
Shader create_program_gs(unsigned int vs, unsigned int fs, unsigned int gs);
void use_program(Shader s);

void shader_bind_1f(Shader s, char* reference, float f);
void shader_bind_3v(Shader s, char* reference, vec3 v);
void shader_bind_4m(Shader s, char* reference, mat4 m);

void free_shader();

#endif
