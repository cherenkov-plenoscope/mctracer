#include "gtest/gtest.h"
#include "Tools/UserInteraction.h"

int main(int argc, char **argv) {
	UserInteraction::print_welcome_screen();
  	::testing::InitGoogleTest(&argc, argv);
  	return RUN_ALL_TESTS();
}

