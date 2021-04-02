#include "vec3.h"
#include <math.h>

void set_vec3(float e0, float e1, float e2, vec3 *v)
{
	v->e[0] = e0;
	v->e[1] = e1;
	v->e[2] = e2;
}
void set_vec3_length(vec3 *v)
{
	float x = v->e[0];
	float y = v->e[1];
	float z = v->e[2];

	v->length = sqrt(x*x + y*y + z*z);
}

void scalar_multiply(float s, vec3 *v)
{
	v->e[0] = v->e[0] * s;	
	v->e[1] = v->e[1] * s;	
	v->e[2] = v->e[2] * s;	
}

void scalar_divide(float s, vec3 *v)
{
	v->e[0] = v->e[0] / s;
	v->e[1] = v->e[1] / s;
	v->e[2] = v->e[2] / s;
}

void unit_vector(vec3 *v, vec3 *uv)
{
	uv->e[0] = v->e[0] / v->length;
	uv->e[1] = v->e[1] / v->length;
	uv->e[2] = v->e[2] / v->length;
}

/*
float length(vec3 *v)
{
	float x = v->e[0];
	float y = v->e[1];
	float z = v->e[3];

	return sqrt(x*x + y*y + z*z);
}
*/




