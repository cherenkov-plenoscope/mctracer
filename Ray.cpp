#include "Ray.h"
//==============================================================================
void Ray::SetID(unsigned long long int nID){
	identifier_number = nID;
}
//==============================================================================
unsigned long long int Ray::ID()const{
	return identifier_number;
}
//==============================================================================
void Ray::SetRay(const Vector3D nsup,const Vector3D ndir){
	support = nsup;
	direction  = ndir/ndir.norm2();
}
//==============================================================================
void Ray::SetSupport(const double x,const double y,const double z){
	support.set(x,y,z);
}
//==============================================================================
void Ray::SetSupport(const Vector3D nsup){
	support = nsup;
}
//==============================================================================
void Ray::SetDirection(const double x,const double y,const double z){
	direction.set(x,y,z);
	direction = direction/direction.norm2();
}
//==============================================================================
void Ray::SetDirection(const Vector3D ndir){
	direction = ndir/ndir.norm2();
}
//==============================================================================
void Ray::disp()const{
	std::cout << "Ray -> " << get_string() << "\n";
}
//==============================================================================
std::string Ray::get_string()const{
	std::stringstream out; out.str("");
	out<<"support: " << support <<", direction: " << direction;
	return out.str();
}
//==============================================================================
Vector3D Ray::PositionOnRay(const double scalar)const{
	return (support + direction*scalar);
}
//==============================================================================
Vector3D Ray::Support()const{
	return support;
}
//==============================================================================
Vector3D Ray::Direction()const{
	return direction;
}
//==============================================================================
void Ray::operator= (Ray eqray){
	support = eqray.support;
	direction  = eqray.direction;
}
//==============================================================================
void Ray::homo_transformation_of_ray(Ray* ray,const HomoTrafo3D *T)const{
	//transform support vector
	T->transform_position(&ray->support);
	//transform direction
	T->transform_orientation(&ray->direction);
}
//==============================================================================
void Ray::pre_trace(
	const CartesianFrame* Frame2CheckForIntersectionOfRayAndMaxSphere, 
	std::vector<const CartesianFrame*> *Ptr2ListOfFramesWithIntersectionsOfRayAndMaxSpehre
)const{
	// the pretracer calculates a list of objects to be testet
	// in having intersections with a ray.
	// The key is to extract a list as small as possible to avoid unneccesary
	// intersection tests 
	// 
	// Using bounding Volume spheres and OctTrees, a speed up in the order of 
	// O(number_of_objects^1) --> O( log(number_of_objects) )
	// is achieved / desired

	if(
		IntersectionWithBoundingSphere(
			Frame2CheckForIntersectionOfRayAndMaxSphere
		)
	){
		// Check the frame for an OctTree
		const OctTreeCube *OctTreeOfFrame2CheckForIntersectionOfRayAndMaxSphere = 
		Frame2CheckForIntersectionOfRayAndMaxSphere->get_OctTree();
		
		if(OctTreeOfFrame2CheckForIntersectionOfRayAndMaxSphere != NULL){
			// This Frame uses an OctTree to store its children
			std::unordered_set<CartesianFrame*> IntersectionCandidates;

			// initialize a special Ray with additional information for 
			// the OctTree traversal
			OctTreeTraversingRay SpecialRay(this);

			// transform the special Ray to the Cartesian frame where 
			// the OctTree lives
			homo_transformation_of_ray(
				&SpecialRay,
				Frame2CheckForIntersectionOfRayAndMaxSphere->
				world2frame()
			);

			// update the special information in the special Ray
			SpecialRay.update();

			// find the intersection candidates
			SpecialRay.IntersectionCandidatesInOctTree(
				OctTreeOfFrame2CheckForIntersectionOfRayAndMaxSphere,
				&IntersectionCandidates
			);

			// add the intersection candidates to the list of frames to check for 
			// an intersection
			for(CartesianFrame* FrameFoundInOctTree : IntersectionCandidates){
				pre_trace(FrameFoundInOctTree,
				Ptr2ListOfFramesWithIntersectionsOfRayAndMaxSpehre
				);
			}

			//cout << IntersectionCandidates.size() << endl;
		}else if(
			Frame2CheckForIntersectionOfRayAndMaxSphere->
			get_number_of_children() > 0
		){
				// this frame has children to be tested and no OctTree
				for(
					int child_itterator=0; 
					child_itterator<
					Frame2CheckForIntersectionOfRayAndMaxSphere->
					get_number_of_children();
					child_itterator++
				){
					pre_trace(
					Frame2CheckForIntersectionOfRayAndMaxSphere->
					get_pointer_to_child(child_itterator),
					Ptr2ListOfFramesWithIntersectionsOfRayAndMaxSpehre
					);
				}
		}else{
				// there are no children in this frame
				// so it is an object itself
				Ptr2ListOfFramesWithIntersectionsOfRayAndMaxSpehre->
				push_back(
					Frame2CheckForIntersectionOfRayAndMaxSphere
				);
				// Only here frames are added to the list of intersection 
				// candidates
		}
	}else{
		// the frame (and its children in case there are some) does not
		// intersect with this ray at all.
	}
}
//==============================================================================
void OctTreeTraversingRay::IntersectionCandidatesInOctTree(
	const OctTreeCube *Cube,
	std::unordered_set<CartesianFrame*> *IntersectionCandidates)const{
	// Check whether this Cube is a leaf or has children
	if(Cube->ChildCubes.size()==0){
		// There are no more sub Cubes in this tree
		// This cube is a leaf of the tree so all the Frames in this leaf 
		// are possible hit candidates and are added to the 
		// IntersectionCandidates
		for(CartesianFrame* FrameInChildCube : Cube->ChildFrames){
			IntersectionCandidates->insert(FrameInChildCube);
		}
		// here in the leaf of the OctTree the recursion ends.
	}else{
		// There are more sub cubes in this OctTree
		for(OctTreeCube* ChildCube : Cube->ChildCubes){
			// check all child cubes of this non leaf cube 
			// for intersections with this ray

			if(IntersectionWithOctTreeCube(ChildCube)){
				// this child cube is intersecting the ray.
				// Now IntersectionCandidatesInOctTree() is called 
				// in a recursive fashion to go through the OctTree
				IntersectionCandidatesInOctTree(
					ChildCube,
					IntersectionCandidates
				);
			}
			// when there is no intersection of the ray and the cube then 
			// also the frames inside the cube do not intersect and can be 
			// neglected
		}
	}
}
//==============================================================================
bool OctTreeTraversingRay::IntersectionWithOctTreeCube(const OctTreeCube* Cube)const{
	/*
	  An Efficient and Robust Ray–Box Intersection Algorithm
	Amy Williams, Steve Barrus, R. Keith Morley, Peter Shirley
	                   University of Utah
	*/

	double tmin, tmax, tYmin, tYmax, tZmin, tZmax;
	
	tmin  = (Cube->limits[0][  sign[0]] - support.x())*inv_dir.x();
	tmax  = (Cube->limits[0][1-sign[0]] - support.x())*inv_dir.x(); 

	tYmin = (Cube->limits[1][  sign[1]] - support.y())*inv_dir.y();
	tYmax = (Cube->limits[1][1-sign[1]] - support.y())*inv_dir.y();

	if ( (tmin > tYmax) || (tYmin > tmax) )
		return false;
	if (tYmin > tmin)
		tmin = tYmin;
	if (tYmax < tmax)
		tmax = tYmax;

	tZmin = (Cube->limits[2][  sign[2]] - support.z())*inv_dir.z();
	tZmax = (Cube->limits[2][1-sign[2]] - support.z())*inv_dir.z();

	if ( (tmin > tZmax) || (tZmin > tmax) )
		return false;
	if (tZmin > tmin)
		tmin = tZmin;
	if (tZmax < tmax)
		tmax = tZmax;

	return (tmax > 0.0) ;
}
//==============================================================================
bool Ray::IntersectionWithBoundingSphere(const CartesianFrame* frame)const{
	// Here we check whether this ray is intersecting the boundary sphere of
	// a CartesianFrame 'frame'. The boundary sphere is the sphere enclosing
	// all the frame's children frames.
	// We create a plane orthogonal to this ray and containing the center
	// of the boundary sphere.
	// plane equation:
	//
	// 	d = x*a + y*b + z*c
	//
	// We set the normal vector n of the plane to the ray's direction vector:
	//
	//  a=direction.x b=direction.y c=direction.z
	//
	// Now we insert the support vevtor of the frame into the plane eqaution:
	//
	//  d = frame_sup.x*dirx + frame_sup.y*diry + frame_sup.z*dirz

	double d = direction*(
		*frame->get_pointer_to_position_of_frame_in_world_frame()
	);
	
	// Insert the ray into plane equation and solve for the ray variable v
	// ray: support + v * direction
	
	double v = (d - support*direction)/(direction*direction);
	
	// Calculate point Q. Q is on the ray and in the plane. So Q is the 
	// closest point on the ray to the center of the frame
	
	Vector3D Q = support + direction*v;
	
	// Calculate the connection vector between Q and and the center of the
	// frame called W
	
	Vector3D W = Q*(-1.0) + 
	*frame->get_pointer_to_position_of_frame_in_world_frame();
	
	// Calculate length of W. The lenght of W is the closest distance
	// between ray and object
	
	double dist = W.norm2();  

	// Now compare the distance in between the ray and the center of the frame
	// 'dist' and the radius of the boundary sphere of the frame which encloses 
	// all the children frames of the frame.
	if(
	dist > 
	*frame->get_pointer_to_radius_of_sphere_enclosing_all_children()
	){
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
		if(v<0){
			// The frame support is behind the source of this ray.
			// We test whether the source of this ray is still inside of the
			// frame, i.e. inside the boundary sphere, or completley behind it.
			if( 
			(
			support*(-1.0)+
			*frame->get_pointer_to_position_of_frame_in_world_frame()
			).norm2() 
			< 
			*frame->
			get_pointer_to_radius_of_sphere_enclosing_all_children() 
			){
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
//==============================================================================
void Ray::disp_possible_hit_list(const CartesianFrame *frame)const{
	// initialize empty vector/list
	std::vector<const CartesianFrame*> list_of_possible_hits;
	// run pre_tracer to test hits
	pre_trace(frame,&list_of_possible_hits);
	// disp result
	std::stringstream out; out.str("");
	out<<"list of possible hit candidates:"<<std::endl;
	out<<get_string();
	for(unsigned int i=0; i<list_of_possible_hits.size(); i++)
	{
		out<<(i+1)<<". "<<list_of_possible_hits.at(i)->get_name_of_frame();
		
		const CartesianFrame * ptr_to_mother_frame = 
		list_of_possible_hits.at(i)->get_pointer_to_mother_frame();
		
		while(ptr_to_mother_frame != NULL)
		{
			out<<" -> "<<ptr_to_mother_frame->get_name_of_frame();
			
			ptr_to_mother_frame = 
			ptr_to_mother_frame->get_pointer_to_mother_frame();
		}
		out<<std::endl;
	}
	std::cout<<out.str();
}
//==============================================================================
void Ray::test_intersection_for_hit_candidates(
	std::vector<const CartesianFrame*> *list_of_objects_which_might_intersect,
	std::vector<Intersection*> *ptr_to_list_of_ptr_to_intersections,
  	const CartesianFrame* object_propagated_from,
	int refl_count
)const{

	std::vector<Intersection*> list_of_ptr_to_intersections_which_might_take_place;

	//==================================================================
	// for all possible hit candidates test intersections
	//==================================================================
	
	size_t number_of_objects_to_test = 
	list_of_objects_which_might_intersect->size();
	
	//std::cout<<"list_of_objects_which_might_intersect->size()";
	//std::cout<<list_of_objects_which_might_intersect->size()<<std::endl;
	
	for(
		size_t object_itterator=0;
		object_itterator<number_of_objects_to_test;
		object_itterator++ 
	){
		/*
		std::cout<<"tracer testing object: ";
		std::cout<<*list_of_objects_which_might_intersect.
		at(object_itterator)->get_pointer_to_name_of_frame()<<std::endl;
		*/
		//==============================================================
		// transform ray into object system using
		// homogenous transformation
		//==============================================================
		Ray ray_in_object_system;
		ray_in_object_system.SetRay(support,direction);
		
		homo_transformation_of_ray(
			&ray_in_object_system,
			list_of_objects_which_might_intersect->at(object_itterator)->
			world2frame()
		);
		
		//==============================================================
		// requesting hit in corresponding object
		// a virtual hit in CartesianFrame decleration is needed to do this
		//==============================================================
		Intersection* ptr_to_new_intersection;
		ptr_to_new_intersection = new Intersection;
		
		list_of_ptr_to_intersections_which_might_take_place.
		push_back(ptr_to_new_intersection);
		
		list_of_objects_which_might_intersect->at(object_itterator)->
		hit(	
			&ray_in_object_system.support,	
			&ray_in_object_system.direction,
			list_of_ptr_to_intersections_which_might_take_place.
			at(object_itterator)
		);
		
		//==============================================================
		// add object to list of hits when hit_flag == true
		//==============================================================

		if(
			list_of_ptr_to_intersections_which_might_take_place.
			at(object_itterator)->get_intersection_flag()
		){
			
			if(refl_count == 0){
			
				ptr_to_list_of_ptr_to_intersections->push_back(
					list_of_ptr_to_intersections_which_might_take_place.
					at(object_itterator)
				);			
			
			}else{
				// test wether the object is reflecting onto itself
				// or not
				if(
					list_of_objects_which_might_intersect->at(object_itterator) 
					!=
					object_propagated_from
				){

					ptr_to_list_of_ptr_to_intersections->push_back(
						list_of_ptr_to_intersections_which_might_take_place.
						at(object_itterator)
					);
					
				}else{
					//std::cout<<"refl onto itself."<<std::endl;
					list_of_ptr_to_intersections_which_might_take_place.
					at(object_itterator)->set_intersection_flag(false);
				}
			}
		}
	}
	// free mempory
	for(
		Intersection *ptr_to_intersection : 
		list_of_ptr_to_intersections_which_might_take_place
	){
		if(!ptr_to_intersection->get_intersection_flag()){
			delete ptr_to_intersection;
		}
	}

}
//==============================================================================
void Ray::calculate_reflected_ray(	
	Intersection * pointer_to_closest_intersection,
	Ray *ray_reflection_on_object){
	//==========================================================
	// calculate reflection ray in object system
	//==========================================================
	
	// transform ray_in_object_system into the system of this 
	// particular frame.
	Ray ray_in_object_system;
	ray_in_object_system.SetRay(support,direction);
	homo_transformation_of_ray(
	&ray_in_object_system,
	pointer_to_closest_intersection->
	get_pointer_to_intersecting_object()->
	world2frame()
	);
	
	Vector3D refl_dir; 
	refl_dir = ray_in_object_system.direction;
	// mirror
	pointer_to_closest_intersection->
	get_reflection_direction_in_object_system(&refl_dir);

	Vector3D refl_sup;
	pointer_to_closest_intersection->
	get_intersection_vec_in_object_system(&refl_sup);
	
	ray_reflection_on_object->SetRay(refl_sup,refl_dir);

	//==========================================================
	// calculate reflection ray in world system
	//==========================================================
	
	//std::cout<<"b and a"<<std::endl;
	
	homo_transformation_of_ray(
	ray_reflection_on_object,
	pointer_to_closest_intersection->
	get_pointer_to_intersecting_object()->
	frame2world()
	);	
	//ray_reflection_on_object->disp();		
}
//==============================================================================
Intersection* Ray::calculate_closest_intersection(	
		std::vector<Intersection*> *pointer_to_list_of_intersections
)const{
	// In a list of valid intersectios with this ray and some frames we look 
	// for the intersection which is closest to the support vector of the ray.
	// We return the intersection closest to the support of the vector.
	std::vector<Intersection*>::
	iterator pointer_to_pointer_to_closest_intersection = min_element( 	
		pointer_to_list_of_intersections->begin(),
		pointer_to_list_of_intersections->end() ,
		*this
	);

	return 	(*pointer_to_pointer_to_closest_intersection);
}
//==============================================================================
Intersection* Ray::get_closest_intersection(
	const CartesianFrame* world,
	GlobalSettings *settings
){
	int refl_count = 0;
	const CartesianFrame* object_propagated_from = NULL;
	//==================================================================
	// claculate a list/vector containing all possible intersection
	// candidates using pre-trace
	//==================================================================
	std::vector<const CartesianFrame*> list_of_objects_which_might_intersect;
	pre_trace( world , &list_of_objects_which_might_intersect );
	
	//==================================================================
	// calculate a list of objects wich _do_ intersect from the list_of_
	// objects_which_might_intersect
	//==================================================================

	std::vector<Intersection*> list_of_ptr_to_intersections;
	
	test_intersection_for_hit_candidates(
		&list_of_objects_which_might_intersect,
		&list_of_ptr_to_intersections,
		object_propagated_from,
		refl_count
	);
		
	//==================================================================
	// test wether there is at least one object intersecting the ray or
	// not by using the size of the list_of_intersecting_objects
	//==================================================================
	Intersection *ptr_to_closest_intersection;	

	if( list_of_ptr_to_intersections.size() == 0 ){
		
		ptr_to_closest_intersection = new Intersection;
		ptr_to_closest_intersection->set_intersection_flag( false );
		
		return ptr_to_closest_intersection;
	}else{
		//==============================================================
		// find the closest object in the list of 
		// list_of_intersecting_objects
		//==============================================================


		ptr_to_closest_intersection = calculate_closest_intersection(
			&list_of_ptr_to_intersections
		);

		return ptr_to_closest_intersection;
	}
}
//==============================================================================
ColourProperties Ray::trace(const CartesianFrame* world,
				int refl_count,
				const CartesianFrame* object_propagated_from,
				GlobalSettings *settings)
{
	
	//==================================================================
	// claculate a list/vector containing all possible intersection
	// candidates using pre-trace
	//==================================================================
	std::vector<const CartesianFrame*> list_of_objects_which_might_intersect;
	pre_trace( world , &list_of_objects_which_might_intersect );
	
	//==================================================================
	// calculate a list of objects wich _do_ intersect from the list_of_
	// objects_which_might_intersect
	//==================================================================

	std::vector<Intersection*> list_of_ptr_to_intersections;
	
	test_intersection_for_hit_candidates(
						&list_of_objects_which_might_intersect,
						&list_of_ptr_to_intersections,
						object_propagated_from,
						refl_count);
		
	//==================================================================
	// test wether there is at least one object intersecting the ray or
	// not by using the size of the list_of_intersecting_objects
	//==================================================================
	ColourProperties colour_to_return;
	
	if(list_of_ptr_to_intersections.size()==0)
	{
		// RETURN
		colour_to_return = settings->get_default_colour();
	}else{
		//==============================================================
		// find the closest object in the list of 
		// list_of_intersecting_objects
		//==============================================================
		
		Intersection *ptr_to_closest_intersection;
		ptr_to_closest_intersection = calculate_closest_intersection(
			&list_of_ptr_to_intersections
		);
		
		//==============================================================
		// test reflection of the object pointer_to_closest_frame is
		// pointing to
		//==============================================================
		if(
		ptr_to_closest_intersection->get_pointer_to_intersecting_object()
		->get_hit_reflection_flag()
		&& refl_count < settings->get_max_number_of_reflections())
		{	
			//==========================================================
			// the object pointer_to_closest_frame is pointing to
			// does reflect, so we increase the reflection counter
			//==========================================================
			
			refl_count++;
			
			//==========================================================
			// calculate the ray reflected by the object
			// pointer_to_closest_frame is pointing to relativ to
			// the world coordinate system
			//==========================================================
			Ray ray_reflection_on_object;
			calculate_reflected_ray(
				ptr_to_closest_intersection,
				&ray_reflection_on_object);
			
			//==========================================================
			// mix colours
			//==========================================================
			// RETURN
			colour_to_return = 
			ptr_to_closest_intersection->
			get_pointer_to_intersecting_object()->
			get_hit_colour();
			
			// use itterativ call of trace to handle reflections
			ColourProperties colour_of_reflected_ray;
			
			colour_of_reflected_ray = 
			ray_reflection_on_object.trace(
			world,
			refl_count,
			ptr_to_closest_intersection->
			get_pointer_to_intersecting_object(),
			settings
			);
			
			colour_to_return.reflection_mix(
			&colour_of_reflected_ray,
			ptr_to_closest_intersection->
			get_pointer_to_intersecting_object()->
			get_ptr2_reflection()
			);

		}else{
			// std::cout<<"there are no reflections."<<std::endl;
			
			// RETURN
			colour_to_return = ptr_to_closest_intersection->
			get_pointer_to_intersecting_object()->
			get_hit_colour();
		}	
	}
	//=============
	//free memory list_of_ptr_to_intersections 
	//=============
	for(
		Intersection *ptr_to_intersection : 
		list_of_ptr_to_intersections
	){
		
		delete ptr_to_intersection;
	}

	return colour_to_return;
}
//==============================================================================
void Ray::propagate(	
	const CartesianFrame* world, 
	ListOfInteractions* history,
	int interaction_count,
	const CartesianFrame* object_propagated_from,
	const GlobalSettings* settings,
	PseudoRandomNumberGenerator* dice
){
	std::cout << "Calling propagate of a Ray instance!" << endl;
}
//==============================================================================
double Ray::get_distance_to_closest_object(
	const CartesianFrame* world,
	const GlobalSettings *settings
)const{
	
	double distance_to_closets_object = 0.0;
	//==================================================================
	// claculate a list/vector containing all possible intersection
	// candidates using pre-trace
	//==================================================================
	std::vector<const CartesianFrame*> list_of_objects_which_might_intersect;
	pre_trace( world , &list_of_objects_which_might_intersect );
	
	//==================================================================
	// calculate a list of objects wich _do_ intersect from the list_of_
	// objects_which_might_intersect
	//==================================================================
	std::vector<Intersection*> list_of_ptr_to_intersections;
	
	int refl_count = 0;
	CartesianFrame* object_propagated_from = nullptr;

	test_intersection_for_hit_candidates(
		&list_of_objects_which_might_intersect,
		&list_of_ptr_to_intersections,
		object_propagated_from,
		refl_count
	);
	
	//==================================================================
	// test wether there is at least one object intersecting the ray or
	// not using the size of the list_of_intersecting_objects
	//==================================================================
	if(list_of_ptr_to_intersections.size()==0)
	{
		// do nothing because there are no intersections
	}else{
		//==============================================================
		// find the closest object in the list of 
		// list_of_intersecting_objects
		//==============================================================

		Intersection *ptr_to_closest_intersection;
		ptr_to_closest_intersection = calculate_closest_intersection(
			&list_of_ptr_to_intersections
		);
		return ptr_to_closest_intersection->get_intersection_distance();
	}
	return distance_to_closets_object;
}
//==============================================================================
bool Ray::operator() (Intersection* one, Intersection* two)const{
	return 	one->get_intersection_distance() < two->get_intersection_distance();
}
//==============================================================================
CsvRow Ray::getRayCsvRow(GlobalSettings& settings)const{
	
	CsvRow row;

	if( settings.ShowCsvIdentity() )
		row.append( getCsvID() );

	row.append( support.getCsvRow(settings) );
	row.append(  direction.getCsvRow(settings) );

	return row;
}
//==============================================================================
CsvRow Ray::getCsvRow(GlobalSettings& settings)const{

	CsvRow row;

	if( settings.ShowCsvIdentifier() == true )
		row.push_back("Ray");

	row.append( getRayCsvRow(settings) );
	
	return row;
}
//==============================================================================
CsvRow Ray::getCsvID()const{

	CsvRow rowID;

	rowID.push_back(
		static_cast<ostringstream*>
		( &(ostringstream() << identifier_number ) )->str()
	);

	return rowID;
}
//==============================================================================
void Ray::SetHistory(ListOfInteractions* nhistory){
	history = nhistory;
}
//==============================================================================
ListOfInteractions* Ray::GetHistory()const{
	return history;
}
//==============================================================================
CsvRow Ray::getCsvRowHistory(GlobalSettings& settings)const{
	
	CsvRow row;

	if( settings.ShowCsvIdentity() )
		row.append( getCsvID() );	

	row.append( history->getCsvRow(settings) );

	return row;
}
//==============================================================================
// friends of osstream
//==============================================================================
std::ostream& operator<<(std::ostream& os, const Ray& ray_to_be_displayed){
    os << ray_to_be_displayed.get_string();
    return os;
}

