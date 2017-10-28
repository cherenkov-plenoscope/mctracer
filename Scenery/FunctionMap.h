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
#include "ResourceMap.h"
#include "Core/Function/Function.h"

class FunctionMap: public ResourceMap {
public:
    std::map<std::string, Function::Func1D*> functions;

    bool has(const std::string key)const;
    Function::Func1D* get(const std::string key)const;
    
    template<class ProtoFunction>
    ProtoFunction* add(const std::string key) {
        assert_not_in_use_yet(key);
        ProtoFunction* child = new ProtoFunction;
        functions.insert(std::pair<std::string, Function::Func1D*>(key, child)); 
        return child;        
    }

    ~FunctionMap();
private:

    void assert_has(const std::string key)const;
    void assert_not_in_use_yet(const std::string key);
};
#endif // __FUNCTIONMAP_H_INCLUDED__