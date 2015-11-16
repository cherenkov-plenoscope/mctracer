 /* example.i */
 %module mctracer
 %{
 /* Put header files here or function declarations like below */
 #include "Core/Printable.h"
 #include "Vector3D.h"
 #include "Rotation3D.h"
 #include "HomoTrafo3D.h"
 #include "Frame.h"
 #include "XmlFactory/WorldFactory.h"
 #include "TracerSettings.h"
 #include "FreeOrbitCamera.h"
 #include "Core/SurfaceEntity.h"
 #include "Disc.h"
 #include "Cylinder.h"
 #include "Plane.h"
 #include "Color.h"
 #include "Cameras/SkyDome.h"

 
 #include "SegmetedReflectorGenerator.h"
 #include "SphereCapWithHexagonalBound.h"
 #include "Photon.h"
 #include "PropagationEnvironment.h"

 #include "Geometry/Sphere.h"
 %}

 %apply int { size_t };
 %apply unsigned int { uint };

 
 %include <std_string.i>
 %include "Core/Printable.h"
 %include "Vector3D.h"
 %include "Rotation3D.h"
 %include "HomoTrafo3D.h"
 %include "Frame.h"
 %include "XmlFactory/WorldFactory.h"
 %include "TracerSettings.h"
 %include "FreeOrbitCamera.h"
 %include "Core/SurfaceEntity.h"
 %include "Disc.h"
 %include "Cylinder.h"
 %include "Plane.h"
 %include "Color.h"
 %include "Cameras/SkyDome.h"
 %include "SegmetedReflectorGenerator.h"
 %include "SphereCapWithHexagonalBound.h"
 %include "Photon.h"
 %include "PropagationEnvironment.h"

 %include "Geometry/Sphere.h"

 