#include "Xml/Factory/Function.h"
#include "Tools/StringTools.h"
using std::vector;
using std::string;


//------------------------------------------------------------------------------
namespace Xml {
//------------------------------------------------------------------------------
void init_function_with_node(Function::Constant* function, const Node &node) {
    function->init(
        node.to_double("value"),
        limits_from_node(node));
}
//------------------------------------------------------------------------------
void init_function_with_node(Function::Polynom3* function, const Node &node) {
    function->init(
        node.to_double("x3"),
        node.to_double("x2"),
        node.to_double("x1"),
        node.to_double("x0"),
        limits_from_node(node));
}
//------------------------------------------------------------------------------
void init_function_with_node(Function::LinInterpol* function, const Node &node) {

    vector<vector<double>> table;
    for(
        Xml::Node child=node.first_child(); 
        child; 
        child=child.next_child()
    ) {
        if(StringTools::is_equal(child.name(), "xy")) {
            vector<double> row = {
                child.to_double("x"),
                child.to_double("y")
            };
            table.push_back(row);
        }
    }
    function->init(table);		
}
//------------------------------------------------------------------------------
void init_function_with_node(Function::Concat* function, const Node &node, FunctionMap* functions) {
    string attribute = "f0";    
    vector<Function::Func1D*> funcs;
    unsigned int count = 0;
    while (node.has_attribute(attribute)) {

        funcs.push_back(functions->get(node.attribute(attribute)));
        count++;
        attribute = "f" + std::to_string(count);
    };

    function->init(funcs);
}
//------------------------------------------------------------------------------
Function::Limits limits_from_node(const Xml::Node &node) {
    return Function::Limits(
        node.to_double("lower_limit"), 
        node.to_double("upper_limit")
    );    
}
//------------------------------------------------------------------------------
}//Xml
