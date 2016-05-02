#include "gtest/gtest.h"
#include "Core/Frame.h"
#include "Core/Vec3.h"
#include "Core/Rot3.h"
#include "Core/Ray.h"
#include "Core/HomTra3.h"
using std::cout;

class SizeOfTest : public ::testing::Test {};
//------------------------------------------------------------------------------
TEST_F(SizeOfTest, sizes_of_mct) {

    cout << "Size in bytes\n";

    Vec3 v;
    cout << "Vec3    " << sizeof(v) << "\n";

    Rot3 r;
    cout << "Rot3    " << sizeof(r) << "\n";

    HomTra3 h;
    cout << "HomTra3 " << sizeof(h) << "\n";

    Ray ray(Vec3::null, Vec3::unit_z);
    cout << "Ray     " << sizeof(ray) << "\n";

    Frame tree("0123456789", Vec3::null, Rot3::null);
    cout << "Frame   " << sizeof(tree) << "\n";

    Color col(123, 24, 12);
    cout << "Color   " << sizeof(col) << "\n";     
}
//------------------------------------------------------------------------------