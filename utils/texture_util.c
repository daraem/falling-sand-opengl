#define GL_GLEXT_PROTOTYPES

#include "texture_util.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

unsigned char* load_image(char* path, int* width, int* height) {
	int channels;
	stbi_set_flip_vertically_on_load(true);
	
	unsigned char* data = stbi_load(path, width, height, &channels, 0);
	return data;
}

void free_image(unsigned char* data) {
	stbi_image_free(data);
}

void create_texture(unsigned int* texture, unsigned char* data, int width, int height, char* id, Shader sp, int bind_id) {
	glGenTextures(1, texture);
	glBindTexture(GL_TEXTURE_2D, *texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glUniform1i(glGetUniformLocation(sp.sp, id), bind_id);
}

void load_cubemap(unsigned int* texture, char* texture_arr[], int cubemap_len) {
	glGenTextures(1, texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, *texture);

	stbi_set_flip_vertically_on_load(false);
	int width, height, channels;
	for(int i = 0; i<cubemap_len; i++) {
		char* src = texture_arr[i];

		unsigned char* data = stbi_load(src, &width, &height, &channels, 0);
		if(!data) return;
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}
