#ifndef RAY_H_	// include guard
#define RAY_H_

#include "vec3.h"

vec3 orig;
vec3 dir;

// function setters
void set_ray(vec3 *a, vec3 *b);

// function getters 
vec3 origin();
vec3 direction();
vec3 point_at_parameter(float p); 

#endif // RAY_H_
