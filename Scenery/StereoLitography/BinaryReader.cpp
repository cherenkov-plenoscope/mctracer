// Copyright 2015 Sebastian A. Mueller
#include "StereoLitography.h"
#include <math.h>
#include <sstream>
#include "Core/binary_input_output.h"
#include "Scenery/Primitive/Triangle.h"
using std::string;
using std::vector;

namespace relleums {
namespace StereoLitography {

BinaryReader::BinaryReader(const string _filename):
    filename(_filename) {
    start();
}

void BinaryReader::start() {
    fin.open(filename, std::ios::binary);
        assert_file_is_open();
        read_header();
        read_total_number_of_facets();
        read_facets();
    fin.close();
}

void BinaryReader::read_header() {
    stl_header = read_chars(HEADER_SIZE_BYTES);
    assert_is_no_ascii_format();
}

void BinaryReader::read_total_number_of_facets() {
    total_number_of_facets = bio::read_uint32(fin);
}

string BinaryReader::get_report()const {
    std::stringstream info, out;
    info << __FILE__ << " " << __LINE__ << "\n";
    info << "BinaryReader: in file '" << filename << "'\n";

    if (facets_with_bad_attribute_count.size() > 0) {
        info << "The attribute_byte_count is not zero in ";
        info << facets_with_bad_attribute_count.size() << " of ";
        info << total_number_of_facets <<" facets\n";
        info << "I do not know what attribute_byte_count means but\n";
        info << "the STL standart says it should be zero for each triangle.\n";
    } else {
        info << "OK\n";
    }

    out << " __fin_info__\n";
    out << txt::place_first_infront_of_each_new_line_of_second(
        "| ", info.str());
    out << "|___________\n";
    return out.str();
}

void BinaryReader::assert_is_no_ascii_format()const {
    if (stl_header_implies_ascii_format()) {
        std::stringstream info;
        info << __FILE__ << " " << __LINE__ << "\n";
        info << "BinaryReader: Unable to read file: '" << filename << "'\n";
        info << "Expected binary file, but actual header ";
        info << "implies ascii format.\n";
        throw CanNotReadAscii(info.str());
    }
}

void BinaryReader::assert_file_is_open()const {
    if (!fin.is_open()) {
        std::stringstream info;
        info << __FILE__ << " " << __LINE__ << "\n";
        info << "BinaryReader: Unable to open file: '" << filename << "'\n";
        throw CanNotReadFile(info.str());
    }
}

vector<float> BinaryReader::read_floats(const unsigned int n) {
    vector<float> block(n);
    fin.read(reinterpret_cast<char*>(block.data()), block.size()*sizeof(float));
    return block;
}

string BinaryReader::read_chars(const unsigned int n) {
    vector<char> block(n);
    fin.read(block.data(), block.size()*sizeof(char));
    string str(block.begin(), block.end());
    return str;;
}

string BinaryReader::str()const {
    std::stringstream out;
    out << "BinaryReader(" << filename << ")\n";
    out << "header:\n";
    out << txt::place_first_infront_of_each_new_line_of_second(
        "  ",
        stl_header);
    out << "number of facets: " << total_number_of_facets << "\n";
    return out.str();
}

void BinaryReader::read_facets() {
    for (uint32_t i = 0; i < total_number_of_facets; i++)
        facets.push_back(read_and_create_next_facet());
}

Facet BinaryReader::read_and_create_next_facet() {
    current_triangle_number++;

    // 12 floats
    vector<float> normal_and_3_vertexes = read_floats(
        TRIANGLE_SIZE_BYTES);

    // 2byte short
    uint16_t attribute_byte_count = bio::read_uint16(fin);
    check_attribute_byte_count_is_zero(attribute_byte_count);

    Vec3 normal(
        normal_and_3_vertexes[0],
        normal_and_3_vertexes[1],
        normal_and_3_vertexes[2]);

    Vec3 A(
        normal_and_3_vertexes.at(3),
        normal_and_3_vertexes.at(4),
        normal_and_3_vertexes.at(5));

    Vec3 B(
        normal_and_3_vertexes.at(6),
        normal_and_3_vertexes.at(7),
        normal_and_3_vertexes.at(8));

    Vec3 C(
        normal_and_3_vertexes.at(9),
        normal_and_3_vertexes.at(10),
        normal_and_3_vertexes.at(11));

    Facet facet = {normal, A, B, C};
    return facet;
}

vector<Facet> BinaryReader::get_facets()const {
    return facets;
}

void BinaryReader::check_attribute_byte_count_is_zero(
    const uint16_t attribute_byte_count
) {
    if (attribute_byte_count != 0)
        facets_with_bad_attribute_count.push_back(current_triangle_number);
}

void BinaryReader::assert_normal_is_actually_normalized(const Vec3 normal) {
    double norm = normal.norm();
    double deviation = fabs(norm - 1.0);
    if (deviation > 1e-3) {
        std::stringstream info;
        info << __FILE__ << " " << __LINE__ << "\n";
        info << "BinaryReader: in file '" << filename << "', triangle number ";
        info << current_triangle_number << " of ";
        info << total_number_of_facets << "\n";
        info << "The normal vector " << normal.str();
        info << " is not exactly normalized.\n";
        info << "STL header:\n";
        info << stl_header << "\n";
        throw BadNormal(info.str());
    }
}

unsigned int BinaryReader::get_number_of_facets()const {
    return current_triangle_number;
}

bool BinaryReader::stl_header_implies_ascii_format()const {
    return txt::is_equal("solid", stl_header.substr(0, 5));
}

string BinaryReader::get_header()const {
    return stl_header;
}

}  // namespace StereoLitography
}  // namespace relleums
