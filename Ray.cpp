#include "Ray.h"
//======================================================================
void Ray::set_ray(const Vector3D nbase,const Vector3D ndir){
	base = nbase;
	dir  = ndir/ndir.norm2();
}
//======================================================================
void Ray::set_base(const double x,const double y,const double z){
	base.set(x,y,z);
}
//======================================================================
void Ray::set_base(const Vector3D nbase){
	base = nbase;
}
//======================================================================
void Ray::set_dir(const double x,const double y,const double z){
	dir.set(x,y,z);
	dir = dir/dir.norm2();
}
//======================================================================
void Ray::set_dir(const Vector3D ndir){
	dir = ndir/ndir.norm2();
}
//======================================================================
void Ray::disp()const{
	std::cout<<get_string();
}
//======================================================================
std::string Ray::get_string()const{
	std::stringstream out; out.str("");
	out<<"ray : "<<base.get_string()<<" + var*"<<dir.get_string()<<std::endl;
	return out.str();
}
//======================================================================
Vector3D Ray::get_position_on_ray(const double scalar)const{
	return (base + dir*scalar);
}
//======================================================================
void Ray::pre_trace(
const CartesianFrame* frame_to_check_for_interaction_of_ray_and_max_sphere, 
std::vector<const CartesianFrame*> *Ptr2ListOfFramesWithIntersectionsOfRayAndMaxSpehre
)const{
	// the pretracer calculates a list of objects to be testet
	// in having intersections with a ray.
	//
	// THIS IS A SPEED UP IN 
	// O(number_of_objects^1) --> O( log(number_of_objects) )

	if(
	is_ray_hitting_frame(
	frame_to_check_for_interaction_of_ray_and_max_sphere)
	)
	{
		// the frame is hit by this ray
		if(
		frame_to_check_for_interaction_of_ray_and_max_sphere->
		get_number_of_children() > 0
		)
		{
				// this frame has children to be tested
				for(
				int child_itterator=0; 
				child_itterator<
				frame_to_check_for_interaction_of_ray_and_max_sphere->
				get_number_of_children();
				child_itterator++
				)
				{
					pre_trace(
					frame_to_check_for_interaction_of_ray_and_max_sphere->
					get_pointer_to_child(child_itterator),
					Ptr2ListOfFramesWithIntersectionsOfRayAndMaxSpehre
					);
				}
		}else{
				// there are no children in this frame
				// so it is an object
				Ptr2ListOfFramesWithIntersectionsOfRayAndMaxSpehre->
				push_back(
				frame_to_check_for_interaction_of_ray_and_max_sphere
				);
		}
	}else{
		// the frame (and its children in case there are some) does not
		// intersect with this ray.
	}
}
//======================================================================
bool Ray::is_ray_hitting_frame(const CartesianFrame* frame)const{
	// create a plane orthogonal to this ray and containing the center
	// of the max norm sphere
	// plane equation
	// 	d = x*a + y*b + z*c
	// set normal vector n of plane to ray direction
	//  a=dir.x b=dir.y c=dir.z
	// insert base point of frame into plane eqaution
	//  d = frame_base.x*dirx + frame_base.y*diry + frame_base.z*dirz
	//std::cout<<"frame: "<<frame->name_of_frame<<" P: "<<frame->pos.get_string()<<" "; 
	
	//Vector3D pos = frame->T_frame2world.get_translation();
	
	double d = dir*(
	*frame->get_pointer_to_position_of_frame_in_world_frame()
	);
	
	// Insert ray into plane equation and solve out the ray variable v
	// ray: base + v*dir
	
	double v = (d - base*dir)/(dir*dir);
	//std::cout<<"v: "<<v<<" ";
	
	// Calculate point Q. Q is on the ray and in the plane. So Q is the 
	// closest point of ray to the point P
	
	Vector3D Q = base + dir*v;
	
	// calculate the connection vector of Q and P called W
	
	Vector3D W = Q*(-1.0) + 
	*frame->get_pointer_to_position_of_frame_in_world_frame();
	
	// calculate length of W. The lenght of W is the closest distance
	// between ray and object
	
	double dist = W.norm2();  
	
	//std::cout<<"dist to base: "<<dist<<" ";
	//std::cout<<"radius_of_sphere_enclosing_all_children: "<<frame->radius_of_sphere_enclosing_all_children<<" ";
	
	// compare dist and diameter of frame
	if(
	dist > 
	*frame->get_pointer_to_radius_of_sphere_enclosing_all_children()
	){
		//std::cout<<"hit: Distance ray to frame is to big."<<std::endl;
		return false;
	}else{
		//std::cout<<"hit: Distance ray to frame is small enough."<<std::endl;
		if(v<0){
			// the frame base is behind the source of this ray
			// test wether the source of this ray is still inside of the
			// frame
			if( 
			(
			base*(-1.0)+
			*frame->get_pointer_to_position_of_frame_in_world_frame()
			).norm2() 
			< 
			*frame->
			get_pointer_to_radius_of_sphere_enclosing_all_children() 
			){
				// the base of this ray is still inside the frame
				return true;
			}else{
				// the base of this ray is not inside the frame
				return false;
			}
		}else{
			// the frame base is in front of the source of this ray
			return true;
		}
	}
}
//======================================================================
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
		out<<(i+1)<<". "<<*list_of_possible_hits.at(i)->
		get_pointer_to_name_of_frame();
		
		const CartesianFrame * ptr_to_mother_frame = 
		list_of_possible_hits.at(i)->get_pointer_to_mother_frame();
		
		while(ptr_to_mother_frame != NULL)
		{
			out<<" -> "<<*ptr_to_mother_frame->
			get_pointer_to_name_of_frame();
			
			ptr_to_mother_frame = 
			ptr_to_mother_frame->get_pointer_to_mother_frame();
		}
		out<<std::endl;
	}
	std::cout<<out.str();
}
//======================================================================
void Ray::operator= (Ray ray){
	base=ray.base;
	dir =ray.dir;
}
//======================================================================
void Ray::homo_transformation_of_ray(Ray* ray,const HomoTrafo3D *T)const{
	//transform base vector
	T->transform_position(&ray->base);
	//transform direction
	T->transform_orientation(&ray->dir);
}
//======================================================================
void Ray::test_intersection_for_hit_candidates(
			std::vector<const CartesianFrame*> *list_of_objects_which_might_intersect,
			std::vector<const CartesianFrame*> *list_of_intersecting_objects,
			std::vector<Intersection*> 
			*ptr_to_list_of_ptr_to_intersections_which_might_take_place,
			std::vector<Intersection*> *ptr_to_list_of_ptr_to_intersections,
			const CartesianFrame* object_reflected_from,
			int refl_count
)const{
	//==================================================================
	// for all possible hit candidates test intersections
	//==================================================================
	
	int number_of_objects_to_test = 
	list_of_objects_which_might_intersect->size();
	
	//std::cout<<"list_of_objects_which_might_intersect->size()";
	//std::cout<<list_of_objects_which_might_intersect->size()<<std::endl;
	
	for(int object_itterator=0;
		object_itterator<number_of_objects_to_test;
		object_itterator++ )
	{
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
		ray_in_object_system.set_ray(base,dir);
		
		homo_transformation_of_ray(
		&ray_in_object_system,
		list_of_objects_which_might_intersect->
		at(object_itterator)->get_pointer_to_T_world2frame()
		);
		
		//==============================================================
		// requesting hit in corresponding object
		// a virtual hit in CartesianFrame decleration is needed to do this
		//==============================================================
		Intersection* ptr_to_new_intersection;
		ptr_to_new_intersection = new Intersection;
		
		ptr_to_list_of_ptr_to_intersections_which_might_take_place->
		push_back(ptr_to_new_intersection);
		
		list_of_objects_which_might_intersect
		->at(object_itterator)->
		hit(	&ray_in_object_system.base,	
				&ray_in_object_system.dir,
				ptr_to_list_of_ptr_to_intersections_which_might_take_place
				->at(object_itterator)
				);
		
		/*
		ptr_to_list_of_ptr_to_intersections_which_might_take_place
		->at(object_itterator)->disp();
			*/	
		//==============================================================
		// add object to list of hits when hit_flag == true
		//==============================================================
		/*
		if(list_of_objects_which_might_intersect->at(object_itterator)
		->is_hit())*/
		if(ptr_to_list_of_ptr_to_intersections_which_might_take_place
		->at(object_itterator)->get_intersection_flag())
		{
			//std::cout<<"ptr_to_list_of_ptr_to_intersections_which_might_take_place->at(object_itterator)->get_intersection_flag()";
		
			if(refl_count == 0)
			{
			list_of_intersecting_objects->push_back(
			// pointer to intersecting object
			list_of_objects_which_might_intersect->at(object_itterator));	
			
			ptr_to_list_of_ptr_to_intersections->push_back(
			ptr_to_list_of_ptr_to_intersections_which_might_take_place->
			at(object_itterator)
			);			
			
			}else{
				// test wether the object is reflecting onto itself
				// or not
				if(
				list_of_objects_which_might_intersect->at(object_itterator) 
				!=
				object_reflected_from
				)
				{
					ptr_to_list_of_ptr_to_intersections->push_back(
					ptr_to_list_of_ptr_to_intersections_which_might_take_place->
					at(object_itterator)
					);
					
					list_of_intersecting_objects->push_back(
					// pointer to hitted object
					list_of_objects_which_might_intersect->at(object_itterator));	
				}else{
					//std::cout<<"refl onto itself."<<std::endl;
				}
			}
		}
	}
}
//======================================================================
void Ray::calculate_reflected_ray(	
Intersection * pointer_to_closest_intersection,
Ray *ray_reflection_on_object){
			//==========================================================
			// calculate reflection ray in object system
			//==========================================================
			
			// transform ray_in_object_system into the system of this 
			// particular frame.
			Ray ray_in_object_system;
			ray_in_object_system.set_ray(base,dir);
			homo_transformation_of_ray(
			&ray_in_object_system,
			pointer_to_closest_intersection->
			get_pointer_to_intersecting_object()->
			get_pointer_to_T_world2frame()
			);
			
			Vector3D refl_dir; 
			refl_dir = ray_in_object_system.dir;
			// mirror
			pointer_to_closest_intersection->
			get_reflection_direction_in_object_system(&refl_dir);

			
			Vector3D refl_base;
			pointer_to_closest_intersection->
			get_intersection_vec_in_object_system(&refl_base);
			
			
			ray_reflection_on_object->set_ray(refl_base,refl_dir);

			//==========================================================
			// calculate reflection ray in world system
			//==========================================================
			
			//std::cout<<"b and a"<<std::endl;
			
			homo_transformation_of_ray(
			ray_reflection_on_object,
			pointer_to_closest_intersection->
			get_pointer_to_intersecting_object()->
			get_pointer_to_T_frame2world()
			);	
			//ray_reflection_on_object->disp();		
}
//======================================================================
/*
CartesianFrame* calculate_closest_frame(	CartesianFrame *pointer_to_closest_frame,
		std::vector<CartesianFrame*> *pointer_to_list_of_intersecting_objects)
{
	//==============================================================
	// find the closest object
	//==============================================================
	// insert comparator function into template
	std::vector<CartesianFrame*>::iterator pointer_to_pointer_to_closest_object = 
	min_element( 	pointer_to_list_of_intersecting_objects->begin(),
					pointer_to_list_of_intersecting_objects->end() ,
					compare_distance);
						
	//(*pointer_to_pointer_to_closest_object)->disp();	
	return 	(*pointer_to_pointer_to_closest_object);
}*/
//======================================================================
Intersection* Ray::calculate_closest_intersection(	
		Intersection *pointer_to_closest_intersection,
		std::vector<Intersection*> *pointer_to_list_of_intersections)const
{
	//==============================================================
	// find the closest intersection
	//==============================================================
	// insert comparator function into template
	std::vector<Intersection*>::
	iterator pointer_to_pointer_to_closest_intersection = 
	min_element( 	pointer_to_list_of_intersections->begin(),
					pointer_to_list_of_intersections->end() ,
					*this);
					//compare_distance);
	//(*pointer_to_pointer_to_closest_object)->disp();	
	return 	(*pointer_to_pointer_to_closest_intersection);
}
//======================================================================
ColourProperties Ray::trace(const CartesianFrame* world,
				int refl_count,
				const CartesianFrame* object_reflected_from,
				GlobalSettings *settings){
	
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

	std::vector<const CartesianFrame*> list_of_intersecting_objects;
	std::vector<Intersection*> list_of_ptr_to_intersections_which_might_take_place;
	std::vector<Intersection*> list_of_ptr_to_intersections;
	
	test_intersection_for_hit_candidates(
						&list_of_objects_which_might_intersect,
						&list_of_intersecting_objects,
						&list_of_ptr_to_intersections_which_might_take_place,
						&list_of_ptr_to_intersections,
						object_reflected_from,
						refl_count);
		
	//==================================================================
	// test wether there is at least one object intersecting the ray or
	// not using the size of the list_of_intersecting_objects
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
		ptr_to_closest_intersection,
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
	for(unsigned int i=0;
		i<list_of_ptr_to_intersections_which_might_take_place.size();
		i++ )
	{
		delete 
		list_of_ptr_to_intersections_which_might_take_place.
		at(i);
	}	
	//=============
	return colour_to_return;
}
//======================================================================
/*
void Ray::trace_science(CartesianFrame* world,
				int refl_count,
				const CartesianFrame* object_reflected_from,
				GlobalSettings *settings,
				double dbl_passed_distance_from_source_to_sensor){
	
	//==================================================================
	// claculate a list/vector containing all possible intersection
	// candidates using pre-trace
	//==================================================================
	std::vector<CartesianFrame*> list_of_objects_which_might_intersect;
	pre_trace( world , &list_of_objects_which_might_intersect );
	
	//==================================================================
	// calculate a list of objects wich _do_ intersect from the list_of_
	// objects_which_might_intersect
	//==================================================================
	std::vector<CartesianFrame*> list_of_intersecting_objects;
	std::vector<Intersection*> list_of_ptr_to_intersections_which_might_take_place;
	std::vector<Intersection*> list_of_ptr_to_intersections;
	
	test_intersection_for_hit_candidates(
						&list_of_objects_which_might_intersect,
						&list_of_intersecting_objects,
						&list_of_ptr_to_intersections_which_might_take_place,
						&list_of_ptr_to_intersections,
						object_reflected_from,
						refl_count);
	
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

		Intersection *ptr_to_closest_intersection = NULL;
		ptr_to_closest_intersection = calculate_closest_intersection(
		ptr_to_closest_intersection,
		&list_of_ptr_to_intersections
		);
		
		
		//~ CartesianFrame *pointer_to_closest_frame;
		//~ pointer_to_closest_frame = calculate_closest_frame(
		//~ pointer_to_closest_frame,
		//~ &list_of_intersecting_objects);
		
		//==============================================================
		// test reflection of the object pointer_to_closest_frame is
		// pointing to
		//==============================================================
		
		
		Vector3D vec_intersection_in_world_system;
		
		ptr_to_closest_intersection->
		get_intersection_vec_in_object_system(
		&vec_intersection_in_world_system);
		
		ptr_to_closest_intersection->
		get_pointer_to_intersecting_object()->
		get_pointer_to_T_frame2world()->
		transform_position(&vec_intersection_in_world_system);
		
		Vector3D vec_additional;
		vec_additional =  (base-vec_intersection_in_world_system);
		//vec_additional.disp();
		double dbl_new_additional_distance = vec_additional.norm2();
		
		double dbl_new_complete_passed_distance = 
		dbl_passed_distance_from_source_to_sensor+
		dbl_new_additional_distance;
			
		if(
		ptr_to_closest_intersection->get_pointer_to_intersecting_object()->
		get_hit_reflection_flag()
		&& refl_count < settings->get_max_number_of_reflections())
		{	
			refl_count++;
			//std::cout<<"refl count: "<<refl_count<<std::endl;
			//std::cout<<"ray dist  : "<<dbl_new_complete_passed_distance<<" [m]"<<std::endl;
			//==========================================================
			// calculate the ray reflected by the object
			// pointer_to_closest_frame is pointing to relativ to
			// the world coordinate system
			//==========================================================
			
			Ray ray_reflection_on_object;
			calculate_reflected_ray(
				ptr_to_closest_intersection,
				&ray_reflection_on_object);
				
			// calculate new additional distance
			
			// pass ray on to next object and add the new distance	
			ray_reflection_on_object.	
			trace_science(
			world,
			refl_count,
			ptr_to_closest_intersection->get_pointer_to_intersecting_object(),
			settings,
			dbl_new_complete_passed_distance);
			//std::cout<<"science_ray was reflected"<<std::endl;
			//ray_reflection_on_object.disp();
		}else{
			if(	ptr_to_closest_intersection->get_pointer_to_intersecting_object()
				->get_sensor_flag())
			{	
				//========================================
				// create sensorinteraction object
				SensorIntersection *new_sensor_hit;
				new_sensor_hit = new SensorIntersection;
		
				Vector3D vec_intersection_in_object_system;
				
				ptr_to_closest_intersection->
				get_intersection_vec_in_object_system(
				&vec_intersection_in_object_system);
				
				new_sensor_hit->set_sensor_hit(
				vec_intersection_in_object_system.get_x(),
				vec_intersection_in_object_system.get_y(),
				dbl_new_complete_passed_distance);
				
				ptr_to_closest_intersection->
				get_pointer_to_intersecting_object()->
				push_back_SensorIntersection(new_sensor_hit);
				//========================================
				
				//~ ptr_to_closest_intersection->
				//~ get_pointer_to_intersecting_object()->
				//~ push_back_intersection(
				//~ ptr_to_closest_intersection,
				//~ dbl_new_complete_passed_distance
				//~ );
				
				//std::cout<<"Hit it!"<<std::endl;
				//if(refl_count>0){std::cout<<"Hit it after reflection"<<std::endl;}
			}else{
				// do nothing, this ray is lost
			}
		}	
	}

	//=============
	//free memory list_of_ptr_to_intersections 
	//=============
	for(unsigned int i=0;
		i<list_of_ptr_to_intersections_which_might_take_place.size();
		i++ )
	{
		delete 
		list_of_ptr_to_intersections_which_might_take_place.
		at(i);
	}	
	//=============
}*/
//======================================================================
double Ray::get_distance_to_closest_object(const CartesianFrame* world,
				int refl_count,
				CartesianFrame* object_reflected_from,
				const GlobalSettings *settings,
				double dbl_passed_distance_from_source_to_sensor)const{
	
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
	std::vector<const CartesianFrame*> list_of_intersecting_objects;
	std::vector<Intersection*> list_of_ptr_to_intersections_which_might_take_place;
	std::vector<Intersection*> list_of_ptr_to_intersections;
	
	test_intersection_for_hit_candidates(
						&list_of_objects_which_might_intersect,
						&list_of_intersecting_objects,
						&list_of_ptr_to_intersections_which_might_take_place,
						&list_of_ptr_to_intersections,
						object_reflected_from,
						refl_count);
	
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
		ptr_to_closest_intersection,
		&list_of_ptr_to_intersections
		);
		return ptr_to_closest_intersection->get_intersection_distance();
	}
	return distance_to_closets_object;
}
//======================================================================
bool Ray::operator() (Intersection* one, Intersection* two)const{
	return 	one->get_intersection_distance()<
			two->get_intersection_distance();
}
//======================================================================
