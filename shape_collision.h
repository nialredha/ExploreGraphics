#ifndef SHAPE_COLLISION_H_	// include guard
#define SHAPE_COLLISION_H_

#include "vec3.h"

typedef struct sphere 
{
	vec3 center;
	float radius;
}	sphere;

typedef struct record_collision
{
	float t;
	vec3 p;
	vec3 normal;
}	record_collision;


bool sphere_collide(vec3 *orig, vec3 *dir, float t_min, float t_max);


#endif //SHAPE_COLLISION_H_
