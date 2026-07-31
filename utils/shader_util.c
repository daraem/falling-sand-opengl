#define GL_GLEXT_PROTOTYPES

#include "shader_util.h"

char* shader_source_arr[20];
int shader_source_count = 0;

const char* load_shader(char* file) {
	char* source = malloc(sizeof(char) * 8096);
	
	shader_source_arr[shader_source_count] = source;
	shader_source_count += 1;

	int fd = open(file, O_APPEND, O_RDONLY);
	if(fd < 0) {
		perror(strerror(fd));
		return NULL;
	}

	int rd = -1;
	char c; int count = 0;
	while(rd != 0) {
		rd = read(fd, &c, sizeof(char));
		source[count] = c;
		count++;
	}
	source[count] = '\0';
	const char* ret_source = source;
	return ret_source;
}

unsigned int create_shader(const char* source, GLenum type) {
	unsigned int shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);
	return shader;
}

Shader create_program(unsigned int vs, unsigned int fs) {
	Shader s;
	s.sp = glCreateProgram();
	glAttachShader(s.sp, vs);
	glAttachShader(s.sp, fs);
	glLinkProgram(s.sp);
	return s;
}

Shader create_program_gs(unsigned int vs, unsigned int fs, unsigned int gs) {
	Shader s;
	s.sp = glCreateProgram();
	glAttachShader(s.sp, vs);
	glAttachShader(s.sp, fs);
	glAttachShader(s.sp, gs);
	glLinkProgram(s.sp);
	return s;
}

void use_program(Shader s) {
	glUseProgram(s.sp);
}

void shader_bind_4m(Shader s, char* reference, mat4 m) {
	unsigned int loc_4m = glGetUniformLocation(s.sp, reference);
	glUniformMatrix4fv(loc_4m, 1, GL_FALSE, (float*)m);
}

void shader_bind_3v(Shader s, char* reference, vec3 v) {
	glUniform3fv(
		glGetUniformLocation(s.sp, reference),
		1,
		v
	);
}

void shader_bind_1f(Shader s, char* reference, float f) {
	glUniform1f(
		glGetUniformLocation(s.sp, reference),
		f
	);
}

void free_shader() {
	for(int i = 0; i<shader_source_count; i++) {
		free(shader_source_arr[i]);
	}
	shader_source_count = 0;
}
