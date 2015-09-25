 /* example.i */
 %module mctracer
 %{
 /* Put header files here or function declarations like below */
 #include "Vector3D.h"
 #include "Rotation3D.h"
 #include "HomoTrafo3D.h"
 #include "Frame.h"
 #include "WorldFactory.h"
 #include "TracerSettings.h"
 #include "FreeOrbitCamera.h"
 #include "Core/SurfaceEntity.h"
 #include "Disc.h"
 #include "Cylinder.h"
 #include "Color.h"
 #include "SegmetedReflectorGenerator.h"
 #include "SphereCapWithHexagonalBound.h"
 #include "Photon.h"
 #include "PropagationEnvironment.h"
 %}
 
 %include <std_string.i>
 %include "Vector3D.h"
 %include "Rotation3D.h"
 %include "HomoTrafo3D.h"
 %include "Frame.h"
 %include "WorldFactory.h"
 %include "TracerSettings.h"
 %include "FreeOrbitCamera.h"
 %include "Core/SurfaceEntity.h"
 %include "Disc.h"
 %include "Cylinder.h"
 %include "Color.h"
 %include "SegmetedReflectorGenerator.h"
 %include "SphereCapWithHexagonalBound.h"
 %include "Photon.h"
 %include "PropagationEnvironment.h"

 