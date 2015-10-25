//=================================
// include guard
#ifndef __StlReader_H_INCLUDE__
#define __StlReader_H_INCLUDE__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <iostream> 
#include <string>
#include "TracerException.h"
#include "Tools/Tools.h"
#include "Tools/FileTools.h"
#include "Tools/StringTools.h"
#include <fstream>
#include <stdio.h>
#include <string.h>
#include "Core/Frame.h"

//=================================
/* 
https://en.wikipedia.org/wiki/STL_%28file_format%29#Binary_STL

__Binary STL (STereoLithography)__

A binary STL file has an 80-character header (which is generally ignored, 
but should never begin with "solid" because that will lead most software to 
assume that this is an ASCII STL file[citation needed]). Following the header 
is a 4-byte unsigned integer indicating the number of triangular facets in the 
file. Following that is data describing each triangle in turn. The file simply 
ends after the last triangle.

Each triangle is described by twelve 32-bit floating-point numbers: three for 
the normal and then three for the X/Y/Z coordinate of each vertex – just as 
with the ASCII version of STL. After these follows a 2-byte ("short") unsigned 
integer that is the "attribute byte count" – in the standard format, this 
should be zero because most software does not understand anything else.[6]

Floating-point numbers are represented as IEEE floating-point numbers and are 
assumed to be little-endian, although this is not stated in documentation.
*/
class StlReader {

	const std::string filename;
	const uint header_size_in_chars = 80;
	const uint triangle_size_in_32bit_floats = 12;
    std::ifstream stl_file;

    /* 	
    The STL file format has arbitrary units. Some files are stored in meters
    others are in millimeters or inches. To correct for this, a scaling
    factor can be used during construction.
    */
    double scaling_factor = 1.0;

    // internal
    uint current_triangle_number = 0;
    std::string stl_header;
    uint32_t number_of_triangles;
    Frame* stl_object;
public:

    StlReader(const std::string filename);
    StlReader(const std::string filename, const double scaling_factor);
    ~StlReader();
    Frame* get_frame()const;
    std::string get_print()const;
    uint get_number_of_triangles()const;
private:

	void start();
	void assert_file_is_open()const;
	std::vector<float> read_floats(const uint n);
	std::string read_chars(const uint n);
	uint32_t read_single_uint32();
	uint16_t read_single_uint16();
	void read_triangles();
	Frame* read_and_create_next_triangle();
	void assert_attribute_byte_count_is_zero(
		const uint16_t attribute_byte_count
	); 
	void assert_normal_is_actually_normalized(const Vector3D nomral);
	std::string get_current_triangle_name();
public:

	class CanNotReadSTL : public TracerException {
		using TracerException::TracerException;
	};

	class BadAttributeByteCount : public TracerException {
		using TracerException::TracerException;
	};

	class BadNormal : public TracerException {
		using TracerException::TracerException;
	};

};
#endif // __StlReader_H_INCLUDE__ 
