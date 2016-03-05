#include "gtest/gtest.h"
#include "CorsikaIO/MmcsCorsikaFullEventGetter.h"
#include "CorsikaIO/MmcsCorsikaSubBlockGetter.h"
#include "CorsikaIO/MmcsCorsikaPhotonData.h"
#include "CorsikaIO/MmcsCorsikaTools.h"

class MmcsCorsikaEventGetterTest : public ::testing::Test {};
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
TEST_F(MmcsCorsikaEventGetterTest, word_2_float) {

  string h = "hans";

  float H = MmcsCorsikaTools::str2float_4byte_bin_map(h);
  string r = MmcsCorsikaTools::float2str_4byte_bin_map(H);

  EXPECT_EQ(h, r);
}
//------------------------------------------------------------------------------
TEST_F(MmcsCorsikaEventGetterTest, float_2_word) {

  float leet = 1.337;

  string leet_word = MmcsCorsikaTools::float2str_4byte_bin_map(leet);
  float r_leet = MmcsCorsikaTools::str2float_4byte_bin_map(leet_word);

  EXPECT_EQ(leet, r_leet);
}
//------------------------------------------------------------------------------
TEST_F(MmcsCorsikaEventGetterTest, zero_float_2_word) {

  float n = 0.0;
  string emp = MmcsCorsikaTools::float2str_4byte_bin_map(n);
  EXPECT_EQ(string(4, '\0'), emp);
}
//------------------------------------------------------------------------------
TEST_F(MmcsCorsikaEventGetterTest, zero_word_2_float) {

  string emp(4, '\0');
  float n = MmcsCorsikaTools::str2float_4byte_bin_map(emp);
  EXPECT_EQ(0.0, n);
}
//----------------------------------------------------------------------
TEST_F(MmcsCorsikaEventGetterTest, invalid_file_size) {

	EXPECT_THROW(
		MmcsCorsikaFullEventGetter event_getter("./MMCS_files/not_a_valid_MMCS_file");
		while(event_getter.has_still_events_left())
			MmcsCorsikaEvent event = event_getter.get_next_event();
		,
		TracerException
	);
}
//----------------------------------------------------------------------
TEST_F(MmcsCorsikaEventGetterTest, gzip_file) {
	
	EXPECT_THROW(
		MmcsCorsikaFullEventGetter event_getter("./MMCS_files/gzip_file_ending.tar.gz");
		while(event_getter.has_still_events_left())
				MmcsCorsikaEvent event = event_getter.get_next_event();
		,
		TracerException
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

	EXPECT_THROW(
		MmcsCorsikaFullEventGetter event_getter("./no_such_path/no_such_file");
		while(event_getter.has_still_events_left())
				MmcsCorsikaEvent event = event_getter.get_next_event();
		,
		TracerException
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
//----------------------------------------------------------------------
//TEST_F(MmcsCorsikaEventGetterTest, reader_speed) {
// 2015 Oct 01 read 2.3GByte 10k events: 58s
// 2015 Oct 01 read 2.3GByte 10k events and converting all photons to tracer photons: 120s