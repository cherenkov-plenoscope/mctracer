#include "gtest/gtest.h"
#include "Tools/StringTools.h"
using std::string;
using std::vector;

class StringToolsTest : public ::testing::Test {};
//------------------------------------------------------------------------------
TEST_F(StringToolsTest, tokenize_text_several_delimiters) {

    string text = "hans klaus\tpeter";

    vector<string> tokens = 
        StringTools::tokenize_text_using_either_one_of_delimiters(text," \t");

    ASSERT_EQ(3, tokens.size());
    EXPECT_EQ("hans", tokens.at(0));
    EXPECT_EQ("klaus", tokens.at(1));
    EXPECT_EQ("peter", tokens.at(2));

    tokens = 
        StringTools::tokenize_text_using_either_one_of_delimiters(text," ");

    ASSERT_EQ(2, tokens.size());
    EXPECT_EQ("hans", tokens.at(0));
    EXPECT_EQ("klaus\tpeter", tokens.at(1));

    tokens = 
        StringTools::tokenize_text_using_either_one_of_delimiters(text,"\t");

    ASSERT_EQ(2, tokens.size());
    EXPECT_EQ("hans klaus", tokens.at(0));
    EXPECT_EQ("peter", tokens.at(1));
}
//------------------------------------------------------------------------------
TEST_F(StringToolsTest, tokenize_epmty_text) {

    string text = "";

    vector<string> tokens = 
        StringTools::tokenize_text_using_either_one_of_delimiters(text," \t");

    EXPECT_EQ(0, tokens.size());
}
//------------------------------------------------------------------------------
TEST_F(StringToolsTest, tokenize_text_delimiter_in_the_end) {

    string text = "hans peter ";

    vector<string> tokens = 
        StringTools::tokenize_text_using_either_one_of_delimiters(text," \t");

    ASSERT_EQ(2, tokens.size());
    EXPECT_EQ("hans", tokens.at(0));
    EXPECT_EQ("peter", tokens.at(1));
}
//------------------------------------------------------------------------------
TEST_F(StringToolsTest, tokenize_text_several_same_delimiters) {

    string text = "hans   peter";

    vector<string> tokens = 
        StringTools::tokenize_text_using_either_one_of_delimiters(text," \t");

    ASSERT_EQ(2, tokens.size());
    EXPECT_EQ("hans", tokens.at(0));
    EXPECT_EQ("peter", tokens.at(1));
}
//------------------------------------------------------------------------------
TEST_F(StringToolsTest, tokenize_text_several_different_delimiters) {

    string text = "hans \t peter";

    vector<string> tokens = 
        StringTools::tokenize_text_using_either_one_of_delimiters(text," \t");

    ASSERT_EQ(2, tokens.size());
    EXPECT_EQ("hans", tokens.at(0));
    EXPECT_EQ("peter", tokens.at(1));
}
//------------------------------------------------------------------------------
TEST_F(StringToolsTest, is_ending) {
    
    EXPECT_TRUE ( StringTools::is_ending("hans.xml",".xml") );
    EXPECT_FALSE( StringTools::is_ending("hans.xml",".XML") );
    EXPECT_FALSE( StringTools::is_ending("blabla_text_bla",".xml") );
    EXPECT_FALSE( StringTools::is_ending("text.xmltext",".xml") );
    EXPECT_TRUE ( StringTools::is_ending(".xml",".xml") );
    EXPECT_TRUE ( StringTools::is_ending(".xml.xml",".xml") );
    EXPECT_TRUE ( StringTools::is_ending("\t\n\r.xml",".xml") );
    EXPECT_TRUE ( StringTools::is_ending("text .xml",".xml") );
    EXPECT_FALSE( StringTools::is_ending("text.x ml",".xml") );
    EXPECT_TRUE ( StringTools::is_ending("abc","c") );
    EXPECT_TRUE ( StringTools::is_ending("abc\n","\n") );
}
//----------------------------------------------------------------------
TEST_F(StringToolsTest, is_equal) {
    
    EXPECT_TRUE ( StringTools::is_equal("","")   );
    EXPECT_FALSE( StringTools::is_equal("a","b") );
    EXPECT_TRUE ( StringTools::is_equal("a","a") );
    EXPECT_FALSE( StringTools::is_equal("a","A") );
    EXPECT_TRUE ( StringTools::is_equal("Auto","Auto") );
    EXPECT_FALSE( StringTools::is_equal("Auto","auto") );
    EXPECT_TRUE ( StringTools::is_equal("Auto ","Auto ") );
    EXPECT_FALSE( StringTools::is_equal("Auto ","Auto") );
    EXPECT_TRUE ( StringTools::is_equal("\n","\n") );
    EXPECT_FALSE( StringTools::is_equal(" Auto","Auto") );
}
//------------------------------------------------------------------------------
TEST_F(StringToolsTest, cut_leading_token) {
    
    string names = "Hans,Peter,Klaus";

    string first_name = 
    StringTools::cut_leading_token_infront_of_delimiter(names,',');
    
    EXPECT_EQ("Hans", first_name);
    EXPECT_EQ("Peter,Klaus", names);

    names = ",Hans,Peter,Klaus";
    first_name = StringTools::cut_leading_token_infront_of_delimiter(names,','); 
    EXPECT_EQ("", first_name);
    EXPECT_EQ("Hans,Peter,Klaus", names);

    names = ",Hans/Peter/Klaus/";
    first_name = StringTools::cut_leading_token_infront_of_delimiter(names,'/'); 
    EXPECT_EQ(",Hans", first_name);
    EXPECT_EQ("Peter/Klaus/", names);

    names = "Hans ,Peter,Klaus";
    first_name = StringTools::cut_leading_token_infront_of_delimiter(names,','); 
    EXPECT_EQ("Hans ", first_name);
    EXPECT_EQ("Peter,Klaus", names);

    names =  " Hans,Peter,Klaus";
    first_name = StringTools::cut_leading_token_infront_of_delimiter(names,','); 
    EXPECT_EQ(" Hans", first_name);
    EXPECT_EQ("Peter,Klaus", names);
}
//------------------------------------------------------------------------------
TEST_F(StringToolsTest, contains_char) {
    
    string text = "Hans Peter is an awesome engineer who build AMS!";
    EXPECT_TRUE(StringTools::string_contains_char(text, 'H'));
    EXPECT_TRUE(StringTools::string_contains_char(text, 'A'));
    EXPECT_TRUE(StringTools::string_contains_char(text, 'w'));
    EXPECT_TRUE(StringTools::string_contains_char(text, '!'));
    EXPECT_FALSE(StringTools::string_contains_char(text, '?'));
    EXPECT_FALSE(StringTools::string_contains_char(text, '*'));
}
//------------------------------------------------------------------------------
TEST_F(StringToolsTest, place_infront_of_each_line) {
    
    std::stringstream out;
    out << "Dear Customer,\n";
    out << "\n";
    out << "Please Shut the fuck up!\n";
    out << "\n";
    out << "Kind regards, your customer service\n";

    std::string result =
    StringTools::place_first_infront_of_each_new_line_of_second("->",out.str());

    std::stringstream expected_out;
    expected_out << "->Dear Customer,\n";
    expected_out << "->\n";
    expected_out << "->Please Shut the fuck up!\n";
    expected_out << "->\n";
    expected_out << "->Kind regards, your customer service\n";

    EXPECT_TRUE(StringTools::is_equal(expected_out.str(), result));
}
//------------------------------------------------------------------------------
TEST_F(StringToolsTest, fill_with_whitespaces_until_column) {

    std::string hi = "hallo world";
    std::string ho = 
    StringTools::fill_up_text_with_whitespaces_until_column(hi, 80);

    EXPECT_EQ(80, ho.length());

    std::string empty = "";
    std::string empty_but_whitespaces = 
    StringTools::fill_up_text_with_whitespaces_until_column(empty, 1337);

    EXPECT_EQ(1337, empty_but_whitespaces.length());

    std::string str42 = "0123456789012345678901234567890123456789012";
    std::string strfull = 
    StringTools::fill_up_text_with_whitespaces_until_column(str42, 19);

    EXPECT_EQ((19*3), strfull.length());
}
//------------------------------------------------------------------------------
TEST_F(StringToolsTest, strip_leading_and_tailing_whitespaces) {
    std::string no_whitespaces = "no_whitespaces";

    EXPECT_TRUE(
    StringTools::is_equal(
        no_whitespaces,
        StringTools::strip_whitespaces(no_whitespaces)
    )
    );

    std::string whitespaces_in_between = "whitespaces in between";

    EXPECT_TRUE(
    StringTools::is_equal(
        whitespaces_in_between,
        StringTools::strip_whitespaces(whitespaces_in_between)
    )
    );

    std::string leading_whitespaces = "    leading_whitespaces";

    EXPECT_TRUE(
    StringTools::is_equal(
        "leading_whitespaces",
        StringTools::strip_whitespaces(leading_whitespaces)
    )
    );

    std::string trailing_ws = "trailing_ws  ";

    EXPECT_TRUE(
    StringTools::is_equal(
        "trailing_ws",
        StringTools::strip_whitespaces(trailing_ws)
    )
    );
}
//------------------------------------------------------------------------------
TEST_F(StringToolsTest, to_double) {

    EXPECT_NO_THROW(
    EXPECT_EQ(42.1337,StringTools::to_double("42.1337"));
    EXPECT_EQ(1337,StringTools::to_double("1337"));
    EXPECT_EQ(1,StringTools::to_double("1"));
    EXPECT_EQ(.1,StringTools::to_double(".1"));
    EXPECT_EQ(.1442,StringTools::to_double(".1442"));
    EXPECT_EQ(1.337,StringTools::to_double(" 1.337"));
    EXPECT_EQ(.1,StringTools::to_double(" .1"));
    EXPECT_EQ(1.337e42,StringTools::to_double("1.337e42"));
    EXPECT_EQ(1.337E42,StringTools::to_double("1.337E42"));
    );

    EXPECT_THROW(
    EXPECT_NE(42.1337,StringTools::to_double("42p1337"));,
    StringTools::CanNotParseDouble
    );

    EXPECT_THROW(
    EXPECT_NE(1337,StringTools::to_double("1337 "));,
    StringTools::CanNotParseDouble
    );

    EXPECT_THROW(
    EXPECT_NE(1,StringTools::to_double(""));,
    StringTools::CanNotParseDouble
    );

    EXPECT_THROW(
    EXPECT_NE(.1442,StringTools::to_double(".1 442"));,
    StringTools::CanNotParseDouble
    );
}
//------------------------------------------------------------------------------
TEST_F(StringToolsTest, to_bool) {

    EXPECT_NO_THROW(
    EXPECT_TRUE(StringTools::to_bool("true"));
    EXPECT_TRUE(StringTools::to_bool("TRUE"));
    EXPECT_FALSE(StringTools::to_bool("false"));
    EXPECT_FALSE(StringTools::to_bool("FALSE"));   
    );

    EXPECT_THROW(
    EXPECT_NE(true, StringTools::to_bool(" true"));,
    StringTools::CanNotParseBool
    );

    EXPECT_THROW(
    EXPECT_NE(true, StringTools::to_bool("true "));,
    StringTools::CanNotParseBool
    );

    EXPECT_THROW(
    EXPECT_NE(true, StringTools::to_bool("wahr"));,
    StringTools::CanNotParseBool
    );

    EXPECT_THROW(
    EXPECT_NE(true, StringTools::to_bool("Troe"));,
    StringTools::CanNotParseBool
    );

    EXPECT_THROW(
    EXPECT_NE(true, StringTools::to_bool("yes"));,
    StringTools::CanNotParseBool
    );

    EXPECT_THROW(
    EXPECT_NE(true, StringTools::to_bool("no"));,
    StringTools::CanNotParseBool
    );

    EXPECT_THROW(
    EXPECT_NE(true, StringTools::to_bool("0"));,
    StringTools::CanNotParseBool
    );

    EXPECT_THROW(
    EXPECT_NE(true, StringTools::to_bool(""));,
    StringTools::CanNotParseBool
    );
}
//------------------------------------------------------------------------------
TEST_F(StringToolsTest, to_int) {

    EXPECT_NO_THROW(
    EXPECT_EQ(1, StringTools::to_int("1"));
    EXPECT_EQ(12, StringTools::to_int("12"));
    EXPECT_EQ(1337, StringTools::to_int("1337"));
    EXPECT_EQ(42, StringTools::to_int(" 42"));
    EXPECT_EQ(123456789, StringTools::to_int("123456789")); 
    );

    EXPECT_THROW(
    EXPECT_NE(1, StringTools::to_int("1 "));,
    StringTools::CanNotParseInt
    );

    EXPECT_THROW(
    EXPECT_NE(1, StringTools::to_int(""));,
    StringTools::CanNotParseInt
    );

    EXPECT_THROW(
    EXPECT_NE(123456, StringTools::to_int("123 456"));,
    StringTools::CanNotParseInt
    );

    EXPECT_THROW(
    EXPECT_NE(12.344, StringTools::to_int("12.344"));,
    StringTools::CanNotParseInt
    );

    EXPECT_THROW(
    EXPECT_NE(1, StringTools::to_int("no"));,
    StringTools::CanNotParseInt
    );

    EXPECT_THROW(
    EXPECT_NE(6.6, StringTools::to_int("6p6"));,
    StringTools::CanNotParseInt
    );
}
//------------------------------------------------------------------------------
TEST_F(StringToolsTest, to_Tuple3) {

    EXPECT_EQ(Tuple3(1,2,3), StringTools::to_Tuple3("[1,2,3]"));
    EXPECT_EQ(Tuple3(1.2,3.4,5.6), StringTools::to_Tuple3("[1.2,3.4,5.6]"));
    EXPECT_EQ(Tuple3(1.2,3.4,5.6), StringTools::to_Tuple3("[ 1.2,3.4,5.6]"));
    EXPECT_EQ(Tuple3(1.2,3.4,5.6), StringTools::to_Tuple3("[1.2 ,3.4,5.6]"));
    EXPECT_EQ(Tuple3(1.2,3.4,5.6), StringTools::to_Tuple3("[1.2, 3.4,5.6]"));
    EXPECT_EQ(Tuple3(1.2,3.4,5.6), StringTools::to_Tuple3("[1.2,3.4 ,5.6]"));
    EXPECT_EQ(Tuple3(1.2,3.4,5.6), StringTools::to_Tuple3("[1.2,3.4, 5.6]"));
    EXPECT_EQ(Tuple3(1.2,3.4,5.6), StringTools::to_Tuple3("[1.2,3.4,5.6 ]"));
    EXPECT_EQ(Tuple3(1.2,3.4,5.6), StringTools::to_Tuple3("[ 1.2 ,3.4,5.6]"));
    EXPECT_EQ(Tuple3(1.2,3.4,5.6), StringTools::to_Tuple3("[1.2 , 3.4,5.6]"));
    EXPECT_EQ(Tuple3(1.2,3.4,5.6), StringTools::to_Tuple3("[1.2, 3.4 ,5.6]"));
    EXPECT_EQ(Tuple3(1.2,3.4,5.6), StringTools::to_Tuple3("[1.2,3.4 , 5.6]"));
    EXPECT_EQ(Tuple3(1.2,3.4,5.6), StringTools::to_Tuple3("[1.2,3.4 , 5.6]"));
    EXPECT_EQ(Tuple3(1.2,3.4,5.6), StringTools::to_Tuple3("[1.2,3.4, 5.6 ]"));
    EXPECT_EQ(Tuple3(1.2,3.4,5.6), StringTools::to_Tuple3("[1.2,3.4 , 5.6]"));
    EXPECT_EQ(Tuple3(1.2,3.4,5.6), StringTools::to_Tuple3("[ 1.2 , 3.4 , 5.6 ]"));

    EXPECT_THROW(
        StringTools::to_Tuple3("1,2,3]");,
        StringTools::CanNotParseTuple3
    );

    EXPECT_THROW(
        StringTools::to_Tuple3("[1,2,3");,
        StringTools::CanNotParseTuple3
    );

    EXPECT_THROW(
        StringTools::to_Tuple3("1,2,3");,
        StringTools::CanNotParseTuple3
    );

    EXPECT_THROW(
        StringTools::to_Tuple3("[1,2]");,
        StringTools::CanNotParseTuple3
    );

    EXPECT_THROW(
        StringTools::to_Tuple3("[1 2,3]");,
        StringTools::CanNotParseTuple3
    );

    EXPECT_THROW(
        StringTools::to_Tuple3("[1 2 3]");,
        StringTools::CanNotParseTuple3
    );

    EXPECT_THROW(
        StringTools::to_Tuple3("[1,2 3]");,
        StringTools::CanNotParseTuple3
    );

    EXPECT_THROW(
        StringTools::to_Tuple3("[1,,3]");,
        StringTools::CanNotParseTuple3
    );

    EXPECT_THROW(
        StringTools::to_Tuple3("[,,]");,
        StringTools::CanNotParseTuple3
    );

    EXPECT_THROW(
        StringTools::to_Tuple3("[1,,]");,
        StringTools::CanNotParseTuple3
    );

    EXPECT_THROW(
        StringTools::to_Tuple3("[,2,]");,
        StringTools::CanNotParseTuple3
    );
}