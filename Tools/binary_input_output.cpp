// Copyright 2017 Sebastian A. Mueller
#include "Tools/binary_input_output.h"

namespace relleums {
namespace bio {

void append_float32(const float &v, std::ostream &fout) {
    fout.write(reinterpret_cast<const char*>(&v), sizeof(v));
}

float read_float32(std::istream &fin) {
    float v;
    fin.read(reinterpret_cast<char*>(&v), sizeof(v));
    return v;
}

void append_int32(const int32_t &v, std::ostream &fout) {
    fout.write(reinterpret_cast<const char*>(&v), sizeof(v));
}

int32_t read_int32(std::istream &fin) {
    int32_t v;
    fin.read(reinterpret_cast<char*>(&v), sizeof(v));
    return v;
}

void append_uint32(const uint32_t &v, std::ostream &fout) {
    fout.write(reinterpret_cast<const char*>(&v), sizeof(v));
}

uint32_t read_uint32(std::istream &fin) {
    uint32_t v;
    fin.read(reinterpret_cast<char*>(&v), sizeof(v));
    return v;
}

void append_uint16(const uint16_t &v, std::ostream &fout) {
    fout.write(reinterpret_cast<const char*>(&v), sizeof(v));
}

uint16_t read_uint16(std::istream &fin) {
    uint16_t v;
    fin.read(reinterpret_cast<char*>(&v), sizeof(v));
    return v;
}

void append_uint8(const uint8_t &v, std::ostream &fout) {
    fout.write(reinterpret_cast<const char*>(&v), sizeof(v));
}

uint8_t read_uint8(std::istream &fin) {
    uint8_t v;
    fin.read(reinterpret_cast<char*>(&v), sizeof(v));
    return v;
}

double read_float64(std::istream &fin) {
    double v;
    fin.read(reinterpret_cast<char*>(&v), sizeof(v));
    return v;
}

uint64_t read_uint64(std::istream &fin) {
    uint64_t v;
    fin.read(reinterpret_cast<char*>(&v), sizeof(v));
    return v;
}

}  // namespace bio
}  // namespace relleums
