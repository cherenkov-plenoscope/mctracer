// Copyright 2017 Sebastian A. Mueller
#ifndef CORE_BINARY_INPUT_OUTPUT_H_
#define CORE_BINARY_INPUT_OUTPUT_H_

#include <stdint.h>
#include <fstream>

namespace relleums {
namespace binio {

void append_float32(const float &v, std::ostream &fout);
float read_float32(std::istream &fin);

void append_uint8(const uint8_t &v, std::ostream &fout);
uint8_t read_uint8(std::istream &fin);

void append_int32(const int32_t &v, std::ostream &fout);
int32_t read_int32(std::istream &fin);

void append_uint32(const uint32_t &v, std::ostream &fout);
uint32_t read_uint32(std::istream &fin);

void append_uint16(const uint16_t &v, std::ostream &fout);
uint16_t read_uint16(std::istream &fin);

double read_float64(std::istream &fin);
uint64_t read_uint64(std::istream &fin);

}  // namespace binio
}  // namespace relleums

#endif  // CORE_BINARY_INPUT_OUTPUT_H_
