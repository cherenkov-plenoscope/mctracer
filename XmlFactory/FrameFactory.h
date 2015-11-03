//=================================
// include guard
#ifndef __FRAMEFACTORY_H_INCLUDED__
#define __FRAMEFACTORY_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Core/Frame.h"
#include "XmlIO/XmlFileIo.h"

//------------------------------------------------------------------------------
class FrameFactory : public XmlFileIo{
	std::string name;
	Vector3D position;
	Rotation3D rotation;

public:
	FrameFactory(const pugi::xml_node node);
	std::string get_name()const;
	Vector3D get_position()const;
	Rotation3D get_rotation()const;
private:
	void extract_frame(const pugi::xml_node node);
	Rotation3D extract_frmae_rotation(const pugi::xml_node node);
	Rotation3D extract_frmae_rotation_xyz_angles(const pugi::xml_node node);
	Rotation3D extract_frmae_rotation_based_on_z_reflects(const pugi::xml_node node);
	Vector3D extract_frmae_position(const pugi::xml_node node);
	std::string extract_frmae_name(const pugi::xml_node node);
};
#endif // __FRAMEFACTORY_H_INCLUDED__ 