#include "FunctionMap.h"
#include <sstream>
using std::stringstream;
//------------------------------------------------------------------------------
bool FunctionMap::has(const string key)const {
    return functions.find(key) != functions.end();
}
//------------------------------------------------------------------------------
Function::Func1D* FunctionMap::get(const string key)const {
    assert_has(key);
    return functions.find(key)->second;
}
//------------------------------------------------------------------------------
void FunctionMap::assert_has(const string key)const {
    if(!has(key)) {
        stringstream info;
        info << "FunctionMap " << __FILE__ << ", " << __LINE__ << "\n";
        info << "There is no function called '" << key << "'. \n";
        throw NoSuchKey(info.str());
    }   
}
//------------------------------------------------------------------------------
void FunctionMap::assert_not_in_use_yet(const string key) {
    if(has(key)) {
        stringstream info;
        info << "FunctionMap " << __FILE__ << ", " << __LINE__ << "\n";
        info << "The key '" << key << "' is already in use.\n";
        throw KeyAlreadyInUse(info.str());
    }
}
//------------------------------------------------------------------------------
FunctionMap::~FunctionMap() {
    for (std::pair<string, Function::Func1D*> function: functions)
        delete function.second; 
}
//------------------------------------------------------------------------------