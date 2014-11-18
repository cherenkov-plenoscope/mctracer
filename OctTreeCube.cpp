#include "OctTreeCube.h"
/*
//------------------------------------------------------------------------------
double OctTreeCube::EdgeLengthOfChildCube()const{
    return EdgeLength/2.0;
}
//------------------------------------------------------------------------------
void OctTreeCube::set_OctTree(   
    Vector3D CenterPosition,
    double EdgeLength)
{
    this -> CenterPosition = CenterPosition;
    this -> EdgeLength = EdgeLength;

    limits[0][0] = CenterPosition.x() - EdgeLengthOfChildCube();
    limits[0][1] = CenterPosition.x() + EdgeLengthOfChildCube();

    limits[1][0] = CenterPosition.y() - EdgeLengthOfChildCube();
    limits[1][1] = CenterPosition.y() + EdgeLengthOfChildCube();

    limits[2][0] = CenterPosition.z() - EdgeLengthOfChildCube();
    limits[2][1] = CenterPosition.z() + EdgeLengthOfChildCube();	
}
//------------------------------------------------------------------------------
void OctTreeCube::FillOctTree( 
	const std::vector<CartesianFrame*> ChildrenToFillIn,
	const uint max_number_of_frames_in_OctTree,
	const Vector3D position_of_frame_in_mother_frame,
	const double radius_of_frame
){

	ChildFrames = ChildrenToFillIn;

    if(ChildrenToFillIn.size() > max_number_of_frames_in_OctTree){

        for(uint x=0; x<2; x++){
            for(uint y=0; y<2; y++){
                for(uint z=0; z<2; z++){

                    OctTreeCube * ChildCube;
                    ChildCube = new OctTreeCube;

                    // calculate CubesCenterPosition position of child cube

                    Vector3D ChildCubesCenterPosition = 
                    CalculateCentrePositionOfChildCube(x,y,z);

                    ChildCube->set_OctTree(
                    	ChildCubesCenterPosition,
                    	EdgeLengthOfChildCube()
                    );

                    std::vector<CartesianFrame*> SubSetOfFrames = 
                    ChildCube->CalculateSubSetOfFramesInCube(ChildrenToFillIn);

                    ChildCube->FillOctTree( 
                    	SubSetOfFrames,
                    	max_number_of_frames_in_OctTree,
                    	position_of_frame_in_mother_frame,
                    	radius_of_frame
                    );

                    ChildCubes.push_back(ChildCube);
                }
            }
        }
    }
}
//------------------------------------------------------------------------------
Vector3D OctTreeCube::CalculateCentrePositionOfChildCube(uint x,uint y,uint z
)const{
    
    Vector3D offset(
        -EdgeLength/4.0 + x*EdgeLength/2.0,
        -EdgeLength/4.0 + y*EdgeLength/2.0,
        -EdgeLength/4.0 + z*EdgeLength/2.0
    );

    return CenterPosition + offset;
}
//------------------------------------------------------------------------------
std::vector<CartesianFrame*> OctTreeCube::CalculateSubSetOfFramesInCube(
	const std::vector<CartesianFrame*> possible_children
){

	std::vector<CartesianFrame*> SubSetOfFrames;

    for(CartesianFrame* child : possible_children){

    	Vector3D position_of_frame_in_mother_frame; //= 
    //	*child->get_pointer_to_position_of_frame_in_mother_frame();

    	double radius_of_frame = 1;
    //	*(child->get_pointer_to_radius_of_sphere_enclosing_all_children());

    	// range of the bounding sphere of a frame in 
    	// its mother frame coordinates

    	double Xmin = position_of_frame_in_mother_frame.x() - radius_of_frame;
    	double Xmax = position_of_frame_in_mother_frame.x() + radius_of_frame;
    	double Ymin = position_of_frame_in_mother_frame.y() - radius_of_frame;
    	double Ymax = position_of_frame_in_mother_frame.y() + radius_of_frame;
     	double Zmin = position_of_frame_in_mother_frame.z() - radius_of_frame;
    	double Zmax = position_of_frame_in_mother_frame.z() + radius_of_frame;

    	if( (
    		Xmax >= limits[0][0] && 
    		Xmin <= limits[0][1] ) && (
    		Ymax >= limits[1][0] && 
    		Ymin <= limits[1][1] ) && (
    		Zmax >= limits[2][0] && 
    		Zmin <= limits[2][1] ) 
    	){
    		SubSetOfFrames.push_back(child);
    	}
    }
    return SubSetOfFrames;
}
//------------------------------------------------------------------------------
void OctTreeCube::create_OctTree(){

	OctTree = new OctTreeCube;
	Vector3D center(0.0, 0.0 ,0.0);
	
	SetOctTree(
		OctTree,
		center,
		2.0*radius_of_sphere_enclosing_all_children
	);

	FillOctTree(OctTree,children);

}*/