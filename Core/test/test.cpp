// Copyright 2018 Sebastian A. Mueller
#define CATCH_CONFIG_MAIN

#include <stdint.h>
#include <math.h>
#include <vector>
#include <string>
#include <sstream>
#include <exception>
#include <algorithm>
#include "Core/test/catch.hpp"
#include "Core/mctracer.h"

namespace re = relleums;

TEST_CASE("default_ctor", "Color") {
    re::Color c;
    CHECK(c.r == 0);
    CHECK(c.g == 0);
    CHECK(c.b == 0);
}

TEST_CASE("reflection_mix", "Color") {
    re::Color base(0, 0, 0);
    re::Color green(0, 100, 0);
    const double reflectivity = 0.5;
    base.reflection_mix(green, reflectivity);
    CHECK(base.r == 0);
    CHECK(base.g == 50);
    CHECK(base.b == 0);
}

TEST_CASE("ctor_mix", "Color") {
    std::vector<re::Color> rainbow;
    rainbow.push_back(re::Color(100, 0, 0));
    rainbow.push_back(re::Color(0, 100, 0));
    rainbow.push_back(re::Color(0, 0, 100));
    re::Color mix = re::Color(rainbow);
    CHECK(mix.r == Approx(33).margin(1));
    CHECK(mix.g == Approx(33).margin(1));
    CHECK(mix.b == Approx(33).margin(1));
}




/*
TEST_CASE("LinInterpolTest.linear_interpolation") {
	ve<ve<f8>> two_entry_table = {{0.0, 0.0}, {1.0, 1.0}};

	//  1-|     /   two_entry_table = y = f(x) = x
	//    |    /
	//    |   /
	//    |  /
	//    | /
	//    |/____.___
	//   0      1

	lict::function::LinInterpol f(two_entry_table);
	for (f8 x = -0.1; x < 1.1; x = x+0.011) {
    if (x >= 0.0 && x < 1.0)
      REQUIRE(f.evaluate(x) == Approx(x).margin(1e-9));
    else
      CHECK_THROWS_AS(f.evaluate(x), std::out_of_range);
  }
}

TEST_CASE("LinInterpolTest.max_value") {
  for (f8 amp = 0.0; amp < 1.337*4.2; amp = amp+1e-2) {
    ve<ve<f8>> table;
    for (f8 x = 0; x < 1.0; x = x+1e-2) {
        f8 y = amp*sin(x*2.0*M_PI);
        table.push_back({x, y});
    }
    lict::function::LinInterpol f(table);
    REQUIRE(amp == Approx(f.max()).margin(1e-6));
    REQUIRE(-amp == Approx(f.min()).margin(1e-6));
  }
}

TEST_CASE("Vec3.ORIGIN") {
  lict::Vec3 a = lict::ORIGIN;
  CHECK(0.0 == a.x);
  CHECK(0.0 == a.y);
  CHECK(0.0 == a.z);
  CHECK(0.0 == a.norm());
}

TEST_CASE("Vec3.UnitVectors") {
  lict::Vec3 a;
  a = lict::UNIT_X;
  CHECK(1.0 == a.x);
  CHECK(0.0 == a.y);
  CHECK(0.0 == a.z);
  CHECK(1.0 == a.norm());

  a = lict::UNIT_Y;
  CHECK(0.0 == a.x);
  CHECK(1.0 == a.y);
  CHECK(0.0 == a.z);
  CHECK(1.0 == a.norm());

  a = lict::UNIT_Z;
  CHECK(0.0 == a.x);
  CHECK(0.0 == a.y);
  CHECK(1.0 == a.z);
  CHECK(1.0 == a.norm());
}

TEST_CASE("Vec3Test.ConstructorAndGetter") {
  co f8 x = (rand()-.5);
  co f8 y = (rand()-.5);
  co f8 z = (rand()-.5);
  lict::Vec3 v(x, y, z);
  CHECK(x == v.x);
  CHECK(y == v.y);
  CHECK(z == v.z);
}

TEST_CASE("Vec3Test.SetterAndGetter") {
  co f8 x = (rand()-.5);
  co f8 y = (rand()-.5);
  co f8 z = (rand()-.5);
  lict::Vec3 v;
  v.set(x, y, z);
  CHECK(x == v.x);
  CHECK(y == v.y);
  CHECK(z == v.z);
}

TEST_CASE("Vec3Test.EuclideanNorm") {
  co f8 x = (rand()-.5);
  co f8 y = (rand()-.5);
  co f8 z = (rand()-.5);
  lict::Vec3 v(x, y, z);
  CHECK(sqrt(x*x+y*y+z*z) == v.norm());
  v.set(1.0, 0.0, 0.0);
  CHECK(1.0 == v.norm());
  v.set(0.0, 1.0, 0.0);
  CHECK(1.0 == v.norm());
  v.set(0.0, 0.0, 1.0);
  CHECK(1.0 == v.norm());
}

TEST_CASE("Vec3Test.crossUnitVectors") {
  lict::Vec3 u = lict::UNIT_X;
  lict::Vec3 v = lict::UNIT_Y;
  lict::Vec3 w = u.cross(v);
  CHECK(1.0 == w.z);
  CHECK(lict::UNIT_Z == w);
}
*/