//=================================
// include guard
#ifndef __FrameFab_H_INCLUDED__
#define __FrameFab_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Core/Frame.h"
#include "Xml/Xml.h"

//------------------------------------------------------------------------------
class FrameFab {
public:
	std::string name;
	Vector3D pos;
	Rotation3D rot;

	FrameFab(const Xml::Node &node);
private:

	void fab_frame(const Xml::Node &node);
	Rotation3D fab_rotation(const Xml::Node &node);
	Rotation3D fab_rotation_xyz_angles(const Xml::Node &node);
	Rotation3D fab_rotation_based_on_z_reflects(const Xml::Node &node);
	Vector3D fab_position(const Xml::Node &node);
	std::string fab_name(const Xml::Node &node);
};
#endif // __FrameFab_H_INCLUDED__ 