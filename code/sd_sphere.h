#ifndef SD_SPHERE_H_	// include guard
#define SD_SPHERE_H_

const int MAX_MARCHING_STEPS = 500;	// Max steps you are willing to search for a "hit"
const float MAX_DIST = 1000.0;		// Max distance to search for a "hit"
const float MIN_DIST = 0.0;			// Where to start the march
const float EPSILON = 0.001;		// How close you need to be to be considered a "hit"

struct vec3 
{
	float x;
	float y;
	float z;
};

struct vec2
{
	float x;
	float y;
};

struct Ray
{
	vec3 origin;
	vec3 direction;
};

struct Sphere
{
	vec3 center;
	float radius;
};

struct vec3 normalize(vec3 vector);
float length(vec3 vector);
struct vec3 ray_direction(float field_of_view, vec2 uv);
float sphere_sdf(Sphere *sphere, vec3 *point);
float map_world(Sphere *sphere, vec3 *point);
float shortest_distance_to_surface(Sphere *sphere, vec3 eye_pos, vec3 march_dir, 
								   vec3 *point_in_space, float start, float end, float new_angle);
struct vec3 estimate_normal(Sphere *sphere, vec3 point);
float diffuse(vec3 current_pos, vec3 light_pos, vec3 normal);
void rotate(float angle, vec2 *p);
float distort_surface(float freq, vec3 point);


#endif	// SD_SPHERE_H_
