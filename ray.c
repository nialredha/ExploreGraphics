#include "ray.h"
#include "vec3.h"

void set_ray(vec3 *a, vec3 *b)
{
	vec3 orig = *a;	
	vec3 dir = *b;
}

vec3 origin()
{
	return orig; 
}

vec3 direction()
{
	return dir;
}

vec3 point_at_parameter(float p)
{
	return orig + p*dir;
}


