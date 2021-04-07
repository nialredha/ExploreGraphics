#include <stdio.h>
#include <math.h>
#include "shape_collision.h"
#include "vec3.h"

bool sphere_collide(vec3 *orig, vec3 *dir, sphere *s, float t_min, float t_max)
{
	record_collision rec;
	vec3 oc;
	diff(&oc, orig, s.vec3.center);

	float a = dot(dir, dir);
	float b = 2.0 * dot(&oc, dir);
	float c = dot(&oc, &oc) - s->radius*s->radius;
	float discrim = b*b - 4.0*a*c;

	if (discrim > 0.0)
	{
		float temp = (-b - sqrt(discrim)) / 2.0*a;
		if (temp > t_min && temp < t_max)
		{
			rec.t = temp;
			point_at_param(&rec.vec3.p, orig, dir, &rec.t);
			diff(&rec.vec3.normal, &rec.vec3.p, s->vec3->center);
			s_div(s->radius, &rec.vec3.normal);
			return true;
		}
		float temp = (-b + sqrt(discrim)) / 2.0*a;
		if (temp > t_min && temp < t_max)
		{
			rec.t = temp;
			point_at_param(&rec.vec3.p, orig, dir, &rec.t);
			diff(&rec.vec3.normal, &rec.vec3.p, s->vec3->center);
			s_div(s->radius, &rec.vec3.normal);
			return true;
		}
	}
	return false;
}


