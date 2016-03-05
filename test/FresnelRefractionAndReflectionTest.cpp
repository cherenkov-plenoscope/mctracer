#include "gtest/gtest.h"
#include "Core/FresnelRefractionAndReflection.h"

using namespace std;

class FresnelRefractionAndReflectionTest : public ::testing::Test {
};
//----------------------------------------------------------------------
TEST_F(FresnelRefractionAndReflectionTest, orthogonal_incident) {
  double n_from = 1.0;
  double n_going_to = 1.33;

  Vector3D incident(0.0,0.0,-1.0); incident.normalize();
  Vector3D normal(0.0,0.0,1.0); normal.normalize();
  FresnelRefractionAndReflection fresnel(incident,normal,n_from,n_going_to);

  EXPECT_TRUE(0.0 < fresnel.reflection_propability() && 
              0.05 > fresnel.reflection_propability());
  EXPECT_EQ(incident, fresnel.get_refrac_dir_in_object_system());
}
//----------------------------------------------------------------------
TEST_F(FresnelRefractionAndReflectionTest, flat_incident) {
  double n_from = 1.0;
  double n_going_to = 1.33;

  Vector3D incident(100.0,0.0,-1.0); incident.normalize();
  Vector3D normal(0.0,0.0,1.0); normal.normalize();
  FresnelRefractionAndReflection fresnel(incident,normal,n_from,n_going_to);

  double incident_to_normal = incident.get_angle_in_between_in_rad(normal);

  double outgoing_to_normal = asin(sin(incident_to_normal)* n_from/n_going_to);

  EXPECT_EQ(
    outgoing_to_normal, 
    fresnel.get_refrac_dir_in_object_system().get_angle_in_between_in_rad(normal*-1.0)
  );
}
//----------------------------------------------------------------------
TEST_F(FresnelRefractionAndReflectionTest, orthogonal_incident_same_index) {
  double n_from = 1.0;
  double n_going_to = n_from;

  Vector3D incident(0.0,0.0,-1.0); incident.normalize();
  Vector3D normal(0.0,0.0,1.0); normal.normalize();
  FresnelRefractionAndReflection fresnel(incident,normal,n_from,n_going_to);

  EXPECT_TRUE(0.0 == fresnel.reflection_propability());
  EXPECT_EQ(incident, fresnel.get_refrac_dir_in_object_system());
}
//----------------------------------------------------------------------
TEST_F(FresnelRefractionAndReflectionTest, flat_incident_same_index) {
  double n_from = 1.0;
  double n_going_to = n_from;

  Vector3D incident(100.0,0.0,-1.0); incident.normalize();
  Vector3D normal(0.0,0.0,1.0); normal.normalize();
  FresnelRefractionAndReflection fresnel(incident,normal,n_from,n_going_to);

  EXPECT_EQ(incident, fresnel.get_refrac_dir_in_object_system());
}