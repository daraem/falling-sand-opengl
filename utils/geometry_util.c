#include "geometry_util.h"

void circle(int detail_f, vec3* arr) {
	for(int i = 0; i<detail_f; i++) {
		float d = (360/detail_f) * i;
		float x = 1 * cos(glm_rad(d));
		float y = 1 * sin(glm_rad(d));
		glm_vec3((float[4]){x, y, 0.0, 0.0}, arr[i]);
	}
	//glm_vec3((float[4]){arr[0][0], arr[0][1], 0.0, 0.0}, arr[detail_f-1]);
}

void sphere(int detail_f, vec3* arr) {
	float delta = 360./detail_f;
	for(int i = 0; i<detail_f; i++) {
		for(int j = 0; j<detail_f; j++) {
			float theta = glm_rad(delta * i);
			float phi = glm_rad(delta * j);
			float x = sin(theta) * cos(phi);
			float y = sin(theta) * sin(phi); 
			float z = cos(theta);
			glm_vec3((float[4]){x, y, z, 0.0}, arr[detail_f * i + j]);
		}
	}
}

void pyramid_nbase(int base, float height, vec3* arr) {
	float delta = 360./base;
	
	vec3 hpoint = {0.0, height, 0.0};

	int offset = 0;
	for(int i = 0; i<base; i++) {
		float phi = glm_rad(delta * i);
		float phi_next = glm_rad(delta * (i+1));

		float x = cos(phi);
		float y = 0.0; 
		float z = sin(phi);

		float x_n = cos(phi_next);
		float z_n = sin(phi_next);

		vec3 n_1, n_2;

		glm_vec3_sub(hpoint, (vec3){x, y, z}, n_1);
		glm_vec3_sub(hpoint, (vec3){x_n, y, z_n}, n_2);

		vec3 n;
		glm_vec3_crossn(n_1, n_2, n);

		int b = offset * 6;

		arr[b + 0][0] = hpoint[0];
		arr[b + 0][1] = hpoint[1];
		arr[b + 0][2] = hpoint[2];
		
		arr[b + 1][1] = -n[0];
		arr[b + 1][2] = -n[1];
		arr[b + 1][3] = -n[2];

		arr[b + 2][0] = x;
		arr[b + 2][1] = y;
		arr[b + 2][2] = z;
	
		arr[b + 3][1] = -n[0];
		arr[b + 3][2] = -n[1];
		arr[b + 3][3] = -n[2];

		arr[b + 4][0] = x_n;
		arr[b + 4][1] = y;
		arr[b + 4][2] = z_n;

		arr[b + 5][1] = -n[0];
		arr[b + 5][2] = -n[1];
		arr[b + 5][3] = -n[2];

		offset++;
	}
}

void sphere_test(int detail_f, vec3* arr) {

	float delta_lat = 180./detail_f;
	float delta_lon = 360./detail_f;
	
	int offset = 0;
	for(int row = 0; row<detail_f; row++) {
		float lat_1 = glm_rad(delta_lat * row);
		float lat_2 = glm_rad(delta_lat * (row+1));

		for(int col = 0; col<detail_f; col++) {
			float lon_1 = glm_rad(delta_lon * col);
			float lon_2 = glm_rad(delta_lon * (col + 1));

			// Punto 1
			float x = sin(lat_1) * cos(lon_1);
			float y = sin(lat_1) * sin(lon_1);
			float z = cos(lat_1);
			// Punto 2
			float x_2 = sin(lat_2) * cos(lon_1);
			float y_2 = sin(lat_2) * sin(lon_1);
			float z_2 = cos(lat_2);
			// Punto 3
			float x_3 = sin(lat_2) * cos(lon_2);
			float y_3 = sin(lat_2) * sin(lon_2); 
			float z_3 = cos(lat_2); 
			// Punto 4
			float x_4 = sin(lat_1) * cos(lon_2); 
			float y_4 = sin(lat_1) * sin(lon_2); 
			float z_4 = cos(lat_1); 
			
			vec3 n_1, n_2;

			glm_vec3_sub((vec3){x_2, y_2, z_2}, (vec3){x, y, z}, n_1);
			glm_vec3_sub((vec3){x_4, y_4, z_4}, (vec3){x_3, y_3, z_3}, n_2);

			vec3 n;
			glm_vec3_crossn(n_1, n_2, n);

			int b_i = 12 * offset;
			// 1 2 3
			
			arr[b_i + 0][0] = x;
			arr[b_i + 0][1] = y;
			arr[b_i + 0][2] = z;
		
			arr[b_i + 1][0] = -n[0];
			arr[b_i + 1][1] = -n[1];
			arr[b_i + 1][2] = -n[2];

			arr[b_i + 2][0] = x_2;
			arr[b_i + 2][1] = y_2;
			arr[b_i + 2][2] = z_2;

			arr[b_i + 3][0] = -n[0];
			arr[b_i + 3][1] = -n[1];
			arr[b_i + 3][2] = -n[2];

			arr[b_i + 4][0] = x_3;
			arr[b_i + 4][1] = y_3;
			arr[b_i + 4][2] = z_3;

			arr[b_i + 5][0] = -n[0];
			arr[b_i + 5][1] = -n[1];
			arr[b_i + 5][2] = -n[2];

			// 1 4 3

			arr[b_i + 6][0] = x_3;
			arr[b_i + 6][1] = y_3;
			arr[b_i + 6][2] = z_3;

			arr[b_i + 7][0] = -n[0];
			arr[b_i + 7][1] = -n[1];
			arr[b_i + 7][2] = -n[2];

			arr[b_i + 8][0] = x_4;
			arr[b_i + 8][1] = y_4;
			arr[b_i + 8][2] = z_4;

			arr[b_i + 9][0] = -n[0];
			arr[b_i + 9][1] = -n[1];
			arr[b_i + 9][2] = -n[2];

			arr[b_i + 10][0] = x;
			arr[b_i + 10][1] = y;
			arr[b_i + 10][2] = z;

			arr[b_i + 11][0] = -n[0];
			arr[b_i + 11][1] = -n[1];
			arr[b_i + 11][2] = -n[2];

			offset += 1;
		}
	}
}

void plane(int width, int div, vec3 arr[]) {
	float interval = (float)width/div;
	int offset = 0;
	for(int row = 0; row < div; row++) {
		for(int col = 0; col<div; col++) {
			int b_i = offset * 18;

			vec3 p1_2 = {0.0, 0.0, (interval * -(row+1)) - (interval * -row)};
			vec3 p1_3 = {(interval * (col+1)) - (interval * col), 0.0, (interval * -(row+1)) - (interval * -row)};

			vec3 norm = {(p1_2[1] * p1_3[2]) - (p1_2[2] * p1_3[1]), 
				     (p1_2[2] * p1_3[0]) - (p1_2[0] * p1_3[2]), 
				     (p1_2[0] * p1_3[1]) - (p1_2[1] * p1_3[0])
			};

			arr[b_i + 0][0] = (interval * col);
			arr[b_i + 0][1] = 0.0;
			arr[b_i + 0][2] = (interval * -row);
		
			arr[b_i + 1][0] = -norm[0]; 
			arr[b_i + 1][1] = -norm[1];
			arr[b_i + 1][2] = -norm[2];

			arr[b_i + 2][0] = (float)col/div; 
			arr[b_i + 2][1] = (float)row/div;
			arr[b_i + 2][2] = 0.0;

			arr[b_i + 3][0] = (interval * col);
			arr[b_i + 3][1] = 0.0;
			arr[b_i + 3][2] = (interval * -(row+1));	

			arr[b_i + 4][0] = -norm[0]; 
			arr[b_i + 4][1] = -norm[1];
			arr[b_i + 4][2] = -norm[2];

			arr[b_i + 5][0] = (float)col/div; 
			arr[b_i + 5][1] = (float)(row+1)/div;
			arr[b_i + 5][2] = 0.0;

			arr[b_i + 6][0] = (interval * (col+1));
			arr[b_i + 6][1] = 0.0;
			arr[b_i + 6][2] = (interval * -(row+1));	

			arr[b_i + 7][0] = -norm[0]; 
			arr[b_i + 7][1] = -norm[1];
			arr[b_i + 7][2] = -norm[2];

			arr[b_i + 8][0] = (float)(col+1)/div; 
			arr[b_i + 8][1] = (float)(row+1)/div;
			arr[b_i + 8][2] = 0.0;

			arr[b_i + 9][0] = (interval * (col+1));
			arr[b_i + 9][1] = 0.0;
			arr[b_i + 9][2] = (interval * -(row+1));
		
			arr[b_i + 10][0] = -norm[0]; 
			arr[b_i + 10][1] = -norm[1];
			arr[b_i + 10][2] = -norm[2];

			arr[b_i + 11][0] = (float)(col+1)/div; 
			arr[b_i + 11][1] = (float)(row+1)/div;
			arr[b_i + 11][2] = 0.0;

			arr[b_i + 12][0] = (interval * (col+1));
			arr[b_i + 12][1] = 0.0;
			arr[b_i + 12][2] = (interval * -row);
		
			arr[b_i + 13][0] = -norm[0]; 
			arr[b_i + 13][1] = -norm[1];
			arr[b_i + 13][2] = -norm[2];

			arr[b_i + 14][0] = (float)(col+1)/div; 
			arr[b_i + 14][1] = (float)row/div;
			arr[b_i + 14][2] = 0.0;

			arr[b_i + 15][0] = (interval * col);
			arr[b_i + 15][1] = 0.0;
			arr[b_i + 15][2] = (interval * -row);

			arr[b_i + 16][0] = -norm[0]; 
			arr[b_i + 16][1] = -norm[1];
			arr[b_i + 16][2] = -norm[2];

			arr[b_i + 17][0] = (float)col/div;
			arr[b_i + 17][1] = (float)row/div;
			arr[b_i + 17][2] = 0.0;

			offset++;
		}
	}
} 
