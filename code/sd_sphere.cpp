#include <windows.h>
#include <math.h>
#include <stdio.h>

#include "sd_sphere.h"

float angle = 0;
float time = 0;

struct vec3 normalize(vec3 vector)
{
	vec3 normalized_vector;

	float x = vector.x;
	float y = vector.y;
	float z = vector.z;
	
	float magnitude = sqrt(x*x + y*y + z*z);

	normalized_vector.x = x / magnitude;
	normalized_vector.y = y / magnitude;
	normalized_vector.z = z / magnitude;

	return normalized_vector;
}

float length(vec3 vector)
{
	return sqrt(vector.x*vector.x  + vector.y*vector.y + vector.z*vector.z);
}

struct vec3 ray_direction(float field_of_view, vec2 uv)
{
	float z = 1.0 / tan(field_of_view / 2.0);	// Distance to image plane

	vec3 ray_direction;
	vec3 normalized_ray_direction;

	ray_direction.x = uv.x;
	ray_direction.y = uv.y;
	ray_direction.z = z;

	normalized_ray_direction = normalize(ray_direction);	

	return normalized_ray_direction;  
}

float sphere_sdf(Sphere *sphere, vec3 *point)
{
	float x = point->x - sphere->center.x;
	float y = point->y - sphere->center.y;
	float z = point->z - sphere->center.z;

	vec3 vector = {x, y, z};

	return length(vector) - sphere->radius;
}

float map_world(Sphere *sphere, vec3 *point)
{
	vec2 temp = {point->x, point->y};

	rotate(angle, &temp);

	point->x = temp.x;
	point->y = temp.y;

	float disp = distort_surface(2.0, *point); 
	float dist = sphere_sdf(sphere, point);

	return dist + disp;
}

float shortest_distance_to_surface(Sphere *sphere, vec3 eye_pos, vec3 march_dir, 
								   vec3 *point_in_space, float start, float end, float new_angle)
{
	float depth = start;	// starting point for ray march (0)
	float dist = 0.0;		
	//angle += 0.001;
	angle = new_angle; 
	//time += 0.0001;

	int i = 0;
	
	while (i < MAX_MARCHING_STEPS)
	{

		point_in_space->x = march_dir.x * depth;
		point_in_space->y = march_dir.y * depth;
		point_in_space->z = march_dir.z * depth;

		point_in_space->x = point_in_space->x + eye_pos.x;
		point_in_space->y = point_in_space->y + eye_pos.y;
		point_in_space->z = point_in_space->z + eye_pos.z;

		dist = map_world(sphere, point_in_space);

		if (dist < EPSILON)
		{
			return depth;
		}

		depth += dist;

		if (depth >= end)
		{
			return end;
		}

		i += 1;
	}

	return end;
}

struct vec3 estimate_normal(Sphere *sphere, vec3 point)
{
	vec3 grad_x = {point.x + EPSILON, point.y, point.z};
	vec3 grad_y = {point.x, point.y + EPSILON, point.z};
	vec3 grad_z = {point.x, point.y, point.z + EPSILON};

	vec3 grad_x2 = {point.x - EPSILON, point.y, point.z};
	vec3 grad_y2 = {point.x, point.y - EPSILON, point.z};
	vec3 grad_z2 = {point.x, point.y, point.z - EPSILON};

	float gradient_x = map_world(sphere, &grad_x) - map_world(sphere, &grad_x2);
	float gradient_y = map_world(sphere, &grad_y) - map_world(sphere, &grad_y2);
	float gradient_z = map_world(sphere, &grad_z) - map_world(sphere, &grad_z2);

	vec3 normal = {gradient_x, gradient_y, gradient_z};

	return normalize(normal);
}

float diffuse(vec3 current_pos, vec3 light_pos, vec3 normal)
{
	float x = light_pos.x - current_pos.x;
	float y = light_pos.y - current_pos.y;
	float z = light_pos.z - current_pos.z;

	vec3 light_dir = {x, y, z};
	vec3 normalized_light_dir = normalize(light_dir);

	float dot_product = (normal.x*normalized_light_dir.x) + 
						(normal.y*normalized_light_dir.y) + 
						(normal.z*normalized_light_dir.z);

	float intensity = max(0.0, dot_product);

	return intensity;
}

void rotate(float angle, vec2 *p)
{
	float c = cos(angle);
	float s = sin(angle);

	float x = p->x;
	float y = p->y;

	p->x = (x*c) - (y*s);
	p->y = (x*s) + (y*c);
}

float distort_surface(float freq, vec3 point)
{
	//return sin(freq*point.x) * sin(freq*point.y) * sin(freq*point.z);
	return sin(point.x) * sin(point.y) * sin(point.z);
}

/*
int main(void)
{
	// Define primitive shape 
	Sphere sphere = {0.0, 0.0, 15.0, 1.0};

	vec2 uv = {0.0, 0.0};	// place_holder for the time being 

	vec3 camera_position = {0.0, 0.0, 0.0};

	Ray ray = {camera_position, ray_direction(3.14 / 6.0, uv)};
	vec3 light_position = {0.0, 10.0, 0.0};

	float dist = shortest_distance_to_surface(&sphere, ray.origin, ray.direction, MIN_DIST, MAX_DIST);

	if (dist > MAX_DIST - EPSILON)
	{
		printf("%s\n", "Background");
	}

	else
	{
		printf("%s\n", "ItsAHit");
	}

	return 0;
}
*/



