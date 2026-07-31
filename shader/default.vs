#version 330 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 pData;

out float pixel_type;

uniform mat4 base_mat;
uniform float scale_factor;

void main() {
	pixel_type = pData.z;
	
	vec3 offset = vec3(pData.x, pData.y, 0.0);
	
	mat4 translate_mat = mat4(
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		offset.x, offset.y, offset.z, 1.0
	);
	    
	mat4 scale_mat = mat4(
		scale_factor, 0.0, 0.0, 0.0,
		0.0, scale_factor, 0.0, 0.0,
		0.0, 0.0, scale_factor, 0.0,
		0.0, 0.0, 0.0, 1.0
	);
	    
	mat4 model_mat = base_mat * translate_mat * scale_mat;

	gl_Position = model_mat * vec4(vPos, 1.0);
}
