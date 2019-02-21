// Copyright 2014 Sebastian A. Mueller
#ifndef TOOLS_TIME_H_
#define TOOLS_TIME_H_

#include <ctime>
#include <string>

namespace relleums {
namespace Time {

class Stamp {
    std::time_t initialization_time;
    struct std::tm *now;
 public:
    Stamp();
    void update_now();
    std::string str()const;
    std::string yyyy()const;
    std::string mm()const;
    std::string dd()const;
    std::string HH()const;
    std::string MM()const;
    std::string SS()const;
 private:
    std::string zero_padding(
        const unsigned int value,
        const unsigned int width)const;
};

}  // namespace Time
}  // namespace relleums

#endif  // TOOLS_TIME_H_
