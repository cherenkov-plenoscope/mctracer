//=================================
// include guard
#ifndef __PropagationConfigXmlFactory_H_INCLUDED__
#define __PropagationConfigXmlFactory_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Core/PropagationConfig.h"
#include "Xml/Xml.h"

//------------------------------------------------------------------------------
namespace Xml {
	namespace Configs {
		PropagationConfig get_PropagationConfig_from_node(Xml::Node node);
		std::string to_node(const PropagationConfig &settings);
	}
}//Xml
#endif // __PropagationConfigXmlFactory_H_INCLUDED__