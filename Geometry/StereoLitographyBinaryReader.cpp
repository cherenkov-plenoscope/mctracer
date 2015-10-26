#include "StereoLitographyIo.h"
#include "Triangle.h"
//------------------------------------------------------------------------------
namespace StereoLitographyIo {
//------------------------------------------------------------------------------
BinaryReader::BinaryReader(const std::string _filename):
	filename(_filename)
{
	start();
}
//------------------------------------------------------------------------------
BinaryReader::BinaryReader(
	const std::string _filename, 
	const double _scaling_factor
):	filename(_filename)
{
	scaling_factor = _scaling_factor;
	start();
}
//------------------------------------------------------------------------------
void BinaryReader::start() {

	object = new Frame("stl_file", Vector3D::null, Rotation3D::null);

	stl_file.open(filename, std::ios::binary);
		assert_file_is_open();
		read_header();
		read_total_number_of_triangles();
		read_triangles();
	stl_file.close();

	object->init_tree_based_on_mother_child_relations();
	report_bad_attribute_count();
}
//------------------------------------------------------------------------------
void BinaryReader::read_header() {
	stl_header = read_chars(header_size_in_chars);
	assert_is_no_ascii_format();
}
//------------------------------------------------------------------------------
void BinaryReader::read_total_number_of_triangles() {
	total_number_of_triangles = read_single_uint32();
} 
//------------------------------------------------------------------------------
void BinaryReader::report_bad_attribute_count()const {
	if(triangles_with_bad_attribute_count.size()>0) {
		
		std::stringstream info;
	    info << __FILE__ << " " << __LINE__ << "\n";
		info << "BinaryReader: in file '" << filename << "'\n";
		info << "The attribute_byte_count is not zero in ";
		info << triangles_with_bad_attribute_count.size() << " of ";
		info << total_number_of_triangles <<" triangles\n";
		info << "I do not know what attribute_byte_count means but\n";
		info << "the STL standart says it should be zero for each triangle.\n";
		
		std::cout << " __warning__\n";
		std::cout << StringTools::place_first_infront_of_each_new_line_of_second(
			"| ", info.str()
		);
		std::cout << "|___________\n";
	}	
}
//------------------------------------------------------------------------------
void BinaryReader::assert_is_no_ascii_format()const {

	if(stl_header_implies_ascii_format()) {
		std::stringstream info;
	    info << __FILE__ << " " << __LINE__ << "\n";
		info << "BinaryReader: Unable to read file: '" << filename << "'\n";
		info << "Expected binary file, but actual header implies ascii format.\n";
		throw CanNotReadAscii(info.str());
	}	
}
//------------------------------------------------------------------------------
void BinaryReader::assert_file_is_open()const {

	if(!stl_file.is_open()) {
		std::stringstream info;
	    info << __FILE__ << " " << __LINE__ << "\n";
		info << "BinaryReader: Unable to open file: '" << filename << "'\n";
		throw CanNotReadFile(info.str());	 	
	}
}
//------------------------------------------------------------------------------
std::vector<float> BinaryReader::read_floats(const uint n) {

    std::vector<float> block(n);
    stl_file.read((char*)block.data(), block.size()*sizeof(float));
    return block;
}
//------------------------------------------------------------------------------
std::string BinaryReader::read_chars(const uint n) {

    std::vector<char> block(n);
    stl_file.read((char*)block.data(), block.size()*sizeof(char));
	std::string str(block.begin(),block.end());
    return str;;
}
//------------------------------------------------------------------------------
uint32_t BinaryReader::read_single_uint32() {

    uint32_t number;
    stl_file.read((char*)&number, sizeof(uint32_t));
	return number;
}
//------------------------------------------------------------------------------
uint16_t BinaryReader::read_single_uint16() {

    uint16_t number;
    stl_file.read((char*)&number, sizeof(uint16_t));
	return number;
}
//------------------------------------------------------------------------------
std::string BinaryReader::get_print()const {

	std::stringstream out;
    out << "BinaryReader(" << filename << ")\n";
    out << "header:\n";
    out << StringTools::place_first_infront_of_each_new_line_of_second(
    	"  ",
    	stl_header
    );
    out << "number of triangles: " << total_number_of_triangles << "\n";
    return out.str();
}
//------------------------------------------------------------------------------
void BinaryReader::read_triangles() {

	for(uint32_t i=0; i<total_number_of_triangles; i++)
		object->set_mother_and_child(read_and_create_next_triangle());
}
//------------------------------------------------------------------------------
Frame* BinaryReader::read_and_create_next_triangle() {
	
	current_triangle_number++;

	// 12 floats
	std::vector<float> normal_and_3_vertexes = 
		read_floats(triangle_size_in_32bit_floats);

	// 2byte short
	uint16_t attribute_byte_count = read_single_uint16();
	check_attribute_byte_count_is_zero(attribute_byte_count);

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
void BinaryReader::check_attribute_byte_count_is_zero(
	const uint16_t attribute_byte_count
) {
	if(attribute_byte_count != 0)
		triangles_with_bad_attribute_count.push_back(current_triangle_number);
}
//------------------------------------------------------------------------------
void BinaryReader::assert_normal_is_actually_normalized(const Vector3D normal) {
	
	double norm = normal.norm();
	double deviation = fabs(norm - 1.0);

	if(deviation > 1e-3) {
		std::stringstream info;
	   	info << __FILE__ << " " << __LINE__ << "\n";
		info << "BinaryReader: in file '" << filename << "', triangle number ";
		info << current_triangle_number << " of ";
		info << total_number_of_triangles << "\n";
		info << "The normal vector " << normal;
		info << " is not exactly normalized.\n";
		info << "STL header:\n";
		info << stl_header << "\n";
		throw BadNormal(info.str());	 	
	}
}
//------------------------------------------------------------------------------
std::string BinaryReader::get_current_triangle_name() {
	std::stringstream name;
	name << "triangle_" << current_triangle_number;
	return name.str();
}
//------------------------------------------------------------------------------
Frame* BinaryReader::get_frame()const {
	return object;
}
//------------------------------------------------------------------------------
uint BinaryReader::get_number_of_triangles()const {
	return current_triangle_number;
}
//------------------------------------------------------------------------------
bool BinaryReader::stl_header_implies_ascii_format()const {
	return StringTools::is_equal("solid", stl_header.substr(0,5));
}
//------------------------------------------------------------------------------
std::string BinaryReader::get_header()const {
	return stl_header;
}
//------------------------------------------------------------------------------
} // StereoLitographyIo