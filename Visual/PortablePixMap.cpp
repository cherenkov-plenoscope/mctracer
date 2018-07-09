// Copyright 2014 Sebastian A. Mueller
#include "Visual/PortablePixMap.h"
#include <stdint.h>
#include <string>
#include <sstream>
#include "Tools/StringTools.h"

namespace Visual {

void append_uint8(const uint8_t &v, std::ostream &fout) {
    fout.write(reinterpret_cast<const char*>(&v), sizeof(v));
}

uint8_t read_uint8(std::istream &fin) {
    uint8_t v;
    fin.read(reinterpret_cast<char*>(&v), sizeof(v));
    return v;
}

void append_picture_to_file(const Image &img, std::ostream &fout) {
    std::stringstream head;
    head << "P6\n";
    head << "# CREATOR: MCTRACER\n";
    head << int(img.get_number_of_cols()) << " ";
    head << int(img.get_number_of_rows()) << "\n";
    head << "255\n";
    fout << head.str();
    for (unsigned int row = 0; row < img.get_number_of_rows(); row++) {
        for (unsigned int col = 0; col < img.get_number_of_cols(); col++) {
            const Color c = img.get_color_row_col(row, col);
            append_uint8(c.r, fout);
            append_uint8(c.g, fout);
            append_uint8(c.b, fout);
        }
    }
    return;
}

Image read_picture_from_file(std::istream &fin) {
    std::string magic;
    std::getline(fin, magic);
    std::string comment;
    std::getline(fin, comment);
    std::string resolution;
    std::getline(fin, resolution);

    if (!StringTools::is_equal(magic, "P6")) {
        std::stringstream info;
        info << "PortablePixMap::" << __func__ << "()\n";
        info << "Expected magic file-descriptor to be 'P6', ";
        info << "but actually it is '" << magic << "'\n";
        throw std::runtime_error(info.str());
    }

    const int number_columns = StringTools::to_int(
        StringTools::cut_leading_token_infront_of_delimiter(&resolution, ' '));
    if (number_columns < 0) {
        std::stringstream info;
        info << "PortablePixMap::" << __func__ << "()\n";
        info << "Expected number_columns >= 0, but actually it is '";
        info << number_columns << "'\n";
        throw std::runtime_error(info.str());
    }

    const int number_rows = StringTools::to_int(
        StringTools::cut_leading_token_infront_of_delimiter(&resolution, ' '));
    if (number_rows < 0) {
        std::stringstream info;
        info << "PortablePixMap::" << __func__ << "()\n";
        info << "Expected number_rows >= 0, but actually it is '";
        info << number_rows << "'\n";
        throw std::runtime_error(info.str());
    }

    std::string color_depth;
    std::getline(fin, color_depth);
    const int max_depth = StringTools::to_int(color_depth);
    if (max_depth != 255) {
        std::stringstream info;
        info << "PortablePixMap::" << __func__ << "()\n";
        info << "Expected color-depth == 255, but actually it is '";
        info << max_depth << "'\n";
        throw std::runtime_error(info.str());
    }

    Image img(number_columns, number_rows);
    for (unsigned int row = 0; row < img.get_number_of_rows(); row++) {
        for (unsigned int col = 0; col < img.get_number_of_cols(); col++) {
            img.set_row_col_to_color(
                row,
                col,
                Color(
                    read_uint8(fin),
                    read_uint8(fin),
                    read_uint8(fin)));
        }
    }
    return img;
}

}  // namespace Visual
