// Copyright 2015 Sebastian A. Mueller
#include "Scenery/stereo_litography.h"
#include "Scenery/primitive/Triangle.h"
#include <sstream>
using std::string;
using std::vector;

namespace relleums {
namespace stereo_litography {

void add_stl_to_and_inherit_surface_from_surfac_entity(
    const string path,
    SurfaceEntity* proto,
    const double scale
) {
    BinaryReader reader(path);
    vector<Facet> facets = reader.get_facets();
    unsigned int facet_count = 0;
    for (Facet facet : facets) {
        Triangle* tri = proto->append<Triangle>();
        tri->set_name_pos_rot(
            "triangle_"+std::to_string(facet_count++),
            VEC3_ORIGIN,
            ROT3_UNITY);
        tri->set_normal_and_3_vertecies(
            facet.n,
            facet.a*scale,
            facet.b*scale,
            facet.c*scale);
        tri->take_boundary_layer_properties_from(proto);
    }
}

void add_stl_to_frame(const string path, Frame* proto, const double scale) {
    BinaryReader reader(path);
    vector<Facet> facets = reader.get_facets();
    unsigned int facet_count = 0;
    for (Facet facet : facets) {
        Triangle* tri = proto->append<Triangle>();
        tri->set_name_pos_rot(
            "triangle_"+std::to_string(facet_count++),
            VEC3_ORIGIN,
            ROT3_UNITY);
        tri->set_normal_and_3_vertecies(
            facet.n,
            facet.a*scale,
            facet.b*scale,
            facet.c*scale);
        tri->set_outer_color(&COLOR_GRAY);
        tri->set_inner_color(&COLOR_DARK_GRAY);
    }
}


void BinaryWriter::add_facet_normal_and_three_vertices(
    const Vec3 n,
    const Vec3 a,
    const Vec3 b,
    const Vec3 c) {
    assert_normal_is_actually_normalized(n);
    Facet facet = {n, a, b, c};
    facets.push_back(facet);
}

void BinaryWriter::add_facets(const vector<Facet> additional_facets) {
    for (Facet additional_facet : additional_facets)
        facets.push_back(additional_facet);
}

void BinaryWriter::assert_normal_is_actually_normalized(const Vec3 normal) {
    double norm = normal.norm();
    double deviation = fabs(norm - 1.0);
    if (deviation > 1e-3) {
        std::stringstream info;
        info << __FILE__ << " " << __LINE__ << "\n";
        info << "BinaryWriter: in file '" << filename;
        info << "', triangle number ";
        info << facets.size()+1 << " ";
        info << "The normal vector " << normal.str();
        info << " is not exactly normalized.\n";
        throw BadNormal(info.str());
    }
}

void BinaryWriter::write_to_file(const string _filename) {
    filename = _filename;
    fout.open(filename, std::ios::out | std::ios::binary);
    assert_file_is_open();
    write_stl_header();
    write_number_of_facets(facets.size());
    write_facets();
    fout.close();
}

void BinaryWriter::write_facets() {
    for (Facet facet : facets)
        write_facet(facet);
}

void BinaryWriter::write_facet(const Facet &facet) {
    write_vector(facet.n);
    write_vector(facet.a);
    write_vector(facet.b);
    write_vector(facet.c);
    write_attribute_count(0);
}

void BinaryWriter::write_vector(const Vec3 &vec) {
    float v[3] = {
        static_cast<float>(vec.x),
        static_cast<float>(vec.y),
        static_cast<float>(vec.z)};
    fout.write(reinterpret_cast<char*>(&v), 3*sizeof(float));
}

void BinaryWriter::write_attribute_count(const uint16_t i) {
    fout.write(reinterpret_cast<const char*>(&i), sizeof(uint16_t));
}

void BinaryWriter::write_stl_header() {
    char text[81] =
  "MCTRACER scientific ray tracing by Sebastian Achim Mueller ETH Zurich 2015    <>";
// 0         1         2         3         4         5         6         7         8
// 012345678901234567890123456789012345678901234567890123456789012345678901234567890
    fout.write(reinterpret_cast<const char*>(&text), 80*sizeof(char));
}

void BinaryWriter::write_number_of_facets(const uint32_t i) {
    fout.write(reinterpret_cast<const char*>(&i), 1*sizeof(uint32_t));
}

void BinaryWriter::assert_file_is_open()const {
    if (!fout.is_open()) {
        std::stringstream info;
        info << __FILE__ << " " << __LINE__ << "\n";
        info << "BinaryWriter: Unable to write to file: '" << filename;
        info << "'\n";
        throw CanNotReadFile(info.str());
    }
}



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
    total_number_of_facets = binio::read_uint32(fin);
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
    uint16_t attribute_byte_count = binio::read_uint16(fin);
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



}  // namespace stereo_litography
}  // namespace relleums
