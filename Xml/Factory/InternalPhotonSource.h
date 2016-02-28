//=================================
// include guard
#ifndef __InternalLightSourceXmlFactory_H_INCLUDED__
#define __InternalLightSourceXmlFactory_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Core/Photons.h"
#include "Xml/Xml.h"
using std::vector;
//------------------------------------------------------------------------------
namespace Xml {
	namespace InternalPhotonSource {
		vector<Photon*>* node2photons(const Xml::Node &node);
		vector<Photon*>* pointsource(const Xml::Node &node);
		vector<Photon*>* parallel_disc(const Xml::Node &node);
		void transform(const Xml::Node &node, vector<Photon*>* photons);

		class UnkownTypeOfLightSource : public TracerException {
			using TracerException::TracerException;
		};
	}//InternalPhotonSource
}//Xml
#endif // __InternalLightSourceXmlFactory_H_INCLUDED__