#ifndef VEC3_H_	// include guard
#define VEC3_H_

typedef struct vec3
{
	float e[3];
	float length;
}	vec3;

// function setters declarations 
void set_vec3(float e0, float e1, float e2, vec3 *v);
void set_vec3_length(vec3 *v);
void scalar_multiply(float s, vec3 *v);
void scalar_divide(float s, vec3 *v);
void unit_vector(vec3 *v, vec3 *uv);

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
