#ifndef RAY_H_	// include guard
#define RAY_H_

#include "vec3.h"

// function setters
void set_ray(vec3 *a, vec3 *b);

// function getters 
vec3 orig	origin();
vec3 dir	direction();
vec3 point  point_at_parameter(float p); 

#endif // RAY_H_
