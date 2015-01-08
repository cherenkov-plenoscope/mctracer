#include <iostream> 
#include <string>
#include <math.h>

#include "gtest/gtest.h"
#include "ConfigurationFactory.h"

using namespace std;

// The fixture for testing class Foo.
class ConfigurationFactoryTest : public ::testing::Test {
protected:
	// You can remove any or all of the following functions if its body
	// is empty.
	bool exception_detected = false;
	ConfigurationFactoryTest() {
	// You can do set-up work for each test here.
	}

	virtual ~ConfigurationFactoryTest() {
	// You can do clean-up work that doesn't throw exceptions here.
	}

	// If the constructor and destructor are not enough for setting up
	// and cleaning up each test, you can define the following methods:

	virtual void SetUp() {
	// Code here will be called immediately after the constructor (right
	// before each test).
		exception_detected = false;
	}

	virtual void TearDown() {
	// Code here will be called immediately after each test (right
	// before the destructor).
	}

	// Objects declared here can be used by all tests in the test case for Foo.
};
//------------------------------------------------------------------------------
TEST_F(ConfigurationFactoryTest, constructor) {
	ConfigurationFactory config;
	EXPECT_ANY_THROW({config.create_default_config_file();});
}
//------------------------------------------------------------------------------