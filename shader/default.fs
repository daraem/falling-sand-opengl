#version 330 core

out vec4 FragColor;

in float pixel_type;

vec3 get_color() {
	if(pixel_type == 1) {
		return vec3(0.79, 0.74, 0.57);
	} else {
		return vec3(0.0, 0.0, 1.0);
	}
}

void main() {
	vec3 col = get_color();
	FragColor = vec4(col, 1.0);
}
