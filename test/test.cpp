#include "testVector3D.cpp"
#include "RayTest.cpp"
#include "RayAndBoundingSphereOfFrameInteractionTest.cpp"
#include "testRotation3D.cpp"
#include "testPhoton.cpp"
#include "testWorldFactory.cpp"
#include "testSmartImage.cpp"
#include "testCartesianFrame.cpp"
#include "testReflectionProperties.cpp"
#include "testFunction1D.cpp"
#include "ToolTest.cpp"
#include "SphereTest.cpp"
#include "DiscTest.cpp"
#include "SphereIntersectionTest.cpp"
#include "QuadraticEquationTest.cpp"
#include "CameraManTest.cpp"
//#include "OctTreeCubeTest.cpp"
#include "ApertureCameraTest.cpp"
#include "MmcsCorsikaEventGetterTest.cpp"
#include "gtest/gtest.h"
#include "FiniteStateMashineTEST.cpp"
#include "ConfigurationFactoryTest.cpp"
#include "RayAndFrameInteractionTest.cpp"
#include "XyPlaneRayEquationTest.cpp"
#include "PlaneIntersectionTest.cpp"
#include "ZaxisCylinderRayIntersectionEquationTest.cpp"
#include "HexagonalPrismZTest.cpp"
#include "BiConvexLensTest.cpp"
#include "HomoTrafo3DTest.cpp"

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
