// Copyright 2014 Sebastian A. Mueller
#include <math.h>
#include "catch.hpp"
#include "merlict/FresnelRefractionAndReflection.h"
using namespace merlict;



TEST_CASE("FresnelRefractionAndReflectionTest: orthogonal_incident", "[merlict]") {
  double n_from = 1.0;
  double n_going_to = 1.33;

  Vec3 incident(0.0, 0.0, -1.0); incident.normalize();
  Vec3 normal(0.0, 0.0, 1.0); normal.normalize();
  FresnelRefractionAndReflection fresnel(incident, normal, n_from, n_going_to);

  CHECK(0.0 < fresnel.reflection_propability());
  CHECK(0.05 > fresnel.reflection_propability());
  CHECK(fresnel.get_refrac_dir_in_object_system() == incident);
}

TEST_CASE("FresnelRefractionAndReflectionTest: flat_incident", "[merlict]") {
  double n_from = 1.0;
  double n_going_to = 1.33;

  Vec3 incident(100.0, 0.0, -1.0); incident.normalize();
  Vec3 normal(0.0, 0.0, 1.0); normal.normalize();
  FresnelRefractionAndReflection fresnel(incident, normal, n_from, n_going_to);

  double incident_to_normal = incident.angle_in_between(normal);
  double outgoing_to_normal = asin(sin(incident_to_normal)* n_from/n_going_to);

  CHECK(fresnel.get_refrac_dir_in_object_system().angle_in_between(normal*-1.0) == outgoing_to_normal);
}

TEST_CASE("FresnelRefractionAndReflectionTest: orthogonal_incident_same_index", "[merlict]") {
  double n_from = 1.0;
  double n_going_to = n_from;

  Vec3 incident(0.0, 0.0, -1.0); incident.normalize();
  Vec3 normal(0.0, 0.0, 1.0); normal.normalize();
  FresnelRefractionAndReflection fresnel(incident, normal, n_from, n_going_to);

  CHECK(0.0 == fresnel.reflection_propability());
  CHECK(fresnel.get_refrac_dir_in_object_system() == incident);
}

TEST_CASE("FresnelRefractionAndReflectionTest: flat_incident_same_index", "[merlict]") {
  double n_from = 1.0;
  double n_going_to = n_from;

  Vec3 incident(100.0, 0.0, -1.0); incident.normalize();
  Vec3 normal(0.0, 0.0, 1.0); normal.normalize();
  FresnelRefractionAndReflection fresnel(incident, normal, n_from, n_going_to);

  CHECK(fresnel.get_refrac_dir_in_object_system() == incident);
}
