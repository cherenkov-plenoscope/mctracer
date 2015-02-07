#include "Ray.h"
//------------------------------------------------------------------------------
Ray::Ray(){
}
//------------------------------------------------------------------------------
Ray::Ray(const Ray* ray) {
	SetRay(ray->support, ray->direction);
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
void Ray::normalize_direction(){
	direction.normalize();
}
//------------------------------------------------------------------------------
std::string Ray::get_print()const {
	return get_ray_print();
}
//------------------------------------------------------------------------------
std::string Ray::get_ray_print()const {
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
	T->transform_position(&ray->support);
	T->transform_orientation(&ray->direction);
}
//------------------------------------------------------------------------------
void Ray::transform(const HomoTrafo3D *T) {
	T->transform_position(&support);
	T->transform_orientation(&direction);	
}
//------------------------------------------------------------------------------
double Ray::get_parameter_on_ray_for_closest_distance_to_point(
	const Vector3D &point
)const {
	// We create a plane orthogonal to this ray and containing the point
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
bool Ray::operator() (Intersection* one, Intersection* two)const {
	return 	one->get_intersection_distance() < two->get_intersection_distance();
}
//------------------------------------------------------------------------------
void Ray::SetHistory(ListOfInteractions* history) {
	this->history = history;
}
//------------------------------------------------------------------------------
ListOfInteractions* Ray::GetHistory()const {
	return history;
}
//------------------------------------------------------------------------------
Ray Ray::get_ray_transformed_in_object_system_of(const CartesianFrame* frame)const {
	Ray ray_in_object_system_of_frame(this);
	ray_in_object_system_of_frame.transform(frame->world2frame());
	return ray_in_object_system_of_frame;
}
//------------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& os, const Ray& ray_to_be_displayed) {
    os << ray_to_be_displayed.get_print();
    return os;
}