#include "Ray.h"
//------------------------------------------------------------------------------
Ray::Ray(){
}
//------------------------------------------------------------------------------
Ray::Ray(const Vector3D support, const Vector3D direction){
	SetRay(support, direction);
}
//------------------------------------------------------------------------------
void Ray::SetID(unsigned long long int nID){
	identifier_number = nID;
}
//------------------------------------------------------------------------------
unsigned long long int Ray::ID()const{
	return identifier_number;
}
//------------------------------------------------------------------------------
void Ray::SetRay(const Vector3D nsup,const Vector3D ndir){
	support = nsup;
	direction  = ndir;
	normalize_direction();
}
//------------------------------------------------------------------------------
void Ray::SetSupport(const double x,const double y,const double z){
	support.set(x,y,z);
}
//------------------------------------------------------------------------------
void Ray::SetSupport(const Vector3D nsup){
	support = nsup;
}
//------------------------------------------------------------------------------
void Ray::SetDirection(const double x,const double y,const double z){
	direction.set(x,y,z);
	normalize_direction();
}
//------------------------------------------------------------------------------
void Ray::SetDirection(const Vector3D ndir){
	direction = ndir;
	normalize_direction();
}
//------------------------------------------------------------------------------
void Ray::disp()const{
	std::cout << "Ray -> " << get_string() << "\n";
}
//------------------------------------------------------------------------------
void Ray::normalize_direction(){
	direction.normalize();
}
//------------------------------------------------------------------------------
std::string Ray::get_string()const {
	std::stringstream out;
	out << "support: " << support << ", direction: " << direction;
	return out.str();
}
//------------------------------------------------------------------------------
Vector3D Ray::PositionOnRay(const double scalar)const{
	return support + direction*scalar;
}
//------------------------------------------------------------------------------
Vector3D Ray::Support()const{
	return support;
}
//------------------------------------------------------------------------------
Vector3D Ray::Direction()const{
	return direction;
}
//------------------------------------------------------------------------------
void Ray::operator= (Ray eqray){
	support = eqray.support;
	direction  = eqray.direction;
}
//------------------------------------------------------------------------------
void Ray::homo_transformation_of_ray(Ray* ray,const HomoTrafo3D *T)const{
	//transform support vector
	T->transform_position(&ray->support);
	//transform direction
	T->transform_orientation(&ray->direction);
}
//------------------------------------------------------------------------------
void Ray::find_intersection_candidates_in_tree_of_frames(
	const CartesianFrame* frame, 
	std::vector<const CartesianFrame*> *frames_with_intersection_in_bounding_sphere
)const{
	if(has_intersection_with_bounding_sphere_of(frame)) {

		if(frame->has_children()) {

			if(frame->uses_oct_trees_to_store_its_children()) {
				find_intersections_for_children_in_oct_trees(
					frame,
					frames_with_intersection_in_bounding_sphere
				);
			}else{
				find_intersections_for_all_children_on(
					frame,
					frames_with_intersection_in_bounding_sphere
				);
			}
		}else{
			frames_with_intersection_in_bounding_sphere->push_back(frame);		
		}
	}
}
//------------------------------------------------------------------------------
void Ray::find_intersections_for_children_in_oct_trees(	
	const CartesianFrame* frame, 
	std::vector<const CartesianFrame*> *frames_with_intersection_in_bounding_sphere
)const {
	const OctTreeCube *OctTreeOfFrame2CheckForIntersectionOfRayAndMaxSphere = 
	frame->get_OctTree();
	
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
			frame->
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
			find_intersection_candidates_in_tree_of_frames(FrameFoundInOctTree,
				frames_with_intersection_in_bounding_sphere
			);
		}
	}
}
//------------------------------------------------------------------------------
void Ray::find_intersections_for_all_children_on(
	const CartesianFrame* frame, 
	std::vector<const CartesianFrame*> *frames_with_intersection_in_bounding_sphere
)const {
	for(
		uint child_itterator=0; 
		child_itterator<
		frame->
		get_number_of_children();
		child_itterator++
	){
		find_intersection_candidates_in_tree_of_frames(
			frame->
			get_pointer_to_child(child_itterator),
			frames_with_intersection_in_bounding_sphere
		);
	}
}
//------------------------------------------------------------------------------
void OctTreeTraversingRay::IntersectionCandidatesInOctTree(
	const OctTreeCube *Cube,
	std::unordered_set<CartesianFrame*> *IntersectionCandidates
)const{
	// Check whether this Cube is a leaf or has children
	//if(Cube->ChildCubes.size()==0){
	if(Cube->has_child_cubes())
		// There are more sub cubes in this OctTree
		for(OctTreeCube* const ChildCube : Cube->get_child_cubes()){
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
	}else{
		// There are no more sub Cubes in this tree
		// This cube is a leaf of the tree so all the Frames in this leaf 
		// are possible hit candidates and are added to the 
		// IntersectionCandidates
		for(CartesianFrame* const FrameInChildCube : Cube->get_child_frames()) {
			IntersectionCandidates->insert(FrameInChildCube);
		}
		// here in the leaf of the OctTree the recursion ends.
	}
}
//------------------------------------------------------------------------------
bool OctTreeTraversingRay::IntersectionWithOctTreeCube(
	const OctTreeCube* Cube
)const{
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
//------------------------------------------------------------------------------
double Ray::get_parameter_on_ray_for_closest_distance_to_point(
	const Vector3D &point
)const {
	// We create a plane orthogonal to this ray the point
	// plane equation:
	// 	d = x*a + y*b + z*c
	//
	// We set the normal vector n of the plane to the ray's direction vector:
	//  a=direction.x b=direction.y c=direction.z
	//
	// Now we insert the support vevtor of the frame into the plane eqaution:
	//  d = point.x*dirx + point.y*diry + point.z*dirz
	double d = direction * point;
	
	// Insert the ray into plane equation and solve for the ray parameter
	return (d - support * direction) / (direction * direction);
}
//------------------------------------------------------------------------------
double Ray::get_distance_to_point_from_position_of_ray_at(
	const Vector3D &point, const double ray_parameter_for_position_on_ray
)const {
	// Calculate point Q. Q is on the ray and in the plane. So Q is the 
	// closest point on the ray to the center of the frame
	Vector3D Q = PositionOnRay(ray_parameter_for_position_on_ray);

	// Calculate the connection vector between Q and and the center of the
	// frame called W
	Vector3D W = point - Q;
	
	// Calculate length of W. The lenght of W is the closest distance
	// between ray and object
	return W.norm2();  
}
//------------------------------------------------------------------------------
bool Ray::has_intersection_with_bounding_sphere_of(const CartesianFrame* frame)const{

	double ray_parameter_for_closest_distance_to_center_of_bounding_sphere = 
		get_parameter_on_ray_for_closest_distance_to_point(
			*frame->get_pointer_to_position_of_frame_in_world_frame()
		);

	double distance_to_center_of_bounding_sphere = 
		get_distance_to_point_from_position_of_ray_at(
			*frame->get_pointer_to_position_of_frame_in_world_frame(),
			ray_parameter_for_closest_distance_to_center_of_bounding_sphere
		);

	if(distance_to_center_of_bounding_sphere > 
		frame->get_radius_of_sphere_enclosing_all_children()
	) {
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
			if(support_of_ray_is_inside_of_bounding_sphere_of(frame)) {
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
bool Ray::support_of_ray_is_inside_of_bounding_sphere_of(
	const CartesianFrame *frame
)const {
	double distance_between_support_and_frame = 
		( 
			support*(-1.0) +
			*frame->get_pointer_to_position_of_frame_in_world_frame()
		).norm2();

	return (
		distance_between_support_and_frame
		< 
		frame->get_radius_of_sphere_enclosing_all_children() 
	);
}
//------------------------------------------------------------------------------
void Ray::disp_possible_hit_list(const CartesianFrame *frame)const{
	
	std::vector<const CartesianFrame*> list_of_possible_hits;
	find_intersection_candidates_in_tree_of_frames(frame,&list_of_possible_hits);
	
	std::stringstream out;
	out << "list of possible hit candidates:\n";
	out << get_string();

	for(unsigned int i=0; i<list_of_possible_hits.size(); i++)
	{
		out << (i+1) << ". " << list_of_possible_hits.at(i)->get_name_of_frame();
		
		const CartesianFrame * ptr_to_mother_frame = 
			list_of_possible_hits.at(i)->get_pointer_to_mother_frame();
		
		while(ptr_to_mother_frame != NULL) {
			out << " -> " << ptr_to_mother_frame->get_name_of_frame();
			
			ptr_to_mother_frame = 
			ptr_to_mother_frame->get_pointer_to_mother_frame();
		}
		out<<std::endl;
	}
	std::cout<<out.str();
}
//------------------------------------------------------------------------------
void Ray::find_intersections_in_intersection_candidate_frames(
	std::vector<const CartesianFrame*> *objects_which_might_intersect,
	std::vector<Intersection*> *intersections_taking_place,
  	const CartesianFrame* object_propagated_from
)const{

	std::vector<Intersection*> intersections_which_might_take_place;

	for(const CartesianFrame* object : *objects_which_might_intersect) {

		// transform ray into object system using homogenous transformation
		Ray ray_in_object_system(support,direction);
		
		homo_transformation_of_ray(
			&ray_in_object_system,
			object->world2frame()
		);
		
		// requesting hit in corresponding object
		// a virtual hit in CartesianFrame decleration is needed to do this
		Intersection* intersection_which_might_take_place;
		intersection_which_might_take_place = new Intersection;
		
		intersections_which_might_take_place.
		push_back(intersection_which_might_take_place);
		
		object->hit(	
			&ray_in_object_system.support,	
			&ray_in_object_system.direction,
			intersections_which_might_take_place.back()
		);
		
		// add object to list of hits when hit_flag == true

		if(intersections_which_might_take_place.back()->get_intersection_flag()
		) {
			if(object != object_propagated_from) {
				intersections_taking_place->push_back(
					intersections_which_might_take_place.back()
				);
			}else{
				intersections_which_might_take_place.
					back()->set_intersection_flag(false);
			}
		}
	}
	// free mempory
	for(Intersection *intersection_which_might_take_place : intersections_which_might_take_place
	) {
		if(!intersection_which_might_take_place->get_intersection_flag()) {
			delete intersection_which_might_take_place;
		}
	}

}
//------------------------------------------------------------------------------
void Ray::calculate_reflected_ray(	
	const Intersection * closest_intersection,
	Ray *ray_reflection_on_object
)const{
	// transform ray_in_object_system into the system of this 
	// particular frame.
	Ray ray_in_object_system;
	ray_in_object_system.SetRay(support,direction);

	homo_transformation_of_ray(
		&ray_in_object_system,
		closest_intersection->
			get_pointer_to_intersecting_object()->world2frame()
	);
	
	Vector3D refl_dir = ray_in_object_system.direction;
	// mirror
	closest_intersection->
		get_reflection_direction_in_object_system(&refl_dir);

	Vector3D refl_sup;
	closest_intersection->
		get_intersection_vec_in_object_system(&refl_sup);
	
	ray_reflection_on_object->SetRay(refl_sup,refl_dir);

	// calculate reflection ray in world system
	homo_transformation_of_ray(
		ray_reflection_on_object,
		closest_intersection->
			get_pointer_to_intersecting_object()->frame2world()
	);	
}
//------------------------------------------------------------------------------
Intersection* Ray::calculate_closest_intersection(	
		std::vector<Intersection*> *intersections
)const{
	// In a list of valid intersectios with this ray and some frames we look 
	// for the intersection which is closest to the support vector of the ray.
	// We return the intersection closest to the support of the vector.
	std::vector<Intersection*>::iterator closest_intersection = min_element( 	
		intersections->begin(),
		intersections->end() ,
		*this
	);

	return 	*closest_intersection;
}
//------------------------------------------------------------------------------
Intersection* Ray::get_closest_intersection(
	const CartesianFrame* world,
	const GlobalSettings *settings
)const {
	std::vector<const CartesianFrame*> objects_which_might_intersect;
	find_intersection_candidates_in_tree_of_frames( 
		world , 
		&objects_which_might_intersect
	);
	
	std::vector<Intersection*> intersections;
	
	const CartesianFrame* object_propagated_from = NULL;
	find_intersections_in_intersection_candidate_frames(
		&objects_which_might_intersect,
		&intersections,
		object_propagated_from
	);
		
	if( intersections.size() == 0 ){
		Intersection *false_intersection;
		false_intersection = new Intersection;
		false_intersection->set_intersection_flag( false );
		return false_intersection;
	}else{
		return calculate_closest_intersection(&intersections);
	}
}
//------------------------------------------------------------------------------
std::vector<Intersection*> Ray::get_intersections(
	const CartesianFrame* world,
	const CartesianFrame* object_propagated_from
)const{

	std::vector<const CartesianFrame*> objects_which_might_intersect;
	find_intersection_candidates_in_tree_of_frames( 
		world, 
		&objects_which_might_intersect 
	);
	
	std::vector<Intersection*> intersections;
	
	find_intersections_in_intersection_candidate_frames(
		&objects_which_might_intersect,
		&intersections,
		object_propagated_from
	);

	return intersections;
}
//------------------------------------------------------------------------------
void Ray::delete_intersections(std::vector<Intersection*> &Intersections)const {
	for(Intersection *an_intersection : Intersections) {	
		delete an_intersection;
	}
}
//------------------------------------------------------------------------------
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
//------------------------------------------------------------------------------
double Ray::get_distance_to_closest_object(
	const CartesianFrame *world,
	const GlobalSettings *settings
)const{
	Intersection *closest = get_closest_intersection(world, settings);

	if(closest->get_intersection_flag()) {
		return closest->get_intersection_distance();
	}else{
		return 0.0;
	}
}
//------------------------------------------------------------------------------
bool Ray::operator() (Intersection* one, Intersection* two)const{
	return 	one->get_intersection_distance() < two->get_intersection_distance();
}
//------------------------------------------------------------------------------
CsvRow Ray::getRayCsvRow(GlobalSettings& settings)const{
	
	CsvRow row;

	if( settings.ShowCsvIdentity() )
		row.append( getCsvID() );

	row.append( support.getCsvRow(settings) );
	row.append(  direction.getCsvRow(settings) );

	return row;
}
//------------------------------------------------------------------------------
CsvRow Ray::getCsvRow(GlobalSettings& settings)const{

	CsvRow row;

	if( settings.ShowCsvIdentifier() == true )
		row.push_back("Ray");

	row.append( getRayCsvRow(settings) );
	
	return row;
}
//------------------------------------------------------------------------------
CsvRow Ray::getCsvID()const{

	CsvRow rowID;

	rowID.push_back(
		static_cast<ostringstream*>
		( &(ostringstream() << identifier_number ) )->str()
	);

	return rowID;
}
//------------------------------------------------------------------------------
void Ray::SetHistory(ListOfInteractions* nhistory){
	history = nhistory;
}
//------------------------------------------------------------------------------
ListOfInteractions* Ray::GetHistory()const{
	return history;
}
//------------------------------------------------------------------------------
CsvRow Ray::getCsvRowHistory(GlobalSettings& settings)const{
	
	CsvRow row;

	if( settings.ShowCsvIdentity() )
		row.append( getCsvID() );	

	row.append( history->getCsvRow(settings) );

	return row;
}
//------------------------------------------------------------------------------
// friends of osstream
//------------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& os, const Ray& ray_to_be_displayed){
    os << ray_to_be_displayed.get_string();
    return os;
}

