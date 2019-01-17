#include "Xml/Factory/Function.h"
#include "Tools/StringTools.h"
using std::vector;
using std::string;


//------------------------------------------------------------------------------
namespace Xml {
//------------------------------------------------------------------------------
void init_function_with_node(Function::Constant* function, const Node &node) {
    function->init(
        node.attribute2double("value"),
        limits_from_node(node));
}
//------------------------------------------------------------------------------
void init_function_with_node(Function::Polynom3* function, const Node &node) {
    function->init(
        node.attribute2double("x3"),
        node.attribute2double("x2"),
        node.attribute2double("x1"),
        node.attribute2double("x0"),
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
                child.attribute2double("x"),
                child.attribute2double("y")
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
        node.attribute2double("lower_limit"), 
        node.attribute2double("upper_limit")
    );    
}
//------------------------------------------------------------------------------
}//Xml
