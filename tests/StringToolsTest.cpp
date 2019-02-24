// Copyright 2014 Sebastian A. Mueller
#include "gtest/gtest.h"
#include "Core/mctracer.h"
using namespace relleums;
using std::string;
using std::vector;

class StringToolsTest : public ::testing::Test {};

TEST_F(StringToolsTest, tokenize_text_several_delimiters) {
    string text = "hans klaus\tpeter";
    vector<string> tokens =
        txt::tokenize_text_using_either_one_of_delimiters(text, " \t");
    ASSERT_EQ(3u, tokens.size());
    EXPECT_EQ("hans", tokens.at(0));
    EXPECT_EQ("klaus", tokens.at(1));
    EXPECT_EQ("peter", tokens.at(2));
    tokens =
        txt::tokenize_text_using_either_one_of_delimiters(text, " ");
    ASSERT_EQ(2u, tokens.size());
    EXPECT_EQ("hans", tokens.at(0));
    EXPECT_EQ("klaus\tpeter", tokens.at(1));
    tokens =
        txt::tokenize_text_using_either_one_of_delimiters(text, "\t");
    ASSERT_EQ(2u, tokens.size());
    EXPECT_EQ("hans klaus", tokens.at(0));
    EXPECT_EQ("peter", tokens.at(1));
}

TEST_F(StringToolsTest, tokenize_epmty_text) {
    string text = "";
    vector<string> tokens =
        txt::tokenize_text_using_either_one_of_delimiters(text, " \t");
    EXPECT_EQ(0u, tokens.size());
}

TEST_F(StringToolsTest, tokenize_text_delimiter_in_the_end) {
    string text = "hans peter ";
    vector<string> tokens =
        txt::tokenize_text_using_either_one_of_delimiters(text, " \t");
    ASSERT_EQ(2u, tokens.size());
    EXPECT_EQ("hans", tokens.at(0));
    EXPECT_EQ("peter", tokens.at(1));
}

TEST_F(StringToolsTest, tokenize_text_several_same_delimiters) {
    string text = "hans   peter";
    vector<string> tokens =
        txt::tokenize_text_using_either_one_of_delimiters(text, " \t");
    ASSERT_EQ(2u, tokens.size());
    EXPECT_EQ("hans", tokens.at(0));
    EXPECT_EQ("peter", tokens.at(1));
}

TEST_F(StringToolsTest, tokenize_text_several_different_delimiters) {
    string text = "hans \t peter";
    vector<string> tokens =
        txt::tokenize_text_using_either_one_of_delimiters(text, " \t");
    ASSERT_EQ(2u, tokens.size());
    EXPECT_EQ("hans", tokens.at(0));
    EXPECT_EQ("peter", tokens.at(1));
}

TEST_F(StringToolsTest, is_ending) {
    EXPECT_TRUE(txt::is_ending("hans.xml", ".xml"));
    EXPECT_FALSE(txt::is_ending("hans.xml", ".XML"));
    EXPECT_FALSE(txt::is_ending("blabla_text_bla", ".xml"));
    EXPECT_FALSE(txt::is_ending("text.xmltext", ".xml"));
    EXPECT_TRUE(txt::is_ending(".xml", ".xml"));
    EXPECT_TRUE(txt::is_ending(".xml.xml", ".xml"));
    EXPECT_TRUE(txt::is_ending("\t\n\r.xml", ".xml"));
    EXPECT_TRUE(txt::is_ending("text .xml", ".xml"));
    EXPECT_FALSE(txt::is_ending("text.x ml", ".xml"));
    EXPECT_TRUE(txt::is_ending("abc", "c"));
    EXPECT_TRUE(txt::is_ending("abc\n", "\n"));
}

TEST_F(StringToolsTest, is_equal) {
    EXPECT_TRUE(txt::is_equal("", ""));
    EXPECT_FALSE(txt::is_equal("a", "b"));
    EXPECT_TRUE(txt::is_equal("a", "a"));
    EXPECT_FALSE(txt::is_equal("a", "A"));
    EXPECT_TRUE(txt::is_equal("Auto", "Auto"));
    EXPECT_FALSE(txt::is_equal("Auto", "auto"));
    EXPECT_TRUE(txt::is_equal("Auto ", "Auto "));
    EXPECT_FALSE(txt::is_equal("Auto ", "Auto"));
    EXPECT_TRUE(txt::is_equal("\n", "\n"));
    EXPECT_FALSE(txt::is_equal(" Auto", "Auto"));
}

TEST_F(StringToolsTest, cut_leading_token) {
    string names = "Hans,Peter,Klaus";
    string first_name =
    txt::cut_leading_token_infront_of_delimiter(&names, ',');

    EXPECT_EQ("Hans", first_name);
    EXPECT_EQ("Peter,Klaus", names);

    names = ",Hans,Peter,Klaus";
    first_name = txt::cut_leading_token_infront_of_delimiter(
        &names,
        ',');
    EXPECT_EQ("", first_name);
    EXPECT_EQ("Hans,Peter,Klaus", names);

    names = ",Hans/Peter/Klaus/";
    first_name = txt::cut_leading_token_infront_of_delimiter(
        &names,
        '/');
    EXPECT_EQ(",Hans", first_name);
    EXPECT_EQ("Peter/Klaus/", names);

    names = "Hans ,Peter,Klaus";
    first_name = txt::cut_leading_token_infront_of_delimiter(
        &names,
        ',');
    EXPECT_EQ("Hans ", first_name);
    EXPECT_EQ("Peter,Klaus", names);

    names =  " Hans,Peter,Klaus";
    first_name = txt::cut_leading_token_infront_of_delimiter(
        &names,
        ',');
    EXPECT_EQ(" Hans", first_name);
    EXPECT_EQ("Peter,Klaus", names);
}

TEST_F(StringToolsTest, contains_char) {
    string text = "Hans Peter is an awesome engineer who build AMS!";
    EXPECT_TRUE(txt::string_contains_char(text, 'H'));
    EXPECT_TRUE(txt::string_contains_char(text, 'A'));
    EXPECT_TRUE(txt::string_contains_char(text, 'w'));
    EXPECT_TRUE(txt::string_contains_char(text, '!'));
    EXPECT_FALSE(txt::string_contains_char(text, '?'));
    EXPECT_FALSE(txt::string_contains_char(text, '*'));
}

TEST_F(StringToolsTest, place_infront_of_each_line) {
    std::stringstream out;
    out << "Dear Customer,\n";
    out << "\n";
    out << "Please Shut the fuck up!\n";
    out << "\n";
    out << "Kind regards, your customer service\n";
    std::string result =
        txt::place_first_infront_of_each_new_line_of_second(
            "->",
            out.str());
    std::stringstream expected_out;
    expected_out << "->Dear Customer,\n";
    expected_out << "->\n";
    expected_out << "->Please Shut the fuck up!\n";
    expected_out << "->\n";
    expected_out << "->Kind regards, your customer service\n";
    EXPECT_TRUE(txt::is_equal(expected_out.str(), result));
}

TEST_F(StringToolsTest, fill_with_whitespaces_until_column) {
    std::string hi = "hallo world";
    std::string ho =
        txt::fill_up_text_with_whitespaces_until_column(hi, 80);
    EXPECT_EQ(80u, ho.length());
    std::string empty = "";
    std::string empty_but_whitespaces =
        txt::fill_up_text_with_whitespaces_until_column(empty, 1337);
    EXPECT_EQ(1337u, empty_but_whitespaces.length());
    std::string str42 = "0123456789012345678901234567890123456789012";
    std::string strfull =
        txt::fill_up_text_with_whitespaces_until_column(str42, 19);
    EXPECT_EQ((19u*3u), strfull.length());
}

TEST_F(StringToolsTest, strip_leading_and_tailing_whitespaces) {
    std::string no_whitespaces = "no_whitespaces";
    EXPECT_TRUE(
    txt::is_equal(
        no_whitespaces,
        txt::strip_whitespaces(no_whitespaces)));
    std::string whitespaces_in_between = "whitespaces in between";
    EXPECT_TRUE(
        txt::is_equal(
            whitespaces_in_between,
            txt::strip_whitespaces(whitespaces_in_between)));
    std::string leading_whitespaces = "    leading_whitespaces";
    EXPECT_TRUE(
        txt::is_equal(
            "leading_whitespaces",
            txt::strip_whitespaces(leading_whitespaces)));
    std::string trailing_ws = "trailing_ws  ";
    EXPECT_TRUE(
        txt::is_equal(
            "trailing_ws",
            txt::strip_whitespaces(trailing_ws)));
}

TEST_F(StringToolsTest, to_double) {
    EXPECT_NO_THROW(
    EXPECT_EQ(42.1337, txt::to_double("42.1337"));
    EXPECT_EQ(1337, txt::to_double("1337"));
    EXPECT_EQ(1u, txt::to_double("1"));
    EXPECT_EQ(.1, txt::to_double(".1"));
    EXPECT_EQ(.1442, txt::to_double(".1442"));
    EXPECT_EQ(1.337, txt::to_double(" 1.337"));
    EXPECT_EQ(.1, txt::to_double(" .1"));
    EXPECT_EQ(1.337e42, txt::to_double("1.337e42"));
    EXPECT_EQ(1.337E42, txt::to_double("1.337E42")));
    EXPECT_THROW(
        EXPECT_NE(42.1337, txt::to_double("42p1337")),
        std::invalid_argument);
    EXPECT_THROW(
        EXPECT_NE(1337, txt::to_double("1337 ")),
        std::invalid_argument);
    EXPECT_THROW(
        EXPECT_NE(1u, txt::to_double("")),
        std::invalid_argument);
    EXPECT_THROW(
        EXPECT_NE(.1442, txt::to_double(".1 442")),
        std::invalid_argument);
}

TEST_F(StringToolsTest, to_bool) {
    EXPECT_NO_THROW(
        EXPECT_TRUE(txt::to_bool("true"));
        EXPECT_TRUE(txt::to_bool("TRUE"));
        EXPECT_FALSE(txt::to_bool("false"));
        EXPECT_FALSE(txt::to_bool("FALSE")));
    EXPECT_THROW(
        EXPECT_NE(true, txt::to_bool(" true")),
        std::invalid_argument);
    EXPECT_THROW(
        EXPECT_NE(true, txt::to_bool("true ")),
        std::invalid_argument);
    EXPECT_THROW(
        EXPECT_NE(true, txt::to_bool("wahr")),
        std::invalid_argument);
    EXPECT_THROW(
        EXPECT_NE(true, txt::to_bool("Troe")),
        std::invalid_argument);
    EXPECT_THROW(
        EXPECT_NE(true, txt::to_bool("yes")),
        std::invalid_argument);
    EXPECT_THROW(
        EXPECT_NE(true, txt::to_bool("no")),
        std::invalid_argument);
    EXPECT_THROW(
        EXPECT_NE(true, txt::to_bool("0")),
        std::invalid_argument);
    EXPECT_THROW(
        EXPECT_NE(true, txt::to_bool("")),
        std::invalid_argument);
}

TEST_F(StringToolsTest, to_int) {
    EXPECT_NO_THROW(
        EXPECT_EQ(1, txt::to_int("1"));
        EXPECT_EQ(12, txt::to_int("12"));
        EXPECT_EQ(1337, txt::to_int("1337"));
        EXPECT_EQ(42, txt::to_int(" 42"));
        EXPECT_EQ(123456789, txt::to_int("123456789")));
    EXPECT_THROW(
        EXPECT_NE(1, txt::to_int("1 ")),
        std::invalid_argument);
    EXPECT_THROW(
        EXPECT_NE(1, txt::to_int("")),
        std::invalid_argument);
    EXPECT_THROW(
        EXPECT_NE(123456, txt::to_int("123 456")),
        std::invalid_argument);
    EXPECT_THROW(
        EXPECT_NE(12.344, txt::to_int("12.344")),
        std::invalid_argument);
    EXPECT_THROW(
        EXPECT_NE(1, txt::to_int("no")),
        std::invalid_argument);
    EXPECT_THROW(
        EXPECT_NE(6.6, txt::to_int("6p6")),
        std::invalid_argument);
}

TEST_F(StringToolsTest, to_Vec3) {
    EXPECT_EQ(Vec3(1, 2, 3), txt::to_Vec3("[1,2,3]"));
    EXPECT_EQ(Vec3(1.2, 3.4, 5.6), txt::to_Vec3("[1.2,3.4,5.6]"));
    EXPECT_EQ(Vec3(1.2, 3.4, 5.6), txt::to_Vec3("[ 1.2,3.4,5.6]"));
    EXPECT_EQ(Vec3(1.2, 3.4, 5.6), txt::to_Vec3("[1.2 ,3.4,5.6]"));
    EXPECT_EQ(Vec3(1.2, 3.4, 5.6), txt::to_Vec3("[1.2, 3.4,5.6]"));
    EXPECT_EQ(Vec3(1.2, 3.4, 5.6), txt::to_Vec3("[1.2,3.4 ,5.6]"));
    EXPECT_EQ(Vec3(1.2, 3.4, 5.6), txt::to_Vec3("[1.2,3.4, 5.6]"));
    EXPECT_EQ(Vec3(1.2, 3.4, 5.6), txt::to_Vec3("[1.2,3.4,5.6 ]"));
    EXPECT_EQ(Vec3(1.2, 3.4, 5.6), txt::to_Vec3("[ 1.2 ,3.4,5.6]"));
    EXPECT_EQ(Vec3(1.2, 3.4, 5.6), txt::to_Vec3("[1.2 , 3.4,5.6]"));
    EXPECT_EQ(Vec3(1.2, 3.4, 5.6), txt::to_Vec3("[1.2, 3.4 ,5.6]"));
    EXPECT_EQ(Vec3(1.2, 3.4, 5.6), txt::to_Vec3("[1.2,3.4 , 5.6]"));
    EXPECT_EQ(Vec3(1.2, 3.4, 5.6), txt::to_Vec3("[1.2,3.4 , 5.6]"));
    EXPECT_EQ(Vec3(1.2, 3.4, 5.6), txt::to_Vec3("[1.2,3.4, 5.6 ]"));
    EXPECT_EQ(Vec3(1.2, 3.4, 5.6), txt::to_Vec3("[1.2,3.4 , 5.6]"));
    EXPECT_EQ(
        Vec3(1.2, 3.4, 5.6), txt::to_Vec3("[ 1.2 , 3.4 , 5.6 ]"));
    EXPECT_THROW(
        txt::to_Vec3("1,2,3]"),
        std::invalid_argument);
    EXPECT_THROW(
        txt::to_Vec3("[1,2,3"),
        std::invalid_argument);
    EXPECT_THROW(
        txt::to_Vec3("1,2,3"),
        std::invalid_argument);
    EXPECT_THROW(
        txt::to_Vec3("[1,2]"),
        std::invalid_argument);
    EXPECT_THROW(
        txt::to_Vec3("[1 2,3]"),
        std::invalid_argument);
    EXPECT_THROW(
        txt::to_Vec3("[1 2 3]"),
        std::invalid_argument);
    EXPECT_THROW(
        txt::to_Vec3("[1,2 3]"),
        std::invalid_argument);
    EXPECT_THROW(
        txt::to_Vec3("[1,,3]"),
        std::invalid_argument);
    EXPECT_THROW(
        txt::to_Vec3("[,,]"),
        std::invalid_argument);
    EXPECT_THROW(
        txt::to_Vec3("[1,,]"),
        std::invalid_argument);
    EXPECT_THROW(
        txt::to_Vec3("[,2,]"),
        std::invalid_argument);
}
