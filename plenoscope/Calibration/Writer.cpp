// Copyright 2014 Sebastian A. Mueller
#include "plenoscope/Calibration/Writer.h"
#include <sstream>

namespace plenoscope {
namespace Calibration {

Writer::Writer(const std::string npath): path(npath) {
    clear_file();
}

void Writer::append(const std::vector<CalibrationPhotonResult> &table) {
    file.open(path, std::ios::app | std::ios::binary);
    assert_file_is_open();
    for (CalibrationPhotonResult row : table)
        if (row.reached_sensor)
            write_row(row);
    file.close();
}

void Writer::write_row(const CalibrationPhotonResult &row) {
    // write ID integer
    file.write( (char*)&row.lixel_id, sizeof(uint32_t));
    // write pos dir and time floats
    const unsigned int float_count = 5;
    float v[float_count] = {
        row.x_pos_on_principal_aperture,
        row.y_pos_on_principal_aperture,
        row.x_tilt_vs_optical_axis,
        row.y_tilt_vs_optical_axis,
        row.relative_time_of_flight
    };
    file.write((char*)&v, float_count*sizeof(float));
}

void Writer::assert_file_is_open() {
    if (!file.is_open()) {
        std::stringstream info;
        info << __FILE__ << " " << __LINE__ << "\n";
        info << "Light Field Telescope Calibration: Unable to write to file: '";
        info << path << "'\n";
        throw std::runtime_error(info.str());
    }
}

void Writer::clear_file() {
    file.open(path, std::ios_base::trunc | std::ios::binary);
    assert_file_is_open();
    file.close();
}

}  // namespace Calibration
}  // namespace plenoscope
