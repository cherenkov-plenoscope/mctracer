#include "StereoLitographyIo.h"
#include "Geometry/Triangle.h"
//------------------------------------------------------------------------------
namespace StereoLitographyIo {
//------------------------------------------------------------------------------
BinaryReader::BinaryReader(const std::string _filename):
	filename(_filename)
{
	start();
}
//------------------------------------------------------------------------------
void BinaryReader::start() {

	stl_file.open(filename, std::ios::binary);
		assert_file_is_open();
		read_header();
		read_total_number_of_facets();
		read_facets();
	stl_file.close();
}
//------------------------------------------------------------------------------
void BinaryReader::read_header() {
	stl_header = read_chars(header_size_in_chars);
	assert_is_no_ascii_format();
}
//------------------------------------------------------------------------------
void BinaryReader::read_total_number_of_facets() {
	total_number_of_facets = read_single_uint32();
} 
//------------------------------------------------------------------------------
std::string BinaryReader::get_report()const {
	
	std::stringstream info, out;
	info << __FILE__ << " " << __LINE__ << "\n";
	info << "BinaryReader: in file '" << filename << "'\n";

	if(facets_with_bad_attribute_count.size()>0) {
		
		info << "The attribute_byte_count is not zero in ";
		info << facets_with_bad_attribute_count.size() << " of ";
		info << total_number_of_facets <<" facets\n";
		info << "I do not know what attribute_byte_count means but\n";
		info << "the STL standart says it should be zero for each triangle.\n";
	}else{
		info << "OK\n";
	}

	out << " __STL_file_info__\n";
	out << StringTools::place_first_infront_of_each_new_line_of_second(
		"| ", info.str()
	);
	out << "|___________\n";
	
	return out.str();
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
    out << "number of facets: " << total_number_of_facets << "\n";
    return out.str();
}
//------------------------------------------------------------------------------
void BinaryReader::read_facets() {

	for(uint32_t i=0; i<total_number_of_facets; i++)
		facets.push_back(read_and_create_next_facet());
		
}
//------------------------------------------------------------------------------
Facet BinaryReader::read_and_create_next_facet() {
	
	current_triangle_number++;

	// 12 floats
	std::vector<float> normal_and_3_vertexes = 
		read_floats(triangle_size_in_32bit_floats);

	// 2byte short
	uint16_t attribute_byte_count = read_single_uint16();
	check_attribute_byte_count_is_zero(attribute_byte_count);

	Vec3 normal(
		normal_and_3_vertexes[0],
		normal_and_3_vertexes[1],
		normal_and_3_vertexes[2]
	);

	Vec3 A(
		normal_and_3_vertexes.at(3),
		normal_and_3_vertexes.at(4),
		normal_and_3_vertexes.at(5)
	);

	Vec3 B(
		normal_and_3_vertexes.at(6),
		normal_and_3_vertexes.at(7),
		normal_and_3_vertexes.at(8)
	);

	Vec3 C(
		normal_and_3_vertexes.at(9),
		normal_and_3_vertexes.at(10),
		normal_and_3_vertexes.at(11)
	);

	Facet facet = {normal, A, B ,C};
	return facet;
}
//------------------------------------------------------------------------------
std::vector<Facet> BinaryReader::get_facets()const {
	return facets;
}
//------------------------------------------------------------------------------
void BinaryReader::check_attribute_byte_count_is_zero(
	const uint16_t attribute_byte_count
) {
	if(attribute_byte_count != 0)
		facets_with_bad_attribute_count.push_back(current_triangle_number);
}
//------------------------------------------------------------------------------
void BinaryReader::assert_normal_is_actually_normalized(const Vec3 normal) {
	
	double norm = normal.norm();
	double deviation = fabs(norm - 1.0);

	if(deviation > 1e-3) {
		std::stringstream info;
	   	info << __FILE__ << " " << __LINE__ << "\n";
		info << "BinaryReader: in file '" << filename << "', triangle number ";
		info << current_triangle_number << " of ";
		info << total_number_of_facets << "\n";
		info << "The normal vector " << normal;
		info << " is not exactly normalized.\n";
		info << "STL header:\n";
		info << stl_header << "\n";
		throw BadNormal(info.str());	 	
	}
}
//------------------------------------------------------------------------------
uint BinaryReader::get_number_of_facets()const {
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