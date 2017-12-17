#include "gtest/gtest.h"
#include "SignalProcessing/DiscretSampling.h"
#include "Core/Function/Function.h"

class DiscretSamplingTest : public ::testing::Test {};

TEST_F(DiscretSamplingTest, Config) {
	DiscretSampling::Config config;
	config.slice_width = 1e-1;
	Function::Polynom3 poly3(0.0, 0.0, 1.0, 0.0, Function::Limits(-1.0, 1.0));
	DiscretSampling::LookUpTable table(&poly3, config);
}

TEST_F(DiscretSamplingTest, bad_sample_width_Config) {
	DiscretSampling::Config config;
	config.slice_width = -1e-1;
	Function::Polynom3 poly3(0.0, 0.0, 1.0, 0.0, Function::Limits(-1.0, 1.0));
	EXPECT_THROW(
		DiscretSampling::LookUpTable table(&poly3, config),
		std::invalid_argument);
}

TEST_F(DiscretSamplingTest, zero_function_range) {
	DiscretSampling::Config config;
	config.slice_width = 1e-1;
	Function::Polynom3 poly3(0.0, 0.0, 1.0, 0.0, Function::Limits(0.0, 0.0));
	EXPECT_THROW(
		DiscretSampling::LookUpTable table(&poly3, config),
		std::invalid_argument);
}

/*TEST_F(DiscretSamplingTest, too_many_samples) {
	// Not all machines will throw a malloc exception at the same time
	// or in the same way.
	DiscretSampling::Config config;
	config.slice_width = 1e-9;
	config.samples_per_slice = 1e9;
	Function::Polynom3 poly3(0.0, 0.0, 1.0, 0.0, Function::Limits(-1.0, 1.0));
	EXPECT_THROW(
		DiscretSampling::LookUpTable table(&poly3, config),
		DiscretSampling::LookUpTable::MemoryProblemMaybeTooManySamples
	);
}*/

TEST_F(DiscretSamplingTest, bad_offset_get_discrete_samples) {
	DiscretSampling::Config config;
	config.slice_width = 1e-1;
	config.samples_per_slice = 1e1;
	Function::Polynom3 poly3(0.0, 0.0, 1.0, 0.0, Function::Limits(-1.0, 1.0));
	DiscretSampling::LookUpTable table(&poly3, config);
	const std::vector<double>* discrete_samples;
	EXPECT_THROW(
		discrete_samples = table.at(-0.1),
		std::out_of_range);
	EXPECT_NO_THROW(
		discrete_samples = table.at(0.0));
	EXPECT_NO_THROW(
		discrete_samples = table.at(0.0));
	EXPECT_NO_THROW(
		discrete_samples = table.at(0.99));
	EXPECT_THROW(
		discrete_samples = table.at(1.0),
		std::out_of_range);
	EXPECT_THROW(
		discrete_samples = table.at(1.1),
		std::out_of_range);
	discrete_samples = table.at(0.5);
	EXPECT_EQ(20u, discrete_samples->size());
}

TEST_F(DiscretSamplingTest, get_discrete_samples) {
	DiscretSampling::Config config;
	config.slice_width = 1e-2;
	config.samples_per_slice = 1e3;
	Function::Polynom3 poly3(0.0, 0.0, 1.0, 0.0, Function::Limits(-1.0, 1.0));
	DiscretSampling::LookUpTable table(&poly3, config);
	EXPECT_EQ(200u, table.size());
	const std::vector<double>* discrete_samples;
	for(double s=0.0; s<1.0; s+=1e-4) {
		discrete_samples = table.at(s);
		EXPECT_EQ(200u, discrete_samples->size());
	}
}