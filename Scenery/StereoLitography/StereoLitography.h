//=================================
// include guard
#ifndef __StereoLitography_H_INCLUDE__
#define __StereoLitography_H_INCLUDE__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <iostream> 
#include <string>
#include "Core/TracerException.h"
#include "Tools/Tools.h"
#include "Tools/FileTools.h"
#include "Tools/StringTools.h"
#include <fstream>
#include <stdio.h>
#include <string.h>
#include "Core/Frame.h"
#include "Core/SurfaceEntity.h"
using std::string;
using std::vector;

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
namespace StereoLitography {

    void add_stl_to_and_inherit_surface_from_surfac_entity(
        const string path, 
        SurfaceEntity* proto,
        const double scale = 1.0
    );

    void add_stl_to_frame(
        const string path, 
        Frame* proto,
        const double scale = 1.0
    );
    // The STL file format has arbitrary units. Some files are stored in meters
    // others are in millimeters or inches. To correct for this, a scaling
    // factor can be used during construction.

//--------------------------------
// AUXILUARY
//--------------------------------

    struct Facet {
        Vec3 n;
        Vec3 a;
        Vec3 b;
        Vec3 c; 
    };

//--------------------------------
// BINARY IO
//--------------------------------
    class BinaryIo {
    protected:
            
        const uint header_size_in_chars = 80;
        const uint triangle_size_in_32bit_floats = 12;
    public:

        class CanNotReadFile : public TracerException {
            using TracerException::TracerException;
        };

        class BadNormal : public TracerException {
            using TracerException::TracerException;
        };

        class CanNotReadAscii : public TracerException {
            using TracerException::TracerException;
        };
    };
//--------------------------------
// BINARY WRITER
//--------------------------------

    class BinaryWriter :public BinaryIo{

        std::ofstream file;
        string filename;
        vector<Facet> facets;
    public:

        void add_facets(const vector<Facet> additional_facets);

        void add_facet_normal_and_three_vertices(
            const Vec3 n,
            const Vec3 a,
            const Vec3 b,
            const Vec3 c
        );

        void write_to_file(const string filename);
    private:

        void assert_file_is_open()const;
        void assert_normal_is_actually_normalized(const Vec3 normal);
        void write_stl_header();
        void write_number_of_facets(const uint32_t i);
        void write_attribute_count(const uint16_t i);
        void write_vector(const Vec3 &vec);
        void write_facets();
        void write_facet(const Facet &facet);
    };
//--------------------------------
// BINARY READER
//--------------------------------
    
    class BinaryReader :public BinaryIo{

        const string filename;
        std::ifstream stl_file;
        string stl_header;

        vector<Facet> facets;

        uint current_triangle_number = 0;
        uint32_t total_number_of_facets;

        // keep track of facets with bad "attribute byte count"
        std::vector<uint> facets_with_bad_attribute_count;
    public:

        BinaryReader(const string filename);
        vector<Facet> get_facets()const;
        string str()const;
        string get_header()const;
        uint get_number_of_facets()const;
        string get_report()const;
    private:

        void read_header();
        void read_total_number_of_facets();
        void start();
        Facet read_and_create_next_facet();
        bool stl_header_implies_ascii_format()const;
        void assert_is_no_ascii_format()const;
        void assert_normal_is_actually_normalized(const Vec3 nomral);
        void assert_file_is_open()const;
        void read_facets();
        vector<float> read_floats(const uint n);
        string read_chars(const uint n);
        uint32_t read_single_uint32();
        uint16_t read_single_uint16();
        void check_attribute_byte_count_is_zero(
            const uint16_t attribute_byte_count
        ); 
    };
} // StereoLitography
#endif // __StereoLitography_H_INCLUDE__ 
