#include "Annulus.h"
#include <sstream>
#include "math.h"
using std::string;
using std::stringstream;
using std::vector;

//------------------------------------------------------------------------------
void Annulus::set_outer_inner_radius(
        const double outer_radius, 
        const double inner_radius
) {
    outer_bound.set_radius(outer_radius);
    inner_bound.set_radius(inner_radius);

    post_initialize_radius_of_enclosing_sphere();
}
//------------------------------------------------------------------------------
void Annulus::post_initialize_radius_of_enclosing_sphere(){
    bounding_sphere_radius = outer_bound.get_radius();
}
//------------------------------------------------------------------------------
string Annulus::str()const {
    stringstream out;
    out << SurfaceEntity::str();

    out << "Annulus:\n";
    out << "| Ri: " << inner_bound.get_radius() << "m, ";
    out << "Ro: " << outer_bound.get_radius() << "m\n";
    out << "| area:   " << get_area() << "m^2\n";
    return out.str();
}
//------------------------------------------------------------------------------
double Annulus::get_area()const {
    return M_PI*(
        outer_bound.get_radius()*outer_bound.get_radius() -
        inner_bound.get_radius()*inner_bound.get_radius()
    );
}
//------------------------------------------------------------------------------
void Annulus::calculate_intersection_with(
    const Ray* ray, 
    vector<Intersection> *intersections
)const {

    XyPlaneRayIntersectionEquation xyPlaneRayEquation(ray);

    if( xyPlaneRayEquation.has_causal_solution()) {
        
        double v = xyPlaneRayEquation.get_ray_parameter_for_intersection();
        Vec3 intersection_vector = ray->get_pos_at(v);      

        if( outer_bound.is_inside(&intersection_vector) && 
            !inner_bound.is_inside(&intersection_vector)
        ) {

            if(ray->get_support() != intersection_vector) {
                
                intersections->emplace_back(
                    this,
                    intersection_vector,
                    xyPlaneRayEquation.get_plane_normal_vector(),
                    v,
                    ray->get_direction()
                );
            }
        }
    }
}