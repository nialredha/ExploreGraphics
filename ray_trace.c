#include <stdio.h>
#include "vec3.h"
#include "ray.h"

vec3 color()
{
	vec3 unit_direction;
	vec3 ray_direction = direction();
	unit_vector(&ray_direction, &unit_direction);

	vec3 v2; set_vec3(1.0, 1.0, 1.0, &v2); set_vec3_length(&v2);
	vec3 v3; set_vec3(0.5, 0.7, 1.0, &v3); set_vec3_length(&v3); 
	
	float t = 0.5*(unit_direction.e[1] + 1.0);
	float it = 1.0 - t;	
	scalar_multiply(it, &v2);
	scalar_multiply(t, &v3);

	vec3 v4; 
	set_vec3(v2.e[0] + v3.e[0], v2.e[1] + v3.e[1], v2.e[2] + v3.e[2], &v4);
	set_vec3_length(&v4);

	fprintf("%f %f %f\n", v4.e[0], v4.e[1], v4.e[2]);

	return v4;
}

int main()
{
	FILE *fp;
	const char *filename = "out.ppm";
	fp = fopen(filename, "w");

	int nx = 200;
	int ny = 100;

	vec3 bottom_left; set_vec3(-2.0, -1.0, -1.0, &bottom_left);
					  set_vec3_length(&bottom_left);
	vec3 origin;      set_vec3(0.0, 0.0, 0.0, &origin);
					  set_vec3_length(&origin);
	vec3 horizontal;  set_vec3(4.0, 0.0, 0.0, &horizontal);
					  set_vec3_length(&horizontal);
	vec3 vertical;	  set_vec3(0.0, 2.0, 0.0, &vertical);
					  set_vec3_length(&vertical);

	float r, g, b;
	int ir, ig, ib;

	fprintf(fp, "P3\n%d %d \n255\n", nx, ny);

	for (int i = ny-1; i>=0; --i)
	{
		for (int j = 0; j < nx; ++j)
		{
			/*
			float r = (float) j / (float) nx;
			float g = (float) i / (float) ny;
			float b = 0.2;

			int ir = (int) 255.99*r;
			int ig = (int) 255.99*g;
			int ib = (int) 255.99*b;

			vec3 v1;
			set_vec3(u, v, 0.2, &v1);
			set_vec3_length(&v1);

			int ir = (int) 255.99*v1.e[0];
			int ig = (int) 255.99*v1.e[1];
			int ib = (int) 255.99*v1.e[2];
			*/

			float u = (float) j / (float) nx;
			float v = (float) i / (float) ny;

			/*
			horizontal.e[0] = u*horizontal.e[0];
			horizontal.e[1] = u*horizontal.e[1];
			horizontal.e[2] = u*horizontal.e[2];
			vertical.e[0] = v*vertical.e[0];
			vertical.e[1] = v*vertical.e[1];
			vertical.e[2] = v*vertical.e[2];
			*/

			scalar_multiply(u, &horizontal);
			scalar_multiply(v, &vertical);

			vec3 dir;
			dir.e[0] = bottom_left.e[0] + horizontal.e[0] + vertical.e[0];
			dir.e[1] = bottom_left.e[1] + horizontal.e[1] + vertical.e[1];
			dir.e[2] = bottom_left.e[2] + horizontal.e[2] + vertical.e[2];

			set_ray(&origin, &dir);

			vec3 col = color();

			int ir = (int) 255.99*col.e[0];
			int ig = (int) 255.99*col.e[1];
			int ib = (int) 255.99*col.e[2];

			fprintf(fp, "%d %d %d\n", ir, ig, ib);

			
		}
	}

	fclose(fp);
}



