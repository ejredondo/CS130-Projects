#include "light.h"
#include "phong_shader.h"
#include "ray.h"
#include "render_world.h"
#include "object.h"

vec3 Phong_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& normal,int recursion_depth) const
{
    vec3 color, color_A, color_D, color_S;
    color_A = color_A + this->world.ambient_color * this->world.ambient_intensity * color_ambient;

    for(unsigned i = 0; i < this->world.lights.size(); ++i) {
	//Diffuse
	vec3 light_source = this->world.lights[i]->position;
	vec3 light_to_intersection = intersection_point - light_source;
	vec3 l = - light_to_intersection;
	l = l.normalized();
	vec3 R_d = color_diffuse;
	vec3 L = this->world.lights[i]->Emitted_Light(l);
	
	float light_decay = light_to_intersection.magnitude_squared();
	L = L / light_decay;
	vec3 n = normal;
	n = n.normalized();
	
	float light_normal;
	if(dot(n, l) < 0) {
		light_normal = 0;
	}
	else {
		light_normal = dot(n, l);
	}
	
	color_D = color_D + R_d * L * light_normal;

	//Specular
	//vec3 R_s = color_specular;
	//vec3 R = l - n * (2 * dot(n, l));
	//R = R.normalized();

	//vec3 C = intersection_point - ray.endpoint;
	//C = C.normalized();

	//float R_C;
	//if(dot(R, C) < 0) {
	//	R_C = 0;
	//}
	//else {
	//	R_C = dot(R, C);
	//}

	//float specular_intensity = pow(R_C, specular_power);
	
	//color_S = color_S + R_s * L * specular_intensity;

   }

   color = color_A + color_D + color_S;

   return color;
}
