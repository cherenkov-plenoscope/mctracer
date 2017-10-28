#include "FunctionFromXml.h"
#include "Tools/StringTools.h"
using std::vector;

namespace Xml { 
//------------------------------------------------------------------------------
Function::LinInterpol get_LinInterpol_from(const Xml::Node &node) {

	vector<vector<double>> table;
	for(Xml::Node child=node.first_child();	child; child=child.next_child()) {
		if(StringTools::is_equal(child.name(), "xy")) {
			vector<double> row = {
				child.attribute2double("x"),
				child.attribute2double("y")
			};
			table.push_back(row);
		}
	}
	return Function::LinInterpol(table);
}
//------------------------------------------------------------------------------
}//Xml