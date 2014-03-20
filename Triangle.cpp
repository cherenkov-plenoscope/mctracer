#include "Triangle.h"
//======================
Triangle::Triangle(){
	// default
	A.set_unit_vector_x();
	B.set_unit_vector_y();
	C.set_unit_vector_z();
}
//======================
bool Triangle::set_Triangle(
const Vector3D new_A,
const Vector3D new_B,
const Vector3D new_C){
	
	A = new_A;
	B = new_B;
	C = new_C;	
	
	// test for valid triangle
	Vector3D AB = B-A;
	if(AB.norm2()==0){	
		std::cout<<"Triangle -> set_Triangle() -> ";
		std::cout<<"B - A = Nullvector !"<<std::endl;
		return false;
	}
	Vector3D AC = C-A;
	if(AC.norm2()==0){
		std::cout<<"Triangle -> set_Triangle() -> ";
		std::cout<<"C - A = Nullvector !"<<std::endl;
		return false;
	}
	Vector3D BC = C-B;	
	if(BC.norm2()==0){
		std::cout<<"Triangle -> set_Triangle() -> ";
		std::cout<<"C - B = Nullvector !"<<std::endl;
		return false;
	}	
		
	std::cout<<"Triangle -> set_Triangle() -> ";
	std::cout<<get_Triangle_string()<<std::endl;

	//===================
	// set radius_of_sphere_enclosing_all_children
	//===================
	std::vector<double> dist_corner_to_base;
	// 1)
	dist_corner_to_base.push_back(A.norm2());
	// 2)
	dist_corner_to_base.push_back(B.norm2());
	// 3)
	dist_corner_to_base.push_back(C.norm2());

	// find corner with largets distance to base
	radius_of_sphere_enclosing_all_children = *max_element(	
	dist_corner_to_base.begin() ,
	dist_corner_to_base.end() );

	std::cout<<"Triangle -> set_Triangle() -> ";
	std::cout<<get_Triangle_string();
	std::cout<<" enclosing sphere radius: ";
	std::cout<<radius_of_sphere_enclosing_all_children;
	std::cout<<"[m]"<<std::endl;
	
	return true;
}
//======================
void Triangle::disp()const{
	std::cout<<get_Triangle_string()<<std::endl;
}
//======================
std::string Triangle::get_Triangle_string()const{
	std::stringstream out;
	out<<"Triangle ABC -> A: "<<A<<", B: "<<B<<", C: "<<C;	
	return out.str();	
}
//======================
void Triangle::hit(Vector3D *base,Vector3D *dir, Intersection *intersection)const{
	// Barycentric Technique
	// jimscott@blackpawn.com
	// Point in triangle test
	// http://www.blackpawn.com/texts/pointinpoly/default.html
	// http://en.wikipedia.org/wiki/Barycentric_coordinates_%28mathematics%29

	// calculate normal vector n of plane including triangle ABC
	Vector3D AB = B-A;
	Vector3D AC = C-A;
	Vector3D n = AB.cross_product(AC);
	n = n/n.norm2();
	
	// calculate intersection point P of plane defined by normalvector 
	// n and line given by base + gamma * dir
	
	double gamma = ( (A- (*base)) *n)/((*dir)*n);
	Vector3D P = (*base) + (*dir)*gamma;
	
	if(gamma<=0.0) return; 
	// discard hits behind the ray base 
	// i.e. behind camera or behind light source
	
	Vector3D AP = P - A;
	
	// Compute dot products
	double dot00 = AC*AC;
	double dot01 = AC*AB;
	double dot02 = AC*AP;
	double dot11 = AB*AB;
	double dot12 = AB*AP;
	
	// Compute barycentric coordinates
	double invDenom = 1.0 / (dot00 * dot11 - dot01 * dot01);
	double u = (dot11 * dot02 - dot01 * dot12) * invDenom;
	double v = (dot00 * dot12 - dot01 * dot02) * invDenom;	
	
	if(u >= 0.0 && v >= 0.0 && (u+v)<1.0){
		// valid hid
		intersection->set_intersection_flag(true);
		intersection->set_pointer_to_intersecting_object(this);
		intersection->set_intersection(&P,&n,&gamma);	
	}
	
	//~ std::stringstream out;
	//~ out<<"___________________________________________"<<std::endl;
	//~ out<<" A : "<<A<<std::endl;
	//~ out<<" B : "<<B<<std::endl;	
	//~ out<<" C : "<<C<<std::endl;		
	//~ out<<std::endl;
	//~ out<<" AB : "<<AB<<std::endl;
	//~ out<<" AC : "<<AC<<std::endl;	
	//~ out<<" n  : "<<n<<std::endl;	
	//~ out<<std::endl;
	//~ out<<" base  : "<<*base<<std::endl;
	//~ out<<" dir   : "<<*dir<<" norm2(): "<<dir->norm2()<<"[m]"<<std::endl;
	//~ out<<" gamma : "<<gamma<<std::endl;
	//~ out<<" P     : "<<P<<std::endl;
	//~ out<<std::endl;		
	//~ std::cout<<out.str();
	return;
}
