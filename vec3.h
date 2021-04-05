#ifndef VEC3_H_	// include guard
#define VEC3_H_

typedef struct vec3
{
	float e[3];
	float length;
}	vec3;

// function setters declarations 
void set_vec3(float e0, float e1, float e2, vec3 *v);
void set_length(vec3 *v);
void s_mult(float s, vec3 *v, vec3 *rv);
void s_div(float s, vec3 *v);
void sum(vec3 *result, vec3 *a, vec3 *b);
void diff(vec3 *result, vec3 *a, vec3 *b);
float dot(vec3 *a, vec3 *b);
void normalize(vec3 *v, vec3 *uv);

void point_at_param(vec3 *result, vec3 *a, vec3 *b, float p);

//float length(vec3 *v);

/*
float	x(void);
float	y(void);
float	z(void);
float	r(void);
float	g(void);
float	b(void);
*/



#endif // VEC3_H_
