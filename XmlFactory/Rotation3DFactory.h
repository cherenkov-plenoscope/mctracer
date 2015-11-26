//=================================
// include guard
#ifndef __ROTATION3DFACTORY_H_INCLUDED__
#define __ROTATION3DFACTORY_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Core/Rotation3D.h"
#include "XmlIO/XmlFileIo.h"

//------------------------------------------------------------------------------
class Rotation3DFactory : public XmlFileIo{
	Rotation3D rot;
public:
	Rotation3DFactory(const std::string name ,const pugi::xml_node node);
	Rotation3D get_rot()const;
private:

	Rotation3D extract_frame_rotation_xyz_angles(
		const std::string name, const pugi::xml_node node
	);
};
#endif // __ROTATION3DFACTORY_H_INCLUDED__ 