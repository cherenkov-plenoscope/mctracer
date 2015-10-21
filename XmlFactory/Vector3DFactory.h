//=================================
// include guard
#ifndef __VECTOR3DFACTORY_H_INCLUDED__
#define __VECTOR3DFACTORY_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Vector3D.h"
#include "XmlIO/XmlFileIo.h"

//------------------------------------------------------------------------------
class Vector3DFactory : public XmlFileIo{
	Vector3D vec;
public:
	Vector3DFactory(const std::string name ,const pugi::xml_node node);
	Vector3D get_vec()const;
};
#endif // __VECTOR3DFACTORY_H_INCLUDED__ 