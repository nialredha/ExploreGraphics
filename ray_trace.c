#include <stdio.h>
#include "vec3.h"

int main()
{
	FILE *fp;
	const char *filename = "out.ppm";
	fp = fopen(filename, "w");

	int nx = 200;
	int ny = 100;

	vec3 bottom_left; set_vec3(-2.0, -1.0, -1.0, &bottom_left);
	vec3 origin;      set_vec3(0.0, 0.0, 0.0, &origin);
	vec3 horizontal;  set_vec3(4.0, 0.0, 0.0, &horizontal);
	vec3 vertical;	  set_vec3(0.0, 2.0, 0.0, &vertical);

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
			*/

			float u = (float) j / (float) nx;
			float v = (float) i / (float) ny;

			vec3 v1;
			set_vec3(u, v, 0.2, &v1);

			int ir = (int) 255.99*v1.e[0];
			int ig = (int) 255.99*v1.e[1];
			int ib = (int) 255.99*v1.e[2];
			
			fprintf(fp, "%d %d %d\n", ir, ig, ib);
		}
	}

	fclose(fp);
}



