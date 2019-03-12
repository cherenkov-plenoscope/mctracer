// Copyright 2014 Sebastian A. Mueller
#include "merlict_viewer/Time.h"
#include <sstream>
#include <iomanip>


namespace merlict {
namespace Time {

    Stamp::Stamp() {
        update_now();
    }

    void Stamp::update_now() {
        initialization_time = time(0);
        now = localtime(&initialization_time);
    }

    std::string Stamp::str()const {
        std::stringstream out;
        out << "time stamp: ";
        out << yyyy() << "." << mm() << "." << dd() << " ";
        out << HH() << ":" << MM() << ":" << SS();
        return out.str();
    }

    std::string Stamp::yyyy()const {
        return zero_padding(now->tm_year+1900, 4);
    }

    std::string Stamp::mm()const {
        return zero_padding(now->tm_mon+1, 2);
    }

    std::string Stamp::dd()const {
        return zero_padding(now->tm_mday, 2);
    }

    std::string Stamp::HH()const {
        return zero_padding(now->tm_hour, 2);
    }

    std::string Stamp::MM()const {
        return zero_padding(now->tm_min, 2);
    }

    std::string Stamp::SS()const {
        return zero_padding(now->tm_sec, 2);
    }

    std::string Stamp::zero_padding(
        const unsigned int value, const unsigned int width)const {
        std::stringstream out;
        out << std::setfill('0') << std::setw(width) << value;
        return out.str();
    }

}  // namespace Time
}  // namespace merlict
