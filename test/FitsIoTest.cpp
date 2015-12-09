#include "gtest/gtest.h"
#include "FitsIO/SimpleFitsWriter.h"

class FitsIoTest : public ::testing::Test {};
//----------------------------------------------------------------------
TEST_F(FitsIoTest, key_word_record) {

    EXPECT_THROW(

        Fits::KeyWordRecord::get_keyword_record_for_key_value_and_comment(
            "012345678", "01234567890123456789", "01234567890123456789012345678901234567890123456"
        ),
        
        Fits::KeyWordRecord::KeyWordTooLarge
    );

    EXPECT_THROW(

        Fits::KeyWordRecord::get_keyword_record_for_key_value_and_comment(
            "01234567", "012345678901234567890", "01234567890123456789012345678901234567890123456"
        ),
        
        Fits::KeyWordRecord::ValueTooLarge
    );

    EXPECT_THROW(

        Fits::KeyWordRecord::get_keyword_record_for_key_value_and_comment(
            "01234567", "01234567890123456789", "012345678901234567890123456789012345678901234567"
        ),
        
        Fits::KeyWordRecord::CommentTooLarge
    );

    EXPECT_EQ(Fits::KeyWordRecord::fixed_length, 80);

    EXPECT_EQ(

        Fits::KeyWordRecord::get_keyword_record_for_key_value_and_comment(
            "01234567", "01234567890123456789", "01234567890123456789012345678901234567890123456"
        ).length(),
        
        Fits::KeyWordRecord::fixed_length
    );

    EXPECT_EQ(

        Fits::KeyWordRecord::get_keyword_record_for_key_value_and_comment(
            "0123456", "0123456789012345678", "0123456789012345678901234567890123456789012345"
        ).length(),
        
        Fits::KeyWordRecord::fixed_length
    );

    EXPECT_EQ(

        Fits::KeyWordRecord::get_keyword_record_for_key_value_and_comment(
            "0123", "0123456789", "012345678901234567890123"
        ).length(),
        
        Fits::KeyWordRecord::fixed_length
    );

    EXPECT_EQ(

        Fits::KeyWordRecord::get_keyword_record_for_key_value_and_comment(
            "", "", ""
        ).length(),
        
        Fits::KeyWordRecord::fixed_length
    );
}