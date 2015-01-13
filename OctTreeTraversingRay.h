//=================================
// include guard
#ifndef __OCTTREETRAVERSINGRAY_H_INCLUDE__
#define __OCTTREETRAVERSINGRAY_H_INCLUDE__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Ray.h"

//======================================================================
// A special Ray to traverse the Oct Tree Structure
//======================================================================
// here additional signum and inverse direction information are stored 
// with in the ray to speed up the Oct Tree traversal. Idea is taken from 
//
//	  An Efficient and Robust Ray–Box Intersection Algorithm
//	Amy Williams, Steve Barrus, R. Keith Morley, Peter Shirley
//	                   University of Utah

class OctTreeTraversingRay : public Ray{

	Vector3D inv_dir;
	int sign[3];
public:
	OctTreeTraversingRay(const Ray *r);

	void IntersectionCandidatesInOctTree(
		const OctTreeCube *Cube,
		std::unordered_set<CartesianFrame*> *IntersectionCandidates
	)const;

	bool IntersectionWithOctTreeCube(const OctTreeCube* Cube)const;

	void update();
};
#endif // __OCTTREETRAVERSINGRAY_H_INCLUDE__ 