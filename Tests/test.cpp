// Copyright 2014 Sebastian A. Mueller
#include "gtest/gtest.h"
#include "Tools/UserInteraction.h"

int main(int argc, char **argv) {
    relleums::UserInteraction::print_welcome_screen();
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
