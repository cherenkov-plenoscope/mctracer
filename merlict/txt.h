// Copyright 2014 Sebastian A. Mueller
#ifndef TOOLS_STRINGTOOLS_H_
#define TOOLS_STRINGTOOLS_H_

#include <string>
#include <vector>
#include "merlict/Vec3.h"

namespace merlict {
namespace txt {

bool is_equal(const std::string text_A, const std::string text_B);

std::string cut_leading_token_infront_of_delimiter(
    std::string *text_of_tokens, const char delimiter);

std::string repeat_multiple_times(
    const std::string text_to_repeat,
    const unsigned int times);

bool is_ending(std::string text, std::string ending);

bool string_contains_char(const std::string &text, const char ch);

std::string place_first_infront_of_each_new_line_of_second(
    const std::string front,
    std::string text);

std::vector<std::string> tokenize_text_using_either_one_of_delimiters(
    const std::string &text,
    const std::string delimiters);

std::string strip_whitespaces(std::string text);

std::string fill_up_text_with_whitespaces_until_column(
    const std::string &text, const unsigned int column);

double to_double(std::string text_to_parse);

bool to_bool(std::string text_to_parse);

int to_int(std::string text_to_parse);

Vec3 to_Vec3(const std::string original_text);

}  // namespace txt
}  // namespace relleums

#endif  // TOOLS_STRINGTOOLS_H_
