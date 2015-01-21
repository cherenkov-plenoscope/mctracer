#include "OctTreeTraversingRay.h"
//------------------------------------------------------------------------------
OctTreeTraversingRay::OctTreeTraversingRay(const Ray *r) {
	support = r->Support();
	direction = r->Direction();
}
//------------------------------------------------------------------------------
void OctTreeTraversingRay::IntersectionCandidatesInOctTree(
	const OctTreeCube *Cube,
	std::unordered_set<CartesianFrame*> *IntersectionCandidates
	//std::vector<CartesianFrame*> *IntersectionCandidates
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
			//IntersectionCandidates->push_back(FrameInChildCube);
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
void OctTreeTraversingRay::update(){
	// after any usual transformation of the ray this update has to 
	// be performed to update the additional information which is not 
	// stored or touched with in the usual Ray routines.
	inv_dir.set( 1.0/direction.x() , 1.0/direction.y() , 1.0/direction.z() );

	sign[0] = (inv_dir.x() < 0.0);
	sign[1] = (inv_dir.y() < 0.0);
	sign[2] = (inv_dir.z() < 0.0);
}