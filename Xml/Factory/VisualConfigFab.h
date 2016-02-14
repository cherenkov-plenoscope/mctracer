//=================================
// include guard
#ifndef __VisualConfigXmlFactory_H_INCLUDED__
#define __VisualConfigXmlFactory_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Cameras/VisualConfig.h"
#include "Xml/Xml.h"

//------------------------------------------------------------------------------
namespace Xml {
	namespace Configs {
		VisualConfig get_VisualConfig_from_node(Xml::Node node);
		std::string to_node(const VisualConfig &visual_config);
	}
}//Xml
#endif // __VisualConfigXmlFactory_H_INCLUDED__