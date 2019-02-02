// Copyright 2014 Sebastian A. Mueller
#include "Xml/Factory/FunctionFromXml.h"
#include <vector>
#include "Tools/StringTools.h"
using std::vector;

namespace Xml {

Function::LinInterpol get_LinInterpol_from(const Xml::Node &node) {
    vector<vector<double>> table;
    for (
        Xml::Node child = node.first_child();
        child;
        child = child.next_child()
    ) {
        if (StringTools::is_equal(child.name(), "xy")) {
            vector<double> row = {
                child.to_double("x"),
                child.to_double("y")
            };
            table.push_back(row);
        }
    }
    return Function::LinInterpol(table);
}

}  // namespace Xml
