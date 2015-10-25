#include "StlReader.h"
#include "Triangle.h"
//------------------------------------------------------------------------------
StlReader::StlReader(const std::string _filename):
	filename(_filename)
{
	start();
}
//------------------------------------------------------------------------------
StlReader::StlReader(const std::string _filename, const double _scaling_factor):
	filename(_filename)
{
	scaling_factor = _scaling_factor;
	start();
}
//------------------------------------------------------------------------------
void StlReader::start() {

	stl_file.open(filename, std::ios::binary);
	assert_file_is_open();

	stl_header = read_chars(header_size_in_chars);
	number_of_triangles = read_single_uint32();

	stl_object = new Frame("stl_file", Vector3D::null, Rotation3D::null);
	read_triangles();

	stl_object->init_tree_based_on_mother_child_relations();
}
//------------------------------------------------------------------------------
StlReader::~StlReader() {
	stl_file.close();
}
//------------------------------------------------------------------------------
void StlReader::assert_file_is_open()const {

	if(!stl_file.is_open()) {
		std::stringstream info;
	    info << __FILE__ << " " << __func__ << " " << __LINE__ << "\n";
		info << "StlReader: Unable to open file: '" << filename << "'\n";
		throw CanNotReadSTL(info.str());	 	
	}
}
//------------------------------------------------------------------------------
std::vector<float> StlReader::read_floats(const uint n) {

    std::vector<float> block(n);
    stl_file.read((char*)block.data(), block.size()*sizeof(float));
    return block;
}
//------------------------------------------------------------------------------
std::string StlReader::read_chars(const uint n) {

    std::vector<char> block(n);
    stl_file.read((char*)block.data(), block.size()*sizeof(char));
	std::string str(block.begin(),block.end());
    return str;;
}
//------------------------------------------------------------------------------
uint32_t StlReader::read_single_uint32() {

    uint32_t number;
    stl_file.read((char*)&number, sizeof(uint32_t));
	return number;
}
//------------------------------------------------------------------------------
uint16_t StlReader::read_single_uint16() {

    uint16_t number;
    stl_file.read((char*)&number, sizeof(uint16_t));
	return number;
}
//------------------------------------------------------------------------------
std::string StlReader::get_print()const {

	std::stringstream out;
    out << "StlReader(" << filename << ")\n";
    out << "header:\n";
    out << StringTools::place_first_infront_of_each_new_line_of_second(
    	"  ",
    	stl_header
    );
    out << "number of triangles: " << number_of_triangles << "\n";
    return out.str();
}
//------------------------------------------------------------------------------
void StlReader::read_triangles() {

	for(uint32_t i=0; i<number_of_triangles; i++)
		stl_object->set_mother_and_child(read_and_create_next_triangle());
}
//------------------------------------------------------------------------------
Frame* StlReader::read_and_create_next_triangle() {
	
	current_triangle_number++;

	// 12 floats
	std::vector<float> normal_and_3_vertexes = 
		read_floats(triangle_size_in_32bit_floats);

	// 2byte short
	uint16_t attribute_byte_count = read_single_uint16();
	assert_attribute_byte_count_is_zero(attribute_byte_count);

	Vector3D normal(
		normal_and_3_vertexes[0],
		normal_and_3_vertexes[1],
		normal_and_3_vertexes[2]
	);

	Vector3D A(
		scaling_factor * normal_and_3_vertexes.at(3),
		scaling_factor * normal_and_3_vertexes.at(4),
		scaling_factor * normal_and_3_vertexes.at(5)
	);

	Vector3D B(
		scaling_factor * normal_and_3_vertexes.at(6),
		scaling_factor * normal_and_3_vertexes.at(7),
		scaling_factor * normal_and_3_vertexes.at(8)
	);

	Vector3D C(
		scaling_factor * normal_and_3_vertexes.at(9),
		scaling_factor * normal_and_3_vertexes.at(10),
		scaling_factor * normal_and_3_vertexes.at(11)
	);

	assert_normal_is_actually_normalized(normal);

	Triangle* tri = new Triangle;
	tri->set_name_pos_rot(
		get_current_triangle_name(), 
		Vector3D::null, 
		Rotation3D::null
	);

	tri->set_normal_and_3_vertecies(normal, A, B, C);
	return tri;
}
//------------------------------------------------------------------------------
void StlReader::assert_attribute_byte_count_is_zero(
	const uint16_t attribute_byte_count
) {
	if(attribute_byte_count != 0) {
		std::stringstream info;
	    info << __FILE__ << " " << __func__ << " " << __LINE__ << "\n";
		info << "StlReader: in file '" << filename << "', ";
		info << "triangle number " << current_triangle_number << " ";
		info << "The attribute_byte_count is not zero.\n";
		info << "I do not know what attribute_byte_count means, ";
		info << "but the STL standart says it must be zero for each triangle.\n";
		throw BadAttributeByteCount(info.str());	 	
	}
}
//------------------------------------------------------------------------------
void StlReader::assert_normal_is_actually_normalized(const Vector3D normal) {
	
	double norm = normal.norm();
	double deviation = fabs(norm - 1.0);

	if(deviation > 1e-3) {
		std::stringstream info;
	    info << __FILE__ << " " << __func__ << " " << __LINE__ << "\n";
		info << "StlReader: in file '" << filename << "', triangle number ";
		info << current_triangle_number << " ";
		info << "The normal vector " << normal << " is not exactly normalized.\n";
		throw BadNormal(info.str());	 	
	}
}
//------------------------------------------------------------------------------
std::string StlReader::get_current_triangle_name() {
	std::stringstream name;
	name << "triangle_" << current_triangle_number;
	return name.str();
}
//------------------------------------------------------------------------------
Frame* StlReader::get_frame()const {
	return stl_object;
}
//------------------------------------------------------------------------------
uint StlReader::get_number_of_triangles()const {
	return current_triangle_number;
}