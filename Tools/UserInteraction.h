// Copyright 2014 Sebastian A. Mueller
#ifndef TOOLS_USERINTERACTION_H_
#define TOOLS_USERINTERACTION_H_

#include "Core/Vec3.h"

namespace relleums {
namespace UserInteraction {

void print_welcome_screen();
void ClearScreen();
bool is_Escape_key(const int key);
Vec3 get_Vec3();
static const unsigned int space_key = 32;

}
}  // namespace relleums

#endif  // TOOLS_USERINTERACTION_H_
