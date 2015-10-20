#include <iostream> 
#include <string>
#include <math.h>

#include "gtest/gtest.h"
#include "MmcsCorsikaFileIO/EventIo.h"



// The fixture for testing class Foo.
class EventIoTest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if its body
  // is empty.
	
  EventIoTest() {
    // You can do set-up work for each test here.
  }

  virtual ~EventIoTest() {
    // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  virtual void SetUp() {
    // Code here will be called immediately after the constructor (right
    // before each test).
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test (right
    // before the destructor).
  }

  // Objects declared here can be used by all tests in the test case for Foo.
};
//----------------------------------------------------------------------
TEST_F(EventIoTest, constructor_works) {
  std::vector<unsigned char> data = {55, 138, 31, 212, 176, 4, 0, 0, 7, 0, 0, 0, 72, 4, 0, 0};
  std::stringstream fake_file;
  for (auto ch=data.begin(); ch != data.end(); ch++){
    fake_file << (*ch);
  }
  EventIo::Header my_header(fake_file);
  
  EXPECT_TRUE(my_header.is_sync);
  EXPECT_EQ(1200, my_header.type);
  EXPECT_EQ(0, my_header.version);
  EXPECT_FALSE(my_header.user);
  EXPECT_FALSE(my_header.extended);
  EXPECT_FALSE(my_header.only_sub_objects);
  EXPECT_EQ(1096, my_header.length);
  EXPECT_EQ(7, my_header.id);
  EXPECT_EQ(3, 3.);

}

TEST_F(EventIoTest, constructor_fails) {
  std::vector<unsigned char> data = {55, 138, 32, 212, 176, 4, 0, 0, 7, 0, 0, 0, 72, 4, 0, 0};
  std::stringstream fake_file;
  for (auto ch=data.begin(); ch != data.end(); ch++){
    fake_file << (*ch);
  }
  
  EXPECT_THROW(EventIo::Header my_header(fake_file), TracerException);
  
}

TEST_F(EventIoTest, constructor_with_empty_file) {
  std::vector<unsigned char> data = {};
  std::stringstream fake_file;
  for (auto ch=data.begin(); ch != data.end(); ch++){
    fake_file << (*ch);
  }
  
  EXPECT_THROW(EventIo::Header my_header(fake_file), TracerException);
  
}



TEST_F(EventIoTest, make_runheader) {
std::vector<unsigned char> run_header = { 17, 1, 0, 0, 82, 85, 78, 72, 0, 0, 224, 
    64, 64, 118, 19, 72, 229, 208, 236, 64, 0, 
    0, 128, 63, 0, 216, 86, 72, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 205, 204, 44, 192, 0, 0, 122, 
    68, 0, 80, 67, 71, 0, 0, 128, 63, 0, 
    0, 0, 0, 154, 153, 153, 62, 154, 153, 153, 
    62, 10, 215, 163, 60, 10, 215, 163, 60, 108, 
    231, 23, 78, 0, 124, 18, 73, 0, 36, 244, 
    73, 0, 0, 0, 0, 0, 0, 0, 0, 9, 
    159, 59, 61, 0, 182, 18, 63, 182, 100, 88, 
    61, 0, 0, 32, 64, 225, 122, 4, 64, 51, 
    51, 3, 65, 205, 204, 204, 61, 0, 0, 0, 
    0, 0, 0, 0, 0, 196, 0, 128, 63, 23, 
    122, 30, 60, 1, 0, 128, 63, 59, 196, 22, 
    58, 0, 0, 0, 0, 0, 0, 0, 0, 205, 
    204, 22, 66, 191, 187, 32, 57, 196, 46, 22, 
    65, 111, 18, 3, 59, 224, 92, 223, 80, 0, 
    0, 128, 63, 64, 81, 10, 63, 195, 245, 200, 
    63, 149, 192, 80, 58, 49, 8, 172, 60, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 160, 65, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 4, 25, 134, 62, 137, 
    91, 102, 63, 0, 0, 0, 0, 177, 253, 132, 
    63, 196, 242, 138, 62, 55, 9, 9, 67, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 228, 43, 
    70, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 128, 63, 0, 0, 0, 0, 0, 
    0, 0, 0, 205, 204, 204, 61, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 128, 62, 0, 0, 0, 
    63, 0, 0, 64, 63, 0, 0, 128, 63, 0, 
    0, 0, 63, 205, 204, 76, 62, 252, 40, 109, 
    63, 146, 225, 143, 63, 154, 153, 21, 67, 154, 
    153, 21, 67, 65, 47, 113, 62, 170, 241, 82, 
    62, 113, 61, 10, 62, 248, 83, 99, 62, 0, 
    0, 0, 63, 0, 0, 0, 0, 124, 97, 34, 
    63, 218, 88, 49, 63, 206, 170, 95, 63, 12, 
    147, 41, 63, 139, 108, 199, 62, 125, 93, 28, 
    64, 23, 147, 105, 63, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 128, 63, 0, 
    80, 195, 71, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 
    9, 202, 62, 236, 81, 56, 63, 252, 24, 115, 
    63, 195, 245, 136, 191, 225, 122, 4, 64, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 111, 240, 37, 63, 61, 
    44, 4, 63, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 104, 145, 45, 63, 231, 
    251, 105, 63, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 4, 38, 71, 0, 
    0, 0, 0, 128, 53, 61, 73, 128, 106, 201, 
    73, 128, 150, 152, 74, 160, 55, 32, 75, 99, 
    142, 58, 195, 135, 214, 189, 194, 92, 230, 28, 
    63, 0, 0, 0, 0, 255, 219, 56, 60, 0, 
    213, 152, 68, 5, 29, 143, 68, 9, 51, 163, 
    68, 97, 11, 7, 68, 0, 0, 128, 63, 166, 
    184, 114, 73, 153, 100, 86, 73, 241, 78, 27, 
    73, 163, 132, 60, 73, 40, 107, 110, 78, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 72, 67
  };
  std::stringstream fake_file;
  for (auto ch=run_header.begin(); ch != run_header.end(); ch++){
    fake_file << (*ch);
  }
  
  MmcsCorsikaRunHeader my_run_header = EventIo::make_run_header_from_stream(fake_file);
  std::cout << my_run_header.get_print() << std::endl; 
  
}
