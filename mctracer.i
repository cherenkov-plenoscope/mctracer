%module mctracer
%apply int { size_t };
%apply unsigned int { uint };

%include <std_string.i>
%include <std_vector.i>

%{
	#include "Core/Printable.h"
	#include "Core/Vec3.h"
	#include "Core/HomTra3.h"
	#include "Core/Ray.h"
	#include "Core/RayForPropagation.h"
	#include "Core/Color.h"
	#include "Core/Frame.h"
	#include "Core/FresnelRefractionAndReflection.h"
	#include "Core/Intersection.h"
	#include "Core/Photon.h"
	#include "Core/PhotonMcTruth.h"
	#include "Core/Photons.h"
	#include "Core/PhysicalConstants.h"
	#include "Core/PropagationEnvironment.h"
	#include "Core/PseudoRandomNumberGenerator.h"
	#include "Core/Rot3.h"
	#include "Core/SurfaceEntity.h"
	#include "Core/TimeStamp.h"
	#include "Core/PropagationConfig.h"

	#include "Core/Function/LimitsFunction.h"
	#include "Core/Function/Func1DFunction.h"
	#include "Core/Function/ConstantFunction.h"
	#include "Core/Function/ConcatFunction.h"
	#include "Core/Function/LinInterpolFunction.h"
	#include "Core/Function/Polynom3Function.h"

	#include "PrismZ.h"
	#include "BiConvexLens.h"
	#include "BiConvexLensHexBound.h"
	#include "Scenery/Primitive/Cylinder.h"
	#include "CylinderPrismZ.h"
	#include "SurfaceWithOuterPrismBound.h"
	#include "Disc.h"
	#include "EllipticalCapWithHexagonalBound.h"
	#include "HexagonalPrismZ.h"
	#include "Plane.h"
	#include "RectangularPrismZ.h"
	#include "SphereCapWithCylinderBound.h"
	#include "SphereCapWithHexagonalBound.h"
	#include "Scenery/Sphere.h"
	#include "Scenery/StereoLitography.h"
	#include "Scenery/Triangle.h"

	#include "Cameras/CameraDevice.h"
	#include "Cameras/ApertureCamera.h"
	#include "Cameras/CameraImage.h"
	#include "Cameras/CameraManForFieldOfView.h"
	#include "Cameras/CameraManForRotation.h"
	#include "Cameras/CameraManForStereo3D.h"
	#include "Cameras/CameraManForTranslation.h"
	#include "Cameras/CameraMan.h"
	#include "Cameras/CameraRay.h"
	#include "Cameras/FreeOrbitCamera.h"
	#include "Cameras/PinHoleCamera.h"
	#include "Cameras/SkyDome.h"

	#include "XmlFactory/WorldFactory.h"
	//#include "Corsika/EventIo.h"
	#include "TelescopeArrayControl.h"
	#include "PhotonSensor/PhotonSensor.h"

 %}

namespace std {
    %template(VectorDouble) vector<double>;
    %template(VecVecdouble) vector< vector<double> >;
    %template(VectorPhoton) vector<Photon*>;
    %template(VectorSensor) vector<PhotonSensor::Sensor*>;
};


%feature("autodoc", "1");

%include "Core/Printable.h"
%include "Core/Vec3.h"
%include "Core/HomTra3.h"
%include "Core/Ray.h"
%include "Core/RayForPropagation.h"
%include "Core/Color.h"
%include "Core/Frame.h"
%include "Core/FresnelRefractionAndReflection.h"
%include "Core/Intersection.h"
%include "Core/Photon.h"
%include "Core/PhotonMcTruth.h"
%include "Core/Photons.h"
%include "Core/PhysicalConstants.h"
%include "Core/PropagationEnvironment.h"
%include "Core/PseudoRandomNumberGenerator.h"
%include "Core/Rot3.h"
%include "Core/SurfaceEntity.h"
%include "Core/TimeStamp.h"
%include "Core/PropagationConfig.h"

%include "Core/Function/LimitsFunction.h"
%include "Core/Function/Func1DFunction.h"
%include "Core/Function/ConstantFunction.h"
%include "Core/Function/ConcatFunction.h"
%include "Core/Function/LinInterpolFunction.h"
%include "Core/Function/Polynom3Function.h"

%include "BiConvexLens.h"
%include "BiConvexLensHexBound.h"
%include "Scenery/Primitive/Cylinder.h"
%include "PrismZ.h"
%include "CylinderPrismZ.h"
%include "SurfaceWithOuterPrismBound.h"
%include "Disc.h"
%include "EllipticalCapWithHexagonalBound.h"
%include "HexagonalPrismZ.h"
%include "Plane.h"
%include "RectangularPrismZ.h"
%include "SphereCapWithCylinderBound.h"
%include "SphereCapWithHexagonalBound.h"
%include "Scenery/Sphere.h"
%include "Scenery/StereoLitography.h"
%include "Scenery/Triangle.h"

%include "Cameras/CameraDevice.h"
%include "Cameras/ApertureCamera.h"
%include "Cameras/CameraImage.h"
%include "Cameras/CameraManForFieldOfView.h"
%include "Cameras/CameraManForRotation.h"
%include "Cameras/CameraManForStereo3D.h"
%include "Cameras/CameraManForTranslation.h"
%include "Cameras/CameraMan.h"
%include "Cameras/CameraRay.h"
%include "Cameras/FreeOrbitCamera.h"
%include "Cameras/PinHoleCamera.h"
%include "Cameras/SkyDome.h"

%include "XmlFactory/WorldFactory.h"
//%include "Corsika/EventIo.h"
%include "TelescopeArrayControl.h"
%include "PhotonSensor/PhotonSensor.h"
