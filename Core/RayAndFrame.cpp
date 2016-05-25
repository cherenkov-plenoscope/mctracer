#include "Core/RayAndFrame.h"
namespace RayAndFrame {
//------------------------------------------------------------------------------
bool ray_support_inside_frames_bounding_sphere(const Ray* ray, const Frame *frame) {
	return (frame->get_position_in_world() - ray->get_support()).
		norm_is_less_equal_than(frame->contour_radius());
}
//------------------------------------------------------------------------------
bool ray_has_intersection_with_bounding_sphere_of(const Ray* ray, const Frame *frame) {

	const double alpha = ray->get_parameter_on_ray_for_closest_distance_to_point(
		frame->get_position_in_world()
	);

	const Vec3 q = ray->get_pos_at(alpha);
	const Vec3 shortest_connection = frame->get_position_in_world() - q;
	const double dist_square = shortest_connection*shortest_connection;

	if(dist_square > frame->contour_radius()*frame->contour_radius()) {
		//
		// -------------+-----\--------> ray
		//              |     |
		//            __|__    > dist
		//           /  |  \  |
		//          |   +   | /
		//           \_____/ 
		//              <--->
		//              radius of sphere enclosing all children
		//
		// The ray does not intersect the frame's boundary sphere.
		return false;
	}else{
		if(alpha < 0.0){
			// The frame support is behind the source of this ray.
			// We test whether the source of this ray is still inside of the
			// frame, i.e. inside the boundary sphere, or completley behind it.
			if(ray_support_inside_frames_bounding_sphere(ray, frame)) {
				// The support of the ray is inside the frame's boundary sphere
				// 
				//               _________
				//              /         \                                   //
				//    ________/_____________\________ray_direction_________\  //
				//           /    /          \                             /
		        //          |    /   x frame  |
				//          |   /   /         |
				//           \ /   / radius  /
				//            /   /         /
				//           /  \/________/
				//          /
				//       support    
				//             
				// The ray intersects the boundary sphere when leaving the 
				// sphere.
				return true;
			}else{
				// The support of the ray is outside and behind the frame's   
				// boundary sphere with respect to its support vector.        
				//                                                            
				//               _________                                    
				//              /         \                                   //         
				//    ________/_____________\________ray_direction_________\  //
				//           /               \          /                  /  
		        //          |        x frame  |        /
				//          |       /         |       /
				//           \     / radius  /       /
				//            \   /         /       /
				//              \/________/        /
				//                                /
				//                             support    
				//             
				// The ray does not intersect the boundary sphere at all since
				// it starts behind the frame's boundary sphere.
				return false;
			}
		}else{
			// the frame support is in front of the source of this ray
			// Closest point on ray to center of frame = 
			// ray.support + v * ray.direction
			// 
			// here v > 0
			//               _________
			//              /         \                                       //
			//    ________/_____________\________ray_direction_________\      //
			//      /    /               \                             /
	        //     /    |        x frame  |
			//    /     |       /         |
			// support   \     / radius  /
			//            \   /         /
			//              \/________/
			//              
			//             
			// The ray does not intersect the frame's boundary sphere.
			return true;
		}
	}
}
//------------------------------------------------------------------------------
} //RayAndFrame