//=================================
// include guard
#ifndef __VisualConfigXmlFactory_H_INCLUDED__
#define __VisualConfigXmlFactory_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Visual/Config.h"
#include "Xml/Xml.h"

//------------------------------------------------------------------------------
namespace Xml {
	namespace Configs {
		Config get_VisualConfig_from_node(Xml::Node node);
		std::string to_node(const Config &visual_config);
	}
}//Xml
#endif // __VisualConfigXmlFactory_H_INCLUDED__