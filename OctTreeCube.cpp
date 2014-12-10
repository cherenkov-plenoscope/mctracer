#include "OctTreeCube.h"
#include "CartesianFrame.h"
//------------------------------------------------------------------------------
OctTreeCube::OctTreeCube(
    const Vector3D CenterPosition, const double LengthOfEdge
) {
    this->CenterPosition = CenterPosition;
    this->EdgeLength = LengthOfEdge;

    set_limits(CenterPosition); 
}
//------------------------------------------------------------------------------
void OctTreeCube::set_limits(const Vector3D CenterPosition) {
    limits[0][0] = CenterPosition.x() - half_edge_length();
    limits[0][1] = CenterPosition.x() + half_edge_length();

    limits[1][0] = CenterPosition.y() - half_edge_length();
    limits[1][1] = CenterPosition.y() + half_edge_length();

    limits[2][0] = CenterPosition.z() - half_edge_length();
    limits[2][1] = CenterPosition.z() + half_edge_length();
} 
//------------------------------------------------------------------------------
double OctTreeCube::half_edge_length()const{
    return EdgeLength/2.0;
}
//------------------------------------------------------------------------------
void OctTreeCube::fill(
    const std::vector<CartesianFrame*> &ChildrenToFillIn
) {
    ChildFrames = ChildrenToFillIn;

    if(there_are_so_many_frmaes_that_we_need_sub_cubes(ChildFrames))
        fill_frames_in_additional_sub_cubes(ChildFrames);
}
//------------------------------------------------------------------------------
bool OctTreeCube::there_are_so_many_frmaes_that_we_need_sub_cubes(
    const std::vector<CartesianFrame*> &frames
)const {
    return (frames.size() > max_number_of_frames_in_single_OctTreeCube);
}
//------------------------------------------------------------------------------
void OctTreeCube::fill_frames_in_additional_sub_cubes(
    const std::vector<CartesianFrame*> &frames_to_fill_in
) {
   for(uint x=0; x<2; x++) {
        for(uint y=0; y<2; y++) {
            for(uint z=0; z<2; z++) {

                OctTreeCube * sub_cube = create_sub_cube_at_index_xyz(x, y, z);

                std::vector<CartesianFrame*> SubSetOfFrames = 
                    CalculateSubSetOfFramesInCube(frames_to_fill_in);

                sub_cube->fill(SubSetOfFrames);

                ChildCubes.push_back(sub_cube);
            }
        }
    }
}
//------------------------------------------------------------------------------
OctTreeCube* OctTreeCube::create_sub_cube_at_index_xyz(
    const uint x, const uint y, const uint z)const {
    Vector3D ChildCubesCenterPosition = 
        position_of_child_for_index_xyz(x, y, z);

    OctTreeCube * sub_cube;
    sub_cube = new OctTreeCube(
        ChildCubesCenterPosition,
        half_edge_length()
    );

    return sub_cube;
}
//------------------------------------------------------------------------------
Vector3D OctTreeCube::position_of_child_for_index_xyz(uint x,uint y,uint z)const{
    
    Vector3D offset(
        -EdgeLength/4.0 + x*EdgeLength/2.0,
        -EdgeLength/4.0 + y*EdgeLength/2.0,
        -EdgeLength/4.0 + z*EdgeLength/2.0
    );

    return CenterPosition + offset;
}
//------------------------------------------------------------------------------
const std::vector<CartesianFrame*> OctTreeCube::CalculateSubSetOfFramesInCube(
    const std::vector<CartesianFrame*> possible_children
){
    std::vector<CartesianFrame*> SubSetOfFrames;

    for(CartesianFrame* child : possible_children)
        add_frame_to_list_when_inside_boundaries(child, SubSetOfFrames);

    return SubSetOfFrames;
}
//------------------------------------------------------------------------------
void OctTreeCube::add_frame_to_list_when_inside_boundaries(
    CartesianFrame *frame, std::vector<CartesianFrame*> &SubSetOfFrames
)const {
    if(is_intersecting_cube_boundary_volume(frame))
        SubSetOfFrames.push_back(frame);   
}
//------------------------------------------------------------------------------
bool OctTreeCube::is_intersecting_cube_boundary_volume(
    CartesianFrame* frame
)const {
    Vector3D position_of_frame_in_mother_frame = 
        *frame->get_pointer_to_position_of_frame_in_mother_frame();

    double radius_of_frame =
        frame->get_radius_of_sphere_enclosing_all_children();

    // range of the bounding sphere of a frame in 
    // its mother frame coordinates

    double Xmin = position_of_frame_in_mother_frame.x() - radius_of_frame;
    double Xmax = position_of_frame_in_mother_frame.x() + radius_of_frame;
    double Ymin = position_of_frame_in_mother_frame.y() - radius_of_frame;
    double Ymax = position_of_frame_in_mother_frame.y() + radius_of_frame;
    double Zmin = position_of_frame_in_mother_frame.z() - radius_of_frame;
    double Zmax = position_of_frame_in_mother_frame.z() + radius_of_frame;

    return (
        (
            Xmax >= limits[0][0] && 
            Xmin <= limits[0][1] 
        ) && (
            Ymax >= limits[1][0] && 
            Ymin <= limits[1][1] 
        ) && (
            Zmax >= limits[2][0] && 
            Zmin <= limits[2][1] 
        ) 
    );    
}
//------------------------------------------------------------------------------
uint OctTreeCube::get_max_number_of_frames_in_cube()const {
    return max_number_of_frames_in_single_OctTreeCube;
}
//------------------------------------------------------------------------------
bool OctTreeCube::has_child_cubes()const {
    return (ChildCubes.size() > 0);
}
//------------------------------------------------------------------------------
const std::vector<OctTreeCube*> OctTreeCube::get_child_cubes()const {
    return ChildCubes;
}
//------------------------------------------------------------------------------
const std::vector<CartesianFrame*> OctTreeCube::get_child_frames()const {
    return ChildFrames;
}
//------------------------------------------------------------------------------
Vector3D OctTreeCube::get_center_position()const {
    return CenterPosition;
}
//------------------------------------------------------------------------------
double OctTreeCube::get_edge_length()const {
    return EdgeLength;
}
//------------------------------------------------------------------------------
double OctTreeCube::get_number_of_child_cubes()const {
    return ChildCubes.size();
}
//------------------------------------------------------------------------------
double OctTreeCube::get_number_of_child_frames()const {
    return ChildFrames.size();
}
//------------------------------------------------------------------------------