#include "gtest/gtest.h"
#include "Core/Frame.h"
#include "Core/Vec3.h"
#include "Core/Rot3.h"
#include "Core/Ray.h"
#include "Core/Photon.h"
#include "Cameras/CameraRay.h"
#include "Core/Color.h"
#include "Core/HomTra3.h"
#include "Core/PropagationEnvironment.h"
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

    CameraRay camray(Vec3::null, Vec3::unit_z);
    cout << "CameraRay     " << sizeof(camray) << "\n";

    RayForPropagation rayfp(Vec3::null, Vec3::unit_z);
    cout << "RayForPropagation     " << sizeof(rayfp) << "\n";

    Photon ph(Vec3::null, Vec3::unit_z, 433e-9);
    cout << "Photon     " << sizeof(ph) << "\n";

    Intersection is = Intersection::void_intersection;
    cout << "Intersection     " << sizeof(is) << "\n";

    Frame tree("0123456789", Vec3::null, Rot3::null);
    cout << "Frame   " << sizeof(tree) << "\n";

    PropagationEnvironment environment = PropagationEnvironment::default_environment;
    cout << "PropagationEnvironment   " << sizeof(environment) << "\n";

    Color col(123, 24, 12);
    cout << "Color   " << sizeof(col) << "\n";     
}
//------------------------------------------------------------------------------
#include "Core/Random/Random.h"
#include <vector>
#include <array>
#include <math.h>
#include <algorithm>
TEST_F(SizeOfTest, speed_of_vector_ctor_and_tear_down) {
 
    std::array<double, 1000000> results; 
    Random::Mt19937 prng(0);

    for(uint i=0; i<results.size(); i++) {
        const uint number_of_pushs = uint(ceil(20.0*prng.uniform()));
        
        std::vector<double> vec;

        for(uint p=0; p<number_of_pushs; p++)
            vec.push_back(prng.uniform());

        results[i] = *std::min_element(vec.begin(), vec.end());
    }

    std::cout << *std::max_element(results.begin(), results.end()) << "\n";
}
//------------------------------------------------------------------------------
TEST_F(SizeOfTest, speed_of_permanent_vector) {
 
    std::array<double, 1000000> results; 
    Random::Mt19937 prng(0);
    std::vector<double> vec;
    //vec.reserve(32);

    for(uint i=0; i<results.size(); i++) {
        const uint number_of_pushs = uint(ceil(30.0*prng.uniform()));
        
        vec.clear();
        for(uint p=0; p<number_of_pushs; p++)
            vec.push_back(prng.uniform());

        results[i] = *std::min_element(vec.begin(), vec.end());
    }

    std::cout << *std::max_element(results.begin(), results.end()) << "\n";
}
//------------------------------------------------------------------------------
TEST_F(SizeOfTest, speed_of_permanent_array) {
 
    std::array<double, 1000000> results; 
    Random::Mt19937 prng(0);
    std::array<double, 32> arr;
    uint x = 0;

    for(uint i=0; i<results.size(); i++) {
        const uint number_of_pushs = uint(ceil(30.0*prng.uniform()));
        
        x = 0;
        for(uint p=0; p<number_of_pushs; p++) {
            arr[x] = prng.uniform();
            x++;
        }

        results[i] = *std::min_element(arr.begin(), arr.begin()+x);
    }

    std::cout << *std::max_element(results.begin(), results.end()) << "\n";
}
//------------------------------------------------------------------------------