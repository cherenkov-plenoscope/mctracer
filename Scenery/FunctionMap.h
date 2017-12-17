// Copyright 2014 Sebastian A. Mueller
#ifndef SCENERY_FUNCTIONMAP_H_
#define SCENERY_FUNCTIONMAP_H_

#include <string>
#include <utility>
#include <map>
#include "ResourceMap.h"
#include "Core/Function/Function.h"

struct FunctionMap: public ResourceMap {
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
    void assert_has(const std::string key)const;
    void assert_not_in_use_yet(const std::string key);
};
#endif  // SCENERY_FUNCTIONMAP_H_
