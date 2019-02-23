// Copyright 2014 Sebastian A. Mueller
#ifndef VISUAL_THREADPOOL_H_
#define VISUAL_THREADPOOL_H_

#include <stdint.h>
#include <future>
#include <thread>
#include <vector>

namespace relleums {
namespace visual {


template<class T, class C>
void parallel_for_each(std::vector<T>& ts, C callable) {
    std::vector<std::future<int>> results;

    for(auto const& value: ts) {
        results.emplace_back(std::async(callable, value));
    }
    for (auto& result : results) {
        result.wait();
    }
}



}  // namespace visual
}  // namespace relleums

#endif  // VISUAL_THREADPOOL_H_
