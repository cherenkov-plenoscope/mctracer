#include "gtest/gtest.h"
#include "FitsIO/SimpleFitsWriter.h"

class FitsIoTest : public ::testing::Test {};
//----------------------------------------------------------------------
TEST_F(FitsIoTest, create_key_word_record) {

    EXPECT_THROW(
        Fits::Header::KeyWordRecord::key_value_and_comment(
            "012345678", 
            "01234567890123456789", 
            "01234567890123456789012345678901234567890123456"
        ),
        Fits::Header::KeyWordRecord::KeyWordTooLarge
    );

    EXPECT_THROW(
        Fits::Header::KeyWordRecord::key_value_and_comment(
            "01234567", 
            "012345678901234567890", 
            "01234567890123456789012345678901234567890123456"
        ),
        Fits::Header::KeyWordRecord::ValueTooLarge
    );

    EXPECT_THROW(
        Fits::Header::KeyWordRecord::key_value_and_comment(
            "01234567", 
            "01234567890123456789", 
            "012345678901234567890123456789012345678901234567"
        ),
        Fits::Header::KeyWordRecord::CommentTooLarge
    );

    EXPECT_EQ(Fits::Header::line_width, 80);

    EXPECT_EQ(
        Fits::Header::KeyWordRecord::key_value_and_comment(
            "01234567", 
            "01234567890123456789", 
            "01234567890123456789012345678901234567890123456"
        ).length(),
        Fits::Header::line_width
    );

    EXPECT_EQ(
        Fits::Header::KeyWordRecord::key_value_and_comment(
            "0123456", 
            "0123456789012345678", 
            "0123456789012345678901234567890123456789012345"
        ).length(),
        Fits::Header::line_width
    );

    EXPECT_EQ(
        Fits::Header::KeyWordRecord::key_value_and_comment(
            "0123", 
            "0123456789", 
            "012345678901234567890123"
        ).length(),
        Fits::Header::line_width
    );

    EXPECT_EQ(
        Fits::Header::KeyWordRecord::key_value_and_comment(
            "", "", ""
        ).length(),
        Fits::Header::line_width
    );
}
//------------------------------------------------------------------------------
TEST_F(FitsIoTest, create_comment_record) {
    
    EXPECT_EQ(
        Fits::Header::CommentRecord::get_comment_record("my comment").length(),
        Fits::Header::line_width
    );

    EXPECT_EQ(
        Fits::Header::CommentRecord::get_comment_record("").length(),
        Fits::Header::line_width
    );

    std::string str200 = StringTools::repeat_multiple_times("1234567890", 20);

    EXPECT_EQ(
        Fits::Header::CommentRecord::get_comment_record(str200).length(),
        Fits::Header::line_width*3
    );   

    for(uint c=0; c<1337; c++) {
        std::string payload = StringTools::repeat_multiple_times("+", c); 

        EXPECT_EQ(
            Fits::Header::CommentRecord::get_comment_record(payload).length(),
            (c/Fits::Header::CommentRecord::payload_line_width+1)*Fits::Header::line_width
        );      
    }  
}
//------------------------------------------------------------------------------
TEST_F(FitsIoTest, create_header) {
    
    std::stringstream header;
    header << Fits::Header::KeyWordRecord::key_value_and_comment(
        "SIMPLE", "T", "file does conform to FITS standard"
    );
    header << Fits::Header::KeyWordRecord::key_value_and_comment(
        "BITPIX", "8", "number of bits per data pixel"
    );


    std::string packed_header = Fits::Header::get_header(
        header.str()
    );

    std::cout << packed_header;
}