#include <stdio.h>
#include <math.h>
#include "vec3.h"

vec3 color(vec3 *orig, vec3 *dir);
float hit_sphere(vec3 *center, float radius, vec3 *orig, vec3 *dir); 

int main()
{
	FILE *fp;
	const char *filename = "out.ppm";
	fp = fopen(filename, "w");

	int nx = 200;
	int ny = 100;

	vec3 bottom_left; set_vec3(-2.0, -1.0, -1.0, &bottom_left);
					  set_length(&bottom_left);
	vec3 origin;      set_vec3(0.0, 0.0, 0.0, &origin);
					  set_length(&origin);
	vec3 horizontal;  set_vec3(4.0, 0.0, 0.0, &horizontal);
					  set_length(&horizontal);
	vec3 vertical;	  set_vec3(0.0, 2.0, 0.0, &vertical);
					  set_length(&vertical);
	vec3 s_horiz;
	vec3 s_vert;
	
	float r, g, b;
	int ir, ig, ib;

	fprintf(fp, "P3\n%d %d \n255\n", nx, ny);

	for (int i = ny-1; i>=0; --i)
	{
		for (int j = 0; j < nx; ++j)
		{
			float u = (float) j / (float) nx;
			float v = (float) i / (float) ny;

			s_mult(u, &horizontal, &s_horiz);
			s_mult(v, &vertical, &s_vert);

			vec3 dir;
			sum(&dir, &bottom_left, &s_horiz);
			sum(&dir, &dir, &s_vert);
			set_length(&dir);

			vec3 col = color(&origin, &dir);

			int ir = (int) 255.99*col.e[0];
			int ig = (int) 255.99*col.e[1];
			int ib = (int) 255.99*col.e[2];

			fprintf(fp, "%d %d %d\n", ir, ig, ib);
		}
	}

	fclose(fp);
}

vec3 color(vec3 *orig, vec3 *dir)
{
	vec3 center; set_vec3(0, 0, -1, &center); set_length(&center);
	//vec3 red; set_vec3(1, 0, 0, &red); set_length(&red);

	float t = hit_sphere(&center, 0.5, orig, dir);
	if (t > 0.0)
	{
		vec3 point; point_at_param(&point, orig, dir, t);
		vec3 ray; diff(&ray, &point, &center); set_length(&ray);
		vec3 uray; normalize(&ray, &uray);	
		vec3 temp; set_vec3(uray.e[0]+1, uray.e[1]+1, uray.e[2]+1, &temp);
		vec3 color; s_mult(0.5, &temp, &color);
		return color;
	}

	vec3 unit_direction;
	vec3 ray_direction = *dir; 
	normalize(&ray_direction, &unit_direction);

	vec3 v2; set_vec3(1.0, 1.0, 1.0, &v2); set_length(&v2);
	vec3 v3; set_vec3(0.5, 0.7, 1.0, &v3); set_length(&v3); 
	vec3 s_v2;
	vec3 s_v3;
	
	t = 0.5*(unit_direction.e[1] + 1.0);
	float it = 1.0 - t;	

	s_mult(it, &v2, &s_v2);
	s_mult(t, &v3, &s_v3);

	vec3 v4; 
	sum(&v4, &s_v2, &s_v3);
	set_length(&v4);

	return v4;
}

float hit_sphere(vec3 *center, float radius, vec3 *orig, vec3 *dir)
{
	vec3 oc;
	diff(&oc, orig, center);

	float a = dot(dir, dir);
	float b = 2.0 * dot(&oc, dir);
	float c = dot(&oc, &oc) - radius*radius;
	float discrim = b*b - 4.0*a*c;

	if (discrim < 0.0)
	{
		return -1.0;
	} 
	else 
	{
		return (-b - sqrt(discrim)) / 2.0*a;
	}
}
