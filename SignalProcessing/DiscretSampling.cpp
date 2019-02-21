// Copyright 2014 Sebastian A. Mueller
#include "SignalProcessing/DiscretSampling.h"
#include  <math.h>
#include <sstream>
#include <exception>

using namespace relleums;

namespace DiscretSampling {

Config::Config() {
    slice_width = 1.0;
    samples_per_slice = 10;
}

LookUpTable::LookUpTable(const Function::Func1D* func, const Config config) {
    this->config = config;
    this->func = func;

    if (config.slice_width < 0.0) {
        std::stringstream info;
        info << __FILE__ << ", " << __LINE__ << "\n";
        info << "Expected config->slice_width > 0.0, ";
        info << "but actual it is: " << config.slice_width;
        info << "\n";
        throw std::invalid_argument(info.str());
    }

    if (func->limits().range() == 0.0) {
        std::stringstream info;
        info << __FILE__ << ", " << __LINE__ << "\n";
        info << "Expected the function range to be > 0.0, ";
        info << "but actual it is: " << func->limits().range();
        info << "\n";
        throw std::invalid_argument(info.str());
    }

    number_of_slices = ceil(func->limits().range()/config.slice_width);
    step_width_in_slice = config.slice_width/
        static_cast<double>(config.samples_per_slice);

    fill_table();
}

void LookUpTable::fill_table() {
    try {
        for (unsigned int i = 0; i < config.samples_per_slice; i++) {
            std::vector<double> row;
            row.reserve(number_of_slices);
            const double x_off = step_width_in_slice*static_cast<double>(i);
            for (unsigned int j = 0; j < number_of_slices; j++) {
                const double x = config.slice_width*static_cast<double>(j) +
                    func->limits().lower();
                row.push_back(func->evaluate(x + x_off));
            }
            table.push_back(row);
        }
    } catch (std::bad_alloc& ba) {
        std::stringstream info;
        info << __FILE__ << ", " << __LINE__ << "\n"
        "std::bad_alloc was thrown during filling the "
        "DiscretSamplingLookUpTable:\n" << ba.what() << "\n"
        "Tried to allocate at least: \n"
        "8 byte * " << config.samples_per_slice << " samples/slice * "
        << number_of_slices << " slices = "
        << 8*config.samples_per_slice*number_of_slices << " byte.\n";
        throw std::runtime_error(info.str());
    }
}

const std::vector<double>* LookUpTable::at(const double slice_offset)const {
    if (slice_offset < 0.0 || slice_offset >= 1.0) {
        std::stringstream info;
        info << __FILE__ << ", " << __LINE__ << "\n";
        info << "Expected  0.0 <= slice_offset < 1.0, ";
        info << "but actual it is: " << slice_offset;
        info << "\n";
        throw std::out_of_range(info.str());
    }
    const unsigned int row_to_look_up = floor(
        slice_offset/static_cast<double>(config.samples_per_slice));
    return &table.at(row_to_look_up);
}

unsigned int LookUpTable::size()const {
    return number_of_slices;
}

}  // namespace DiscretSampling
