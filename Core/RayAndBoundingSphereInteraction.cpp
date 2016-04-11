#include "Core/Ray.h"
//------------------------------------------------------------------------------
bool Ray::support_of_ray_is_inside_bounding_sphere_of(const Frame *frame)const {
	return (*frame->get_position_in_world() - support).norm_is_less_equal_than(
		frame->contour_radius());
}
//------------------------------------------------------------------------------
bool Ray::has_intersection_with_bounding_sphere_of(const Frame* frame)const{

	double ray_parameter_for_closest_distance_to_center_of_bounding_sphere = 
		get_parameter_on_ray_for_closest_distance_to_point(
			*frame->get_position_in_world()
		);

	double distance_to_center_of_bounding_sphere = 
		get_distance_to_point_from_position_of_ray_at(
			*frame->get_position_in_world(),
			ray_parameter_for_closest_distance_to_center_of_bounding_sphere
		);

	if(distance_to_center_of_bounding_sphere > frame->contour_radius()) {
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
		if(ray_parameter_for_closest_distance_to_center_of_bounding_sphere < 0.0){
			// The frame support is behind the source of this ray.
			// We test whether the source of this ray is still inside of the
			// frame, i.e. inside the boundary sphere, or completley behind it.
			if(support_of_ray_is_inside_bounding_sphere_of(frame)) {
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