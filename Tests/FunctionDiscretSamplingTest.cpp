#include "gtest/gtest.h"
#include "Core/Function/Function.h"

class FunctionDiscretSamplingTest : public ::testing::Test {};
//------------------------------------------------------------------------------
TEST_F(FunctionDiscretSamplingTest, Config) {

	Function::DiscretSampling::Config config;
	config.slice_width = 1e-1;

	Function::Polynom3 poly3(0.0, 0.0, 1.0, 0.0, Function::Limits(-1.0, 1.0));

	Function::DiscretSampling::LookUpTable table(&poly3, config);
}
//------------------------------------------------------------------------------
TEST_F(FunctionDiscretSamplingTest, bad_sample_width_Config) {

	Function::DiscretSampling::Config config;
	config.slice_width = -1e-1;

	Function::Polynom3 poly3(0.0, 0.0, 1.0, 0.0, Function::Limits(-1.0, 1.0));

	EXPECT_THROW(
		Function::DiscretSampling::LookUpTable table(&poly3, config), 
		Function::DiscretSampling::LookUpTable::BadInput
	);
}
//------------------------------------------------------------------------------
TEST_F(FunctionDiscretSamplingTest, zero_function_range) {

	Function::DiscretSampling::Config config;
	config.slice_width = 1e-1;

	Function::Polynom3 poly3(0.0, 0.0, 1.0, 0.0, Function::Limits(0.0, 0.0));

	EXPECT_THROW(
		Function::DiscretSampling::LookUpTable table(&poly3, config), 
		Function::DiscretSampling::LookUpTable::BadInput
	);
}
//------------------------------------------------------------------------------
TEST_F(FunctionDiscretSamplingTest, to_many_samples) {

	Function::DiscretSampling::Config config;
	config.slice_width = 1e-9;
	config.samples_per_slice = 1e9;

	Function::Polynom3 poly3(0.0, 0.0, 1.0, 0.0, Function::Limits(-1.0, 1.0));

	EXPECT_THROW(
		Function::DiscretSampling::LookUpTable table(&poly3, config), 
		Function::DiscretSampling::LookUpTable::MemoryProblemMaybeTooManySamples
	);
}
//------------------------------------------------------------------------------
TEST_F(FunctionDiscretSamplingTest, bad_offset_get_discrete_samples) {

	Function::DiscretSampling::Config config;
	config.slice_width = 1e-1;
	config.samples_per_slice = 1e1;

	Function::Polynom3 poly3(0.0, 0.0, 1.0, 0.0, Function::Limits(-1.0, 1.0));

	
	Function::DiscretSampling::LookUpTable table(&poly3, config);

	const std::vector<double>* discrete_samples;

	EXPECT_THROW(
		discrete_samples = table.at(-0.1),
		TracerException
	);

	EXPECT_NO_THROW(
		discrete_samples = table.at(0.0)
	);

	EXPECT_NO_THROW(
		discrete_samples = table.at(0.0)
	);

	EXPECT_NO_THROW(
		discrete_samples = table.at(0.99)
	);

	EXPECT_THROW(
		discrete_samples = table.at(1.0),
		TracerException
	);

	EXPECT_THROW(
		discrete_samples = table.at(1.1),
		TracerException
	);

	discrete_samples = table.at(0.5);
	EXPECT_EQ(20, discrete_samples->size());
}
//------------------------------------------------------------------------------
TEST_F(FunctionDiscretSamplingTest, get_discrete_samples) {

	Function::DiscretSampling::Config config;
	config.slice_width = 1e-2;
	config.samples_per_slice = 1e3;

	Function::Polynom3 poly3(0.0, 0.0, 1.0, 0.0, Function::Limits(-1.0, 1.0));

	
	Function::DiscretSampling::LookUpTable table(&poly3, config);
	EXPECT_EQ(200, table.size());

	const std::vector<double>* discrete_samples;

	for(double s=0.0; s<1.0; s+=1e-4) {
		discrete_samples = table.at(s);

		EXPECT_EQ(200, discrete_samples->size());
	}
}