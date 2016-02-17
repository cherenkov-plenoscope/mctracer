//=================================
// include guard
#ifndef __TracerSettingsXmlFactory_H_INCLUDED__
#define __TracerSettingsXmlFactory_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Core/TracerSettings.h"
#include "Xml/Xml.h"

//------------------------------------------------------------------------------
namespace Xml {
	namespace Configs {
		TracerSettings get_TracerSettings_from_node(Xml::Node node);
		std::string to_node(const TracerSettings &settings);
	}
}//Xml
#endif // __TracerSettingsXmlFactory_H_INCLUDED__