#include"lib.h"

#define GRID 400
#define CWIDTH WIDTH/GRID
#define CHEIGHT HEIGHT/GRID

#define BRUSH_SIZE 16

enum block_type {
	AIR, SAND, WATER	
};

Shader default_sp;

unsigned int cVAO;
unsigned int pVBO;

vec2 scale_vec;

int pixel_count;
vec2* pixel_occ_array;
vec3* pixel_pos_array;

int sand_ds;
int water_ds;
float gravity;

void opengl_setup() {
	glViewport(0, 0, WIDTH, HEIGHT);
	glClearColor(0.1, 0.1, 0.1, 1.0);
}

void shader_setup() {
	const char* dfs_s = load_shader("./shader/default.fs");
	const char* dvs_s = load_shader("./shader/default.vs");
	
	unsigned int dfs = create_shader(dfs_s, GL_FRAGMENT_SHADER);
	unsigned int dvs = create_shader(dvs_s, GL_VERTEX_SHADER);
	default_sp = create_program(dfs, dvs);
}

void geometry_setup() {
	unsigned int cVBO;

	glGenBuffers(1, &pVBO);
	glBindBuffer(GL_ARRAY_BUFFER, pVBO);
	glBufferData(GL_ARRAY_BUFFER, GRID * GRID * sizeof(vec3), NULL, GL_STATIC_DRAW);

	glGenBuffers(1, &cVBO);
	glGenVertexArrays(1, &cVAO);

	glBindVertexArray(cVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect_vertex_2d), rect_vertex_2d, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, pVBO);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexAttribDivisor(1, 1);  
}

void scale_2d(int w, int h, vec2 vec) {
	vec[0] = (float)w/WIDTH;
	vec[1] = (float)h/HEIGHT;
}

void screen_coords_2d(float x, float y, vec2 vec) {
	vec[0] = (float)((x/(WIDTH*0.5)) - 1);
	vec[1] = (float)(((y - HEIGHT)/(-HEIGHT*0.5)) - 1);
}

void mouse_to_grid(int x, int y, vec2 vec) {
	double w = CWIDTH;
	double h = CHEIGHT;
	vec[0] = trunc(x/(CWIDTH));
	vec[1] = trunc(y/(CHEIGHT));
}

void click_event(int x, int y, int type) {
	vec2 vec;
	mouse_to_grid(x, y, vec);

	for(int i = 0; i<BRUSH_SIZE; i++) {
		for(int j = 0; j<BRUSH_SIZE; j++) {
			int stride = (GRID * (vec[1] + i)) + (vec[0] + j);
			if(pixel_occ_array[stride][0] == 1 || (vec[0] + j > (GRID - 1) || vec[0] + j < 0)) return;
			pixel_occ_array[stride][0] = type == 1 ? SAND : WATER;
			pixel_occ_array[stride][1] = 1.;
			pixel_count++;
		}
	}

}

void* setup() {
	srand(time(0));
	opengl_setup();
	geometry_setup();
	shader_setup();

	sand_ds = 2;
	water_ds = 3;
	gravity = 0.5;

	pixel_count = 0;
	pixel_occ_array = calloc((GRID * GRID), sizeof(vec2));
	pixel_pos_array = malloc((GRID * GRID) * sizeof(vec3));

	scale_2d(CWIDTH, CHEIGHT, scale_vec);
}

void game_logic(int row, int col) {
	int rand_dir = -1 + ( 2 * (rand() % 2));
	int stride = ((GRID * (row + 0)) + (col + 0));
	if(pixel_occ_array[stride][0] == 0) return;
	if(row == GRID - 1) return;

	float mv_grav = 0;
	int mv = 0;
	float temp_g;

	switch((int)pixel_occ_array[stride][0]) {
		case SAND:
			switch((int)pixel_occ_array[(GRID * (row + 1)) + col][0]) {
				case SAND:
					if(pixel_occ_array[(GRID) * (row + 1) + col + rand_dir][0] == 0) {
						if(rand_dir == 1) {
							if(col < GRID - 1) {
								pixel_occ_array[stride][0] = 0;
								mv = 0; 
								for(int i = 1; i<sand_ds+1; i++) {
									if(pixel_occ_array[GRID * (row + i) + col + i][0] != 0) break;
									if(col + i > GRID - 1) break;
									mv++;
								}
								pixel_occ_array[(GRID * (row + mv)) + col + mv][0] = SAND;
								pixel_occ_array[(GRID * (row + mv)) + col + mv][1] = pixel_occ_array[stride][1];
								pixel_occ_array[stride][1] = 1.;
							}
						} else {
							if(col > 0) {
								pixel_occ_array[stride][0] = 0;
								mv = 0;
								for(int i = 1; i<sand_ds+1; i++) {
									if(pixel_occ_array[GRID * (row + i) + col - i][0] != 0) break;
									if(col - i <= 0) break;
									mv++;
								}
								pixel_occ_array[((GRID) * (row + mv)) + col - mv][0] = SAND;
								pixel_occ_array[((GRID) * (row + mv)) + col - mv][1] = pixel_occ_array[stride][1];
								pixel_occ_array[stride][1] = 1.;
							}
						}
					}
					return;
				case WATER:
					if(pixel_occ_array[(GRID * (row + 1)) + col + 1][0] == 0 && col < GRID - 1 ) {
						pixel_occ_array[stride][0] = 0;
						mv = 0;
						for(int i = 1; i<sand_ds+1; i++) {
							if(pixel_occ_array[GRID * (row + i) + col + i][0] != 0) break;
							if(col + i > GRID - 1) break;
							mv++;
						}
						pixel_occ_array[(GRID * (row + mv)) + col + mv][0] = SAND;
					} else if(pixel_occ_array[(GRID * (row + 1)) + col - 1][0] == 0 && col > 0) {
						pixel_occ_array[stride][0] = 0;
						pixel_occ_array[((GRID) * (row + 1)) + col - 1][0] = SAND;
					} else {
						pixel_occ_array[stride][0] = WATER;
						pixel_occ_array[GRID * (row + 1) + col][0] = SAND;
					}

					return;
				default:
					break;
			}
			if(pixel_occ_array[GRID * (row + 1) + col][0] != 0) return;
			
			mv = 0;
			mv_grav = 0;

			pixel_occ_array[stride][0] = AIR;
			temp_g = pixel_occ_array[stride][1];	
			pixel_occ_array[stride][1] = 1.;
			for(int i = 1; i<trunc(temp_g) + 2; i++) {
				if(row + i > GRID - 1) break;
				if(pixel_occ_array[GRID * (row + i) + col][0] != 0) break;
				mv_grav += gravity;
			}
			mv = trunc(mv_grav) + 1;
			pixel_occ_array[GRID * (row + mv) + col][0] = SAND;
			pixel_occ_array[GRID * (row + mv) + col][1] = temp_g + mv_grav;
			break;
		case WATER:
			if(pixel_occ_array[(GRID * (row + 1)) + col][0] != 0) {
				if(pixel_occ_array[GRID * (row + 1) + col + rand_dir][0] == 0) {
					pixel_occ_array[stride][0] = 0;
					mv = 0;
					for(int i = 1; i<water_ds; i++) {
						if(pixel_occ_array[GRID * (row + i) + col + (i * rand_dir)][0] != 0) break;
						if(col + (i * rand_dir) > GRID - 1 || col + (i * rand_dir) <= 0) break;
						mv++;
					}
					pixel_occ_array[GRID * (row + mv) + col + (mv * rand_dir)][0] = WATER;
					pixel_occ_array[GRID * (row + mv) + col + (mv * rand_dir)][1] = pixel_occ_array[stride][1];
					pixel_occ_array[stride][1] = 1.;
				} else if(pixel_occ_array[stride + 1][0] == 0) {
					pixel_occ_array[stride][0] = 0;
					mv = 0;
					for(int i = 1; i<water_ds+1; i++) {
						if(pixel_occ_array[stride + i][0] != 0) break;
						if(col + i > GRID - 1) break;
						mv++;
					}
					pixel_occ_array[(GRID * row) + col + mv][0] = WATER;
					pixel_occ_array[(GRID * row) + col + mv][1] = pixel_occ_array[stride][1];
					pixel_occ_array[stride][1] = 1.; 
				} else if(pixel_occ_array[stride - 1][0] == 0) {
					pixel_occ_array[stride][0] = 0;
					mv = 0;
					for(int i = 1; i<water_ds+1; i++) {
						if(pixel_occ_array[stride - i][0] != 0) break;
						if(col - i <= 0) break;
						mv++;
					}
					pixel_occ_array[(GRID * row) + col - mv][0] = WATER;
					pixel_occ_array[(GRID * row) + col - mv][1] = pixel_occ_array[stride][1];
					pixel_occ_array[stride][1] = 1.;
				}
				return;
			}
			pixel_occ_array[stride][0] = AIR;
			mv = 0;
			mv_grav = 0;
			temp_g = pixel_occ_array[stride][1];
			pixel_occ_array[stride][1] = 1.;

			for(int i = 1; i<trunc(temp_g) + 2; i++) {
				if(row + i > GRID - 1) break;
				if(pixel_occ_array[GRID * (row + i) + col][0] != 0) break;
				mv_grav += gravity;
			}
			mv = trunc(mv_grav) + 1;
			pixel_occ_array[GRID * (row + mv) + col][0] = WATER;
			pixel_occ_array[GRID * (row + mv) + col][1] = temp_g + mv_grav;
	}
}

void update_pos() {
	for(int row = GRID - 1; row > 0 ; row--) {
		int rand_pass = rand() % 2;
		if(rand_pass == 0) {
			for(int col = 0; col<GRID - 1; col++) {
				game_logic(row, col);
			}
		} else {
			for(int col = GRID - 1; col > 0; col--) {
				game_logic(row, col);
			}
		}

	}
}

void render(Shader s, double frame) {
	int arr_offset = 0;
	for(int row = 0; row<GRID; row++) {
		for(int col = 0; col<GRID; col++) {
			int stride = GRID * row + col;
			if(pixel_occ_array[stride][0] == 0) continue;
			vec2 pos;
			screen_coords_2d(col * (CWIDTH), row * (CHEIGHT), pos);
			vec3 pixel = {pos[0], pos[1], pixel_occ_array[stride][0]};
			
			pixel_pos_array[arr_offset][0] = pixel[0];
			pixel_pos_array[arr_offset][1] = pixel[1];
			pixel_pos_array[arr_offset][2] = pixel[2];

			arr_offset++;
		}
	}
	mat4 base = GLM_MAT4_IDENTITY_INIT;
	shader_bind_4m(default_sp, "base_mat", base);
	shader_bind_1f(default_sp, "scale_factor", scale_vec[0]);

	glBindBuffer(GL_ARRAY_BUFFER, pVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, arr_offset * sizeof(vec3), &pixel_pos_array[0]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexAttribDivisor(1, 1);  

	glBindVertexArray(cVAO);
	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, arr_offset);

	/*
	for(int row = 0; row<GRID; row++) {
		for(int col = 0; col<GRID; col++) {
			int stride = (int)(GRID * (row + 0) + (col + 0));
			if(pixel_occ_array[stride] == 0) continue;
			mat4 trans_c = GLM_MAT4_IDENTITY_INIT;
			vec2 pos;
			screen_coords_2d(col * (CWIDTH), row * (CHEIGHT), pos);
		
			glm_translate(trans_c, (vec3){pos[0], pos[1], 0.0
			glm_scale(trans_c, (vec3){scale_vec[0], scale_vec[1], 1.});
			
			shader_bind_4m(default_sp, "trans", trans_c);
			shader_bind_1f(default_sp, "type", pixel_occ_array[stride]);

			glBindVertexArray(cVAO);
			glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 2);

		}

	}
	*/

}

void* draw(Mouse mouse, double frame) {
	glClear(GL_COLOR_BUFFER_BIT);
	
	use_program(default_sp);

	if(mouse.pressed != -1) click_event(mouse.x, mouse.y, mouse.pressed);
	update_pos();
	render(default_sp, frame);

	glFlush();
}

void* free_game() {
	free(pixel_occ_array);
	free(pixel_pos_array);
}
