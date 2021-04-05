#include "vec3.h"
#include <math.h>

void set_vec3(float e0, float e1, float e2, vec3 *v)
{
	v->e[0] = e0;
	v->e[1] = e1;
	v->e[2] = e2;
}
void set_length(vec3 *v)
{
	float x = v->e[0];
	float y = v->e[1];
	float z = v->e[2];

	v->length = sqrt(x*x + y*y + z*z);
}

void s_mult(float s, vec3 *v, vec3 *rv)
{
	rv->e[0] = v->e[0] * s;	
	rv->e[1] = v->e[1] * s;	
	rv->e[2] = v->e[2] * s;	
}

void s_div(float s, vec3 *v)
{
	v->e[0] = v->e[0] / s;
	v->e[1] = v->e[1] / s;
	v->e[2] = v->e[2] / s;
}

void sum(vec3 *result, vec3 *a, vec3 *b)
{
	result->e[0] = a->e[0] + b->e[0];
	result->e[1] = a->e[1] + b->e[1];
	result->e[2] = a->e[2] + b->e[2];
}

void diff(vec3 *result, vec3 *a, vec3 *b)
{
	result->e[0] = a->e[0] - b->e[0];
	result->e[1] = a->e[1] - b->e[1];
	result->e[2] = a->e[2] - b->e[2];
}

float dot(vec3 *a, vec3 *b)
{
	return (a->e[0] * b->e[0]) + (a->e[1] * b->e[1]) + (a->e[2] * b->e[2]);
}

void normalize(vec3 *v, vec3 *uv)
{
	uv->e[0] = v->e[0] / v->length;
	uv->e[1] = v->e[1] / v->length;
	uv->e[2] = v->e[2] / v->length;
}

void point_at_param(vec3 *result, vec3 *a, vec3 *b, float p)
{
	vec3 s_b;
	s_mult(p, b, &s_b); 	
	sum(result, a, &s_b);
}

