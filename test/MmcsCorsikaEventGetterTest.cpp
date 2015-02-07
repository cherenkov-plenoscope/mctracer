#include <iostream> 
#include <string>
#include <math.h>

#include "gtest/gtest.h"
#include "MmcsCorsikaFileIO/MmcsCorsikaFullEventGetter.h"
#include "MmcsCorsikaFileIO/MmcsCorsikaSubBlockGetter.h"
#include "MmcsCorsikaFileIO/MmcsCorsikaPhotonData.h"
#include "MmcsCorsikaFileIO/MmcsCorsikaTools.h"
#include "ListOfPropagations.h"

using namespace std;

// The fixture for testing class Foo.
class MmcsCorsikaEventGetterTest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if its body
  // is empty.
	
  MmcsCorsikaEventGetterTest() {
    // You can do set-up work for each test here.
  }

  virtual ~MmcsCorsikaEventGetterTest() {
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
  void EXPECT_EXCEPTION_OF_TYPE_FOR_FILE(
  	const int problem_type,const std::string &file
  )const {
 	bool exception_detected = false;
 	try{
		MmcsCorsikaFullEventGetter event_getter(file);
		while(event_getter.has_still_events_left())
			MmcsCorsikaEvent event = event_getter.get_next_event();	
	}catch(TracerException &e ) {
		exception_detected = true;
		EXPECT_EQ(problem_type, e.type());
		if(problem_type != e.type())
			throw e;
	}
	EXPECT_TRUE(exception_detected);	 	
  }
};
//----------------------------------------------------------------------
TEST_F(MmcsCorsikaEventGetterTest, word_size_is_too_large) {
  	EXPECT_THROW(
  		{MmcsCorsikaTools::str2float_4byte_bin_map("Hans Peter");},
 		TracerException
	);
}
//------------------------------------------------------------------------------
TEST_F(MmcsCorsikaEventGetterTest, word_size_is_zero) {
  	EXPECT_THROW(
  		{MmcsCorsikaTools::str2float_4byte_bin_map("");},
 		TracerException
	);
}
//------------------------------------------------------------------------------
TEST_F(Tools, word_2_float) {

  string h = "hans";

  float H = MmcsCorsikaTools::str2float_4byte_bin_map(h);
  string r = MmcsCorsikaTools::float2str_4byte_bin_map(H);

  EXPECT_EQ(h, r);
}
//------------------------------------------------------------------------------
TEST_F(Tools, float_2_word) {

  float leet = 1.337;

  string leet_word = MmcsCorsikaTools::float2str_4byte_bin_map(leet);
  float r_leet = MmcsCorsikaTools::str2float_4byte_bin_map(leet_word);

  EXPECT_EQ(leet, r_leet);
}
//------------------------------------------------------------------------------
TEST_F(Tools, zero_float_2_word) {

  float n = 0.0;
  string emp = MmcsCorsikaTools::float2str_4byte_bin_map(n);
  EXPECT_EQ(string(4, '\0'), emp);
}
//------------------------------------------------------------------------------
TEST_F(Tools, zero_word_2_float) {

  string emp(4, '\0');
  float n = MmcsCorsikaTools::str2float_4byte_bin_map(emp);
  EXPECT_EQ(0.0, n);
}
//----------------------------------------------------------------------
TEST_F(MmcsCorsikaEventGetterTest, invalid_file_size) {

	EXPECT_EXCEPTION_OF_TYPE_FOR_FILE(
		FILE_SIZE_IS_NOT_MULTIPLE_OF_MMCS_BLOCKSIZE,
		"./MMCS_files/not_a_valid_MMCS_file"
	);
}
//----------------------------------------------------------------------
TEST_F(MmcsCorsikaEventGetterTest, gzip_file) {

	EXPECT_EXCEPTION_OF_TYPE_FOR_FILE(
		CAN_NOT_HANDLE_GZIP_FILE,
		"./MMCS_files/gzip_file_ending.tar.gz"
	);
}
//----------------------------------------------------------------------
TEST_F(MmcsCorsikaEventGetterTest, not_existing_file) {

	EXPECT_THROW(
		{MmcsCorsikaFullEventGetter event_getter("./no_such_path/no_such_file");},
		TracerException
	);
}
//----------------------------------------------------------------------
TEST_F(MmcsCorsikaEventGetterTest, not_existing_file_exception_type) {

	EXPECT_EXCEPTION_OF_TYPE_FOR_FILE(
		CAN_NOT_OPEN_MMCS_FILE,
		"./no_such_path/no_such_file"
	);
}
//----------------------------------------------------------------------
TEST_F(MmcsCorsikaEventGetterTest, sub_block_getter) {

	string filename = "./MMCS_files/cer001004";
	MmcsCorsikaSubBlockGetter fritz(filename);

	//fritz.print();

	while(fritz.has_still_sub_blocks_left()) {
		MmcsCorsikaSubBlock heinz_subl_bock = fritz.get_next_sub_block();	
		heinz_subl_bock[0] = heinz_subl_bock[0];
	}

	//fritz.print();
}
//----------------------------------------------------------------------
TEST_F(MmcsCorsikaEventGetterTest, FullEventGetter) {

	try{
		string filename = "./MMCS_files/cer000005";
		MmcsCorsikaFullEventGetter event_getter(filename);

		while(event_getter.has_still_events_left()) {
			
			MmcsCorsikaEvent event = event_getter.get_next_event();	
			//ListOfPropagations *photons = event.transform_to_mcTracer_photons();
			//photons->disp();
		}

	}catch(TracerException &e ){
		std::cout << e.what();
	}
}
//----------------------------------------------------------------------
TEST_F(MmcsCorsikaEventGetterTest, has_still_events_left) {

	try{
		MmcsCorsikaFullEventGetter event_getter("./MMCS_files/cer000005");
		EXPECT_TRUE(event_getter.has_still_events_left());
	}catch(TracerException &e ){
		std::cout << e.what();
	}
}
//----------------------------------------------------------------------
TEST_F(MmcsCorsikaEventGetterTest, photon_data) {

	try{
		MmcsCorsikaPhotonData photons;
		//photons.print();
	}catch(TracerException &e ){
		std::cout << e.what();
	}
}