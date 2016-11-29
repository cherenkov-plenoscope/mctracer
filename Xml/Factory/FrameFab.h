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
	Vec3 pos;
	Rot3 rot;

	FrameFab(const Xml::Node &node);
private:

	void fab_frame(const Xml::Node &node);
	Rot3 fab_rotation(const Xml::Node &node);
	Rot3 fab_rotation_xyz_angles(const Xml::Node &node);
	Rot3 fab_rotation_based_on_z_reflects(const Xml::Node &node);
	Rot3 fab_rotation_based_on_axis_and_angle(const Xml::Node &node);
	Vec3 fab_position(const Xml::Node &node);
	std::string fab_name(const Xml::Node &node);
};
#endif // __FrameFab_H_INCLUDED__ 