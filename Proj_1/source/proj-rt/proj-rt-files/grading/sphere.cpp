#include "sphere.h"
#include "ray.h"
#include <math.h>

// Determine if the ray intersects with the sphere
Hit Sphere::Intersection(const Ray& ray, int part) const
{
    vec3 u = ray.direction;
    vec3 v = ray.endpoint - center;
    
    float a = dot(u,u);
    float b = 2 * dot(u,v);
    float c = dot(v,v) - (radius * radius);

    double discr = (b * b) - (4 * a * c);
    float t_1 = (-b - sqrt(discr)) / (2 * a);
    float t_2 = (-b + sqrt(discr)) / (2 * a);

    Hit hit_1, hit_2;
    
    if(discr > 0) {
	if(t_1 > 0) {
		hit_1.object = this;
		hit_1.dist = t_1;
		return hit_1;
	}
	else {
		hit_1.dist = 0;

		hit_2.object = this;
		hit_2.dist = t_2;
		return hit_2;
        }
    }
    else if(discr == 0) {
	hit_1.object = this;
	hit_1.dist = t_1;
	return hit_1;
    }
    return {0,0,0};
}

vec3 Sphere::Normal(const vec3& point, int part) const
{
    vec3 normal;
    TODO; // compute the normal direction
    return normal;
}

Box Sphere::Bounding_Box(int part) const
{
    Box box;
    TODO; // calculate bounding box
    return box;
}
