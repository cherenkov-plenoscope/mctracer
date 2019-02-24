// Copyright 2014 Sebastian A. Mueller
#include "gtest/gtest.h"
#include <iostream>
#include "Core/welcome_screen.h"

int main(int argc, char **argv) {
    std::cout << relleums::welcome_screen();
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
