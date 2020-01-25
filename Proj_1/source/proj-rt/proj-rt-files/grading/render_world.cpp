#include "render_world.h"
#include "flat_shader.h"
#include "object.h"
#include "light.h"
#include "ray.h"

extern bool disable_hierarchy;

Render_World::Render_World()
    :background_shader(0),ambient_intensity(0),enable_shadows(true),
    recursion_depth_limit(3)
{}

Render_World::~Render_World()
{
    delete background_shader;
    for(size_t i=0;i<objects.size();i++) delete objects[i];
    for(size_t i=0;i<lights.size();i++) delete lights[i];
}

// Find and return the Hit structure for the closest intersection.  Be careful
// to ensure that hit.dist>=small_t.
Hit Render_World::Closest_Intersection(const Ray& ray)
{
    double min_t = std::numeric_limits<double>::max();
    Hit hit;
    Hit close_hit = {0,0,0};
    for(unsigned i = 0; i < this->objects.size(); ++i) {
	hit = this->objects.at(i)->Intersection(ray,-1);
    	if(hit.dist < min_t && hit.dist > small_t){
        	min_t = hit.dist;
		close_hit = hit;
	}
    }
        	  
    return close_hit;
}

// set up the initial view ray and call
void Render_World::Render_Pixel(const ivec2& pixel_index) {
     // set up the initial view ray here
    Ray ray;
    ray.endpoint = this->camera.position;
    ray.direction = (this->camera.World_Position(pixel_index) - ray.endpoint).normalized();
    //Ray ray;
    vec3 color=Cast_Ray(ray,1);
    camera.Set_Pixel(pixel_index,Pixel_Color(color));
}

void Render_World::Render() {
    if(!disable_hierarchy)
        Initialize_Hierarchy();

    for(int j=0;j<camera.number_pixels[1];j++)
        for(int i=0;i<camera.number_pixels[0];i++)
            Render_Pixel(ivec2(i,j));
}

// cast ray and return the color of the closest intersected surface point,
// or the background color if there is no object intersection
vec3 Render_World::Cast_Ray(const Ray& ray,int recursion_depth)
{
    vec3 color;
    Hit intersected; 
    intersected = this->Closest_Intersection(ray);
    if(intersected.dist != 0) {
	vec3 intersect = ray.Point(intersected.dist);
	vec3 norm = intersected.object->Normal(intersect, intersected.part);
	color = intersected.object->material_shader->Shade_Surface(ray, intersect,norm,recursion_depth_limit);
    }
    else {
	color = this->background_shader->Shade_Surface(ray, ray.endpoint, ray.endpoint, this->recursion_depth_limit);
    }// determine the color here	
    return color;
}

void Render_World::Initialize_Hierarchy()
{
    disable_hierarchy = true;; // Fill in hierarchy.entries; there should be one entry for
    // each part of each object.

    hierarchy.Reorder_Entries();
    hierarchy.Build_Tree();
}
