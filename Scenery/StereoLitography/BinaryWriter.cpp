#include "StereoLitography.h"
#include <math.h>
#include <iostream>
#include <sstream>
using std::string;
using std::vector;
//------------------------------------------------------------------------------
namespace StereoLitography {
//------------------------------------------------------------------------------
void BinaryWriter::add_facet_normal_and_three_vertices(
	const Vec3 n,
	const Vec3 a,
	const Vec3 b,
	const Vec3 c
) {
	assert_normal_is_actually_normalized(n);
	Facet facet = {n,a,b,c};
	facets.push_back(facet);
}
//------------------------------------------------------------------------------
void BinaryWriter::add_facets(const vector<Facet> additional_facets) {
	for(Facet additional_facet : additional_facets)
		facets.push_back(additional_facet);
}
//------------------------------------------------------------------------------
void BinaryWriter::assert_normal_is_actually_normalized(const Vec3 normal) {
	
	double norm = normal.norm();
	double deviation = fabs(norm - 1.0);

	if(deviation > 1e-3) {
		std::stringstream info;
	   	info << __FILE__ << " " << __LINE__ << "\n";
		info << "BinaryWriter: in file '" << filename << "', triangle number ";
		info << facets.size()+1 << " ";
		info << "The normal vector " << normal.str() << " is not exactly normalized.\n";
		throw BadNormal(info.str());	 	
	}
}
//------------------------------------------------------------------------------
void BinaryWriter::write_to_file(const string _filename) {
	
	filename = _filename;

	file.open(filename, std::ios::out | std::ios::binary);
	assert_file_is_open();
	write_stl_header();
	write_number_of_facets(facets.size());
	write_facets();
	file.close();
}
//------------------------------------------------------------------------------
void BinaryWriter::write_facets() {
	for(Facet facet : facets)
		write_facet(facet);
}
//------------------------------------------------------------------------------
void BinaryWriter::write_facet(const Facet &facet) {

	write_vector(facet.n);
	write_vector(facet.a);
	write_vector(facet.b);
	write_vector(facet.c);
	write_attribute_count(0);
}
//------------------------------------------------------------------------------
void BinaryWriter::write_vector(const Vec3 &vec) {

	float v[3] = {float(vec.x), float(vec.y), float(vec.z)};
	file.write( (char*)&v, 3*sizeof(float));
}
//------------------------------------------------------------------------------
void BinaryWriter::write_attribute_count(const uint16_t i) {
	file.write( (char*)&i, sizeof(uint16_t));
}
//------------------------------------------------------------------------------
void BinaryWriter::write_stl_header() {

	char text[81] = 
	  "MCTRACER scientific ray tracing by Sebastian Achim Mueller ETH Zurich 2015    <>";	
	//0         1         2         3         4         5         6         7         8
	//012345678901234567890123456789012345678901234567890123456789012345678901234567890
	file.write( (char*)&text, 80*sizeof(char));
}
//------------------------------------------------------------------------------
void BinaryWriter::write_number_of_facets(const uint32_t i) {
    file.write((char*)&i, 1*sizeof(uint32_t));
}
//------------------------------------------------------------------------------
void BinaryWriter::assert_file_is_open()const {

	if(!file.is_open()) {
		std::stringstream info;
	    info << __FILE__ << " " << __LINE__ << "\n";
		info << "BinaryWriter: Unable to write to file: '" << filename << "'\n";
		throw CanNotReadFile(info.str());	 	
	}
}
//------------------------------------------------------------------------------
} // StereoLitography