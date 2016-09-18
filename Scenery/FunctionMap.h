//=================================
// include guard
#ifndef __FUNCTIONMAP_H_INCLUDED__
#define __FUNCTIONMAP_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <string>
#include <map>

using std::string;
using std::map;

#include "ResourceMap.h"
#include "Core/Function/Function.h"

class FunctionMap: public ResourceMap {
public:
    map<string, Function::Func1D*> functions;

    bool has(const string key)const;
    Function::Func1D* get(const string key)const;
    
    template<class ProtoFunction>
    ProtoFunction* add(const string key) {
        assert_not_in_use_yet(key);
        ProtoFunction* child = new ProtoFunction;
        functions.insert(std::pair<string, Function::Func1D*>(key, child)); 
        return child;        
    }

    ~FunctionMap();
private:

    void assert_has(const string key)const;
    void assert_not_in_use_yet(const string key);
};
#endif // __FUNCTIONMAP_H_INCLUDED__