#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "vec3.h"

vec3 color(vec3 *orig, vec3 *dir, vec3 *center, float radius, int flag);
float hit_sphere(vec3 *center, float radius, vec3 *orig, vec3 *dir); 

int main()
{
	FILE *fp;
	const char *filename = "out.ppm";
	fp = fopen(filename, "w");

	int nx = 200;
	int ny = 100;
	int ns = 100;

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
			vec3 col; set_vec3(0, 0, 0, &col);
			vec3 col2; set_vec3(0, 0, 0, &col2);

			for (int s=0; s<ns; ++s)
			{
				
				float u = ((float) (j + (rand()/((double) RAND_MAX)))) / (float) nx;
				float v = ((float) (i + (rand()/((double) RAND_MAX)))) / (float) ny;

				s_mult(u, &horizontal, &s_horiz);
				s_mult(v, &vertical, &s_vert);

				vec3 dir;
				sum(&dir, &bottom_left, &s_horiz);
				sum(&dir, &dir, &s_vert);
				diff(&dir, &dir, &origin);
				set_length(&dir);

				vec3 center1; set_vec3(0, -100.5, -1, &center1); set_length(&center1);
				vec3 center2; set_vec3(0, 0, -1, &center2); set_length(&center2);
				vec3 val;
			
				val = color(&origin, &dir, &center1, 100, 1);
				plus_eq(&col, &val);
				val = color(&origin, &dir, &center2, 0.5, 0);
				plus_eq(&col2, &val);
			}

			s_div_eq(&col, (float) ns);
			s_div_eq(&col2, (float) ns);

			int ir, ig, ib;

			/*
			ir = (int) 255.99*col2.e[0]; 
			ig = (int) 255.99*col2.e[1];
			ib = (int) 255.99*col2.e[2];
			*/

			if (col2.length < 0.0)
			{
				ir = (int) 255.99*col.e[0]; 
				ig = (int) 255.99*col.e[1];
				ib = (int) 255.99*col.e[2];
			}
			else 
			{
				ir = (int) 255.99*col2.e[0]; 
				ig = (int) 255.99*col2.e[1];
				ib = (int) 255.99*col2.e[2];
			}

			fprintf(fp, "%d %d %d\n", ir, ig, ib);
		}
	}

	fclose(fp);
}

vec3 color(vec3 *orig, vec3 *dir, vec3 *center, float radius, int flag)
{
	//vec3 red; set_vec3(1, 0, 0, &red); set_length(&red);

	float t = hit_sphere(center, radius, orig, dir);

	if (t > 0.0)
	{
		vec3 point; point_at_param(&point, orig, dir, t);
		vec3 ray; diff(&ray, &point, center); set_length(&ray);
		vec3 uray; normalize(&ray, &uray);	
		vec3 temp; set_vec3(uray.e[0]+1, uray.e[1]+1, uray.e[2]+1, &temp);
		vec3 color; s_mult(0.5, &temp, &color); set_length(&color);
		return color;
	}
	
	if (flag == 0)
	{
		vec3 fake; fake.length = -1.0;
		return fake;
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
